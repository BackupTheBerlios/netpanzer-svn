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
#ifndef __WINSOCKMESSAGE_HPP
#define __WINSOCKMESSAGE_HPP

#include <winsock.h>

#include "NetPacket.hpp"

enum { _net_message_id_basic_info_request,
       _net_message_id_extended_info_request,
       _net_message_id_udp_request,
       _net_message_id_client_udp_address,
       _net_message_id_basic_game_info,
       _net_message_id_extended_game_info
     };


#pragma pack(1)

class BasicInfoRequest : public NetMessage
{
public:
    char codeword[24];
    DWORD version;
    BasicInfoRequest()
    {
        message_class = _net_message_class_winsock;
        message_id = _net_message_id_basic_info_request;
    }
    
};


class ExtendedInfoRequest : public NetMessage
{
public:
    char codeword[24];
    DWORD version;
    ExtendedInfoRequest()
    {
        message_class = _net_message_class_winsock;
        message_id = _net_message_id_extended_game_info;
    }

};


class UDPAddressRequest : public NetMessage
{
public:
    DWORD wsockID;

    UDPAddressRequest()
    {
        message_class = _net_message_class_winsock;
        message_id = _net_message_id_udp_request;
    }
    
};


class ClientUDPAddress : public NetMessage
{
public:
    DWORD wsockID;

    ClientUDPAddress()
    {
        message_class = _net_message_class_winsock;
        message_id = _net_message_id_client_udp_address;
    }
    
};


class BasicGameInfo : public NetMessage
{

public:
    unsigned char MaxPlayers;
    unsigned char CurrPlayers;
    char pad1;
    char MapName[32];
    char PlayerName[32];
    char GameType[32];

    BasicGameInfo()
    {
        message_class = _net_message_class_winsock;
        message_id = _net_message_id_basic_game_info;
    }

};


class ExtendedGameInfo : public NetMessage
{
public:
    ExtendedGameInfo()
    {
        message_class = _net_message_class_winsock;
        message_id = _net_message_id_extended_game_info;
    }


    //TBD

};


#pragma pack()


#endif __WINSOCKMESSAGE_HPP
