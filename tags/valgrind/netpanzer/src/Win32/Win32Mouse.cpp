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
#include "Win32Mouse.hpp"
#include "gapp.hpp"

#include <windows.h>

void Win32GetMousePos( long *x_pos, long *y_pos )
 {
  RECT client_area;
  POINT pos; 
  GetCursorPos( &pos );
  ScreenToClient( gapp.hwndApp, &pos );
  GetClientRect( gapp.hwndApp, &client_area );
  
  if ( (pos.x >= client_area.left ) && ( pos.x < client_area.right ) &&
       (pos.y >= client_area.top ) && ( pos.y < client_area.bottom )   
     )
        {
         *x_pos = pos.x;
         *y_pos = pos.y;
        }
  else
   {
    *x_pos = (client_area.right - client_area.left) / 2;
	*y_pos = (client_area.bottom - client_area.top) / 2;
   }
 }

