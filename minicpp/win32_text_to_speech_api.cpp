/*******************************************************************************
 * Testing the basic usage of the Windows text-to-speech API.
 * The Unlicense (Public Domain) <http://unlicense.org/>
 * Authored By Joshua Robertson
 *
*******************************************************************************/

/*////////////////////////////////////////////////////////////////////////////*/

#include <stdio.h>
#include <wchar.h>

#include <windows.h>
#include <sapi.h>
#include <objbase.h>

int main (int _argc, char* _argv[])
{
    // Initialize COM, as it is needed for SAPI (Speech API).
    if (FAILED(::CoInitialize(NULL))) {
        fprintf(stderr, "ERROR: CoInitialize failed!\n");
        return 0;
    }

    // Setup the speech voice object for generating speech.
    ISpVoice* voice = NULL;
    HRESULT res = CoCreateInstance(CLSID_SpVoice, NULL,
        CLSCTX_ALL, IID_ISpVoice, (void**)&voice);
    if (FAILED(res)) {
        fprintf(stderr, "CoCreateInstance failed!\n");
        return 0;
    }

    // Get text from the user and repeat it back to them.
    // The Speech API needs wide characters when taking input.
    wchar_t input[1024] = {};
    fprintf(stdout, "Pleae enter some text:\n");
    while (wcscmp(input, L"exit\n") != 0) { // Because \n isn't stripped.
        printf("> ");
        fgetws(input, 1024, stdin);
        voice->Speak(input, SPF_IS_XML, NULL);
    }

    // Release any objects and cleanup after use.
    voice->Release();
    ::CoUninitialize();

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
