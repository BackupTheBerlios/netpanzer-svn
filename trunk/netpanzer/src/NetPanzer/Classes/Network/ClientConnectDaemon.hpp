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
#ifndef _CLIENTCONNECTDAEMON_HPP
#define _CLIENTCONNECTDAEMON_HPP

#include "PlayerState.hpp"
#include "NetPacket.hpp"
#include "ArrayUtil/Timer.hpp"

class ClientConnectDaemon
{
protected:
    static Timer failure_display_timer;
    static Timer time_out_timer;
    static int   time_out_counter;
    static unsigned char connection_state;

    static void netMessageLinkAck( NetMessage *message );
    static void netMessageConnectProcessUpdate( NetMessage *message );
    static void netMessageConnectProcessMessage(	NetMessage *message );
    static void netMessageConnectServerDisconnect( NetMessage *message );

    static void connectFsm( NetMessage *message );
    static void connectProcess( NetMessage *message);

    static void connectFailureResult( unsigned char result_code );

public:

    static void startConnectDaemon( void );

    static void shutdownConnectDaemon( void );

    static void startConnectionProcess();

    static void connectProcess( void );

    static void processNetMessage( NetMessage *message);

    static void serverConnectionBroken( void );

};

#endif // ** _CLIENTCONNECTDAEMON_HPP
