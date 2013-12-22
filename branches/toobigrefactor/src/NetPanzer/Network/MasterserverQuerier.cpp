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
 * Created on September 11, 2013, 7:41 PM
 */

#include "MasterserverQuerier.hpp"

#include <stddef.h>
#include <sstream>
#include "Network/TCPSocket.hpp"
#include "Util/Log.hpp"
#include "Util/StringTokenizer.hpp"
#include "Util/StringUtil.hpp"
#include "Interfaces/GameConfig.hpp"

// old_query = "\\list\\gamename\\master\\final\\list\\gamename\\netpanzer\\final\\";
static const char  masterserver_query[] = "\\list\\gamename\\netpanzer\\final\\";

static inline bool getToken(StringTokenizer& tkn, const std::string& name, std::string& dest)
{
    if ( tkn.hasMore() && (tkn.getNextToken() == name) )
    {
        if ( tkn.hasMore() )
        {
            dest = tkn.getNextToken();
            return true;
        }
    }

    return false;
}

MasterserverQuerier::Data::~Data()
{
    if ( socket )
    {
        socket->destroy();
        socket = 0;
    }
}

void MasterserverQuerier::Data::connect(network::TCPSocketObserver* o)
{
    timer.reset();
    socket = new network::TCPSocket(host, "28900", o);
}

size_t MasterserverQuerier::findMasterserverBySocket(network::TCPSocket* so)
{
    for ( size_t n = 0; n < masterservers.size(); n++ )
    {
        if ( masterservers[n]->socket == so )
        {
            return n;
        }
    }
    return -1;
}


MasterserverQuerier::MasterserverQuerier(ComponentEvents * events, int event_code)
: events(events), event_code(event_code)
{
    servers = 0;
}

MasterserverQuerier::~MasterserverQuerier()
{
    endQuery();
}

void MasterserverQuerier::beginQuery(PtrArray<ServerAddress>* dest)
{
    servers = dest;
    masterservers.deleteAll();
    masterservers.clear();

    for ( int n = 0; n < gameconfig->masterservers.size(); n++ )
    {
        LOGGER.debug("Begin connect to masterserver: '%s'", gameconfig->masterservers[n].c_str() );
        Data * d = new Data(gameconfig->masterservers[n]);
        d->connect(this);
        masterservers.push_back( d );
    }
}

void MasterserverQuerier::endQuery()
{
    masterservers.deleteAll();
    masterservers.clear();
    servers = 0;
}

void MasterserverQuerier::addServer(const NPString& ip, const uint16_t port)
{
    for ( size_t n = 0; n < servers->size(); n++ )
    {
        if ( (servers->get(n)->ip == ip) && (servers->get(n)->port == port) )
        {
            return; // already there
        }
    }

    LOGGER.warning("New server IP received: [%s:%d]", ip.c_str(), port);
    servers->push_back( new ServerAddress(ip, port) );
}

void MasterserverQuerier::parseMasterserverRepply(Data* ms)
{
    if ( ! servers )
        return;
    
    std::string str = ms->data;
    str.erase( str.find("\\final") );

    StringTokenizer tknizer(str,'\\');

    std::string dirip;
    std::string portstr;

    while ( tknizer.hasMore() )
    {
        if (  (! getToken(tknizer, "ip", dirip))
            ||(! getToken(tknizer, "port", portstr)) )
        {
            continue;
        }

        uint16_t port = 0;
        std::stringstream ss(portstr);
        ss >> port;

        if ( dirip.empty() || (port == 0) )
        {
            LOGGER.warning("Invalid IP received from masterserver");
            continue;
        }

        addServer(dirip, port);
    }
}

void MasterserverQuerier::onConnected(network::TCPSocket* so)
{
    size_t i = findMasterserverBySocket(so);
    Data * ms = masterservers[i];
    // only sockets we create can be connected, no need to check, MUST BE OK

    LOGGER.warning("MASTERSERVER Connected [%s]", ms->host.c_str());
    
    ms->timer.reset();

    so->send(masterserver_query,sizeof(masterserver_query)-1);
}

void MasterserverQuerier::onDisconected(network::TCPSocket* so)
{
    size_t i = findMasterserverBySocket(so);
    LOGGER.warning("MASTERSERVER Disconected [%s]", masterservers[i]->host.c_str());

    masterservers[i]->socket = 0;
    delete masterservers[i];
    masterservers.erase(i);
    if ( masterservers.empty() )
    {
        queryFinished();
    }
}

void MasterserverQuerier::onSocketError(network::TCPSocket* so)
{
    size_t i = findMasterserverBySocket(so);
    LOGGER.warning("MASTERSERVER SocketError [%s]", masterservers[i]->host.c_str());

    masterservers[i]->socket = 0;
    delete masterservers[i];
    masterservers.erase(i);
    if ( masterservers.empty() )
    {
        queryFinished();
    }
}

void MasterserverQuerier::onDataReceived(network::TCPSocket* so, const char* data, const int len)
{
    size_t i = findMasterserverBySocket(so);
    Data * ms = masterservers[i];
    
    ms->timer.reset();
    ms->data.append(data,len);

    if ( ms->data.find("\\final") != std::string::npos )
    {
        LOGGER.warning("Masterserver data:[%s]", ms->data.c_str());

        parseMasterserverRepply(ms);

        delete ms;
        masterservers.erase(i);
        if ( masterservers.empty() )
        {
            queryFinished();
        }
    }
}

void MasterserverQuerier::queryFinished()
{
    if ( servers )
    {
        servers = 0;
        events->push(event_code);
    }
}
