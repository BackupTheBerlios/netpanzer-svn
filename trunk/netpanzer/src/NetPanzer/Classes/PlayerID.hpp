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
#ifndef _PLAYERID_HPP
#define _PLAYERID_HPP

#include <string.h>
#include "PObject.hpp"
#include "UILib/Network/Client.hpp"

class PlayerID
{
protected:
  	unsigned short index_id;
	Client::ID networkid;

public:
  	PlayerID( ) 
		: index_id(0xFFFF), networkid(0xFFFFFFFF)
   	{ }
  
	PlayerID( unsigned short index, Client::ID newid ) 
		: index_id(index), networkid(newid)
	{ }
     
	inline void setIndex( unsigned short index )
	{ index_id = index; }

	inline unsigned short getIndex() const
	{ return( index_id ); }

	inline void setNetworkID(Client::ID newid)
	{ networkid = newid; }
  
	inline Client::ID getNetworkID() const
	{ return(networkid); }

	inline void operator=(const PlayerID &rhs)
	{
		index_id = rhs.index_id;
		networkid = rhs.networkid;
	}
 
	inline bool operator==(const PlayerID &rhs) const
	{
		if ( (index_id == rhs.index_id) && (networkid == rhs.networkid) )
			return true;
		
		return false;
	}
};

#endif // ** _PLAYERID_HPP
