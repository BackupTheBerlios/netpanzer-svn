/*
Copyright (C) 2003 by Matthias Braun<matze@braunis.de>
 
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
#ifndef __UNITSYNC_HPP__
#define __UNITSYNC_HPP__

#include "Classes/UnitBase.hpp"

class ClientSocket;

class UnitSync
{
public:
    UnitSync(ClientSocket * c);
    ~UnitSync();

    int getPercentComplete() const;
    
    bool sendNextUnit();

private:
    ClientSocket * client;
    size_t count;
    UnitID unitid;
    size_t unitstosync;
    UnitID lastunit;
};

#endif

