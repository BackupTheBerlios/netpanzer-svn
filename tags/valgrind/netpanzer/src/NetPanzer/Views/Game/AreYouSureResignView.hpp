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

#ifndef __AreYouSureResignView_hpp__
#define __AreYouSureResignView_hpp__


#if _MSC_VER > 1000
	#pragma once
#endif


#include "SpecialButtonView.hpp"
#include "Surface.hpp"


//---------------------------------------------------------------------------
class AreYouSureResignView : public SpecialButtonView
{
private:
	void init();

public:
	AreYouSureResignView();

	virtual void doDraw(const Surface &windowArea, const Surface &clientArea);
	virtual void doActivate();

}; // end AreYouSureResignView

#endif // end __AreYouSureResignView_hpp__