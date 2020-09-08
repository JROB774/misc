/*******************************************************************************
 * Testing a real-time game asset hotloading system for future use.
 * The Unlicense (Public Domain) <http://unlicense.org/>
 * Authored By Joshua Robertson
 *
*******************************************************************************/

/*////////////////////////////////////////////////////////////////////////////*/

// Hotloading is when a game's asset files are setup so that any changes made to
// them whilst the game is running will be updated and reloaded in real-time.
//
// This is very useful and allows for much faster asset iteration and creation
// with the ability to see the updates and changes in real-time within the actual
// game rather than having to reload the whole game to see how the assets look.

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL/SDL.h>
#include <windows.h>

typedef uint8_t   u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t    s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;

#define WIN_TITLE "Hotloading Test"
#define WIN_W     640
#define WIN_H     480
#define WIN_BPP    32

#define HOTLOAD_DIR "data\\"
#define BUFFER_SIZE 2048

static SDL_Surface* window;
static SDL_Event event;
static bool running;

static HANDLE stop_hotloader_event;

static SDL_mutex* asset_lock;
static char asset_to_update[MAX_PATH];

int Hotloader (void* _data)
{
    // gets a handle to the directory containing files to hotload
    HANDLE hotload_dir = CreateFile(HOTLOAD_DIR, FILE_LIST_DIRECTORY,
        FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE, NULL, OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OVERLAPPED, NULL);
    if (hotload_dir == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "ERROR: Failed to get handle to hotload directory!\n");
        return 0;
    }

    OVERLAPPED polling_overlap;
    polling_overlap.OffsetHigh = 0;
    polling_overlap.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    // the events we will be looking out for (stop thread event or file updated)
    HANDLE events[2] = { polling_overlap.hEvent, stop_hotloader_event };

    FILE_NOTIFY_INFORMATION* notify;
    u8 info_buffer[BUFFER_SIZE];
    char filename[MAX_PATH];
    DWORD bytes_returned;
    BOOL result = TRUE;
    int offset = 0;

    // keep running as long as the call to ReadDirectoryChangesW works
    while (result) {
        result = ReadDirectoryChangesW(hotload_dir, &info_buffer, sizeof(info_buffer),
            TRUE /* monitors sub-dirs */, FILE_NOTIFY_CHANGE_FILE_NAME|FILE_NOTIFY_CHANGE_SIZE|
            FILE_NOTIFY_CHANGE_DIR_NAME, &bytes_returned, &polling_overlap, NULL);

        // wait until a file in the directory tree is modified
        DWORD event_fired = WaitForMultipleObjects(2, events, FALSE, INFINITE);

        // if the directory/file change event occurred...
        if (event_fired == WAIT_OBJECT_0) {
            // handle all entries in the file notify information
            offset = 0;
            do {
                // interpret the buffer as notification information
                notify = (FILE_NOTIFY_INFORMATION*)((char*)info_buffer + offset);

                // get the name of the file that was modified
                memset(filename, 0, sizeof(filename));
                WideCharToMultiByte(CP_ACP, 0, notify->FileName, notify->FileNameLength/2,
                    filename, sizeof(filename), NULL, NULL);
                filename[notify->FileNameLength/2] = '\0';

                // handle the modified file event -- the only one we want
                switch (notify->Action) {
                    case (FILE_ACTION_MODIFIED): {
                        SDL_LockMutex(asset_lock);
                        strcpy(asset_to_update, filename);
                        printf("asset_to_update = %s\n", asset_to_update);
                        SDL_UnlockMutex(asset_lock);
                    } break;
                }

                // move on to the next entry...
                offset += notify->NextEntryOffset;

            } while (notify->NextEntryOffset);
        }
        // if the stop hotloader event occurred...
        else if (event_fired == WAIT_OBJECT_0+1) {
            break; // leave the loop -- we're done!
        }
    }

    CloseHandle(hotload_dir);
    return 0;
}

int main (int _argc, char* _argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    freopen("CON", "w", stdout); // redirects stdout
    freopen("CON", "w", stderr); // redirects stderr

    window = SDL_SetVideoMode(WIN_W, WIN_H, WIN_BPP, SDL_HWSURFACE);
    if (!window) { fprintf(stderr, "ERROR: Failed to create SDL window!\n"); }
    SDL_WM_SetCaption(WIN_TITLE, NULL);

    SDL_Surface* image_a = SDL_LoadBMP("data\\images\\image_a.bmp");
    if (!image_a) { fprintf(stderr, "ERROR: Failed to load BMP image_a!\n"); }
    SDL_Rect rect_a = {   0, 0, 320, 480 };
    SDL_Surface* image_b = SDL_LoadBMP("data\\images\\image_b.bmp");
    if (!image_b) { fprintf(stderr, "ERROR: Failed to load BMP image_b!\n"); }
    SDL_Rect rect_b = { 320, 0, 320, 480 };

    // this event is used to stop the thread when neccessary
    stop_hotloader_event = CreateEvent(NULL, TRUE, FALSE, NULL);

    asset_lock = SDL_CreateMutex();
    if (!asset_lock) { fprintf(stderr, "ERROR: Failed to create asset mutex!\n"); }

    SDL_Thread* hotloader_thread = SDL_CreateThread(Hotloader, NULL);
    if (!hotloader_thread) { fprintf(stderr, "ERROR: Failed to create hotloader thread!\n"); }

    running = true;
    while (running) {
        // if there is an asset to update then do so
        SDL_LockMutex(asset_lock);
        if (asset_to_update[0] != '\0') {
            if (strcmp(asset_to_update, "images\\image_a.bmp") == 0) {
                SDL_FreeSurface(image_a);
                image_a = SDL_LoadBMP("data\\images\\image_a.bmp");
            } else if (strcmp(asset_to_update, "images\\image_b.bmp") == 0) {
                SDL_FreeSurface(image_b);
                image_b = SDL_LoadBMP("data\\images\\image_b.bmp");
            }

            // reset this value so it won't compare true again
            memset(asset_to_update, 0, sizeof(asset_to_update));
        }
        SDL_UnlockMutex(asset_lock);

        while (SDL_PollEvent(&event)) {
            // ensure that running is locked whilst we write to it
            if (event.type == SDL_QUIT) {
                running = false;
                SetEvent(stop_hotloader_event);
            }
        }

        SDL_BlitSurface(image_a, NULL, window, &rect_a);
        SDL_BlitSurface(image_b, NULL, window, &rect_b);
        SDL_Flip(window);
    }

    SDL_WaitThread(hotloader_thread, NULL);
    SDL_DestroyMutex(asset_lock);

    SDL_FreeSurface(image_a);
    SDL_FreeSurface(image_b);

    SDL_Quit();

    return 0;
}

/*////////////////////////////////////////////////////////////////////////////*/

/*******************************************************************************
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 *
*******************************************************************************/
