// c library includes
#include <cstdint>
// c++ library includes
#include <string>
#include <iostream>
#include <fstream>

int main (int _argc, char* _argv[])
{
	// make sure a map has been passed to the application
	if (_argc <= 1) { std::cerr << "ERROR: No map file passed!\n"; return 1; }

	// iterate throuugh each map file passed
	for (int i = 1; i < _argc; ++i) {
		// open the passed file for reading
		std::fstream input(_argv[i], std::ios::in);
		if (!input.is_open()) { std::cerr << "ERROR: Map file couldn't be opened!\n"; return 1; }

		// create the new file name for the output file
		std::string filename = _argv[i];
		filename.replace(filename.find(".dat"), std::string::npos, ".map");
		// open a new file to write the packed map data
		std::fstream output(filename, std::ios::out|std::ios::binary|std::ios::trunc);
		if (!output.is_open()) { std::cerr << "ERROR: Cannot open output file!\n"; return 1; }

		// read in the map caption and dimensions
		std::string caption;
		std::string width, height;

		std::getline(input, caption);
		std::getline(input, width);
		std::getline(input, height);

		// write out the map cation and dimensions
		output.write(&caption[0], caption.)
		output << caption << "\n" << width << "\n" << height << "\n";

		// read in the map data line-by-line and write it out as binary data
		std::string line;
		while (std::getline(input, line)) {
			// go through number by number and convert
			std::string::size_type pos = 0;
			char byte;
			for (int j = 0; j < std::stoi(width); ++j) {
				byte = std::stoi(line.substr(pos), &pos, 16);
				output.write(&byte, sizeof(char));
			}
		}

		// close both file streams, no longer needed
		output.close();
		input.close();
	}

	return 0;
}