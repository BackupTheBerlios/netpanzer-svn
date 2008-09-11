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

#include "Resources/ResourceManager.hpp"

#include "FlagSelectionView.hpp"
#include "Views/Game/GameView.hpp"
#include "Views/GameViewGlobals.hpp"
#include "Util/Exception.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Views/Components/Button.hpp"
#include "Views/Components/MouseEvent.hpp"
#include "2D/Color.hpp"

FlagID FlagSelectionView::playerFlagSelected = 0;
Surface FlagSelectionView::playerFlag;

class FlagButton : public Button
{
protected:
    unsigned char fcode;
public:
    FlagButton(int x, int y, Surface &s, unsigned char flagCode)
        : Button("flag")
    {
        setLocation(x, y);
        setSize(s.getWidth(), s.getHeight());
        bimage.create(s.getWidth(), s.getHeight(), 1);
        s.blt(bimage, 0, 0);
        fcode = flagCode;
        
        setExtraBorder();
        borders[1][0] = Color::red;
        borders[1][1] = Color::darkRed;
        borders[2][0] = Color::green;
        borders[2][1] = Color::darkGreen;
    }
    
    void actionPerformed( const mMouseEvent &e)
    {
        if ( e.getID() == mMouseEvent::MOUSE_EVENT_CLICKED )
        {
            FlagSelectionView::setSelectedFlag(fcode);
        }
        else
        {
            Button::actionPerformed(e);
        }
    }
};

// FlagSelectionView
//---------------------------------------------------------------------------
FlagSelectionView::FlagSelectionView() : RMouseHackView()
{
    setSearchName("FlagSelectionView");
    setTitle("Flag Selection");
    setSubTitle("");

    setAllowResize(false);
    setAllowMove(false);
    setVisible(false);

    // The thirty is to give more room to the map selectior view.
    moveTo(bodyTextRect.min.x + bodyTextRect.getSizeX() / 2 + 10 + 30,
           bodyTextRect.min.y + 50);

    resizeClientArea(bodyTextRect.getSizeX() / 2 - 10 - 30, 108);

    init();

} // end FlagSelectionView::FlagSelectionView

// init
//---------------------------------------------------------------------------
void FlagSelectionView::init()
{
    Surface flag;
    // XXX we suppose all the flags has the same size
    Surface * curFlag = ResourceManager::getFlag(0);
    
    //flags.mapFromPalette("netp");

    iXY flagStartOffset(BORDER_SPACE, BORDER_SPACE * 2 + curFlag->getHeight());

    int yOffset = curFlag->getHeight() + 2;

    int x = flagStartOffset.x;
    int y = flagStartOffset.y;

    // XXX to be able to do the loop we need int
    // but it should be FlagID
    // also, the 256 should be MAX_FLAG or similiar.
    for (unsigned int i = 0; i < 256; i++)
    {
        if ( ! ResourceManager::isFlagActive(i) )
            continue;
        
        // XXX we have to copy the flag because of different palettes
        curFlag = ResourceManager::getFlag(i);
        flag.create(curFlag->getWidth(), curFlag->getHeight(), 1);
        curFlag->blt(flag, 0, 0);
        flag.mapFromPalette("netp");
        
        add( new FlagButton( x, y, flag, i) );

        x += flag.getWidth() + 2;

        if (x > flagStartOffset.x + getClientRect().getSizeX() - BORDER_SPACE - (int)flag.getWidth()) {
            x = flagStartOffset.x;
            y += yOffset;
        }
    }

    if(gameconfig->playerflag.isDefaultValue())
    {
        // new player, no flag...
        gameconfig->playerflag=rand()%255; // XXX be lucky, no check for flags.
    }
    
    playerFlagSelected = gameconfig->playerflag;
    if ( ! ResourceManager::isFlagActive(playerFlagSelected) )
        playerFlagSelected = 0;
    
    setSelectedFlag(playerFlagSelected);
    
} // end FlagSelectionView::init

// doDraw
//---------------------------------------------------------------------------
void FlagSelectionView::doDraw(Surface &viewArea, Surface &clientArea)
{
    //iRect r(getViewRect());
    //viewArea.bltLookup(r, Palette::darkGray256.getColorArray());

    char strBuf[256];
    sprintf(strBuf, "Current:");
    int CHAR_XPIX = 8; // XXX hardcoded
    
    clientArea.bltStringShadowed(
            BORDER_SPACE,
            BORDER_SPACE + (playerFlag.getHeight() - Surface::getFontHeight()) / 2,
            strBuf, windowTextColor, windowTextColorShadow);
    
    playerFlag.blt(clientArea, BORDER_SPACE + strlen(strBuf) * CHAR_XPIX + BORDER_SPACE, BORDER_SPACE);

    View::doDraw(viewArea, clientArea);

} // end FlagSelectionView::doDraw
void
FlagSelectionView::setSelectedFlag(FlagID code)
{
    if ( ResourceManager::isFlagActive(code) )
    {
        // XXX have to map because of different palettes.
        Surface * rflag = ResourceManager::getFlag(code);
        playerFlag.create(rflag->getWidth(), rflag->getHeight(), 1);
        rflag->blt(playerFlag, 0, 0);
        playerFlag.mapFromPalette("netp");
        
        playerFlagSelected = code;    
    }
}