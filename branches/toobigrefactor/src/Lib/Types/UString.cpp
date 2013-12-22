/*
Copyright (C) 2013 Netpanzer Team. (www.netpanzer.org), Aaron Perez

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
 *
 * Created on March 4, 2013, 11:53 PM
 */

#include "UString.hpp"
#include <stdlib.h>
#include <string.h>

static size_t utf8tounicode(const char * str, const size_t strlength, UString::char_type * dest)
{
    size_t count = 0;
    size_t slen = strlength;
    const unsigned char * st = reinterpret_cast<const unsigned char*>(str);

    while ( slen ) if ( st[0] < 0x80 )
    {
        dest[count++] = st[0];
        slen -= 1;
        st   += 1;
    }
    else if ( (st[0] & 0xe0) == 0xc0 )
    {
        if ( (slen < 2) || ((st[1] & 0xc0) != 0x80) )
        {
            dest[count++] = UString::replacement_char;
            slen -= 1;
            st   += 1;
        }
        else
        {
            dest[count++] = ((st[0] & 0x1f) << 6) | (st[1] & 0x3f);
            slen -= 2;
            st   += 2;
        }
    }
    else if ( (st[0] & 0xf0) == 0xe0 )
    {
        if ( (slen < 3) || ((st[1] & 0xc0) != 0x80) || ((st[2] & 0xc0) != 0x80) )
        {
            dest[count++] = UString::replacement_char;
            slen -= 1;
            st   += 1;
        }
        else
        {
            dest[count++] = ((st[0] & 0x0f) << 12) | ((st[1] & 0x3f) << 6) | (st[2] & 0x3f);
            slen -= 3;
            st   += 3;
        }
    }
    else if ( (st[0] & 0xf8) == 0xf0 )
    {
        if ( (slen < 4) || ((st[1] & 0xc0) != 0x80) || ((st[2] & 0xc0) != 0x80) || ((st[3] & 0xc0) != 0x80))
        {
            dest[count++] = UString::replacement_char;
            slen -= 1;
            st   += 1;
        }
        else
        {
            dest[count++] = ((st[0] & 0x07) << 18) | ((st[1] & 0x3f) << 12) | ((st[2] & 0x3f) << 6) | (st[3] & 0x3f);
            slen -= 4;
            st   += 4;
        }
    }
    else if ( (st[0] & 0xfc) == 0xf8 )
    {
        if ( (slen < 5) || ((st[1] & 0xc0) != 0x80) || ((st[2] & 0xc0) != 0x80) || ((st[3] & 0xc0) != 0x80) || ((st[4] & 0xc0) != 0x80) )
        {
            dest[count++] = UString::replacement_char;
            slen -= 1;
            st   += 1;
        }
        else
        {
            dest[count++] = ((st[0] & 0x03) << 24) | ((st[1] & 0x3f) << 18) | ((st[2] & 0x3f) << 12) | ((st[3] & 0x3f) << 6) | (st[4] & 0x3f);
            slen -= 5;
            st   += 5;
        }
    }
    else if ( (st[0] & 0xfe) == 0xfc )
    {
        if ( (slen < 6) || ((st[1] & 0xc0) != 0x80) || ((st[2] & 0xc0) != 0x80) || ((st[3] & 0xc0) != 0x80) || ((st[4] & 0xc0) != 0x80) || ((st[5] & 0xc0) != 0x80) )
        {
            dest[count++] = UString::replacement_char;
            slen -= 1;
            st   += 1;
        }
        else
        {
            dest[count++] = ((st[0] & 0x01) << 30) | ((st[1] & 0x3f) << 24) | ((st[2] & 0x3f) << 18) | ((st[3] & 0x3f) << 12) | ((st[4] & 0x3f) << 6) | (st[5] & 0x3f);
            slen -= 6;
            st   += 6;
        }
    }
    else
    {
        dest[count++] = UString::replacement_char;
        slen -= 1;
        st   += 1;
    }

    return count;
}

UString::UString()
{
    reset();
}

UString::UString(const size_t len)
{
    reset();
    reserve(len);
}

UString::UString(const char* utf8string, const size_t len)
{
    reset();
    const size_t slen = len ? len : strlen(utf8string);
    if ( reserve(slen) )
    {
        str_length = utf8tounicode(utf8string, slen, data);
    }
    else
    {
        str_length = 0;
    }
}

UString::UString(const UString& other)
{
    reset();
    if ( other.length() )
    {
        if ( reserve( other.length() ) )
        {
            str_length = other.length();
            memcpy(data, other.data, str_length*sizeof(char_type));
        }
    }
}

UString::~UString()
{
    clear();
}

void UString::operator =(const UString& other)
{
    if ( other.length() )
    {
        if ( reserved_length < other.length() )
        {
            clear();
            reserve(other.length());
        }

        str_length = other.length();
        memcpy(data, other.data, str_length*sizeof(char_type));
    }
    else
    {
        str_length = 0;
    }
}

void UString::clear()
{
    if ( data )
    {
        free(data);
        reset();
    }
}
    
bool UString::reserve(size_t len)
{
    if ( data )
    {
        free(data);
    }
    
    data = static_cast<char_type*>(malloc(len*sizeof(char_type)));
    reserved_length = data ? len : 0;
    return data;
}

bool UString::grow(const size_t how_many)
{
    if ( data )
    {
        const size_t new_reserved_length = reserved_length + how_many;
        char_type * new_data = static_cast<char_type*>(realloc(data, new_reserved_length*sizeof(char_type)));
        if ( new_data )
        {
            data = new_data;
            reserved_length = new_reserved_length;
            return true;
        }        
    }
    return false;
}
