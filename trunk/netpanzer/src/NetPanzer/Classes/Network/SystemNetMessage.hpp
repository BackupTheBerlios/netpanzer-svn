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
#ifndef _SYSTEMNETMESSAGE_HPP
#define _SYSTEMNETMESSAGE_HPP

#include <string.h>

#include "PlayerState.hpp"
#include "NetPacket.hpp"
#include "Types/iXY.hpp"

#ifdef MSVC
#pragma pack(1)
#endif

enum { _net_message_id_system_set_view,
       _net_message_id_system_reset_game_logic,
       _net_message_id_system_chat_mesg_req,
       _net_message_id_system_chat_mesg,
       _net_message_id_system_view_control,
       _net_message_id_system_ping_request,
       _net_message_id_system_ping_ack,
       _net_message_id_system_connect_alert
     };


class SystemSetPlayerView : public NetMessage
{
private:
    int32_t camera_loc_x;
    int32_t camera_loc_y;

public:
    SystemSetPlayerView(int32_t x, int32_t y);
    int32_t getCameraLocX(void);
    int32_t getCameraLocY(void);
}
__attribute__((packed));


class SystemResetGameLogic : public NetMessage
{
public:
    SystemResetGameLogic();
}
__attribute__((packed));

enum { _chat_mesg_scope_player_set,
       _chat_mesg_scope_alliance,
       _chat_mesg_scope_enemies,
       _chat_mesg_scope_all,
       _chat_mesg_scope_server
     };


class SystemChatMesgRequest : public NetMessage
{
public:
    uint8_t message_scope;
    char player_set[32];

private:
    uint16_t source_player_index;
public:
    char message_text[150];

    SystemChatMesgRequest();
    void reset();
    void setMessageScope( unsigned char scope );
    void setPlayerSet( unsigned short player_index );
    void clearPlayerSet( unsigned short player_index );
    uint16_t getSourcePlayerIndex(void);
    void setSourcePlayerIndex(uint16_t playerIndex);
}
__attribute__((packed));


class SystemChatMesg: public NetMessage
{
public:
    unsigned char  message_scope;
private:
    uint16_t source_player_index;
public:
    char message_text[150];

    SystemChatMesg();
    uint16_t getSourcePlayerIndex(void);
    void setSourcePlayerIndex(uint16_t playerIndex);
}
__attribute__((packed));



enum { _view_control_flag_visible_on  = 0x01,
       _view_control_flag_visible_off = 0x02,
       _view_control_flag_center_abs  = 0x04,
       _view_control_flag_close_all   = 0x08
     };

class SystemViewControl : public NetMessage
{
public:
    unsigned char action_flags;
    char view_name[32];

    SystemViewControl();
    void set( char *name, unsigned char flags );
}
__attribute__((packed));


class SystemPingRequest : public NetMessage
{
private:
    uint16_t client_player_index;

public:
    SystemPingRequest(uint16_t playerIndex);
    uint16_t getClientPlayerIndex(void);
}
__attribute__((packed));

class SystemPingAcknowledge : public NetMessage
{
public:
    SystemPingAcknowledge();
}
__attribute__((packed));


enum { _connect_alert_mesg_connect,
       _connect_alert_mesg_disconnect,
       _connect_alert_mesg_client_drop
     };

class SystemConnectAlert : public NetMessage
{
private:
    uint16_t player_id;
public:
    unsigned char alert_enum;

    SystemConnectAlert();
    void set(const PlayerID &player, unsigned char alert_type );
    uint16_t getPlayerID(void);
}
__attribute__((packed));

#ifdef MSVC
#pragma pack()
#endif

#endif // ** _SYSTEMNETMESSAGE_HPP
