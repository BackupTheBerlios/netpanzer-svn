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
//
//---------------------------------------------------------------------------
// Extended to be included in PObject System


#ifndef _POINT_HPP
#define _POINT_HPP

#include <math.h>
#include "codewiz.hpp"
#include "iXY.hpp"
#include "iRect.hpp"
#include "PObject.hpp"

typedef iXY PointXYi;
typedef iRect Recti;

/*
class PointXYi : public PObject 
 {
  public:
        long x, y;

        inline PointXYi() {}
        inline PointXYi(const long nX, const long nY) {
                x = nX;
                y = nY;
        }
        inline PointXYi(const PointXYi &a) {
                x = a.x;
                y = a.y;
        }
        inline PointXYi &operator =(const PointXYi &a) {
                x = a.x;
                y = a.y;
                return *this;
        }
        inline PointXYi &operator =(long a) {
                x = a;
                y = a;
                return *this;
        }

        inline PointXYi &operator +=(const PointXYi &a) { x += a.x; y += a.y; return *this; }
        inline PointXYi &operator -=(const PointXYi &a) { x -= a.x; y -= a.y; return *this; }
        inline PointXYi &operator *=(const PointXYi &a) { x *= a.x; y *= a.y; return *this; }
        inline PointXYi &operator /=(const PointXYi &a) { x /= a.x; y /= a.y; return *this; }
        inline PointXYi &operator *=(long a    ) { x *=   a; y *=   a; return *this; }
        inline PointXYi &operator /=(long a    ) { x /=   a; y /=   a; return *this; }

        inline PointXYi  operator + (const PointXYi &a) const { return PointXYi(x + a.x, y + a.y); }
        inline PointXYi  operator - (const PointXYi &a) const { return PointXYi(x - a.x, y - a.y); }
        inline PointXYi  operator * (const PointXYi &a) const { return PointXYi(x * a.x, y * a.y); }
        inline PointXYi  operator / (const PointXYi &a) const { return PointXYi(x / a.x, y / a.y); }
        inline PointXYi  operator * (long a    ) const { return PointXYi(x *   a, y *   a); }
        inline PointXYi  operator / (long a    ) const { return PointXYi(x /   a, y /   a); }

    inline boolean operator ==(const PointXYi &a) const
        {
                return x == a.x && y == a.y;
        }

        inline boolean operator !=(const PointXYi &a) const
        {
                return x != a.x || y != a.y;
        }

        inline long mag      () const { return (long) ( sqrt(x*x + y*y) ); }
        inline long mag2     () const { return x*x + y*y;       }
        inline PointXYi     getNormal() const { return *this /  long(mag());  }
        inline PointXYi    &normalize()       { return *this /= long(mag());  }
};

inline double distance (const PointXYi &a, const PointXYi &b) { return (a - b).mag();  }
inline double distance2(const PointXYi &a, const PointXYi &b) { return (a - b).mag2(); }
*/

/*
class Recti : public PObject
{
  public:
        PointXYi min, max;

        Recti() {}

        Recti(int x1, int y1, int x2, int y2)
    {
     min = PointXYi(x1, y1);
     max = PointXYi(x2, y2); 
    }
  
   Recti(const Recti &a)
    {
     min = PointXYi(a.min);
     max = PointXYi(a.max);
    }
        
   Recti(PointXYi &nMin, PointXYi &nMax)
    {
     min = PointXYi(nMin);
     max = PointXYi(nMax);
    }


  inline boolean isEmpty() const
  {
        return min.x > max.x || min.y > max.x;
  }

  void empty()
  {
        min.x = min.y = +1;
        max.x = max.y = -1;
  }

   inline PointXYi size() const
    {
                return max-min;
        }

        inline int area() const
        {
        PointXYi s = size();
        if (s.x <= 0) return 0;
        if (s.y <= 0) return 0;
                return s.x*s.y;
        }

        inline Recti operator | (const Recti &a)
     {
      if (  isEmpty()) return a;
      if (a.isEmpty()) return *this;
      return Recti(
                            MIN(min.x, a.min.x), MIN(min.y, a.min.y),
                                MAX(max.x, a.max.x), MAX(max.y, a.max.y)
                  );
     }

        inline boolean contains(const PointXYi &a) const
    { 
        return
                        a.x >= min.x && a.x < max.x &&
                a.y >= min.y && a.y < max.y;
     }
        
        inline boolean intersects(const Recti &a) const
     {
      //Write me!
          //assert(0);
          return a.contains(a.min);
     }

        inline boolean operator ==(const Recti &a)
     {
      return min == a.min && max == a.max;
     }

        inline boolean operator !=(const Recti &a)
     {
          return min != a.min || max != a.max;
     }
        
        inline Recti &operator =(const unsigned a)
         {
                min.x = a;
                min.y = a;
                max.x = a;
                max.y = a;
                return *this;
         }
};
*/
#endif //#ifndef _POINT_HPP
