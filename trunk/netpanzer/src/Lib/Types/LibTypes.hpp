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

#ifndef __LibTypes_hpp__
#define __LibTypes_hpp__


#if _MSC_VER > 1000
	#pragma once
#endif


typedef unsigned char   BYTE; //  8 - bits
typedef unsigned short  WORD; // 16 - bits
typedef unsigned long  DWORD; // 32 - bits

typedef int BOOL;

#ifndef TRUE
	const BOOL TRUE  = (7 == 7);
	const BOOL FALSE = (7 != 7);
#endif

enum
{
	HORIZONTAL = 0,
	VERTICAL   = 1,
};

#ifndef PI
#define PI 3.14159265358979323846
#endif

#define randomize() srand(time(NULL))

#endif // __LibTypes_hpp__
