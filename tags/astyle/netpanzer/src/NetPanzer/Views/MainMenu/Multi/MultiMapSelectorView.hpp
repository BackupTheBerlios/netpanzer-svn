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


#ifndef __MultiMapSelectorView_hpp__
#define __MultiMapSelectorView_hpp__


#include "cView.hpp"
#include "Surface.hpp"
#include "iXY.hpp"
#include "ImageSelectorView.hpp"


//---------------------------------------------------------------------------
class MapInfo
{
public:
    Surface thumbnail;
    char    name[256];
    char    description[256];
    iXY     cells;

}
; // end MapInfo

//---------------------------------------------------------------------------
class MultiMapSelectorView : public ImageSelectorView
{
protected:
    cGrowList <MapInfo> mapList;

public:
    MultiMapSelectorView();
    virtual ~MultiMapSelectorView()
    {}

    virtual void loadImages();

    virtual void doDeactivate()
    {
        //mapList.setNum(0);
        //images.free();
    }

}
; // end MultiMapSelectorView

#endif // end __MultiMapSelectorView_hpp__

#endif
