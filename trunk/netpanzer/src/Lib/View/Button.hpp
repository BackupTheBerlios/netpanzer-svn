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

#ifndef __Button_hpp__
#define __Button_hpp__


#if _MSC_VER > 1000
	#pragma once
#endif


#include "Component.hpp"


//--------------------------------------------------------------------------
class Button : public Component
{
protected:
	String label;
	String actionCommand;
	bool   highlighted;
	bool   clicked;

public:
	Button() : Component()
	{
		reset();
	}

	Button(String s) : Component()
	{
		reset();

		label         = s;
		actionCommand = label;
	}
	virtual ~Button() {}

	void reset()
	{
		highlighted   = false;
		clicked       = false;
		label         = "";
		actionCommand = "";
	}

	bool isHighlighted() { return highlighted; }

	void setActionCommand(String l) { label = l; }
	void setLabel(String l) { label = l; }

	String paramString() { return String(); }

	String getLabel() { return label; }
	String getActionCommand() { return actionCommand; }

	//void   processEvent() { label = l; }
	virtual void draw(const Surface &dest);
	virtual void actionPerformed(const mMouseEvent &me);

}; // end Button

#endif // end __Button_hpp__
