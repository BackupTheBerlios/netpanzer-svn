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
#ifndef _PROJECTILEINTERFACE_HPP
#define _PROJECTILEINTERFACE_HPP

#include <list>

#include "Classes/Projectile.hpp"
#include "Classes/Weapons/Weapon.hpp"

class ProjectileInterface
{
protected:
    static std::list<Projectile*> projectiles;

    static void removeAll();

public:
    static void resetLogic();

    static void newProjectile( unsigned short projectile_type,
                               unsigned short owner_type_id,
                               UnitID &owner, unsigned short damage,
                               iXY &start, iXY &end );

    static void updateStatus();

    static void offloadGraphics( SpriteSorter &sorter );
};

#endif
