/*////////////////////////////////////////////////////////////////////////////*/

template<typename T> Array<T>::Array ()
{
	data = NULL;
	count = 0;
	allocated = 0;
}
template<typename T> Array<T>::~Array ()
{
	Free();
}

template<typename T> Array<T>::Array (const Array& _source)
{
	data = NULL;
	count = 0;
	allocated = 0;

	Resize(_source.allocated);
	if (data) {
		memcpy(data, _source.data, _source.allocated * sizeof(T));
		count = _source.count;
	}
}
template<typename T> Array<T>::Array (Array&& _source)
{
	data = _source.data;
	count = _source.count;
	allocated = _source.allocated;

	_source.data = NULL;
	_source.count = 0;
	_source.allocated = 0;
}

template<typename T> Array<T>& Array<T>::operator= (const Array& _source)
{
	Resize(_source.allocated);
	if (data) {
		memcpy(data, _source.data, _source.allocated * sizeof(T));
		count = _source.count;
	}

	return *this;
}
template<typename T> Array<T>& Array<T>::operator= (Array&& _source)
{
	// Make sure to free the old data, if there is any, before moving new data in.
	if (data) { delete[] data; }

	data = _source.data;
	count = _source.count;
	allocated = _source.allocated;

	_source.data = NULL;
	_source.count = 0;
	_source.allocated = 0;

	return *this;
}

template<typename T> Array<T>& Array<T>::operator+= (const Array& _array)
{
	AddArray(_array);
	return *this;
}
template<typename T> static inline Array<T> operator+ (Array<T> _lhs, const Array<T>& _rhs)
{
	_lhs.AddArray(_rhs);
	return _lhs;
}

template<typename T> T& Array<T>::operator[] (size_t _index)
{
	return data[_index];
}
template<typename T> const T& Array<T>::operator[] (size_t _index) const
{
	return data[_index];
}

template<typename T> T& Array<T>::Last ()
{
	return data[count-1];
}
template<typename T> const T& Array<T>::Last () const
{
	return data[count-1];
}

template<typename T> void Array<T>::AddElement (const T& _element)
{
	// Resize the dynamic array if it is not large enough for another element.
	// We double the size to cut down on more slow allocations in the future.
	if (count >= allocated) {
		size_t new_size = (allocated == 0) ? ARRAY_STARTING_SIZE : allocated * 2;
		Resize(new_size);
	}

	if (data) { data[count++] = _element; }
}
template<typename T> void Array<T>::AddElements (const T* _elements, size_t _count)
{
	// Do not bother if the array to add is empty or invalid.
	if (!_elements || !_count) { return; }

	// If we do not have enough space for the new content, then resize.
	// We double the size to cut down on more slow allocations in the future.
	size_t new_count = count + _count;
	if (allocated < new_count) { Resize(new_count * 2); }

	// Copy the block of data elements into the array at the end.
	if (data) {
		memcpy(&data[count], _elements, _count * sizeof(T));
		count = new_count;
	}
}

template<typename T> void Array<T>::AddArray (const Array& _array)
{
	// Do not bother if the array to add is empty or invalid.
	if (!_array.data || !_array.count) { return; }
	AddElements(_array.data, _array.count);
}

template<typename T> void Array<T>::Resize (size_t _size)
{
	// If the user wants to shrink the array simply lower the count.
	if (_size < count) { count = _size; return; }
	// If the user wants to enlarge but it's smaller than currently allocated then do nothing.
	if (_size <= allocated) { return; }

	// We use new and delete instead of malloc and free in the array and nowhere else
	// because the array needs to handle the potential for types it stores having a
	// constructor and destructor that must be handled to avoid issues (e.g. String).
	T* temp_array = new(std::nothrow) T[_size];
	if (!temp_array) { return; }

	// Copy the elements to the newblock, free the old block, and point to the new block.
	allocated = _size;
	if (data) {
		memcpy(temp_array, data, count * sizeof(T));

		// So we don't destruct the moved pointers when we delete below!!!
		memset(data, 0, count * sizeof(T));
		delete[] data;
	}
	data = temp_array;
}
template<typename T> void Array<T>::Clear ()
{
	count = 0;
}

template<typename T> void Array<T>::Free ()
{
	delete[] data;
	data = NULL;

	count = 0;
	allocated = 0;
}

/*////////////////////////////////////////////////////////////////////////////*/
