// format for the .pak file is as follows (lil endian):
// - 4 bytes: number of files
// - N bytes: 1st filename
// - 4 bytes: 1st file location offset from start
// - 4 bytes: 1st file size
// - ...
// - N bytes: the actual file content

#include <cstdlib>
#include <cstdint>

#include <ios>
#include <iomanip>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

/* the two potential byteorders */
#define LIL_ENDIAN 1234
#define BIG_ENDIAN 4321

/* stores this particular system's byteorder */
#ifndef BYTEORDER
#if defined(__linux__)
#include <endian.h>
#define BYTEORDER __BYTE_ORDER
#else
#if defined(__hppa__) || defined(__m68k__) || defined(mc68000) || defined(_M_M68K) || \
    (defined(__MIPS__) && defined(__MISPER__)) || defined(__ppc__) || defined(__POWERPC__) || \
    defined(_M_PPC) || defined(__sparc__)
#define BYTEORDER BIG_ENDIAN
#else
#define BYTEORDER LIL_ENDIAN
#endif
#endif
#endif

#if defined(__GNUC__) && defined(__i386__)
inline std::uint32_t Swap32 (std::uint32_t _x)
{
	__asm__("bswap %0": "=r"(_x): "0"(_x));
	return _x;
}
#elif defined(__GNUC__) && defined(__x86_64__)
inline std::uint32_t Swap32 (std::uint32_t _x)
{
	__asm__("bswapl %0": "=r"(_x): "0"(_x));
	return _x;
}
#elif defined(__GNUC__) && (defined(__powerpc__) || defined(__ppc))
inline std::uint32_t Swap32 (std::uint32_t _x)
{
	std::uint32_t result;
	__asm__("rlwimi %0,%2,24,16,23": "=&r"(result): "0"(_x >> 24), "r"(_x));
	__asm__("rlwimi %0,%2,8,8,15": "=&r"(result): "0"(result), "r"(_x));
	__asm__("rlwimi %0,%2,24,0,7": "=&r"(result): "0"(result), "r"(_x));
	return result;
}
#elif defined(__GNUC__) && (defined(__M68000__) || defined(__M68020__)) && !defined(__mcoldfire__)
inline std::uint32_t Swap32 (std::uint32_t _x)
{
	__asm__("rorw #8,%0\n\tswap %0\n\trorw #8,%0": "=d"(_x): "0"(_x): "cc");
	return _x;
}
#else
inline std::uint32_t Swap32 (std::uint32_t _x)
{
	return static_cast<std::uint32_t>((_x << 24)|((_x << 8) & 0x00FF0000)|((_x >> 8) & 0x0000FF00)|(_x >> 24));
}
#endif

#if (BYTEORDER == LIL_ENDIAN)
#define LSwap32(_x) _x
#define BSwap32(_x) Swap32(_x)
#else
#define LSwap32(_x) Swap32(_x)
#define BSwap32(_x) _x
#endif

std::ifstream::pos_type FileSize(std::string _filename)
{
    std::ifstream in(_filename, std::ios::ate|std::ios::binary);
    return in.tellg(); 
}

struct Entry
{
	std::string name;
	std::uint32_t offset;
	std::vector<std::uint8_t> data;
};

int main (int _argc, char* _argv[])
{
	// ensure that input is supplied
	if (_argc < 2) {
		std::cout << "No input given!" << std::endl;
		return 1;
	}

	// create entries for all files to be packed
	std::vector<Entry> entries;
	for (int i = 1; i < _argc; ++i) {
		// add the new entry
		entries.push_back(Entry{});
		auto& entry = entries.back();
		// fill out the metadata for the entry
		std::string name = _argv[i];
		std::size_t slash = name.find_last_of("/\\");
		if (slash == std::string::npos) { slash = 0; }
		if (slash != 0) { ++slash; }
		entry.name = name.substr(slash); // trim directory portion
		entry.offset = 0;
		for (int j = 0; j < entries.size()-1; ++j) {
			entry.offset += entries.at(j).data.size();
		}
		// get the actual file data
		std::ifstream file(name, std::ios::binary);
		if (!file.is_open()) { std::cout << "Failed to open file! (" << name << ")" << std::endl; }
		else {
			std::size_t filesize = FileSize(name);
			entry.data.resize(filesize);
			file.read((char*)&entry.data[0], filesize);
			file.close();
		}
	}

	// all offsets now have to have the size of the file header
	// directory added in order to be accurate
	std::uint32_t dirsize = sizeof(std::uint32_t); // num files
	for (auto& entry: entries) { dirsize += (entry.name.length()+1) + (sizeof(std::uint32_t)*2); }
	for (auto& entry: entries) { entry.offset += dirsize; }

	// create a .pak file for packing data
	std::ofstream pak("bin\\data.pak", std::ios::trunc|std::ios::binary);
	if (!pak.is_open()) { std::cout << "Failed to create .pak!" << std::endl; }
	else {
		// store the number of files in the pak
		std::uint32_t numfiles = _argc-1;
		numfiles = LSwap32(numfiles);
		pak.write((char*)&numfiles, sizeof(numfiles));

		// store the directory list (name|offset|size name|offset|size ...)
		for (auto& entry: entries) {
			pak.write(&entry.name[0], entry.name.length()+1);
			std::uint32_t offset = LSwap32(entry.offset);
			pak.write((char*)&offset, sizeof(offset));
			std::uint32_t size = LSwap32(entry.data.size());
			pak.write((char*)&size, sizeof(size));
		}

		// store the actual file data for each file
		for (auto& entry: entries) {
			pak.write((char*)&entry.data[0], entry.data.size());
		}

		// the .pak is now complete
		pak.close();
	}

	// opens the .pak so it can be saved into the data.cpp file
	std::ifstream infile("bin\\data.pak", std::ios::binary);
	if (!infile.is_open()) { std::cout << "Failed to open the PAK!" << std::endl; }
	else {
		// gets the size of the file so it knows how much to read into a buffer
		std::vector<std::uint8_t> buffer;
		std::size_t filesize = FileSize("bin\\data.pak");
		buffer.resize(filesize);
		// reads all of the file into the buffer, unaltered
		infile.read((char*)&buffer[0], filesize);
		// closes the file after use
		infile.close();

		// open the output file to write the data to in binary
		std::ofstream outfile("source\\data.cpp", std::ios::trunc);
		if (!outfile.is_open()) { std::cout << "Failed to pack the data!" << std::endl; }
		else {
			// write the contents of the file, wrapped in c++ code
			outfile << "#ifdef RELEASE\n";
			outfile << "\nconstexpr std::uint8_t DATA[] = {\n\n";
			std::size_t counter = 0;
			for (auto& byte: buffer) {
				if ((counter != 0) && ((counter % 16) == 0)) { outfile << "\n"; }
				outfile << std::uppercase << "0x" << std::setfill('0') << std::setw(2) << std::hex << (int)byte;
				if (++counter < buffer.size()) { outfile << ","; }
			}
			outfile << "\n\n};\n\n";
			outfile << "#endif // RELEASE";
			// closes the file after use
			outfile.close();
		}
	}

	return 0;
}