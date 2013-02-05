/*
Copyright (C) 2004 by Matthias Braun <matze@braunis.de>
 
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
#ifndef __STRINGUTIL_HPP__
#define __STRINGUTIL_HPP__

#include "Core/CoreTypes.hpp"
#include "string.h"
#include <vector>

namespace StringUtil
{
    inline NPString trim(const NPString& str)
    {
        NPString::size_type start = str.find_first_not_of(" \t\n\r");
        NPString::size_type end = str.find_last_not_of(" \t\n\r");

        return NPString(str, start, end-start+1);
    }
    
    struct cstr_sorter : public std::binary_function<const char*, const char*, bool>
    {
        bool operator()(const char* a, const char* b) const
        {
            return strcmp(a, b) < 0;
        }
    };
}

void string_to_params( const NPString& str, std::vector<NPString>& parameters );

#endif

