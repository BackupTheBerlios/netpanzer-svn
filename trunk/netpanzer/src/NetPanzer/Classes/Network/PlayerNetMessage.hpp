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

#include "NetMessage.hpp"

enum { _net_message_id_player_connect_id,
       _net_message_id_player_sync_state,
       _net_message_id_player_score_update,
       _net_message_id_player_alliance_request,
       _net_message_id_player_alliance_update,
     };

#ifdef MSVC
#pragma pack(1)
#endif

class PlayerConnectID : public NetMessage
{
public:
    NetworkPlayerState connect_state;

    PlayerConnectID(NetworkPlayerState state)
        : connect_state(state)
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

    PlayerStateSync(NetworkPlayerState state)
        : player_state(state)
    {
        message_class = _net_message_class_player;
        message_id = _net_message_id_player_sync_state;
    }
} __attribute__((packed));

class PlayerScoreUpdate : public NetMessage
{
private:
    uint16_t kill_by_player_index;
    uint16_t kill_on_player_index;
public:
    uint8_t  unit_type;

    PlayerScoreUpdate()
    {
        message_class = _net_message_class_player;
        message_id = _net_message_id_player_score_update;
    }

    void set(uint16_t kill_by_index, uint16_t kill_on_index, uint8_t unit_type)
    {
        kill_by_player_index = htol16(kill_by_index);
        kill_on_player_index = htol16(kill_on_index);
        this->unit_type = unit_type;
    }

    void setKillByPlayerIndex(uint16_t kill_by_index)
    {
        kill_by_player_index = htol16(kill_by_index);
    }
    uint16_t getKillByPlayerIndex() const
    {
        return ltoh16(kill_by_player_index);
    }

    void setKillOnPlayerIndex(uint16_t kill_on_index)
    {
        kill_on_player_index = htol16(kill_on_index);
    }
    uint16_t getKillOnPlayerIndex() const
    {
        return htol16(kill_on_player_index);
    }
}
__attribute__((packed));


enum { _player_make_alliance,
       _player_break_alliance
     };

class PlayerAllianceRequest : public NetMessage
{
private:
    uint16_t allie_by_player_index;
    uint16_t allie_with_player_index;
public:
    uint8_t  alliance_request_type;

    PlayerAllianceRequest()
    {
        message_class = _net_message_class_player;
        message_id = _net_message_id_player_alliance_request;
    }

    void set(uint16_t allie_by_player_index, uint16_t allie_with_player_index,
            uint8_t alliance_request_type)
    {
        this->allie_by_player_index = htol16(allie_by_player_index);
        this->allie_with_player_index = htol16(allie_with_player_index);
        this->alliance_request_type = alliance_request_type;
    }
    uint16_t getAllieByPlayerIndex() const
    {
        return ltoh16(allie_by_player_index);
    }
    uint16_t getAllieWithPlayerIndex() const
    {
        return ltoh16(allie_with_player_index);
    }
}
__attribute__((packed));


class PlayerAllianceUpdate : public NetMessage
{
private:
    uint16_t allie_by_player_index;
    uint16_t allie_with_player_index;
public:
    uint8_t  alliance_update_type;

    PlayerAllianceUpdate()
    {
        message_class = _net_message_class_player;
        message_id = _net_message_id_player_alliance_update;
    }

    void set(uint16_t by_player_index, uint16_t with_player_index,
            uint8_t update_type )
    {
        allie_by_player_index = htol16(by_player_index);
        allie_with_player_index = htol16(with_player_index);
        alliance_update_type = update_type;
    }

    uint16_t getAllieByPlayerIndex() const
    {
        return ltoh16(allie_by_player_index);
    }
    uint16_t getAllieWithPlayerIndex() const
    {
        return ltoh16(allie_with_player_index);
    }
}
__attribute__((packed));

#ifdef MSVC
#pragma pack()
#endif

#endif // ** _PLAYERNETMESSAGE_HPP
