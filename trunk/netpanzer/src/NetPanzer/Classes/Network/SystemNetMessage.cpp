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
#include <config.h>

#include "SystemNetMessage.hpp"

SystemSetPlayerView::SystemSetPlayerView(int32_t x, int32_t y)
{
    message_class = _net_message_class_system;
    message_id = _net_message_id_system_set_view;
    camera_loc_x = htol32(x);
    camera_loc_y = htol32(y);
}

int32_t SystemSetPlayerView::getCameraLocX() const
{
    return ltoh32(camera_loc_x);
}

int32_t SystemSetPlayerView::getCameraLocY() const
{
    return ltoh32(camera_loc_y);
}


SystemResetGameLogic::SystemResetGameLogic()
{
    message_class = _net_message_class_system;
    message_id = _net_message_id_system_reset_game_logic;
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

uint16_t SystemPingRequest::getClientPlayerIndex() const
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

uint16_t SystemConnectAlert::getPlayerID() const
{
    return ltoh16(player_id);
}
