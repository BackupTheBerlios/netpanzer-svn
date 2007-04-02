/*
Copyright (C) 2007 by Aaron Perez <aaronps@gmail.com>

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

#include <string>
#include <sstream>
#include <stdexcept>
#include "SDL.h"
#include "Heartbeat.hpp"
#include "NetworkGlobals.hpp"
#include "GameConfig.hpp"
#include "Util/StringTokenizer.hpp"
#include "Util/StringUtil.hpp"
#include "Util/Log.hpp"

using namespace std;
using namespace network;

#define MSQUERY_TIMEOUT 5 * 1000
#define HEARTBEAT_INTERVAL 5 * 60 * 1000

class MasterserverInfo {
public:
    MasterserverInfo(){ touch(); };
    ~MasterserverInfo(){};
    void touch() { lastTicks = SDL_GetTicks(); };
    string recdata;
    Uint32 lastTicks;
    
};

Heartbeat::Heartbeat()
{
    StringTokenizer mstokenizer(gameconfig->masterservers, ',');
    string servname;
    while( (servname = mstokenizer.getNextToken()) != "") {
        servname = removeSurroundingSpaces(servname);
        try {
            Address addr = Address::resolve(servname, MASTERSERVER_PORT);
            mslist.push_back(addr);
        } catch (runtime_error e) {
            LOGGER.warning("Bad masterserver address: %s", e.what());
        }
    }
    
    stringstream msg;
    msg << "\\heartbeat\\gamename\\netpanzer\\port\\" << gameconfig->serverport                     
        << "\\protocol\\" << NETPANZER_PROTOCOL_VERSION
        << "\\final\\";
    
    hb_message=msg.str();
    nextHeartbeatTicks = SDL_GetTicks() + HEARTBEAT_INTERVAL;
    startHeartbeat();
    
}

Heartbeat::~Heartbeat()
{
    if ( !masterservers.empty() ) {
        map<TCPSocket *, MasterserverInfo *>::iterator msiter;
        for (msiter=masterservers.begin(); msiter!=masterservers.end(); msiter++) {
            delete msiter->second;
            msiter->first->destroy();
        }
        masterservers.clear();
    }
    mslist.clear();
}

void
Heartbeat::checkHeartbeat()
{
    Uint32 now = SDL_GetTicks();

    if ( !masterservers.empty() ) {
        map<TCPSocket *, MasterserverInfo *>::iterator msiter;
        for (msiter=masterservers.begin(); msiter!=masterservers.end(); msiter++) {
            if ( now - msiter->second->lastTicks > MSQUERY_TIMEOUT ) {
                LOGGER.warning("Masterserver timeout [%s]", msiter->first->getAddress().getIP().c_str());
                delete msiter->second;
                msiter->first->destroy();
                masterservers.erase(msiter);
            }
        }
    }
    
    if ( now > nextHeartbeatTicks ) {
        nextHeartbeatTicks += HEARTBEAT_INTERVAL;
        startHeartbeat();
    }

}

void
Heartbeat::startHeartbeat()
{
    vector<Address>::iterator iter = mslist.begin();
    while ( iter != mslist.end() ) {
        TCPSocket *s = new TCPSocket(*iter, this);
        MasterserverInfo * msi = new MasterserverInfo();
        masterservers[s]=msi;
        iter++;
    }
}

void
Heartbeat::onConnected(TCPSocket *so)
{
    LOGGER.debug("Masterserver connected [%s]", so->getAddress().getIP().c_str());
    masterservers[so]->touch();
    so->send(hb_message.c_str(), hb_message.size());
}

void
Heartbeat::onDisconected(TCPSocket *so)
{
    LOGGER.debug("Masterserver disconected [%s]", so->getAddress().getIP().c_str());
    delete masterservers[so];
    masterservers.erase(so);
}

void
Heartbeat::onDataReceived(TCPSocket *so, const char *data, const int len)
{
    string str;
    
    MasterserverInfo * msi = masterservers[so];
    msi->touch();
    str = msi->recdata;
    str.append(data,len);
    
    if (str[0] != '\\') {
        LOGGER.warning("Bad answer from masterserver, missing initial separator [%s]", so->getAddress().getIP().c_str());
        delete msi;
        masterservers.erase(so);
        so->destroy();
        return; // invalid answer;
    }
    
    string lastpart;
    if (str[str.length()-1] != '\\') {
        // received incomplete
        string::size_type p = str.rfind('\\');
        msi->recdata = str.substr(p);
        str.erase(p);
    } else {
        msi->recdata = "\\";
    }
    
    StringTokenizer tknizer(str,'\\');
    
    string token = tknizer.getNextToken();
    while ( !token.empty()) {
        if ( token == "error" ) {
            LOGGER.warning("Masterserver returns error: '%s'", tknizer.getNextToken().c_str());
        } else if ( token == "final") {
            LOGGER.debug("Masterserver answer ok, disconecting [%s]", so->getAddress().getIP().c_str());
            delete msi;
            masterservers.erase(so);
            so->destroy();
            break;
        } else {
            LOGGER.warning("Masterservend sent unknown answer: '%s'", tknizer.getNextToken().c_str());
        }
    }
}





