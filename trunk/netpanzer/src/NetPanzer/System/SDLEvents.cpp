/*
   Copyright (C) 2004 Matthias Braun <matze@braunis.de>,
 
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


#include <ctype.h>
#include "SDL.h"
#include "Interfaces/KeyboardInterface.hpp"
#include "Interfaces/MouseInterface.hpp"
#include "SDLVideo.hpp"
#include "2D/Palette.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Interfaces/GameManager.hpp"
#include "Util/Log.hpp"

bool handleSDLEvents()
{
    static SDL_Event event;

    KeyboardInterface::sampleKeyboard();
    while(SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            return true;
            break;
        case SDL_MOUSEBUTTONDOWN:
            MouseInterface::onMouseButtonDown(&event.button);
            break;
        case SDL_MOUSEBUTTONUP:
            MouseInterface::onMouseButtonUp(&event.button);
            break;
        case SDL_MOUSEMOTION:
            MouseInterface::onMouseMoved(&event.motion);
            break;
        case SDL_KEYDOWN: {
                KeyboardInterface::keyPressed(event.key.keysym.sym);
                char c = event.key.keysym.unicode & 0x7F;
                if (isprint(c)) {
                    KeyboardInterface::putChar(c);
                } else {
                    // it's not a normal char put the 0 into the char buffer to
                    // indicate extended chars...
                    KeyboardInterface::putChar(0);
                    c = (event.key.keysym.unicode & 0x7F) + 96;
                    // if key into a to z then put unicode and 96
                    if ((event.key.keysym.sym > 96) && (event.key.keysym.sym < 123)) 
                        KeyboardInterface::putChar(c);
                    else KeyboardInterface::putChar(event.key.keysym.sym);
                }
                break;
            }
        case SDL_KEYUP:
            KeyboardInterface::keyReleased(event.key.keysym.sym);
            break;
       
        case SDL_ACTIVEEVENT:
            if ( (event.active.state&SDL_APPACTIVE)
                 && (event.active.gain==1)
                 && GameConfig::video_fullscreen )
                Screen->setPalette(Palette::color);
             break;
        case SDL_VIDEORESIZE:
            GameConfig::video_width = event.resize.w;
            GameConfig::video_height = event.resize.h;
            GameManager::setVideoMode();
            break;
        }
    }

    return false;
}