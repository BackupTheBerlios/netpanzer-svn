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
#include "Exception.hpp"
#include "SDLDraw.hpp"

SDLDraw::SDLDraw()
        : FrontBuffer(0)
{
    SDL_InitSubSystem(SDL_INIT_VIDEO);
    // XXX unfortunately SDL initializes the keyboard again :-/
    SDL_EnableUNICODE(1);
}

SDLDraw::~SDLDraw()
{
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void SDLDraw::setVideoMode(int width, int height, int bpp, bool fullscreen)
{
    Uint32 flags = 0;
    flags |= fullscreen ? SDL_FULLSCREEN : 0;
    flags |= SDL_DOUBLEBUF | SDL_HWSURFACE | SDL_HWPALETTE;
    FrontBuffer = SDL_SetVideoMode(width, height, bpp, flags);
    if(!FrontBuffer)
        throw Exception("Couldn't set display mode (%dx%d:%d, %d)",
                        width, height, bpp, flags);

    curWidth = width;
    curHeight = height;
    curBPP = bpp;

    // let's scare the mouse :)
    SDL_ShowCursor(SDL_DISABLE);
    // and set a window title
    SDL_WM_SetCaption("NetPanzer", 0);
}

bool SDLDraw::isDisplayModeAvailable(int width, int height, int bpp,
                                     bool fullscreen)
{
    Uint32 flags = fullscreen ? SDL_FULLSCREEN : 0;
    flags |= SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_HWPALETTE;

    return SDL_VideoModeOK(width, height, bpp, flags);
}

void SDLDraw::lockDoubleBuffer(unsigned char **DoubleBuffer)
{
    if(SDL_MUSTLOCK(FrontBuffer) && !SDL_LockSurface(FrontBuffer))
        throw Exception("Couldn't lock display buffer");

    *DoubleBuffer = (unsigned char *)FrontBuffer->pixels;
}

void SDLDraw::unlockDoubleBuffer()
{
    if(SDL_MUSTLOCK(FrontBuffer))
        SDL_UnlockSurface(FrontBuffer);
}

void SDLDraw::copyDoubleBufferandFlip()
{
    if(FrontBuffer->flags & SDL_DOUBLEBUF) {
        if (!SDL_Flip(FrontBuffer))
            throw Exception("Error while swapping double buffer");
    } else {
        SDL_UpdateRect(FrontBuffer, 0, 0, 0, 0);
    }
}

void SDLDraw::setPalette(RGBColor *color)
{
    SDL_Color c[256];
    int i;

    for(i=0; i<256; i++) {
        c[i].r = color[i].red;
        c[i].g = color[i].green;
        c[i].b = color[i].blue;
    }

    SDL_SetColors(FrontBuffer, c, 0, 256);
}

bool SDLDraw::isFullScreen() const
{
    return FrontBuffer->flags & SDL_FULLSCREEN;
}

