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
#ifndef _POWERUP_INTERFACE_HPP
#define _POWERUP_INTERFACE_HPP

#include <vector>

#include "PowerUp.hpp"
#include "Timer.hpp"
#include "PlayerID.hpp"
#include "NetPacket.hpp"

class PowerUpList : public std::vector<PowerUp*>
{
protected:
    int id_counter;

public:
    PowerUpList();
    ~PowerUpList();

    PowerUp* find(int ID);
};

class PowerUpInterface
{
protected:
    static PowerUpList powerup_list;

    static int powerupids;
    static int power_up_limit;
    static int power_up_regen_time_upper_bound;
    static int power_up_regen_time_lower_bound;

    static Timer regen_timer;

    static void setPowerUpLimits( unsigned long map_size_x, unsigned long map_size_y );

    static void generatePowerUp( void );

    static void netMessagePowerUpCreate( NetMessage *message );
    static void netMessagePowerUpHit( NetMessage *message );

public:

    static void initialize( void );
    static void resetLogic( void );

    static void updateState( void );
    static void offloadGraphics( SpriteSorter &sorter );

    static void processNetMessages( NetMessage *message );

    static void syncPowerUps( PlayerID player_id );
};

#endif // ** _POWERUP_INTERFACE_HPP
