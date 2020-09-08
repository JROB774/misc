#include <cmath>

#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>

constexpr int   TILE_COLUMNS = 10, TILE_ROWS = 34;
constexpr int   TILE_COUNT = TILE_COLUMNS * TILE_ROWS;
constexpr float TILE_PADDING = 8.f;
constexpr float TILE_SIZE = 64.f + TILE_PADDING; // so it takes into account the double-padding

int main (int _argc, char* _argv[])
{
	// open the output file to begin writing data
	std::ofstream file("tiletable.cpp", std::ios::trunc);
	if (!file.is_open()) { std::cout << "Failed to create output file!" << std::endl; }
	else {
		// write the actual content to the file
		file << "struct __TilePos__ { float x, y; };\n";
		file << "constexpr __TilePos__ TILE_LOOKUP[] = {\n";
		for (int i = 0; i < TILE_COUNT; ++i) {
			if ((i % 5) == 0) { file << "\n"; }
			float x = std::floor(i % TILE_COLUMNS);
			float y = std::floor(i / TILE_COLUMNS);
			float xpos = (x * TILE_SIZE) + ((x+1) * TILE_PADDING);
			float ypos = (y * TILE_SIZE) + ((y+1) * TILE_PADDING);
			file << "{ " << std::setw(3) << xpos << ", " << std::setw(4) << ypos << " }, ";
		}
		file << "\n\n};";

		// close the file after use
		file.close();
	}

	return 0;
}