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
#ifndef __SDLDraw_hpp__
#define __SDLDraw_hpp__

#include <SDL.h>

#include "RGBColor.hpp"
#include "UIDraw.hpp"

// DirectDraw class declarations
//---------------------------------------------------------------------------
class SDLDraw: public UIDraw
{
private:
    SDL_Surface *FrontBuffer;

public:
    SDLDraw();
    virtual ~SDLDraw();

    bool setVideoMode(DWORD width, DWORD height, DWORD bpp, bool fullscreen);
    bool isDisplayModeAvailable(int width, int height, int bpp);
    bool lockDoubleBuffer(BYTE **DoubleBuffer);
    bool unlockDoubleBuffer();
    bool createFrameBuffer(DWORD width, DWORD height, DWORD bpp);
    void setGDIStatus(bool enable);
    void restoreAll();
    bool copyDoubleBufferandFlip();
    void setPalette(RGBColor *color);

    virtual bool isFullScreen() const;
}
; // end DirectDraw

#endif // end __UIDraw_hpp__
