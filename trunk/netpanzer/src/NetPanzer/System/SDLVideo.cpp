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

#include <iostream>

#include "Util/Log.hpp"
#include "Util/Exception.hpp"
#include "SDLVideo.hpp"

SDLVideo* Screen; // get rid of this later...

SDLVideo::SDLVideo()
        : frontBuffer(0), backBuffer(0)
{
    if(SDL_InitSubSystem(SDL_INIT_VIDEO)) {
        throw Exception("Couldn't initialize SDL_video subsystem: %s",
                SDL_GetError());
    }
    // XXX unfortunately SDL initializes the keyboard again :-/
    SDL_EnableUNICODE(1);
}

SDLVideo::~SDLVideo()
{
    if(backBuffer)
        SDL_FreeSurface(backBuffer);
    
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void SDLVideo::setVideoMode(int width, int height, int bpp, Uint32 flags)
{
    // eventually delete old backbuffer
    if(frontBuffer && !(frontBuffer->flags & SDL_DOUBLEBUF)) {
        if(backBuffer)
            SDL_FreeSurface(backBuffer);
    }
    
    flags |= SDL_HWPALETTE | SDL_ANYFORMAT;
    frontBuffer = SDL_SetVideoMode(width, height, bpp, flags);
    if(!frontBuffer)
        throw Exception("Couldn't set display mode (%dx%d, %X): %s",
                        width, height, flags, SDL_GetError());

    if(! (frontBuffer->flags & SDL_DOUBLEBUF)) {
        backBuffer = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height,
                bpp, 0, 0, 0, 0);
        if(!backBuffer)
            throw Exception("Couldn't create backBuffer");
    } else {
        backBuffer = frontBuffer;
    }

    // let's scare the mouse :)
    SDL_ShowCursor(SDL_DISABLE);
    // and set a window title
    SDL_WM_SetCaption("NetPanzer", 0);
}

bool SDLVideo::isDisplayModeAvailable(int width, int height, int bpp,
                                     Uint32 flags)
{
    flags |= SDL_HWPALETTE | SDL_ANYFORMAT;

    return SDL_VideoModeOK(width, height, bpp, flags);
}

void SDLVideo::lockDoubleBuffer(unsigned char **buffer)
{
    if(SDL_MUSTLOCK(backBuffer) && SDL_LockSurface(frontBuffer))
        throw Exception("Couldn't lock display buffer");

    *buffer = (unsigned char *)backBuffer->pixels;
}

void SDLVideo::unlockDoubleBuffer()
{
    if(SDL_MUSTLOCK(backBuffer))
        SDL_UnlockSurface(backBuffer);
}

void SDLVideo::copyDoubleBufferandFlip()
{
    if(! (frontBuffer->flags & SDL_DOUBLEBUF)) {
        if(SDL_MUSTLOCK(frontBuffer) && SDL_LockSurface(frontBuffer))
            throw Exception("Couldn't lock display buffer");
        SDL_BlitSurface(backBuffer, 0, frontBuffer, 0);
        if(SDL_MUSTLOCK(frontBuffer))
            SDL_UnlockSurface(frontBuffer);
    }

    if (SDL_Flip(frontBuffer))
        throw Exception("Error while swapping double buffer");        
}

void SDLVideo::setPalette(RGBColor *color)
{
    SDL_Color c[256];
    int i;

    for(i=0; i<256; i++) {
        c[i].r = color[i].red;
        c[i].g = color[i].green;
        c[i].b = color[i].blue;
    }

    SDL_SetColors(backBuffer, c, 0, 256);
    if(frontBuffer != backBuffer && frontBuffer->format->BitsPerPixel == 8)
        SDL_SetColors(frontBuffer, c, 0, 256);
}

SDL_Surface* SDLVideo::getSurface()
{
    return backBuffer;
}

