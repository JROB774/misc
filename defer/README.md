# Defer

Allows for code to be deferred to the end of scope for later execution.

## Installation

Include `defer.hpp` into your project (C++11 is required).

## Usage

```c++
void my_function ()
{
    FILE* file = fopen("myfile.txt", "r");
    defer { fclose(file); };
    // Do stuff with file...
    // File closed on exit...
}
```

## Revision History

```
v1.0.3  [24-08-2020]  Fixed a compile-time naming bug.
v1.0.2  [31-03-2020]  Changed some of the source code formatting.
v1.0.1  [26-11-2019]  Added support for __COUNTER__ define.
v1.0.0  [24-11-2019]  Initial release.
```

## License

Available under Public Domain or MIT License.
