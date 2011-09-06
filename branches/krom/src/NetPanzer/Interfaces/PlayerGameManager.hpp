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
#ifndef _PLAYERGAMEMANAGER_HPP
#define _PLAYERGAMEMANAGER_HPP

#include "Interfaces/BaseGameManager.hpp"
#include "Classes/ScreenSurface.hpp"

#include "InfoSocket.hpp"
#include "Heartbeat.hpp"

//#include "UI/FontManager.hpp"
//#include "Panels/TestPanel.hpp"

class SDLVideo;

class PlayerGameManager : public BaseGameManager
{
protected:
    virtual void initializeInputDevices();

    virtual void initializeVideoSubSystem();
    virtual void shutdownVideoSubSystem();

    virtual void initializeSoundSubSystem();

    virtual void shutdownNetworkSubSystem();

    virtual bool mainLoop();

    virtual void inputLoop();
    virtual void graphicsLoop();

public:
    PlayerGameManager();
    
    virtual bool launchNetPanzerGame();

    void launchMultiPlayerGame();
    void quitGame();

private:
    //UI::FontManager fontManager;
    SDLVideo* sdlVideo;

    Heartbeat * heartbeat;
    InfoSocket * infosocket;

    void initializeWindowSubSystem();
    void processSystemKeys();

    void hostMultiPlayerGame();
    void joinMultiPlayerGame();
};

#endif
