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
#ifndef _BOUNDBOX_HPP
#define _BOUNDBOX_HPP

#include <assert.h>

#include "Types/iRect.hpp"
//#include <math.h>
#include <stdlib.h>

class BoundBox
{
public:
    iXY siz;
    inline BoundBox() {}
    inline BoundBox(const BoundBox& a) : siz(a.siz) {}
    inline BoundBox(const iXY& siz) : siz(siz) {}

    bool bounds(const iXY &center, const iXY &test ) const
    {
        const int x = abs(center.x - test.x);
        const int y = abs(center.y - test.y);
        return (x < siz.x) && (y < siz.y);
    }

    void setSize(const iXY& siz) { this->siz = siz; }

    inline iRect getAbsRect(const iXY& center) const
    {
        return( iRect( center.x - siz.x, center.y - siz.y, siz.x*2, siz.y*2 ) );
    }

};

#endif
