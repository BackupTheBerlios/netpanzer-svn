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
#ifndef _POWERUP_HPP
#define _POWERUP_HPP

#include "SpriteSorter.hpp"
#include "PowerUpNetMessage.hpp"

enum { _power_up_lifecycle_state_active,
       _power_up_lifecycle_state_inactive
     };

class PowerUpState
{
public:
    iXY map_loc;
    iXY world_loc;
    int      ID;
    int      type;
    int      life_cycle_state;
};

class PowerUp
{
protected:
    bool isPowerUpHit( UnitID *unit_id );

public:
    PowerUpState powerup_state;

public:
    PowerUp();

    PowerUp(iXY map_loc, int ID, int type);

    PowerUp(iXY map_loc, int type);

    virtual void updateState()
    { }

    virtual void offloadGraphics(SpriteSorter& )
    { }

    virtual void onHit(PowerUpHitMesg* )
    { }
    PowerUp *next;
};
#endif // ** _POWERUP_HPP
