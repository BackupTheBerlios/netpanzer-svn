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

    return strcmp(e1->getName(), e2->getName());

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
    //setDisplayStatusBar(true);
    setVisible(false);

    moveTo(bodyTextRect.min.x, bodyTextRect.min.y + 190);
    resizeClientArea(bodyTextRect.getSizeX(), 158);
    //moveTo(68, 204);
    //resize(610 - 68, 161);

    //setScrollBar(true);

    viewableItemCount =  0;
    highlightedItem   = -1;
    selectedItem      = -1;

    maxYOffset = 0;

    // Define the scrollBar fot this view.
    //scrollBar = new ScrollBar(VERTICAL, 0, 1, 0, 100);

    //add(scrollBar);

    maxViewableItems = getClientRect().getSizeY() / (TEXT_GAP_SPACE + Surface::getFontHeight()) - 1;
    topViewableItem  = 0;

    //iXY size(20, 20);
    //iXY pos(getClientRect().getSizeX() - size.x, 0);

    //upButton.setLabel("+");
    //upButton.setBounds(iRect(pos, pos + size));
    //add(&upButton);
    //
    //pos = iXY(getClientRect().getSizeX() - size.x, getClientRect().getSizeY() - size.y);
    //downButton.setLabel("-");
    //downButton.setBounds(iRect(pos, pos + size));
    //add(&downButton);

} // end GetSessionHostView constructor

// doDraw
//---------------------------------------------------------------------------
void GetSessionHostView::doDraw(Surface &viewArea, Surface &clientArea)
{
    static float curGroupTime = 0.0f;

    //iRect r(getViewRect());
    //viewArea.bltLookup(r, Palette::darkGray256.getColorArray());

    //start winsock hack
    if ((curGroupTime += TimerInterface::getTimeSlice()) > 5.0f) {
        updateHostList();
        curGroupTime = 0.0f;
    }


    drawHostList(clientArea);

    //end winsock hack

    //if (hosts.size() > 0)
    //{
    //	clientArea.bltStringCenter(hosts[0].getName(), Color::white);
    //}

    //char strBuf[256];
    //sprintf(strBuf, "%d", selectedItem);
    //clientArea.bltStringCenter(strBuf, Color::white);

    View::doDraw(viewArea, clientArea);

} // end GetSessionHostView::doDraw

// drawHostList
//---------------------------------------------------------------------------
void GetSessionHostView::drawHostList(Surface &dest)
{
    char host_info_str[256];
    int curIndex = 0;
    int color;

    Surface tempSurface( getClientRect().getSizeX() - 2, (TEXT_GAP_SPACE + Surface::getFontHeight()), getClientRect().getSizeX() - 2, 1 );

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
    //for (int i = topViewableItem; i < topViewableItem + maxViewableItems; i++)

    /*
    	int i;
     
    	int shit = 0;
    	int color;
    	for (i = scrollBar.getValue(); i < scrollBar.getValue() + viewableItemCount; i++)
    	{
    		iXY offset;
    		offset.x = 2;
    		offset.y = 2 + shit * (CHAR_YPIX + ITEM_GAP_SPACE);
    		
    		// Get a color for the item.
    		if (i == selectedItem)
    		{
    			color = Color::green;
    		} else
    		if (i == highlightedItem)
    		{
    			color = Color::red;
    		} else
    		{
    			color = Color::white;
    		}
    		
    		// Display the name of this shape.
    		dest.bltString(offset.x, offset.y, hosts[i].getName(), color);
     
    		shit++;
    	}
    */
} // end GetSessionHostView::drawHostList

// updateHostList
//---------------------------------------------------------------------------
void GetSessionHostView::updateHostList()
{
    int num_games;
    //char game_name[80];
    // XXX
    // HRESULT hr;

    SessionList session_list;
    // winsock hack
    /*
    //bug#15+
    hr =	EnumerateGames( gapp.hwndApp );

    if(hr == DPERR_USERCANCEL)
     {
    //WE NEED TO BACK OUT TO SELECTSESSIONTYPEVIEW if this
        //happens
    Desktop::setVisibilityAllWindows(false);
    Desktop::setVisibility("GetSessionView", true);
     }

    if((hr == DPERR_EXCEPTION) || (hr == DPERR_GENERIC))
     {
     //WE NEED TO TELL THEM THAT A WEIRD DPLAY ERROR OCCURRED
     //AND BUMP THEM OUT TO HOST OR JOIN MENU
    Desktop::toggleVisibility("DirectPlayErrorExceptionView");
     }
    	
    num_games = GetNumGames();
    */

    Desktop::setVisibilityAllWindows(false);
    Desktop::setVisibility("GetSessionView", true);

    num_games = session_list.getSize();


    // Hack to allow only 10 sessions.
    if (num_games > 10) {
        num_games = 10;
    }

    char curHostName[256];
    if (hosts.size() > 0 && selectedItem != -1 && selectedItem < (int) hosts.size()) {
        sprintf(curHostName, "%s", hosts[selectedItem].getName());
    } else {
        strcpy(curHostName, "");
    }

    hosts.clear();

    for (int num = 0; num < num_games; num++) {
        hosts.resize(hosts.size() + 1);
        sprintf(hosts[hosts.size() - 1].name, "%s", session_list[ num ].name );
        sprintf(hosts[hosts.size() - 1].map, "%s", session_list[ num ].map );
        sprintf(hosts[hosts.size() - 1].game_type, "%s", session_list[ num ].game_type );
        hosts[hosts.size() - 1].current_players = session_list[ num ].current_players;
        hosts[hosts.size() - 1].max_players = session_list[ num ].max_players;

    }

    // See if the host name still exists, if so, remap the index to selectedItem.
    selectedItem = -1;

    for (int num = 0; num < num_games; num++) {
        if (strcasecmp(hosts[num].getName(), curHostName) == 0) {
            selectedItem = num;
        }
    }

    //int maxViewableItems = getClientRect().getSize().y / (CHAR_YPIX + ITEM_GAP_SPACE);
    //
    //viewableItemCount = (hosts.size() < maxViewableItems) ? hosts.size() : maxViewableItems;
    /*
    	scrollBar.setViewableAmount(viewableItemCount);
    	scrollBar.setItemCount(hosts.size());
    	scrollBar.setItemsSize(CHAR_YPIX + ITEM_GAP_SPACE);
    */
} // end GetSessionHostView::updateHostList

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
void GetSessionHostView::mouseMove(const iXY & prevPos, const iXY &newPos)
{
    highlightedItem = findItemContaining(newPos);

} // end GetSessionHostView::mouseMove
