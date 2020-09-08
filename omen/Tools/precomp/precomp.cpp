// c library includes
#include <cmath>
// c++ library includes
#include <iostream>
#include <iomanip>
#include <fstream>

int main ()
{
	// open the output file for writing (replaces previous contents)
	std::fstream output("precomp.lua", std::ios::out|std::ios::trunc);
	if (!output.is_open()) { std::cerr << "ERROR: Couldn't open file for writing!\n"; return 1; }

	// write out the header section of the lua file
	output <<
	"--[[\n"
	"Precomp script contains the precomputed cos and sin functions to determine\n"
	"the distance for a given ray, this is much faster than doing it at runtime.\n"
	"These values are placed within a lookup table and the player's current\n"
	"angle is used to determine which value should be used.\n\n"
	"THIS FILE WAS AUTO-GENERATED USING A CUSTOM APPLICATION CALLED \"PRECOMP\"\n"
	"MADE FOR THE DEVELOPMENT OF THIS GAME\n"
	"--]]\n\n"
	"local precomp = { cos = {}, sin = {} }\n\n";

	// set the desired precision for accuracy
	output << std::setprecision(54);

	// calculate and write the table entries for the lua file
	for (double i = 0.0; i <= 360.0; ++i) {
		output << "precomp.cos[" << i << "] = " << std::cos(i) << "; ";
		output << "precomp.sin[" << i << "] = " << std::sin(i) << "\n";
	}

	// write out the footer section and close the stream
	output << "\nreturn precomp";
	output.close();

	return 0;
}