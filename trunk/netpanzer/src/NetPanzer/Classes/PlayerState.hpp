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

    uint16_t getPlayerIndex() const;

    friend class PlayerState;
private:
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

    PlayerState::PlayerState();
    PlayerState::PlayerState(const PlayerState& other);

    void operator= (const PlayerState& other);

    void setName(const std::string& newname);
    void setPlayerID( PlayerID player_id );
    uint16_t getID() const
    {
        return ID.getIndex();
    }

    void resetStats();
    const std::string& getName() const;
    void lockStats();
    void unlockStats();
    short getKills() const;
    short getLosses() const;
    void incKills( unsigned short unit_type );
    void decKills( unsigned short unit_type );
    void incLosses( unsigned short unit_type );
    void decLosses( unsigned short unit_type );
    void incObjectivesHeld();
    void decObjectivesHeld();
    short getObjectivesHeld() const;
    void setObjectivesHeld( short objectives );
    void setID( unsigned short index, SocketClient::ID networkid  );
    void setID( unsigned short index );
    void setID( SocketClient::ID networkid );
    PlayerID getPlayerID() const;
    void setStatus( unsigned char status );
    unsigned char getStatus() const;
    void setFlag(unsigned char flag);
    unsigned char getFlag() const;
    short getTotal() const;
    NetworkPlayerState getNetworkPlayerState() const;
    void setFromNetworkPlayerState(const NetworkPlayerState* state);
    void setColor( uint32_t index );
    uint8_t getColor() const;
};

#endif // ** _PLAYERSTATE_HPP
