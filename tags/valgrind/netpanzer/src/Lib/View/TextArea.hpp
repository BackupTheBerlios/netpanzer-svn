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

#ifndef __TextArea_hpp__
#define __TextArea_hpp__


#if _MSC_VER > 1000
	#pragma once
#endif


#include "TextComponent.hpp"
#include "String.hpp"


//--------------------------------------------------------------------------
class TextArea : public TextComponent
{
private:
	int columns;
	int rows;
	int scrollbarVisibility;

public:
	enum
	{
		SCROLLBARS_BOTH,
		SCROLLBARS_HORIZONTAL_ONLY,
		SCROLLBARS_NONE,
		SCROLLBARS_VERTICAL_ONLY,
	};

	TextArea(String text) {}
	TextArea(int rows, int columns) {}
	TextArea(String text, int rows, int columns) {}
	TextArea(String text, int rows, int columns, int scrollbars) {}

	virtual ~TextArea() {}

	void append() {}
	int  getColumns() { return columns; }
	int  getRows() { return rows; }
	int  getScrollbarVisibility() { return scrollbarVisibility; }
	void insert(String str, int pos) {}
	void replaceRange(String str, int start, int end) {}
	void setColumns(int columns) { TextArea::columns = columns; }
	void setRows(int rows) { TextArea::rows = rows; }

}; // end TextArea

#endif // end __TextArea_hpp__
