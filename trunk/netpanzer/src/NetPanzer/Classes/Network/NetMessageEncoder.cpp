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
#include "NetMessageEncoder.hpp"
 
 #include "string.h"

 #include "NetworkState.hpp"
 #include "Server.hpp"
 #include "Client.hpp"

NetMessageEncoder PUBLIC_MESSAGE_ENCODER;

 
 
 #define _SUB_PACKET_LIMIT 255 
 typedef unsigned char SubPacketType;
 

 void NetMessageEncoder::initalize( void ) 
  {
   resetEncoder();
  }
 
 void NetMessageEncoder::resetEncoder( void )
  {
   encode_message.message_class = _net_message_class_multi ;
   encode_message.message_count = 0;
   memset( encode_message.data, 0, _MULTI_PACKET_LIMIT );
   encode_message_index = 0;
  } 
 
 void NetMessageEncoder::encodeMessage( NetMessage *message, unsigned short size )
  {
   if( ( (encode_message_index + size + sizeof(SubPacketType)) > _MULTI_PACKET_LIMIT )
     )
    {
     encode_message.message_size = (unsigned short) encode_message_index;
     
     if( NetworkState::status == _network_state_server )
      {
       SERVER->sendMessage( &encode_message, encode_message.realSize(), 0 ); 
      }
     else
      {
       CLIENT->sendMessage( &encode_message, encode_message.realSize(), 0 ); 
      }

     resetEncoder( );
    } // ** if
  
  
   (SubPacketType) *(encode_message.data + encode_message_index) = (SubPacketType) size;
   
   encode_message_index += sizeof(SubPacketType);
   
   memmove( &encode_message.data[ encode_message_index ],
            message,
            size 
          );

   encode_message_index += size;
   encode_message.message_count++;
  }

 boolean NetMessageEncoder::encodeMessage( NetMessage *message, unsigned short size, MultiMessage **encoded_message )
  {
   if( ( (encode_message_index + size + sizeof(SubPacketType)) > _MULTI_PACKET_LIMIT )
     )
    {
     encode_message.message_size = (unsigned short) encode_message_index;
     
     *encoded_message = (MultiMessage *) &encode_message;
     return( _TRUE );
    } // ** if
  
  
   (SubPacketType) *(encode_message.data + encode_message_index) = (SubPacketType) size;
   
   encode_message_index += sizeof(SubPacketType);
   
   memmove( &encode_message.data[ encode_message_index ],
            message,
            size 
          );

   encode_message_index += size;
   encode_message.message_count++;
   
   *encoded_message = NULL;
   return( _FALSE );
  }

 void NetMessageEncoder::getEncodeMessage( MultiMessage **message )
  {
   if ( encode_message.message_count > 0 )
    {
     encode_message.message_size = (unsigned short) encode_message_index;
     
     *message = (MultiMessage *) &encode_message;
    } // ** if
   else
    {
     *message = NULL;
    } 
  } 

 void NetMessageEncoder::setDecodeMessage( MultiMessage *message )
  {
   decode_message.message_count = message->message_count;
   decode_message.message_size = message->message_size;
   memmove( decode_message.data, message->data, message->message_size );
   decode_message_index = 0;
   decode_current_count = 0;
  } 

 boolean NetMessageEncoder::decodeMessage( NetMessage **message )
  {
   SubPacketType message_size;

   if ( decode_current_count == decode_message.message_count )
    return( _FALSE );

   message_size = (SubPacketType) *(decode_message.data + decode_message_index);
   decode_message_index += sizeof( SubPacketType );
    
   *message = (NetMessage *) (decode_message.data + decode_message_index);       
   decode_message_index += message_size;
   
   decode_current_count++;
   return( _TRUE );
  }

void NetMessageEncoder::sendEncodedMessage( void )
 {
  if ( encode_message.message_count > 0 )
   {
    encode_message.message_size = (unsigned short) encode_message_index;
     
    if( NetworkState::status == _network_state_server )
     {
      SERVER->sendMessage( &encode_message, encode_message.realSize(), 0 ); 
     }
    else
     {
      CLIENT->sendMessage( &encode_message, encode_message.realSize(), 0 ); 
     }

    resetEncoder( );
   } // ** if 
 }
 
