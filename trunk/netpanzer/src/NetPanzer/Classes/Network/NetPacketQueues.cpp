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
#include "stdafx.hpp"
#include "NetPacketQueues.hpp"
#include "CodeWiz.hpp"
#include "ConsoleInterface.hpp"


#define _DEFAULT_WINDOW_TIME     (0.15) // in (seconds) 150ms
#define _WINDOW_ESTIMATE_WEIGHT  (0.25) // 0 <= weight <= 1
#define _WINDOW_ESTIMATE_BOUND   (0.15)  // in seconds;

ReorderQueue::ReorderQueue( )
  : NetPacketBlockArray(  )
 {
  reset();
 }

void ReorderQueue::initialize( unsigned long size )
 {
  NetPacketBlockArray::initialize( size );
  reset();
 }
   
void ReorderQueue::reset( void )
 {
  current_sequence_num = 0;
  dequeue_sequence_num = 0;
  packet_window_time = (float) _DEFAULT_WINDOW_TIME;
  packet_window_estimate = (float) _DEFAULT_WINDOW_TIME;
  packet_window_time_weight = (float) _WINDOW_ESTIMATE_WEIGHT;
  packet_window_wait_flag = _FALSE;
  window_estimate_init = _FALSE;
  lost_packets = 0;
  out_of_order_packets = 0;
 }

void ReorderQueue::enqueue( NetPacket *net_packet )
 {
  unsigned long insert_index;
  unsigned long packet_sequence_num;
  unsigned long net_packet_size;
  
  packet_recv_stamp = now();
  packet_window_time = packet_recv_stamp - next_packet_recv_stamp;
  next_packet_recv_stamp = packet_recv_stamp;
   
  if( window_estimate_init == _FALSE  )
   {
    packet_window_time = (float) _DEFAULT_WINDOW_TIME;
    window_estimate_init = _TRUE;
   }

  packet_window_estimate = (packet_window_time_weight)*( packet_window_time ) +
                           (1-packet_window_time_weight)*(packet_window_estimate); 

  if( packet_window_estimate > _WINDOW_ESTIMATE_BOUND )
   {
    packet_window_estimate = (float) _WINDOW_ESTIMATE_BOUND; 
   }

  packet_sequence_num = ((NetMessage *) (&(net_packet->data)) )->sequence;

  //LOG( ("*** Packet # %d ***", packet_sequence_num ) );

  insert_index = packet_sequence_num % size;

  if( (array[ insert_index ].in_use == _TRUE) && (array[ insert_index ].out_of_order == _FALSE) )
   { 
    if( array[ insert_index ].sequence != packet_sequence_num )
     {
      LOG( ("*** REORDER QUEUE OVERFLOW Packet %d Current %d ***", packet_sequence_num, current_sequence_num ) );
      //ConsoleInterface::postMessage( "*** REORDER QUEUE OVERFLOW Packet %d Current %d ***", packet_sequence_num, current_sequence_num  );
      while( (dequeue_sequence_num % size) != (insert_index + 1) )
       { 
        unsigned long dequeue_index;
        dequeue_index = ((unsigned long ) dequeue_sequence_num) % size;

        array[ dequeue_index ].in_use = _FALSE;
        array[ dequeue_index ].out_of_order = _FALSE;
      
        dequeue_sequence_num++; 
       } // ** while
      } // ** if
   } // ** if
 
  net_packet_size = (sizeof( NetPacket ) - _MAX_NET_PACKET_SIZE) + net_packet->packet_size; 
  memmove( (void *) &(array[ insert_index ].packet), (void *) net_packet, net_packet_size ); 

  array[ insert_index ].in_use = _TRUE;
  array[ insert_index ].out_of_order = _FALSE;
  array[ insert_index ].sequence = packet_sequence_num;

  if( packet_sequence_num > current_sequence_num)
   {
    //LOG( ("*** OUT OF ORDER  Packet %d Current %d ***", packet_sequence_num, current_sequence_num ) );
    //ConsoleInterface::postMessage( "*** OUT OF ORDER  Packet %d Current %d ***", packet_sequence_num, current_sequence_num );

    unsigned long sequence_counter = current_sequence_num;
    unsigned long sequence_index;

    while( sequence_counter != packet_sequence_num )
     {
      sequence_index = sequence_counter % size;

      array[ sequence_index ].in_use = _TRUE;
      array[ sequence_index ].out_of_order = _TRUE;
      sequence_counter++;
     }

    current_sequence_num = packet_sequence_num + 1;
    out_of_order_packets++;
   }
  else
   if( packet_sequence_num < current_sequence_num )
    { 
     out_of_order_packets++; 
     //LOG( ("*** OUT OF ORDER  Packet %d Current %d ***", packet_sequence_num, current_sequence_num ) );
     //ConsoleInterface::postMessage( "*** OUT OF ORDER  Packet %d Current %d ***", packet_sequence_num, current_sequence_num );
    }
   else
    {
     current_sequence_num++;
    }
 }

void ReorderQueue::dequeue( NetPacket *net_packet )
 {
  unsigned long dequeue_index;
  unsigned long net_packet_size;

  
  dequeue_index = ((unsigned long ) dequeue_sequence_num) % size;
  
  net_packet_size = (sizeof( NetPacket ) - _MAX_NET_PACKET_SIZE) + array[ dequeue_index ].packet.packet_size; 
  memmove( (void *) net_packet, (void *) &(array[ dequeue_index ].packet), net_packet_size );	       

  array[ dequeue_index ].in_use = _FALSE;
  array[ dequeue_index ].out_of_order = _FALSE;

  dequeue_sequence_num++;
 }

boolean ReorderQueue::isReady( void )
 {
  unsigned long dequeue_index;

  if( (packet_window_wait_flag) )
   {
    dequeue_index = ((unsigned long ) dequeue_sequence_num) % size;

    if( array[ dequeue_index ].out_of_order == _FALSE  )
     {
      packet_window_wait_flag = _FALSE;
      return( _TRUE );
     }

    if( packet_window_timer.count()  )
     {
      array[ dequeue_index ].in_use = _FALSE;
      array[ dequeue_index ].out_of_order = _FALSE;
      
      dequeue_sequence_num++;
      packet_window_wait_flag = _FALSE;
      lost_packets++;
     }
    return( _FALSE );
   }
  else
   {
    dequeue_index = ((unsigned long ) dequeue_sequence_num) % size;

    if( array[ dequeue_index ].in_use == _FALSE)
     { return(_FALSE);}

    if( array[ dequeue_index ].out_of_order == _TRUE  )
     {
      #ifdef _USE_PACKET_WINDOW
      packet_window_wait_flag = _TRUE;
      packet_window_timer.changePeriod( packet_window_estimate );
      return( _FALSE );
      #else
      array[ dequeue_index ].in_use = _FALSE;
      array[ dequeue_index ].out_of_order = _FALSE;
      
      dequeue_sequence_num++;
      packet_window_wait_flag = _FALSE;
      lost_packets++;
      return( _FALSE );
      #endif      
     }
   } 
  
  return( _TRUE );
 }

void ReorderQueue::getStats( float *packet_window_time, float *packet_window_estimate,
                             int *lost_packets, int *out_of_order_packets )
 {
  if( packet_window_time != NULL )
   { *packet_window_time = ReorderQueue::packet_window_time; }
 
  if( packet_window_estimate != NULL )
   { *packet_window_estimate = ReorderQueue::packet_window_estimate; }

  if( lost_packets != NULL )
   { *lost_packets = ReorderQueue::lost_packets; }

  if( out_of_order_packets != NULL )
   { *out_of_order_packets = ReorderQueue::out_of_order_packets; }
 }
