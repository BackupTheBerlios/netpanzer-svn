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

#ifndef __MenuTemplateView_hpp__
#define __MenuTemplateView_hpp__


#if _MSC_VER > 1000
	#pragma once
#endif


#include "SpecialButtonView.hpp"
#include "Surface.hpp"
#include "PackedSurface.hpp"
#include "iRect.hpp"
#include "iXY.hpp"
#include "String.hpp"


//--------------------------------------------------------------------------
class MenuTemplateView : public SpecialButtonView
{
protected:
	static  void loadNetPanzerLogo();

	virtual void loadBackgroundSurface();
	virtual void loadTitleSurface();

	void doLoadBackgroundSurface(String string);
	void doLoadTitleSurface(String string);

	float curTitleFlashTime;  // Where am I at in the flash?
	float titleFlashTimeHalf; // Time it takes for a half flash.

	typedef void (*ITEM_FUNC)(void);

public:
	MenuTemplateView();

	virtual void doDraw(const Surface &windowArea, const Surface &clientArea);
	virtual void doActivate();
	virtual void doDeactivate();
	virtual void processEvents();

	void initInGameOptionButtons();
	void initPreGameOptionButtons();

	virtual void initButtons();

	static char currentMultiView[256];  // Used to keep track of where you are in the multiplayer menus.
	static char currentView[256];       // Used to keep track of which menu you are currently in.

	//static PackedSurface netPanzerLogo;
	static bool          firstTimeInMenu;
	//static PackedSurface globeSurface;
	static Surface       backgroundSurface;
	static PackedSurface titlePackedSurface;

}; // end MenuTemplateView

#endif // end __MenuTemplateView_hpp__
