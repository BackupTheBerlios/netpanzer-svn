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
#include <config.h>

#include <iostream>
#include <exception>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#include "Log.hpp"
#include "Config.hpp"
#include "MasterServer.hpp"

using namespace masterserver;

static MasterServer* masterserv = 0;

static void signalhandler(int signum)
{
    static volatile sig_atomic_t handler_running = 0;
    if(handler_running)
        return;
    handler_running = 1;
    
    delete masterserv;
    masterserv = 0;
    closeLog();
    freeConfig();

    exit(0);
}

int main(int , char** )
{   
    loadConfig();
    initializeLog();
    
    try {
        masterserv = new MasterServer;

        // fork
        pid_t pid = fork();
        if(pid < 0) {
            perror("Error while forking: ");
            exit(EXIT_FAILURE);
        } if(pid > 0) {
            exit(EXIT_SUCCESS);
        }

        pid_t sid = setsid();
        if(sid < 0) {
            *masterserver::log << "Couldn't get session ID: " << strerror(errno)
                << std::endl;
            exit(EXIT_FAILURE);
        }

        umask(0);

        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);

        signal(SIGTERM, signalhandler);
        signal(SIGINT, signalhandler);
        signal(SIGQUIT, signalhandler);

        masterserv->run();
    } catch(std::exception& e) {
        *masterserver::log << "Fatal Error: " << e.what() << std::endl;
        std::cerr << "Fatal Error: " << e.what() << "\n";
        delete masterserv;
        masterserv = 0;

        closeLog();
        freeConfig();
        return 1;
    }
    
    assert(false); // we should never come here...
    delete masterserv;

    closeLog();
    freeConfig();
    return 0;
}

