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
#include "Choice.hpp"
#include "Views/Components/ViewGlobals.hpp"

#include "2D/Color.hpp"
#include "2D/TextRenderingSystem.hpp"

#include "GameInput/InputManager.hpp"
#include "2D/ComponentEvents.hpp"
#include "Util/Log.hpp"


Choice::~Choice()
{
    choice_renders.deleteAll();
}

//---------------------------------------------------------------------------
void Choice::reset()
{
    index     = 0;
    minWidth  = 0;
    rect.setSize( TextRenderingSystem::line_height(), // begins as square
                  TextRenderingSystem::line_height());
    hovering  = false;
    selecting = false;
    mouseover = 0;
}

//---------------------------------------------------------------------------
void Choice::addItem(const std::string& item)
{
    choiceList.push_back(item);

    TextRenderer *t = new TextRenderer(item.c_str());
    choice_renders.push_back(t);

    int borderSpace = 12 * 2; // was border size

    rect.setSize(std::max( (t->getWidth() + borderSpace), rect.getWidth()),
                 TextRenderingSystem::line_height() );
}

//---------------------------------------------------------------------------
void Choice::select(size_t index)
{
    assert(index < choiceList.size());

    if(index == Choice::index)
        return;

    Choice::index = mouseover = index;
    onSelectionChanged();
}

// draw
//---------------------------------------------------------------------------
void Choice::draw(Surface &dest) const
{
    iRect   r(rect);

    dest.RoundRect(iRect(r.getLocationX(), r.getLocationY(), r.getWidth() - 2, r.getHeight() - 2), 4, Color::gray96);
    dest.RoundRect(iRect(r.getLocationX()+1, r.getLocationY()+1, r.getWidth() - 1, r.getHeight() - 1), 4, componentActiveTextColor);
    dest.FillRoundRect(iRect(r.getLocationX()+1, r.getLocationY()+1, r.getWidth() - 2, r.getHeight() - 2), 4, componentBodyColor);

    const TextRenderer *t;

    int width = r.getWidth();

    if ( !selecting )
    {
        t = choice_renders[index];

        t->draw(dest, r.getLocationX() + ((width - t->getWidth()) / 2),
                      r.getLocationY(),
                      componentActiveTextColor );
    }
    else
    {
        r.setHeight( TextRenderingSystem::line_height() );

        size_t count = choiceList.size();

        for (size_t i = 0; i < count; i++)
        {
            t = choice_renders[i];
            PIX color = componentActiveTextColor;
            if ( i == mouseover )
            {
                // Higlight the selected item.
                dest.fillRect(r, componentActiveTextColor);
                color = Color::black;
            }

            t->draw(dest, r.getLocationX() + ((width - t->getWidth()) / 2),
                          r.getLocationY(),
                          color );

            r.translate(iXY(0, TextRenderingSystem::line_height()));
        }
    }
    //isOpen = 0;
} // end Choice::draw

void Choice::logic()
{
    // @todo fix/redo the choice fit test
//    if ( selecting && (rect.getLocationY() == real_y) )
//    {
//        // Make sure the choice fits on the screen.
//        if ( static_cast<unsigned>(rect.getEndY()) >= dest.getHeight() )
//        {
//            // Check to see if it will fit above the base position.
//            if ( (rect.getLocationY() + TextRenderingSystem::line_height()) - rect.getHeight() > 0)
//            {
//                // Since it fits above, put it there.
//                rect.translate(0,-(rect.getHeight() - TextRenderingSystem::line_height()));
//            }
//            else
//            {
//                // Since it does not fit above, just put it below and keep it on the screen.
//                rect.translate(0,-(rect.getEndY() + 1 - dest.getHeight()));
//            }
//        }
//    }
}

// setMinWidth
//---------------------------------------------------------------------------
void Choice::setMinWidth(int minWidth)
{
    rect.setWidth(minWidth);
    Choice::minWidth = minWidth;

} // end Choice::setMinWidth

void Choice::onHoverStart()
{
    hovering = true;
}

void Choice::onHoverStop()
{
    hovering = false;
}

void Choice::onSelectStart()
{
    selecting = true;
    rect.setHeight(choiceList.size() * TextRenderingSystem::line_height());
}

void Choice::onSelectStop()
{
    selecting = false;
    rect.setHeight(TextRenderingSystem::line_height());
    rect.setLocationY(real_y);

    if ( hovering )
    {
        if ( mouseover != index )
        {
            index = mouseover;
            onSelectionChanged();
            events->push( changeEvent );
            LOGGER.warning("Selected element %d", mouseover);
        }
    }
}

void Choice::handleInput(GameInput::InputState* input)
{
    if ( selecting && hovering )
    {
        const int my = GameInput::InputManager::getMouseY();
        const size_t lipos = (my - rect.getLocationY()) / TextRenderingSystem::line_height();
        
        if ( lipos < choiceList.size() )
        {
            mouseover = lipos;
        }
    }
}
