/*
Copyright (C) 2004 Matthias Braun <matze@braunis.de>
 
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
#ifndef __HEARTBEATTHREAD_HPP__
#define __HEARTBEATTHREAD_HPP__

#include <vector>

#include <arpa/inet.h>
#include <pthread.h>

namespace masterserver
{

class MasterServer;

/** This class is responsible for notifying the other masterservers from time
 * to time that we're still running.
 */
class HeartbeatThread
{
public:
    HeartbeatThread(MasterServer* masterserver);
    ~HeartbeatThread();

private:
    static void* threadMain(void* data);
    void sendHeartbeats();
    void sendHeartbeat(struct sockaddr_in* address);

    void readNeighborCache(std::vector<std::string>& list);
    
    void requestMasterServerList();
    void requestMasterServerList2(struct sockaddr_in* address);
    void addMasterServer(const std::string& address);

    MasterServer* masterserver;
    
    volatile bool running;
    std::vector<struct sockaddr_in> serveraddresses;
    pthread_t thread;
};

} // end of namespace masterserver

#endif

