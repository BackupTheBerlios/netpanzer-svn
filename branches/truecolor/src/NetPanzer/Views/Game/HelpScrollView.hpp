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

#ifndef __HelpScrollView_hpp__
#define __HelpScrollView_hpp__

#include <vector>
#include <string>

#include "Views/MainMenu/SpecialButtonView.hpp"
#include "Views/Components/Button.hpp"

//---------------------------------------------------------------------------
class HelpScrollView : public SpecialButtonView
{
protected:
    int columns;
    int rows;

    int  maxYOffset;
    void drawHelpText( const int &x, const int &y);

    std::vector<std::string> text;

    enum { TEXT_GAP_SPACE = 2 };
    void insert(const char *string);

    Button * upButton;
    Button * downButton;

    int maxViewableItems;
    int topViewableItem;

public:
    HelpScrollView();
    virtual ~HelpScrollView()
    {
    }

    virtual void doDraw();
    virtual void actionPerformed(mMouseEvent me);
    virtual void doActivate();
}
; // end HelpScrollView

#endif // end __HelpScrollView_hpp__
