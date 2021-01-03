/*******************************************************************************
 * Command-line utility for packing and unpacking the GPAK data format.
 * Available Under MIT License (See EOF)
 * Released 20-08-2019
 * Version 1.0.0
 * Authored by Joshua Robertson
 *
*******************************************************************************/

/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

#if defined(_MSC_VER)
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#endif

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cctype>

#include <filesystem>
#include <algorithm>

#include <vector>
#include <string>

namespace fs = std::filesystem;

typedef  uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;

/* -------------------------------------------------------------------------- */

// Inline implementation of a defer macro from: github.com/jrob774/defer

#ifdef __COUNTER__
#define defer \
const auto& DEFER_CONCAT2(defer, __COUNTER__) = defer_::Defer_Help() + [&]()
#else
#define defer \
const auto& DEFER_CONCAT2(defer,    __LINE__) = defer_::Defer_Help() + [&]()
#endif

#define DEFER_CONCAT2(a, b) DEFER_CONCAT1(a, b)
#define DEFER_CONCAT1(a, b) a##b

namespace defer_
{

template<typename T>
struct Defer
{
    T lambda;

             Defer   (T block): lambda(block) { /* Nothing! */ }
            ~Defer   ()                       {    lambda();   }

             Defer   (const Defer& d) = delete;
    Defer& operator= (const Defer& d) = delete;
};
struct Defer_Help
{
    template<typename T>
    Defer<T> operator+ (T type) { return type; }
};

} // defer_

/* -------------------------------------------------------------------------- */

enum GPAK_Result { GPAK_ERR_UNPACK, GPAK_ERR_PACK, GPAK_ERR_EMPTY, GPAK_SUCCESS };

static bool switch_print_details   = false;
static bool switch_force_overwrite = false;

static constexpr int VER_MAJOR = 1;
static constexpr int VER_MINOR = 0;
static constexpr int VER_PATCH = 0;

static constexpr const char* HELP_TEXT =
"A utility for packing and unpacking the GPAK data file format.\n"
"\n"
"GPAKUnpacker [OPTIONS] <FILES/DIRS>\n"
"\n"
"OPTIONS:\n"
"  /d : Print information regarding packed/unpacked files.\n"
"  /f : Force overwrite files when packing/unpacking.\n";

static inline bool should_overwrite (const std::string& file_name)
{
    if (switch_force_overwrite) return true;
    char input = 0;
    printf("'%s' already exists. Overwrite? (Yes/No/All) [y/n/a]\n", file_name.c_str());
    while (scanf(" %c", &input) == 1 && (input != 'y' && input != 'n' && input != 'a'))
    {
        printf("Invalid action!\n");
    }
    if (input == 'a') switch_force_overwrite = true;
    return (input == 'y' || input == 'a');
}

static inline std::string strip_file_name (std::string file_name)
{
    std::string::size_type last_slash = file_name.rfind('/');
    std::string path;
    if (last_slash != std::string::npos)
    {
        path = file_name.substr(0, last_slash+1);
    }
    return path;
}

static inline bool does_file_exist (const std::string& path_name)
{
    return (fs::is_regular_file(path_name) && fs::exists(path_name));
}

static inline GPAK_Result gpak_unpack (const char* file_name)
{
    printf("Unpacking GPAK '%s'\n", file_name);

    // Define a local struct so we can store GPAK entries in a vector easily.
    struct GPAK_Entry { u16 name_length; std::string name; u32 file_size; };

    FILE* file = fopen(file_name, "rb");
    if (!file) return GPAK_ERR_UNPACK;
    defer { fclose(file); };

    std::vector<GPAK_Entry> entries;
    u32 entry_count;

    fread(&entry_count, sizeof(u32), 1, file);
    entries.resize(entry_count);

    for (auto& e: entries)
    {
        fread(&e.name_length, sizeof(u16),  1,             file);
        e.name.resize(e.name_length);
        fread(&e.name[0],     sizeof(char), e.name_length, file);
        fread(&e.file_size,   sizeof(u32),  1,             file);
    }

    size_t max_size = std::max_element(entries.begin(), entries.end(),
    [](const GPAK_Entry& _a, const GPAK_Entry& _b) { return (_a.file_size < _b.file_size); })->file_size;

    std::vector<u8> file_buffer;
    file_buffer.resize(max_size);

    std::string base_path(strip_file_name(file_name));
    for (auto& e: entries)
    {
        fread(&file_buffer[0], sizeof(u8), e.file_size, file);

        std::string filename(base_path + e.name);
        std::string filepath = strip_file_name(filename);

        if (!fs::exists(filepath))
        {
            fs::create_directory(filepath);
        }

        if (!does_file_exist(filename) || should_overwrite(filename))
        {
            FILE* output = fopen(filename.c_str(), "wb");
            if (output)
            {
                fwrite(&file_buffer[0], sizeof(u8), e.file_size, output);
                fclose(output);
                if (switch_print_details)
                {
                    printf("Unpacked file '%s'\n", filename.c_str());
                }
            }
            else
            {
                fprintf(stderr, "[Error] Failed to unpack '%s'\n", filename.c_str());
            }
        }
    }

    return GPAK_SUCCESS;
}

static inline GPAK_Result gpak_pack (const char* path_name)
{
    printf("Packing GPAK '%s'\n", path_name);

    std::string pathname(path_name);
    while (pathname.find_last_of("/\\") == pathname.length()-1) pathname.pop_back();
    std::string filename(pathname + ".gpak");

    if (does_file_exist(filename) && !should_overwrite(filename)) return GPAK_SUCCESS;

    FILE* file = fopen(filename.c_str(), "wb");
    if (!file) return GPAK_ERR_PACK;
    defer { fclose(file); };

    std::vector<std::string> files;
    for (auto& f: fs::recursive_directory_iterator(path_name))
    {
        if (f.is_regular_file())
        {
            files.push_back(f.path().string());
            std::replace(files.back().begin(), files.back().end(), '\\', '/');
        }
    }
    if (files.empty())
    {
        return GPAK_ERR_EMPTY;
    }

    u32 entry_count = static_cast<u32>(files.size());
    fwrite(&entry_count, sizeof(u32), 1, file);

    size_t max_size = 0;
    for (auto& f: files)
    {
        std::string name = f.substr(pathname.length() + 1);
        u16 name_length  = static_cast<u16>(name.length());
        u32 file_size    = static_cast<u32>(fs::file_size(f));

        max_size = std::max<size_t>(max_size, static_cast<size_t>(file_size));

        fwrite(&name_length, sizeof(u16),  1,           file);
        fwrite(name.c_str(), sizeof(char), name_length, file);
        fwrite(&file_size,   sizeof(u32),  1,           file);
    }

    std::vector<u8> file_buffer;
    file_buffer.resize(max_size);

    for (auto& f: files)
    {
        FILE* input = fopen(f.c_str(), "rb");
        if (input)
        {
            size_t file_size = static_cast<size_t>(fs::file_size(f));
            fread (&file_buffer[0], sizeof(u8), file_size, input);
            fwrite(&file_buffer[0], sizeof(u8), file_size, file);
            fclose(input);
            if (switch_print_details)
            {
                printf("Packed file '%s'\n", f.c_str());
            }
        }
        else
        {
            fprintf(stderr, "[Error] Failed to pack '%s'\n", f.c_str());
        }
    }

    return GPAK_SUCCESS;
}

int main (int argc, char** argv)
{
    int start_arg = 1;
    for (; start_arg<argc; ++start_arg)
    {
        char* arg = argv[start_arg];
        if (arg[0] == '/' || arg[0] == '-')
        {
            switch (tolower(arg[1]))
            {
                case ('?'): printf("%s", HELP_TEXT); return 0; break;
                case ('d'): switch_print_details       = true; break;
                case ('f'): switch_force_overwrite     = true; break;
                case ('-'):
                {
                    std::string cmd(arg+1);
                    std::transform(cmd.begin(), cmd.end(), cmd.begin(), [](char _c){ return (char)tolower(_c); });
                    if (cmd == "-version")
                    {
                        printf("GPAK Unpacker %d.%d.%d\n", VER_MAJOR, VER_MINOR, VER_PATCH);
                        return 0;
                    }
                    else if (cmd == "-help")
                    {
                        printf("%s\n", HELP_TEXT);
                        return 0;
                    }
                    else
                    {
                        fprintf(stderr, "Unknown Option: %s\n", arg);
                        return 1;
                    }
                } break;
                default:
                {
                    fprintf(stderr, "Unknown Option: %s\n", arg);
                    return 1;
                } break;
            }
        }
        else
        {
            break; // No more options/switches!
        }
    }

    if ((argc - start_arg) < 1)
    {
        printf("Requires at least one GPAK/directory to unpack/pack!\n");
        return 0;
    }

    for (; start_arg<argc; ++start_arg)
    {
        const char* arg = argv[start_arg];
        switch ((fs::is_regular_file(arg)) ? gpak_unpack(arg) : gpak_pack(arg))
        {
            case (GPAK_ERR_UNPACK): fprintf(stderr, "[Error] Failed to unpack GPAK '%s'!\n", arg); break;
            case (GPAK_ERR_PACK  ): fprintf(stderr, "[Error] Failed to pack GPAK '%s'!\n"  , arg); break;
            case (GPAK_ERR_EMPTY ): fprintf(stderr, "[Error] No files found at '%s'!\n"    , arg); break;
        }
    }

    return 0;
}

/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/

/*////////////////////////////////////////////////////////////////////////////*/
/*////////////////////////////////////////////////////////////////////////////*/
/*////////////////////////////////////////////////////////////////////////////*/

/*******************************************************************************
 *
 * REVISION HISTORY:
 *
 * v1.0.0  [20-08-2019]  Initial release.
 *
*******************************************************************************/

/*******************************************************************************
 * MIT License
 *
 * Copyright (c) 2019 Joshua Robertson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
*******************************************************************************/
