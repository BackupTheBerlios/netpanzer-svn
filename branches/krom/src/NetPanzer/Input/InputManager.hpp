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

#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

class Action;

class InputManager
{
public:
    static void initialize();
    static void deinitialize();

    static bool addAction( Action * action);
    static const Action* getAction(const std::string& name);

    static void setButtonPress(int button);
    static void setButtonRelease(int button);

};

#endif // INPUTMANAGER_HPP
