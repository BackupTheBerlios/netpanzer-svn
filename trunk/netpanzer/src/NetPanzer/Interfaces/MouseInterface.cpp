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
#ifndef UNIX
#include "MouseInterface.hpp"

#include <windows.h>
#include "Win32Mouse.hpp"

#include "netPanzerGlobals.h"
#include "DDHardSurface.hpp"

#include "cMouse.hpp"


#ifndef _MAP_EDITOR_BUILD
// ** NOTE: TEMPORARY UNIT MOUSE STANDARDIZED
#include "gdatstct.hpp"
sprite_dbase CURSOR_DBASE;
#endif // ** _MAP_EDITOR_BUILD


    unsigned char MouseInterface::cursor_x_size;
    unsigned char MouseInterface::cursor_y_size;
    Surface	  MouseInterface::mouse_cursor( false );

    PointXYi MouseInterface::mouse_pos;  
   
    float    MouseInterface::button_hold_threshold = (const float) .10;

    bool   MouseInterface::left_button_down = false;
    bool   MouseInterface::left_button_up = false;
    bool   MouseInterface::left_button_dbclick_active = false;
    PointXYi  MouseInterface::left_button_down_pos;
    PointXYi  MouseInterface::left_button_up_pos;
    TIMESTAMP MouseInterface::left_button_hold_time;
     
    bool   MouseInterface::right_button_down = false;
    bool   MouseInterface::right_button_up = false;
    bool   MouseInterface::right_button_dbclick_active = false;
    PointXYi  MouseInterface::right_button_down_pos;
    PointXYi  MouseInterface::right_button_up_pos;
    TIMESTAMP MouseInterface::right_button_hold_time;
   
    bool   MouseInterface::middle_button_down = false;
    bool   MouseInterface::middle_button_up = false;
    bool   MouseInterface::middle_button_dbclick_active = false;
    PointXYi  MouseInterface::middle_button_down_pos;
    PointXYi  MouseInterface::middle_button_up_pos;
    TIMESTAMP MouseInterface::middle_button_hold_time;
   
    unsigned char MouseInterface::button_mask;

    MouseEventQueue MouseInterface::event_queue;


void MouseInterface::initialize( void )
 {
  event_queue.initialize( 20 );
  mouse_cursor.setOffsetCenter();
  #ifndef _MAP_EDITOR_BUILD
  // ** NOTE: TEMPORARY
  CURSOR_DBASE.load_dbase( ".\\gdbase\\cursor.dbs" );
  #endif // ** _MAP_EDITOR_BUILD
 }


 void MouseInterface::setMousePosition( long x, long y )
  {
   mouse_pos.x = x;
   mouse_pos.y = y;
  }
    
 bool MouseInterface::buttonHeld( unsigned char mask)
  {

   if ( (mask & _LEFT_BUTTON_MASK) && left_button_down )
    {
		 if( (now() - left_button_hold_time) > button_hold_threshold )
          return( true );
        
    } 
   else
    if ( (mask & _RIGHT_BUTTON_MASK) && right_button_down )
     {
          if( (now() - right_button_hold_time) > button_hold_threshold )
           return( true );
        
     } 
    else 
     if ( (mask & _MIDDLE_BUTTON_MASK) && middle_button_down )
      {
           if( (now() - middle_button_hold_time) > button_hold_threshold )
            return( true );
         
      } 
   
    return( false );
  }

 void MouseInterface::resetButtonHoldStatus( void )
  {
   left_button_hold_time  = now();
   right_button_hold_time = now();
   middle_button_hold_time = now(); 
  }

 void MouseInterface::setLeftButtonDown( void )
  {
   MouseEvent event;
 
   //assert( left_button_down == false );
   left_button_down = true;
   left_button_up = false;
   button_mask = button_mask | 0x01;
   left_button_down_pos = mouse_pos;
   left_button_hold_time = now();

   event.button = _left_mbutton;
   event.event = _event_mbutton_down;
   event.down_pos = left_button_down_pos;
   event_queue.enqueue( event );

  }

 void MouseInterface::setLeftButtonUp( void )
  {
   MouseEvent event;

   //assert( left_button_up == false );
   left_button_down = false;
   left_button_up = true;
   button_mask = button_mask & (~0x01);
   left_button_up_pos = mouse_pos;
   

   event.button = _left_mbutton;
   event.up_pos = left_button_up_pos;

         
   if ( left_button_dbclick_active == true )
    {
     event.event = _event_mbutton_dbclick;
     left_button_dbclick_active = false;
    }
   else
    event.event  = _event_mbutton_click;
         
   event.down_pos = left_button_down_pos;
   event_queue.enqueue( event );
  
   event.event = _event_mbutton_up;
   event_queue.enqueue( event );
	
  }
         
void MouseInterface::setLeftButtonDoubleDown( void )
 {
  left_button_down = true;
  left_button_dbclick_active = true;
  left_button_up = false;
  button_mask = button_mask | 0x01;
  left_button_down_pos = mouse_pos;
  left_button_hold_time = now();
 }


 void MouseInterface::setRightButtonDown( void )
  {
   MouseEvent event;
 
   //assert( right_button_down == false );
   right_button_down = true;
   right_button_up = false;
   button_mask = button_mask | 0x04;
   right_button_down_pos = mouse_pos;
   right_button_hold_time = now(); 
 
   event.button = _right_mbutton;
   event.event = _event_mbutton_down;
   event.down_pos = right_button_down_pos;
   event_queue.enqueue( event );

  }

 void MouseInterface::setRightButtonUp( void )
  {
   MouseEvent event;

   //assert( right_button_up == false );
   right_button_down = false;
   right_button_up = true;
   button_mask = button_mask & (~0x04);
   right_button_up_pos = mouse_pos;
   
   event.button = _right_mbutton;
   event.up_pos = right_button_up_pos;


   if ( right_button_dbclick_active == true )
    {
     event.event = _event_mbutton_dbclick;
     right_button_dbclick_active = false;
    }
   else
    event.event  = _event_mbutton_click;
        
   event.down_pos = right_button_down_pos;
   event_queue.enqueue( event );
  
   event.event = _event_mbutton_up;
   event_queue.enqueue( event );
 
  }

void MouseInterface::setRightButtonDoubleDown( void )
 {
  right_button_down = true;
  right_button_dbclick_active = true;
  right_button_up = false;
  button_mask = button_mask | 0x04;
  right_button_down_pos = mouse_pos;
  right_button_hold_time = now();
 }


 void MouseInterface::setMiddleButtonDown( void )
  {
   MouseEvent event;

   //assert( middle_button_down == false );
   middle_button_down = true;
   middle_button_up = false;
   button_mask = button_mask | 0x02;
   middle_button_down_pos = mouse_pos;
   middle_button_hold_time = now();
  
   event.button = _middle_mbutton;
   event.event = _event_mbutton_down;
   event.down_pos = middle_button_down_pos;
   event_queue.enqueue( event );

  }

void MouseInterface::setMiddleButtonUp( void )
 {
   MouseEvent event;

   //assert( middle_button_up == false );
   middle_button_down = false;
   middle_button_up = true;
   button_mask = button_mask & (~0x02);
   middle_button_up_pos = mouse_pos;
   
   event.button = _middle_mbutton;
   event.up_pos = middle_button_up_pos;


   
  if ( middle_button_dbclick_active == true )
   {
    event.event = _event_mbutton_dbclick;
    middle_button_dbclick_active = false;
   }
  else
   event.event  = _event_mbutton_click;

  event.down_pos = middle_button_down_pos;
  event_queue.enqueue( event );
  
  event.event = _event_mbutton_up;
  event_queue.enqueue( event );

 }

void MouseInterface::setMiddleButtonDoubleDown( void )
 {
  middle_button_down = true;
  middle_button_dbclick_active = true;
  middle_button_up = false;
  button_mask = button_mask | 0x02;
  middle_button_down_pos = mouse_pos;
  middle_button_hold_time = now();
 }


void MouseInterface::setCursor( unsigned char cursor_type )
 {
  #ifndef _MAP_EDITOR_BUILD

  switch( cursor_type )
   {
    case _mcursor_default :
	 cursor_x_size = CURSOR_DBASE.sprite_list[ 0 ].x_size;
     cursor_y_size = CURSOR_DBASE.sprite_list[ 0 ].y_size;

     mouse_cursor.setTo( (void *) CURSOR_DBASE.sprite_list[ 0 ].data,
	                     iXY( cursor_x_size, cursor_y_size ),
						 cursor_x_size, 
						 1 

   					   );

     mouse_cursor.setOffsetCenter();
     mouse.setPointer( &mouse_cursor );
    break;    

    case _mcursor_noentry :
     cursor_x_size = CURSOR_DBASE.sprite_list[ 4 ].x_size;
     cursor_y_size = CURSOR_DBASE.sprite_list[ 4 ].y_size;
     mouse_cursor.setTo( (void *) CURSOR_DBASE.sprite_list[ 4 ].data,
	                     iXY( cursor_x_size, cursor_y_size ),
						 cursor_x_size, 
						 1 
                       );
     mouse_cursor.setOffsetCenter();
     mouse.setPointer( &mouse_cursor );
	break;          

    case _mcursor_move    :
     cursor_x_size = CURSOR_DBASE.sprite_list[ 3 ].x_size;
     cursor_y_size = CURSOR_DBASE.sprite_list[ 3 ].y_size;
     mouse_cursor.setTo( (void *) CURSOR_DBASE.sprite_list[ 3 ].data,
 	                     iXY( cursor_x_size, cursor_y_size ),
						 cursor_x_size,
						 1 
                       );
     mouse_cursor.setOffsetCenter();
     mouse.setPointer( &mouse_cursor );
	break;

    case _mcursor_select  :
     cursor_x_size = CURSOR_DBASE.sprite_list[ 1 ].x_size;
     cursor_y_size = CURSOR_DBASE.sprite_list[ 1 ].y_size;
	 mouse_cursor.setTo( (void *) CURSOR_DBASE.sprite_list[ 1 ].data,
	                     iXY( cursor_x_size, cursor_y_size ),
	 					 cursor_x_size, 
						 1 
                       );
     mouse_cursor.setOffsetCenter();
     mouse.setPointer( &mouse_cursor );
	break;
        
    case _mcursor_target  :   
     cursor_x_size = CURSOR_DBASE.sprite_list[ 2 ].x_size;
     cursor_y_size = CURSOR_DBASE.sprite_list[ 2 ].y_size;
	 mouse_cursor.setTo( (void *) CURSOR_DBASE.sprite_list[ 2 ].data,
	                     iXY( cursor_x_size, cursor_y_size ),
						 cursor_x_size, 
						 1 
                       );
     mouse_cursor.setOffsetCenter();
     mouse.setPointer( &mouse_cursor );
    break;

    case _mcursor_make_allie  :   
     cursor_x_size = CURSOR_DBASE.sprite_list[ 5 ].x_size;
     cursor_y_size = CURSOR_DBASE.sprite_list[ 5 ].y_size;
	 mouse_cursor.setTo( (void *) CURSOR_DBASE.sprite_list[ 5 ].data,
	                     iXY( cursor_x_size, cursor_y_size ),
						 cursor_x_size, 
						 1 
                       );
     mouse_cursor.setOffsetCenter();
     mouse.setPointer( &mouse_cursor );
    break;

    case _mcursor_break_allie  :   
     cursor_x_size = CURSOR_DBASE.sprite_list[ 6 ].x_size;
     cursor_y_size = CURSOR_DBASE.sprite_list[ 6 ].y_size;
	 mouse_cursor.setTo( (void *) CURSOR_DBASE.sprite_list[ 6 ].data,
	                     iXY( cursor_x_size, cursor_y_size ),
						 cursor_x_size, 
						 1 
                       );
     mouse_cursor.setOffsetCenter();
     mouse.setPointer( &mouse_cursor );
    break;

   } // ** switch 
  #endif // ** _MAP_EDITOR_BUILD
 }
    
void MouseInterface::Win32UpdateMousePos( void )
 {
  long x_pos, y_pos;
  Win32GetMousePos( &x_pos, &y_pos );
  mouse_pos.x = x_pos;
  mouse_pos.y = y_pos;
 }

void MouseInterface::showHardwareCursor( void )
 {
  Win32ShowHardwareMouse();
 }

void MouseInterface::hideHardwareCursor( void )
 {
  Win32HideHardwareMouse();
 }

void MouseInterface::updateCursor( void )
 {
  Win32UpdateMousePos();

  //FRAME_BUFFER.lock();
  //mouse_cursor.bltTrans( FRAME_BUFFER, mouse_pos.x, mouse_pos.y ); 
  //FRAME_BUFFER.unlock();
 }
#endif
