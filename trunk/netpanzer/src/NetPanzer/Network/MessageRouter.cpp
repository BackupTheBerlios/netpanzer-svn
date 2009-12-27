/*
Copyright (C) 2008 by Aaron Perez <aaronps@gmail.com>

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
 * 
 * Created on December 13, 2008, 6:58 PM
 */

#include <queue>

#include "Resources/ResourceManager.hpp"
#include "Core/GlobalGameState.hpp"

#include "Interfaces/PlayerInterface.hpp"


#include "Interfaces/ChatInterface.hpp"


#include "MessageRouter.hpp"
#include "MessageClassHandler.hpp"
#include "Util/Log.hpp"
#include "Classes/Network/NetPacket.hpp"
#include "Classes/Network/NetMessageDecoder.hpp"
#include "Classes/Network/TerminalNetMesg.hpp"
#include "Interfaces/GameManager.hpp"
#include "Objectives/ObjectiveInterface.hpp"
#include "PowerUps/PowerUpInterface.hpp"
#include "Classes/Network/NetworkServer.hpp"
#include "Classes/Network/NetworkClient.hpp"
#include "Classes/Network/NetworkState.hpp"
#include "Classes/Network/NetPacketDebugger.hpp"


class MultiMessage;

static MessageClassHandler * handlers[256];
static std::queue<NetPacket> receive_queue;

class NoHandler : public MessageClassHandler
{
public:
    void handlePacket(const NetPacket *p)
    {
        LOGGER.warning("Received packet of unknown class '%d'",
                       p->getNetMessage()->message_class);
    }
} nohandler;

class MultiHandler : public MessageClassHandler
{
public:
    void handlePacket(const NetPacket *p)
    {
        static NetMessageDecoder decoder;
        decoder.setDecodeMessage((const MultiMessage *) p->getNetMessage());

        NetPacket temp;
        temp.fromPlayer = p->fromPlayer;
        temp.fromClient = p->fromClient;

        NetMessage * dmsg;
        while(decoder.decodeMessage(&dmsg))
        {
            // avoid recursion
            if ( dmsg->message_class != p->getNetMessage()->message_class )
            {
                memcpy(temp.data, dmsg, dmsg->getSize());
                handlers[dmsg->message_class]->handlePacket(&temp);
            }
        }
    }
} multihandler;

class TerminalHandler : public MessageClassHandler
{
public:
    void handlePacket(const NetPacket *p)
    {
        switch ( p->getNetMessage()->message_id )
        {
            case _net_message_id_term_unit_cmd:
                UnitInterface::processNetPacket(p);
                break;
            case _net_message_id_term_unit_gen:
            case _net_message_id_term_output_loc:
                ObjectiveInterface::processTerminalNetPacket(p);
                break;
            default:
                LOGGER.warning("Unknown Terminal message(%d) from %d",
                               p->getNetMessage()->message_id, p->fromPlayer);
        }
    }
}terminalhandler;

class ToMessageHandler : public MessageClassHandler
{
private:
    void (*func)(const NetMessage *);
public:
    ToMessageHandler(void (*f)(const NetMessage *)) : func(f) {}
    void handlePacket(const NetPacket *p)
    {
        func(p->getNetMessage());
    }
};

class ToPacketHandler : public MessageClassHandler
{
private:
    void (*func)(const NetPacket *);
public:
    ToPacketHandler(void (*f)(const NetPacket *)) : func(f) {}
    void handlePacket(const NetPacket *p)
    {
        func(p);
    }
};

static ToMessageHandler systemhandler(&GameManager::processSystemMessage);
static ToPacketHandler serverconnecthandler(&ServerConnectDaemon::processNetPacket);
static ToMessageHandler clientconnecthandler(&ClientConnectDaemon::processNetMessage);
static ToMessageHandler resourcehandler(&ResourceManager::processResourceMessage);
static ToMessageHandler playerhandler(&PlayerInterface::processNetMessage);
static ToMessageHandler unithandler(&UnitInterface::processNetMessage);
static ToMessageHandler objectivehandler(&ObjectiveInterface::processNetMessages);
static ToMessageHandler gamecontrolhandler(&GameControlRulesDaemon::processNetMessage);
static ToMessageHandler poweruphandler(&PowerUpInterface::processNetMessages);
static ToPacketHandler chathandler(&ChatInterface::processChatMessages);

void
MessageRouter::initialize(bool isServer)
{
    for (int n=0; n<256; n++)
    {
        handlers[n] = &nohandler;
    }

    // set default handlers
    //handlers[_net_message_class_client_server]  = ; each handle itself

    // both
    handlers[_net_message_class_multi]          = &multihandler;
    handlers[_net_message_class_system]         = &systemhandler;
    handlers[_net_message_class_player]         = &playerhandler;
    handlers[_net_message_class_resource]       = &resourcehandler;
    handlers[_net_message_class_chat]           = &chathandler;
    if ( isServer )
    {
        handlers[_net_message_class_terminal]      = &terminalhandler;
        handlers[_net_message_class_connect]       = &serverconnecthandler;
        handlers[_net_message_class_client_server] = NetworkServer::getPacketHandler();
    }
    else
    {
        handlers[_net_message_class_unit]         = &unithandler;
        handlers[_net_message_class_objective]    = &objectivehandler;
        handlers[_net_message_class_game_control] = &gamecontrolhandler;
        handlers[_net_message_class_powerup]      = &poweruphandler;
        handlers[_net_message_class_connect]      = &clientconnecthandler;
        handlers[_net_message_class_client_server] = NetworkClient::getPacketHandler();
    }
    // XXX has to free the queue some time;
    //receive_queue.initialize(200);
}

void
MessageRouter::setMessageClassHandler(MsgClassID c, MessageClassHandler * h)
{
    handlers[c] = h;
}

void
MessageRouter::clearMessageClassHandler(MsgClassID c)
{
    handlers[c] = &nohandler;
}


void
MessageRouter::enqueueIncomingPacket( const void *data, Uint16 size,
                            Uint16 fromPlayer, ClientSocket *fromClient)
{
    static NetPacket TEMP_PACKET;

    TEMP_PACKET.fromPlayer = fromPlayer;
    TEMP_PACKET.fromClient = fromClient;
    assert(size <= _MAX_NET_PACKET_SIZE);

    memcpy(TEMP_PACKET.data, data, size);
    receive_queue.push( TEMP_PACKET );
}

void
MessageRouter::routePackets()
{
    static NetPacket np;
    while ( ! receive_queue.empty() )
    {
        np = receive_queue.front();
        receive_queue.pop();
        NetworkState::incPacketsReceived(np.getSize());
#ifdef NETWORKDEBUG
        NetPacketDebugger::logPacket("R", &np);
#endif

        handlers[np.getNetMessage()->message_class]->handlePacket(&np);
    }
}

void
MessageRouter::routePacket(const NetPacket& np)
{
    handlers[np.getNetMessage()->message_class]->handlePacket(&np);
}

bool
MessageRouter::getNextPacket(NetPacket& np)
{
    if ( ! receive_queue.empty() )
    {
        np = receive_queue.front();
        receive_queue.pop();
        return true;
    }
    return false;
}
