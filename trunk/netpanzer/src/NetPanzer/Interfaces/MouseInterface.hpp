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
#ifndef _MOUSEINTERFACE_HPP
#define _MOUSEINTERFACE_HPP

#include "PObject.hpp"
#include "Point.hpp" 
#include "QueueTemplate.hpp"
#include "Surface.hpp"

#include "cTimeStamp.hpp"

#define _LEFT_BUTTON_MASK    0x01
#define _MIDDLE_BUTTON_MASK  0x02
#define _RIGHT_BUTTON_MASK   0x04

class MouseEvent : public PObject
{
public:
	enum {
		EVENT_CLICK = 0x01,
		EVENT_DBCLICK = 0x02,
		EVENT_DOWN = 0x04,
		EVENT_UP = 0x08
	};
  	unsigned char button;
	unsigned char event;
	PointXYi down_pos;
	PointXYi   up_pos;
};

typedef QueueTemplate< MouseEvent > MouseEventQueue;  

class MouseInterface : public PObject
{
private:
  	static Surface mouse_cursor;
   	static unsigned char cursor_x_size;
	static unsigned char cursor_y_size;
  
protected:
  	static PointXYi mouse_pos;  
   
   	static float    button_hold_threshold;

	static bool  left_button_down;
	static bool  left_button_up;
	static bool  left_button_dbclick_active; 
	static PointXYi left_button_down_pos;
	static PointXYi left_button_up_pos;
	static TIMESTAMP left_button_hold_time;
     
	static bool right_button_down;
	static bool right_button_up;
	static bool right_button_dbclick_active;
	static PointXYi right_button_down_pos;
	static PointXYi right_button_up_pos;
	static TIMESTAMP right_button_hold_time;
   
	static bool middle_button_down;
	static bool middle_button_up;
	static bool middle_button_dbclick_active;
	static PointXYi middle_button_down_pos;
	static PointXYi middle_button_up_pos;
	static TIMESTAMP middle_button_hold_time;
   
	static unsigned char button_mask;
  
public:
	enum {
		left_button = 0x01,
		middle_button = 0x02,
		right_button = 0x04
	};

	enum CursorType {
		defaultcursor, noentry, move, select, target, allie, break_allie
	};

	static MouseEventQueue event_queue;        
   
	static void initialize();
   
	static inline void getMousePosition( long *x, long *y )
	{
		*x = mouse_pos.x;
		*y = mouse_pos.y;
	}
 
   static inline unsigned char getButtonMask( void )
    {
     return( button_mask );
    }
     
   static bool buttonHeld( unsigned char mask);
   static void resetButtonHoldStatus( void );
   
   static void setLeftButtonDown( void );
   static void setLeftButtonUp( void );
   static void setLeftButtonDoubleDown( void );
         
   static void setRightButtonDown( void );
   static void setRightButtonUp( void );
   static void setRightButtonDoubleDown( void );

   static void setMiddleButtonDown( void );
   static void setMiddleButtonUp( void );
   static void setMiddleButtonDoubleDown( void );
   
   static void setCursor(CursorType type);
    
   static void updateCursor( void );
};

#endif // ** _MOUSEINTERFACE_HPP
