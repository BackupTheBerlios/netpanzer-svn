#ifndef __OPCODEDEBUGGER_HPP__
#define __OPCODEDEBUGGER_HPP__

#include <iostream>

class UnitOpcode;

class OpcodeDebugger
{
public:
    static void logOpcode(std::ostream& stream, UnitOpcode* opcode);
};

#endif

