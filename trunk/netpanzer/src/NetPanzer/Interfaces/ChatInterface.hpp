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
#ifndef _CHATINTERFACE_HPP
#define _CHATINTERFACE_HPP

#include "SystemNetMessage.hpp"

class ChatInterface
 {
  protected:
   static SystemChatMesgRequest current_chat_mesg;
   static void (* addChatString)( const char *message_text );  
   
   static void chatMessageRequest( NetMessage *message );
   static void chatMessage( NetMessage *message );

  public:
   
   static void processChatMessages( NetMessage *message );
   
   // ** ChatView Interface Methods
   static void setNewMessageCallBack( void (* addStringCallBack)( const char *message_text ) ); 
   static void setMessageScopeAll( void );
   static void setMessageScopeAllies( void );
   static void setMessageScopeEnemies( void );
   static void setMessageScopeServer( void );
   static void sendCurrentMessage( const char *message_text  );


 };

#endif // ** _CHATINTERFACE_HPP
