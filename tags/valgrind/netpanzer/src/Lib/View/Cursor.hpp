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

#ifndef __Cursor_hpp__
#define __Cursor_hpp__


#if _MSC_VER > 1000
	#pragma once
#endif



//--------------------------------------------------------------------------
class Cursor
{
protected:
	int type;

public:
	enum
	{
		CROSSHAIR_CURSOR,
		DEFAULT_CURSOR,
		DEFAULT_DRAG_CURSOR,
		DEFAULT_DROP_CURSOR,
		DEFAULT_NODROP_CURSOR,
		E_RESIZE_CURSOR,
		HAND_CURSOR,
		MOVE_CURSOR,
		N_RESIZE_CURSOR,
		NE_RESIZE_CURSOR,
		NW_RESIZE_CURSOR,
		S_RESIZE_CURSOR,
		SE_RESIZE_CURSOR,
		SW_RESIZE_CURSOR,
		TEXT_CURSOR,
		W_RESIZE_CURSOR,
		WAIT_CURSOR,
	};

public:
	Cursor() {}
	Cursor(int type) { setType(type); }
	virtual ~Cursor() {}

	inline int getType() { return type; }
	
	void setType(int type) { Cursor::type = type; }

}; // end Cursor

#endif // end __Cursor_hpp__
