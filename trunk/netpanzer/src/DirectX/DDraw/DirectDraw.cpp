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

#include "stdafx.hpp"
#include "DirectDraw.hpp"
#include "DirectDrawError.hpp"
#include "gapp.hpp"

#define _FPU_ALIGN (16)  

#define _MMX_ALIGN (16) 

#define _COPY_TYPE_BYTE_THRESHOLD (46)

#define _SURFACE_LOCK_RETRY_COUNT (3)

#define _CPUID __asm _emit 0x0F __asm _emit 0xA2

void BlitRectsMMX( unsigned char *pSrc, long srcPitch, unsigned char *pDest, long destPitch, 
                   long bytesPerPixel, long nRects, RECT *pSrcRects, RECT *pDestRects );

void BlitRectsFPU( unsigned char *pSrc, long srcPitch, unsigned char *pDest, long destPitch, 
                   long bytesPerPixel, long nRects, RECT *pSrcRects, RECT *pDestRects );
    
void BlitRectsMemcpy( unsigned char *pSrc, long srcPitch, unsigned char *pDest, long destPitch, 
                      long bytesPerPixel, long nRects, RECT *pSrcRects, RECT *pDestRects );


//extern globalApp gapp;

unsigned char *DOUBLE_BUFFER = NULL;

unsigned long DBUFFER_WIDTH;
unsigned long DBUFFER_HEIGHT;
unsigned long DBUFFER_VIEW_WIDTH;
unsigned long DBUFFER_VIEW_HEIGHT;
unsigned long X_CLIP_OFS;
unsigned long Y_CLIP_OFS;
unsigned long OFFSET_TO_VIEW;


// enumDisplayModesCallback
//---------------------------------------------------------------------------
HRESULT WINAPI enumDisplayModesCallback(LPDDSURFACEDESC lpDDSurfaceDesc,  LPVOID lpContext)
{
	DWORD mode_count;
	DirectDraw *ddraw;

	ddraw = (DirectDraw *) lpContext;

	mode_count = ddraw->addDisplayMode(
										lpDDSurfaceDesc->dwWidth,
										lpDDSurfaceDesc->dwHeight,
										lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount);
	if (mode_count == MAX_VIDEO_MODES)
	{
		return( DDENUMRET_CANCEL );
	} else
	{
		return(DDENUMRET_OK);
	}
} // end enumDisplayModesCallback

// DirectDraw constructor
//---------------------------------------------------------------------------
DirectDraw::DirectDraw()
{
	lpDirectDraw   = NULL;
	lpFrontBuffer  = NULL;
	lpBackBuffer   = NULL;
	lpDoubleBuffer = NULL;

	memset((void *) &DDPrimaryDesc,    0, sizeof(DDPrimaryDesc));
	memset((void *) &DDDoubleBuffDesc, 0, sizeof(DDDoubleBuffDesc));
	memset((void *) &ddscapsPrim,      0, sizeof(ddscapsPrim));
	memset((void *) &ddscapsDBuff,     0, sizeof(ddscapsDBuff));

	doubleBufferLocked        = FALSE;
	GDIEnabled                = FALSE;
	DBufferTransferInProgress = FALSE;
    VideoBufferingModeInitialized = FALSE;
} // end DirectDraw constructor

// initialize
//---------------------------------------------------------------------------
BOOL DirectDraw::initialize( )
{
 if ( DDrawCreate() == FALSE) 
  { return( FALSE ); }

 enumDisplayModes();

 checkCPUTechnology();
 selectBlittingFunction();
 return( TRUE );
} // end initialize

// shutdown
//---------------------------------------------------------------------------
void DirectDraw::shutdown()
{
	palette.cleanUp();

	cleanUpSurfaces();

	if (lpDirectDraw != NULL)
	{
		lpDirectDraw->Release();
		lpDirectDraw = NULL;
	}
} // end shutdown

// resetDisplayModeList
//---------------------------------------------------------------------------
void DirectDraw::resetDisplayModeList()
{
	displayModeCount = 0;
} // end resetDisplayModeList

// addDisplayMode
//---------------------------------------------------------------------------
int DirectDraw::addDisplayMode(int width, int height, int bpp)
{
	displayModeList[displayModeCount].width  = width;
	displayModeList[displayModeCount].height = height;     
	displayModeList[displayModeCount].bpp    = bpp;

	displayModeCount++;

	return displayModeCount;
} // addDisplayMode

// isDisplayModeAvailable
//---------------------------------------------------------------------------
BOOL DirectDraw::isDisplayModeAvailable(int width, int height, int bpp)
{
	for (DWORD i = 0; i < displayModeCount; i++)
	{
		if (
			(displayModeList[ i ].width == width)   &&
			(displayModeList[ i ].height == height) &&
			(displayModeList[ i ].bpp == bpp))
		{
			return TRUE;
		}   
	}

	return FALSE;
} // end isDisplayModeAvailable

// sets the video mode using the current mode flags.
BOOL DirectDraw::setVideoMode(int width, int height, int bpp)
{
  if (currentDisplayMode.width != width ||
    currentDisplayMode.height != height ||
    currentDisplayMode.bpp != bpp)
  {
    return setVideoMode(width, height, bpp, currentModeFlags);
  }
  else
  {
    return true;
  }

}

// returns width, height and bits-per-pixel of current mode
void DirectDraw::getVideoMode(DWORD &width, DWORD &height, DWORD &bpp)
{
  width  = currentDisplayMode.width;
  height = currentDisplayMode.height;
  bpp    = currentDisplayMode.bpp;
}

// Gets all the display modes available.
//
// EXAMPLE
// -----------
//
// DisplayMode *list = NULL;
// int num;
//
// ...
// getDisplayModes(&list, num);
// ... 
// cout << list[i]->width;
// cout << list[i]->height;
// cout << list[i]->bpp;
void DirectDraw::getDisplayModes(DisplayMode **modes, int &num)
{
  *modes = displayModeList;
  num = displayModeCount;
}

// decreaseDisplayModeResolution();
//
// Decreases the screen resolution if possible, otherwise
// leaves the display mode as is.
//---------------------------------------------------------------------------
BOOL DirectDraw::decreaseDisplayModeResolution(int *width, int *height )
{
  DisplayMode *newDisplay = NULL;
  int newNumPixels = 0;
  int currNumPixels = currentDisplayMode.width * 
                      currentDisplayMode.height;

	for (DWORD i = 0; i < displayModeCount; i++)
	{
    int numPixels = displayModeList[i].width * 
                    displayModeList[i].height;
		
    if (displayModeList[i].bpp == currentDisplayMode.bpp &&
  	    numPixels < currNumPixels)
    {
      if (newDisplay == NULL ||
          numPixels > newNumPixels)
      {
        newDisplay = &displayModeList[i];
        newNumPixels = newDisplay->width * newDisplay->height;
      }
    }
      
	}
  
  if (newDisplay != NULL)
  {
    setVideoMode(newDisplay->width,
                 newDisplay->height,
                 newDisplay->bpp,
                 currentModeFlags);
    
	*width = newDisplay->width;
	*height = newDisplay->height;
 
    return ( TRUE );
  }

 return ( FALSE );
} // end decreaseDisplayModeResolution

// increaseDisplayModeResolution();
//
// Increases the screen resolution if possible, otherwise
// leaves the display mode as is.
//---------------------------------------------------------------------------
BOOL DirectDraw::increaseDisplayModeResolution( int *width, int *height )
{
  DisplayMode *newDisplay = NULL;
  int newNumPixels = 0;
  int currNumPixels = currentDisplayMode.width * 
                      currentDisplayMode.height;

	for (DWORD i = 0; i < displayModeCount; i++)
	{
    int numPixels = displayModeList[i].width * 
                    displayModeList[i].height;
		
    if (displayModeList[i].bpp == currentDisplayMode.bpp &&
  	    numPixels > currNumPixels)
    {
      if (newDisplay == NULL ||
          numPixels < newNumPixels)
      {
        newDisplay = &displayModeList[i];
        newNumPixels = newDisplay->width * newDisplay->height;
      }
    }
      
	}
  
  if (newDisplay != NULL)
  {
    setVideoMode(newDisplay->width,
                 newDisplay->height,
                 newDisplay->bpp,
                 currentModeFlags);
  	
	*width = newDisplay->width;
	*height = newDisplay->height;
 
    return ( TRUE );
  }

 return ( FALSE );
} // end increaseDisplayModeResolution

// enumDisplayModes
//---------------------------------------------------------------------------
BOOL DirectDraw::enumDisplayModes()
{
	HRESULT hr;

	resetDisplayModeList();

	hr = lpDirectDraw->EnumDisplayModes(0, NULL, (void *) this, enumDisplayModesCallback);

	if(hr != DD_OK) 
	{
		DDErrorBox("enumDisplayModes" , hr);
		return FALSE;
	}

	return FALSE;
} // end enumDisplayModes

// DDrawCreate
//---------------------------------------------------------------------------
BOOL DirectDraw::DDrawCreate()
{
	HRESULT hr;

	hr = DirectDrawCreate(NULL, &lpDirectDraw, NULL);

	if(hr != DD_OK)
	{
		free(lpDirectDraw);
		lpDirectDraw = NULL;
            
		DDErrorBox("DDrawCreate", hr);
		return FALSE;
	}

	return TRUE;
} // end DDrawCreate

// cleanUpSurfaces
//---------------------------------------------------------------------------
void DirectDraw::cleanUpSurfaces()
{
	if (lpDirectDraw != NULL)
	{
		if (lpFrontBuffer != NULL)
		{
			lpFrontBuffer->Release();
			lpFrontBuffer = NULL;
		}

		if (lpDoubleBuffer != NULL)
		{
			lpDoubleBuffer->Release();
			lpDoubleBuffer = NULL;
		}
	}
} // end cleanUpSurfaces

// getViewableRect
//---------------------------------------------------------------------------
void DirectDraw::getViewableRect(RECT *rect)
{
	rect->top    = DBufferDesc.y_clip_offset;
	rect->left   = DBufferDesc.x_clip_offset;
	rect->bottom = DBufferDesc.height - DBufferDesc.y_clip_offset;
	rect->right  = DBufferDesc.width;   
} // end getViewableRect

void DirectDraw::setPalette(RGBColor *color)
{
		palette.loadLibPalette(color, lpFrontBuffer);
}

// setVideoMode
//---------------------------------------------------------------------------
BOOL DirectDraw::setVideoMode(DWORD width, DWORD height, DWORD bpp, BYTE modeFlags)
{
	BOOL result;

	cleanUpSurfaces();

	if(modeFlags & VIDEO_MODE_WINDOWED)
	{
		result = windowedSurfaceInitialization(width, height, bpp, modeFlags);
	}
	else
	{
		result = fullScreenSurfaceInitialization(width, height, bpp, modeFlags);
	}


	return( result ); 
} // end setVideoMode


#if _MSC_VER > 1000
 #pragma optimize( "", off )
#endif

BOOL DirectDraw::checkCPUTechnology( void )
 {

   // MMX detection code courtesy of some bloak on the net
   // nice execption handling routines :)

	volatile BOOL retval = TRUE;
	volatile DWORD RegEDX;

	MMXAvailable = 1;
    FPUAvailable = 1;

    __try {
		_asm {
			mov eax, 1		// set up CPUID to return processor version and features
							//	0 = vendor string, 1 = version info, 2 = cache info
			_CPUID           // code bytes = 0fh,  0a2h
            mov RegEDX, edx	// features returned in edx
	   	}
   	} __except(EXCEPTION_EXECUTE_HANDLER)
	{
		MMXAvailable = 0;
        retval = 0;
	}

	if (retval == 0)
	{
        MMXAvailable = 0;
		return FALSE;        	// processor does not support CPUID
	}

	if (RegEDX & 0x800000) 		// bit 23 is set for MMX technology
	{
	   __try { _asm emms } 		// try executing the MMX instruction "emms"
	   __except(EXCEPTION_EXECUTE_HANDLER) { MMXAvailable = 0; retval = FALSE; }

		return retval;
	}
   	else
		{
         MMXAvailable = 0;
         return FALSE;        	// processor supports CPUID but does not support MMX technology
        }

	// if retval == 0 here, it means the processor has MMX technology but
	// floating-point emulation is on; so MMX technology is unavailable
	MMXAvailable = 0;
    FPUAvailable = 0;

	return retval;

 }

#if _MSC_VER > 1000
 #pragma optimize( "", on )
#endif 

void DirectDraw::selectBlittingFunction( void )
 {
  if ( MMXAvailable == TRUE )
   {
    BlitRectsFuncPtr = BlitRectsMMX; 
   }
  else
   if ( FPUAvailable == TRUE )
    {
     BlitRectsFuncPtr = BlitRectsFPU; 
    }
   else
    {
     BlitRectsFuncPtr = BlitRectsMemcpy; 
    }  
 }

BOOL DirectDraw::setBlittingFunction( int blitFuncEnum )
 {
  switch ( blitFuncEnum )
   {
    case BLIT_FUNC_MMX :
     {
      if ( MMXAvailable == TRUE )
       {
        BlitRectsFuncPtr = BlitRectsMMX; 
        return( TRUE );
       }
      return( FALSE );
     } break;

    case BLIT_FUNC_FPU :
     {
      if ( FPUAvailable == TRUE )
       {
        BlitRectsFuncPtr = BlitRectsFPU; 
        return( TRUE );
       }
      return( FALSE );
     } break;

    case BLIT_FUNC_MEMCPY :
     {
      BlitRectsFuncPtr = BlitRectsMemcpy; 
      return( TRUE );
     } break;

   } // switch
  return( FALSE );

 }

// windowedSurfaceInitialization
//---------------------------------------------------------------------------
BOOL DirectDraw::windowedSurfaceInitialization(DWORD width, DWORD height, DWORD bpp, BYTE modeFlags)
{
	BOOL maximize_window_area = FALSE;
    DDSURFACEDESC currentVideoMode;
	int           bitsPerPlane, planes;
	HRESULT       hr;

	hr = lpDirectDraw->SetCooperativeLevel(gapp.hwndApp, DDSCL_NORMAL);
    
	if(hr != DD_OK) 
	{
		DDErrorBox("DDSetCooperativeLevel" , hr);
		return FALSE;
	}

	cooperationLevelFlags = DDSCL_NORMAL;

	// Get the current video mode to see if color bit depth 
	// is the same current color bit depth
	currentVideoMode.dwSize = sizeof(currentVideoMode);

	hr = lpDirectDraw->GetDisplayMode(&currentVideoMode);

	if(hr != DD_OK) 
	{
		DDErrorBox("GetDisplayMode:" , hr);
		return FALSE;
	}

	if (currentVideoMode.ddpfPixelFormat.dwRGBBitCount != bpp)
	{
		MessageBox(
					gapp.hwndApp,
					"The current color depth does not match specified depth\n",
					"Warning",
					MB_OK);   

		return FALSE;
	}

	if ( currentVideoMode.dwWidth == width && currentVideoMode.dwHeight == height )
     { maximize_window_area = TRUE; }

    
    currentDisplayMode.width  = currentVideoMode.dwWidth;
	currentDisplayMode.height = currentVideoMode.dwHeight;
	currentDisplayMode.bpp    = currentVideoMode.ddpfPixelFormat.dwRGBBitCount;


	if ((modeFlags & VIDEO_MODE_FORCE_MODE_SWITCH) != 0)
	{
		enumDisplayModes();

		if (isDisplayModeAvailable(width, height, bpp) == FALSE)
		{
			MessageBox(gapp.hwndApp, "Video Mode Not Supported", "Error", MB_OK);
			return(FALSE );
		}

		hr = lpDirectDraw->SetDisplayMode(width, height, bpp);

		if(hr != DD_OK)
		{
			DDErrorBox("DDSetVideoMode" , hr);
			return FALSE;
		}
	}

	if ( maximize_window_area == TRUE )
     {  
      SetWindowLong( gapp.hwndApp, GWL_STYLE, WS_POPUP | WS_VISIBLE);
     }
    else
     { 
      SetWindowLong( gapp.hwndApp, GWL_STYLE, 
                     WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_VISIBLE | WS_MINIMIZEBOX );
     }

    RECT rcWork;
	RECT rc;
	HDC  hdc;

	hdc = GetDC(NULL);
	
    if ( hdc == NULL )
     { MessageBox(  gapp.hwndApp, "GetDC Failed in Windowed Initialization", "DirectDraw", MB_OK ); }  
    
    planes = GetDeviceCaps(hdc, PLANES);
    bitsPerPlane = GetDeviceCaps(hdc, BITSPIXEL);	
    bitsPerPlane = planes * bitsPerPlane;

	ReleaseDC(NULL, hdc);

	SetRect(&rc, 0, 0, width, height);

	AdjustWindowRectEx(
						&rc,
						GetWindowStyle(gapp.hwndApp),
						GetMenu(gapp.hwndApp) != NULL,
						GetWindowExStyle(gapp.hwndApp));

	SetWindowPos(gapp.hwndApp, NULL, 0, 0, rc.right-rc.left, rc.bottom-rc.top, SWP_NOZORDER | SWP_NOACTIVATE);

	SetWindowPos(gapp.hwndApp, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);

	SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWork, 0);
	GetWindowRect(gapp.hwndApp, &rc);

	if (rc.left < rcWork.left)
	{
		rc.left = rcWork.left;
	}
	if (rc.top  < rcWork.top)
	{
		rc.top  = rcWork.top;
	}
	
	SetWindowPos(gapp.hwndApp, NULL, rc.left, rc.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

	DDPrimaryDesc.dwSize         = sizeof(DDPrimaryDesc);
	DDPrimaryDesc.dwFlags        = DDSD_CAPS;
	DDPrimaryDesc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

	hr = lpDirectDraw->CreateSurface(&DDPrimaryDesc, &lpFrontBuffer, NULL);
    
	if(hr != DD_OK) 
	{
		DDErrorBox("CreatePrimarySurface", hr);
		return FALSE;
	}

	hr = lpDirectDraw->CreateClipper(0, &lpClipper, NULL);

	if(hr != DD_OK)
	{ 
		DDErrorBox("CreateClipper", hr);
		return FALSE; 
	}

	hr = lpClipper->SetHWnd(0, gapp.hwndApp);

	if(hr != DD_OK)
	{ 
		DDErrorBox("Clipper: SetHWnd", hr);
		return FALSE; 
	}

	hr = lpFrontBuffer->SetClipper( lpClipper );

	if (hr != DD_OK)
	{ 
		DDErrorBox("SetClipper", hr);
		return FALSE; 
	}

	palette.initialize(lpDirectDraw, _palette_mode_window );

	lpFrontBuffer->SetPalette(palette.lpDDPalette);

	currentModeFlags = modeFlags;

	VideoBufferingModeInitialized = TRUE;
    return TRUE;
} // end windowedSurfaceInitialization
        
// fullScreenSurfaceInitialization
//---------------------------------------------------------------------------
BOOL DirectDraw::fullScreenSurfaceInitialization(DWORD width, DWORD height, DWORD bpp, BYTE modeFlags)
{
	HRESULT hr;

	SetWindowLong( gapp.hwndApp, GWL_STYLE, WS_POPUP | WS_VISIBLE );
    
    hr = lpDirectDraw->SetCooperativeLevel(gapp.hwndApp, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN | DDSCL_ALLOWREBOOT);

	if(hr != DD_OK) 
	{
		DDErrorBox("DDSetCooperativeLevel", hr);
		return FALSE;
	}

	cooperationLevelFlags = DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN | DDSCL_ALLOWREBOOT;

	enumDisplayModes();

	if (isDisplayModeAvailable(width, height, bpp) == FALSE)
	{
		MessageBox(gapp.hwndApp, "Video Mode Not Supported", "Error", MB_OK);
		return( FALSE );
	}

	hr = lpDirectDraw->SetDisplayMode(width, height, bpp);

	if(hr != DD_OK)
	{
		DDErrorBox( "DDSetVideoMode", hr);
		return FALSE;
	}

	currentDisplayMode.width  = width;
	currentDisplayMode.height = height;
	currentDisplayMode.bpp    = bpp;

	DDPrimaryDesc.dwSize         = sizeof(DDPrimaryDesc);
	DDPrimaryDesc.dwFlags        = DDSD_CAPS;
	DDPrimaryDesc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE ; 

	if ((modeFlags & VIDEO_MODE_TRIPLE_BUFFER))
	{
		DDPrimaryDesc.dwFlags           = DDPrimaryDesc.dwFlags | DDSD_BACKBUFFERCOUNT;
		DDPrimaryDesc.dwBackBufferCount = 1;
		DDPrimaryDesc.ddsCaps.dwCaps    = DDPrimaryDesc.ddsCaps.dwCaps | DDSCAPS_COMPLEX | DDSCAPS_FLIP;
	}

	hr = lpDirectDraw->CreateSurface(&DDPrimaryDesc, &lpFrontBuffer, NULL);

	if (hr != DD_OK) 
	{
		DDErrorBox("CreatePrimarySurface", hr);
		return(FALSE);
	}

	if ((modeFlags & VIDEO_MODE_TRIPLE_BUFFER))
	{
		ddscapsPrim.dwCaps = DDSCAPS_BACKBUFFER;

		hr = lpFrontBuffer->GetAttachedSurface(&ddscapsPrim, &lpBackBuffer);

		if ( hr != DD_OK )
		 {
		  if( ( hr == DDERR_INVALIDOBJECT) || (hr == DDERR_INVALIDPARAMS) )
		   {
		  	DDErrorBox("CreateBackBuffer", hr);
	  		return FALSE;
	       }   
	      else
		   {
	        // ** if VIDEO_MODE_TRIPLE_BUFFER fails goto VIDEO_MODE_DOUBLE_BUFFER
		    DDPrimaryDesc.dwSize         = sizeof(DDPrimaryDesc);
            DDPrimaryDesc.dwFlags        = DDSD_CAPS;
	        DDPrimaryDesc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE; 

		    lpFrontBuffer->Release();
		    lpFrontBuffer = NULL;

            hr = lpDirectDraw->CreateSurface(&DDPrimaryDesc, &lpFrontBuffer, NULL);

	        if (hr != DD_OK) 
	         {
		      DDErrorBox("CreatePrimarySurface", hr);
		      return(FALSE);
	         }
	       
		     modeFlags = modeFlags & (~VIDEO_MODE_TRIPLE_BUFFER);
			 modeFlags = modeFlags | VIDEO_MODE_DOUBLE_BUFFER;
		   } // ** ELSE 
		  } // ** if ( hr != DD_0K ) 
	
	}

	palette.initialize(lpDirectDraw, _palette_mode_fullscreen );

	lpFrontBuffer->SetPalette(palette.lpDDPalette);

	currentModeFlags = modeFlags;

	VideoBufferingModeInitialized = TRUE;

	return TRUE;
} // end fullScreenSurfaceInitialization

// createFrameBuffer
//---------------------------------------------------------------------------
BOOL DirectDraw::createFrameBuffer(DWORD width, DWORD height, DWORD bpp)
{

	memset( (void *) &DDDoubleBuffDesc, 0, sizeof(DDDoubleBuffDesc) );
	DDDoubleBuffDesc.dwSize = sizeof(DDDoubleBuffDesc);     
	DDDoubleBuffDesc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;


	DDDoubleBuffDesc.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
	DDDoubleBuffDesc.ddpfPixelFormat.dwFlags = DDPF_RGB; 

	if( bpp <= 8 )
	{
		DDDoubleBuffDesc.ddpfPixelFormat.dwFlags = DDDoubleBuffDesc.ddpfPixelFormat.dwFlags | DDPF_PALETTEINDEXED8;
	}

	DDDoubleBuffDesc.ddpfPixelFormat.dwRGBBitCount = bpp;

	DDDoubleBuffDesc.dwWidth = width;
	DDDoubleBuffDesc.dwHeight = height;
	DDDoubleBuffDesc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;

	HRESULT hr;

	hr = lpDirectDraw->CreateSurface(&DDDoubleBuffDesc, &lpDoubleBuffer, NULL);

	if(hr != DD_OK)
	{
		DDErrorBox( "CreateDoubleBufferSurface", hr);
		return FALSE;
	}

	DBufferDesc.width                  = DBUFFER_WIDTH       = width;
	DBufferDesc.height                 = DBUFFER_HEIGHT      = height;
	DBufferDesc.viewable_width         = DBUFFER_VIEW_WIDTH  = width;
	DBufferDesc.viewable_height        = DBUFFER_VIEW_HEIGHT = height;
	DBufferDesc.x_clip_offset          = X_CLIP_OFS     = 0;
	DBufferDesc.y_clip_offset          = Y_CLIP_OFS     = 0;
	DBufferDesc.offset_to_view         = OFFSET_TO_VIEW = 0; 
	DBufferDesc.offset_to_screen_start = 0;

	return TRUE;
} // end createFrameBuffer

// createFrameBuffer
//---------------------------------------------------------------------------
BOOL DirectDraw::createFrameBuffer(DWORD width, DWORD height, DWORD bpp, DWORD clipWidth, DWORD clipHeight)
{
   DWORD bufferWidth;
   DWORD bufferHeight;
   
   bufferWidth  = width + clipWidth;
   bufferHeight = height + clipHeight + clipHeight;

   createFrameBuffer(bufferWidth, bufferHeight, bpp);
   
   DBufferDesc.width                  = DBUFFER_WIDTH       = bufferWidth;
   DBufferDesc.height                 = DBUFFER_HEIGHT      = bufferHeight;
   DBufferDesc.viewable_width         = DBUFFER_VIEW_WIDTH  = width;
   DBufferDesc.viewable_height        = DBUFFER_VIEW_HEIGHT = height;
   DBufferDesc.x_clip_offset          = X_CLIP_OFS          = clipWidth;
   DBufferDesc.y_clip_offset          = Y_CLIP_OFS          = clipHeight;
   
   DBufferDesc.offset_to_view         = OFFSET_TO_VIEW = bufferWidth * clipHeight + clipWidth; 
   DBufferDesc.offset_to_screen_start = bufferWidth * clipHeight + clipWidth;

   return TRUE;
} // end createFrameBuffer

// lockDoubleBuffer
//---------------------------------------------------------------------------
BOOL DirectDraw::lockDoubleBuffer(unsigned char **DoubleBuffer)
{
	HRESULT result;
    
	if (doubleBufferLocked == TRUE)
	{
		return FALSE;
	}
            
	memset(&ddDbufferPtrSurf, 0, sizeof(ddDbufferPtrSurf));
	ddDbufferPtrSurf.dwSize = sizeof(ddDbufferPtrSurf);

	result = lpDoubleBuffer->Lock(NULL, &ddDbufferPtrSurf, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);

	if (result != DD_OK)
	{
		DDErrorBox("LockDoubleBuffer", result);
		return FALSE;
	}  

	*DoubleBuffer = (unsigned char *) ddDbufferPtrSurf.lpSurface;

	doubleBufferLocked = TRUE;

	return TRUE;
} // end lockDoubleBuffer

// unlockDoubleBuffer
//---------------------------------------------------------------------------
BOOL DirectDraw::unlockDoubleBuffer()
{
	HRESULT result;

	if (!doubleBufferLocked)
	{
		return FALSE;
	}

	result = lpDoubleBuffer->Unlock(ddDbufferPtrSurf.lpSurface);

	if (result != DD_OK)
	{
		DDErrorBox("UnlockDoubleBuffer", result);
		return  FALSE;
	}

	doubleBufferLocked = FALSE;

	return TRUE; 
} // end unlockDoubleBuffer

// copyDoubleBufferandFlip
//---------------------------------------------------------------------------
BOOL DirectDraw::copyDoubleBufferandFlip()
{
	HRESULT result;

	DBufferTransferInProgress = TRUE;

	if (GDIEnabled)
	{
		DBufferTransferInProgress = FALSE;
		return TRUE;
	}

	RECT rect;
	getViewableRect(&rect);

	if (!(currentModeFlags & VIDEO_MODE_WINDOWED))
	{
		// We are in full screen mode.
		if (currentModeFlags & VIDEO_MODE_TRIPLE_BUFFER)
		{
			ddscapsPrim.dwCaps = DDSCAPS_BACKBUFFER;
			result = lpFrontBuffer->GetAttachedSurface(&ddscapsPrim, &lpBackBuffer);
            

            if(result == DDERR_SURFACELOST)
			result = lpFrontBuffer->Restore();

			if(result != DD_OK) 
			{
				DDErrorBox("CopyDoubleBufferandFlip : Get Attached", result);  
				return FALSE;
			}  

		    if( (DD_OK != lpBackBuffer->GetBltStatus( DDGBS_CANBLT ) ) || 
                (DD_OK != lpDoubleBuffer->GetBltStatus( DDGBS_CANBLT) )  
              )
             { 
  			   result = lpFrontBuffer->Flip( NULL, 0 );

			   if(result == DDERR_SURFACELOST)
			    {
				 result = lpFrontBuffer->Restore();
			    }

			   if( (result != DD_OK) && (result != DDERR_WASSTILLDRAWING) )
			    {
				 DDErrorBox("CopyDoubleBufferandFlip : Flip", result);
				 return FALSE;
			    }  
              
               return( TRUE ); 
              }
            
            //result = lpBackBuffer->BltFast( 0,0, lpDoubleBuffer, &rect, DDBLTFAST_NOCOLORKEY );

            result = DDBltRects( lpDoubleBuffer, lpBackBuffer, 1, &rect, &rect );

			if(result == DDERR_SURFACELOST)
			{
				result = lpBackBuffer->Restore();
			}

			if(result != DD_OK) 
			{
				DDErrorBox("CopyDoubleBufferandFlip : Blit", result);
				return FALSE;
			}  

			//result = lpFrontBuffer->Flip( NULL, DDFLIP_WAIT);
			result = lpFrontBuffer->Flip( NULL, 0 );

			if(result == DDERR_SURFACELOST)
			{
				result = lpFrontBuffer->Restore();
			}

			if( (result != DD_OK) && (result != DDERR_WASSTILLDRAWING) )
			{
				DDErrorBox("CopyDoubleBufferandFlip : Flip", result);
				return FALSE;
			}  

		} else
		{			
            if( (DD_OK != lpFrontBuffer->GetBltStatus( DDGBS_CANBLT ) ) || 
                (DD_OK != lpDoubleBuffer->GetBltStatus( DDGBS_CANBLT ) )  
              )
             { return( TRUE ); }

			//result = lpFrontBuffer->BltFast( 0,0, lpDoubleBuffer, &rect, DDBLTFAST_NOCOLORKEY );
            
            result = DDBltRects( lpDoubleBuffer, lpFrontBuffer, 1, &rect, &rect );

			if(result == DDERR_SURFACELOST)
			{
				result = lpFrontBuffer->Restore();
			}
		
			if( result != DD_OK)
			{
				DDErrorBox("DoubeBufferedCopy : Blt", result);
				return FALSE;
			}  

		}     
	} else
	{
		getViewableRect(&rect);
        
        if( (DD_OK != lpFrontBuffer->GetBltStatus( DDGBS_CANBLT ) ) || 
             (DD_OK != lpDoubleBuffer->GetBltStatus( DDGBS_CANBLT ) )   
           )
         { return( TRUE ); }

		//result = lpFrontBuffer->Blt(&gapp.wndRectangle, lpDoubleBuffer, &rect, DDBLT_WAIT, NULL);

        result = DDBltRects( lpDoubleBuffer, lpFrontBuffer, 1, &rect, &gapp.wndRectangle );

		if(result == DDERR_SURFACELOST)
		result = lpFrontBuffer->Restore();

		if(result != DD_OK)
		{
			DDErrorBox("WindowedCopy : Blt", result);  
			return ( FALSE );
		}  
	}  

	DBufferTransferInProgress = FALSE;

	return TRUE;
} // end copyDoubleBufferandFlip

// restoreAll
//---------------------------------------------------------------------------
void DirectDraw::restoreAll()
{
	HRESULT result;

	if ( (lpDirectDraw == NULL) || 
         (GDIEnabled == FALSE)  ||
         (VideoBufferingModeInitialized == FALSE) )
	{
		return;
	}

	if (!(currentModeFlags & VIDEO_MODE_WINDOWED))
	{ 
		result = lpDirectDraw->SetCooperativeLevel(gapp.hwndApp, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN | DDSCL_ALLOWREBOOT);

		result = lpDirectDraw->SetDisplayMode(currentDisplayMode.width,
							   currentDisplayMode.height,
							   currentDisplayMode.bpp);

		if(result != DD_OK)
		{
			DDErrorBox("restoreAll: SetVideoMode", result);
			return;
		}
	}

	result = lpFrontBuffer->Restore();

	if(result != DD_OK) 
	{
		DDErrorBox("restoreAll: FrontBuffer", result);
		return;
	}    

	result = lpDoubleBuffer->Restore();

	if(result != DD_OK) 
	{
		DDErrorBox("restoreAll: DoubleBuffer", result);
		return;
	}    


	if (currentModeFlags & VIDEO_MODE_TRIPLE_BUFFER)
	{
		result = lpBackBuffer->Restore();

		if(result != DD_OK) 
		{
			DDErrorBox("restoreAll: BackBuffer", result);
			return;     
		}     
	}

	lpFrontBuffer->SetPalette( palette.lpDDPalette );
} // end restoreAll

// setGDIStatus
//---------------------------------------------------------------------------
void DirectDraw::setGDIStatus(BOOL enable)
{
	if ( (lpDirectDraw == NULL) || (VideoBufferingModeInitialized == FALSE) )
	 {
	  return;
	 }

	if (enable)
	{
		if (GDIEnabled)
		{
			return;
		}

		GDIEnabled = TRUE;
		while(DBufferTransferInProgress);
		
		lpDirectDraw->FlipToGDISurface();
	    while( ShowCursor( TRUE ) <= 0 );
    }

	if (!enable)
	{
      while( ShowCursor( FALSE ) > 0 );

	  if (!GDIEnabled)
	   {
	    return;
	   }

	  GDIEnabled = FALSE;
    }

} // end setGDIStatus

// loadPaletteACT
//---------------------------------------------------------------------------
void DirectDraw::loadPaletteACT(char *filename)
{
	palette.loadACTPalette(filename, lpFrontBuffer);
} // end loadPaletteACT

HRESULT DirectDraw::DDBltRects( LPDIRECTDRAWSURFACE pSrc, LPDIRECTDRAWSURFACE pDest,
                                long nRects, RECT *pSrcRects, RECT *pDestRects       )
 {
	HRESULT ddrval=0;
	DDSURFACEDESC ddsd;
	unsigned char *src, *dest;
	long srcPitch, destPitch, bytesPerPixel, count;

	// lock the dest surface
	memset( &ddsd, 0, sizeof(ddsd) );
	ddsd.dwSize=sizeof(ddsd);
	count = _SURFACE_LOCK_RETRY_COUNT;
	do 
     {
		ddrval=pDest->Lock( 0, &ddsd, 0, 0 );
		if( ddrval==DDERR_SURFACELOST ) 
         {
			ddrval = pDest->Restore();
			if( SUCCEEDED(ddrval) )
				ddrval=pDest->Lock( 0, &ddsd, 0, 0 );
		 }  
		count--;
	 } while( !SUCCEEDED(ddrval) && count );

	if( ddrval==DD_OK )
	 {
	  // grab dest surface descriptors
	  dest=(unsigned char *)ddsd.lpSurface;
	  destPitch=ddsd.lPitch;

	  // assuming src and dest have same format and that
	  // dwRGBBitCount is a multiple of 8
	  bytesPerPixel=ddsd.ddpfPixelFormat.dwRGBBitCount/8;

	  // lock the source surface
	  memset( &ddsd, 0, sizeof(ddsd) );
	  ddsd.dwSize=sizeof(ddsd);
      count = _SURFACE_LOCK_RETRY_COUNT;
	  do 
       {
		ddrval=pSrc->Lock( 0, &ddsd, 0, 0 );
		if( ddrval==DDERR_SURFACELOST ) 
         {
		  ddrval = pSrc->Restore();
		   if( SUCCEEDED(ddrval) )
		    ddrval=pSrc->Lock( 0, &ddsd, 0, 0 );
		 }
		count--;
	   } while( !SUCCEEDED(ddrval) && count );

	  if( ddrval==DD_OK )
	   {
		// grab source surface descriptors
		src=(unsigned char *)ddsd.lpSurface;
		srcPitch=ddsd.lPitch;

		
        BlitRectsFuncPtr( src, srcPitch, dest, destPitch, bytesPerPixel, nRects, pSrcRects, pDestRects );
        
        ddrval=pSrc->Unlock( 0 );
	   }

	  ddrval=pDest->Unlock( 0 );
	}

	return ddrval;
 }

#if _MSC_VER > 1000
 #pragma optimize( "", off )
#endif

void BlitRectsMMX( unsigned char *pSrc, long srcPitch, unsigned char *pDest, long destPitch, 
                   long bytesPerPixel, long nRects, RECT *pSrcRects, RECT *pDestRects )
{

	long y, rows, prebytes, qwords, postbytes;
	unsigned char *src, *dest;

	while( nRects-- )
	{
		rows= pSrcRects->bottom-pSrcRects->top;
		src = pSrc  + pSrcRects->top *srcPitch  + pSrcRects->left *bytesPerPixel;
		dest= pDest + pDestRects->top*destPitch + pDestRects->left*bytesPerPixel;

		postbytes=bytesPerPixel * ( pSrcRects->right-pSrcRects->left );


		if( postbytes>=_COPY_TYPE_BYTE_THRESHOLD)
		{
			prebytes=(_MMX_ALIGN-(((long)dest)%_MMX_ALIGN))%_MMX_ALIGN;
			if( prebytes>postbytes ) prebytes=postbytes;
			postbytes=postbytes-prebytes;
			qwords=postbytes/8;
			postbytes=postbytes-qwords*8;

			for( y=rows;y;y-- )
			{
				__asm
				{
					MOV		ESI,[src]
					MOV		EDI,[dest]

					MOV		ECX,[prebytes]
					JCXZ	QWORDSTART
				REP	MOVS	[EDI],[ESI]

QWORDSTART:

					MOV		ECX,[qwords]
					JCXZ	POSTBYTES

ALIGN 16
QWORDLOOP:
					MOVQ    MM0,[ESI]
					MOVQ    [EDI],MM0
					ADD     ESI, 8
					ADD     EDI, 8
					LOOP	QWORDLOOP

POSTBYTES:
					MOV		ECX,[postbytes]
					JCXZ	DONE
				REP	MOVS	[EDI],[ESI]

DONE:
				}

				src+=srcPitch;
				dest+=destPitch;
			}
		}
		else
		{
			for( y=rows;y;y-- )
			{
				memcpy( dest, src, postbytes );

				src+=srcPitch;
				dest+=destPitch;
			}
		}


		pSrcRects++;
		pDestRects++;
	}

	__asm
	{
		EMMS
	}

}
#if _MSC_VER > 1000
 #pragma optimize( "", on )
#endif


void BlitRectsMemcpy( unsigned char *pSrc, long srcPitch, unsigned char *pDest, long destPitch, 
                      long bytesPerPixel, long nRects, RECT *pSrcRects, RECT *pDestRects )
{
	long y, rows, bytes;
	unsigned char *src, *dest;

	while( nRects-- )
	{
		rows= pSrcRects->bottom-pSrcRects->top;
		src = pSrc  + pSrcRects->top *srcPitch  + pSrcRects->left *bytesPerPixel;
		dest= pDest + pDestRects->top*destPitch + pDestRects->left*bytesPerPixel;

		bytes=bytesPerPixel * ( pSrcRects->right-pSrcRects->left );

		for( y=rows;y;y-- )
		{
			memcpy( dest, src, bytes );

			src+=srcPitch;
			dest+=destPitch;
		}

		pSrcRects++;
		pDestRects++;
	}
}

#if _MSC_VER > 1000
 #pragma optimize( "", off )
#endif

void BlitRectsFPU( unsigned char *pSrc, long srcPitch, unsigned char *pDest, long destPitch, 
                   long bytesPerPixel, long nRects, RECT *pSrcRects, RECT *pDestRects )
{

	long y, rows, prebytes, qdwords, postbytes;
	unsigned char *src, *dest;

	while( nRects-- )
	{
		rows= pSrcRects->bottom-pSrcRects->top;
		src = pSrc  + pSrcRects->top *srcPitch  + pSrcRects->left *bytesPerPixel;
		dest= pDest + pDestRects->top*destPitch + pDestRects->left*bytesPerPixel;

		postbytes=bytesPerPixel * ( pSrcRects->right-pSrcRects->left );

		if( postbytes>=_COPY_TYPE_BYTE_THRESHOLD )
		{
			prebytes = (_FPU_ALIGN-(((long)dest)%_FPU_ALIGN))%_FPU_ALIGN;
			if( prebytes>postbytes ) prebytes=postbytes;
			postbytes = postbytes-prebytes;
			qdwords = postbytes/16;
			postbytes = postbytes-qdwords*16;

			for( y=rows;y;y-- )
			{
				__asm
				{
					MOV		ESI,[src]
					MOV		EDI,[dest]

					MOV		ECX,[prebytes]
					JCXZ	QDWORDSTART
				REP	MOVS	[EDI],[ESI]

QDWORDSTART:

					MOV		ECX,[qdwords]
					JCXZ	POSTBYTES

ALIGN 16
QDWORDLOOP:
					FILD    QWORD PTR [ESI]        
					FILD    QWORD PTR [ESI+8]        
					FXCH
					FISTP   QWORD PTR [EDI]
					FISTP   QWORD PTR [EDI+8]

					ADD     ESI, 16
					ADD     EDI, 16
					LOOP	QDWORDLOOP

POSTBYTES:
					MOV		ECX,[postbytes]
					JCXZ	DONE
				REP	MOVS	[EDI],[ESI]

DONE:
				}

				src+=srcPitch;
				dest+=destPitch;
			}
		}
		else
		{
			for( y=rows;y;y-- )
			{
				memcpy( dest, src, postbytes );

				src+=srcPitch;
				dest+=destPitch;
			}
		}

		pSrcRects++;
		pDestRects++;
	}

}
#if _MSC_VER > 1000
  #pragma optimize( "", on )
#endif
