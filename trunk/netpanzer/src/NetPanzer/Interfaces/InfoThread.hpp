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
#ifndef __INFOTHREAD_HPP__
#define __INFOTHREAD_HPP__

#include <sstream>
#include <SDL_thread.h>
#include "Network/UDPSocket.hpp"
#include "Util/TimeStamp.hpp"

/** This class is responsible for answering queries about the server status.
 * Also contains code to detect if the server has hung and shuts it down in
 * this case.
 */
class InfoThread
{
public:
    InfoThread(int port);
    ~InfoThread();

    /** time when the server completed it's last frame. This is used to detect
     * servers that are in an endless loop (happens sometimes :-(  )
     */
    TimeStamp lastFrame;

private:
    static int threadMain(void* data);
    void handleStatusRequests();

    void sendInfo(std::stringstream& out);
    void sendRules(std::stringstream& out);
    void sendPlayers(std::stringstream& out);
    void checkServerHang();

    network::UDPSocket* socket;
    volatile bool running;
    SDL_Thread* thread;
};

#endif
