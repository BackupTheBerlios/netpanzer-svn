#ifndef __LIB_UTIL_HPP__
#define __LIB_UTIL_HPP__

// Utility functions which aren't defined in unix.

/***** Additions by Matze... Are these already defined on msvc? */

#ifndef MSVC
template<typename T>
static inline void SWAP(T& v1, T& v2)
{
	T temp = v1;
	v1 = v2;
	v2 = temp;
}

template<typename T>
static inline void SWAP(T& v1, T& v2, T& temp)
{
	temp = v1;
	v1 = v2;
	v2 = temp;
}

template<typename T>
static inline const T& MIN(const T& v1, const T& v2)
{
	return v1 < v2 ? v1 : v2;
}

template<typename T>
static inline const T& MAX(const T& v1, const T& v2)
{
	return v1 < v2 ? v2 : v1;
}
#endif

#endif
