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


#ifndef __MiniMapOptionsView_hpp__
#define __MiniMapOptionsView_hpp__


#if _MSC_VER > 1000
	#pragma once
#endif


#include "GameTemplateView.hpp"
#include "Choice.hpp"


class Surface;


//---------------------------------------------------------------------------
class MiniMapOptionsView : public GameTemplateView
{
private:
	void addColorButton(Surface &s, PIX color, iXY &pos, const char *toolTip, ITEM_FUNC function);

	Choice yourUnits;
	Choice yourOutposts;
	Choice alliedOutposts;
	Choice enemyOutposts;
	Choice sectorLines;
	Choice worldViewBox;
	Choice destinationBox;

public:
	MiniMapOptionsView();
	virtual ~MiniMapOptionsView() {}

	virtual void doDraw(const Surface &windowArea, const Surface &clientArea);

}; // end __MiniMapOptionsView_hpp__

#endif // end __MiniMapOptionsView_hpp__
