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

#include <string>

#include <SDL_net.h>
#include <SDL_thread.h>

/** This class is responsible for notifying the masterserver from time to time
 * that we're still alive.
 */
class HeartbeatThread
{
public:
    /** Starts a thread and sends a heartbeat packet to the masterserver every 5
     * minutes. Throws std::exception's when something goes wrong while
     * intializing.
     */
    HeartbeatThread();
    /** Sends a statechanged message to the masterserver indicating that the
     * server gets shutdown. Remember that the masterserver will query us to
     * make sure the packet wasn't faked. So the infothread should return the
     * same.
     */
    ~HeartbeatThread();

private:
    static int threadMain(void* data);
    void sendHeartbeat();
    void sendPacket(const std::string& data);
    
    volatile bool running;
    std::vector<IPaddress> serveraddrs;
    SDL_Thread* thread;
};

#endif

