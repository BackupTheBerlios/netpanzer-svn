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

#ifndef __Choice_hpp__
#define __Choice_hpp__


#if _MSC_VER > 1000
	#pragma once
#endif


#include "Component.hpp"
#include "cGrowList.hpp"
#include "MouseEvent.hpp"


//--------------------------------------------------------------------------
class Choice : public Component
{
private:
	cGrowList <String> choiceList;
	int                index;
	int                minWidth;
	int                isOpen;
	int                adjustedY;  // The y translation value to keep the choice on the screen.

	enum { ChoiceItemHeight = 14 };

public:
	Choice()
	{
		reset();
	}
	virtual ~Choice() {}

	void   reset();
	void   add(const String &item);
	void   addItem(const String &item);
	void   addItemDefault(const String &item);
	void   copyItems(const Choice &choice);
	int    getItemCount() const { return choiceList.getCount(); }
	int    getSelectedIndex() const { return index; }
	void   insert(String item, int index);
	String paramString() { return String(); }
	void   remove(String item);
	void   remove(int index);
	void   removeAll();
	void   select(const String &item);
	void   select(int index);
	void   setMinWidth(int minWidth);
	
	virtual void draw(const Surface &dest);
	virtual void actionPerformed(const mMouseEvent &me);

}; // end Choice

#endif // end __Choice_hpp__
