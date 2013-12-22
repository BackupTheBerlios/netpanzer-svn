/*
Copyright (C) 2011 by Aaron Perez <aaronps@gmail.com>

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

#include "GFlagSelectionView.hpp"
#include "Classes/ScreenSurface.hpp"

#include "Interfaces/PlayerInterface.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Classes/Network/NetworkClient.hpp"
#include "Classes/WorldInputCmdProcessor.hpp"

#include "Views/GameViewGlobals.hpp"
#include "Resources/ResourceManager.hpp"
#include "2D/Components/Button.hpp"
#include "2D/Components/Label.hpp"

#include "2D/Palette.hpp"
#include "Network/PlayerRequests/ChangeFlagRequest.hpp"

#include <vector>
#include <string>

#define BORDER_SPACE 4

class GFlagButton : public Button
{
public:
    GFlagButton( int x, int y, Surface* s )
    {
        setLocation(x, y);
        setSize(s->getWidth(), s->getHeight());
        background_images.push_back(s);

        setExtraBorder();
        borders[1] = Color::red;
        borders[2] = Color::green;
    }

//    void actionPerformed( const mMouseEvent &e)
//    {
//        if ( e.getID() == mMouseEvent::MOUSE_EVENT_CLICKED )
//        {
//            background_images[0]->frameToBuffer(GameConfig::player_flag_data,
//                                                sizeof(GameConfig::player_flag_data));
//
//            ChangeFlagRequest req;
//            memcpy(req.player_flag,
//                   GameConfig::player_flag_data,
//                   sizeof(GameConfig::player_flag_data));
//
//            CLIENT->sendMessage(&req, sizeof(req));
//
////            Desktop::setVisibility("GFlagSelectionView", false);
//            COMMAND_PROCESSOR.Flagtimer.reset();
////            PlayerInterface::getLocalPlayer()->setStateActive();
//        }
//        else
//        {
//            Button::actionPerformed(e);
//        }
//    }
};

GFlagSelectionView::GFlagSelectionView() : View()
{
    setSearchName("GFlagSelectionView");

    setAllowMove(false);

    loaded = false;
}

void GFlagSelectionView::init()
{
    removeComponents();

    iRect viewrect = getClientRect();

    rect.setLocation( (viewrect.getWidth()/2) - 250,
                      (viewrect.getHeight()/2) - 250);
    rect.setSize(500,500);

    int tx = rect.getLocationX() + 20;
    int ty = rect.getLocationY() + 14 + (FLAG_HEIGHT - Surface::getFontHeight()) / 2;
    add( new Label(tx, ty, "Current:", windowTextColor) );

    loc_player_flag.x = tx + Surface::getTextLength("Current:") + BORDER_SPACE;
    loc_player_flag.y = rect.getLocationY() + 14;

    iXY flagStartOffset(rect.getLocationX() + 14, rect.getLocationY() + 14*3);

    int yOffset = FLAG_HEIGHT + 8;

    unsigned x = flagStartOffset.x;
    int y = flagStartOffset.y;

    PtrArray<Surface> game_flags;

    ResourceManager::loadAllFlags(game_flags);

    for (unsigned int i = 0; i < game_flags.size(); ++i)
    {
        add( new GFlagButton(x, y, game_flags[i]) );

        x += FLAG_WIDTH + 8;

        if ( x > flagStartOffset.x + rect.getWidth() - 20 - FLAG_WIDTH)
        {
            x = flagStartOffset.x;
            y += yOffset;
        }
    }
    loaded = true;
}

void GFlagSelectionView::doDraw( Surface& dest )
{
//    dest.BltRoundRect(rect, 14, Palette::filterDarkGray());
    dest.RoundRect(rect,14, Color::gray);

    ResourceManager::getFlag(PlayerInterface::getLocalPlayerIndex())->blt(dest, loc_player_flag.x, loc_player_flag.y); // full blit

    View::doDraw( dest );
} // end doDraw

void GFlagSelectionView::doActivate()
{
    if ( ! loaded )
    {
        init();
    }
}

void GFlagSelectionView::doDeactivate()
{
    if ( ! isVisible() )
    {
//        removeComponents();
        loaded = false;
    }
}

void
GFlagSelectionView::onDesktopResized( const iXY& oldResolution, const iXY& newResolution)
{
    resize(iXY(newResolution.x, newResolution.y));
    moveTo(iXY(0,0));
}

void GFlagSelectionView::processEvents()
{
    COMMAND_PROCESSOR.process(false);
}
