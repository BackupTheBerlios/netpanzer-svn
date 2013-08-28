/*
Copyright (C) 2013 Netpanzer Team. (www.netpanzer.org), Aaron Perez

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

#include "SceneManager.hpp"
#include "Scene.hpp"
#include "GameInput/InputManager.hpp"
#include "GameInput/InputHandler.hpp"

#include "SDL.h"

#define TIMEINTERVAL (20)
static void loopSleep()
{
    static Uint32 nextTime = 0;

    Uint32 now = SDL_GetTicks();
    if (now < nextTime)
    {
        SDL_Delay(nextTime - now);
    }
    nextTime += TIMEINTERVAL;
}

#include "Classes/ScreenSurface.hpp"
#include "Interfaces/MouseInterface.hpp"
#include "Util/Log.hpp"
#include "GameInput/InputState.hpp"
#include "Actions/ActionManager.hpp"
#include "Interfaces/GameConfig.hpp"
#include "System/SDLVideo.hpp"
#include "2D/Palette.hpp"
#include "Interfaces/GameManager.hpp"
#include "System/SDLVideo.hpp"

static Scene * current_scene = 0;
static bool shall_run = false;

class DefaultInputHandler : public GameInput::InputHandler
{
private:
    bool handleInput(GameInput::InputState * input)
    {
        if ( input->hasAction(GameInput::Action_Quit) )
        {
            ActionManager::runAction("quit");
        }
        
        if ( input->hasAction(GameInput::Action_BecomeActive) )
        {
            if ( GameConfig::video_fullscreen )
            {
                Screen->setPalette((SDL_Color*)Palette::color);
            }
        }
        
        if ( input->hasAction(GameInput::Action_ResizeWindow) )
        {
            GameConfig::video_width = GameInput::InputManager::getResizeWidth();
            GameConfig::video_height= GameInput::InputManager::getResizeHeight();
            GameManager::setVideoMode();
        }
        
        if ( input->hasAction(GameInput::Action_ScreenShot) )
        {
            Screen->doScreenshoot();
        }
        
        return true;
    }
};

static DefaultInputHandler * dih = 0;

bool SceneManager::init(Scene* first_scene)
{
    current_scene = first_scene;

    GameInput::InputManager::initialize();
    dih = new DefaultInputHandler();
    GameInput::InputManager::pushInputHandler(dih);
    
    shall_run = current_scene != 0;
    return shall_run;
}

bool SceneManager::run()
{
    screen->lock();
    
    current_scene->draw();
    
    MouseInterface::draw(*screen);
    
    screen->unlock();
    screen->copyToVideoFlip();
    
    GameInput::InputManager::logic();
    
    current_scene->logic();
    
    loopSleep();
        
    return shall_run;
}

void SceneManager::endLoop()
{
    shall_run = false;
}

void SceneManager::setNextScene(Scene* next_scene)
{
    current_scene = next_scene;
    next_scene->onEnterCurrentScene();
}
