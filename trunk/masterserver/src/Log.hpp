#ifndef __LOG_HPP__
#define __LOG_HPP__

#include <iostream>

namespace masterserver
{

extern std::ostream* log;

void initializeLog();
void closeLog();

}

#endif

