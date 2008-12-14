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

#include "SDL.h"

#include "Objectives/Objective.hpp"
#include "ArrayUtil/ArrayTemplate.hpp"

#include "Classes/Network/NetPacket.hpp"
#include "Classes/Network/NetMessageEncoder.hpp"

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

class NetPacket;

class ObjectiveInterface
{
protected:
    static std::vector<Objective*> objective_list;

    static void cleanUpObjectiveList();

    static SDL_mutex* mutex;

public:
    static void cleanUp();

    static void resetLogic();

    static void loadObjectiveList( const char *file_path );

    static unsigned char quearyObjectiveLocationStatus(iXY &loc,
            Uint16 player_id, Objective **objective_ptr);

    static void processTerminalNetPacket(const NetPacket* packet);
    static void sendMessage(const ObjectiveMessage* message,
            const PlayerState* player = 0);

    static void processNetMessages(const NetMessage* message);

    static void updateObjectiveStatus();

    static void offloadGraphics( SpriteSorter &sorter );

    static bool testRuleObjectiveOccupationRatio(Uint16 player_index,
            float precentage );

    static void disownPlayerObjectives(Uint16 player_id);

    static ObjectiveState * getObjectiveState( ObjectiveID objective_id );

    static OutpostStatus getOutpostStatus( ObjectiveID objective_id );

    static size_t getObjectiveCount()
    {
        return objective_list.size();
    }

    static int getObjectiveLimit();

protected:
    static NetMessageEncoder message_encoder;

public:
    static void syncObjectives( ClientSocket * client );

    // Objective positions, almost exclusivly for mini map
protected:
    static ObjectiveID   objective_position_enum_index;
    static unsigned long objective_position_enum_list_size;
    static Uint16        objective_position_enum_player_id;

public:
    static void startObjectivePositionEnumeration(Uint16 playerid);
    static bool objectivePositionEnumeration(iRect *objective_rect, unsigned char *objective_disposition, ObjectiveID *objective_id);
};


#endif //  ** _OBJECTIVEINTERFACE_HPP
