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

ScreenSurface::ScreenSurface(SDLVideo* newdraw, int width, int height, int bpp)
        : Surface(), draw(newdraw)
{
    myMem = false;
    frameCount = 1;

    pix = iXY(width, height);
    stride = width;
    center = iXY(width/2, height/2);
    frameCount = 1;
    doesExist = false;
}

void ScreenSurface::lock()
{
    assert(doesExist == false);
    draw->lockDoubleBuffer( (unsigned char **) &frame0 );
    mem = frame0;
    doesExist = true;
}

void ScreenSurface::unlock() 
{
    assert(doesExist == true);
    draw->unlockDoubleBuffer();
    doesExist = false;
}

void ScreenSurface::copyToVideoFlip()
{
    draw->copyDoubleBufferandFlip();
}
