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
#include "SDLDraw.hpp"

bool SDLDraw::initialize()
{
	return true;
}

void SDLDraw::shutdown()
{
}

bool SDLDraw::setVideoMode(DWORD width, DWORD height, DWORD bpp, BYTE mode_flags)
{
  FrontBuffer = SDL_SetVideoMode(width, height, bpp, SDL_FULLSCREEN /*| SDL_DOUBLEBUF | SDL_HWSURFACE*/);
  if(FrontBuffer==NULL)
	  return FALSE;
  return TRUE;
}

bool SDLDraw::isDisplayModeAvailable(int width, int height, int bpp)
{
	if(SDL_VideoModeOK(width, height, bpp, 0 /*SDL_FULLSCREEN | SDL_DOUBLEBUF | SDL_HWSURFACE*/))
		return TRUE;

	return FALSE;
}

bool SDLDraw::lockDoubleBuffer(BYTE **DoubleBuffer)
{
	if(SDL_LockSurface(FrontBuffer)<0)
		return FALSE;

	*DoubleBuffer = (unsigned char *)FrontBuffer->pixels;

	return TRUE;
}

bool SDLDraw::unlockDoubleBuffer()
{
	SDL_LockSurface(FrontBuffer);
	return TRUE;
}

bool SDLDraw::createFrameBuffer(DWORD width, DWORD height, DWORD bpp)
{
	return TRUE;
}

void SDLDraw::setGDIStatus(bool enable)
{
}

void SDLDraw::restoreAll()
{
}

bool SDLDraw::copyDoubleBufferandFlip()
{
	if(SDL_Flip(FrontBuffer)==0)
		return TRUE;
	return FALSE;
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
