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
#ifndef _PLAYERID_HPP
#define _PLAYERID_HPP

#include <stdint.h>
#include "Network/SocketClient.hpp"

class PlayerID
{
public:
    PlayerID( )
            : index_id(0xFFFF), networkid(0xFFFFFFFF)
    { }

    PlayerID( unsigned short index, SocketClient::ID newid )
            : index_id(index), networkid(newid)
    { }

    PlayerID(const PlayerID& other)
            : index_id(other.index_id), networkid(other.networkid)
    { }

    void operator = (const PlayerID& other)
    {
        index_id = other.index_id;
        networkid = other.networkid;
    }

    void setIndex( unsigned short index )
    {
        index_id = index;
    }

    unsigned short getIndex() const
    {
        return( index_id );
    }

    void setNetworkID(SocketClient::ID newid)
    {
        networkid = newid;
    }

    SocketClient::ID getNetworkID() const
    {
        return(networkid);
    }

    bool operator==(const PlayerID &rhs) const
    {
        if ( (index_id == rhs.index_id) && (networkid == rhs.networkid) )
            return true;

        return false;
    }

private:
    uint16_t index_id;
    SocketClient::ID networkid;
};

#endif // ** _PLAYERID_HPP
