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

#include "Projectile.hpp"
#include "Weapon.hpp"


class ProjectileInterface : public PObject
 {
  protected:
   static InternalLink *projectile_list_start;
   static InternalLink *projectile_list_end;

   static void removeAll( void );
  
  public:
   
  static void resetLogic(void);
  
  static void newProjectile( unsigned short projectile_type,
                             unsigned short owner_type_id,
                             UnitID &owner, unsigned short damage, 
                             PointXYi &start, PointXYi &end );

  static void updateStatus( void );

  static void offloadGraphics( SpriteSorter &sorter );

 };

#endif _PROJECTILEINTERFACE_HPP
