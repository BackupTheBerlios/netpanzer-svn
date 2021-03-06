/*
Copyright (C) 2008 by Aaron Perez <aaronps@gmail.com>

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

 * Created on September 4, 2008, 11:43 PM
 */

#ifndef _CORETYPES_HPP
#define	_CORETYPES_HPP

// SDL.h included for the basic types [U|S]int[8|16|32]
#include "SDL.h"
#include "Util/Endian.hpp"

typedef Sint32 PowerUpID;
#define PowerUpID_toPortable(a) htol32(a)
#define PowerUpID_fromPortable(a) ltoh32(a)

typedef Uint16 ObjectiveID;
#define ObjectiveID_toPortable(a) htol16(a)
#define ObjectiveID_fromPortable(a) ltoh16(a)
#define OBJECTIVE_NONE 0xffff

typedef Uint16 UnitID;

typedef Uint8 FlagID;

#endif	/* _CORETYPES_HPP */

