/*
Copyright (C) 1998 Pyrosoft Inc. (www.pyrosoftgames.com), Matthew Bogue

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

#ifndef __stdafx_hpp__
#define __stdafx_hpp__


#if _MSC_VER > 1000
	#pragma once
#endif

#include <sdl.h>
#include <assert.h>
#include <ctype.h>
#include <conio.h>
#include <dos.h>
#include <direct.h>
#include <float.h>
#include <fstream.h>
#include <io.h>
#include <iomanip.h>
#include <iostream.h>
#include <limits.h>
#include <malloc.h>
#include <math.h>
#include <new.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <ddraw.h>
#include <dsound.h>

#include "codewiz.hpp"

#ifndef __LibTypes_hpp__
	#include "LibTypes.hpp"
#endif

#ifndef WIN32_EXTRA_LEAN
	#define WIN32_EXTRA_LEAN
#endif

//#pragma warning (disable: 4244) //warning C4244: 'initializing' : conversion from 'double' to 'float', possible loss of data
//#pragma warning (disable: 4305) //warning C4305: 'argument' : truncation from 'const double' to 'float'
//#pragma warning (disable: 4800) //warning C4800: 'int' : forcing value to bool 'true' or 'false' (performance warning)

#undef min
#undef max

// MIN
//---------------------------------------------------------------------------
template <class T>
inline const T &MIN(const T &a, const T &b)
{
	return (a < b) ? a : b;
} // end MIN

// MAX
//---------------------------------------------------------------------------
template <class T>
inline const T &MAX(const T &a, const T &b)
{
	return (a > b) ? a : b;
} // end MAX

// SWAP
//---------------------------------------------------------------------------
template <class T>
inline void SWAP(T &a, T &b)
{
	T temp(a); a = b; b = temp;
} // end SWAP

// SWAP
//---------------------------------------------------------------------------
template <class T>
inline void SWAP(T &a, T &b, T &t)
{
	t = a; a = b; b = t;
} // end SWAP

#endif // __stdafx_hpp__
