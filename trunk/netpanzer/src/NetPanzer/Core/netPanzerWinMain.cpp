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
// XXX need new unix code here
#ifdef WIN32
#include <config.h>
#include <stdlib.h>
#include <windows.h>
#include "netPanzerWinMain.h"
#include "netPanzerWinProc.h"
#include "gapp.hpp"
#include "netPanzerGlobals.h"
#include "resource.h"
#include "GameManager.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine, int nCmdShow)
{
    //first declare a message structure to hold Windows messages
    //and a structure to contain the data pertinent to this application--
    MSG message;

    //clear out the GlobalApp data--
    memset((void *) &gapp, 0, sizeof(gapp));

#ifdef USE_SDL
    SDL_Init(SDL_INIT_EVERYTHING);
#endif

    //Initialize the Windows portion of PanzerKrieg (create a window,
    //fill in the contents of our GlobalApp structure, etc.)
    if( WinInit( hInstance, nCmdShow) == false)
		return false;
	
    if( GameManager::initialize( lpCmdLine ) == false) 
		return false;

    //just cause they do it in the book, for now only--
    ShowCursor( false );

    while( 1 )
    {
	if( PeekMessage( &message, 0, 0, 0, PM_NOREMOVE ) )
   	{
	    if(GetMessage( &message, 0, 0, 0))
	    {
		TranslateMessage(&message);
		DispatchMessage(&message);	
	    } 
	    else
	    {
		return message.wParam;
	    }
	} // ** if PeekMessage
	else
	{
	    GameManager::mainLoop();
	}
	} // ** while
}
////////////////////////////////////////////////////////////////
//END WINMAIN///////////////////////////////////////////////////



//this function creates and registers a windows class for our
//application. then it creates our window using that class--
static bool WinInit( HANDLE hInstance, int nCmdShow)
{
    RECT        client_win_rect;
    WNDCLASS	wc;

    gapp.hinstance   = (HINSTANCE) hInstance;

    wc.style		 = CS_DBLCLKS;
    wc.lpfnWndProc	 = PanzerProc;
    wc.cbClsExtra	 = 0;
    wc.cbWndExtra	 = 0;
    wc.hInstance	 = (HINSTANCE) hInstance;
    wc.hIcon		 = LoadIcon( (HINSTANCE) hInstance, MAKEINTATOM(IDI_ICON1));
    wc.hCursor		 = LoadCursor( 0, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) GetStockObject( BLACK_BRUSH);
    wc.lpszMenuName	 = 0;
    wc.lpszClassName = "netPanzerClass";

    if( !RegisterClass( &wc )) {
	fprintf (stderr, "Failed to register ApplicationClass.\n");
	return false;
    }

    client_win_rect.left = 0;
    client_win_rect.right = 640;
    client_win_rect.top = 0;
    client_win_rect.bottom = 480;

    AdjustWindowRect( &client_win_rect, WS_SYSMENU | WS_BORDER | WS_CAPTION |
		      WS_VISIBLE | WS_MINIMIZEBOX, false );
    
    gapp.hwndApp = CreateWindowEx(
		WS_EX_APPWINDOW,
		"netPanzerClass",
		"netPanzer",
		WS_SYSMENU | WS_BORDER | WS_CAPTION
			   | WS_VISIBLE | WS_MINIMIZEBOX,
		0,
		0,
		client_win_rect.right, //GetSystemMetrics(SM_CXSCREEN),
		client_win_rect.bottom, //GetSystemMetrics(SM_CYSCREEN),
		0,
		0,
		(HINSTANCE) hInstance,
		0 );

    if( gapp.hwndApp == 0) {
	fprintf(stderr, "Couldn't create main window.\n");	
	return false;
    }
    else 
	return true;
}
#endif

