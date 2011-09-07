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
#ifndef __FlagSelectionView_hpp__
#define __FlagSelectionView_hpp__

#include "Core/CoreTypes.hpp"
#include "Views/MainMenu/RMouseHackView.hpp"
#include "2D/Surface.hpp"

//---------------------------------------------------------------------------
class FlagSelectionView : public RMouseHackView
{
private:
    static FlagID playerFlagSelected;
    static Surface playerFlag;
    
    enum { BORDER_SPACE = 4 };

public:
    FlagSelectionView();
    virtual ~FlagSelectionView()
    {}
    
    static void setSelectedFlag(FlagID code);
    
    static FlagID getSelectedFlag()
    {
        return playerFlagSelected;
    }

    virtual void doDraw(Surface &windowArea, Surface &clientArea);
    //virtual int  lMouseUp(const iXY &downPos, const iXY &upPos);
    //virtual void drawBorder(Surface& )
    //{}

    void init();
}
; // end FlagSelectionView

#endif // end __FlagSelectionView_hpp__
