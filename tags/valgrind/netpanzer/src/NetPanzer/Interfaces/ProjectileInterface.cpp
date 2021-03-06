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
#include "ProjectileInterface.hpp"

#include "Sound.hpp"

#include "ParticleInterface.hpp"
#include "MissleWeapon.hpp"
#include "BulletWeapon.hpp"
#include "ShellWeapon.hpp"
#include "WorldViewInterface.hpp"
#include "Math.hpp"


InternalLink * ProjectileInterface::projectile_list_start = 0;
InternalLink * ProjectileInterface::projectile_list_end = 0;

void ProjectileInterface::removeAll( void )
 {
  InternalLink *traversal_ptr;

  traversal_ptr = projectile_list_start;

  while( traversal_ptr != 0 )
   {
    InternalLink *destroy_ptr;

    destroy_ptr = traversal_ptr;

    traversal_ptr = traversal_ptr->next_ptr;

    destroy_ptr->removeFromList( (InternalLink **) &projectile_list_start, (InternalLink **) &projectile_list_end );

	if (destroy_ptr != 0)
	 {
	  delete( (Projectile *) (destroy_ptr) );
	  destroy_ptr = 0;
	 }

   } // ** while 

 }

void ProjectileInterface::resetLogic(void)
 {
  removeAll();
  
  projectile_list_start = 0;
  projectile_list_end = 0;
 }



void ProjectileInterface::newProjectile( unsigned short projectile_type,
                                         unsigned short owner_type_id, 
                                         UnitID &owner, unsigned short damage, 
                                         PointXYi &start, PointXYi &end )
 {
  Projectile *temp;

  if (projectile_type == 0)
  {
	  temp = new Weapon( owner, owner_type_id, damage, start, end );
	  
	  // We should make sure and customize all these.
	  temp->addToList( &projectile_list_start, &projectile_list_end );
	  assert(false);
  }
  else if (projectile_type == 1)
  {
	  // The following code launches 4 missles spaced out evenly like they would be in a
	  // missle bay.
	  PointXYi startPos;
	  PointXYi endPos;

	  const float startRadius = 5.0f;
	  const int endRadius     = 50;

	  fXY direction = Math::unitDirection(start, end);
	  fXY offsetPos;

	  //// West outer
	  endPos    = PointXYi(end.x + (rand() % endRadius << 1) - endRadius, end.y + (rand() % endRadius << 1) - endRadius);
	  offsetPos = Math::unitDirectionWest(direction) * startRadius * 2;
	  startPos = start + iXY(offsetPos.x, offsetPos.y);
	  temp = new MissleWeapon( owner, owner_type_id, damage, startPos, endPos);
	  temp->addToList( &projectile_list_start, &projectile_list_end );

	  ParticleInterface::addMissleLaunchPuff(startPos, direction, owner_type_id);

	  // West inner
	  endPos    = PointXYi(end.x + (rand() % endRadius << 1) - endRadius, end.y + (rand() % endRadius << 1) - endRadius);
	  offsetPos = Math::unitDirectionWest(direction) * startRadius;
	  startPos = start + iXY(offsetPos.x, offsetPos.y);
	  temp = new MissleWeapon( owner, owner_type_id, damage, startPos, endPos);
	  temp->addToList( &projectile_list_start, &projectile_list_end );
	  ParticleInterface::addMissleLaunchPuff(startPos, direction, owner_type_id);

	  // East inner
	  endPos    = PointXYi(end.x + (rand() % endRadius << 1) - endRadius, end.y + (rand() % endRadius << 1) - endRadius);
	  offsetPos = Math::unitDirectionEast(direction) * startRadius;
	  startPos = start + iXY(offsetPos.x, offsetPos.y);
	  temp = new MissleWeapon( owner, owner_type_id, damage, startPos, endPos);
	  temp->addToList( &projectile_list_start, &projectile_list_end );
	  ParticleInterface::addMissleLaunchPuff(startPos, direction, owner_type_id);

	  //// East outer
	  endPos    = PointXYi(end.x + (rand() % endRadius << 1) - endRadius, end.y + (rand() % endRadius << 1) - endRadius);
	  offsetPos = Math::unitDirectionEast(direction) * startRadius * 2;
	  startPos = start + iXY(offsetPos.x, offsetPos.y);
	  temp = new MissleWeapon( owner, owner_type_id, damage, startPos, endPos);
	  temp->addToList( &projectile_list_start, &projectile_list_end );
	  ParticleInterface::addMissleLaunchPuff(startPos, direction, owner_type_id);
  }
  else if (projectile_type == 2)
  {
	  temp = new BulletWeapon( owner, owner_type_id, damage, start, end );
	  temp->addToList( &projectile_list_start, &projectile_list_end );
  }
  else if (projectile_type == 3)
  {
	  temp = new ShellWeapon( owner, owner_type_id, damage, start, end );
	  temp->addToList( &projectile_list_start, &projectile_list_end );
  }
  else if (projectile_type == 4)
  {
	  // The following code launches 4 missles spaced out evenly like they would be in a
	  // missle bay.
	  PointXYi startPos;
	  PointXYi endPos;

	  const float startRadius = 5.0f;
	  const int endRadius     = 30;

	  fXY direction = Math::unitDirection(start, end);
	  fXY offsetPos;

	  //// West outer
	  //endPos    = PointXYi(end.x + (rand() % endRadius << 1) - endRadius, end.y + (rand() % endRadius << 1) - endRadius);
	  //offsetPos = Math::unitDirectionWest(direction) * startRadius * 2;
	  //startPos = start + iXY(offsetPos.x, offsetPos.y);
	  //temp = new MissleWeapon( owner, owner_type_id, damage, startPos, endPos);
	  //temp->addToList( &projectile_list_start, &projectile_list_end );

	  ParticleInterface::addMissleLaunchPuff(startPos, direction, owner_type_id);

	  // West inner
	  endPos    = PointXYi(end.x + (rand() % endRadius << 1) - endRadius, end.y + (rand() % endRadius << 1) - endRadius);
	  offsetPos = Math::unitDirectionWest(direction) * startRadius;
	  startPos = start + iXY(offsetPos.x, offsetPos.y);
	  temp = new MissleWeapon( owner, owner_type_id, damage, startPos, endPos);
	  temp->addToList( &projectile_list_start, &projectile_list_end );
	  ParticleInterface::addMissleLaunchPuff(startPos, direction, owner_type_id);

	  // East inner
	  endPos    = PointXYi(end.x + (rand() % endRadius << 1) - endRadius, end.y + (rand() % endRadius << 1) - endRadius);
	  offsetPos = Math::unitDirectionEast(direction) * startRadius;
	  startPos = start + iXY(offsetPos.x, offsetPos.y);
	  temp = new MissleWeapon( owner, owner_type_id, damage, startPos, endPos);
	  temp->addToList( &projectile_list_start, &projectile_list_end );
	  ParticleInterface::addMissleLaunchPuff(startPos, direction, owner_type_id);

	  //// East outer
	  //endPos    = PointXYi(end.x + (rand() % endRadius << 1) - endRadius, end.y + (rand() % endRadius << 1) - endRadius);
	  //offsetPos = Math::unitDirectionEast(direction) * startRadius * 2;
	  //startPos = start + iXY(offsetPos.x, offsetPos.y);
	  //temp = new MissleWeapon( owner, owner_type_id, damage, startPos, endPos);
	  //temp->addToList( &projectile_list_start, &projectile_list_end );
	  //ParticleInterface::addMissleLaunchPuff(startPos, direction, owner_type_id);
  }
  
  //assert( temp != 0 );

  //temp->addToList( &projectile_list_start, &projectile_list_end );
  
  //SFX
  long distance = WorldViewInterface::getCameraDistance( start );
  sound->PlayAmbientSound( owner_type_id, Sound::_fire_gun, distance );
  sound->PlayAmbientSound( _not_applicable, Sound::_random_battle, distance );
 }



void ProjectileInterface::updateStatus( void )
 {
  InternalLink *traversal_ptr;
  Projectile *projectile_ptr;

  traversal_ptr = projectile_list_start;

  while( traversal_ptr != 0 )
   {
    projectile_ptr =  (Projectile *) traversal_ptr;

    if ( projectile_ptr->lifecycle_status == _lifecycle_weapon_in_active )
     {
      InternalLink *destroy_ptr;
      destroy_ptr = traversal_ptr;
      traversal_ptr = traversal_ptr->next_ptr;
      destroy_ptr->removeFromList( (InternalLink **) &projectile_list_start, (InternalLink **) &projectile_list_end );

	  if (destroy_ptr != 0)
	  {
	 	  delete( (Projectile *) (destroy_ptr) );
	      destroy_ptr = 0;
	  }

     } // ** if 
    else
     {
      projectile_ptr->updateStatus();
      traversal_ptr = (traversal_ptr->next_ptr);

     }  // ** else

   } // ** while 

 }

void ProjectileInterface::offloadGraphics( SpriteSorter &sorter )
 {
  InternalLink *traversal_ptr;
  Projectile *projectile_ptr;

  traversal_ptr = projectile_list_start;

  while( traversal_ptr != 0 )
   {
    projectile_ptr =  (Projectile *) traversal_ptr;

    projectile_ptr->offloadGraphics( sorter );
    
	traversal_ptr = (traversal_ptr->next_ptr);
   } // ** while 

 }
