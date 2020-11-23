# TGA Loader

A simple image library that allows for TGA files to be loaded, either from disk
or memory, into an array of pixels.

TGA file types that are currently supported by the library:

|   Image Type   | Uncompressed | RLE Compressed |
| :------------: | :----------: | :------------: |
| **Color Map**  | &#10060;     | &#10060;       |
| **Greyscale**  | &#10060;     | &#10060;       |
| **True Color** | &#10004;     | &#10004;       |

## Installation

Define `TGA_LOADER_IMPLEMENTATION` in *one* file before including the library.

An example of including the library:

```c
#define TGA_LOADER_IMPLEMENTATION
#include "tga_loader.h"
```

## Usage

```c
int w, h, bpp;
uint8_t* pixels = tga_load_from_file("myfile.tga", &w,&h,&bpp);
// Do stuff with the pixel array...
tga_free(pixels);

```

## Revision History

```
v1.0.1  [11-11-2020]  Added define TGA_NO_CRT for disabling inclusion of CRT.
                      Added macros for replacing CRT function calls.
                      Now defining our structs with the typedef keyword.
                      Removed the use of false keyword in TGA_ASSERT calls.
v1.0.0  [24-08-2020]  Initial release.
```

## License

Available under Public Domain or MIT License.
