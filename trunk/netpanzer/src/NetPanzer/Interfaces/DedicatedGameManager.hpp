/*
Copyright (C) 2003 Ivo Danihelka <ivo@danihelka.net>
 
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
#ifndef _DEDICATEDGAMEMANAGER_HPP
#define _DEDICATEDGAMEMANAGER_HPP

#include "BaseGameManager.hpp"

class DedicatedGameManager : public BaseGameManager
{
protected:
    virtual void initializeVideoSubSystem();
    virtual void shutdownVideoSubSystem();
    
    virtual void initializeInputDevices();
    virtual void initializeGameConfig(const std::string& configfile);

    virtual void inputLoop();
public:
    virtual bool launchNetPanzerGame();
};

#endif
