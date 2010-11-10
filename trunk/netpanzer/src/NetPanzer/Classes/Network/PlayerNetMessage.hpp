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
#ifndef _PLAYERNETMESSAGE_HPP
#define _PLAYERNETMESSAGE_HPP

#include "Core/CoreTypes.hpp"
#include "NetMessage.hpp"

enum { _net_message_id_player_connect_id,
       _net_message_id_player_sync_state,
       _net_message_id_player_score_update
     };

#ifdef MSVC
#pragma pack(1)
#endif

class PlayerConnectID : public NetMessage
{
public:
    PlayerID player_id;

    PlayerConnectID(PlayerID id)
        : player_id(id)
    {
        message_class = _net_message_class_player;
        message_id = _net_message_id_player_connect_id;
    }
}
__attribute__((packed));


class PlayerStateSync : public NetMessage
{
public:
    NetworkPlayerState player_state;
    
    PlayerStateSync()
    {
        message_class = _net_message_class_player;
        message_id = _net_message_id_player_sync_state;
    }
    
    PlayerStateSync(NetworkPlayerState state)
        : player_state(state)
    {
        message_class = _net_message_class_player;
        message_id = _net_message_id_player_sync_state;
    }
}
__attribute__((packed));

class PlayerScoreUpdate : public NetMessage
{
private:
    PlayerID kill_by_player_index;
    PlayerID kill_on_player_index;
public:
    Uint8  unit_type;

    PlayerScoreUpdate()
    {
        message_class = _net_message_class_player;
        message_id = _net_message_id_player_score_update;
    }

    void set(PlayerID kill_by_index, PlayerID kill_on_index, Uint8 unit_type)
    {
        kill_by_player_index = kill_by_index;
        kill_on_player_index = kill_on_index;
        this->unit_type = unit_type;
    }

    void setKillByPlayerIndex(PlayerID kill_by_index)
    {
        kill_by_player_index = kill_by_index;
    }
    PlayerID getKillByPlayerIndex() const
    {
        return kill_by_player_index;
    }

    void setKillOnPlayerIndex(PlayerID kill_on_index)
    {
        kill_on_player_index = kill_on_index;
    }
    PlayerID getKillOnPlayerIndex() const
    {
        return kill_on_player_index;
    }
}
__attribute__((packed));

#ifdef MSVC
#pragma pack()
#endif

#endif // ** _PLAYERNETMESSAGE_HPP
