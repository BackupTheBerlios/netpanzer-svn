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
#include "ObjectiveInterface.hpp"

#include <stdio.h>
#include <memory>

#include "MapInterface.hpp"
#include "PlayerInterface.hpp"
#include "Util/FileSystem.hpp"
#include "Outpost.hpp"

#include "ObjectiveNetMessage.hpp"

#include "Server.hpp"

std::vector<Objective*> ObjectiveInterface::objective_list;

unsigned long ObjectiveInterface::objective_position_enum_list_size;
unsigned long ObjectiveInterface::objective_position_enum_index;
PlayerID      ObjectiveInterface::objective_position_enum_player_id;

NetMessageEncoder ObjectiveInterface::message_encoder;

void ObjectiveInterface::cleanUpObjectiveList()
{
    std::vector<Objective*>::iterator i;
    for(i = objective_list.begin(); i != objective_list.end(); i++) {
        delete *i;
    }

    objective_list.clear();
}

void ObjectiveInterface::resetLogic()
{
    cleanUpObjectiveList();
}

void ObjectiveInterface::loadObjectiveList( const char *file_path )
{
    size_t objective_count = 0;
    char comment[64] = "";

    // XXX FIX THIS! Make it using physfs!!!
    FILE* infile = fopen(FileSystem::getRealName(file_path).c_str(), "rt" );
    assert( infile != 0 );

    cleanUpObjectiveList();

    fscanf( infile, "%s %u", comment, &objective_count );

    unsigned long loc_x, loc_y;
    unsigned long world_x, world_y;
    char name[64];
    objective_list.clear();
    for (size_t objective_index = 0; objective_index < objective_count; objective_index++ ) {
        Objective *objective_obj;

        fscanf( infile, "%s %s", comment, name );
        fscanf( infile, "%s %ld %ld", comment, &loc_x, &loc_y );

        MapInterface::mapXYtoPointXY( loc_x, loc_y, &world_x, &world_y );

        objective_obj = new Outpost( objective_index, iXY( world_x, world_y ),
                                     BoundBox( -48, -32, 48, 32 )
                                   );

        strcpy( objective_obj->objective_state.name, name );

        objective_list.push_back(objective_obj);
    } // ** for
}

unsigned char ObjectiveInterface::
quearyObjectiveLocationStatus( iXY &loc,
                               PlayerID &player,
                               Objective **objective_ptr )
{
    std::vector<Objective*>::iterator i;
    for(i = objective_list.begin(); i != objective_list.end(); i++) {
        ObjectiveState *objective_state = & ((*i)->objective_state);

        if ( objective_state->selection_box.contains( loc ) == true ) {
            if ( objective_state->occupation_status == _occupation_status_occupied ) {
                if ( player.getIndex() == objective_state->occupying_player.getIndex() ) {
                    *objective_ptr = *i;
                    return( _player_occupied_objective_found );
                } // ** if
                else {
                    *objective_ptr = *i;
                    return( _enemy_occupied_objective_found );
                } // ** else
            } // ** if
            else {
                *objective_ptr = *i;
                return( _unoccupied_objective_found );
            } // ** else
        } // ** if
    } // ** if

    return( _no_objective_found );
}

void ObjectiveInterface::sendMessage(ObjectiveMessage *message)
{
    objective_list[message->objective_id]->processMessage( message );
}


void ObjectiveInterface::processNetMessages(NetMessage *message)
{
    switch( message->message_id ) {
    case _net_message_id_occupation_status_update : {
            ObjectiveOccupationUpdate *occupation_update;
            occupation_update = (ObjectiveOccupationUpdate *) message;
            sendMessage( &(occupation_update->status_update) );
        }
        break;

    case _net_message_id_objective_sync : {
            ObjectiveSyncMesg *sync_mesg;

            sync_mesg = (ObjectiveSyncMesg *) message;
            sendMessage( &(sync_mesg->sync_data) );
        }
        break;

    } // ** switch

}

void ObjectiveInterface::updateObjectiveStatus()
{
    std::vector<Objective*>::iterator i;
    for(i = objective_list.begin(); i != objective_list.end(); i++) {
        (*i)->updateStatus();
    }
}

void ObjectiveInterface::offloadGraphics( SpriteSorter &sorter )
{
    std::vector<Objective*>::iterator i;
    for(i = objective_list.begin(); i != objective_list.end(); i++) {
        (*i)->offloadGraphics(sorter);
    }
}


bool ObjectiveInterface::
testRuleObjectiveOccupationRatio(unsigned short player_index, float precentage)
{
    size_t list_size = objective_list.size();
    long occupation_ratio;
    long occupied;

    occupied = 0;
    occupation_ratio = (unsigned long) ( ((float) list_size) * precentage );

    if ( occupation_ratio <= 0 )
        occupation_ratio = 1;

    std::vector<Objective*>::iterator i;
    for(i = objective_list.begin(); i != objective_list.end(); i++) {
        ObjectiveState *objective_state = & ((*i)->objective_state);

        if (  objective_state->occupation_status == _occupation_status_occupied ) {
            unsigned short occuping_player_index;
            occuping_player_index = objective_state->occupying_player.getIndex();

            if ( occuping_player_index == player_index ) {
                occupied++;
            } // ** if
            else
                if ( PlayerInterface::isAllied(occuping_player_index,player_index) &&
                        PlayerInterface::isAllied(player_index,occuping_player_index)
                   ) {
                    occupied++;
                }

        } // ** if
    } // ** for


    if ( occupied >= occupation_ratio )
        return ( true );

    return( false );
}

void ObjectiveInterface::disownPlayerObjectives( PlayerID &player )
{
    DisownPlayerObjective disown_player_objective;

    for(size_t i = 0; i < objective_list.size(); i++) {
        disown_player_objective.set(i, player);
        sendMessage(&disown_player_objective);
    }
}

ObjectiveState * ObjectiveInterface::getObjectiveState( short objective_id )
{
    return & (objective_list.at(objective_id)->objective_state);
}

OutpostStatus ObjectiveInterface::getOutpostStatus( short objective_id )
{
    OutpostStatus outpost_status;
    
    Outpost *outpost_ptr 
        = dynamic_cast<Outpost*> (objective_list.at(objective_id));
    assert(outpost_ptr != 0);

    outpost_ptr->getOutpostStatus( outpost_status );

    return outpost_status;
}

void ObjectiveInterface::startObjectivePositionEnumeration()
{
    objective_position_enum_index	= 0;
    objective_position_enum_list_size = objective_list.size();
    objective_position_enum_player_id = PlayerInterface::getLocalPlayerID();
}

bool ObjectiveInterface::objectivePositionEnumeration(iRect *objective_rect,
        unsigned char *objective_disposition, int *objective_id)
{
    ObjectiveState *objective_state;

    if( objective_position_enum_index <  objective_position_enum_list_size ) {
        objective_state = &(objective_list[ objective_position_enum_index ]->objective_state);

        (*objective_rect) = objective_state->area.getAbsRect( objective_state->location );
        if( objective_id != 0 ) {
            (*objective_id) = objective_position_enum_index;
        }

        if ( objective_state->occupation_status == _occupation_status_occupied ) {
            if ( objective_position_enum_player_id.getIndex() != objective_state->occupying_player.getIndex() ) {
                if ( PlayerInterface::isAllied( objective_state->occupying_player , objective_position_enum_player_id )  ) {
                    (*objective_disposition) = _objective_disposition_allie;
                } else {
                    (*objective_disposition) = _objective_disposition_enemy;
                }
            } else {
                (*objective_disposition) = _objective_disposition_player;
            }
        } else {
            (*objective_disposition) = _objective_disposition_unoccupied;
        }

        objective_position_enum_index++;
        return( true );
    } // ** if

    return( false );
}

void ObjectiveInterface::syncObjectives( PlayerID connect_player )
{
    ObjectiveSyncMesg sync_mesg;
    MultiMessage *encode_message;

    message_encoder.resetEncoder();

    std::vector<Objective*>::iterator i;
    for(i = objective_list.begin(); i != objective_list.end(); i++) {
        (*i)->getSyncData( sync_mesg.sync_data );

        while ( message_encoder.encodeMessage( &sync_mesg, sizeof(ObjectiveSyncMesg), &encode_message ) ) {
            SERVER->sendMessage( connect_player, encode_message,
                                 encode_message->realSize(), 0);
            message_encoder.resetEncoder();
        } // ** if
    }

    message_encoder.getEncodeMessage( &encode_message );
    if ( encode_message != 0 ) {
        SERVER->sendMessage( connect_player, encode_message,
                             encode_message->realSize(), 0);
    } // ** if
}

void ObjectiveInterface::updatePlayerObjectiveCounts()
{
    unsigned long player_index, player_count;
    PlayerState *player_state = 0;

    player_count = PlayerInterface::getMaxPlayers();

    for( player_index = 0; player_index < player_count; player_index++ ) {
        player_state = PlayerInterface::getPlayerState( (unsigned short) player_index );
        player_state->setObjectivesHeld( 0 );
    }

    std::vector<Objective*>::iterator i;
    for(i = objective_list.begin(); i != objective_list.end(); i++) {
        ObjectiveState *objective_state = & ((*i)->objective_state);

        if( objective_state->occupation_status == _occupation_status_occupied ) {
            player_state = PlayerInterface::getPlayerState( objective_state->occupying_player);
            player_state->incObjectivesHeld();
        }
    }

}
