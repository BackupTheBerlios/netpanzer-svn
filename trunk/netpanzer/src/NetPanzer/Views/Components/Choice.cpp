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

#include <algorithm>
#include "Views/Components/Choice.hpp"
#include "Views/Components/View.hpp"
#include "Views/Components/StateChangedCallback.hpp"
#include "ViewGlobals.hpp"

enum { ChoiceItemHeight = 14 };

//---------------------------------------------------------------------------
void Choice::reset()
{
    index     = 0;
    minWidth  = 0;
    size.x    = 10;
    size.y    = ChoiceItemHeight;
    isOpen    = 0;
    mouseover = 0;
}

//---------------------------------------------------------------------------
void Choice::addItem(const std::string& item)
{
    choiceList.push_back(item);

    int borderSpace = borderSize * 2;

    size.x = std::max((Surface::getTextLength(item) + borderSpace), size.y);
    size.y = ChoiceItemHeight;
}

//---------------------------------------------------------------------------
void Choice::select(const std::string& item)
{
    for(size_t i = 0; i < choiceList.size(); i++) {
        if(choiceList[i] == item) {
            if(index == i)
                return;

            mouseover = index = i;
            onSelectionChanged();
            if(callback)
                callback->stateChanged(this);

            return;
        }
    }
}

//---------------------------------------------------------------------------
void Choice::select(size_t index)
{
    assert(index < choiceList.size());

    if(index == Choice::index)
        return;

    Choice::index = mouseover = index;
    onSelectionChanged();
    if(callback)
        callback->stateChanged(this);
}

//---------------------------------------------------------------------------
void Choice::actionPerformed(const mMouseEvent &me)
{
    if (   (me.getID() == mMouseEvent::MOUSE_EVENT_PRESSED)
        && (me.getModifiers() & InputEvent::BUTTON1_MASK) )
    {
        isOpen = true;
        size.y = choiceList.size() * ChoiceItemHeight;
    }
    else if (  (me.getID() == mMouseEvent::MOUSE_EVENT_DRAGGED)
            && (me.getModifiers() & InputEvent::BUTTON1_MASK) )
    {
        isOpen = true;
        size.y = choiceList.size() * ChoiceItemHeight;

        iRect r(position.x, position.y, size.x, ChoiceItemHeight);

        for (size_t i = 0; i < choiceList.size(); i++)
        {
            // Find the selected item.
            if ( r.contains(me.getPoint()) )
            {
                mouseover = i;
                break;
            }

            r.translate(iXY(0, ChoiceItemHeight));
        }
    }
    else if (  (me.getID() == mMouseEvent::MOUSE_EVENT_CLICKED)
            && (me.getModifiers() & InputEvent::BUTTON1_MASK) )
    {
        isOpen = false;
        size.y = ChoiceItemHeight;
        position.y = real_y;

        // set new element
        if ( mouseover == index )
        {
            return;
        }

        index = mouseover;
        onSelectionChanged();
        if(callback)
            callback->stateChanged(this);
    }
    else if ( me.getID() == mMouseEvent::MOUSE_EVENT_EXITED )
    {
        isOpen = false;
        size.y = ChoiceItemHeight;
        position.y = real_y;
    }
}

// draw
//---------------------------------------------------------------------------
void Choice::draw(Surface &dest)
{
    iRect   r;

    // Draw the label.
    iXY pos;

    if ( isOpen && (position.y == real_y) )
    {
        // Make sure the choice fits on the screen.
        if ( position.y + size.y >= dest.getHeight() )
        {
            // Check to see if it will fit above the base position.
            if ( (position.y + ChoiceItemHeight) - size.y > 0)
            {
                // Since it fits above, put it there.
                position.y -= size.y - ChoiceItemHeight;
            }
            else
            {
                // Since it does not fit above, just put it below and keep it on the screen.
                position.y -= position.y + size.y + 1 - dest.getHeight();
            }
        }
    }


    //pos.x = min.x - strlen((const char *) label) * CHAR_XPIX - CHAR_XPIX;
    pos.x = position.x;
    pos.y = position.y - Surface::getFontHeight() - 4;

    // Draw the name of the choice.
    dest.bltStringShadowed( position.x, real_y - Surface::getFontHeight() - 4,
                            label.c_str(), componentActiveTextColor, Color::black);

    getBounds(r);

    dest.RoundRect(iRect(r.getLocationX(), r.getLocationY(), r.getWidth() - 2, r.getHeight() - 2), 4, Color::gray96);
    dest.RoundRect(iRect(r.getLocationX()+1, r.getLocationY()+1, r.getWidth() - 1, r.getHeight() - 1), 4, componentActiveTextColor);
    dest.FillRoundRect(iRect(r.getLocationX()+1, r.getLocationY()+1, r.getWidth() - 2, r.getHeight() - 2), 4, componentBodyColor);

    if ( !isOpen )
    {
        dest.bltStringShadowed( r.getLocationX() + ((r.getWidth() - dest.getTextLength(choiceList[index])) / 2),
                                r.getLocationY() + ((r.getHeight() - dest.getFontHeight()) / 2),
                                choiceList[index].c_str(),
                                componentActiveTextColor, Color::black);
    }
    else
    {
        r.setHeight( ChoiceItemHeight );

        size_t count = choiceList.size();

        for (size_t i = 0; i < count; i++)
        {
            if ( i == mouseover )
            {
                // Higlight the selected item.
                dest.fillRect(r, componentActiveTextColor);
                dest.bltString( r.getLocationX() + ((r.getWidth() - dest.getTextLength(choiceList[i])) / 2),
                                r.getLocationY() + ((r.getHeight() - dest.getFontHeight()) / 2),
                                choiceList[i].c_str(), Color::black);

            }
            else
            {
                dest.bltStringShadowed( r.getLocationX() + ((r.getWidth() - dest.getTextLength(choiceList[i])) / 2),
                                        r.getLocationY() + ((r.getHeight() - dest.getFontHeight()) / 2),
                                        choiceList[i].c_str(), componentActiveTextColor, Color::black);
            }

            r.translate(iXY(0, ChoiceItemHeight));
        }
    }
    //isOpen = 0;
} // end Choice::draw

// setMinWidth
//---------------------------------------------------------------------------
void Choice::setMinWidth(int minWidth)
{
    Choice::size.x   = minWidth;
    Choice::minWidth = minWidth;

} // end Choice::setMinWidth
