#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include <windows.h> // Using this for traversing directories.
#include <pcre/pcre2.h> // Using this for regex comparisons.

static constexpr const char HELP_TEXT[] =
"A utility for searching files for matching patterns.\n"
"\n"
"fsearch [OPTIONS] <PATTERN> <FILES/DIRS>\n"
"\n"
"OPTIONS:\n"
"/R Directories will be searched recursively for files.\n";

static size_t filesize;
static char* filedata;

static pcre2_match_data* match;
static pcre2_code* regex;
static PCRE2_SPTR pattern;
static PCRE2_SPTR subject;

static int errnum;
static PCRE2_SIZE erroff;

static bool switch_recursive_search; // Recursively searches directories.

static bool ReadEntireFile (const char* _filename, size_t* _filesize);

static bool IsDirectory (const char* _path);
static char** ListDirectoryContents (const char* _path);
static void FreeDirectoryList (char** _list);

static void PrintPCRE2Error ();

static void SearchDirectory (const char* _directory);
static void SearchFile (const char* _filename);

static int FindPatternInSubject ();

int main (int _argc, char* _argv[])
{
	// Check for various options and handle accordingly.
	int start_arg;
	for (start_arg=1; start_arg<_argc; ++start_arg) {
		if (_argv[start_arg][0] == '/' || _argv[start_arg][0] == '-') {
			switch (toupper(_argv[start_arg][1])) { // Case-insensitive comparison.
				case ('?'): { printf("%s", HELP_TEXT); return 0; } break;
				case ('R'): { switch_recursive_search = true; } break;
			}
		} else {
			break; // No more options.
		}
	}

	if (_argc-start_arg < 2) {
		printf("Requires a pattern and at least one file/dir!\n");
	} else {
		// After handling options we begin searching.
		pattern = (PCRE2_SPTR)_argv[start_arg];
		regex = pcre2_compile(pattern, PCRE2_ZERO_TERMINATED, 0, &errnum, &erroff, NULL);
		if (!regex) {
			PrintPCRE2Error();
		} else {
			match = pcre2_match_data_create_from_pattern(regex, NULL);
			// Goes through each file/directory searching for instances.
			for (int i=start_arg+1; i<_argc; ++i) {
				if (IsDirectory(_argv[i])) { SearchDirectory(_argv[i]); }
				else { SearchFile(_argv[i]); }
			}
			pcre2_match_data_free(match);
			pcre2_code_free(regex);
			free(filedata);
		}
	}

	return 0;
}

static bool ReadEntireFile (const char* _filename, size_t* _filesize)
{
	FILE* file = fopen(_filename, "rb");
	if (!file) { return false; }

	// Gets the size of the file.
	fseek(file, 0L, SEEK_END);
	size_t new_filesize = ftell(file);
	rewind(file);

	// If the given file data doesn't exist, or is too small, we allocate enough space.
	// We use this system because it saves on constant reallocations during runtime.
	if (!filedata || (*_filesize) < new_filesize) {
		if ((*_filesize) == 0) { (*_filesize) = new_filesize; }
		else { while ((*_filesize) < new_filesize) { (*_filesize) *= 2; } }
		char* temp = (char*)realloc(filedata, (*_filesize));
		if (temp) { filedata = temp; }
		else { return false; } // Failed to reallocate.
	}

	fread(filedata, sizeof(char), new_filesize, file);
	fclose(file);

	filedata[new_filesize] = '\0';

	return true;
}

static bool IsDirectory (const char* _path)
{
	DWORD attributes = GetFileAttributes(_path);
	if (attributes != INVALID_FILE_ATTRIBUTES) {
		return (attributes & FILE_ATTRIBUTE_DIRECTORY);
	}
	return false;
}

static char** ListDirectoryContents (const char* _path)
{
	//
	// @IMPROVE: still not safe at any of the failure points
	//

	// A list of the directory's content.
	char** list = NULL;
	int listlen = 0;

	// Do not attempt to list if the provided path is too long.
	if (strlen(_path) <= MAX_PATH) {
		// This is neccessary in Win32's find file functions.
		int pathlen = strlen(_path)+strlen("\\*")+1;
		char* path = (char*)malloc(pathlen*sizeof(char));
		if (path) {
			path[0] = '\0'; // So we concatenate from start.

			strcat(path, _path);
			strcat(path, "\\*");

			// Gets the first file/directory in the directory.
			WIN32_FIND_DATA find_data = {0};
			HANDLE findfile = FindFirstFile(path, &find_data);
			// Of the handle is invalid there is no content.
			if (findfile != INVALID_HANDLE_VALUE) {
				// Go through and list any remaining content.
				do {
					// Prevents listing itself and parent directory in content.
					const char* filename = find_data.cFileName;
					if ((strcmp(filename, ".")!=0) && (strcmp(filename, "..")!=0)) {
						// Allocates space for the item in the list.
						listlen++;
						// @IMPROVE: THIS ISN'T A SAFE WAY TO USE REALLOC!!!
						char** temp_list = (char**)realloc(list, listlen*sizeof(char*));
						if (!temp_list) { break; } // failed
						else {
							// Add the item to the new list entry.
							list = temp_list;
							int itemlen = strlen(_path)+strlen("\\")+strlen(filename)+1;
							int index = listlen-1;
							list[index] = (char*)malloc(itemlen*sizeof(char));
							if (!list[index]) { break; } // Failed.
							else {
								list[index][0] = '\0'; // So we concatenate from start.

								strcat(list[index], _path);
								strcat(list[index], "\\");
								strcat(list[index], filename);
							}

						}
					}
				}
				while (FindNextFile(findfile, &find_data));

				// Adds a null-terminator to mark the end of the list.
				char** temp_list = (char**)realloc(list, (listlen+1)*sizeof(char*));
				if (temp_list) {
					list = temp_list;
					list[listlen] = NULL;
				}
			}
			// End the find file operation.
			FindClose(findfile);
		}
	}

	// Returns the list of content.
	return list;
}

static void FreeDirectoryList (char** _list)
{
	for (char** item=_list; *item; ++item) {
		free(*item);
	}
	free(_list);
}

static void PrintPCRE2Error ()
{
	PCRE2_UCHAR buffer[512];

	pcre2_get_error_message(errnum, buffer, sizeof(buffer));
	printf("PCRE2 error at offset %d: %s!\n", erroff, buffer);
}

static void SearchDirectory (const char* _directory)
{
	// Go through each item in the list and handle it.
	char** list = ListDirectoryContents(_directory);
	if (list) {
		for (char** item=list; *item; ++item) {
			if (IsDirectory(*item)) {
				// If we're searching recursively then enter sub-directories.
				if (switch_recursive_search) { SearchDirectory(*item); }
			} else {
				// If it's a file then search as normal.
				SearchFile(*item);
			}
		}
		FreeDirectoryList(list);
	}
}

static void SearchFile (const char* _filename)
{
	if (!ReadEntireFile(_filename, &filesize)) {
		printf("Couldn't read file` '%s'!\n", _filename);
	} else {
		subject = (PCRE2_SPTR)filedata;
		int total = FindPatternInSubject();
		if (total > 0) { // We don't want to spam the command-line.
			printf("Found %d instances of '%s' in file '%s'.\n",
				total, pattern, _filename);
		}
	}
}

static int FindPatternInSubject ()
{
	int total = 0;
	size_t subject_len = strlen((char*)subject);
	// Perform a match check between the pattern and the file contents.
	int offset = 0;
	while (pcre2_match(regex, subject, subject_len, offset, 0, match, NULL) >= 0) {
		offset = (int)pcre2_get_ovector_pointer(match)[1];
		total++;
	}
	return total;
}

/******************************************************************************\
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
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
\******************************************************************************/
