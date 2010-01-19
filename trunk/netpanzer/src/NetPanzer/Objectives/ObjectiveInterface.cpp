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

#include "Objectives/ObjectiveInterface.hpp"

#include <stdio.h>
#include <memory>
#include <string>
#include <sstream>
#include <stdexcept>
#include <iostream>

#include "Interfaces/MapInterface.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "Util/FileSystem.hpp"
#include "Util/Exception.hpp"
#include "Util/FileStream.hpp"
#include "Util/Log.hpp"
#include "Objectives/Outpost.hpp"
#include "Interfaces/GameConfig.hpp"

#include "Classes/Network/ObjectiveNetMessage.hpp"
#include "Classes/Network/TerminalNetMesg.hpp"
#include "Classes/Network/NetPacket.hpp"
#include "Network/ClientSocket.hpp"

std::vector<Objective*> ObjectiveInterface::objective_list;

unsigned long ObjectiveInterface::objective_position_enum_list_size;
ObjectiveID   ObjectiveInterface::objective_position_enum_index;
Uint16        ObjectiveInterface::objective_position_enum_player_id;

NetMessageEncoder ObjectiveInterface::message_encoder;

SDL_mutex* ObjectiveInterface::mutex = 0;

void
ObjectiveInterface::cleanUpObjectiveList()
{
    std::vector<Objective*>::iterator i;
    for(i = objective_list.begin(); i != objective_list.end(); i++) {
        delete *i;
    }

    objective_list.clear();
}

void
ObjectiveInterface::cleanUp()
{
    cleanUpObjectiveList();
}

void
ObjectiveInterface::resetLogic()
{
    cleanUpObjectiveList();
}

static inline std::string readToken(std::istream& in, std::string tokenname)
{
    if(in.eof())
        throw std::runtime_error("file too short.");
    
    // skip whitespace characters and comments
    char c;
    do {
        in.get(c);
        if(c == '#') {  // comment till end of line
            do {
                in.get(c);
            } while(!in.eof() && c != '\n');
        }
    } while(!in.eof() && isspace(c));

    if(in.eof())
        throw std::runtime_error("file too short.");

    // read token
    std::string token;
    while(!in.eof() && !isspace(c)) {
        token += c;
        in.get(c);
    }
    if(token != tokenname) {
        std::stringstream msg;
        msg << "Expected token '" << tokenname << "' got '" << token << "'.";
        throw std::runtime_error(msg.str());
    }

    while(!in.eof() && isspace(c))
        in.get(c);

    // read token contents
    std::string result;
    while(!in.eof() && c != '\n') {
        result += c;
        in.get(c);
    }

    return result;
}

void
ObjectiveInterface::loadObjectiveList(const char *file_path)
{
    ObjectiveID objective_count = 0;

    try
    {
        IFileStream in(file_path);

        cleanUpObjectiveList();

        std::string objectivecount = readToken(in, "ObjectiveCount:");
        std::stringstream ss(objectivecount);
        ss >> objective_count;

        size_t loc_x, loc_y;
        size_t world_x, world_y;
        std::string name;
        cleanUpObjectiveList();
        for (ObjectiveID objective_index = 0; objective_index < objective_count; objective_index++ )
        {
            Objective *objective_obj;
       
            name = readToken(in, "Name:");
            std::string location = readToken(in, "Location:");
            std::stringstream ss(location);
            ss >> loc_x >> loc_y;
            
            MapInterface::mapXYtoPointXY( loc_x, loc_y, &world_x, &world_y );

            objective_obj = new Outpost(objective_index, iXY(world_x, world_y),
                    BoundBox( -48, -32, 48, 32 )
                    );
            
            strcpy(objective_obj->objective_state.name, name.c_str());
            objective_list.push_back(objective_obj);
        } // ** for
    }
    catch(std::exception& e)
    {
        throw Exception("Error while reading outpost definition '%s': %s",
                file_path, e.what());
    }
}

unsigned char
ObjectiveInterface::quearyObjectiveLocationStatus(iXY &loc,
        Uint16 player_id, Objective **objective_ptr )
{
    std::vector<Objective*>::iterator i;
    for(i = objective_list.begin(); i != objective_list.end(); i++) {
        ObjectiveState *objective_state = & ((*i)->objective_state);

        if ( objective_state->selection_box.contains( loc ) == true ) {
            if ( objective_state->occupation_status == _occupation_status_occupied ) {
                if (objective_state->occupying_player->getID() == player_id) {
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

void
ObjectiveInterface::processTerminalNetPacket(const NetPacket* packet)
{
    const NetMessage* message = packet->getNetMessage();
    const ObjectiveMessage* objectiveMessage = 0;
    switch(message->message_id) {
        case _net_message_id_term_unit_gen: {
            const TerminalOutpostUnitGenRequest* request =
                (const TerminalOutpostUnitGenRequest*) message;
            objectiveMessage = &(request->unit_gen_request);
            break;
        }
        case _net_message_id_term_output_loc: {
            const TerminalOutpostOutputLocRequest* request =
                (const TerminalOutpostOutputLocRequest*) message;
            objectiveMessage = &(request->output_loc_request);
            break;
        }
        default:
            LOGGER.warning(
                    "Unknown objective terminal message (id %u, player %u)",
                    message->message_id, packet->fromPlayer);
            return;
    }                                       

    const PlayerState* player 
        = PlayerInterface::getPlayer(packet->fromPlayer);
    if(!player) {
        LOGGER.warning("Couldn't find player for packet %u.",
                packet->fromPlayer);
        return;
    }
    
    sendMessage(objectiveMessage, player);
}

void
ObjectiveInterface::sendMessage(const ObjectiveMessage* message,
        const PlayerState* player)
{
    if(message->getObjectiveID() >= objective_list.size()) {
        LOGGER.warning("Received malformed objective message: id out of range");
        return;
    }
    Objective* objective = objective_list[message->getObjectiveID()];
    if(player && objective->objective_state.occupying_player != player) {
        LOGGER.warning(
                "Terminal message from player %u for objective %u not allowed.",
                player->getID(), message->getObjectiveID());
        return;
    }
    objective_list[message->getObjectiveID()]->processMessage(message);
}

void
ObjectiveInterface::processNetMessages(const NetMessage* message)
{
    switch(message->message_id) {
        case _net_message_id_occupation_status_update:
        {
            const ObjectiveOccupationUpdate *occupation_update
                = (const ObjectiveOccupationUpdate*) message;
            sendMessage( &(occupation_update->status_update) );
            break;
        }

        case _net_message_id_objective_sync:
        {
            const ObjectiveSyncMesg* sync_mesg 
                = (const ObjectiveSyncMesg*) message;
            sendMessage( &(sync_mesg->sync_data) );
            break;
        }
        
        default:
            LOGGER.warning("Unknown Objective Message received (id %d)",
                    message->message_id);
            break;
    }
}

void
ObjectiveInterface::updateObjectiveStatus()
{
    std::vector<Objective*>::iterator i;
    for(i = objective_list.begin(); i != objective_list.end(); i++) {
        (*i)->updateStatus();
    }
}

void
ObjectiveInterface::offloadGraphics( SpriteSorter &sorter )
{
    std::vector<Objective*>::iterator i;
    for(i = objective_list.begin(); i != objective_list.end(); i++) {
        (*i)->offloadGraphics(sorter);
    }
}


bool
ObjectiveInterface::testRuleObjectiveOccupationRatio(
        Uint16 player_index, float precentage)
{
    size_t occupation_ratio = (size_t)
        ( ((float) objective_list.size()) * precentage  + 0.999);

    if (occupation_ratio == 0)
        occupation_ratio = 1;

    std::vector<Objective*>::iterator i;
    size_t occupied = 0;
    for(i = objective_list.begin(); i != objective_list.end(); i++) {
        ObjectiveState *objective_state = & ((*i)->objective_state);
        if(objective_state->occupation_status == _occupation_status_occupied) {
            Uint16 occuping_player_index 
                = objective_state->occupying_player->getID();

            if ( occuping_player_index == player_index ) {
                occupied++;
            } else if (PlayerInterface::isAllied(
                        occuping_player_index, player_index) &&
                        PlayerInterface::isAllied(player_index,
                            occuping_player_index)) {
                    occupied++;
            }
        }
    }

    if(occupied >= occupation_ratio)
        return true;

    return false;
}

void
ObjectiveInterface::disownPlayerObjectives(Uint16 player_id)
{
    DisownPlayerObjective disown_player_objective;

    for(Uint16 i = 0; i < objective_list.size(); i++) {
        disown_player_objective.set(i, player_id);
        sendMessage(&disown_player_objective);
    }
}

ObjectiveState*
ObjectiveInterface::getObjectiveState( ObjectiveID objective_id )
{
    return & (objective_list.at(objective_id)->objective_state);
}

OutpostStatus
ObjectiveInterface::getOutpostStatus( ObjectiveID objective_id )
{
    OutpostStatus outpost_status;
    
    Outpost *outpost_ptr 
        = dynamic_cast<Outpost*> (objective_list.at(objective_id));
    assert(outpost_ptr != 0);

    outpost_ptr->getOutpostStatus( outpost_status );

    return outpost_status;
}

void
ObjectiveInterface::startObjectivePositionEnumeration()
{
    objective_position_enum_index	= 0;
    objective_position_enum_list_size = objective_list.size();
    objective_position_enum_player_id = PlayerInterface::getLocalPlayerIndex();
}

bool
ObjectiveInterface::objectivePositionEnumeration(iRect *objective_rect,
        unsigned char *objective_disposition, ObjectiveID *objective_id)
{
    ObjectiveState *objective_state;

    if( objective_position_enum_index <  objective_position_enum_list_size )
    {
        objective_state = &(objective_list[ objective_position_enum_index ]->objective_state);

        (*objective_rect) = objective_state->area.getAbsRect( objective_state->location );
        if( objective_id != 0 )
        {
            (*objective_id) = objective_position_enum_index;
        }

        if ( objective_state->occupation_status == _occupation_status_occupied )
        {
            if ( objective_position_enum_player_id != objective_state->occupying_player->getID() )
            {
                if (PlayerInterface::isAllied(objective_state->occupying_player->getID() , objective_position_enum_player_id)  )
                {
                    (*objective_disposition) = _objective_disposition_allie;
                }
                else
                {
                    (*objective_disposition) = _objective_disposition_enemy;
                }
            }
            else
            {
                (*objective_disposition) = _objective_disposition_player;
            }
        }
        else
        {
            (*objective_disposition) = _objective_disposition_unoccupied;
        }

        objective_position_enum_index++;
        return true;
    }

    return false;
}

void
ObjectiveInterface::syncObjectives( ClientSocket * client )
{
    unsigned char buffer[_MAX_NET_PACKET_SIZE];
    unsigned int buffer_pos = 0;
    ObjectiveSyncMesg msg;
    msg.setSize(sizeof(ObjectiveSyncMesg));
    std::vector<Objective*>::iterator i;
    for(i = objective_list.begin(); i != objective_list.end(); ++i)
    {
        if ( buffer_pos+sizeof(ObjectiveSyncMesg) > sizeof(buffer) )
        {
            client->sendMessage(buffer,buffer_pos);
            buffer_pos=0;
        }
        
        (*i)->getSyncData( msg.sync_data );
        memcpy(buffer+buffer_pos, &msg, sizeof(ObjectiveSyncMesg));
        buffer_pos += sizeof(ObjectiveSyncMesg);
    }

    client->sendMessage(buffer,buffer_pos);
}

int
ObjectiveInterface::getObjectiveLimit()
{
    float percentage
        = (float) gameconfig->objectiveoccupationpercentage / 100.0;
    int wincount = (int) ( ((float) getObjectiveCount()) * percentage + 0.999);
    if(wincount == 0)
        wincount = 1;

    return wincount;
}

