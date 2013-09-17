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

#include "CheckBox.hpp"
#include "2D/Color.hpp"
#include "Views/Components/ViewGlobals.hpp"
#include "2D/TextRenderingSystem.hpp"
#include "2D/ComponentEvents.hpp"

CheckBox::CheckBox(const NPString& label, bool state, const int changeEvent)
    : Component(), state(state)
{
    setLabel(label);
    textColor = componentActiveTextColor;
    this->changeEvent = changeEvent;
    selecting = false;
}

void CheckBox::setLabel(const NPString& label)
{
    CheckBox::label = label;
    label_render.setText(label.c_str());
    setSize( label_render.getWidth() + TextRenderingSystem::line_height(), TextRenderingSystem::line_height());
}

void CheckBox::draw(Surface& s) const
{
    iRect r(rect.getLocation(), iXY(TextRenderingSystem::line_height(), TextRenderingSystem::line_height()));
    r.grow(-4, -4);

    s.FillRoundRect( r,2,  componentBodyColor);
    s.RoundRect( r, 2, textColor);

    label_render.draw(s, rect.getLocationX() + rect.getHeight(), rect.getLocationY(), textColor);

    if ( state )
    {
        r.grow(-3, -3);
        s.fillRect( r, textColor);
    }
}

void CheckBox::onSelectStart()
{
    selecting = true;
    textColor = Color::gray96;
}

void CheckBox::onSelectStop()
{
    if ( textColor == Color::gray96 )
    {
        state = !state;
        onStateChanged();
        events->push(changeEvent);
        textColor = componentFocusTextColor;
    }
    else
    {
        textColor = componentActiveTextColor;
    }
    selecting = false;
}

void CheckBox::onHoverStart()
{
    textColor = selecting ? Color::gray96 : componentFocusTextColor;
}

void CheckBox::onHoverStop()
{
    textColor = componentActiveTextColor;
}
