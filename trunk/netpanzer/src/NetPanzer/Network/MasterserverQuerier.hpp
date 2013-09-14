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

#ifndef MASTERSERVERQUERIER_HPP
#define	MASTERSERVERQUERIER_HPP

#include "Core/CoreTypes.hpp"
#include "Util/NTimer.hpp"
#include "ArrayUtil/PtrArray.hpp"
#include "Network/TCPSocket.hpp"

#include "2D/ComponentEvents.hpp"
#include "Network/ServerAddress.hpp"

class MasterserverQuerier : public network::TCPSocketObserver
{
public:
    MasterserverQuerier(ComponentEvents * events, int event_code);
    ~MasterserverQuerier();
    
    void beginQuery(PtrArray<ServerAddress> * dest);
    void endQuery();
    
    bool isQuerying() const { return servers != 0; }
    
private:
    MasterserverQuerier(const MasterserverQuerier&);
    void operator=(const MasterserverQuerier&);
    
    struct Data
    {
        Data(const NPString& host) : host(host), socket(0) {}
        ~Data();
        
        void connect(network::TCPSocketObserver * o);
        
        NPString host;
        NPString data;
        NTimer timer;
        network::TCPSocket * socket;
    };
    
    PtrArray<Data> masterservers;
    PtrArray<ServerAddress> * servers;
    ComponentEvents * events;
    int event_code;
    
    void queryFinished();
    
    size_t findMasterserverBySocket(network::TCPSocket * so);
    void addServer(const NPString& ip, const uint16_t port);
    void parseMasterserverRepply(Data * ms);
    
    void onConnected(network::TCPSocket *so);
    void onDisconected(network::TCPSocket *so);
    void onSocketError(network::TCPSocket *so);
    void onDataReceived(network::TCPSocket *so, const char *data, const int len);
};

#endif	/* MASTERSERVERQUERIER_HPP */

