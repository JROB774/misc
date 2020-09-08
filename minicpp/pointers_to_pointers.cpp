/*******************************************************************************
 * Learning about and testing how pointers to other pointers work in C/C++.
 * The Unlicense (Public Domain) <http://unlicense.org/>
 * Authored By Joshua Robertson
 *
*******************************************************************************/

/*////////////////////////////////////////////////////////////////////////////*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main (int _argc, char* _argv[])
{
    // this would be the actual asset that gets handled by user
    char* asset_data = malloc(256 * sizeof(char));
    strcpy(asset_data, "This is my string!");

    // this is the generic pointer to an asset for hotloading
    void* generic_data = &asset_data;

    // the generic asset is converted based on the type of asset
    char* new_data = *(char**)generic_data;
    // this allows for modification when reloading the asset
    strcpy(new_data, "The string should change!");

    // resulting in both versions being changed accordingly:
    printf("\nasset_data is: %s", asset_data);
    printf("\nnew_data is: %s", new_data);
    // note how generic_data has not been invalidated
    printf("\ngeneric_data is: %s\n\n", *(char**)generic_data);

    // even if freed the pointer is still valid for use
    free(asset_data);
    asset_data = malloc(256 * sizeof(char));
    strcpy(asset_data, "A brand new string!");

    // see how all three still point to the same data correctly
    printf("\nasset_data is: %s", asset_data);
    printf("\nnew_data is: %s", new_data);
    printf("\ngeneric_data is: %s\n\n", *(char**)generic_data);

    // we don't want to have a leak now!
    free(asset_data);

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

