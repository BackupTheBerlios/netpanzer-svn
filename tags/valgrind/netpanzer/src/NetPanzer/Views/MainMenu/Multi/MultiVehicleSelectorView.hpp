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

// XXX old code?

#if 0

#ifndef __MultiVehicleSelectorView_hpp__
#define __MultiVehicleSelectorView_hpp__


#include "cView.hpp"
#include "Surface.hpp"
#include "iXY.hpp"
#include "ImageSelectorView.hpp"


//---------------------------------------------------------------------------
class MultiVehicleSelectorView : public ImageSelectorView
{
public:
	MultiVehicleSelectorView();
	virtual ~MultiVehicleSelectorView() {}

	virtual void loadImages();
	virtual void doDeactivate()
	{
		//images.free();
	}

}; // end MultiVehicleSelectorView

#endif // end __MultiVehicleSelectorView_hpp__

#endif
