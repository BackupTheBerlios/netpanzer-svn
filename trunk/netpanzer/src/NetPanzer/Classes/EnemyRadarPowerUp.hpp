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
#ifndef _ENEMY_RADAR_POWERUP_HPP
#define _ENEMY_RADAR_POWERUP_HPP

#include "PowerUp.hpp"
#include "Sprite.hpp"

extern SpritePacked ENEMY_RADAR_POWERUP_ANIM;
extern SpritePacked ENEMY_RADAR_POWERUP_ANIM_SHADOW;

class EnemyRadarPowerUp : public PowerUp
 {
  protected:
   SpritePacked enemy_radar_animation;
   SpritePacked enemy_radar_animation_shadow;
  
   void setRadar( UnitID &unit_id );

  public:

   EnemyRadarPowerUp(PointXYi map_loc, int type);
   virtual ~EnemyRadarPowerUp()
   { }

   virtual void updateState( void );

   virtual void offloadGraphics( SpriteSorter &sorter );

   virtual void onHit( PowerUpHitMesg *message  ); 

 };

#endif // ** _ENEMY_RADAR_POWERUP_HPP
