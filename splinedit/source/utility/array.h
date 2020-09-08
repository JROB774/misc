#ifndef __ARRAY_H__ /*////////////////////////////////////////////////////////*/
#define __ARRAY_H__

static constexpr size_t ARRAY_STARTING_SIZE = 64;

template<typename T> struct Array
{
	T* data;

	size_t count;
	size_t allocated;

	Array ();
	~Array ();

	Array (const Array& _source);
	Array (Array&& _source);

	Array& operator= (const Array& _source);
	Array& operator= (Array&& _source);

	Array& operator+= (const Array& _array);

	T& operator[] (size_t _index);
	const T& operator[] (size_t _index) const;

	T& Last ();
	const T& Last () const;

	void AddElement (const T& _element);
	void AddElements (const T* _elements, size_t _count);

	void AddArray (const Array& _array);

	void Resize (size_t _size);
	void Clear ();

	void Free ();
};

template<typename T> static inline Array<T> operator+ (Array<T> _lhs, const Array<T>& _rhs);

#endif /* __ARRAY__ //////////////////////////////////////////////////////////*/
