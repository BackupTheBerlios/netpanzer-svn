/* 
Copyright (C) 2008 by Aaron Perez <aaronps@gmail.com>

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
 *
 * Created on October 22, 2008, 11:57 PM
 */

#ifndef _RESOURCEMANAGERMESSAGES_HPP
#define	_RESOURCEMANAGERMESSAGES_HPP

#include "config.h"
#include "Core/CoreTypes.hpp"
#include "Classes/Network/NetMessage.hpp"

enum
{
    _resource_message_id_sync_flag,
    _resource_message_id_release_flag
};

class ResourceManagerSyncFlagMessage : public NetMessage
{
private:
    FlagID flag;

public:
    Uint8 flagdata[20*14]; // 20*14 = flagsize 8bpp = 280

    ResourceManagerSyncFlagMessage()
    {
        message_class = _net_message_class_resource;
        message_id = _resource_message_id_sync_flag;
    }
    
    void setFlagID(FlagID flag)
    {
        this->flag = flag;
    }
    
    FlagID getFlagID()
    {
        return flag;
    }
} __attribute__((packed));

class ResourceManagerReleaseFlagMessage : public NetMessage
{
private:
    FlagID flag;

public:
    ResourceManagerReleaseFlagMessage()
    {
        message_class = _net_message_class_resource;
        message_id = _resource_message_id_release_flag;
    }
    
    void setFlagID(FlagID flag)
    {
        this->flag = flag;
    }
    
    FlagID getFlagID()
    {
        return flag;
    }
} __attribute__((packed));

#endif	/* _RESOURCEMANAGERMESSAGES_HPP */

