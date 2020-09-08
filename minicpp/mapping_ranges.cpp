/*******************************************************************************
 * A function for mapping a value within one range to a value in another range.
 * The Unlicense (Public Domain) <http://unlicense.org/>
 * Authored By Joshua Robertson
 *
*******************************************************************************/

/*////////////////////////////////////////////////////////////////////////////*/

#include <stdlib.h>
#include <stdio.h>

double MapRange (double _val, double _a1, double _a2, double _b1, double _b2)
{
    // Maps a value from the range (_a1,_a2) to a value in the range (_b1,_b2).
    return _b1 + ((_b2 - _b1) / (_a2 - _a1)) * (_val - _a1);
}

int main (int _argc, char* _argv[])
{
    printf("4 from (0,10) to (0,100) = %f\n",    MapRange(  4,  0, 10,  0,100));
    printf("127 from (0,255) to (0,360) = %f\n", MapRange(127,  0,255,  0,360));
    printf("2 from (0,4) to (5,9) = %f\n",       MapRange(  2,  0,  4,  5,  9));
    printf("20, from (0,10) to (0,20) = %f\n",   MapRange( 20,  0, 10,  0, 20));

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
