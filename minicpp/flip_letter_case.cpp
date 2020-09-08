/*******************************************************************************
 * Testing a custom implementation of a letter case-flipping function.
 * The Unlicense (Public Domain) <http://unlicense.org/>
 * Authored By Joshua Robertson
 *
*******************************************************************************/

/*////////////////////////////////////////////////////////////////////////////*/

#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <string>

int main (int _argc, char* _argv[])
{
    // Note that no checks are being carried so any non-letter characters would
    // also have the operation applied -- converting them into other characters.

    // Constant expression values.
    constexpr char CHARACTERS[] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz" };
    constexpr size_t STR_LENGTH = 0x9D72DF;
    constexpr char CASE_BIT = 0x20;

    // Generate a random fixed-length string of characters.
    std::string str(STR_LENGTH, ' ');
    for (char& c: str) { c = CHARACTERS[rand() % (sizeof(CHARACTERS) - 1)]; }

    // Start timing the program.
    clock_t begin = clock();
    for (char& c: str) { c ^= CASE_BIT; }

    // Ouput the elapsed time to flip all characters.
    clock_t end = clock();
    double elapsed = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Elapsed Time: %f\n", elapsed);

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
