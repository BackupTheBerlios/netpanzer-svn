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

#include "PaletteFix.hpp"
#include <windows.h>
#include <dsetup.h>
#include "resource.h"

struct LogicalPalette
{ 
	WORD         palVersion; 
	WORD         palNumEntries; 
	PALETTEENTRY palPalEntry[256]; 
}; 

HINSTANCE hWndInstance;
HWND      hWndApp;
HICON     hTaskIcon;

HPALETTE       logPaletteHandle;
LogicalPalette logicalPalette; 
COLORREF       logicalColors[25];
HPALETTE       systemPaletteHandle;
LogicalPalette systemPalette; 
COLORREF       systemColors[25];
PALETTEENTRY   paletteTable[256];
int            paletteMode;
 
void saveSystemPalette();
void activateSystemPalette(void);

LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	
  switch(uMsg)
   {
    case MYWM_NOTIFYICON:
     {
      switch ( (UINT) lParam )
       {
        case  WM_LBUTTONDOWN :
         {
          activateSystemPalette();
         }break;
       }
     }break;
   
   }

  //this is a callback provided by windows to handle
  //any messages we don't--
  return DefWindowProc(hwnd, uMsg, wParam, lParam);

}

void saveSystemPalette( void )
 {
  unsigned int i = 0;
  
  for( i=0; i<25; i++)
   {
    systemColors[ i ] = GetSysColor( i );
   }
  
  HDC hdc;
  UINT error;

  hdc = GetDC( hWndApp );
  error = GetSystemPaletteEntries( hdc, 0, 256, systemPalette.palPalEntry );
    
  if ( error == 0 )
   {
    MessageBox(hWndApp, "GetSystemPaletteEntries: Failed", "Error", MB_OK);     
   }

  systemPalette.palVersion = 0x300;
  systemPalette.palNumEntries = 256;
  systemPaletteHandle = CreatePalette( (CONST LOGPALETTE *) &systemPalette );
 
  if ( systemPaletteHandle == NULL )
   {
    MessageBox(hWndApp, "Create System Palette Failed", "Error", MB_OK);     
   }
 
  ReleaseDC( hWndApp , hdc);
 }

void activateSystemPalette()
{
	HDC      hdc;
	UINT     error;
	HPALETTE palHandle;

	if (systemPaletteHandle == NULL)
 	 { return; }

	hdc = GetDC(hWndApp);
	error = SetSystemPaletteUse(hdc, SYSPAL_STATIC); 
	
	if (error == SYSPAL_ERROR)
	{
		MessageBox(hWndApp, "SetSystemPaletteUse Failed", "Error", MB_OK);
	}

	palHandle = SelectPalette(hdc, systemPaletteHandle, FALSE);

	if (palHandle == NULL)
	{
		MessageBox(hWndApp, "SelectPalette Failed", "Error", MB_OK);
	}

	error = RealizePalette(hdc);
	
	if (error == GDI_ERROR)
	{
		MessageBox(hWndApp, "RealizePalette Failed", "Error", MB_OK);
	}

	ReleaseDC(hWndApp, hdc);
    
    INT indexList[25];

    for(int i = 0; i < 25; i++ )
     {
      indexList[ i ] = i; 
     }

    SetSysColors( 25, indexList, systemColors );
} // end activateSystemPalette


BOOL MyTaskBarAddIcon(HWND hwnd, UINT uID, HICON hicon, LPSTR lpszTip) 
{ 
    BOOL res; 
    NOTIFYICONDATA tnid; 
 
    tnid.cbSize = sizeof(NOTIFYICONDATA); 
    tnid.hWnd = hwnd; 
    tnid.uID = uID; 
    tnid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP; 
    tnid.uCallbackMessage = MYWM_NOTIFYICON; 
    tnid.hIcon = hicon; 
    if (lpszTip) 
        lstrcpyn(tnid.szTip, lpszTip, sizeof(tnid.szTip)); 
    else 
        tnid.szTip[0] = '\0'; 
 
    res = Shell_NotifyIcon(NIM_ADD, &tnid); 
 
    if (hicon) 
        DestroyIcon(hicon); 
 
    return res; 
} 
 


//this function creates and registers a windows class for our
//application. then it creates our window using that class--
static BOOL WinInit( HANDLE hInstance, int nCmdShow)
{
    WNDCLASS	wc;

	hWndInstance = (HINSTANCE) hInstance;

	wc.style		= CS_DBLCLKS;
	wc.lpfnWndProc	= WinProc;
	wc.cbClsExtra	= 0;
	wc.cbWndExtra	= 0;
	wc.hInstance	= (HINSTANCE) hInstance;
	wc.hIcon		= LoadIcon( (HINSTANCE) hInstance, MAKEINTATOM(IDI_APPLICATION));
	wc.hCursor		= LoadCursor( NULL, IDC_ARROW);
	wc.hbrBackground= (HBRUSH) GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName	= NULL;
	wc.lpszClassName= "PaletteFixClass";

	if( !RegisterClass( &wc )) return FALSE;

     hWndApp = CreateWindowEx(
		WS_EX_TOOLWINDOW,
		"PaletteFixClass",
		"PaletteFix",
		WS_BORDER | WS_CAPTION,
		0,
		0,
		256, //GetSystemMetrics(SM_CXSCREEN),
		256, //GetSystemMetrics(SM_CYSCREEN),
		NULL,
		NULL,
		(HINSTANCE) hInstance,
		NULL );

    saveSystemPalette();

    hTaskIcon = LoadIcon( (HINSTANCE) hInstance, MAKEINTATOM(IDI_ICON1));

    MyTaskBarAddIcon( hWndApp, IDI_ICON1, hTaskIcon, "Fixes The Palette" );     
	
    return TRUE;

}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//first declare a message structure to hold Windows messages
	//and a structure to contain the data pertinent to this application--
	MSG message;


	if( WinInit( hInstance, nCmdShow) == FALSE) return FALSE;
	

	while(GetMessage( &message, NULL, 0, 0))
	{

		TranslateMessage(&message);
		DispatchMessage(&message);	
	}
  
   return message.wParam;

}
////////////////////////////////////////////////////////////////
//END WINMAIN///////////////////////////////////////////////////
