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

#include "TitledBorder.hpp"
#include "Surface.hpp"
#include "iRect.hpp"


// draw
//--------------------------------------------------------------------------
void TitledBorder::draw(const Surface &dest)
{
	Surface s;
	iRect   r;

	getBounds(r);

	s.setTo(dest, r);

	s.drawRect(iRect(0, 0, s.getPixX() - 2, s.getPixY() - 2), Color::gray96);
	s.drawRect(iRect(1, 1, s.getPixX() - 1, s.getPixY() - 1), Color::white);
	s.drawRect(iRect(1, 1, s.getPixX() - 2, s.getPixY() - 2), Color::black);

	s.bltString(10, 0, (const char *) name, Color::red);

} // end TitledBorder::draw
