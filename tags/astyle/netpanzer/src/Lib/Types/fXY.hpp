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
#ifndef __fXY_hpp__
#define __fXY_hpp__

class fXY
{
public:
    float x, y;

    fXY()
    {}
    fXY(float x, float y)
    {
        fXY::x = x;
        fXY::y = y;
    }
    fXY(const fXY &a)
    {
        x = a.x;
        y = a.y;
    }

    inline fXY &operator =(const fXY &a)
    {
        x = a.x;
        y = a.y;
        return *this;
    }

    inline fXY &operator =(float a)
    {
        x = a;
        y = a;
        return *this;
    }

    inline fXY &operator +=(const fXY &a)
    {
        x += a.x; y += a.y; return *this;
    }
    inline fXY &operator -=(const fXY &a)
    {
        x -= a.x; y -= a.y; return *this;
    }
    inline fXY &operator *=(const fXY &a)
    {
        x *= a.x; y *= a.y; return *this;
    }
    inline fXY &operator /=(const fXY &a)
    {
        x /= a.x; y /= a.y; return *this;
    }
    inline fXY &operator *=(float      a)
    {
        x *=   a; y *=   a; return *this;
    }
    inline fXY &operator /=(float      a)
    {
        x /=   a; y /=   a; return *this;
    }

    inline bool operator <=(const fXY &a)
    {
        if (x <= a.x && y <= a.y) return true; return false;
    }
    inline bool operator <=(float      a)
    {
        if (x <= a   && y <= a)   return true; return false;
    }
    inline bool operator >=(const fXY &a)
    {
        if (x >= a.x && y >= a.y) return true; return false;
    }
    inline bool operator >=(float      a)
    {
        if (x >= a   && y >= a)   return true; return false;
    }
    inline bool operator  <(const fXY &a)
    {
        if (x <  a.x && y <  a.y) return true; return false;
    }
    inline bool operator  <(float      a)
    {
        if (x <  a   && y <  a)   return true; return false;
    }
    inline bool operator  >(const fXY &a)
    {
        if (x >  a.x && y >  a.y) return true; return false;
    }
    inline bool operator  >(float      a)
    {
        if (x >  a   && y >  a)   return true; return false;
    }

    inline fXY operator + (const fXY &a) const
    {
        return fXY(x + a.x, y + a.y);
    }
    inline fXY operator - (const fXY &a) const
    {
        return fXY(x - a.x, y - a.y);
    }
    inline fXY operator * (const fXY &a) const
    {
        return fXY(x * a.x, y * a.y);
    }
    inline fXY operator / (const fXY &a) const
    {
        return fXY(x / a.x, y / a.y);
    }
    inline fXY operator + (float      a) const
    {
        return fXY(x +   a, y +   a);
    }
    inline fXY operator - (float      a) const
    {
        return fXY(x -   a, y -   a);
    }
    inline fXY operator * (float      a) const
    {
        return fXY(x *   a, y *   a);
    }
    inline fXY operator / (float      a) const
    {
        return fXY(x /   a, y /   a);
    }

    inline bool operator ==(const fXY &a) const
    {
        return x == a.x && y == a.y;
    }

    inline bool operator ==(float a) const
    {
        return x == a && y == a;
    }

    inline bool operator !=(const fXY &a) const
    {
        return x != a.x || y != a.y;
    }

    inline bool operator !=(float a) const
    {
        return x != a && y != a;
    }

    // Negation.
    inline fXY operator -() const
    {
        return fXY(-x, -y);
    }

    inline void zero()
    {
        x = y = 0.0f;
    }

    double mag() const;
    inline float  mag2() const
    {
        return      x * x + y * y ;
    }

    inline double getArea() const
    {
        return x * y;
    }

};

//---------------------------------------------------------------------------
inline double distance(const fXY &a, const fXY &b)
{
    return (a - b).mag();
}

//---------------------------------------------------------------------------
inline float distance2(const fXY &a, const fXY &b)
{
    return (a - b).mag2();
}

#endif // __fXY_hpp__
