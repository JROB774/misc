/*******************************************************************************
 * UTILITY
 * Generic defintions and utility functions for use throughout the code.
 *
*******************************************************************************/

#ifndef __TCE_UTILITY_H__ /*//////////////////////////////////////////////////*/
#define __TCE_UTILITY_H__

typedef uint8_t   u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t    s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;

// More descriptive and clear definitions of the multi-purpose static keyword.
#define GLOBAL static
#define PERSISTENT static
#define INTERNAL static

// All defined functions should use either one of the below definitions.
//
// We use static functions as we only have one compilation unit so by declaring all
// functions static we potentially speed up the compilation and linking process.
#define STDDEF INTERNAL
#define INLDEF INTERNAL inline

#define CAST(__type, __value) ((__type)(__value))

#define MALLOC(__type, __count) (__type*)malloc((__count) * sizeof(__type))
#define REALLOC(__type, __data, __count) (__type*)realloc((__data), (__count) * sizeof(__type))
#define CALLOC(__type, __count) (__type*)calloc((__count), sizeof(__type))

#define FREE(__data) free((__data))

#define ARRAY_COUNT(__array) (sizeof(__array) / sizeof((__array)[0]))

namespace TCE
{

typedef size_t AssetHandle;
typedef size_t TextureHandle;
typedef size_t AnimationHandle;
typedef size_t FontHandle;
typedef size_t SoundHandle;
typedef size_t LightHandle;

GLOBAL constexpr AssetHandle INVALID_ASSET_HANDLE = CAST(AssetHandle, -1);

// We predefines these types so utility functions can use them.
template<typename T> struct Array;
struct String;

// A custom color union for easy translations between access methods.
union Color
{
	u32 numeric = 0;
	SDL_Color component;
};

// A simple point structure for representing a 2D position/point.
struct Point
{
	int x, y;
};

INLDEF bool operator== (const Point& _lhs, const Point& _rhs);
INLDEF bool operator!= (const Point& _lhs, const Point& _rhs);

// Floating-point variations of the standard math.h constant defines.
GLOBAL constexpr float M_Ef        = 2.718281828459f;
GLOBAL constexpr float M_LOG2Ef    = 1.442695040889f;
GLOBAL constexpr float M_LOG10Ef   = 0.4342944819033f;
GLOBAL constexpr float M_LN2f      = 0.6931471805599f;
GLOBAL constexpr float M_LN10f     = 2.302585092994f;
GLOBAL constexpr float M_PIf       = 3.141592653590f;
GLOBAL constexpr float M_PI_2f     = 1.570796326795f;
GLOBAL constexpr float M_PI_4f     = 0.7853981633974f;
GLOBAL constexpr float M_1_PIf     = 0.3183098861838f;
GLOBAL constexpr float M_2_PIf     = 0.6366197723676f;
GLOBAL constexpr float M_2_SQRTPIf = 1.128379167095f;
GLOBAL constexpr float M_SQRT2f    = 1.414213562373f;
GLOBAL constexpr float M_SQRT1_2f  = 0.7071067811865f;
GLOBAL constexpr float M_TAUf      = 6.283185307180f;

INLDEF float RandomFloat (float _min, float _max);

template<typename T> INLDEF void Swap (T& _a, T& _b);

INLDEF float ToDegrees (float _radians);
INLDEF float ToRadians (float _degrees);

INLDEF bool CharacterIsIn (char _c, const char* _str);

INLDEF const char* EatLeadingDelimiters (const char* _str, const char* _delim);
INLDEF const char* EatLeadingWhitespace (const char* _str);

INLDEF bool IsAlphabetical (char _c);
INLDEF bool IsNumeric (char _c);
INLDEF bool IsAlphanumeric (char _c);
INLDEF bool IsWhitespace (char _c);

INLDEF size_t GetSizeOfFile (FILE* _file);

STDDEF String ReadEntireFile (const char* _filename);
INLDEF Array<String> ReadEntireFileIntoLines (const char* _filename);

struct Tokenizer
{
	// The current position in the pointed to string data.
	const char* cursor;
	// The delimiters to tokenize with.
	const char* delim;

	size_t token_length;
	const char* token;
};

STDDEF void GetToken (Tokenizer& _tokenizer);

STDDEF String    TokenToString (Tokenizer& _tokenizer);
STDDEF int       TokenToInteger (Tokenizer& _tokenizer);
STDDEF float     TokenToNumber (Tokenizer& _tokenizer);
STDDEF bool      TokenToBoolean (Tokenizer& _tokenizer);
STDDEF SDL_Color TokenToColor (Tokenizer& _tokenizer);

} // TCE

#endif /* __TCE_UTILITY_H__ //////////////////////////////////////////////////*/
