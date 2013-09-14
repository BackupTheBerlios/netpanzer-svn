/*
Copyright (C) 2013 Netpanzer Team. (www.netpanzer.org), Aaron Perez Sanchez

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

 *
 * Created on January 31, 2013, 12:57 AM
 */

#ifndef GAMESERVERINFO_HPP
#define	GAMESERVERINFO_HPP

#include "Core/CoreTypes.hpp"
#include "Network/Address.hpp"

class GameServerInfo
{
public:
    GameServerInfo(const NPString& ip, const uint16_t port)
        : query_state(NOT_STARTED), ip(ip), port(port), retryNum(0)
    {
        // nothing
    }

    enum
    {
        NOT_STARTED,
        STARTED,
        COMPLETE,
        TIMEOUT
    } query_state;

    network::Address address;
    NPString ip;
    uint16_t port;

    NPString server_name;
    NPString current_map;
    NPString map_cycle;
    bool needs_password;
    unsigned current_players;
    unsigned max_players;

    NPString gamestyle;

    unsigned units_per_player;
    unsigned runtime;
    unsigned time_limit;
    unsigned frag_limit;
    unsigned objective_limit;
    
    NPString game_version;
    unsigned protocol;
    unsigned queryTimestamp;
    unsigned ping;
    unsigned retryNum;
};

#endif	/* GAMESERVERINFO_HPP */

