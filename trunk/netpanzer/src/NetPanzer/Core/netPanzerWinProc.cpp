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
#include "netPanzerWinProc.h"
#include "netPanzerGlobals.h"
#include "netPanzerMain.h"
#include "gapp.hpp"
#include "Win32Mouse.hpp"
#include "MouseInterface.hpp"
#include "KeyboardInterface.hpp"
#include "DirectDrawGlobals.hpp"
#include "DirectPlay.h"
#include "winsockglobals.h"
#include "WinSockClientList.h"
#include "WinSockServer.h"
#include "WinSockClient.h"
#include "Server.hpp"

#include "PlayerInterface.hpp"
#include "GameManager.hpp"
#include "cMouse.hpp"

#include "CodeWiz.hpp"

extern GlobalApp gapp;

BOOL gShiftState;

char MapUnshiftedToShifted ( char char_code );

LRESULT CALLBACK PanzerProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	
	PAINTSTRUCT PaintStruct;
	HDC			hDeviceContext;


	switch(uMsg)
	{

     case SM_STREAMEVENT:
         ServStreamAsyncMsg(hwnd, wParam, lParam);
         break;

     case SM_DGRAMEVENT:
         ServDgramAsyncMsg(hwnd, wParam, lParam);
         break;

     case CM_STREAMEVENT:
	     ClientStreamAsyncMsg(hwnd, wParam, lParam);
         break;

     case CM_DGRAMEVENT:
         ClientDgramAsyncMsg(hwnd, wParam, lParam);
         break;

     case CM_GETHOST:
         HandleGetHostMsg(hwnd, wParam, lParam);
         break;

     case SM_REMOVECLIENT :
      {
       PlayerID client_id( 0, lParam );
       
       if( wParam != 0 )
        { SERVER->dropClient( client_id ); }
       else
        { DelClientByWinsockID( lParam ) ; }     
      } break;
	  
      case WM_ACTIVATEAPP :
       {
        BOOL fActive;
        fActive = (BOOL) wParam;
        
        if ( fActive == TRUE )
         {
          LOG( ("WM_ACTIVATEAPP : GDI FALSE") );
          DDraw.restoreAll();
          DDraw.setGDIStatus( FALSE ); 
          //DDraw.palette.activateCurrentPalette();
         }
        
        if ( fActive == FALSE )
         {
          LOG( ("WM_ACTIVATEAPP : GDI TRUE") );
          DDraw.setGDIStatus( TRUE ); 
          //DDraw.palette.activateSystemPalette();
         }
       
       }
      return(0);
      
 	  
      
      case WM_ACTIVATE :
       {
        WORD fActive;
        BOOL fMinimized;
        fActive = LOWORD(wParam);
        fMinimized = (BOOL) HIWORD(wParam);

        if ( (fActive == WA_ACTIVE) || (fActive == WA_CLICKACTIVE) )
         {
          LOG( ("WM_ACTIVATE") );
          //DDraw.setGDIStatus( FALSE ); 
          Win32HideHardwareMouse();
          //DDraw.palette.activateCurrentPalette();
         }

        if( fActive == WA_INACTIVE )
         {
          LOG( ("WM_ACTIVATE") );
          //DDraw.setGDIStatus( TRUE ); 
          Win32ShowHardwareMouse();
          //DDraw.palette.activateSystemPalette();
         }
       }
      return(0);
                   
      case WM_CHAR :
       {
        int repeat = lParam & 0x0000FFFF;
        char scan_code = (char ) ((lParam & 0x00FF0000) >> 16);

        switch( wParam )
         {
          case 8  :
          case 13 :
          case 27 :
		  case 9  :
           {
            wParam = scan_code;
            KeyboardInterface::putChar( (char) 0 );                 
           } break;          
         }

        KeyboardInterface::putChar( (char) wParam, repeat );         
       }
      return(0);
      
      case WM_LBUTTONDOWN:
       {
         MouseInterface::setLeftButtonDown();
         cMouse::setButtonMask(LMOUSE_BUTTON_MASK);
       } break;
  
      case WM_LBUTTONUP:
       {
        MouseInterface::setLeftButtonUp();
        cMouse::clearButtonMask(!LMOUSE_BUTTON_MASK);
       } break;

      case WM_LBUTTONDBLCLK:
	   {
        MouseInterface::setLeftButtonDoubleDown();
	   } break;
	    
	  case WM_MBUTTONDOWN: 
       {
        MouseInterface::setMiddleButtonDown();
        cMouse::setButtonMask(MMOUSE_BUTTON_MASK);
       } break;
  
      case WM_MBUTTONUP:
       {
        MouseInterface::setMiddleButtonUp();
        cMouse::clearButtonMask(!MMOUSE_BUTTON_MASK);
       } break;

      case WM_MBUTTONDBLCLK:
	   {
        MouseInterface::setMiddleButtonDoubleDown();
	   } break;

      case WM_RBUTTONDOWN:
       {
        MouseInterface::setRightButtonDown();
        cMouse::setButtonMask(RMOUSE_BUTTON_MASK);
       } break;

      case WM_RBUTTONUP:
       {
        MouseInterface::setRightButtonUp();
        cMouse::clearButtonMask(!RMOUSE_BUTTON_MASK);
	   } break;

      case WM_RBUTTONDBLCLK:
	   {
        MouseInterface::setRightButtonDoubleDown();
	   } break;

		
	   case WM_CREATE:
       break;

		//at the moment this just fills in the gapp structure with
		//full screen stuff. eventually we'll want to support non-full
		//screen modes--
		case WM_SIZE:
		case WM_MOVE:
		  GetClientRect(gapp.hwndApp, &gapp.wndRectangle);
		  ClientToScreen(gapp.hwndApp, (LPPOINT)&gapp.wndRectangle);
		  ClientToScreen(gapp.hwndApp, (LPPOINT)&gapp.wndRectangle+1);
	 	break;
        
        //handle when they press 'esc' or 'f12'. note that WM_KEYDOWN
		//gets generated when a key is pressed -- not pressed AND released.
		//windows has a separate message for release, WM_KEYUP. this will
		//soon be replaced by calling an event filling routine that will
		//report what has happened in an event structure that the game
		//loop will read to determine how to draw the next frame--
		
        case WM_KEYUP:
		  {	

           long key_data = lParam;
           char scan_code;
           char c;
           unsigned int char_code;
            
            switch( wParam )
			{
				
                case VK_RIGHT :
                case VK_UP :
                case VK_DOWN :
                case VK_LEFT :
                case VK_INSERT :         
                case VK_DELETE :
                case VK_END :           
                case VK_HOME :          
                 {
                  KeyboardInterface::putChar( (char ) 0 );
                  scan_code = (char ) ((key_data & 0x00FF0000) >> 16);
                  KeyboardInterface::putChar( (char ) scan_code, (key_data & 0x0000FFFF) );
                 }
                return 0;
                /*
                default :
                 {
                  
                  char_code = MapVirtualKey( wParam, 2 );
                  if ( char_code != 0  )
                   {
                    c = (char ) (char_code & 0x0000FFFF);

                    if ( (!gShiftState) && (c >= 65) && (c <= 90) )
                      { c += 32; }                   
                    else
                     if ( (gShiftState) && ( (c < 65) || (c > 90) ) )
                      { c = MapUnshiftedToShifted( c ); }                   

                    KeyboardInterface::putChar( c );
                   }
                  else
                   {
                    KeyboardInterface::putChar( (char ) 0 );
                    scan_code = (char ) ((key_data & 0x00FF0000) >> 16);
                    KeyboardInterface::putChar( (char ) scan_code );                   
                   }                   
                 }
                return 0;
                */
            } // ** switch
           
          } // ** case
         return 0;
          
		case WM_PAINT:
		  hDeviceContext = BeginPaint( hwnd, &PaintStruct);
		  EndPaint( hwnd, &PaintStruct);
		return 1;
            		     		
		case WM_DESTROY:
		 {
          PANZER_THREAD_ACTIVE = FALSE;
		  //while( PANZER_THREAD_FINISHED == FALSE );
	      //TerminateThread( (HANDLE) PANZER_THREAD_HANDLE, 0 );
		  netPanzerShutdown();
		  PostQuitMessage(0);
		 }
         break;

	}

	//this is a callback provided by windows to handle
	//any messages we don't--
	return DefWindowProc(hwnd, uMsg, wParam, lParam);

}


char MapUnshiftedToShifted ( char char_code )
 {
  switch( char_code )
  {
  case '`' : return( '~' );
  case '1' : return( '!' );
  case '2' : return( '@' );
  case '3' : return( '#' );
  case '4' : return( '$' );
  case '5' : return( '%' );
  case '6' : return( '^' );
  case '7' : return( '&' );
  case '8' : return( '*' );
  case '9' : return( '(' );
  case '0' : return( ')' );
  case '-' : return( '_' );
  case '=' : return( '+' );
  case '[' : return( '}' );
  case ']' : return( '}' );
  case ';' : return( ':' );
  case 39  : return( '"' );
  case ',' : return( '<' );
  case '.' : return( '>' );
  case '/' : return( '?' );
  case '\\' : return( '|' );
  }
  
  return( 0 );
 };
