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
#ifndef _NETPACKETQUEUES_HPP
#define _NETPACKETQUEUES_HPP

#include <string.h>
#include "NetPacket.hpp"
#include "QueueTemplate.hpp"
#include "ArrayTemplate.hpp"
#include "Timer.hpp"

class NetPacketQueue : public QueueTemplate< NetPacket >
 {
  public:

  inline void add( NetPacket *object, unsigned long index )
   {
    unsigned long net_packet_size;
    
    assert( index < size );
    net_packet_size = (sizeof( NetPacket ) - _MAX_NET_PACKET_SIZE) + object->packet_size;
    memmove( (void *) &array[index], (void *) object, net_packet_size );
   }  

  inline void dequeue( NetPacket *object )
   {             
    unsigned long net_packet_size;
 
    assert( front != rear );       
    front = ( front + 1 ) % size;
    net_packet_size = (sizeof( NetPacket ) - _MAX_NET_PACKET_SIZE) + array[ front ].packet_size; 
    memmove( (void *) object, (void *) &array[ front ], net_packet_size );	       
   }
 
   inline void enqueue( NetPacket &object )
    {
     //assert( front != rear );
     add( &object, (rear + 1) % size );
     rear = (rear + 1) % size;
    }
 
 };

class NetPacketBlock
 {
  public:

   NetPacket     packet;
   bool       in_use;
   bool       out_of_order;
   unsigned char sequence;
  
   NetPacketBlock( )
    {
     in_use = false;
     out_of_order = false;
    }
 
   unsigned short getPacketSize()
    {
     return( packet.packet_size);
    }
  
   unsigned char getSequence( void )
    {
     return( ((NetMessage *) (&(packet.data)) )->sequence );
    }

 };

typedef ArrayTemplate< NetPacketBlock > NetPacketBlockArray;

class ReorderQueue : public NetPacketBlockArray
 {
  protected:
   unsigned char current_sequence_num;
   unsigned char dequeue_sequence_num;
   
   // ** variables for exponential averaging of packet window
   TIMESTAMP packet_recv_stamp; 
   TIMESTAMP next_packet_recv_stamp;   
   bool window_estimate_init;
   float packet_window_time;
   float packet_window_estimate;
   float packet_window_time_weight;
  
   // ** variables for tracking packet window 
   bool packet_window_wait_flag;
   Timer   packet_window_timer;

   // ** variable for statictics
   int lost_packets;
   int out_of_order_packets;

  public:
   
   ReorderQueue( );

   void initialize( unsigned long size );
   
   void reset( void );

   void enqueue( NetPacket *net_packet );

   void dequeue( NetPacket *net_packet );

   bool isReady( void );

   void getStats( float *packet_window_time, float *packet_window_estimate,
                  int *lost_packets, int *out_of_order_packets );


 };



#endif // ** _NETPACKETQUEUES_HPP
