namespace DataManager
{
	namespace
	{
		struct FileEntry
		{
			std::string name;
			std::uint32_t offset;
			std::uint32_t size;
		};

		std::vector<FileEntry> entries;
	}

	void Init ()
	{
		// dont bother doing this in debug because we use files on disk in that case
		#ifdef RELEASE

		// reads the file entry list in the embedded PAK data file and stores
		// all the entry metadata for easy access later in the program

		// determines how many files are in the PAK
		std::uint32_t filecount = *(reinterpret_cast<const std::uint32_t*>(DATA));
		filecount = SDL_SwapLE32(filecount); // swap from lil endian if on big endian

		// reserves that many file entries and begins parsing the data
		entries.reserve(filecount);
		std::size_t cursor = sizeof(std::uint32_t); // just after the filecount value
		for (std::uint32_t i = 0; i < filecount; ++i) {
			// create a new entry and cache it for easy access
			entries.push_back(FileEntry{});
			auto& entry = entries.back();
			// get the name of the file entry
			while (DATA[cursor] != '\0') { entry.name += DATA[cursor++]; }
			// get the offset/pos and size
			entry.offset = *(reinterpret_cast<const std::uint32_t*>(DATA+(++cursor)));
			entry.size = *(reinterpret_cast<const std::uint32_t*>(DATA+(cursor+=sizeof(std::uint32_t))));
			// swap from lil endian if on big endian
			entry.offset = SDL_SwapLE32(entry.offset);
			entry.size = SDL_SwapLE32(entry.size);
			// setup for the next file
			cursor+=sizeof(std::uint32_t);
		}

		#endif // RELEASE
	}

	#ifdef RELEASE
	SDL_RWops* Load (std::string _filename)
	{
		// finds the specified file in the packed data
		FileEntry* file = nullptr;
		for (auto& entry: entries) { if (entry.name == _filename) { file = &entry; break; } }

		// returns the raw file data to be handlled
		if (!file) { return nullptr; }
		else { return SDL_RWFromMem((void*)&DATA[file->offset], file->size); }
	}
	#else
	SDL_RWops* Load (std::string _filename)
	{
		// loads the file directly whilst in debug mode, no need to constantly pack data
		std::string filepath = "data\\" + _filename;
		return SDL_RWFromFile(filepath.c_str(), "r");
	}
	#endif // RELEASE
}