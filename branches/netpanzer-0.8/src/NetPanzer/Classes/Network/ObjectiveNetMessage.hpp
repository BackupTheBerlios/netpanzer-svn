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
#ifndef _OBJECTIVE_NET_MESSAGE_HPP
#define _OBJECTIVE_NET_MESSAGE_HPP

#include "NetMessage.hpp"

#include "ObjectiveMessageTypes.hpp"

enum { _net_message_id_occupation_status_update,
       _net_message_id_objective_sync };

#ifdef MSVC
#pragma pack(1)
#endif

class ObjectiveOccupationUpdate : public NetMessage
{
public:
    UpdateOccupationsStatus status_update;

    ObjectiveOccupationUpdate()
    {
        message_class = _net_message_class_objective;
        message_id = _net_message_id_occupation_status_update;
    }
}
__attribute__((packed));

class ObjectiveSyncMesg : public NetMessage
{
public:
    SyncObjective sync_data;

    ObjectiveSyncMesg()
    {
        message_class = _net_message_class_objective;
        message_id = _net_message_id_objective_sync;
    }
} __attribute__((packed));

#ifdef MSVC
#pragma pack()
#endif

#endif // ** _OBJECTIVE_NET_MESSAGE_HPP
