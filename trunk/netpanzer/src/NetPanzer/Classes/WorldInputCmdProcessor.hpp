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
#ifndef _WORLDINPUTCMDPROCESSOR_HPP
#define _WORLDINPUTCMDPROCESSOR_HPP
 
#include "SelectionList.hpp"
#include "MouseInterface.hpp"
#include "Timer.hpp"

class WorldInputCmdProcessor : public PObject
 {  
  protected:
   Recti world_win;
        
   boolean left_button_hold_action_complete;

   boolean selection_box_active;
   PointXYi box_press, box_release;      
   boolean selectBoundBoxUnits( void );

   boolean local_key_table[256];
   boolean manual_control_state;
   boolean previous_manual_control_state;
   boolean manual_fire_state;
   
   SelectionList selection_group_lists[10];
   SelectionList working_list;
   unsigned long current_selection_list_index;
   SelectionList target_list;
   
   void initializeSelectionLists( void );
   void switchSelectionList( unsigned long new_list_index );
   void setSelectionList( unsigned long new_list_index );
   void cycleSelectedUnits( unsigned long new_list_index ); 
      
   unsigned long unit_cycle_search_iterator;
   void cycleNextUnitAndChangeFocus( void ); 
   
   static boolean unit_flag_visibility_state;
   void toggleUnitFlagVisibility( void );

   unsigned char getCursorStatus( PointXYi &loc ); 
   void setMouseCursor( unsigned char world_cursor_status );

   void getManualControlStatus( void );
 
   void sendManualMoveCommand( unsigned char orientation, 
                               boolean start_stop );
   void sendManualFireCommand( PointXYi &world_pos );
   void sendMoveCommand( PointXYi &world_pos );
   void sendAttackCommand( PointXYi &world_pos );
   void sendAllianceRequest( PointXYi &world_pos, boolean make_break );

   void evalLeftMButtonEvents( MouseEvent &event );
   void evalRightMButtonEvents( MouseEvent &event );
   
   void evaluateMouseEvents( void );

   unsigned long keyboard_input_mode;   

   void keyboardInputModeCommand( void );
   
   long enter_key_hit_count;
   void setKeyboardInputModeChatMesg( void );
   void keyboardInputModeChatMesg( void );
   void setKeyboardInputModeAllieChatMesg( void );
   void keyboardInputModeAllieChatMesg( void );
   
   void evaluateKeyCommands( void );
   void evaluateGroupingKeys( void );
 
   void evaluateKeyboardEvents( void );
  
   boolean getConsoleInputString( char *input_string );

   static short selected_objective_id;
  
  public:

   WorldInputCmdProcessor();

   void process( void );
   
   void inFocus( void );
   
   void updateScrollStatus( PointXYi &mouse_pos );

   void updateControls( void );
    
   void closeSelectionBox( void );
     
   // ** methods for external input control
   boolean isUnitSelected( void );
   boolean selectUnits( Recti bound_box );
   boolean isValidMoveLocation( PointXYi location );
   boolean sendMoveCommandExternal( PointXYi location );
   void    deselectUnitsExternal( void );
  
   void startChatMessageAll( void );
   void startChatMessageAllie( void );
   
   void createGroup( unsigned long group );
   void activateGroup( unsigned long group ); 

   static boolean  isObjectiveSelected( void );
   static char *   getSelectedObjectiveName( void );
   static PointXYi getSelectedObjectiveWorldPos( void ); 
 }; 

extern WorldInputCmdProcessor COMMAND_PROCESSOR;

#endif // ** _WORLDINPUTCMDPROCESSOR_HPP
