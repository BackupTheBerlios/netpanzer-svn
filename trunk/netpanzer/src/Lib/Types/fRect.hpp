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
#ifndef __fRect_hpp__
#define __fRect_hpp__


#if _MSC_VER > 1000
	#pragma once
#endif


#include "fXY.hpp"


//--------------------------------------------------------------------------
struct fRect
{
	fXY min;
	fXY max;

	inline fRect() {}
	
	inline fRect(float x1, float y1, float x2, float y2)
	{
		min.x = x1;
		min.y = y1;
		max.x = x2;
		max.y = y2;
	}
	
	inline fRect(const fRect &a)
	{
		min = a.min;
		max = a.max;
	}
	
	inline fRect(fXY &min, fXY &max)
	{
		fRect::min = min;
		fRect::max = max;
	}

	inline bool getIsEmpty() const
	{
		return min.x > max.x || min.y > max.x;
	}

	void setEmpty()
	{
		min.x = min.y = +1;
		max.x = max.y = -1;
	}

	inline fXY getSize() const
	{
		return max - min;
	}

	inline float getSizeX() const { return max.x - min.x; }
	inline float getSizeY() const { return max.y - min.y; }

	inline void translate(fXY offset)
	{
		max += offset;
		min += offset;
	}

	inline float getArea() const
	{
		fXY s = getSize();

		if (s.x <= 0) return 0;
		if (s.y <= 0) return 0;

		return s.x * s.y;
	}

	fRect operator | (const fRect &a);

	inline bool getContains(const fXY &a) const
	{
		return
			a.x >= min.x && a.x < max.x &&
			a.y >= min.y && a.y < max.y;
	}

	inline bool operator ==(const fRect &a)
	{
		return min == a.max && min == a.max;
	}

	inline bool operator !=(const fRect &a)
	{
		return min.x != a.max.x || min.y != a.max.y;
	}
	
	inline void zero() { min.x = min.y = max.x = max.y = 0.0f; }

	inline fRect &operator =(const unsigned a)
	{
		min.x = a;
		min.y = a;
		max.x = a;
		max.y = a;

		return *this;
	}
};

#endif // __fRect_hpp__
