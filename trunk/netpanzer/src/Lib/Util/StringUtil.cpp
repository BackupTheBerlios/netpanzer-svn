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
#include <config.h>

#include "StringUtil.hpp"

#include <ctype.h>

std::string removeSurroundingSpaces(const std::string& str)
{
    if(str.size() == 0)
        return "";
    
    std::string::size_type s = 0;
    while(s < str.size() && isspace(str[s]))
        s++;
   
    std::string::size_type e = str.size() - 1;
    while(isspace(str[e])) {
        if(e <= 1)
            break;
        e--;
    }
    
    return std::string(str, s, e-s+1);
}

