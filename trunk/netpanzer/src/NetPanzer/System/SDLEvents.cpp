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
#include <config.h>

#include <SDL.h>
#include "KeyboardInterface.hpp"
#include "MouseInterface.hpp"
#include "cMouse.hpp"

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
            switch(event.button.button) {
            case 1:
                MouseInterface::setLeftButtonDown();
                cMouse::setButtonMask(LMOUSE_BUTTON_MASK);
                break;
            case 3:
                MouseInterface::setRightButtonDown();
                cMouse::setButtonMask(RMOUSE_BUTTON_MASK);
                break;
            case 2:
                MouseInterface::setMiddleButtonDown();
                cMouse::setButtonMask(MMOUSE_BUTTON_MASK);
                break;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            switch(event.button.button) {
            case 1:
                MouseInterface::setLeftButtonUp();
                cMouse::clearButtonMask(!LMOUSE_BUTTON_MASK);
                break;
            case 3:
                MouseInterface::setRightButtonUp();
                cMouse::clearButtonMask(!RMOUSE_BUTTON_MASK);
                break;
            case 2:
                MouseInterface::setMiddleButtonUp();
                cMouse::clearButtonMask(!MMOUSE_BUTTON_MASK);
                break;
            }
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
                    KeyboardInterface::putChar(event.key.keysym.sym);
                }
                break;
            }
        case SDL_KEYUP:
            KeyboardInterface::keyReleased(event.key.keysym.sym);
            break;
        }
    }

    return false;
}
