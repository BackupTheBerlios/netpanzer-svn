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

#include "Log.hpp"
#include "Config.hpp"
#include "MasterServer.hpp"

using namespace masterserver;

static MasterServer* masterserv = 0;

static void signalhandler(int )
{
    delete masterserv;
    masterserv = 0;
    closeLog();
    freeConfig();
    exit(1);
}

int main(int , char** )
{   
    signal(SIGINT, signalhandler);

    loadConfig();
    initializeLog();
    
    try {
        masterserv = new MasterServer;
        masterserv->run();
    } catch(std::exception& e) {
        *masterserver::log << "Fatal Error: " << e.what() << "\n";
        std::cerr << "Fatal Error: " << e.what() << "\n";
        delete masterserv;
        masterserv = 0;

        closeLog();
        freeConfig();
        return 1;
    }
    delete masterserv;

    closeLog();
    freeConfig();
    return 0;
}

