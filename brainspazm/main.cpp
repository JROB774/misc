/*******************************************************************************
MAIN
An interpreter used to run programs in the Brainfuck language. More
information on the language can be found at the Wikipedia page
<https://en.wikipedia.org/wiki/Brainfuck>. This interpreter simply loops
through a string containing the contents of the program code and
interprets each of the unique command characters until completion.

*******************************************************************************/

// c library includes
#include <cstdlib>
#include <cstdio>
#include <cstdint>
// c++ library includes
#include <string>
#include <iostream>
#include <fstream>
#include <stack>
#include <algorithm>

/**
  * Displays an error message to the console and terminates the program with
  * a failure state.
  *
  * @PARAM _error: The error message to display to the console.
**/
static inline void Error (const std::string _error)
{
	std::cerr << "ERROR: " << _error << "\n";
	std::getchar();
	std::exit(EXIT_FAILURE);
}

namespace Brainfuck
{
	namespace /* PRIVATE MEMBERS */
	{
		constexpr const std::size_t DATA_SIZE = 0x8000;

		std::string program;           // the program code itself
		std::size_t pc = 0;            // the program counter
		std::stack<std::size_t> stack; // the stack for handling loops

		int8_t data[DATA_SIZE];        // the program's data, pointed at by the pointer (32KB given)
		int8_t* pointer = nullptr;     // the program's pointer
	}

	/**
	  * Initialises the brainfuck program and sets everything up for
	  * interpretation to begin running.
	  *
	  * @PARAM _filename: The name of the program file to load.
	**/
	static void Init (const std::string _filename)
	{
		// loads the brainfuck program file and stores it in memory
		std::ifstream file(_filename, std::ios::in|std::ios::binary);

		if (file.is_open()) {
			// resizes the string to fit the whole file, faster than constant reallocation
			file.seekg(0, std::ios::end);
			program.resize(file.tellg());
			// resets the file cursor to the beginning and reads the whole file into memory
			file.seekg(0, std::ios::beg);
			file.read(&program[0], program.capacity());
			// closes the file after use
			file.close();
		}
		else { Error("Couldn't open program for interpretation!"); }

		// does a check that all loops are correctly formatted before attempting to run
		int loopcount = 0;
		for (auto it: program) {
			if (it == '[') { loopcount += 1; continue; }
			else if (it == ']') {
				loopcount -= 1;
				if (loopcount < 0) { break; }
			}
		}
		if (loopcount != 0) { Error("Unmatched loop bracket!"); }

		// makes sure all the program's data defaults to zero and sets the start position
		std::fill_n(data, DATA_SIZE, 0);
		pointer = &data[0];
	}

	/**
	  * Runs through the brainfuck program interpreting and handling
	  * each of the program tasks.
	**/
	static void Run ()
	{
		// runs as long as there is code to interpret
		while (pc < program.length()) {
			// interprets the command accordingly
			switch (program.at(pc)) {
				// increment the data pointer (to point to the next cell to the right)
				case ('>'): { if (pointer != &data[DATA_SIZE - 1]) { ++pointer; ++pc; } else { Error("Out of higher bound!"); } break; }
				// decrement the data pointer (to point to the next cell to the left)
				case ('<'): { if (pointer != &data[0]) { --pointer; ++pc; } else { Error("Out of lower bound!"); } break; }
				// increment the byte at the data pointer
				case ('+'): { ++*pointer; ++pc; break; }
				// decrement the byte at the data pointer
				case ('-'): { --*pointer; ++pc; break; }
				// output the byte at the data pointer
				case ('.'): { std::putchar(*pointer); ++pc; break; }
				// accept one byte of input, storing its value in the byte at the data pointer (the if avoids EOF affecting a cell)
				case (','): { char in = std::getchar(); if (in >= 0) { *pointer = in; } ++pc; break; }
				// if the byte at the data pointer is zero then jump to the matching ']' otherwise loop through the contents
				case ('['): {
					if (*pointer != 0) { stack.push(pc); }
					else {
						// skips any embedded loops inside the one that is being skipped
						int openbracket = 0;
						while (true) {
							++pc;
							if (program.at(pc) == '[') { ++openbracket; }
							else if (program.at(pc) == ']') {
								if (openbracket <= 0) { break; }
								else { --openbracket; }
							}
						}
					}
					++pc;
					break;
				}
				// jump back to the matching '[' to repeat or break from the loop
				case (']'): { pc = stack.top(); stack.pop(); break; }
				// iterate the program counter if unknown character
				default:    { ++pc; break; }
			}
		}
	}
}

int main (int _argc, char* _argv[])
{
	// makes sure a brainfuck program was passed in
	if (_argc < 2) { Error("No Brainfuck program passed!"); }

	// initialises and runs the program
	Brainfuck::Init(_argv[1]);
	Brainfuck::Run();

	// terminates the program successfully
	std::getchar();
	return EXIT_SUCCESS;
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
