/**
 *  \file   Buffer.h
 *  \author Jason Fernandez
 *  \date   3/17/2018
 *
 *  https://github.com/jfern2011/util
 */

#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <cstdlib>
#include <cstdio>
#include <execinfo.h>

/**
 * @def _warn_overflow_(index, max)
 *
 * Warns the user of a buffer overflow and prints a stack trace of
 * the error (may require the -rdynamic linker option)
 */
#define _warn_overflow_(index, max)                                  \
{                                                                    \
    if (max <= index)                                                \
    {                                                                \
        void* buffer[1024];                                          \
        int nptrs = ::backtrace(buffer, 1024 );                      \
                                                                     \
        char** strings =                                             \
            ::backtrace_symbols(buffer, nptrs);                      \
                                                                     \
        std::printf("%s:%d [warning]: index = %u, size is %d\n",     \
                    __FILE__, __LINE__, index, max );                \
                                                                     \
        if (strings != nullptr)                                      \
        {                                                            \
            for (int j = 0; j < nptrs; j++)                          \
                std::printf("[%d] %s\n", nptrs-j-1, strings[j]);     \
            std::free(strings);                                      \
        }                                                            \
        std::fflush(stdout);                                         \
    }                                                                \
}

/**
 **********************************************************************
 *
 * A wrapper for a C++ multi-dimensional array. This implements bounds
 * checking to make it easier to catch buffer overflows at runtime.
 * This class is kept as simple as possible to match the complexity of
 * accessing raw arrays
 *
 * @tparam T  The type of each element
 * @tparam N1 Number of elements along the 1st dimension
 * @tparam N2 Number of elements along higher dimensions
 *
 **********************************************************************
 */
template <typename T, int N1, int... N2>
class Buffer
{
	static_assert(N1 > 0, "Dimensions must be greater than zero.");

public:

	Buffer()
	{
	}

	~Buffer()
	{
	}

	/**
	 * Indexing operator. This call produces a Buffer whose number of
	 * dimensions is reduced by 1
	 *
	 * For example, if we have a Buffer<int,2,3>, then we'll get
	 * back a Buffer<int,3>
	 *
	 * @param [in] index The index to look up. An out of bounds value
	 *                   produces a warning message
	 *
	 * @return The element at \a index
	 */
	inline Buffer<T,N2...>& operator[](int index)
	{
		_warn_overflow_(index, N1);
		return data[index];
	}

	/**
	 * Indexing operator. This call produces a Buffer whose number of
	 * dimensions is reduced by 1
	 *
	 * For example, if we have a Buffer<int,2,3>, then we'll get
	 * back a Buffer<int,3>
	 *
	 * @param [in] index The index to look up. An out of bounds value
	 *                   produces a warning message
	 *
	 * @return A *const* reference to the element at \a index
	 */
	inline const
	Buffer<T,N2...>& operator[](int index) const
	{
		_warn_overflow_(index, N1);
		return data[index];
	}

private:

	Buffer<T,N2...> data[N1];
};

/**
 **********************************************************************
 *
 * A wrapper for a simple C++ array. Aside from behaving like a normal
 * array, this performs bounds checking to make it easier to catch
 * buffer overflows at runtime. The implementation is kept simple with
 * the goal of matching the runtime performance of raw arrays
 *
 * @tparam T The type of each buffer element
 * @tparam N The number of elements
 *
 **********************************************************************
 */
template <typename T, int N>
class Buffer<T,N>
{
	static_assert(N > 0, "Buffer must contain at least 1 item.");

public:

	/**
	 * Constructor
	 */
	Buffer()
	{
	}

	/**
	 * Destructor
	 */
	~Buffer()
	{
	}

	/**
	 * Grab the element at the specified index. This is equivalent to
	 * the indexing operator []
	 *
	 * @param [in] index The index to look up. An out of bounds value
	 *                   produces a warning message
	 *
	 * @return The element at \a index
	 */
	inline T& at(int index)
	{
		_warn_overflow_(index, N);
		return data[index];
	}

	/**
	 * Indexing operator. This will return a reference to the element
	 * at the specified index
	 *
	 * @param [in] index The index to look up. An out of bounds value
	 *                   produces a warning message
	 *
	 * @return The element at \a index
	 */
	inline T& operator[](int index)
	{
		_warn_overflow_(index, N);
		return data[index];
	}

	/**
	 * Indexing operator. This will return a *const* reference to the
	 * element at the specified index
	 *
	 * @param [in] index The index to look up. An out of bounds value
	 *                   produces a warning message
	 *
	 * @return The element at \a index
	 */
	inline const T& operator[](int index) const
	{
		_warn_overflow_(index, N);
		return data[index];
	}

	/**
	 * Deference operator
	 *
	 * @return The first element in this Buffer
	 */
	inline T& operator*()
	{
		return data[0];
	}

	/**
	 * Type conversion to a pointer. Allows passing a Buffer to stuff
	 * like std::memcpy()
	 *
	 * @return A pointer to the data
	 */
	inline operator T*()
	{
		return data;
	}

	/**
	 * Type conversion to a pointer. Allows passing a Buffer to stuff
	 * like std::memcpy()
	 *
	 * @return A const pointer to the data
	 */
	inline operator const T*() const
	{
		return data;
	}

	/**
	 * Pointer arithmetic (addition)
	 *
	 * @param[in] offset The offset to add to the start of the buffer
	 *
	 * @return  A pointer to the address at offset \a offset from the
	 *          buffer start, or nullptr on error
	 */
	inline T* operator+(int offset)
	{
		_warn_overflow_(offset, N);
		return data + offset;
	}

private:

	T data[N];
};

#endif
