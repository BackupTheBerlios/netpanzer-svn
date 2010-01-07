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

#include "Util/Log.hpp"
#include "Util/Exception.hpp"
#include "SDLVideo.hpp"
#include "2D/Surface.hpp"
#include <stdlib.h>
#ifdef _WIN32
  #include "Interfaces/GameConfig.hpp"
#endif

#define flaginfo(f,xxflag) if ( f & xxflag ) LOGGER.warning("\t" #xxflag)

SDLVideo* Screen; // get rid of this later...

static int best_bpp = 0;

static void
logVideoInfo()
{
        const SDL_VideoInfo *v = SDL_GetVideoInfo();
        LOGGER.warning("Video Info:");
        LOGGER.warning("\tHardware Surfaces: %d", v->hw_available);
        LOGGER.warning("\tWindow Manager: %d", v->wm_available);
        LOGGER.warning("\tHardware -> Hardware blit accelerated: %d", v->blit_hw);
        LOGGER.warning("\tHardware Colorkey -> Hardware blit accelerated: %d", v->blit_hw_CC);
        LOGGER.warning("\tHardware Alpha -> Hardware blit accelerated: %d", v->blit_hw_A);
        LOGGER.warning("\tSoftware -> Hardware blit accelerated: %d", v->blit_sw);
        LOGGER.warning("\tSoftware Colorkey -> Hardware blit accelerated: %d", v->blit_sw_CC);
        LOGGER.warning("\tSoftware Alpha -> Hardware blit accelerated: %d", v->blit_sw_A);
        LOGGER.warning("\tFill color accelerated: %d", v->blit_fill);
        LOGGER.warning("\tHardware memory: %d", v->video_mem);
        LOGGER.warning("\tSize: %d x %d", v->current_w, v->current_h);
        LOGGER.warning("\tbpp: %d (%d)",v->vfmt->BitsPerPixel, v->vfmt->BytesPerPixel);
}

static void
logAvailableModes(Uint32 flags)
{
    SDL_Rect ** modes = SDL_ListModes(NULL, flags | SDL_FULLSCREEN);
    if ( modes == (SDL_Rect**) 0 )
    {
        LOGGER.warning("No modes available");
    }
    else if ( modes == (SDL_Rect**)-1 )
    {
        LOGGER.warning("All modes available");
    }
    else
    {
        for ( int n = 0; modes[n]; ++n )
        {
            LOGGER.warning("Mode: %d x %d", modes[n]->w, modes[n]->h);
        }
    }
}

static void
logFlags(Uint32 f)
{
    flaginfo(f,SDL_SWSURFACE);
    flaginfo(f,SDL_HWSURFACE);
    flaginfo(f,SDL_ASYNCBLIT);
    flaginfo(f,SDL_ANYFORMAT);
    flaginfo(f,SDL_HWPALETTE);
    flaginfo(f,SDL_DOUBLEBUF);
    flaginfo(f,SDL_FULLSCREEN);
    flaginfo(f,SDL_OPENGL);
    flaginfo(f,SDL_OPENGLBLIT);
    flaginfo(f,SDL_RESIZABLE);
    flaginfo(f,SDL_HWACCEL);
    flaginfo(f,SDL_SRCCOLORKEY);
    flaginfo(f,SDL_RLEACCEL);
    flaginfo(f,SDL_SRCALPHA);
    flaginfo(f,SDL_PREALLOC);
}

SDLVideo::SDLVideo()
        : frontBuffer(0), backBuffer(0)
{
#ifdef _WIN32
    if ( gameconfig->usedirectx )
    {
        putenv("SDL_VIDEODRIVER=directx");
    }
#endif
    if ( SDL_InitSubSystem(SDL_INIT_VIDEO) )
    {
        throw Exception("Couldn't initialize SDL_video subsystem: %s",
                         SDL_GetError()
                        );
    }

    if ( ! best_bpp )
    {
        const SDL_VideoInfo *v = SDL_GetVideoInfo();
        if ( v )
        {
            best_bpp = v->vfmt->BitsPerPixel;
        }
    }

    logVideoInfo();

    // XXX unfortunately SDL initializes the keyboard again :-/
    SDL_EnableUNICODE(1);
}

SDLVideo::~SDLVideo()
{
    if ( backBuffer )
    {
        SDL_FreeSurface(backBuffer);
    }
    
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

static bool getNearestFullScreenMode(int flags, int* width, int* height)
{
    SDL_Rect** modes = SDL_ListModes(0, flags);
    if ( modes == 0 )
    {
        return false;
    }
    else if ( modes == (SDL_Rect**)-1 )
    {
        return true;
    }
    else
    {
        unsigned int mode_size = (*width) * (*height);
        LOGGER.warning("mode_size=%u",mode_size);
        unsigned int mindif = -1;
        unsigned int nearest = 0;
        for ( int n = 0; modes[n]; ++n )
        {
            unsigned int new_size = modes[n]->w * modes[n]->h;
            unsigned int new_dif = abs(mode_size-new_size);
            LOGGER.warning("new_size=%u , new_dif=%u", new_size, new_dif);

            if ( new_dif < mindif )
            {
                nearest = n;
                mindif = new_dif;
                LOGGER.warning("nearest is %d", nearest);
            }
        }
        LOGGER.warning("final nearest is %d", nearest);
        *width = modes[nearest]->w;
        *height = modes[nearest]->h;
    }
}

void SDLVideo::setVideoMode(int width, int height, Uint32 flags)
{
    // eventually delete old backbuffer
    if (    frontBuffer
         && !(frontBuffer->flags & SDL_DOUBLEBUF)
         && backBuffer
       )
    {
        SDL_FreeSurface(backBuffer);
    }

    int bpp = best_bpp;
    int new_width = width;
    int new_height = height;
    if ( ! (flags&SDL_FULLSCREEN) )
    {
        bpp = 0;
        flags |= SDL_ANYFORMAT;
    }
    else
    {
        getNearestFullScreenMode(flags, &new_width, &new_height);
        LOGGER.warning("Setting fullscreen mode %d x %d (original %d x %d)",
                            new_width, new_height, width, height);
    }

    LOGGER.warning("Flags before setting mode");
    logFlags(flags);

    logAvailableModes(flags);

    frontBuffer = SDL_SetVideoMode(new_width, new_height, bpp, flags);

    if ( !frontBuffer )
    {
        throw Exception("Couldn't set display mode, original %d x %d, modified %d x %d (%X): %s",
                        width, height, new_width, new_height, flags, SDL_GetError());
    }

    if ( !(frontBuffer->flags & SDL_DOUBLEBUF) )
    {
        LOGGER.warning("--->Doing custom buffer<---");
        backBuffer = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                          frontBuffer->w,
                                          frontBuffer->h,
                                          frontBuffer->format->BitsPerPixel,
                                          frontBuffer->format->Rmask,
                                          frontBuffer->format->Gmask,
                                          frontBuffer->format->Bmask,
                                          frontBuffer->format->Amask
                                          );

        if ( !backBuffer )
        {
            throw Exception("Couldn't create backBuffer");
        }
    }
    else
    {
        backBuffer = frontBuffer;
    }

    LOGGER.warning("VidMode info: %d x %d x %d (%d)",
                    frontBuffer->w,
                    frontBuffer->h,
                    frontBuffer->format->BitsPerPixel,
                    frontBuffer->format->BytesPerPixel
                  );

    LOGGER.warning("Flags after setting mode");
    logFlags(frontBuffer->flags);

    if ( Surface::blackScreen )
    {
        SDL_FreeSurface(Surface::blackScreen);
    }

    SDL_Surface *s = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                          frontBuffer->w,
                                          frontBuffer->h,
                                          frontBuffer->format->BitsPerPixel,
                                          frontBuffer->format->Rmask,
                                          frontBuffer->format->Gmask,
                                          frontBuffer->format->Bmask,
                                          frontBuffer->format->Amask
                                          );

    SDL_SetAlpha(s, SDL_SRCALPHA, 128);
    Surface::blackScreen = s;

//    logVideoInfo();

    SDL_ShowCursor(SDL_DISABLE);

    SDL_WM_SetCaption("netPanzer " PACKAGE_VERSION, 0);
}

bool SDLVideo::isDisplayModeAvailable(int width, int height, Uint32 flags)
{
    int bpp = best_bpp;
    if ( ! (flags&SDL_FULLSCREEN) )
    {
        bpp = 0;
        flags |= SDL_ANYFORMAT;
    }

    int res = SDL_VideoModeOK(width, height, bpp, flags);

    LOGGER.warning("Mode %dx%dx%d %s is %s", width, height, bpp,
                   (flags&SDL_FULLSCREEN) ? "full screen" : "windowed",
                   res ? "available" : "NOT available"
                  );
    
    return res;
}

void SDLVideo::lockDoubleBuffer()
{
    if ( SDL_MUSTLOCK(backBuffer) && SDL_LockSurface(backBuffer) )
    {
        throw Exception("Couldn't lock display buffer");
    }
}

void SDLVideo::unlockDoubleBuffer()
{
    if ( SDL_MUSTLOCK(backBuffer) )
    {
        SDL_UnlockSurface(backBuffer);
    }
}

void SDLVideo::copyDoubleBufferandFlip()
{
    if ( !(frontBuffer->flags & SDL_DOUBLEBUF) )
    {
        SDL_BlitSurface(backBuffer, 0, frontBuffer, 0);
    }

    if ( SDL_Flip(frontBuffer) )
    {
        throw Exception("Error while swapping double buffer");
    }
}

SDL_Surface * SDLVideo::getSurface()
{
    return backBuffer;
}
