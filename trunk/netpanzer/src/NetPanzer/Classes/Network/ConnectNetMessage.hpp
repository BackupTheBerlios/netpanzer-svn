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
#include <stdint.h>
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
    uint32_t protocol_version;
    
public:
    ClientConnectJoinRequest();
    uint32_t getProtocolVersion() const;
    void setProtocolVersion(uint32_t version);
} __attribute__((packed));

enum { _join_request_result_success,
       _join_request_result_invalid_protocol,
       _join_request_result_server_busy
     };

class ClientConnectJoinRequestAck : public NetMessage
{
private:
    int32_t  result_code;
    uint32_t server_protocol_version;

public:
    ClientConnectJoinRequestAck();

    int32_t getResultCode() const;
    void setResultCode(int32_t result);
    uint32_t getServerProtocolVersion() const;
    void setServerProtocolVersion(uint32_t protocol_version);
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
    uint8_t result_code;

    ClientConnectResult()
    {
        message_class = _net_message_class_connect;
        message_id = _net_message_id_client_connect_result;
    }
} __attribute__((packed));


class ConnectProcessUpdate : public NetMessage
{
private:
    uint32_t queue_position;

public:
    ConnectProcessUpdate();
    uint32_t getQueuePosition() const;
    void setQueuePosition(uint32_t position);
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
    uint32_t message_enum;
    int32_t  percent_complete;

public:
    ConnectProcessStateMessage();
    uint32_t getMessageEnum() const;
    void setMessageEnum(uint32_t message);
    int32_t getPercentComplete() const;
    void setPercentComplete(int32_t percent);
} __attribute__((packed));

class ConnectClientSettings : public NetMessage
{
public:
    char player_name[64];
private:
    int16_t player_flag;
public:
    uint8_t unit_color;

    ConnectClientSettings();
    int16_t getPlayerFlag() const;
    void set(const char *player_name, uint8_t unit_color,
             uint16_t player_flag );
} __attribute__((packed));

class ConnectMesgServerGameSettings : public NetMessage
{
private:
    uint16_t max_players;
    uint16_t max_units;
public:
    char     map_name[32];
private:
    int32_t  cloud_coverage;
    float    wind_speed;
    int32_t  game_type;
public:
    uint8_t  powerup_state;
private:
    int32_t  frag_limit;
    int32_t  time_limit;
    time_t   elapsed_time;

public:
    ConnectMesgServerGameSettings();
    uint16_t getMaxPlayers() const;
    void setMaxPlayers(uint16_t maxPlayers);
    uint16_t getMaxUnits() const;
    void setMaxUnits(uint16_t maxUnits);
    int32_t getCloudCoverage() const;
    void setCloudCoverage(int32_t cloudCoverage);
    float getWindSpeed() const;
    void setWindSpeed(float windSpeed);
    int32_t getGameType() const;
    void setGameType(int32_t gameType);
    uint8_t getPowerupState() const;
    void setPowerupState(uint8_t powerupState);
    int32_t getFragLimit() const;
    void setFragLimit(int32_t fragLimit);
    int32_t getTimeLimit() const;
    void setTimeLimit(int32_t timeLimit);
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
    uint16_t player_id;

public:
    ConnectMesgNetPanzerClientDisconnect()
    {
        message_class = _net_message_class_connect;
        message_id = _net_message_id_connect_netPanzer_client_disconnect;
    }

    void setPlayerID(uint16_t id)
    {
        player_id = htol16(id);
    }
    uint16_t getPlayerID() const
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
