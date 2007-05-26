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
#ifndef _CONNECTNETMESSAGE_HPP
#define _CONNECTNETMESSAGE_HPP

#include <time.h>
#include "NetMessage.hpp"
#include "PlayerState.hpp"

enum { _net_message_id_connect_join_game_request,
       _net_message_id_connect_join_game_request_ack,
       _net_message_id_client_start_connect,
       _net_message_id_client_connect_request,
       _net_message_id_client_connect_result,
       _net_message_id_connect_client_settings,
       _net_message_id_client_connect_process_update,
       _net_message_id_client_connect_process_state_mesg,
       _net_message_id_connect_server_game_setup,
       _net_message_id_connect_client_game_setup_ping,
       _net_message_id_connect_client_game_setup_ack,
       _net_message_id_connect_client_abort,
       _net_message_id_connect_netPanzer_client_disconnect,
       _net_message_id_connect_netPanzer_server_disconnect
     };

#ifdef MSVC
#pragma pack(1)
#endif

class ClientConnectJoinRequest : public NetMessage
{
private:
    Uint32 protocol_version;
    
public:
    ClientConnectJoinRequest();
    Uint32 getProtocolVersion() const;
    void setProtocolVersion(Uint32 version);
} __attribute__((packed));

enum { _join_request_result_success,
       _join_request_result_invalid_protocol,
       _join_request_result_server_busy
     };

class ClientConnectJoinRequestAck : public NetMessage
{
private:
    Sint32  result_code;
    Uint32 server_protocol_version;

public:
    ClientConnectJoinRequestAck();

    Sint32 getResultCode() const;
    void setResultCode(Sint32 result);
    Uint32 getServerProtocolVersion() const;
    void setServerProtocolVersion(Uint32 protocol_version);
} __attribute__((packed));

class ClientConnectStartConnect : public NetMessage
{
public:
    ClientConnectStartConnect()
    {
        message_class = _net_message_class_connect;
        message_id = _net_message_id_client_start_connect;
    }
} __attribute__((packed));

class ClientConnectRequest : public NetMessage
{
public:
    ClientConnectRequest()
    {
        message_class = _net_message_class_connect;
        message_id = _net_message_id_client_connect_request;
    }
} __attribute__((packed));

enum { _connect_result_success,
       _connect_result_server_busy,
       _connect_result_server_full
     };

class ClientConnectResult : public NetMessage
{
public:
    Uint8 result_code;

    ClientConnectResult()
    {
        message_class = _net_message_class_connect;
        message_id = _net_message_id_client_connect_result;
    }
} __attribute__((packed));


class ConnectProcessUpdate : public NetMessage
{
private:
    Uint32 queue_position;

public:
    ConnectProcessUpdate();
    Uint32 getQueuePosition() const;
    void setQueuePosition(Uint32 position);
} __attribute__((packed));

enum { _connect_state_message_load_game_data,
       _connect_state_message_sync_player_info,
       _connect_state_message_sync_player_info_percent,
       _connect_state_message_sync_units,
       _connect_state_message_sync_units_percent,
       _connect_state_sync_complete
     };

class ConnectProcessStateMessage : public NetMessage
{
private:
    Uint32 message_enum;
    Sint32  percent_complete;

public:
    ConnectProcessStateMessage();
    Uint32 getMessageEnum() const;
    void setMessageEnum(Uint32 message);
    Sint32 getPercentComplete() const;
    void setPercentComplete(Sint32 percent);
} __attribute__((packed));

class ConnectClientSettings : public NetMessage
{
public:
    char player_name[64];
private:
    Sint16 player_flag;
public:
    Uint8 unit_color;

    ConnectClientSettings();
    Sint16 getPlayerFlag() const;
    void set(const char *player_name, Uint8 unit_color,
             Uint16 player_flag );
} __attribute__((packed));

class ConnectMesgServerGameSettings : public NetMessage
{
private:
    Uint16 max_players;
    Uint16 max_units;
public:
    char     map_name[32];
private:
    Sint32  cloud_coverage;
    float    wind_speed;
    Sint32  game_type;
public:
    Uint8  powerup_state;
private:
    Sint32  frag_limit;
    Sint32  time_limit;
    time_t   elapsed_time;

public:
    ConnectMesgServerGameSettings();
    Uint16 getMaxPlayers() const;
    void setMaxPlayers(Uint16 maxPlayers);
    Uint16 getMaxUnits() const;
    void setMaxUnits(Uint16 maxUnits);
    Sint32 getCloudCoverage() const;
    void setCloudCoverage(Sint32 cloudCoverage);
    float getWindSpeed() const;
    void setWindSpeed(float windSpeed);
    Sint32 getGameType() const;
    void setGameType(Sint32 gameType);
    Uint8 getPowerupState() const;
    void setPowerupState(Uint8 powerupState);
    Sint32 getFragLimit() const;
    void setFragLimit(Sint32 fragLimit);
    Sint32 getTimeLimit() const;
    void setTimeLimit(Sint32 timeLimit);
    time_t getElapsedTime() const;
    void setElapsedTime(time_t elapsedTime);
} __attribute__((packed));

class ConnectMesgClientGameSetupAck : public NetMessage
{
public:
    ConnectMesgClientGameSetupAck()
    {
        message_class = _net_message_class_connect;
        message_id = _net_message_id_connect_client_game_setup_ack;
    }
} __attribute__((packed));

class ConnectMesgClientGameSetupPing : public NetMessage
{
public:
    ConnectMesgClientGameSetupPing()
    {
        message_class = _net_message_class_connect;
        message_id = _net_message_id_connect_client_game_setup_ping;
    }
} __attribute__((packed));

class ConnectMesgNetPanzerClientDisconnect : public NetMessage
{
private:
    Uint16 player_id;

public:
    ConnectMesgNetPanzerClientDisconnect()
    {
        message_class = _net_message_class_connect;
        message_id = _net_message_id_connect_netPanzer_client_disconnect;
    }

    void setPlayerID(Uint16 id)
    {
        player_id = htol16(id);
    }
    Uint16 getPlayerID() const
    {
        return ltoh16(player_id);
    }
} __attribute__((packed));

class ConnectMesgNetPanzerServerDisconnect : public NetMessage
{
public:
    ConnectMesgNetPanzerServerDisconnect()
    {
        message_class = _net_message_class_connect;
        message_id = _net_message_id_connect_netPanzer_server_disconnect;
    }
} __attribute__((packed));

#ifdef MSVC
#pragma pack()
#endif

#endif // ** _CONNECTNETMESSAGE_HPP
