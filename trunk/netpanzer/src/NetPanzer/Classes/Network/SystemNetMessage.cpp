/*
Copyright (C) 2004 Hollis Blanchard
 
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

#include "SystemNetMessage.hpp"

SystemSetPlayerView::SystemSetPlayerView(int32_t x, int32_t y)
{
    message_class = _net_message_class_system;
    message_id = _net_message_id_system_set_view;
    camera_loc_x = htol32(x);
    camera_loc_y = htol32(y);
}

int32_t SystemSetPlayerView::getCameraLocX(void)
{
    return ltoh32(camera_loc_x);
}

int32_t SystemSetPlayerView::getCameraLocY(void)
{
    return ltoh32(camera_loc_y);
}


SystemResetGameLogic::SystemResetGameLogic()
{
    message_class = _net_message_class_system;
    message_id = _net_message_id_system_reset_game_logic;
}


SystemChatMesgRequest::SystemChatMesgRequest( )
{
    reset();
}

void SystemChatMesgRequest::reset( void )
{
    message_class = _net_message_class_system;
    message_id = _net_message_id_system_chat_mesg_req;
    memset( player_set, 0, sizeof(player_set));
    message_scope = _chat_mesg_scope_all;
}

void SystemChatMesgRequest::setMessageScope( unsigned char scope )
{
    message_scope = scope;
}

void SystemChatMesgRequest::setPlayerSet( unsigned short player_index )
{
    unsigned long index;
    unsigned char shift;
    unsigned char mask = 1;

    index = ( player_index );
    shift = (unsigned char) ( 7 - (index & (unsigned long) 7 ) ); // 7 - (index % 8)
    index = index >> 3;                                           // index / 8
    mask = mask << shift;

    player_set[index] = player_set[ index ] | mask;
}

void SystemChatMesgRequest::clearPlayerSet( unsigned short player_index )
{
    unsigned long index;
    unsigned char shift;
    unsigned char mask = 1;

    index = (player_index);
    shift = (unsigned char) ( 7 - (index & (unsigned long) 7 ) );
    index = index >> 3;
    mask = ~(mask << shift);

    player_set[ index ] = player_set[index] & mask;
}

uint16_t SystemChatMesgRequest::getSourcePlayerIndex(void)
{
    return ltoh16(source_player_index);
}


void SystemChatMesgRequest::setSourcePlayerIndex(uint16_t playerIndex)
{
    source_player_index = htol16(playerIndex);
}

SystemChatMesg::SystemChatMesg()
{
    message_class = _net_message_class_system;
    message_id = _net_message_id_system_chat_mesg;
    memset(message_text, 0, sizeof(message_text));
}

uint16_t SystemChatMesg::getSourcePlayerIndex(void)
{
    return ltoh16(source_player_index);
}

void SystemChatMesg::setSourcePlayerIndex(uint16_t playerIndex)
{
    source_player_index = htol16(playerIndex);
}



SystemViewControl::SystemViewControl()
{
    message_class = _net_message_class_system;
    message_id = _net_message_id_system_view_control;
    action_flags = 0;
    memset(view_name, 0, sizeof(view_name));
}

void SystemViewControl::set( char *name, unsigned char flags )
{
    action_flags = flags;
    strcpy( view_name, name );
}


SystemPingRequest::SystemPingRequest(uint16_t playerIndex)
{
    message_class = _net_message_class_system;
    message_id = _net_message_id_system_ping_request;
    client_player_index = htol16(playerIndex);
}

uint16_t SystemPingRequest::getClientPlayerIndex(void)
{
    return ltoh16(client_player_index);
}


SystemPingAcknowledge::SystemPingAcknowledge()
{
    message_class = _net_message_class_system;
    message_id = _net_message_id_system_ping_ack;
}


SystemConnectAlert::SystemConnectAlert()
{
    message_class = _net_message_class_system;
    message_id = _net_message_id_system_connect_alert;
}

void SystemConnectAlert::set(const PlayerID &player, unsigned char alert_type )
{
    player_id = player.getIndex();
    alert_enum = alert_type;
}

uint16_t SystemConnectAlert::getPlayerID(void)
{
    return ltoh16(player_id);
}
