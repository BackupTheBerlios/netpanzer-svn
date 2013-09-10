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
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA  * 
 * Created on August 29, 2013, 9:48 PM
 */

#include "JoinGameLayer.hpp"

#include "2D/Components/AreaComponent.hpp"

#include "Classes/ScreenSurface.hpp"
#include "Classes/GameServerInfo.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Network/TCPSocket.hpp"
#include "Network/UDPSocket.hpp"
#include "Network/Address.hpp"
#include "Util/StringTokenizer.hpp"
#include "Util/StringUtil.hpp"
#include "Util/Log.hpp"
#include <string>
#include <sstream>

#define MENU_WIDTH  (640)
#define MENU_HEIGHT (480-26-26)

struct Events
{
    enum
    {
        RefreshGameList = 1,
        RefreshGameData
    };
};

static const size_t MAX_QUERIES = 4;
static const Uint32 QUERY_TIMEOUT = 3 * 1000;
static const Uint32 MS_TIMEOUT = 12 * 1000;

static const char  masterserver_query[] = "\\list\\gamename\\netpanzer\\final\\";

/* Notes
 * 
 * // old_query = "\\list\\gamename\\master\\final\\list\\gamename\\netpanzer\\final\\";
 * // new_query = "\\list\\gamename\\netpanzer\\final\\";
 *
 * original table: name, map, players, ping
 * if proto != NETPANZER_PROTOCOL_VERSION, show getNetpanzerProtocolMessage(server->protocol)
 * 
 * password protected servers: lock_image.loadBMP("pics/default/lock.bmp");
 * 
 * 
 * for querying game servers
 * 
 * udpsocket = new network::UDPSocket(this);
 * 
void
ServerQueryThread::sendNextQuery()
{
    if ( querying_server.size() >= 5 ) // max 5 query at time
        return;
    if ( ! not_queried.empty() )
    {
        ServerInfo *info = not_queried.back();
        not_queried.pop_back();
        sendQuery(info);
    }
}

void 
ServerQueryThread::sendQuery(ServerInfo *server)
{
    if (server->tryNum++ >= 3) { // 3 retrys fixed for now
        server->status = ServerInfo::TIMEOUT;
        sendNextQuery();
        return;
    }
    
    if (server->ipaddress == network::Address::ANY) {
        server->ipaddress = network::Address::resolve(server->address, server->port);
    }

    stringstream serveraddr;
    serveraddr << server->address << ":" << server->port;
    querying_server[serveraddr.str()]=server;
    LOGGER.warning("Querying server [%s]", serveraddr.str().c_str());
    
    char q[] = "\\status\\final\\";
    server->querystartticks = SDL_GetTicks();
    udpsocket->send(server->ipaddress,q,sizeof(q)-1);
    
}

void
ServerQueryThread::onDataReceived(network::UDPSocket *s, const network::Address& from, const char *data, const int len)
{
    (void)s;
    stringstream fromaddress;
    fromaddress << from.getIP() << ":" << from.getPort();
    
    string str;
    str.append(data,len);
    
    ServerInfo * server = querying_server[fromaddress.str()];
    if (server) {
        parseServerData(server,str);
        querying_server.erase(fromaddress.str());
    } else {
        LOGGER.warning("Received answer from unknown server [%s]", fromaddress.str().c_str());
        querying_server.erase(fromaddress.str()); // Quick & Dirty, should use find()
    }
    
    sendNextQuery();
    
}

void
ServerQueryThread::parseServerData(ServerInfo *server, string &data)
{
    server->ping = SDL_GetTicks() - server->querystartticks;
    
    StringTokenizer tokenizer(data, '\\');

    std::string token;
    while( (token = tokenizer.getNextToken()) != "") {
        if(token == "hostname") {
            server->name = tokenizer.getNextToken();
        } else if(token == "mapname") {
            server->map = tokenizer.getNextToken();
        } else if(token == "numplayers") {
            std::stringstream str(tokenizer.getNextToken());
            str >> server->players;
        } else if(token == "maxplayers") {
            std::stringstream str(tokenizer.getNextToken());
            str >> server->maxplayers;
        } else if(token == "protocol") {
            std::stringstream str(tokenizer.getNextToken());
            str >> server->protocol;
        } else if(token == "password") {
            server->needs_password = tokenizer.getNextToken().compare("y") == 0 ? true : false;
        } else {
            // handle more tokens...
        }
    }
    server->status = ServerInfo::RUNNING;
    
}
 * 
 * void
ServerQueryThread::checkTimeOuts()
{
    Uint32 now = SDL_GetTicks();
    
    if ( querying_msdata.empty() && querying_server.empty() && not_queried.empty())
    {
        LOGGER.warning("Stopping querys to servers, no more servers");
        running = false;
        state = STATE_DONE;
        if (udpsocket)
        {
            udpsocket->destroy();
            udpsocket = 0;
        }
        return;
    }
    
    map<network::TCPSocket *,MSInfo *>::iterator msiter;
    for (msiter=querying_msdata.begin(); msiter!=querying_msdata.end(); msiter++) {
        if ( now - msiter->second->lastTicks > MS_TIMEOUT ) {
            LOGGER.warning("Masterserver [%s] timeout", msiter->first->getAddress().getIP().c_str());
            delete msiter->second;
            msiter->first->destroy();
            querying_msdata.erase(msiter);
            sendNextQuery();
        }
    }
    
    
    map<string, ServerInfo *>::iterator i;
    for (i=querying_server.begin(); i!=querying_server.end(); i++) {
        if ( i->second->status == ServerInfo::TIMEOUT ) {
            LOGGER.warning("Server [%s] timeout, removing", i->first.c_str());
            querying_server.erase(i);
            break; // no more today, needed for invalid iterator
        } else if ( now - i->second->querystartticks > QUERY_TIMEOUT ) {
            LOGGER.warning("Server [%s] timeout, retrying", i->first.c_str());
            sendQuery(i->second);
        }
    }
}
 * 
 * 
 * 
 */

class ServerListQuery : public network::TCPSocketObserver
{
private:
    struct Data
    {
        Data(const NPString& host) : host(host), socket(0) {}
        
        ~Data()
        {
            if ( socket )
            {
                socket->destroy();
                socket = 0;
            }
        }
        
        void connect(network::TCPSocketObserver * o)
        {
            timer.reset();
            socket = new network::TCPSocket(host, "28900", o);
        }
        
        NPString host;
        NPString data;
        NTimer timer;
        network::TCPSocket * socket;
    };
    
    PtrArray<Data> masterservers;
    PtrArray<GameServerInfo> game_servers;
    
    size_t findMasterserverBySocket(network::TCPSocket * so)
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

    
public:
    ServerListQuery() {}
    ~ServerListQuery()
    {
        
    }
    
    void beginQuery()
    {
        masterservers.deleteAll();
        masterservers.clear();
        
        NPString server;
        StringTokenizer tokenizer(*GameConfig::server_masterservers, ',');
        while( (server = tokenizer.getNextToken()) != "")
        {
            LOGGER.debug("Begin connect to masterserver: '%s'", StringUtil::trim(server).c_str() );
            Data * d = new Data(StringUtil::trim(server));
            d->connect(this);
            masterservers.push_back( d );
        }
    }
    
    void onDataReceived(network::TCPSocket *so, const char *data, const int len)
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
        }
    }
    
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
    
    void addServer(const NPString& ip, const uint16_t port)
    {
        for ( size_t n = 0; n < game_servers.size(); n++ )
        {
            if ( (game_servers[n]->ip == ip) && (game_servers[n]->port == port) )
            {
                return; // already there
            }
        }

        LOGGER.warning("New server IP received: [%s:%d]", ip.c_str(), port);
        game_servers.push_back( new GameServerInfo(ip, port) );
    }

    void parseMasterserverRepply(Data * ms)
    {
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
    
    void onConnected(network::TCPSocket *so)
    {
        Data * ms = masterservers[findMasterserverBySocket(so)];
        // only sockets we create can be connected, no need to check, MUST BE OK
        
        LOGGER.warning("MASTERSERVER Connected [%s]", ms->host.c_str());
        
        ms->timer.reset();
        
        so->send(masterserver_query,sizeof(masterserver_query)-1);
    }
    
    void onDisconected(network::TCPSocket *so)
    {
        size_t i = findMasterserverBySocket(so);
        LOGGER.warning("MASTERSERVER Disconected [%s]", masterservers[i]->host.c_str());

        masterservers[i]->socket = 0;
        delete masterservers[i];
        masterservers.erase(i);
    }
    
    void onSocketError(network::TCPSocket *so)
    {
        size_t i = findMasterserverBySocket(so);
        LOGGER.warning("MASTERSERVER SocketError [%s]", masterservers[i]->host.c_str());

        masterservers[i]->socket = 0;
        delete masterservers[i];
        masterservers.erase(i);
    }
    
};



JoinGameLayer::JoinGameLayer() : ComponentLayer(0)
{
    area = new AreaComponent( MENU_WIDTH, MENU_HEIGHT );
    
    addComponent(area);
    
}

JoinGameLayer::~JoinGameLayer()
{
}

void JoinGameLayer::recalculateComponentLocations()
{
    int sw = screen->getWidth();
    int sh = screen->getHeight();

    area->setLocation((sw/2) - (area->getWidth()/2), ((sh/2) - (area->getHeight()/2)) );

    int x = area->getLocationX() + 10;
    int y = area->getLocationY() + 10;
    
    // @todo move the components
}

void JoinGameLayer::handleComponentEvents()
{
    int event;
    while ( (event = component_events.nextEvent()) ) switch ( event )
    {
        case Events::RefreshGameList:
            break;
            
        case Events::RefreshGameData:
            break;
            
    }
}
