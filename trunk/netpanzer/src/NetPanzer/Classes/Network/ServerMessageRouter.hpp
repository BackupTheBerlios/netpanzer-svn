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
#ifndef _SERVERMESSAGEROUTER_HPP
#define _SERVERMESSAGEROUTER_HPP

#include "PObject.hpp"
#include "NetPacket.hpp"
#include "NetMessageEncoder.hpp"

class ServerMessageRouter : public PObject
 {
  protected:
   static NetMessage *temp_message;
   static NetMessageEncoder message_encoder;
   static void classTerminalMessages( NetMessage *message );

  public:
   
   static void initialize( void );
   static void cleanUp( void );
   static void routeMessages( void );

 };

#endif // ** _SERVERMESSAGEROUTER_HPP
