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
#include "Component.hpp"
#include "ViewGlobals.hpp"
#include "View.hpp"

int Component::borderSize = 12;

// reset
//---------------------------------------------------------------------------
void Component::reset()
{
	foreground = gForeground;
	background = gBackground;
	size.zero();
	min.zero();
	enabled    = true;
	visible    = true;
	parent     = 0;

	//name = strdup("");
	//if (name = 0)
	//{
	//	FUBAR("ERROR: Unable to allocate component name.");
	//}
} // end Component::reset

// contains
//---------------------------------------------------------------------------
bool Component::contains(int x, int y) const
{
	return (x >= min.x &&
			x < min.x + size.x &&
			y >= min.y &&
			y < min.y + size.y);

} // end Component::contains

//---------------------------------------------------------------------------
void Component::setParent(void *parent)
{
	assert(parent != 0);
	if (parent == 0)
	{
		FUBAR("ERROR: Component parent == 0");
	}

	Component::parent = parent;
}

// setEnabled
//---------------------------------------------------------------------------
void Component::setEnabled(bool enabled)
{
	Component::enabled = enabled;

} // end Component::setEnabled

// setForeground
//---------------------------------------------------------------------------
void Component::setForeground(PIX foreground)
{
	Component::foreground = foreground;

} // end Component::setForeground

// setLocation
//---------------------------------------------------------------------------
void Component::setLocation(int x, int y)
{
	min.x = x;
	min.y = y;

} // end Component::setLocation
