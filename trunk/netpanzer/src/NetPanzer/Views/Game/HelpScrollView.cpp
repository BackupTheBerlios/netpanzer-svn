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

#include "HelpScrollView.hpp"
#include "GameView.hpp"
#include "GameViewGlobals.hpp"
#include "Desktop.hpp"


// HelpScrollView
//---------------------------------------------------------------------------
HelpScrollView::HelpScrollView() : SpecialButtonView()
{
    setSearchName("HelpScrollView");
    setTitle("Help Information");
    setSubTitle("");

    setAllowResize(false);
    setAllowMove(false);

    moveTo(bodyTextRect.min);
    resize(bodyTextRect.getSize());

    insert("Key");
    insert("");
    insert("  LMC = Left Mouse Click");
    insert("  RMC = Right Mouse Click");
    insert("  RMH = Right Mouse Hold");
    insert("");
    insert("");
    insert("Unit Related");
    insert("");
    insert("  LMC                              Selects a unit");
    insert("  LMH                              Makes a bouding box for unit selection");
    insert("  RMC                              De-selects all selected units");
    insert("  LMC + Ctrl                       Directs (force/manual) fire of a unit");
    insert("  LMC + Shift, LMC + Ctrl          Add unit to selected set");
    //insert("  Ctrl + [0..9], Shift + [0..9]    Define a group of units");
    insert("  Shift + [0..9]                   Define a group of units");
    insert("  Alt + [1..0]                     Recall a group of units");
    insert("  [1..0]                           Cycles through the corresponding group of units");
    //insert("  'E'                              Select all units on screen");

    //insert("  LMC + 'T'............................Track the selected unit");
    //insert("  LMC + 'W'............................Set the selected objective's way point");
    //insert("  RMH..................................Fast map scroll");
    //insert("  X....................................Scatter units");
    //insert("  S....................................Stop units");
    //insert("  LMC + Ctrl + Alt.....................Escort unit");
    insert("");
    insert("");
    insert("Game Related");
    insert("");
    insert("  LMC + 'A'                        Allies with the clicked unit's team");
    //insert("  Ctrl + [F1..F5]                  Bookmark world locations");
    //insert("  Alt + [F1..F5]                   Recall bookmarked world locations");
    insert("  F2                               In game menus");
    insert("");
    insert("");
    insert("Outpost Related");
    insert("");
    insert("  'O'                              Cycle through your outposts");
    insert("  Ctrl + RMC on captured outpost   Displays the outpost view");
    insert("  Double RMC on captured outpost   Displays the outpost view");
    //insert("  Alt + RMC on captured outpost........Sets selected outpost's delivery location");
    insert("");
    insert("");
    insert("Chat Related");
    insert("");
    insert("  Enter                            Send message to all");
    insert("  Ctrl + 'A'                       Send message to allies");
    insert("  Ctrl + 'E'                       Send message to enemies");
    insert("");
    insert("");
    insert("View Related");
    insert("");
    insert("  F1                               Help View");
    insert("  F6                               Rank View");
    insert("  F7                               Chat View");
    insert("  F8                               Mini Map View");
    insert("  TAB                              Toolbar");
    insert("  RMH + mouse movement             Moves any view on the screen");
    insert("  RMH + [1 - 5]                    Sets the background drawing mode");
    insert(" ");
    insert(" ");
    insert("Mini Map View Related");
    insert("");
    insert("  RMH + [1 - 7]                    Sets the blending level of the minimap");
    insert("  RMH + [+ or -]                   Scales the size of the minimap");
    //insert("  RMH + 'O'                        Toggles outposts");
    //insert(" ");
    //insert(" ");
    //insert("System Related");
    //insert("");
    //insert("  F9                               BMP screen shot");
    //insert("  Alt + '-'                        Decrease brightness");
    //insert("  Alt + '='                        Increase brightness");

    int CHAR_YPIX = Surface::getFontHeight();
    maxViewableItems = (getClientRect().getSizeY() - (TEXT_GAP_SPACE + CHAR_YPIX)) / (TEXT_GAP_SPACE + CHAR_YPIX) - 1;
    topViewableItem  = 0;

    iXY size(20, 20);
    iXY pos(getClientRect().getSizeX() - size.x, 0);

    upButton.setLabel("+");
    upButton.setBounds(iRect(pos, pos + size));
    add(&upButton);

    pos = iXY(getClientRect().getSizeX() - size.x, getClientRect().getSizeY() - size.y);
    downButton.setLabel("-");
    downButton.setBounds(iRect(pos, pos + size));
    add(&downButton);

} // end HelpScrollView::HelpScrollView

// doDraw
//---------------------------------------------------------------------------
void HelpScrollView::doDraw(Surface &viewArea, Surface &clientArea)
{
    if (Desktop::getVisible("GameView")) {
        bltViewBackground(viewArea);
    }

    drawHelpText(clientArea, 0, 0);

    iXY pos;

    pos.x = 4;
    pos.y = clientArea.getPixY() - Surface::getFontHeight();

    clientArea.bltString(pos, "Note: Use the right mouse button to accomplish fast mouse clicking.", Color::white);

    //char strBuf[256];
    //sprintf(strBuf, "%d", scrollBar->getValue());
    //clientArea.bltStringCenter(strBuf, Color::red);

    View::doDraw(viewArea, clientArea);
} // end HelpScrollView::doDraw

// drawHelpText
//--------------------------------------------------------------------------
void HelpScrollView::drawHelpText(Surface &dest, const int &x, const int &y)
{
    PIX color   = Color::white;

    //if (scrollBar != 0)
    //{
    //	int minView = scrollBar->getValue();
    //	int maxView = minView + scrollBar->getViewableAmount();
    //
    //	if(maxView > scrollBar->getMaximum())
    //	{
    //		maxView = scrollBar->getMaximum();
    //	}
    //
    int curIndex = 0;
    for (int i = topViewableItem; i < topViewableItem + maxViewableItems; i++) {
        dest.bltString(1, 6 + curIndex * (TEXT_GAP_SPACE +
                    Surface::getFontHeight()), text[i].c_str(), color);
        curIndex++;
    }
    //}

} // end HelpScrollView::drawHelpText

// insert
//--------------------------------------------------------------------------
void HelpScrollView::insert(const char *string)
{
    text.push_back(std::string(string));
} // end HelpScrollView::insert

// actionPerformed
//--------------------------------------------------------------------------
void HelpScrollView::actionPerformed(mMouseEvent me)
{
    if (me.getID() == mMouseEvent::MOUSE_EVENT_CLICKED) {
        if (me.getSource(upButton)) {
            if (--topViewableItem < 0) {
                topViewableItem = 0;
            }
        } else if (me.getSource(downButton)) {
            if (++topViewableItem >= (long) text.size() - maxViewableItems) {
                topViewableItem = (long) text.size() - maxViewableItems;
            }
        }
    }

} // end HelpScrollView::actionPerformed

// doActivate
//--------------------------------------------------------------------------
void HelpScrollView::doActivate()
{
    /* empty */
} // end HelpScrollView::doActivate
