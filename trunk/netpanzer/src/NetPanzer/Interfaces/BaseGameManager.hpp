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
#ifndef _BASEGAMEMANAGER_HPP
#define _BASEGAMEMANAGER_HPP

#include <string>

class BaseGameManager
{
private:
    static const int TIMEINTERVAL = 20;
    void loadGameData();
    void LoadUnitSurfaces();
    void sleeping();
protected:
    virtual void initializeNetworkSubSystem();
    virtual void shutdownNetworkSubSystem();

    virtual void initializeInputDevices();
    virtual void shutdownInputDevices();

    virtual void initializeVideoSubSystem() = 0;
    virtual void shutdownVideoSubSystem() = 0;

    virtual void initializeSoundSubSystem();
    virtual void shutdownSoundSubSystem();

    virtual void initializeGameConfig(const std::string& configfile);
    virtual void shutdownGameConfig();

    // initialize all static objects / interfaces;
    virtual void initializeGameObjects();
    virtual void shutdownGameObjects();

    virtual void simLoop();
    virtual void inputLoop();
    virtual void graphicsLoop();

    virtual void shutdownSubSystems();
public:
    virtual void shutdown();
    virtual ~BaseGameManager() { }

    virtual void initialize(const std::string& configfile = "");

    // cyclic executive for loop back server / client
    virtual void mainLoop();

    virtual bool launchNetPanzerGame() = 0;
};

#endif
