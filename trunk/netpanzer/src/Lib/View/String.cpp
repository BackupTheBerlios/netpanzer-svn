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


#include <config.h>
#include "String.hpp"

// String
//---------------------------------------------------------------------------
// Purpose: Conversion constructor: Convert char * to String.
//---------------------------------------------------------------------------
String::String(const char *string)
{
	length = strlen(string);
	mem    = new char [length + 1];
	if (mem == 0)
	{
		FUBAR("ERROR: Unable to allocate string.");
	}
	strcpy(mem, string);

} // String::String

// String
//---------------------------------------------------------------------------
// Purpose: Copy constructor.
//---------------------------------------------------------------------------
String::String(const String &string)
{
	length = string.length;
	mem    = new char [length + 1];
	if (mem == 0)
	{
		FUBAR("ERROR: Unable to allocate string.");
	}
	strcpy(mem, string.mem);

} // String::String

// ~String
//---------------------------------------------------------------------------
// Purpose: Destructor.
//---------------------------------------------------------------------------
String::~String()
{
	delete [] mem;

} // end String::~String

//---------------------------------------------------------------------------
// Purpose: Overloaded = operator; avoids self-assignment.
//---------------------------------------------------------------------------
const String &String::operator=(const String &string)
{
	if (&string != this)
	{
		delete [] mem;
		length = string.length;
		mem    = new char [length + 1];
		if (mem == 0)
		{
			FUBAR("ERROR: Unable to allocate string.");
		}
		strcpy(mem, string.mem);
	}

	return *this;
} // end String::operator=

// operator+=
//---------------------------------------------------------------------------
// Purpose: Concatenate right operand to this object and store in this object.
//---------------------------------------------------------------------------
String &String::operator+=(const String &string)
{
	char *tempPtr = mem;
	length += string.length;
	mem    = new char [length + 1];
	if (mem == 0)
	{
		FUBAR("ERROR: Unable to allocate string.");
	}
	strcpy(mem, tempPtr);
	strcat(mem, string.mem);
	delete [] tempPtr;

	return *this;

} // end String::operator+=
