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

#ifndef __String_hpp__
#define __String_hpp__


#if _MSC_VER > 1000
	#pragma once
#endif


//--------------------------------------------------------------------------
class String
{
private:
	char *mem;            // Pointer to start of string.
	int   length;         // String length.

	friend ostream &operator<<(ostream &output, const String &string)
	{
		output << string.mem;

		return output;
	}

	friend istream &operator>>(istream &input, String &string)
	{
		char temp[256];

		input >> setw(256) >> temp;
		string = temp;

		return input;
	}

public:
	String(const char * = "");  // Convertion constructor.
	String(const String &string);     // Copy constructor.
	~String();
	
	const String &operator =(const String &string);  // Assignment.
	String &operator += (const String &string);      // Concatenation.

	operator const char *() const { return mem; }
	
	// Is string empty?
	int operator !() const { return length == 0; }

	// test s1 == s2.
	int operator ==(const String &string) const { return (strcmp(mem, string.mem) == 0); }

	// // test s1 == s2.
	int operator !=(const String &string) const { return (strcmp(mem, string.mem) != 0); }

	// test s1 < s2.
	int operator <(const String &string) const { return (strcmp(mem, string.mem) < 0); }

	// test s1 > s2.
	int operator >(const String &string) const { return (strcmp(mem, string.mem) > 0); }

	// test s1 >= s2.
	int operator >=(const String &string) const { return (strcmp(mem, string.mem) >= 0); }

	// test s1 <= s2.
	int operator <=(const String &string) const { return (strcmp(mem, string.mem) <= 0); }
	
	// return char reference.
	char &operator [](int subscript)
	{
		if (subscript < 0 || subscript >= length)
		{
			FUBAR("ERROR: Indexing string out of bounds.");
		}

		return mem[subscript];
	}
	
	inline int getLength() const { return length; }

}; // end String

#endif // end __String_hpp__
