/*
Copyright (C) 2003 Matthias Braun <matze@braunis.de>
                                                                                
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.
                                                                                
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
                                                                                
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#ifndef __LIB_UTIL_HPP__
#define __LIB_UTIL_HPP__

// Utility functions (these have been used but not defined in the code?)

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
