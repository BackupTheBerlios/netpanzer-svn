
#include "SceneManager.hpp"
#include "Scene.hpp"



#include "System/SDLEvents.hpp"
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

static Scene * current_scene = 0;
static bool shall_run = false;

bool SceneManager::init(Scene* first_scene)
{
    current_scene = first_scene;
    
    shall_run = current_scene != 0;
    
    return shall_run;
}

bool SceneManager::run()
{
    handleSDLEvents();
    
    screen->lock();
    
    current_scene->draw();
    
    MouseInterface::draw(*screen);
    
    screen->unlock();
    screen->copyToVideoFlip();
    
    current_scene->input();
    
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
}
