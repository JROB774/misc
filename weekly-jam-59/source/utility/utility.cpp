/*////////////////////////////////////////////////////////////////////////////*/

namespace TCE
{

INLDEF bool operator== (const Point& _lhs, const Point& _rhs)
{
	return (_lhs.x == _rhs.x && _lhs.y == _rhs.y);
}
INLDEF bool operator!= (const Point& _lhs, const Point& _rhs)
{
	return !(_lhs == _rhs);
}

INLDEF float RandomFloat (float _min, float _max)
{
	if (_min > _max) { Swap<float>(_min, _max); }

	float random_value = CAST(float, rand()) / CAST(float, RAND_MAX);
	float difference = _max - _min;

	return _min + (random_value * difference);
}

template<typename T> INLDEF void Swap (T& _a, T& _b)
{
	T temp = _a;

	_a = _b;
	_b = temp;
}

INLDEF float ToDegrees (float _radians)
{
	return (_radians * 180.0f) / M_PIf;
}
INLDEF float ToRadians (float _degrees)
{
	return (_degrees * M_PIf) / 180.0f;
}

INLDEF bool CharacterIsIn (char _c, const char* _str)
{
	for (const char* c=_str; *c; ++c) {
		if (_c == *c) { return true; }
	}
	return false;
}

INLDEF const char* EatLeadingDelimiters (const char* _str, const char* _delim)
{
	const char* c = _str;
	while (*c != '\0' && CharacterIsIn(*c, _delim)) {
		++c;
	}
	return c;
}

INLDEF const char* EatLeadingWhitespace (const char* _str)
{
	return EatLeadingDelimiters(_str, " \n\r\t\v\f");
}

INLDEF bool IsAlphabetical (char _c)
{
	return (((_c >= 'a') && (_c <= 'z')) || ((_c >= 'A') && (_c <= 'Z')));
}
INLDEF bool IsNumeric (char _c)
{
	return ((_c >= '0') && (_c <= '9'));
}
INLDEF bool IsAlphanumeric (char _c)
{
	return (IsAlphabetical(_c) || IsNumeric(_c));
}
INLDEF bool IsWhitespace (char _c)
{
	return ((_c == ' ') || (_c == '\n') || (_c == '\r') || (_c == '\t') || (_c == '\v') || (_c == '\f'));
}

INLDEF size_t GetSizeOfFile (FILE* _file)
{
	// Ensure that the file handle is valid.
	if (!_file) { return 0; }

	fseek(_file, 0L, SEEK_END);
	size_t size = ftell(_file);
	rewind(_file);

	return size;
}

STDDEF String ReadEntireFile (const char* _filename)
{
	String file_content;

	FILE* file = fopen(_filename, "rb");
	if (!file) { return file_content; }

	size_t file_size = GetSizeOfFile(file);

	file_content.Resize(file_size+1); // +1 for null-terminator.
	fread(file_content.c_string, file_size, sizeof(char), file);
	fclose(file);
	file_content.length = file_size;
	file_content.c_string[file_size] = '\0';

	return file_content;
}

INLDEF Array<String> ReadEntireFileIntoLines (const char* _filename)
{
	String file_content = ReadEntireFile(_filename);
	Array<String> lines = file_content.Tokenize("\r\n");

	return lines;
}

STDDEF void GetToken (Tokenizer& _tokenizer)
{
	_tokenizer.token_length = 0; // Ensure this is valid (zero) on entry.

	_tokenizer.cursor = EatLeadingDelimiters(_tokenizer.cursor, _tokenizer.delim);
	_tokenizer.token = _tokenizer.cursor; // This is where the token starts.

	// Read until we hit a delimiter then we end the token.
	while (!CharacterIsIn(*_tokenizer.cursor, _tokenizer.delim) && *_tokenizer.cursor != '\0') {
		++_tokenizer.token_length;
		++_tokenizer.cursor;
	}
}

STDDEF String TokenToString (Tokenizer& _tokenizer)
{
	// @IMPROVE: HANDLE EMBEDDED " CHARACTERS IN A STRING!!!

	_tokenizer.cursor = EatLeadingDelimiters(_tokenizer.cursor, _tokenizer.delim);

	// If no starting quote then the string is invalid.
	String string;
	if (_tokenizer.cursor[0] == '\"') {
		size_t string_length = 0;
		const char* string_start = ++_tokenizer.cursor; // Skip the first quote.
		while (_tokenizer.cursor[0] != '\"' && _tokenizer.cursor[0] != '\0') {
			++string_length;
			++_tokenizer.cursor;
		}
		if (string_length > 0) {
			string.AddCStringOfLength(string_start, string_length);
		}
	}
	return string;
}

STDDEF int TokenToInteger (Tokenizer& _tokenizer)
{
	_tokenizer.cursor = EatLeadingDelimiters(_tokenizer.cursor, _tokenizer.delim);

	char* new_cursor_pos = NULL;
	int integer = CAST(int, strtol(_tokenizer.cursor, &new_cursor_pos, 0));
	_tokenizer.cursor = new_cursor_pos;

	return integer;
}

STDDEF float TokenToNumber (Tokenizer& _tokenizer)
{
	_tokenizer.cursor = EatLeadingDelimiters(_tokenizer.cursor, _tokenizer.delim);

	char* new_cursor_pos = NULL;
	float number = strtof(_tokenizer.cursor, &new_cursor_pos);
	_tokenizer.cursor = new_cursor_pos;

	return number;
}

STDDEF bool TokenToBoolean (Tokenizer& _tokenizer)
{
	_tokenizer.cursor = EatLeadingDelimiters(_tokenizer.cursor, _tokenizer.delim);

	if (_tokenizer.cursor[0] != '\0') {
		if (IsNumeric(_tokenizer.cursor[0])) {
			// Booleans can be integral values.
			return TokenToInteger(_tokenizer);
		} else {
			// Booleans can be string values.
			GetToken(_tokenizer);
			if (_tokenizer.token[0] != '\0') {
				if (strncmp(_tokenizer.token, "true", _tokenizer.token_length) == 0) {
					return true;
				}
			}
		}
	}
	return false;
}

STDDEF SDL_Color TokenToColor (Tokenizer& _tokenizer)
{
	_tokenizer.cursor = EatLeadingDelimiters(_tokenizer.cursor, _tokenizer.delim);

	SDL_Color color = {0};
	color.r = CAST(u8, TokenToInteger(_tokenizer));
	color.g = CAST(u8, TokenToInteger(_tokenizer));
	color.b = CAST(u8, TokenToInteger(_tokenizer));
	color.a = CAST(u8, TokenToInteger(_tokenizer));
	return color;
}

} // TCE

/*////////////////////////////////////////////////////////////////////////////*/
