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
#include "PlayerState.hpp"

//If you modify this array, also modify the constant above
uint8_t *playerColorArray[] = {
	&Color::red,
	&Color::green,
	&Color::orange,
	&Color::brown,
	&Color::yellow,
	&Color::cyan,
	&Color::gray,
	&Color::magenta,
	&Color::pink,
	&Color::unitAqua,
	&Color::unitYellow,
	&Color::unitRed,
	&Color::unitBlue,
	&Color::unitDarkBlue,
	&Color::unitLightGreen,
	&Color::unitGreen,
	&Color::unitBlueGray,
	&Color::unitDarkRed,
	&Color::unitBlack,
	&Color::unitDarkGreen,
	&Color::unitWhite,
	&Color::unitLightOrange,
	&Color::unitOrange,
	&Color::unitGray,
	&Color::unitDarkGray,
	&Color::tan,
	&Color::chartreuse,
	&Color::cobaltGreen,
	&Color::emeraldGreen,
	&Color::forestGreen,
	&Color::darkOliveGreen,
	&Color::terreVerte,
	&Color::darkBlue,
	&Color::darkBrown,
	&Color::darkCyan,
	&Color::darkGray,
	&Color::darkGreen,
	&Color::darkMagenta,
	&Color::darkOrange,
	&Color::darkPink,
	&Color::darkRed,
	&Color::darkYellow,
	&Color::lightBlue,
	&Color::lightBrown,
	&Color::lightCyan,
	&Color::lightGray,
	&Color::lightGreen,
	&Color::lightMagenta,
	&Color::lightOrange,
	&Color::lightPink,
	&Color::lightRed,
	&Color::lightYellow,
	&Color::gray32,
	&Color::gray64,
	&Color::gray96,
	&Color::gray128,
	&Color::gray160,
	&Color::gray192,
	&Color::gray224
};

static const size_t playerColorCount 
    = sizeof(playerColorArray) / sizeof(uint8_t*);

void PlayerState::setColor( uint32_t index ) {
	colorIndex = index % playerColorCount;
}

uint8_t PlayerState::getColor() {
    assert(colorIndex < playerColorCount);
    return ( *playerColorArray[ colorIndex ] );
}

