#include <config.h>

#include "Log.hpp"

#include <fstream>

#define LOGFILE LOCALSTATEDIR"/masterserver"

namespace masterserver
{

std::ostream* log = 0;

void initializeLog()
{
    // try to open a file
    log = new std::ofstream(LOGFILE);
    if(!log->good()) {
        std::cout << "Couldn't open '" << LOGFILE 
                  << "' falling back to stdout.\n";
        delete log;
        log = &std::cout;
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
