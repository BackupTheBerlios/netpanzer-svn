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

#ifndef __TextField_hpp__
#define __TextField_hpp__


#if _MSC_VER > 1000
	#pragma once
#endif


#include "TextComponent.hpp"


//--------------------------------------------------------------------------
class TextField : public TextComponent
{
private:
    int columns;

public:
    TextField()
    {}
    TextField(String text)
    {}
    TextField(int columns)
    {}
    TextField(String Text, int columns)
    {}

    virtual ~TextField()
    {}

    int  getColumns()
    {
        return columns;
    }
    void setColumns(int columns)
    {
        TextField::columns = columns;
    }

}
; // end TextField

#endif // end __TextField_hpp__
