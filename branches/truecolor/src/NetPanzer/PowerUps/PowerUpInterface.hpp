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
#ifndef _POWERUP_INTERFACE_HPP
#define _POWERUP_INTERFACE_HPP

#include "Core/CoreTypes.hpp"

class PowerUp;
class NetMessage;
class ClientSocket;
class SpriteSorter;

class PowerUpInterface
{
public:
    static void initialize();
    static void resetLogic();

    static void updateState();
    static void offloadGraphics( SpriteSorter &sorter );

    static void processNetMessages(const NetMessage* message );

    static void syncPowerUps( ClientSocket * client );
};

#endif // ** _POWERUP_INTERFACE_HPP
