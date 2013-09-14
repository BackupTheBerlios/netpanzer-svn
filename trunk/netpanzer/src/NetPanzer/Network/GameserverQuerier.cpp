/*
Copyright (C) 2013 Netpanzer Team. (www.netpanzer.org), Aaron Perez

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
 * Created on September 14, 2013, 12:15 PM
 */

#include "GameserverQuerier.hpp"
#include "Util/Log.hpp"
#include "Util/StringTokenizer.hpp"
#include "Classes/ServerListDataSource.hpp"
#include <sstream>
#include <string>

#define MAXQUERY (5)
#define SERVERTIMEOUT (2000)

const char query[] = "\\status\\final\\";

GameserverQuerier::GameserverQuerier(ServerListDataSource * ds) : udpsocket(0), ds(ds)
{
    
}

GameserverQuerier::~GameserverQuerier()
{
    if ( udpsocket )
    {
        udpsocket->destroy();
        udpsocket = 0;
    }
}

void GameserverQuerier::onDataReceived(network::UDPSocket* so, const network::Address& from, const char* data, const int len)
{
    (void)so;
    std::stringstream fromaddress;
    fromaddress << from.getIP() << ":" << from.getPort();
    
    const std::string address(from.getIP());
    const uint16_t port(from.getPort());
    
    bool found = false;
    
    for ( unsigned n = 0; n < checking_servers.size(); n++ )
    {
        GameServerInfo * info = checking_servers[n];
        if ( (info->ip == address) && (info->port == port) )
        {
            found = true;
            NPString str;
            str.append(data, len);
            
            parseServerData(info, str);
            
            // *info now owned by ds
            
            checking_servers.erase(n);
            break;
        }
    }
    
    if ( ! found )
    {
        LOGGER.warning("Received answer from unknown server [%s]", fromaddress.str().c_str());
    }
    
    sendNextQuery();
}

void GameserverQuerier::onSocketError(network::UDPSocket* so)
{
    
}

void GameserverQuerier::sendQuery( GameServerInfo * info)
{
    unsigned serverIndex = 0;
    for ( ; serverIndex < checking_servers.size(); serverIndex++ )
    {
        if ( checking_servers[serverIndex] == info )
            break;
    }
    
    if ( serverIndex >= checking_servers.size() ) // is a new one
    {
        LOGGER.warning("New query server [%s:%u]", info->ip.c_str(), info->port);
        // address shall be numeric
        info->address = network::Address::resolve(info->ip, info->port);
        checking_servers.push_back(info);
    }
    else
    {
        LOGGER.warning("Querying server [%s:%u]", info->ip.c_str(), info->port);
    }
    
    info->queryTimestamp = SDL_GetTicks();
    udpsocket->send(info->address, query, sizeof(query)-1);
}

void GameserverQuerier::sendNextQuery()
{
    if ( checking_servers.size() >= MAXQUERY ) // max 5 query at time
        return;
    
    if ( ! not_queried.empty() )
    {
        const ServerAddress * a = not_queried.back();
        not_queried.pop_back();
        
        GameServerInfo * info = new GameServerInfo(a->ip, a->port);

//        delete a; // ServerAddress is not owned by me, don't delete it
        
        sendQuery(info);
    }
}

void GameserverQuerier::parseServerData(GameServerInfo* info, NPString& data)
{
    info->ping = SDL_GetTicks() - info->queryTimestamp;
    
    StringTokenizer tokenizer(data, '\\');

    std::string token;
    while( (token = tokenizer.getNextToken()) != "") {
        if (token == "protocol") {
            std::stringstream str(tokenizer.getNextToken());
            str >> info->protocol;
        } else if (token == "hostname") {
            info->server_name = tokenizer.getNextToken();
        } else if(token == "gameversion") {
            info->game_version = tokenizer.getNextToken();
        } else if(token == "mapname") {
            info->current_map = tokenizer.getNextToken();
        } else if(token == "mapcycle") {
            info->map_cycle = tokenizer.getNextToken();
        } else if(token == "password") {
            info->needs_password = tokenizer.getNextToken().compare("y") == 0 ? true : false;
        } else if(token == "numplayers") {
            std::stringstream str(tokenizer.getNextToken());
            str >> info->current_players;
        } else if(token == "maxplayers") {
            std::stringstream str(tokenizer.getNextToken());
            str >> info->max_players;
        } else if(token == "gamestyle") {
            info->gamestyle = tokenizer.getNextToken();
        } else if(token == "units_per_player") {
            std::stringstream str(tokenizer.getNextToken());
            str >> info->units_per_player;
        } else if(token == "time") {
            std::stringstream str(tokenizer.getNextToken());
            str >> info->runtime;
        } else if(token == "timelimit") {
            std::stringstream str(tokenizer.getNextToken());
            str >> info->time_limit;
        } else if(token == "fraglimit") {
            std::stringstream str(tokenizer.getNextToken());
            str >> info->frag_limit;
        } else if(token == "objectivelimit") {
            std::stringstream str(tokenizer.getNextToken());
            str >> info->objective_limit;
        } else {
            // handle more tokens...
        }
    }
    
    info->query_state = GameServerInfo::COMPLETE;
    
//    LOGGER.warning("-- SERVER REPPLY -- address : %s:%u", info->ip.c_str(), info->port );
//    LOGGER.warning("game version : %s", info->game_version.c_str() );
//    LOGGER.warning("name : %s", info->server_name.c_str() );
//    LOGGER.warning("map : %s", info->current_map.c_str() );
//    LOGGER.warning("cycle : %s", info->map_cycle.c_str() );
//    LOGGER.warning("password : %s", info->needs_password ? "yes" : "no" );
//    LOGGER.warning("players : %u", info->current_players );
//    LOGGER.warning("max players : %u", info->max_players );
//    LOGGER.warning("gametype : %s", info->gamestyle.c_str() );
//    LOGGER.warning("units/player : %u", info->units_per_player );
//    LOGGER.warning("runtime : %u", info->runtime );
//    LOGGER.warning("timelimit : %u", info->time_limit );
//    LOGGER.warning("fraglimit : %u", info->frag_limit );
//    LOGGER.warning("objlimit : %u", info->objective_limit );
//    LOGGER.warning("protocol : %u", info->protocol );
//    LOGGER.warning("ping : %u", info->ping );
//    LOGGER.warning("retries : %u", info->retryNum );
//    LOGGER.warning(" : %s", info-> );
    
    ds->addServer(info);
}

void GameserverQuerier::checkTimeouts()
{
    if ( udpsocket )
    {
        unsigned now = SDL_GetTicks();
        for ( unsigned n = 0; n < checking_servers.size(); n++ )
        {
            GameServerInfo * info = checking_servers[n];
            if ( (now - info->queryTimestamp) >= SERVERTIMEOUT )
            {
                if ( info->retryNum++ >= 3 ) // @todo make the retry numbers comfigurable?
                {
                    info->ping = 9999;
                    info->query_state = GameServerInfo::TIMEOUT;
                    checking_servers.erase(n);
                    LOGGER.warning("-- SERVER TIMEOUTTTTT -- %s:%u", info->ip.c_str(), info->port);
                    ds->addServer(info);
                    sendNextQuery();
                }
                else
                {
                    sendQuery(info);
                }
            }
        }
        
        // if empty, then not_queried is empty too
        if ( checking_servers.empty() )
        {
            endQuery();
        }
    }
}

void GameserverQuerier::beginQuery(const PtrArray<ServerAddress>& list)
{
    if ( (udpsocket == 0) && list.size() )
    {
        not_queried.initialize(list.size());
        for ( unsigned n = 0; n < list.size(); n++ )
        {
            not_queried.push_back(list[n]);
        }

        udpsocket = new network::UDPSocket(this);

        for (unsigned n = 0; n < MAXQUERY; n++ )
        {
            sendNextQuery();
        }
    }
}

void GameserverQuerier::endQuery()
{
    if ( udpsocket )
    {
        // @todo notify finished?
        not_queried.fastClear();
        checking_servers.deleteAll();
        udpsocket->destroy();
        udpsocket = 0;
    }
}
