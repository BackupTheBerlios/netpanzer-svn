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
#include "DirectDrawPalette.hpp"
#include "DirectDrawError.hpp"
#include "gapp.hpp"


//---------------------------------------------------------------------------
DirectDrawPalette::DirectDrawPalette()
{ 
	DirectDrawPalette::logPaletteHandle    = 0;
	DirectDrawPalette::systemPaletteHandle = 0;
	DirectDrawPalette::lpDDPalette         = 0;
	DirectDrawPalette::lpDDLink            = 0;
    DirectDrawPalette::paletteMode         = _palette_mode_fullscreen;
}

//---------------------------------------------------------------------------
void DirectDrawPalette::build332Palette(PALETTEENTRY *colorTable)
{
	long i;

	for (i=0; i<256; i++)
	{
		colorTable[i].peRed   = (BYTE)(((i >> 5) & 0x07) * 255 / 7);
		colorTable[i].peGreen = (BYTE)(((i >> 2) & 0x07) * 255 / 7);
		colorTable[i].peBlue  = (BYTE)(((i >> 0) & 0x03) * 255 / 3);
		colorTable[i].peFlags = (BYTE) PC_NOCOLLAPSE;
	}
}

// load_text_palette
//---------------------------------------------------------------------------
void DirectDrawPalette::loadTextPalette(const char *filename)
{
	BYTE red;
	BYTE green;
	BYTE blue;

	FILE *fp = fopen(filename, "rt");

	assert(fp != 0);

	for(int i = 0; i < 256; i++)
	{
		fscanf(fp,"%u %u %u",&red, &green, &blue);
 
		paletteTable[i].peRed   = red   << 2;
		paletteTable[i].peGreen = green << 2;
		paletteTable[i].peBlue  = blue  << 2;
		paletteTable[i].peFlags = PC_NOCOLLAPSE;
	}

	fclose(fp);
} // end load_text_palette

// initialize
//---------------------------------------------------------------------------
void DirectDrawPalette::initialize(LPDIRECTDRAW lpDirectDraw, int paletteMode )
{
	HRESULT result;

	if(lpDDPalette != 0)
	 {
 	  lpDDPalette->Release();
	 }

    if ( systemPaletteHandle != 0 )
     {
      DeleteObject( systemPaletteHandle );
	  systemPaletteHandle = 0;
     }
 
    if ( logPaletteHandle != 0 )
     {
      DeleteObject( logPaletteHandle );
 	  logPaletteHandle = 0;
     }

	DirectDrawPalette::logPaletteHandle    = 0;
	DirectDrawPalette::systemPaletteHandle = 0;
	DirectDrawPalette::lpDDPalette         = 0;
	DirectDrawPalette::lpDDLink            = lpDirectDraw;
    DirectDrawPalette::paletteMode         = paletteMode;

	saveSystemPalette();

    build332Palette(paletteTable);

	result = lpDDLink->CreatePalette(DDPCAPS_8BIT, paletteTable, &lpDDPalette, 0);

	if(result != DD_OK)
	{
		DDErrorBox("CreatePalette", result);
	}
} // end initialize

// cleanUp
//---------------------------------------------------------------------------
void DirectDrawPalette::cleanUp()
{
   activateSystemPalette();

	if(lpDDPalette != 0)
	{
		lpDDPalette->Release();
	}

   if ( systemPaletteHandle != 0 )
    {
     DeleteObject( systemPaletteHandle );
	 systemPaletteHandle = 0;
    }
 
   if ( logPaletteHandle != 0 )
    {
     DeleteObject( logPaletteHandle );
 	 logPaletteHandle = 0;
   }
    
} // end cleanUp



void DirectDrawPalette::saveSystemPalette( void )
 {
  unsigned int i = 0;

  if ( paletteMode == _palette_mode_fullscreen ) 
   { return; }
  
  for( i=0; i<25; i++)
   {
    systemColors[ i ] = GetSysColor( i );
   }
  
  HDC hdc;
  UINT error;

  hdc = GetDC( 0 );
  error = GetSystemPaletteEntries( hdc, 0, 256, systemPalette.palPalEntry );
    
  if ( error == 0 )
   {
    MessageBox(gapp.hwndApp, "GetSystemPaletteEntries: Failed", "Error", MB_OK);     
   }

  systemPalette.palVersion = 0x300;
  systemPalette.palNumEntries = 256;
  systemPaletteHandle = CreatePalette( (CONST LOGPALETTE *) &systemPalette );
 
  if ( systemPaletteHandle == 0 )
   {
    MessageBox(gapp.hwndApp, "Create System Palette Failed", "Error", MB_OK);     
   }
 
  ReleaseDC( gapp.hwndApp , hdc);
 }


// createLogicalPalette
//---------------------------------------------------------------------------
void DirectDrawPalette::createLogicalPalette()
{
    int i;
    
    for(i = 0; i < 256; i++)
    {     
		logicalPalette.palPalEntry[i].peRed   = paletteTable[i].peRed;
		logicalPalette.palPalEntry[i].peGreen = paletteTable[i].peGreen;
		logicalPalette.palPalEntry[i].peBlue  = paletteTable[i].peBlue;
		logicalPalette.palPalEntry[i].peFlags = 0; 
    }

	
    for(i = 1; i < 256; i++)
    {     
   	logicalPalette.palPalEntry[i].peFlags = PC_NOCOLLAPSE; 
    }
    
	logicalPalette.palVersion    = 0x300;
	logicalPalette.palNumEntries = 256;

	if (logPaletteHandle != 0)
	{
		DeleteObject(logPaletteHandle);
		logPaletteHandle = 0;
	}

	logPaletteHandle = CreatePalette((CONST LOGPALETTE *) &logicalPalette);

  if ( paletteMode != _palette_mode_fullscreen ) 
   {
    UINT nearestIndex;
    INT indexList[25];

    for( i = 0; i < 25; i++ )
     {
      nearestIndex = GetNearestPaletteIndex( logPaletteHandle, systemColors[ i ] );
      logicalColors[ i ] = RGB( logicalPalette.palPalEntry[nearestIndex].peRed,
                                logicalPalette.palPalEntry[nearestIndex].peGreen,
                                logicalPalette.palPalEntry[nearestIndex].peBlue );
      indexList[ i ] = i; 
     }
   }
} // end createLogicalPalette
  
// activateSystemPalette
//---------------------------------------------------------------------------
void DirectDrawPalette::activateSystemPalette()
{
	HDC      hdc;
	UINT     error;
	HPALETTE palHandle;

	if (systemPaletteHandle == 0)
 	 { return; }

    if ( paletteMode == _palette_mode_fullscreen ) 
     { return; }

	hdc = GetDC(0);
	error = SetSystemPaletteUse(hdc, SYSPAL_STATIC); 
	
	if (error == SYSPAL_ERROR)
	{
		MessageBox(gapp.hwndApp, "SetSystemPaletteUse Failed", "Error", MB_OK);
	}

	palHandle = SelectPalette(hdc, systemPaletteHandle, false);

	if (palHandle == 0)
	{
		MessageBox(gapp.hwndApp, "SelectPalette Failed", "Error", MB_OK);
	}

	error = RealizePalette(hdc);
	
	if (error == GDI_ERROR)
	{
		MessageBox(gapp.hwndApp, "RealizePalette Failed", "Error", MB_OK);
	}

	ReleaseDC(gapp.hwndApp, hdc);
    
    INT indexList[25];

    for(int i = 0; i < 25; i++ )
     {
      indexList[ i ] = i; 
     }

    SetSysColors( 25, indexList, systemColors );
} // end activateSystemPalette

// activateCurrentPalette
//---------------------------------------------------------------------------
void DirectDrawPalette::activateCurrentPalette()
{
	HDC      hdc;
	UINT     error;
	HPALETTE palHandle;

	if(logPaletteHandle == 0)
	 { return; }

    if ( paletteMode == _palette_mode_fullscreen ) 
     { return; }

	hdc   = GetDC(0);
	
    error = SetSystemPaletteUse(hdc, SYSPAL_NOSTATIC); 

	if (error == SYSPAL_ERROR)
	{
		MessageBox(gapp.hwndApp, "SetSystemPaletteUse Failed", "Error", MB_OK);
	}
    
	
    palHandle = SelectPalette(hdc, logPaletteHandle, false);

	if (palHandle == 0)
	{
		MessageBox(gapp.hwndApp, "SelectPalette Failed", "Error", MB_OK);
	}

	error = RealizePalette(hdc);

	if (error == GDI_ERROR)
	{
		MessageBox(gapp.hwndApp, "RealizePalette Failed", "Error", MB_OK);
	}

    
    if (lpDDPalette != 0)
	{
		lpDDPalette->SetEntries(0, 0, 256, logicalPalette.palPalEntry);
	}
   

	ReleaseDC( gapp.hwndApp, hdc);

    INT indexList[25];

    for(int i = 0; i < 25; i++ )
     {
      indexList[ i ] = i; 
     }

    SetSysColors( 25, indexList, logicalColors );

} // end activateCurrentPalette

// loadACTPalette
//---------------------------------------------------------------------------
void DirectDrawPalette::loadACTPalette(const char *filename, LPDIRECTDRAWSURFACE lpAttachSurface)
{
	BYTE red;
	BYTE green;
	BYTE blue;

	FILE *in_file = fopen(filename, "rb");
	assert(in_file != 0);

	for(int i = 0; i < 256; i++)
	{
		fread(&red,   1, sizeof(BYTE), in_file);
		fread(&green, 1, sizeof(BYTE), in_file);
		fread(&blue,  1, sizeof(BYTE), in_file);
 
		paletteTable[i].peRed   = red;
		paletteTable[i].peGreen = green; 
		paletteTable[i].peBlue  = blue;
		paletteTable[i].peFlags = PC_NOCOLLAPSE;
	}

	fclose(in_file);

	HRESULT result;
	if (lpDDPalette != 0)
	{
		lpDDPalette->Release();
		lpDDPalette = 0;
	}

	createLogicalPalette();
    
    result = lpDDLink->CreatePalette(DDPCAPS_8BIT, logicalPalette.palPalEntry, &lpDDPalette, 0);

	if (result != DD_OK)
	{
		MessageBox(gapp.hwndApp, "Create Palette Failed", "Error", MB_OK);
	}

	result = lpAttachSurface->SetPalette(lpDDPalette);

	if (result != DD_OK)
	{
		MessageBox(gapp.hwndApp, "SetPalette Failed", "Error", MB_OK);
	}

	result = lpDDPalette->SetEntries(0, 0, 256,  logicalPalette.palPalEntry);

	if (result != DD_OK)
	{
		MessageBox(gapp.hwndApp, "Set Entries Failed", "Error", MB_OK);
	}

    activateCurrentPalette();

} // end loadACTPalette


//---------------------------------------------------------------------------
void DirectDrawPalette::loadLibPalette(const RGBColor *color, LPDIRECTDRAWSURFACE lpAttachSurface)
{
	for(int i = 0; i < 256; i++)
	{
		paletteTable[i].peRed   = color[i].red;
		paletteTable[i].peGreen = color[i].green; 
		paletteTable[i].peBlue  = color[i].blue;
		paletteTable[i].peFlags = PC_NOCOLLAPSE;
	}

	HRESULT result;
	if (lpDDPalette != 0)
	{
		lpDDPalette->Release();
		lpDDPalette = 0;
	}

	createLogicalPalette();
    
    result = lpDDLink->CreatePalette(DDPCAPS_8BIT, logicalPalette.palPalEntry, &lpDDPalette, 0);

	if (result != DD_OK)
	{
		MessageBox(gapp.hwndApp, "Create Palette Failed", "Error", MB_OK);
	}

	result = lpAttachSurface->SetPalette(lpDDPalette);

	if (result != DD_OK)
	{
		MessageBox(gapp.hwndApp, "SetPalette Failed", "Error", MB_OK);
	}

	result = lpDDPalette->SetEntries(0, 0, 256,  logicalPalette.palPalEntry);

	if (result != DD_OK)
	{
		MessageBox(gapp.hwndApp, "Set Entries Failed", "Error", MB_OK);
	}

    activateCurrentPalette();

} // end loadACTPalette
