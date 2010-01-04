/*
Copyright (C) 2009 by Aaron Perez <aaronps@gmail.com>

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

 * Created on January 4, 2009, 07:07 AM
 */

#ifndef _IMAGEARRAY_HPP_
#define _IMAGEARRAY_HPP_

class Surface;
struct lua_State;

class ImageArray
{
public:
    ImageArray() : images(0), num_images(0) {}
    ~ImageArray();

    unsigned int size() { return num_images; }
    Surface* getImage(unsigned int n) { return images[n]; }

    bool loadImageSheetArray(lua_State* L);

private:
    Surface** images;
    unsigned int num_images;
    
};

#endif
