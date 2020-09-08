/*******************************************************************************
 * Lists all of the function exports within a specified library.
 * The Unlicense (Public Domain) <http://unlicense.org/>
 * Authored By Joshua Robertson
 *
*******************************************************************************/

/*////////////////////////////////////////////////////////////////////////////*/

#include <stdlib.h>
#include <stdio.h>

#include <windows.h>

int main (int _argc, char* _argv[])
{
    if (_argc < 2) { printf("Program needs a library to scan!\n"); return 0; }

    HMODULE library = LoadLibraryEx(_argv[1], NULL, DONT_RESOLVE_DLL_REFERENCES);
    if (!library) { printf("Failed to load library '%s'!\n", _argv[1]); return 0; }

    PIMAGE_NT_HEADERS header = (PIMAGE_NT_HEADERS)((BYTE*)library + ((PIMAGE_DOS_HEADER)library)->e_lfanew);
    PIMAGE_EXPORT_DIRECTORY exports = (PIMAGE_EXPORT_DIRECTORY)((BYTE*)library + header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
    BYTE** names = (BYTE**)((int)library + exports->AddressOfNames);

    printf("Retrieving library functions...\n");
    for (int i=0; i<exports->NumberOfNames; ++i) {
        printf("%s\n", (BYTE*)library + (int)names[i]);
    }
    printf("Library functions retrieved!\n");

    FreeLibrary(library);
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
