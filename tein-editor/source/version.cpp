namespace Version
{
	constexpr int MAJOR = 0;
	constexpr int MINOR = 1;
	constexpr int PATCH = 5;

	inline std::string GetString ()
	{
		std::stringstream stream;
		stream << MAJOR << "." << MINOR << "." << PATCH;
		return stream.str();
	}

	inline int GetNumber ()
	{
		return ((MAJOR * 1000) + (MINOR * 100) + PATCH);
	}
}