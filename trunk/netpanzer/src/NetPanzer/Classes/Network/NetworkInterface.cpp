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
#include <config.h>
#include "NetworkInterface.hpp"
#include "codewiz.hpp"

NetPacket TEMP_PACKET;

void EnqueueIncomingPacket( void *message, unsigned long message_size, DPID toID, DPID fromID )
 {
  TEMP_PACKET.toID = toID;
  TEMP_PACKET.fromID = fromID;
  if( message_size > _MAX_NET_PACKET_SIZE )
   {
    LOG( ("ERROR: NetPacket Overflow") );
    message_size = _MAX_NET_PACKET_SIZE;
   }
 
  TEMP_PACKET.packet_size = (unsigned short) message_size; 

  memmove( TEMP_PACKET.data, message, message_size );
  NetworkInterface::receive_queue.enqueue( TEMP_PACKET );
 }

NetPacket TEMP_PACKET2;

void EnqueueUnreliablePacket( void *message, unsigned long message_size, DPID toID, DPID fromID )
 {
  TEMP_PACKET2.toID = toID;
  TEMP_PACKET2.fromID = fromID;

  if( message_size > _MAX_NET_PACKET_SIZE )
   {
    LOG( ("ERROR: NetPacket Overflow") );
    message_size = _MAX_NET_PACKET_SIZE;
   }
  
  TEMP_PACKET2.packet_size = (unsigned short) message_size; 
  
  memmove( TEMP_PACKET2.data, message, message_size );
  NetworkInterface::non_guarantee_queue.enqueue( &TEMP_PACKET2 );
 }


NetPacketQueue NetworkInterface::loop_back_send_queue;
NetPacketQueue NetworkInterface::loop_back_recv_queue;
NetPacketQueue NetworkInterface::receive_queue;
ReorderQueue   NetworkInterface::non_guarantee_queue;

NetworkInterface::NetworkInterface( void )
 {
  loop_back_send_queue.initialize( 25 );
  loop_back_recv_queue.initialize( 100 );
  receive_queue.initialize( 200 );
  non_guarantee_queue.initialize( 100 );
 }

NetworkInterface::~NetworkInterface()
 {
  loop_back_send_queue.deallocate();
  loop_back_recv_queue.deallocate();
  receive_queue.deallocate();
  non_guarantee_queue.deallocate();
 }
