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

#include <stdint.h>
#include "Util/Endian.hpp"
#include <string>

typedef std::string NPString;

//#define __TEST_PLAYERID__

typedef int32_t PowerUpID;
#define PowerUpID_toPortable(a) htol32(a)
#define PowerUpID_fromPortable(a) ltoh32(a)

typedef uint16_t ObjectiveID;
#define ObjectiveID_toPortable(a) htol16(a)
#define ObjectiveID_fromPortable(a) ltoh16(a)
#define OBJECTIVE_NONE 0xffff

typedef uint16_t UnitID;
#define UnitID_toPortable(a) htol16(a)
#define UnitID_fromPortable(a) ltoh16(a)

typedef uint8_t FlagID;
#define FLAG_WIDTH 20
#define FLAG_HEIGHT 14

typedef uint8_t TeamID;
#define MIN_TEAM_ID (0)
#define MAX_TEAM_ID (0xfe)
#define INVALID_TEAM_ID (0xff)

typedef uint8_t UnitType;

#ifndef __TEST_PLAYERID__
    typedef uint8_t PlayerID;
    #define MIN_PLAYER_ID (0)
    #define MAX_PLAYER_ID (0xfe)
    #define INVALID_PLAYER_ID (0xff)
#else
    class TestPlayerID
    {
    private:
        friend class PlayerInterface; // for array indexing
        friend class UnitInterface;   // for array indexing
        friend class InfoSocket;      // for string conversion (using int)
        friend class DedicatedGameManager; // for string conversion (using int)
        friend class RankView;        // for height calculation (using int)
        friend class ScriptManager;   // for passing to lua as int
        int c;
        operator unsigned int() { return c; }

    public:
        TestPlayerID() {}
        ~TestPlayerID() {}
        bool operator>=(const TestPlayerID& o) { return c>=o.c; }
        bool operator<(const TestPlayerID& o) { return c<o.c; }
        bool operator!=(const TestPlayerID& o) { return c!=o.c; }
        bool operator==(const TestPlayerID& o) const { return c==o.c; }
        void operator++() { c++; }
        void operator=(const int v) { c = v; }

    }__attribute__((packed));;

    typedef TestPlayerID PlayerID;
    #define MIN_PLAYER_ID (TestPlayerID())
    #define MAX_PLAYER_ID (TestPlayerID())
    #define INVALID_PLAYER_ID (TestPlayerID())

#endif

#ifndef __GNUC__
    #define __attribute__(x) /*nothing*/
#endif

#endif	/* _CORETYPES_HPP */

