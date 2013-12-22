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

#include "System/SDLVideo.hpp"
#include "Classes/ScreenSurface.hpp"
#include "Util/Log.hpp"
#include "System/SDLSound.hpp"

ScreenSurface* screen = 0;

ScreenSurface::ScreenSurface(SDLVideo* newdraw, int width, int height)
        : Surface(), draw(newdraw)
{
    twidth =width;
    theight=height;
    tpitch = width;

}

ScreenSurface::~ScreenSurface()
{
    mem = 0;
}

void ScreenSurface::lock()
{
    assert(getDoesExist() == false);
    draw->lockDoubleBuffer( (unsigned char **) &mem );
    tpitch = draw->getSurface()->pitch;
//    mem = frame0;
}

void ScreenSurface::unlock() 
{
    assert(getDoesExist() == true);
    draw->unlockDoubleBuffer();
    mem = 0;
}

void ScreenSurface::copyToVideoFlip()
{
    draw->copyDoubleBufferandFlip();
}
