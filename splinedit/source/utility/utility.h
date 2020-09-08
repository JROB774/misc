#ifndef __UTILITY_H__ /*//////////////////////////////////////////////////////*/
#define __UTILITY_H__

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

template<typename T> INLDEF void Swap (T& _a, T& _b);

struct Point
{
	float x, y;

	Point& operator+= (const Point& _rhs);
	Point& operator-= (const Point& _rhs);
	Point& operator*= (const Point& _rhs);
	Point& operator/= (const Point& _rhs);
};

INLDEF Point operator+ (Point _lhs, const Point& _rhs);
INLDEF Point operator- (Point _lhs, const Point& _rhs);
INLDEF Point operator* (Point _lhs, const Point& _rhs);
INLDEF Point operator/ (Point _lhs, const Point& _rhs);

#endif /* __UTILITY_H__ //////////////////////////////////////////////////////*/
