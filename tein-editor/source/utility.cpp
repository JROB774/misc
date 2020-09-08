namespace Utility
{
	inline std::string StrUpper (std::string _str)
	{
		std::transform(_str.begin(), _str.end(), _str.begin(), ::toupper);
		return _str;
	}

	inline std::string StrLower (std::string _str)
	{
		std::transform(_str.begin(), _str.end(), _str.begin(), ::tolower);
		return _str;
	}

	inline std::vector<std::string> StrToken (const std::string& _str, std::string _delim)
	{
		std::vector<std::string> tokens;
		std::string::size_type start = 0, end = 0;

		while (end != std::string::npos) {
			end = _str.find_first_of(_delim, start);
			std::string token = _str.substr(start, end-start);
			if (token.length() > 0) { tokens.push_back(token); }
			start = end + 1;
		}

		return tokens;
	}
	inline std::vector<std::string> StrToken (const std::string& _str)
	{
		return StrToken(_str, " \t\n\v\f\r");
	}

	inline std::string StrSection (const std::string& _str, std::string _begindelim, std::string _enddelim)
	{
		std::string::size_type start = _str.find_last_of("\\/");
		std::string::size_type count = _str.find_last_of(".");

		if (start == std::string::npos) { start = 0; }
		if (start != 0) { ++start; }
		if (count == std::string::npos) { count = _str.length(); }

		count -= start;

		return _str.substr(start, count);
	}

	template<typename T>
	inline constexpr T Wrap (const T& _value, const T& _lower, const T& _upper)
	{
		if (_upper < _value) { return _value - _upper; }
		else if (_value < _lower) { return _value + _upper; }
		return _value;
	}

	template<typename T>
	inline constexpr int Sign (T _value, std::false_type _signed)
	{
		return T(0) < _value;
	}
	template<typename T>
	inline constexpr int Sign (T _value, std::true_type _signed)
	{
		return (T(0) < _value) - (_value < T(0));
	}
	template<typename T>
	inline constexpr int Sign (T _value)
	{
		return Sign(_value, std::is_signed<T>());
	}
}