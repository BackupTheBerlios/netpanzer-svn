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

#include "Log.hpp"

#include <ios>
#include <fstream>

#include "Config.hpp"

namespace masterserver
{

std::ostream* log = &std::cout;

void initializeLog()
{
    // try to open a file
    const std::string& logfile=config->getSection("server").getValue("logfile");
    log = new std::ofstream(logfile.c_str(), std::ios_base::app);
    if(!log->good()) {
        std::cout << "Couldn't open '" << logfile
                  << "' falling back to stdout.\n";
        delete log;
        log = &std::cout;
    } else {
        *log << "\nRestarted masterserver" << std::endl;
    }
}

void closeLog()
{
    if(log != &std::cout) {
        *log << "Logfile closed." << std::endl;
        delete log;
        log = 0;
    }
}

}
