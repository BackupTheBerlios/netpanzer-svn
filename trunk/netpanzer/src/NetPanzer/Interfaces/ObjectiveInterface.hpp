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
#ifndef _OBJECTIVEINTERFACE_HPP
#define _OBJECTIVEINTERFACE_HPP

#include "Objective.hpp"
#include "ArrayTemplate.hpp"

#include "NetPacket.hpp"
#include "NetMessageEncoder.hpp"

typedef ArrayTemplate< Objective * > ObjectiveList;

enum { _no_objective_found,
       _player_occupied_objective_found,
       _enemy_occupied_objective_found,
       _unoccupied_objective_found 
     };

enum { _objective_disposition_unoccupied,
       _objective_disposition_player,
	   _objective_disposition_allie,
	   _objective_disposition_enemy
	 };

class ObjectiveInterface
 {
  protected:
   static ObjectiveList objective_list;

   static void cleanUpObjectiveList( void );
  
  public:

   static void resetLogic( void );
  
   static void loadObjectiveList( char *file_path );
  
   static unsigned char quearyObjectiveLocationStatus( PointXYi &loc, 
                                                       PlayerID &player,
                                                       Objective **objective_ptr );

   static void sendMessage( ObjectiveMessage *message );
   
   static void processNetMessages( NetMessage *message );

   static void updateObjectiveStatus( void );

   static void offloadGraphics( SpriteSorter &sorter );
      
   static bool testRuleObjectiveOccupationRatio( unsigned short player_index, float precentage );

   static void disownPlayerObjectives( PlayerID &player );
    
   static ObjectiveState * getObjectiveState( short objective_id );
 
   static OutpostStatus getOutpostStatus( short objective_id );

   static void updatePlayerObjectiveCounts( void );

  protected:
   static NetMessageEncoder message_encoder;
  
  public:
   static void syncObjectives( PlayerID connect_player );

  // Objective positions, almost exclusivly for mini map
  protected:
   static unsigned long objective_position_enum_index;
   static unsigned long objective_position_enum_list_size;
   static PlayerID      objective_position_enum_player_id;
    
  public:
   static void    startObjectivePositionEnumeration( void );
   static bool objectivePositionEnumeration( Recti *objective_rect, unsigned char *objective_disposition, int *objective_id );
 
 };


#endif //  ** _OBJECTIVEINTERFACE_HPP
