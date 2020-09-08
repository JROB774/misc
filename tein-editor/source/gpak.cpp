namespace GPAK
{
	namespace
	{
		struct Entry
		{
			std::uint16_t namelength;
			std::string name;
			std::uint32_t size;
		};

		constexpr const char* FILE_EXTENSION = ".gpak";
		constexpr std::size_t APPROX_NUMENTRIES = 1320;

		std::uint32_t numentries;
		std::vector<Entry> entries;

		inline bool operator< (const Entry& _lhs, const Entry& _rhs)
		{
			return _lhs.size < _rhs.size;
		}

		void SearchDir (std::string _path)
		{
			// gets all files and directories in the current path
			std::vector<std::string> files = Platform::ListDirectoryContents(_path);

			// looks through all the contents of the current path
			std::vector<std::string> dirs;
			for (auto& file: files) {
				// if its a directory then cache it for later searching
				if (Platform::IsDirectory(file)) { dirs.push_back(file); }
				// otherwise fill out a new file entry
				else {
					// create a reference, easier to read + access
					entries.push_back(Entry());
					auto& entry = entries.back();
					// adds the correct data to the entry for later packing
					entry.name = file;
					std::replace(entry.name.begin(), entry.name.end(), '\\', '/'); // back-slashes will not work
					entry.size = Platform::GetFileSizeInBytes(file);
					// iterates number of entries for packing
					++numentries;
				}
			}

			// now go through the sub-directories in alphabetical order
			if (!dirs.empty()) { for (auto& dir: dirs) { SearchDir(dir); } }
		}
	}

	void Unpack (std::string _path)
	{
		// we need to reset stuff or there is potential for crashes
		numentries = 0;
		entries.clear();

		// opens the given file for binary reading (and unpacking)
		std::ifstream infile(_path, std::ios::binary);
		if (!infile.is_open()) {
			ERROR_LOG(Error::PRIORITY_LOW, Error::TYPE_STD, "Failed to open GPAK '" + _path + "'!");
			return;
		}

		// read the total number of file entries (first 4 bytes)
		infile.read((char*)&numentries, sizeof(numentries));
		entries.resize(numentries); // saves multiple memory allocations

		// begin reading file entries until there are no more
		for (auto& entry: entries) {
			// reads the different data members in to program memory (flip if on big endian)
			infile.read((char*)&entry.namelength, sizeof(entry.namelength));
			entry.namelength = SDL_SwapLE16(entry.namelength); // byte-swap
			entry.name.resize(entry.namelength);
			infile.read((char*)&entry.name[0], entry.namelength);
			infile.read((char*)&entry.size, sizeof(entry.size));
			entry.size = SDL_SwapLE32(entry.size); // byte-swap
		}

		// creates the base directory for unpacking files to
		std::string basedir = _path.substr(0, _path.length() - std::strlen(FILE_EXTENSION)) + "\\";
		// if it already exists ask the user if they want to replace it before continuing (be polite :D)
		if (Platform::FileOrDirectoryExists(basedir)) {
			if (Platform::MessageBox(
					Platform::MSGBOX_TYPE_YESNO,
					"GPAK Unpack",
					"'" + basedir + "' already exists!\nWould you like to overwrite/continue?"
				) == Platform::MSGBOX_RESULT_NO) {
				return;
			}
		}

		// make the base directory where all files will be saved (use name of input file)
		if (!Platform::CreateNewDirectory(basedir)) {
			ERROR_LOG(Error::PRIORITY_LOW, Error::TYPE_STD, "Failed to create directory '" + basedir + "'!");
			return;
		}

		// allocate a buffer that is large enough to store the biggest file
		std::vector<std::uint8_t> buffer;
		std::size_t maxsize = std::max_element(entries.begin(), entries.end())->size;
		buffer.resize(maxsize);

		// entries are in order, so now the files can simply be extracted one-by-one
		for (auto& entry: entries) {
			// read the data for the current file entry into the buffer
			infile.read((char*)&buffer[0], entry.size);
			// determine if the file's specific directory needs to be made or not
			std::string dir = basedir + entry.name.substr(0, entry.name.find_last_of("/\\"));
			if (!Platform::FileOrDirectoryExists(dir)) {
				if (!Platform::CreateNewDirectory(dir)) {
					ERROR_LOG(Error::PRIORITY_LOW, Error::TYPE_STD, "Failed to create directory '" + dir + "'!");
					return; // do not continue, causes too many issues
				}
			}
			// write the data from the buffer to an actual file
			std::string filename = basedir + entry.name;
			std::ofstream outfile(filename, std::ios::trunc|std::ios::binary);
			// ask the user if they want to continue even if this file couldn't be unpacked
			if (!outfile.is_open()) {
				if (Platform::MessageBox(
						Platform::MSGBOX_TYPE_YESNO,
						"GPAK Unpack",
						"Failed to unpack file '" + filename + "'!\nDo you want to continue?"
					) == Platform::MSGBOX_RESULT_NO) {
					return;
				}
			}
			outfile.write((char*)&buffer[0], entry.size);
			outfile.close();
		}

		// closes the file when no longer needed
		infile.close();
	}

	void Pack (std::string _path)
	{
		// we need to reset stuff or there is potential for crashes
		numentries = 0;
		entries.clear();

		// removes trailing slash(es) from the path (causes issues later otherwise)
		while (_path.find_last_of("/\\") == _path.length() - 1) { _path.pop_back(); }

		// checks to see if the path given exists and is actually a directory (not a file)
		if (!Platform::FileOrDirectoryExists(_path) || !Platform::IsDirectory(_path)) {
			ERROR_LOG(Error::PRIORITY_LOW, Error::TYPE_STD, "The path '" + _path + "' is not a directory!");
			return;
		}

		// begins searching for all the files present in the directory (including sub-dirs)
		entries.reserve(APPROX_NUMENTRIES); // cuts down memory allocations
		SearchDir(_path);

		// removes any unneccessary path prefix info from file names (+ saves name lengths)
		for (auto& entry: entries) {
			entry.name = entry.name.substr(_path.length() + 1);
			entry.namelength = entry.name.length();
		}

		// generates/creates the name for the output pack file
		std::string outname = _path + FILE_EXTENSION;
		// if the file already exists, ask the user if they want to overwrite (be polite :D)
		if (Platform::FileOrDirectoryExists(outname)) {
			if (Platform::MessageBox(
					Platform::MSGBOX_TYPE_YESNO,
					"GPAK Pack",
					"'" + outname + "' already exists!\nWould you like to overwrite/continue?"
				) == Platform::MSGBOX_RESULT_NO) {
				return;
			}
		}
		// opens the output file (the .gpak) where all data will be packed
		std::ofstream outfile(outname, std::ios::trunc|std::ios::binary);
		if (!outfile.is_open()) {
			ERROR_LOG(Error::PRIORITY_LOW, Error::TYPE_STD, "Failed to create GPAK '" + outname + "'!");
			return;
		}

		// writes the total number of file entries to the file
		outfile.write((char*)&numentries, sizeof(numentries));

		// writes the directory list / look-up list for all file entries
		for (auto& entry: entries) {
			// writes the name length, name, and file size (in that order) (flip if on big endian)
			entry.namelength = SDL_SwapLE16(entry.namelength); // byte-swap
			outfile.write((char*)&entry.namelength, sizeof(entry.namelength));
			outfile.write((char*)&entry.name[0], entry.namelength);
			entry.size = SDL_SwapLE32(entry.size); // byte-swap
			outfile.write((char*)&entry.size, sizeof(entry.size));
		}

		// allocate a buffer that is large enough to store the biggest file
		std::vector<std::uint8_t> buffer;
		std::size_t maxsize = std::max_element(entries.begin(), entries.end())->size;
		buffer.resize(maxsize);

		// writes each file's raw data to the pack (in same order as the dir list!)
		for (auto& entry: entries) {
			// opens the file for reading into the pack
			std::string filename = _path + "\\" + entry.name;
			std::ifstream infile(filename, std::ios::binary);
			// ask the user if they want to continue even if this file couldn't be packed
			if (!outfile.is_open()) {
				if (Platform::MessageBox(
						Platform::MSGBOX_TYPE_YESNO,
						"GPAK Unpack",
						"Failed to pack file '" + filename + "'!\nDo you want to continue?"
					) == Platform::MSGBOX_RESULT_NO) {
					return;
				}
			}
			// copies its entire contents into the pack
			infile.read((char*)&buffer[0], entry.size);
			outfile.write((char*)&buffer[0], entry.size);
			// closes the file after use
			infile.close();
		}

		// closes the file after use
		outfile.close();
	}
}