/*
Copyright (C) 1998 Pyrosoft Inc. (www.pyrosoftgames.com), Matthew Bogue
 
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

#include "System/SDLVideo.hpp"
#include "ScreenSurface.hpp"

ScreenSurface* screen = 0;

ScreenSurface::ScreenSurface(SDLVideo* newdraw)
        : Surface(), draw(newdraw)
{
    numFrames = 1;

    frames=new SDL_Surface*;
    frames[0]=newdraw->getSurface();
    frames[0]->refcount++;
    thisFrame=frames[0];
    curFrame=0.0;

}

void ScreenSurface::lock()
{
    draw->lockDoubleBuffer();
}

void ScreenSurface::unlock() 
{
    draw->unlockDoubleBuffer();
}

void ScreenSurface::copyToVideoFlip()
{
    draw->copyDoubleBufferandFlip();
}
