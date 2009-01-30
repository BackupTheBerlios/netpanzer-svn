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
#include <config.h>

#include "Views/Components/Button.hpp"

#include "Views/MainMenu/Options/InterfaceView.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Views/GameViewGlobals.hpp"
#include "Views/Components/Desktop.hpp"
#include "Views/Components/Label.hpp"

enum
{
    DEC_SCROLL,
    INC_SCROLL
};

static int getScrollRate()
{
    return (int) gameconfig->scrollrate;
}

// InterfaceView
//---------------------------------------------------------------------------
InterfaceView::InterfaceView() : OptionsTemplateView()
{
    setSearchName("InterfaceView");
    setTitle("Interface Options");
    setSubTitle("");

    initButtons();

} // end InterfaceView::InterfaceView

void InterfaceView::initButtons()
{
    OptionsTemplateView::initButtons();

    int xTextStart       = bodyTextRect.min.x;
    int x                = xTextStart;
    int y                = bodyTextRect.min.y;
    int yOffset          =  15;

    int arrowButtonWidth =  16;

    // Settings
    //----------------------------------------------------------------------

    x = xTextStart;
    add( new Label( x, y, "OPTION                                               SETTING", viewHeadingColor) );
    y += yOffset;

    x = xTextStart;
    add( new Label( x, y, "Scroll Rate", Color::white) );

    x = optionsMeterStartX;
    add( Button::createTextButton("decscroll","<",iXY(x-1,y),arrowButtonWidth, DEC_SCROLL));
    x += optionsMeterWidth + arrowButtonWidth;
    add( Button::createTextButton("incscroll",">",iXY(x+1,y),arrowButtonWidth, INC_SCROLL));
    y += yOffset;
}

// doDraw
//---------------------------------------------------------------------------
void InterfaceView::doDraw()
{
    MenuTemplateView::doDraw();

    char strBuf[256];

    const int arrowButtonWidth = 16;
    int   x               = optionsMeterStartX + arrowButtonWidth;
    int   y               = bodyTextRect.min.y + 15;

    Surface tempSurface(optionsMeterWidth, 14, 1);

    // Scroll Rate
    tempSurface.fill(meterColor);
    tempSurface.drawButtonBorder(meterTopLeftBorderColor, meterBottomRightBorderColor);
    sprintf(strBuf, "%d %%", int((float(getScrollRate()) / 
                    float(gameconfig->scrollrate.getMax())) * 100.0f));
    tempSurface.bltStringCenter(strBuf, meterTextColor);
    drawImage(tempSurface, x, y);
} // end InterfaceView::doDraw

void
InterfaceView::onComponentClicked(Component* c)
{
    switch ( c->getCustomCode() )
    {
        case DEC_SCROLL:
            if(gameconfig->scrollrate - 100 >= gameconfig->scrollrate.getMin())
            {
                gameconfig->scrollrate = gameconfig->scrollrate - 100;
            }
            break;

        case INC_SCROLL:
            if(gameconfig->scrollrate + 100 <= gameconfig->scrollrate.getMax())
            {
                gameconfig->scrollrate = gameconfig->scrollrate + 100;
            }
            break;
    }
}

// loadTitleSurface
//---------------------------------------------------------------------------
void InterfaceView::loadTitleSurface()
{
    doLoadTitleSurface("interfaceTitle");
} // end InterfaceView::loadTitleSurface
