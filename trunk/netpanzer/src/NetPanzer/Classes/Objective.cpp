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
#include "stdafx.hpp"
#include "Objective.hpp"
#include "PlayerInterface.hpp"
#include "ConsoleInterface.hpp"

Objective::Objective( short ID, PointXYi location, BoundBox area )
 {
  objective_state.selection_state = _FALSE;
  objective_state.ID = ID;
  objective_state.location = location;
  objective_state.capture_area = area;
  objective_state.objective_status = _objective_status_null;
  objective_state.occupation_status = _occupation_status_unoccupied;
 }

void Objective::objectiveMesgUpdateOccupation( ObjectiveMessage *message )
 {
  UpdateOccupationsStatus *occupation_update;
  
  occupation_update = (UpdateOccupationsStatus *) message;

  objective_state.occupation_status = occupation_update->occupation_status;
  objective_state.occupying_player = occupation_update->occupying_player;

  if( objective_state.occupation_status != _occupation_status_unoccupied )
   {
    PlayerState *player_state;
    player_state = PlayerInterface::getPlayerState( objective_state.occupying_player );
 
    ConsoleInterface::postMessage( "%s has been occupied by %s", objective_state.name, player_state->getName() );
   }
 } 

void Objective::objectiveMesgSync( ObjectiveMessage *message )
 {
  SyncObjective *sync_mesg;
  
  sync_mesg = (SyncObjective *) message;

  objective_state.objective_status = sync_mesg->objective_status;
  objective_state.occupation_status = sync_mesg->occupation_status;
  objective_state.occupying_player = sync_mesg->occupying_player;
 
 } 


void Objective::getSyncData( SyncObjective &objective_sync_mesg )
 {
  objective_sync_mesg.set( objective_state.ID,
                           objective_state.objective_status,
                           objective_state.occupation_status,
                           objective_state.occupying_player
                         );
 }


void Objective::processMessage( ObjectiveMessage *message )
 {
  switch( message->message_type )
   {
    case _objective_mesg_update_occupation :
     objectiveMesgUpdateOccupation( message );
    break;   
   
    case _objective_mesg_sync_objective :
     objectiveMesgSync( message );
    break;
     
   
   }  

 }
