#include <config.h>

#include "Log.hpp"

#include <ios>
#include <fstream>

#define LOGFILE LOCALSTATEDIR"/masterserver"

namespace masterserver
{

std::ostream* log = 0;

void initializeLog()
{
    // try to open a file
    log = new std::ofstream(LOGFILE, std::ios_base::app);
    if(!log->good()) {
        std::cout << "Couldn't open '" << LOGFILE 
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
