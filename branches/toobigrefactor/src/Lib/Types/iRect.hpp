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
#ifndef __iRect_hpp__
#define __iRect_hpp__

#include "iXY.hpp"

struct iRect
{
private:
    iXY position;
    iXY size;
    
public:
    friend iRect operator+(const iRect& r, const iXY& xy);
    
    inline iRect()
    {}

    inline iRect(const int x, const int y, const unsigned width, const unsigned height)
    {
        position.x = x;
        position.y = y;
        size.x = width;
        size.y = height;
    }
    
    inline iRect(const iRect &a)
    {
        position = a.position;
        size = a.size;
    }
    
    inline iRect(const iXY& pos, const iXY& siz) : position(pos), size(siz)
    {
        // empty
    }

    inline const iXY& getLocation() const { return position; }
    inline int getLocationX()       const { return position.x; }
    inline int getLocationY()       const { return position.y; }
    inline int getEndX()            const { return position.x + size.x - 1; }
    inline int getEndY()            const { return position.y + size.y - 1; }
    
    inline void setLocation(const int x, const int y) { position.x = x; position.y = y; }
    inline void setLocation(const iXY& loc) { position = loc; }
    inline void setLocationX(const int x)   { position.x = x; }
    inline void setLocationY(const int y)   { position.y = y; }
    
    inline const iXY getMax() const { return position+size; }
    
    inline void setSize(const int x, const int y) { size.x = x; size.y = y; }
    inline void setSize(const iXY& s) { size = s; }
    inline void setWidth(const int w) { size.x = w; }
    inline void setHeight(const int h) { size.y = h; }
    
    inline const iXY& getSize()  const { return size; }
    inline unsigned getWidth()  const { return size.x; }
    inline unsigned getHeight() const { return size.y; }

    inline void translate(iXY offset) { position += offset; }
    inline void translate(const int x, const int y) { position.x += x; position.y += y; }
    
    inline void grow(const int x, const int y) { position.x-=x; position.y -=y; size.x +=x*2; size.y += y*2; }

    inline void scale(const float x, const float y)
    {
        position.x *= x;
        position.y *= y;
        size.x *= x;
        size.y *= y;
    }
    
    bool isEmpty() const { return (size.x <= 0) || (size.y <= 0); }

    inline int getArea() const
    {
        int s = size.x * size.y;
        return (s < 0) ? 0 : s;
    }
    
    inline bool contains(const iXY &a) const
    {
        return ( a.x >= position.x ) && ( a.x <= (position.x+size.x) )
            && ( a.y >= position.y ) && ( a.y <= (position.y+size.y) );
    }
    
    inline bool contains(const int x, const int y) const
    {
        return ( x >= position.x ) && ( x <= (position.x+size.x) )
            && ( y >= position.y ) && ( y <= (position.y+size.y) );
    }

    inline bool operator ==(const iRect &a)
    {
        return position == a.position && size == a.size;
    }

    inline bool operator !=(const iRect &a)
    {
        return !(operator==(a));
    }

    inline void zero() { position.x = position.y = size.x = size.y = 0; }

    inline bool clip( const iRect &a ) const
    {
        if ( (a.position.y > (position.y + size.y)) || ((a.position.y + a.size.y) < position.y)
          || (a.position.x > (position.x + size.x)) || ((a.position.x + a.size.x) < position.x) )
            return true;

        return false;
    }
};

inline iRect operator+(const iRect& r, const iXY& xy)
{
    return iRect(r.position.x + xy.x, r.position.y + xy.y, r.size.x, r.size.y);
}

#endif // __iRect_hpp__
