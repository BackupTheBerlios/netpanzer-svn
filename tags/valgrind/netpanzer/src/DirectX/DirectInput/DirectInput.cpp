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
#include <windows.h>
#include <stdio.h>
#include "DirectInput.hpp"
#include "gapp.hpp"

LPDIRECTINPUT  DirectInput::lpDInput = 0; 
LPDIRECTINPUTDEVICE DirectInput::lpDIKeyDevice = 0;

bool DirectInput::initialize( void )
 {
  HRESULT hresult;

  hresult = DirectInputCreate( gapp.hinstance, 
                               DIRECTINPUT_VERSION, 
                               &lpDInput, 
                               0); 
  if ( hresult != DI_OK )
   { 
    MessageBox(gapp.hwndApp, "Direct Input Initalization Failed"
	                         ,"DirectInput: ERROR", MB_OK);   
    return( false );
   } 

  return( true );
 }

void DirectInput::shutdown( void )
 {
  if ( lpDIKeyDevice != 0 )
   {
    lpDIKeyDevice->Unacquire();
    lpDIKeyDevice->Release();
    lpDIKeyDevice = 0;
   }
  
  if ( lpDInput != 0 )
   {
    lpDInput->Release();
    lpDInput = 0;
   }
 }

bool DirectInput::initializeDirectKeyboard( void )
 {
  HRESULT  hresult; 
  
  hresult = lpDInput->CreateDevice( GUID_SysKeyboard, &lpDIKeyDevice, 0); 
  if ( hresult != DI_OK )
   { 
    MessageBox(gapp.hwndApp, "Could Not Create KeyBoard Device"
	                         ,"DirectInput: ERROR", MB_OK);   
    
    return false; 
   } 

  hresult = lpDIKeyDevice->SetDataFormat(&c_dfDIKeyboard); 
 
  if ( hresult != DI_OK )
   { 
    MessageBox(gapp.hwndApp, "Could Not Set KeyBoard Data Format"
	                         ,"DirectInput: ERROR", MB_OK);   

    lpDIKeyDevice->Release(); 
    return false; 
   }
   
  hresult = lpDIKeyDevice->SetCooperativeLevel(gapp.hwndApp, 
                                               DISCL_BACKGROUND| DISCL_NONEXCLUSIVE); 
 
  if ( hresult != DI_OK )
   { 
    MessageBox(gapp.hwndApp, "Could Not Set KeyBoard Cooperative Level"
	                         ,"DirectInput: ERROR", MB_OK);   
    return false; 
   } 
 

  lpDIKeyDevice->Acquire();

  return ( true );
 }

void DirectInput::shutdownDirectKeyboard( void )
 { 
  lpDIKeyDevice->Unacquire();
  lpDIKeyDevice->Release(); 
 }

void DirectInput::getKeyboardState( char *key_table)
 {
  HRESULT hresult;
  HWND hFocusWnd;

  hFocusWnd = GetFocus();

  if ( hFocusWnd != gapp.hwndApp )
   return;
  
  hresult = lpDIKeyDevice->GetDeviceState( 256 ,(LPVOID) key_table );
  
  if ( hresult == DIERR_INPUTLOST )
   {
    hresult = lpDIKeyDevice->Acquire();
    if ( hresult != DI_OK )
     {
      MessageBox(gapp.hwndApp, "Could Not Reacquire Keyboard Device",
                 "DirectInput: ERROR", MB_OK);   
     }  
   
   }
    
 }
