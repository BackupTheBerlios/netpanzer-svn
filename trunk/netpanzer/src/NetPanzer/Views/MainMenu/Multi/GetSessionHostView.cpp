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

#include "GetSessionHostView.hpp"
#include "Desktop.hpp"
#include "Util/TimerInterface.hpp"
#include "GameViewGlobals.hpp"

#include "Client.hpp"

// cHostCompareName
//---------------------------------------------------------------------------
int cHostCompareName(const void *elem1, const void *elem2)
{
    cHost *e1;
    cHost *e2;

    e1 = (cHost *) elem1;
    e2 = (cHost *) elem2;

    return SDL_strcmp(e1->getName(), e2->getName());

} // end cHostCompareName

// GetSessionHostView
//---------------------------------------------------------------------------
GetSessionHostView::GetSessionHostView() : View()
{
    setSearchName("GetSessionHostView");
    setTitle("Select Session Host");
    setSubTitle("");

    setAllowResize(false);
    setAllowMove(false);
    setVisible(false);

    moveTo(bodyTextRect.min.x, bodyTextRect.min.y + 190);
    resizeClientArea(bodyTextRect.getSizeX(), 158);

    viewableItemCount =  0;
    highlightedItem   = -1;
    selectedItem      = -1;

    maxYOffset = 0;

    maxViewableItems = getClientRect().getSizeY() / (TEXT_GAP_SPACE + Surface::getFontHeight()) - 1;
    topViewableItem  = 0;

} // end GetSessionHostView constructor

// doDraw
//---------------------------------------------------------------------------
void GetSessionHostView::doDraw(Surface &viewArea, Surface &clientArea)
{
    static float curGroupTime = 0.0f;

    if ((curGroupTime += TimerInterface::getTimeSlice()) > 5.0f) {
        curGroupTime = 0.0f;
    }


    drawHostList(clientArea);

    View::doDraw(viewArea, clientArea);

} // end GetSessionHostView::doDraw

// drawHostList
//---------------------------------------------------------------------------
void GetSessionHostView::drawHostList(Surface &dest)
{
    char host_info_str[256];
    int curIndex = 0;
    int color;

    Surface tempSurface( getClientRect().getSizeX() - 2,
                         TEXT_GAP_SPACE + Surface::getFontHeight(),
                         1 );

    tempSurface.fill(Color::darkGreen);
    sprintf( host_info_str, "%-24s %-24s %-12s %10s", "Host",
             "Map",
             "Game Type",
             "Players"   );

    tempSurface.bltString( 4, 1, host_info_str, Color::white );
    tempSurface.blt(dest, 0, 1 );

    curIndex++;

    for (size_t i = 0; i < hosts.size(); i++) {
        sprintf( host_info_str, "%-24s %-24s %-15s %4d/%-4d", hosts[i].name,
                 hosts[i].map,
                 hosts[i].game_type,
                 hosts[i].current_players,
                 hosts[i].max_players  );


        color = Color::white;

        if (i == (size_t) highlightedItem) {
            color = Color::red;
        } else if (i == (size_t) selectedItem) {
            color = Color::green;
        }

        dest.bltString(4, 6 + curIndex * (TEXT_GAP_SPACE + Surface::getFontHeight()), host_info_str, color);

        curIndex++;
    }
} // end GetSessionHostView::drawHostList

// findItemContaining
//---------------------------------------------------------------------------
int GetSessionHostView::findItemContaining(const iXY &pos)
{
    int curIndex = 0;

    int length = getClientRect().getSizeX() - 12;

    for (size_t i = 0; i < hosts.size(); i++) {
        iXY tempPos;

        tempPos.x = 4;
        tempPos.y = 6 + curIndex * (TEXT_GAP_SPACE + Surface::getFontHeight()) + (TEXT_GAP_SPACE + Surface::getFontHeight());

        iRect r(tempPos.x, tempPos.y, tempPos.x + length, tempPos.y + TEXT_GAP_SPACE + Surface::getFontHeight());

        if (r.contains(pos)) {
            highlightedItem = i;

            return i;
        }

        curIndex++;
    }

    return -1;

} // end GetSessionHostView::findItemContaining

// lMouseDown
//---------------------------------------------------------------------------
void GetSessionHostView::lMouseDown(const iXY &pos)
{
    selectedItem = findItemContaining(pos);

    if ((selectedItem != -1) && (selectedItem < (int) hosts.size())) {
        // TODO
    }

} // end GetSessionHostView::lMouseDown

// actionPerformed
//---------------------------------------------------------------------------
void GetSessionHostView::actionPerformed(mMouseEvent me)
{
    if (me.getID() == mMouseEvent::MOUSE_EVENT_EXITED) {
        highlightedItem = -1;
    }

} // end GetSessionHostView::actionPerformed

// mouseMove
//---------------------------------------------------------------------------
void GetSessionHostView::mouseMove(const iXY&, const iXY &newPos)
{
    highlightedItem = findItemContaining(newPos);
} // end GetSessionHostView::mouseMove
