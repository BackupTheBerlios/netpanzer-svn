#ifndef __NETPACKETDEBUGGER_HPP__
#define __NETPACKETDEBUGGER_HPP__

#include <iostream>

class NetMessage;
class UMesgAICommand;

class NetPacketDebugger
{
public:
    static void logMessage(const char* domain, NetMessage* message);

private:
    static void logTerminalMessage(std::ostream& stream, NetMessage* message);
    static void logAICommand(std::ostream& str, const UMesgAICommand& command);
    static void logMultiMessage(std::ostream& str, NetMessage* message);
    static void logUnitMessage(std::ostream& log, NetMessage* message);
    static void logUnitOpcodeMessage(std::ostream& log, NetMessage* message);
    static void logClientServerMessage(std::ostream& log, NetMessage* message);
    static void logConnectMessage(std::ostream& log, NetMessage* message);
};

#endif

