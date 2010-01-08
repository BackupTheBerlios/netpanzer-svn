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

#include "ImageArray.hpp"
#include "Surface.hpp"
#include "lua/lua.hpp"
#include "Util/Log.hpp"

ImageArray::~ImageArray()
{
    if ( images )
    {
        delete[] images;
    }
}

bool
ImageArray::loadImageSheetArray(lua_State* L)
{
    num_images = lua_objlen(L, -1);
    images = new Surface*[num_images];

    for ( int n = 1; n <= num_images; ++n )
    {
        lua_rawgeti(L, -1, n); // current record

        lua_rawgeti(L, -1, 1); // file_name
        lua_rawgeti(L, -2, 2); // width
        lua_rawgeti(L, -3, 3); // height
        lua_rawgeti(L, -4, 4); // nframes

        Surface* s = new Surface();
        s->loadPNGSheet( lua_tostring(L, -4),
                        lua_tointeger(L, -3),
                        lua_tointeger(L, -2),
                        lua_tointeger(L, -1));

        lua_pop(L, 5);

        s->setColorkey();
        s->setOffsetCenter();
        images[n-1] = s;
    }
    return true;
}
