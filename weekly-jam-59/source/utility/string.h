/*******************************************************************************
 * STRING
 * A string type without the STL bloat of inheritance and exceptions.
 *
*******************************************************************************/

#ifndef __TCE_STRING_H__ /*////////////////////////////////////////////////*/
#define __TCE_STRING_H__

namespace TCE
{

struct String
{
	static constexpr size_t STARTING_SIZE = 64;
	static constexpr size_t UNDEFINED_POSITION = CAST(size_t, -1);

	char* c_string;

	size_t length;
	size_t allocated;

	String ();
	~String ();

	String (const String& _source);
	String (String&& _source);
	String (const char* _source);

	String& operator= (const String& _source);
	String& operator= (String&& _source);
	String& operator= (const char* _source);

	String& operator+= (const String& _string);
	String& operator+= (const char* _string);

	char& operator[] (size_t _index);
	const char& operator[] (size_t _index) const;

	void AddCharacter (const char& _character);
	void AddCharacterAtPosition (const char& _character, size_t _pos);

	void AddCString (const char* _string);
	void AddCStringOfLength (const char* _string, size_t _length);

	void AddString (const String& _string);

	void RemoveCharacterAt (size_t _pos);

	size_t FindFirst (const char* _characters, size_t _start = 0) const;
	size_t FindLast (const char* _characters, size_t _end = String::UNDEFINED_POSITION) const;

	String Substring (size_t _start = 0, size_t _length = UNDEFINED_POSITION) const;

	Array<String> Tokenize (const char* _delim) const;

	void Resize (size_t _size);
	void Clear ();

	void Free ();
};

INLDEF String operator+ (String _lhs, const String& _rhs);
INLDEF String operator+ (String _lhs, const char* _rhs);

INLDEF bool operator== (const String& _lhs, const String& _rhs);
INLDEF bool operator!= (const String& _lhs, const String& _rhs);
INLDEF bool operator<  (const String& _lhs, const String& _rhs);
INLDEF bool operator>  (const String& _lhs, const String& _rhs);
INLDEF bool operator<= (const String& _lhs, const String& _rhs);
INLDEF bool operator>= (const String& _lhs, const String& _rhs);

} // TCE

#endif /* __TCE_STRING_H__ ///////////////////////////////////////////////////*/
