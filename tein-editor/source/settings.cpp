namespace Settings
{
	namespace
	{
		std::string data;

		inline std::string GetValue (std::string _name)
		{
			// determine the line that the variable is on
			std::size_t linestart = data.find(_name);
			// determine the start and end points of the variable value
			std::size_t start = data.find('=', linestart) + 1;
			std::size_t end = data.find_first_of('\n', linestart);

			// if either of these points couldn't be found then just return a blank string
			if (start == std::string::npos) { return std::string(); }
			else { return data.substr(start, end - start); }
		}
	}

	void Init ()
	{
		// open the settings file for reading into memory
		std::ifstream file("settings.dat");
		if (!file.is_open()) {
			// if we fail to open the settings file we simply create a new one
			ERROR_LOG(Error::PRIORITY_LOW, Error::TYPE_STD, "Failed to open settings.dat, creating new default!");
			// opens the settings file, to copy over its data
			bool failed = false;
			SDL_RWops* stream = DataManager::Load("settings.dat");
			if (!stream) { failed = true; }
			else {
				std::string tempdata;
				tempdata.resize(SDL_RWsize(stream));
				if (SDL_RWread(stream, (void*)&tempdata[0], SDL_RWsize(stream), 1) == 0) { failed = true; }
				else {
					std::ofstream newfile("settings.dat", std::ios::trunc);
					if (!newfile.is_open()) { failed = true; }
					else {
						// copies the contents to a new settings file
						newfile << tempdata;
						newfile.close();
						// and loads this new file, hopefully not failing this time
						file.open("settings.dat");
						if (!file.is_open()) { failed = true; }
					}
				}
				// cleans up the stream after use as it's no longer needed
				SDL_RWclose(stream);
				stream = nullptr;
			}
			// if we failed, inform the user and crash fatally
			if (failed) { ERROR_LOG(Error::PRIORITY_MAX, Error::TYPE_SDL, "Failed to create new settings file!"); }
		}

		// reads the settings file and stores the raw data for later access
		std::stringstream stream;
		stream << file.rdbuf();
		data = stream.str();

		file.close();
	}

	int GetInteger (std::string _name)
	{
		// if there was no value, we assume a zero value
		std::string value = GetValue(_name);
		if (value.empty()) { return 0; }

		// extract the integer from the string value
		try { return std::stoi(value); }
		catch (...) { return 0; }
	}

	std::string GetString (std::string _name)
	{
		// if there was no value return an empty string
		std::string value = GetValue(_name);
		if (value.empty()) { return std::string(); }

		// important that we trim leading and trailing whitespace on string
		std::size_t start = value.find_first_not_of(" \t\n\v\f\r");
		std::size_t end = value.find_last_not_of(" \t\n\v\f\r");

		// if either of these points couldn't be found then just return a blank string
		if (start == std::string::npos || end == std::string::npos) { return std::string(); }
		else { return value.substr(start, end); }
	}

	bool GetBoolean (std::string _name)
	{
		// if there was no value, we assume a false value
		std::string value = GetValue(_name);
		if (value.empty()) { return false; }

		// important that we trim leading and trailing whitespace on string (for the comparison)
		std::size_t start = value.find_first_not_of(" \t\n\v\f\r");
		std::size_t end = value.find_last_not_of(" \t\n\v\f\r");

		// if either of these points couldn't be found then we assume a false value
		if (start == std::string::npos || end == std::string::npos) { return false; }
		else {
			value = value.substr(start, end);
			return (value == "true");
		}
	}

	SDL_Color GetColor (std::string _name)
	{
		// if there was no value, we assume white
		std::string value = GetValue(_name);
		if (value.empty()) { return SDL_Color { 0xFF, 0xFF, 0xFF, 0xFF }; }

		// converts the different color components into an actual color
		try {
			auto tokens = Utility::StrToken(value, ",");
			return SDL_Color {
				(std::uint8_t)std::stoi(tokens.at(0)),
				(std::uint8_t)std::stoi(tokens.at(1)),
				(std::uint8_t)std::stoi(tokens.at(2)),
				(std::uint8_t)std::stoi(tokens.at(3))
			};			
		}
		catch (...) { return SDL_Color { 0xFF, 0xFF, 0xFF, 0xFF }; }
	}
}