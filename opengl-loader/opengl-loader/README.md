# OpenGL Loader

A simple OpenGL function and extension loader library.

## Installation

Define `OPENGL_IMPLEMENTATION` in *one* file before including the library.

An example of including the library:

```c
#define OPENGL_IMPLEMENTATION
#include "opengl.h"
```

## Usage

```c
#define OPENGL_LOAD_GL_VERSION_MAJOR 3
#define OPENGL_LOAD_GL_VERSION_MINOR 3

// Loads the core profile for OpenGL version 3.3 using the default loader.
if (!opengl_load_procs(opengl_default_proc_loader))
{
    printf("Failed to load OpenGL functions!");
    return 1;
}
```

## Revision History

```
v1.0.4  [11-08-2020]  Replaced bools with ints.
v1.0.3  [11-08-2020]  Converted the code to be compatible with C.
v1.0.2  [05-06-2020]  Added a needed cast to opengl_has_gl_extension.
v1.0.1  [28-05-2020]  Added version number defines.
v1.0.0  [28-05-2020]  Initial release.
```

## License

Available under Public Domain or MIT License.
