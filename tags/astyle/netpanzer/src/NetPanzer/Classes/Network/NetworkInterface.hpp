/*
Copyright (C) 1998 Pyrosoft Inc. (www.pyrosoftgames.com), Matthew Bogue
 
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
#ifndef _NETWORKINTERFACE_HPP
#define _NETWORKINTERFACE_HPP

#include "NetPacketQueues.hpp"

void EnqueueIncomingPacket( void *message, unsigned long message_size,
                            Client::ID toID, Client::ID fromID );

void EnqueueUnreliablePacket( void *message, unsigned long message_size,
                              Client::ID toID, Client::ID fromID );

class NetworkInterface
{
public:
    static NetPacketQueue loop_back_send_queue;
    static NetPacketQueue loop_back_recv_queue;
    static NetPacketQueue receive_queue;
    static ReorderQueue   non_guarantee_queue;

protected:

public:
    NetworkInterface( void );
    ~NetworkInterface();

};


#endif // ** _NETWORKINTERFACE_HPP