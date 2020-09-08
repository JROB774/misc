/*******************************************************************************
 * Algorithms for Highest Common Factor and Lowest Common Multiple.
 * The Unlicense (Public Domain) <http://unlicense.org/>
 * Authored By Joshua Robertson
 *
*******************************************************************************/

/*////////////////////////////////////////////////////////////////////////////*/

#include <stdlib.h>
#include <stdio.h>

int HCF (int _a, int _b)
{
    // Implemented using the Euclidean Algorithm for calculating HCF/GCD:
    // <https://en.wikipedia.org/wiki/Euclidean_algorithm>

    // Special case helps avoid division by zero.
    if (_a == 0) { return _b; }
    if (_b == 0) { return _a; }

    int max = (_a < _b) ? _b : _a;
    int min = (_a < _b) ? _a : _b;

    int quotient = 0;
    int remainder = 0;

    while (true) { // Potentially bad?
        quotient = max / min;
        if ((remainder = max % min) == 0) {
            break;
        }
        max = min;
        min = remainder;
    }

    return min;
}

int LCM (int _a, int _b)
{
    // Standard algorithm for calculating LCM if the HCF/GCD is known.

    // Special case to avoid division by zero.
    if (_a == 0 && _b == 0) { return 0; }
    return (_a * _b) / HCF(_a, _b);
}

int main (int _argc, char* _argv[])
{
    printf("HCF of (60,30) = %d\n", HCF(60,30));
    printf("LCM of (60,30) = %d\n", LCM(60,30));

    printf("HCF of (32,10) = %d\n", HCF(32,10));
    printf("LCM of (32,10) = %d\n", LCM(32,10));

    printf("HCF of (64,40) = %d\n", HCF(64,40));
    printf("LCM of (64,40) = %d\n", LCM(64,40));

    printf("HCF of ( 0, 0) = %d\n", HCF( 0, 0));
    printf("LCM of ( 0, 0) = %d\n", LCM( 0, 0));

    printf("HCF of (10, 0) = %d\n", HCF(10, 0));
    printf("LCM of (10, 0) = %d\n", LCM(10, 0));

    printf("HCF of ( 0,25) = %d\n", HCF( 0,25));
    printf("LCM of ( 0,25) = %d\n", LCM( 0,25));

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
