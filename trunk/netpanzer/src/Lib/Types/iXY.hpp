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
#ifndef __iXY_hpp__
#define __iXY_hpp__


#if _MSC_VER > 1000
	#pragma once
#endif


struct iXY
{
	int x;
	int y;

	iXY() {}
	
	iXY(const int x, const int y)
	{
		iXY::x = x;
		iXY::y = y;
	}
	
	iXY(const iXY &a)
	{
		x = a.x;
		y = a.y;
	}
	
	inline iXY &operator =(const iXY &a)
	{
		x = a.x;
		y = a.y;
		return *this;
	}
	
	inline iXY &operator =(const int &a)
	{
		x = a;
		y = a;
		return *this;
	}

	inline iXY  &operator +=(const iXY &a) { x += a.x; y += a.y; return *this; }
	inline iXY  &operator +=(int        a) { x +=   a; y +=   a; return *this; }
	inline iXY  &operator -=(const iXY &a) { x -= a.x; y -= a.y; return *this; }
	inline iXY  &operator -=(int        a) { x -=   a; y -=   a; return *this; }
	inline iXY  &operator *=(const iXY &a) { x *= a.x; y *= a.y; return *this; }
	inline iXY  &operator *=(int        a) { x *=   a; y *=   a; return *this; }
	inline iXY	&operator /=(const iXY &a) { x /= a.x; y /= a.y; return *this; }
	inline iXY	&operator /=(int        a) { x /=   a; y /=   a; return *this; }

	inline bool operator <=(const iXY &a) { if (x <= a.x && y <= a.y) return true; return false; }
	inline bool operator <=(int        a) { if (x <= a   && y <= a)   return true; return false; }
	inline bool operator >=(const iXY &a) { if (x >= a.x && y >= a.y) return true; return false; }
	inline bool operator >=(int        a) { if (x >= a   && y >= a)   return true; return false; }
	inline bool operator  <(const iXY &a) { if (x <  a.x && y <  a.y) return true; return false; }
	inline bool operator  <(int        a) { if (x <  a   && y <  a)   return true; return false; }
	inline bool operator  >(const iXY &a) { if (x >  a.x && y >  a.y) return true; return false; }
	inline bool operator  >(int        a) { if (x >  a   && y >  a)   return true; return false; }

	inline iXY operator + (const iXY &a) const { return iXY(x + a.x, y + a.y); }
	inline iXY operator - (const iXY &a) const { return iXY(x - a.x, y - a.y); }
	inline iXY operator * (const iXY &a) const { return iXY(x * a.x, y * a.y); }
	inline iXY operator / (const iXY &a) const { return iXY(x / a.x, y / a.y); }
	inline iXY operator % (const iXY &a) const { return iXY(x % a.x, y % a.y); }
	inline iXY operator + (int        a) const { return iXY(x +   a, y +   a); }
	inline iXY operator - (int        a) const { return iXY(x -   a, y -   a); }
	inline iXY operator * (int        a) const { return iXY(x *   a, y *   a); }
	inline iXY operator / (int        a) const { return iXY(x /   a, y /   a); }
	inline iXY operator % (int        a) const { return iXY(x %   a, y %   a); }

	inline bool operator ==(const iXY &a) const
	{
		return x == a.x && y == a.y;
	}

	inline bool operator ==(int a) const
	{
		return x == a && y == a;
	}

	inline bool operator !=(const iXY &a) const
	{
		return x != a.x || y != a.y;
	}

	inline bool operator !=(int a) const
	{
		return x != a && y != a;
	}

	// Negation.
	inline iXY operator -() const { return iXY(-x, -y); }

	inline void zero() { x = y = 0; }

	double mag() const;
	inline float  mag2() const { return float(x * x + y * y); }

	inline int getArea() const { return x * y; }

};

//---------------------------------------------------------------------------
inline double distance(const iXY &a, const iXY &b)
{
	return (a - b).mag();
}

//---------------------------------------------------------------------------
inline float distance2(const iXY &a, const iXY &b)
{
	return (a - b).mag2();
}

#endif // __iXY_hpp__
