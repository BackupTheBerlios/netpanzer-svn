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
#ifndef _PLAYERSTATE_HPP
#define _PLAYERSTATE_HPP

#include <string>
#include <string.h>
#include "PlayerID.hpp"
#include "PlayerUnitConfig.hpp"
#include "2D/Palette.hpp"

enum { _player_state_free,
       _player_state_allocated,
       _player_state_connecting,
       _player_state_active
     };

#ifdef MSVC
#pragma pack(1)
#endif

class NetworkPlayerState
{
public:
    NetworkPlayerState(const std::string& newname,
            uint8_t newflag, uint16_t newid, uint8_t newstatus,
            int16_t newkills, int16_t newkill_points, int16_t newlosses,
            int16_t newloss_points, int16_t newtotal,
            int16_t newobjectives_held, uint32_t newcolorIndex)
        : flag(newflag), playerindex_id(newid), status(newstatus),
          kills(newkills), kill_points(newkill_points), losses(newlosses),
          loss_points(newloss_points), total(newtotal),
          objectives_held(newobjectives_held), colorIndex(newcolorIndex)
    {
        memset(name, 0, sizeof(name));
        strncpy(name, newname.c_str(), 64);
        name[63] = '\0';
    }

    char name[64];
    uint8_t flag;
    uint16_t playerindex_id;
    uint8_t status;
    int16_t kills;
    int16_t kill_points;
    int16_t losses;
    int16_t loss_points;
    int16_t total;
    int16_t objectives_held;
    uint32_t colorIndex;
}
__attribute__((packed));

#ifdef MSVC
#pragma pack()
#endif

class PlayerState
{
private:
    std::string name;
    unsigned char flag;
    PlayerID ID;
    unsigned char status;
    short kills;
    short kill_points;
    short losses;
    short loss_points;
    short total;
    short objectives_held;
    bool stats_locked;
    uint32_t colorIndex;

public:
    PlayerUnitConfig unit_config;

    PlayerState::PlayerState()
        : flag(0), status(0), kills(0), kill_points(0), losses(0),
          loss_points(0), total(0), objectives_held(0), stats_locked(false)
    {
    }

    PlayerState::PlayerState(const PlayerState& other)
        : name(other.name), flag(other.flag), status(other.status),
          kills(other.kills), kill_points(other.kill_points),
          losses(other.losses), loss_points(other.loss_points),
          total(other.total), objectives_held(other.objectives_held),
          stats_locked(other.stats_locked), unit_config(other.unit_config)
    {
    }

    void operator= (const PlayerState& other)
    {
        name = other.name;
        flag = other.flag;
        status = other.status;
        kills = other.kills;
        kill_points = other.kill_points;
        losses = other.losses;
        loss_points = other.loss_points;
        total = other.total;
        objectives_held = other.objectives_held;
        stats_locked = other.stats_locked;
        unit_config = other.unit_config;
    }
    
    void setName(const std::string& newname)
    {
        name = newname;
    }

    void setID( PlayerID player_id )
    {
        ID = player_id;
    }

    void resetStats()
    {
        kills  = 0;
        kill_points = 0;
        losses = 0;
        loss_points = 0;
        total = 0;
        objectives_held = 0;
        stats_locked = false;
    }

    const std::string& getName() const
    {
        return name;
    }

    void lockStats()
    {
        stats_locked = true;
    }

    void unlockStats()
    {
        stats_locked = false;
    }

    short getKills()
    {
        return( kills );
    }

    short getLosses()
    {
        return( losses );
    }

    void incKills( unsigned short unit_type )
    {
        if ( stats_locked == true ) return;
        kills++;
        kill_points += 2 * unit_config.getUnitPointValue( (unsigned char)  unit_type ) ;
    }

    void decKills( unsigned short unit_type )
    {
        if ( stats_locked == true ) return;
        kills--;
    }

    void incLosses( unsigned short unit_type )
    {
        if ( stats_locked == true ) return;
        losses++;
        loss_points += unit_config.getUnitPointValue( (unsigned char) unit_type );
    }

    void decLosses( unsigned short unit_type )
    {
        if ( stats_locked == true ) return;
        losses--;
    }

    void incObjectivesHeld()
    {
        if ( stats_locked == true ) return;
        objectives_held++;
    }

    void decObjectivesHeld()
    {
        if ( stats_locked == true ) return;
        objectives_held++;
    }

    short getObjectivesHeld()
    {
        return( objectives_held );
    }

    void setObjectivesHeld( short objectives )
    {
        if ( stats_locked == true ) return;
        objectives_held = objectives;
    }

    void setID( unsigned short index, SocketClient::ID networkid  )
    {
        ID.setIndex(index);
        ID.setNetworkID(networkid);
    }

    void setID( unsigned short index )
    {
        ID.setIndex( index );
    }

    void setID( SocketClient::ID networkid )
    {
        ID.setNetworkID(networkid);
    }

    PlayerID getPlayerID()
    {
        return( ID );
    }

    void setStatus( unsigned char status )
    {
        PlayerState::status = status;
    }

    unsigned char getStatus()
    {
        return( status );
    }

    void setFlag(unsigned char flag)
    {
        PlayerState::flag = flag;
    }

    unsigned char getFlag()
    {
        return flag;
    }

    short getTotal()
    {
        return( kill_points - loss_points );
    }

    NetworkPlayerState getNetworkPlayerState()
    {
        return NetworkPlayerState(name, flag, ID.getIndex(), status,
                kills, kill_points, losses, loss_points, total,
                objectives_held, colorIndex);
    }

    void setFromNetworkPlayerState(NetworkPlayerState* state)
    {
        state->name[63] = '\0';
        name = state->name;
        flag = state->flag;
        ID.setIndex(state->playerindex_id);
        status = state->status;
        kills = state->kills;
        kill_points = state->kill_points;
        losses = state->losses;
        loss_points = state->loss_points;
        total = state->total;
        objectives_held = state->objectives_held;
	colorIndex = state->colorIndex;
    }

    void setColor( uint32_t index );
    uint8_t getColor();
};

#endif // ** _PLAYERSTATE_HPP
