/*
Copyright (C) 2011 by Aaron Perez <aaronps@gmail.com>

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

#include "InputManager.hpp"
#include "Action.hpp"

#include <string>
#include <map>

#define MAX_BUTTONS (512)

typedef std::map<std::string, Action *> ActionMap;

static bool * buttons_state = 0;
static ActionMap all_actions = 0;

void InputManager::initialize()
{
    buttons_state = new bool[MAX_BUTTONS];
    for ( int n = MAX_BUTTONS; n; )
    {
        buttons_state[--n] = false;
    }

    all_actions = new ActionMap();

}

void InputManager::deinitialize()
{
    if ( buttons_state )
    {
        delete[] buttons_state;
        buttons_state = 0;
    }

    if ( all_actions )
    {
        ActionMap::iterator iter = all_actions->begin();
        while ( iter != all_actions->end() )
        {
            delete iter->second;
            iter++;
        }
        delete all_actions;
        all_actions = 0;
    }
}

void InputManager::setButtonPress(int button)
{
    if ( button < MAX_BUTTONS )
    {
        buttons_state[button] = true;
    }
}

void InputManager::setButtonRelease(int button)
{
    if ( button < MAX_BUTTONS )
    {
        buttons_state[button] = false;
    }

}
