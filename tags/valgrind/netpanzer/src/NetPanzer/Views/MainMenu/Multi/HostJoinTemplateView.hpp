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

#ifndef __HostJoinTemplateView_hpp__
#define __HostJoinTemplateView_hpp__


#if _MSC_VER > 1000
	#pragma once
#endif


#include "View.hpp"
#include "Surface.hpp"
#include "MenuTemplateView.hpp"


extern Surface           playerColor;

void bReady();


//---------------------------------------------------------------------------
class HostJoinTemplateView : public MenuTemplateView
{
private:
	void addVehicleButtons(const iXY &pos);
	void drawVehicleInfo(const Surface &dest, const iXY &pos);
	void drawFlagInfo(const Surface &dest, const iXY &pos);
	void drawPlayerInfo(const Surface &dest, const iXY &pos);
	void drawMapInfo(const Surface &dest, const iXY &pos);
	void drawNameInfo(const Surface &clientArea, const iXY &pos);
	
	virtual void loadBackgroundSurface();

public:
	static char gameTypeBuf[256];

	HostJoinTemplateView();

	virtual void doDraw(const Surface &windowArea, const Surface &clientArea);
	virtual void doActivate();

}; // end HostJoinTemplateView

#endif // end __HostJoinTemplateView_hpp__
