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

#ifndef __DirectDraw_hpp__
#define __DirectDraw_hpp__

#include "stdafx.hpp"
#include "DirectDrawPalette.hpp"


enum { VIDEO_MODE_WINDOWED          = 0x01,
       VIDEO_MODE_TRIPLE_BUFFER     = 0x02,
       VIDEO_MODE_DOUBLE_BUFFER     = 0x04,
       VIDEO_MODE_AUTO_FBUFFER      = 0x08,
	   VIDEO_MODE_FORCE_MODE_SWITCH = 0x10, };

enum { BLIT_FUNC_MMX,
       BLIT_FUNC_FPU,
       BLIT_FUNC_MEMCPY };

#define MAX_VIDEO_MODES 64

typedef struct
{
	int width;
	int height;
	int bpp;
} DisplayMode, FAR *lpDisplayMode;

typedef struct
{
	long width;
	long height;
	long viewable_width;
	long viewable_height;
	long x_clip_offset;
	long y_clip_offset;
	long offset_to_view;
	long offset_to_screen_start;
} DoubleBufferDesc, FAR *lpDoubleBufferDesc;

 
// DirectDraw class declarations
//---------------------------------------------------------------------------
class DirectDraw
{
public:
	DirectDrawPalette palette;
    
protected:
	int defaultVideoMode;
    
	BOOL             doubleBufferLocked;
	DDSURFACEDESC    ddDbufferPtrSurf; 
	DoubleBufferDesc DBufferDesc;
	DWORD            displayModeCount;
	DisplayMode      displayModeList[MAX_VIDEO_MODES];
	BOOL             GDIEnabled;
	BOOL             DBufferTransferInProgress;
    BOOL             VideoBufferingModeInitialized;
	
    void getViewableRect(RECT *rect);
	BOOL enumDisplayModes();
	BOOL DDrawCreate();
	void cleanUpSurfaces();
    
	BOOL MMXAvailable;
    BOOL FPUAvailable;
    BOOL checkCPUTechnology( void );
    void selectBlittingFunction( void );

    BOOL windowedSurfaceInitialization(DWORD width, DWORD height,DWORD bpp,BYTE modeFlags);
	BOOL fullScreenSurfaceInitialization(DWORD width,DWORD height,DWORD bpp,BYTE modeFlags);
    

    void (*BlitRectsFuncPtr)( unsigned char *pSrc, long srcPitch, unsigned char *pDest, long destPitch, 
                              long bytesPerPixel, long nRects, RECT *pSrcRects, RECT *pDestRects );
    
    HRESULT DDBltRects( LPDIRECTDRAWSURFACE pSrc, LPDIRECTDRAWSURFACE pDest,
                        long nRects, RECT *pSrcRects, RECT *pDestRects       );
     		

public:
	//directdraw surface description--
	DDSURFACEDESC DDPrimaryDesc;
	DDSURFACEDESC DDDoubleBuffDesc;
	DDSCAPS       ddscapsPrim;
	DDSCAPS       ddscapsDBuff;

	LPDIRECTDRAW lpDirectDraw;
    
	LPDIRECTDRAWSURFACE lpFrontBuffer;
	LPDIRECTDRAWSURFACE lpBackBuffer;
	LPDIRECTDRAWSURFACE lpDoubleBuffer;

	LPDIRECTDRAWCLIPPER lpClipper;

	BOOL  fEnabled;                  //DirectDraw enabled
	DWORD cooperationLevelFlags;
	BYTE  currentModeFlags;
	
	DisplayMode currentDisplayMode;

	DirectDraw();
        		
	BOOL initialize();
	void shutdown();
  
  BOOL setVideoMode(int width, int height, int bpp = 8);
  void getVideoMode(DWORD &width, DWORD &height, DWORD &bpp);
  void getDisplayModes(DisplayMode **modes, int &num);

  BOOL setBlittingFunction( int blitFuncEnum );

  BOOL setVideoMode(DWORD width, DWORD height, DWORD bpp, BYTE mode_flags);
  BOOL createFrameBuffer(DWORD width, DWORD height, DWORD bpp);
  BOOL createFrameBuffer(DWORD width, DWORD height, DWORD bpp, DWORD clip_width, DWORD clip_height);
  BOOL lockDoubleBuffer(BYTE **DoubleBuffer);
  BOOL unlockDoubleBuffer();
  BOOL copyDoubleBufferandFlip();
  void setGDIStatus(BOOL enable);
  BOOL getGDIStatus( void ) { return( GDIEnabled ); }
  void restoreAll();
  void loadPaletteACT(char *filename);
  void resetDisplayModeList();
  int  addDisplayMode(int width, int height, int bpp);

  BOOL isDisplayModeAvailable(int width, int height, int bpp);
  BOOL decreaseDisplayModeResolution( int *width, int *height );
  BOOL increaseDisplayModeResolution( int *width, int *height );
}; // end DirectDraw

#endif // end __DirectDraw_hpp__
