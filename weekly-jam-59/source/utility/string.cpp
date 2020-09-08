/*////////////////////////////////////////////////////////////////////////////*/

namespace TCE
{

String::String ()
{
	c_string = NULL;
	length = 0;
	allocated = 0;
}
String::~String ()
{
	Free();
}

String::String (const String& _source)
{
	c_string = NULL;
	length = 0;
	allocated = 0;

	// Do not bother if the string to add is empty or invalid.
	if (!_source.c_string || !_source.length) { return; }

	Resize(_source.allocated);
	if (c_string) {
		memcpy(c_string, _source.c_string, _source.allocated * sizeof(char));
		length = _source.length;
	}
}
String::String (String&& _source)
{
	c_string = _source.c_string;
	length = _source.length;
	allocated = _source.allocated;

	_source.c_string = NULL;
	_source.length = 0;
	_source.allocated = 0;
}
String::String (const char* _source)
{
	c_string = NULL;
	length = 0;
	allocated = 0;

	// Do not bother if the c-string is invalid.
	if (!_source) { return; }

	// We double the size to cut down on more slow allocations in the future.
	size_t source_length = strlen(_source);
	Resize((source_length+1) * 2);
	if (c_string) {
		memcpy(c_string, _source, (source_length+1) * sizeof(char));
		length = source_length;
	}
}

String& String::operator= (const String& _source)
{
	// Do not bother if the string to add is empty or invalid.
	if (!_source.c_string || !_source.length) { return *this; }

	Resize(_source.allocated);
	if (c_string) {
		memcpy(c_string, _source.c_string, _source.allocated * sizeof(char));
		length = _source.length;
	}

	return *this;
}
String& String::operator= (String&& _source)
{
	// Make sure to free the old data, if there is any, before moving new data in.
	if (c_string) { FREE(c_string); }

	c_string = _source.c_string;
	length = _source.length;
	allocated = _source.allocated;

	_source.c_string = NULL;
	_source.length = 0;
	_source.allocated = 0;

	return *this;
}
String& String::operator= (const char* _source)
{
	// Do not bother if the c-string is invalid.
	if (!_source) { return *this; }

	// We double the size to cut down on more slow allocations in the future.
	size_t source_length = strlen(_source);
	Resize((source_length+1) * 2);
	if (c_string) {
		memcpy(c_string, _source, (source_length+1) * sizeof(char));
		length = source_length;
	}

	return *this;
}

String& String::operator+= (const String& _string)
{
	AddString(_string);
	return *this;
}
INLDEF String operator+ (String _lhs, const String& _rhs)
{
	_lhs.AddString(_rhs);
	return _lhs;
}

String& String::operator+= (const char* _string)
{
	AddCString(_string);
	return *this;
}
INLDEF String operator+ (String _lhs, const char* _rhs)
{
	_lhs.AddCString(_rhs);
	return _lhs;
}

char& String::operator[] (size_t _index)
{
	return c_string[_index];
}
const char& String::operator[] (size_t _index) const
{
	return c_string[_index];
}

void String::AddCharacter (const char& _character)
{
	// Resize the string if it is not large enough for another character.
	// We double the size to cut down on more slow allocations in the future.
	if ((length+1) >= allocated) {
		size_t new_size = (allocated == 0) ? STARTING_SIZE : allocated * 2;
		Resize(new_size);
	}

	if (c_string) { c_string[length++] = _character; c_string[length] = '\0'; }
}
void String::AddCharacterAtPosition (const char& _character, size_t _pos)
{
	// Do not bother if the position is out of bounds.
	if (_pos > length) { return; }

	// Resize the string if it is not large enough for another character.
	// We double the size to cut down on more slow allocations in the future.
	if ((length+1) >= allocated) {
		size_t new_size = (allocated == 0) ? STARTING_SIZE : allocated * 2;
		Resize(new_size);
	}
	// We do not want to continue if data failed to allocate.
	if (!c_string) { return; }

	// Move the string contents after _pos forward one character.
	size_t length_after_pos = (length+1) - _pos;
	memmove(c_string+_pos+sizeof(char), c_string+_pos, length_after_pos * sizeof(char));

	c_string[_pos] = _character;
	++length;
}

void String::AddCString (const char* _string)
{
	// Do not bother if the c-string is invalid.
	if (!_string) { return; }

	size_t source_length = strlen(_string);

	// If we do not have enough space for the new content, then resize.
	// We double the size to cut down on more slow allocations in the future.
	size_t new_length = length + source_length;
	if (allocated < (new_length+1)) { Resize((new_length+1) * 2); }

	if (c_string) {
		strcat(c_string, _string);
		length = new_length;
	}
}
void String::AddCStringOfLength (const char* _string, size_t _length)
{
	// Do not bother if the string to add is empty or invalid.
	if (!_string || !_length) { return; }

	// If we do not have enough space for the new content, then resize.
	// We double the size to cut down on more slow allocations in the future.
	size_t new_length = length + _length;
	if (allocated < (new_length+1)) { Resize((new_length+1) * 2); }

	if (c_string) {
		strncat(c_string, _string, _length);
		length = new_length;
	}
}

void String::AddString (const String& _string)
{
	// Do not bother if the string to add is empty or invalid.
	if (!_string.c_string || !_string.length) { return; }
	AddCString(_string.c_string);
}

void String::RemoveCharacterAt (size_t _pos)
{
	// Do not bother if the position is out of bounds.
	if (_pos >= length) { return; }
	// Do not bother if the string is empty.
	if (length == 0) { return; }

	// Move the string contents after _pos back one character.
	size_t length_after_pos = length - _pos;
	memmove(c_string+_pos, c_string+_pos+sizeof(char), length_after_pos * sizeof(char));

	// The string has now shrunk by one character.
	--length;
}

size_t String::FindFirst (const char* _characters, size_t _start) const
{
	for (size_t i=_start; i<length; ++i) {
		for (const char* c=_characters; *c; ++c) {
			if (c_string[i] == *c) {
				return i;
			}
		}
	}
	return UNDEFINED_POSITION;
}
size_t String::FindLast (const char* _characters, size_t _end) const
{
	if (_end == String::UNDEFINED_POSITION) { _end = length; }

	size_t pos = String::UNDEFINED_POSITION;
	for (size_t i=0; i<_end; ++i ) {
		for (const char* c=_characters; *c; ++c) {
			if (c_string[i] == *c) {
				pos = i;
				break;
			}
		}
	}
	return pos;
}

String String::Substring (size_t _start, size_t _length) const
{
	// Do not bother if the starting position is out of range.
	String substring;
	if (_start > (length-1)) { return substring; }

	// If this is the case then the caller wants from _start until the end of the string.
	if (_length == UNDEFINED_POSITION || (_start + _length) > length) {
		_length = length-_start;
	}
	substring.AddCStringOfLength(c_string+_start, _length);

	return substring;
}

Array<String> String::Tokenize (const char* _delim) const
{
	Array<String> tokens;
	size_t start = 0, end = 0;

	String token;
	while (end != UNDEFINED_POSITION) {
		end = FindFirst(_delim, start);
		token = Substring(start, (end - start));
		if (token.length > 0) { tokens.AddElement(token); } // Only add meaningful tokens.
		start = end + 1;
	}

	return tokens;
}

void String::Resize (size_t _size)
{
	// If the user wants to shrink the array then move the null-terminator.
	if (_size < length) { c_string[_size] = '\0'; length = _size; return; }
	// If the user wants to enlarge but it's smaller than currently allocated do nothing.
	if (_size <= allocated) { return; }

	char* temp_string = MALLOC(char, _size);
	if (!temp_string) { return; }

	// We do this so that there is always a null-terminator set.
	temp_string[0] = '\0';

	// Copy the elements to the newblock, free the old block, and point to the new block.
	allocated = _size;
	if (c_string) {
		memcpy(temp_string, c_string, (length+1) * sizeof(char));
		FREE(c_string);
	}
	c_string = temp_string;
}
void String::Clear ()
{
	if (c_string) { c_string[0] = '\0'; }
	length = 0;
}

void String::Free ()
{
	FREE(c_string);
	c_string = NULL;

	length = 0;
	allocated = 0;
}

INLDEF bool operator== (const String& _lhs, const String& _rhs)
{
	return (strcmp(_lhs.c_string, _rhs.c_string) == 0);
}
INLDEF bool operator!= (const String& _lhs, const String& _rhs)
{
	return !(_lhs == _rhs);
}
INLDEF bool operator<  (const String& _lhs, const String& _rhs)
{
	return (strcmp(_lhs.c_string, _rhs.c_string) < 0);
}
INLDEF bool operator>  (const String& _lhs, const String& _rhs)
{
	return (_rhs < _lhs);
}
INLDEF bool operator<= (const String& _lhs, const String& _rhs)
{
	return !(_lhs > _rhs);
}
INLDEF bool operator>= (const String& _lhs, const String& _rhs)
{
	return !(_lhs < _rhs);
}

} // TCE

/*////////////////////////////////////////////////////////////////////////////*/
