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


#include <string.h>
#include "SDL_endian.h"
#include "ConnectNetMessage.hpp"
#include "Resources/ResourceManager.hpp"

ClientConnectJoinRequest::ClientConnectJoinRequest()
{
    message_class = _net_message_class_connect;
    message_id = _net_message_id_connect_join_game_request;
}

Uint32 ClientConnectJoinRequest::getProtocolVersion() const
{
    return SDL_SwapLE32(protocol_version);
}

void ClientConnectJoinRequest::setProtocolVersion(Uint32 version)
{
    protocol_version = SDL_SwapLE32(version);
}

ClientConnectJoinRequestAck::ClientConnectJoinRequestAck()
{
    message_class = _net_message_class_connect;
    message_id = _net_message_id_connect_join_game_request_ack;
}

Sint32 ClientConnectJoinRequestAck::getResultCode() const
{
    return SDL_SwapLE32(result_code);
}

void ClientConnectJoinRequestAck::setResultCode(Sint32 result)
{
    result_code = SDL_SwapLE32(result);
}

Uint32 ClientConnectJoinRequestAck::getServerProtocolVersion() const
{
    return SDL_SwapLE32(server_protocol_version);
}

void ClientConnectJoinRequestAck::setServerProtocolVersion(Uint32 protocol_version)
{
    server_protocol_version = SDL_SwapLE32(protocol_version);
}


ConnectProcessUpdate::ConnectProcessUpdate()
{
    message_class = _net_message_class_connect;
    message_id = _net_message_id_client_connect_process_update;
}

Uint32 ConnectProcessUpdate::getQueuePosition() const
{
    return SDL_SwapLE32(queue_position);
}

void ConnectProcessUpdate::setQueuePosition(Uint32 position)
{
    queue_position = SDL_SwapLE32(position);
}


ConnectProcessStateMessage::ConnectProcessStateMessage()
{
    message_class = _net_message_class_connect;
    message_id = _net_message_id_client_connect_process_state_mesg;
}

Uint32 ConnectProcessStateMessage::getMessageEnum() const
{
    return SDL_SwapLE32(message_enum);
}

void ConnectProcessStateMessage::setMessageEnum(Uint32 message)
{
    message_enum = SDL_SwapLE32(message);
}

Sint32 ConnectProcessStateMessage::getPercentComplete() const
{
    return SDL_SwapLE32(percent_complete);
}

void ConnectProcessStateMessage::setPercentComplete(Sint32 percent)
{
    percent_complete = SDL_SwapLE32(percent);
}


ConnectClientSettings::ConnectClientSettings()
{
    message_class = _net_message_class_connect;
    message_id = _net_message_id_connect_client_settings;
    memset(&player_name, 0, sizeof(player_name));
}

void
ConnectClientSettings::set( const char *player_name, Uint8 unit_color,
                            const char * flagname )
{
    strncpy(ConnectClientSettings::player_name, player_name, 64);
    ConnectClientSettings::player_name[63] = '\0';
    ConnectClientSettings::unit_color = unit_color;
    ResourceManager::getFlagData(flagname, (Uint8*)&flagdata);
}


ConnectMesgServerGameSettings::ConnectMesgServerGameSettings()
{
    message_class = _net_message_class_connect;
    message_id = _net_message_id_connect_server_game_setup;
    memset(map_name, 0, sizeof(map_name));
}

Uint16 ConnectMesgServerGameSettings::getMaxPlayers() const
{
    return SDL_SwapLE16(max_players);
}

void ConnectMesgServerGameSettings::setMaxPlayers(Uint16 maxPlayers)
{
    max_players = SDL_SwapLE16(maxPlayers);
}

Uint16 ConnectMesgServerGameSettings::getMaxUnits() const
{
    return SDL_SwapLE16(max_units);
}

void ConnectMesgServerGameSettings::setMaxUnits(Uint16 maxUnits)
{
    max_units = SDL_SwapLE16(maxUnits);
}

Sint32 ConnectMesgServerGameSettings::getCloudCoverage() const
{
    return SDL_SwapLE32(cloud_coverage);
}

void ConnectMesgServerGameSettings::setCloudCoverage(Sint32 cloudCoverage)
{
    cloud_coverage = SDL_SwapLE32(cloudCoverage);
}

float ConnectMesgServerGameSettings::getWindSpeed() const
{
    return (float)SDL_SwapLE32((Uint32)wind_speed);
}

void ConnectMesgServerGameSettings::setWindSpeed(float windSpeed)
{
    wind_speed = (float)SDL_SwapLE32((Uint32)windSpeed);
}

Sint32 ConnectMesgServerGameSettings::getGameType() const
{
    return SDL_SwapLE32(game_type);
}

void ConnectMesgServerGameSettings::setGameType(Sint32 gameType)
{
    game_type = SDL_SwapLE32(gameType);
}

Sint32 ConnectMesgServerGameSettings::getFragLimit() const
{
    return SDL_SwapLE32(frag_limit);
}

void ConnectMesgServerGameSettings::setFragLimit(Sint32 fragLimit)
{
    frag_limit = SDL_SwapLE32(fragLimit);
}

Sint32 ConnectMesgServerGameSettings::getTimeLimit() const
{
    return SDL_SwapLE32(time_limit);
}

void ConnectMesgServerGameSettings::setTimeLimit(Sint32 timeLimit)
{
    time_limit = SDL_SwapLE32(timeLimit);
}

time_t ConnectMesgServerGameSettings::getElapsedTime() const
{
    return (time_t)SDL_SwapLE32((Uint32)elapsed_time);
}

void ConnectMesgServerGameSettings::setElapsedTime(time_t elapsedTime)
{
    elapsed_time = (time_t)SDL_SwapLE32((Uint32)elapsedTime);
}

