/*******************************************************************************
 * ARRAY
 * A dynamic array type without the STL bloat of inheritance and exceptions.
 *
*******************************************************************************/

#ifndef __TCE_ARRAY_H__ /*////////////////////////////////////////////////////*/
#define __TCE_ARRAY_H__

namespace TCE
{

template<typename T> struct Array
{
	static constexpr size_t STARTING_SIZE = 64;

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

	T& AddElement (const T& _element);
	T& AddElementAtPosition (const T& _element, size_t _pos);

	void AddElements (const T* _elements, size_t _count);

	void RemoveElementAt (size_t _index);

	void AddArray (const Array& _array);

	void Resize (size_t _size);
	void Clear ();

	void Free ();
};

template<typename T> INLDEF Array<T> operator+ (Array<T> _lhs, const Array<T>& _rhs);

} // TCE

#endif /* __TCE_ARRAY_H__ ////////////////////////////////////////////////////*/
