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
#ifndef _UNITSTATE_HPP
#define _UNITSTATE_HPP

#include <string.h>
#include "BoundBox.hpp"
#include "Angle.hpp"

#include "UnitLifecycles.hpp"

enum { _threat_level_all_clear,
       _threat_level_under_attack,
       _threat_level_defending
     };

class UnitState
 {
  public:
   unsigned long var_flags;
   
   unsigned char  unit_type;

   bool        select;
   iXY       location;
   BoundBox       bbox;

   AngleInt       body_angle;
   AngleInt       turret_angle;
   
   unsigned short orientation;
   unsigned short speed_rate;
   unsigned short speed_factor;

   unsigned short reload_time;
   short	      max_hit_points;
   short          hit_points;
   unsigned short damage_factor;
   unsigned long  weapon_range;
   unsigned long  defend_range; 
  
   unsigned short fuel_level;

   unsigned char  threat_level;
   unsigned char  lifecycle_state;
   unsigned char  ai_lifecycle_state;
   
   UnitState( );
   
   inline bool bounds( iXY &loc )
    {
     return( bbox.bounds( location, loc ) );
    } 
 
   inline void operator=( UnitState &unit_state )
    {
     memmove( this, &unit_state, sizeof( UnitState ) );
	}
 
   int percentDamageInt( void );
   float percentDamageFloat( void );
 
 };


#endif
