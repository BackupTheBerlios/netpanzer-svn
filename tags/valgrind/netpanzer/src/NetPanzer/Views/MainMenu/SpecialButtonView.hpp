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

#ifndef __SpecialButtonView_hpp__
#define __SpecialButtonView_hpp__


#if _MSC_VER > 1000
	#pragma once
#endif


#include "RMouseHackView.hpp"


//--------------------------------------------------------------------------
class SpecialButtonView : public RMouseHackView
{
protected:
	void addSpecialButton(const iXY &pos, const char *labelString, ITEM_FUNC function)
	{
		addSpecialButton(pos, labelString, labelString, function);
	}
	void addSpecialButton(const iXY &pos, const char *labelString, const char *fileString, ITEM_FUNC function);

public:
	SpecialButtonView() : RMouseHackView() {}
	virtual void mouseMove(const iXY & prevPos, const iXY &newPos);
	virtual void doActivate();

}; // end SpecialButtonView

#endif // end __SpecialButtonView_hpp__
