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
 *
 * Created on February 15, 2013, 10:40 AM
 */

#ifndef SCENEMANAGER_HPP
#define	SCENEMANAGER_HPP

class Scene;
class InputHandler;

class SceneManager
{
public:
    static bool init( Scene * first_scene );
    static void deinit();
    
    static bool run();
    
    static void endLoop();
    
    static void setNextScene( Scene * next_scene );
};


#endif	/* SCENEMANAGER_HPP */

