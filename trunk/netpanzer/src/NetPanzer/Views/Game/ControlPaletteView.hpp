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
//---------------------------------------------------------------------------
#ifndef __ControlPaletteView_hpp__
#define __ControlPaletteView_hpp__


#if _MSC_VER > 1000
	#pragma once
#endif


#include "View.hpp"
#include "Surface.hpp"
#include "ControlView.hpp"
#include "Desktop.hpp"


//---------------------------------------------------------------------------
class ControlPaletteView : public View
{
private:
	//static int drawMethod;  // Should it be drawn solid or blended...
	

  public:
	ControlPaletteView();

	static void init()
	{
		Desktop::add(new ControlView("0ControlView"));
		Desktop::add(new ControlView("1ControlView"));
		Desktop::add(new ControlView("2ControlView"));

		setVertical();
	}

	static void setVertical()
	{
		View *v;
		
		v = Desktop::getView("0ControlView");

		if (v != 0)
		{
			v->moveTo(getDocPos(0));
		} else
		{
			assert(false);
		}

		v = Desktop::getView("1ControlView");

		if (v != 0)
		{
			v->moveTo(getDocPos(1));
		} else
		{
			assert(false);
		}

		v = Desktop::getView("2ControlView");

		if (v != 0)
		{
			v->moveTo(getDocPos(2));
		} else
		{
			assert(false);
		}

	}

	static void setHorizontal()
	{
		//Desktop::getView("0ControlView")->moveTo(getHorizontalDocPos(0));
		//Desktop::getView("1ControlView")->moveTo(getHorizontalDocPos(1));
		//Desktop::getView("2ControlView")->moveTo(getHorizontalDocPos(2));
	}

	// Handles returning the vertical docking position depending of resolution.
	static iXY getDocPos(int controlPaletteNumber)
	{
		iXY pos;

		if (ControlView::orientation == ControlView::VERTICAL)
		{
			pos.x = 0;
			pos.y = ControlView::iconBarMinSize + controlPaletteNumber * ControlView::getViewSize().y;
		}
		else if (ControlView::orientation == ControlView::HORIZONTAL)
		{
			pos.x = ControlView::iconBarMinSize + controlPaletteNumber * ControlView::getViewSize().x;
			pos.y = 0;
		} else
		{
			assert(false);
		}

		return pos;
	}

	virtual void doDraw(const Surface &windowArea, const Surface &clientArea);

}; // end ControlPaletteView

#endif // end __ControlPaletteView_hpp__
