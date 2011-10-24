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


#include "Resources/ResourceManager.hpp"

#include "FlagSelectionView.hpp"
#include "Views/Game/GameView.hpp"
#include "Views/GameViewGlobals.hpp"
#include "Util/Exception.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Views/Components/Button.hpp"
#include "Views/Components/Label.hpp"
#include "Views/Components/MouseEvent.hpp"
#include "2D/Color.hpp"

#define BORDER_SPACE 4

class FlagButton : public Button
{
protected:
    FlagID fcode;
    FlagSelectionView* fsv;
public:
    FlagButton(FlagSelectionView* fsv, int x, int y, Surface &s, FlagID flagCode)
        : Button("flag")
    {
        this->fsv = fsv;
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
            fsv->setSelectedFlag(fcode);
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
    moveTo(bodyTextRect.min.x + bodyTextRect.getSizeX() / 2 + 30,
           bodyTextRect.min.y + 5);

    resizeClientArea(bodyTextRect.getSizeX() / 2 - 30, 180);

    int tx = BORDER_SPACE;
    int ty = BORDER_SPACE + (FLAG_HEIGHT - Surface::getFontHeight()) / 2;

    add( new Label(tx, ty, "Current:", windowTextColor, windowTextColorShadow, true) );

    loc_player_flag.x = tx + Surface::getTextLength("Current:") + BORDER_SPACE;
    loc_player_flag.y = BORDER_SPACE;

    init();

} // end FlagSelectionView::FlagSelectionView

// init
//---------------------------------------------------------------------------
void FlagSelectionView::init()
{
    iXY flagStartOffset(BORDER_SPACE, BORDER_SPACE * 2 + FLAG_HEIGHT);

    int yOffset = FLAG_HEIGHT + 2;

    int x = flagStartOffset.x;
    int y = flagStartOffset.y;

    ResourceManager::loadAllFlags(game_flags, flag_names);
    menu_flags.copy(game_flags);
    menu_flags.mapFromPalette("netp");

    unsigned int max_flags = menu_flags.getNumFrames();
    for (unsigned int i = 0; i < max_flags; ++i)
    {
        menu_flags.setFrame(i);
        add( new FlagButton(this, x, y, menu_flags, i) );

        x += FLAG_WIDTH + 2;

        if (x > flagStartOffset.x + getClientRect().getSizeX() - BORDER_SPACE - FLAG_WIDTH)
        {
            x = flagStartOffset.x;
            y += yOffset;
        }
    }

    setSelectedFlag(0);
} // end FlagSelectionView::init

// doDraw
//---------------------------------------------------------------------------
void FlagSelectionView::doDraw(Surface &viewArea, Surface &clientArea)
{
    //iRect r(getViewRect());
    //viewArea.bltLookup(r, Palette::darkGray256.getColorArray());
    //clientArea.drawRect(clientArea.getRect(), Color::white);
    
    menu_flags.blt(clientArea, loc_player_flag.x, loc_player_flag.y);

    View::doDraw(viewArea, clientArea);

} // end FlagSelectionView::doDraw

void
FlagSelectionView::setSelectedFlag(FlagID code)
{
    if ( code < flag_names.size() )
    {
        menu_flags.setFrame(code);
        game_flags.setFrame(code);
        game_flags.frameToBuffer(GameConfig::player_flag_data, sizeof(GameConfig::player_flag_data));
    }
}
