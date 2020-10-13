#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

// using this for traversing directories
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static const char HELP_TEXT[] =
"A utility for printing the line count for C/C++ files.\n"
"\n"
"linecount [OPTIONS] <FILES/DIRS>\n"
"\n"
"OPTIONS:\n"
"/D            Print detailed line count info for each file supplied.\n"
"/R            Directories will be searched recursively for files.\n"
"/E:<FILE/DIR> Excludes the specified file/directory from the line count.\n";

#define EXCLUDE_START_ALLOCATED 32

static bool switch_print_detailed; // display line count for each file
static bool switch_recursive_search; // recursively searches directories'

static const char** exclude_list; // files and dirs to exclude
static int exclude_count; // number of files and dirs in the list
static int exclude_allocated; // number of slots allocated

static size_t filename_padding; // how much padding to add to filenames to align them

static int total_code_count;
static int total_comment_count;
static int total_blank_count;
static int total_line_count;

static bool IsDirectory (const char* _path);
static char** ListDirectoryContents (const char* _path);
static void FreeDirectoryList (char** _list);

static char* RemoveLeadingSpace (char* _str);
static size_t FindFirstInString (const char* _str, const char _c);
static bool LineIsCode (char* _line, bool* _in_block_comment);

static void ExcludeItem (const char* _path);
static bool ItemIsExcluded (const char* _path);

static void ParseFile (const char* _filename);
static void ParseDirectory (const char* _directory);

static void CheckFilename (const char* _filename);
static void CheckDirectoryFilenames (const char* _directory);

int main (int _argc, char* _argv[])
{
	// setup the exclusion list for use in the program
	exclude_allocated = EXCLUDE_START_ALLOCATED;
	exclude_list = malloc(exclude_allocated * sizeof(char*));
	if (exclude_list) {
		memset(exclude_list, 0, exclude_allocated * sizeof(char*));
	} else {
		printf("Failed to exclude files/dirs!\n");
	}

	// check for various options and handle accordingly
	int start_arg;
	for (start_arg=1; start_arg<_argc; ++start_arg) {
		if (_argv[start_arg][0] == '/' || _argv[start_arg][0] == '-') {
			switch (toupper(_argv[start_arg][1])) { // case-insensitive comparison
				case ('?'): { printf("%s", HELP_TEXT); return 0; } break;
				case ('D'): { switch_print_detailed = true; } break;
				case ('R'): { switch_recursive_search = true; } break;
				case ('E'): { ExcludeItem(_argv[start_arg]); } break;
			}
		} else {
			break; // no more options
		}
	}

	if (_argc-start_arg < 1) {
		printf("Requires at least one file/dir!\n");
	} else {
		// find out which filename is longest and use that as the padding length for the file names
		filename_padding = 5; // the length of the word "TOTAL"
		if (switch_print_detailed) {
			for (int i=start_arg; i<_argc; ++i) {
				if (!ItemIsExcluded(_argv[i])) {
					if (IsDirectory(_argv[i])) {
						if (switch_recursive_search) {
							CheckDirectoryFilenames(_argv[i]);
						}
					} else {
						CheckFilename(_argv[i]);
					}
				}
			}
		}

		printf("| %-*s | %10s | %10s | %10s | %10s |\n", filename_padding, "FILE", "CODE", "COMMENTS", "BLANK", "TOTAL");

		for (int i=start_arg; i<_argc; ++i) {
			// only handle non-excluded files/directories
			if (!ItemIsExcluded(_argv[i])) {
				if (IsDirectory(_argv[i])) {
					if (switch_recursive_search) {
						ParseDirectory(_argv[i]);
					}
				} else {
					ParseFile(_argv[i]);
				}
			}
		}

		// print out the information for all the files that were parsed
		printf("| %-*s | %10d | %10d | %10d | %10d |\n", filename_padding, "TOTAL", total_code_count, total_comment_count, total_blank_count, total_line_count);
	}

	// cleanup the exclude list after use
	if (exclude_list) {
		free(exclude_list);
	}

	return 0;
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
	// a list of the directory's content
	char** list = NULL;
	int listlen = 0;

	// do not attempt to list if the provided path is too long
	if (strlen(_path) <= MAX_PATH) {
		// this is neccessary in Win32's find file functions
		int pathlen = strlen(_path)+strlen("\\*")+1;
		char* path = malloc(pathlen*sizeof(char));
		if (path) {
			path[0] = '\0'; // so we strcat from start

			strcat(path, _path);
			strcat(path, "\\*");

			// gets the first file/dir in the directory
			WIN32_FIND_DATA filedata = {0};
			HANDLE findfile = FindFirstFile(path, &filedata);
			// if the handle is invalid there is no content
			if (findfile != INVALID_HANDLE_VALUE) {
				// go through and list any remaining content
				do {
					// prevents listing self and parent dir in content
					const char* filename = filedata.cFileName;
					if ((strcmp(filename, ".")!=0) && (strcmp(filename, "..")!=0)) {
						// allocates space for the item in the list
						listlen++;
						char** temp_list = realloc(list, listlen*sizeof(char*));
						if (!temp_list) { break; } // failed
						else {
							// add the item to the new list entry
							list = temp_list;
							int itemlen = strlen(_path)+strlen("\\")+strlen(filename)+1;
							int index = listlen-1;
							list[index] = malloc(itemlen*sizeof(char));
							if (!list[index]) { break; } // failed
							else {
								list[index][0] = '\0'; // so we strcat from start

								strcat(list[index], _path);
								strcat(list[index], "\\");
								strcat(list[index], filename);
							}

						}
					}
				}
				while (FindNextFile(findfile, &filedata));

				// adds a null to mark the end of the list
				char** temp_list = realloc(list, (listlen+1)*sizeof(char*));
				if (temp_list) {
					list = temp_list;
					list[listlen] = NULL;
				}
			}
			// end the find file operation
			FindClose(findfile);
		}
	}

	// returns the list of content
	return list;
}

static void FreeDirectoryList (char** _list)
{
	for (char** item=_list; *item; ++item) {
		free(*item);
	}
	free(_list);
}

static char* RemoveLeadingSpace (char* _str)
{
	while (isspace((unsigned char)*_str)) {
		++_str;
	}
	return _str;
}

static size_t FindFirstInString (const char* _str, const char _c)
{
	size_t index = -1;
	for (size_t i=0; i<strlen(_str); ++i) {
		if (_str[i] == _c) {
			index = i;
			break;
		}
	}
	return index;
}

static bool LineIsCode (char* _line, bool* _in_block_comment)
{
	int line_len = strlen(_line);
	bool line_is_code = false;

	for (int i=0; i<line_len; ++i) {
		// determine block comment state
		bool state_change = false;
		if (_line[i] == '/' && _line[i+1] == '*') {
			if (!(*_in_block_comment)) {
				*_in_block_comment = true;
				state_change = true;
				i+=2;
			}
		} else if (_line[i] == '*' && _line[i+1] == '/') {
			if (*_in_block_comment) {
				*_in_block_comment = false;
				state_change = true;
				i+=2;
			}
		}

		// if outside of the line bounds then leave
		if (i >= line_len) { break; }

		// if there is a non white-space character outside of a block comment
		// at any point in the line, then there is code present and the line
		// is deemed a code line instead of a comment / block-comment line
		if (!(*_in_block_comment) && !isspace(_line[i])) { line_is_code = true; }

		// if the state was changed move back one afterwards to avoid stupid bugs
		if (state_change) { --i; }
	}

	return line_is_code;
}

static void ExcludeItem (const char* _path)
{
	// if we need more space in the exclusion list then make it
	if (exclude_count == exclude_allocated) {
		exclude_allocated *= 2;
		const char** temp_list = realloc(exclude_list, exclude_allocated * sizeof(char*));
		if (temp_list) {
			exclude_list = temp_list;
		} else {
			return;
		}
	}
	// add files/directories to exclude
	const char* item_to_exclude = _path + (FindFirstInString(_path, ':')+1);
	exclude_list[exclude_count++] = item_to_exclude;
}

static bool ItemIsExcluded (const char* _path)
{
	bool excluded = false;
	if (exclude_list) {
		for (int i=0; i<exclude_count; ++i) {
			if (strcmp(_path, exclude_list[i]) == 0) {
				excluded = true;
				break;
			}
		}
	}
	return excluded;
}

static void ParseFile (const char* _filename)
{
	// don't handle excluded files
	if (ItemIsExcluded(_filename)) { return; }

	int code_count = 0;
	int comment_count = 0;
	int blank_count = 0;
	int line_count = 0;

	bool in_block_comment = false;

	FILE* file = fopen(_filename, "r");
	if (!file) { printf("Couldn't open '%s'!\n", _filename); }
	else {
		char raw_line[4096]; // should be more than enough for any line
		while (fgets(raw_line, sizeof(raw_line), file)) {
			// strip leading space from the line for easier parsing
			char* line = RemoveLeadingSpace(raw_line);
			int line_len = strlen(line);

			if (in_block_comment) {
				// if in a block comment scan the line for closing statement
				int block_comment_exit_pos = 0;
				for (int j=0; j<line_len-1; ++j) {
					if (line[j] == '*' && line[j+1] == '/') {
						in_block_comment = false;
						block_comment_exit_pos = j+2;
						break;
					}
				}
				// if still in the block comment then add a line
				if (in_block_comment) { comment_count++; }
				else {
					// otherwise parse the line normally
					if (block_comment_exit_pos < line_len) {
						char* sub_line = line+block_comment_exit_pos;
						if (LineIsCode(sub_line, &in_block_comment)) {
							++code_count;
							continue;
						}
					}
					comment_count++;
				}
			} else {
				// check what type of line the current line is
				if (line[0] == '\0') { ++blank_count; }
				else if (line[0] == '/' && line[1] == '/') { ++comment_count; }
				else {
					if (LineIsCode(line, &in_block_comment)) { ++code_count; }
					else { ++comment_count; }
				}
			}
		}

		line_count = code_count + comment_count + blank_count;

		// add these numbers to the total for all files
		total_code_count += code_count;
		total_comment_count += comment_count;
		total_blank_count += blank_count;
		total_line_count += line_count;

		// print out the information for the file if detailed is switched on
		if (switch_print_detailed) {
			printf("| %-*s | %10d | %10d | %10d | %10d |\n", filename_padding, _filename, code_count, comment_count, blank_count, line_count);
		}

		fclose(file);
	}
}

static void ParseDirectory (const char* _directory)
{
	// go through each item in the list and handle it
	char** list = ListDirectoryContents(_directory);
	if (list) {
		for (char** item=list; *item; ++item) {
			// don't handle excluded directories/files
			if (!ItemIsExcluded(*item)) {
				if (IsDirectory(*item)) {
					if (switch_recursive_search) { // if we're searching recursively then enter sub-dirs
						ParseDirectory(*item);
					}
				} else { // if its a file parse as normal
					ParseFile(*item);
				}
			}
		}
		// clean up all memory -- avoid leaks
		FreeDirectoryList(list);
	}
}

static void CheckFilename (const char* _filename)
{
	size_t length = strlen(_filename);
	if (length > filename_padding) {
		filename_padding = length;
	}
}

static void CheckDirectoryFilenames (const char* _directory)
{
	char** list = ListDirectoryContents(_directory);
	for (char** item=list; *item; ++item) {
		if (!ItemIsExcluded(*item)) {
			if (IsDirectory(*item)) {
				if (switch_recursive_search) {
					CheckDirectoryFilenames(*item);
				}
			} else {
				CheckFilename(*item);
			}
		}
	}
	FreeDirectoryList(list);
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
