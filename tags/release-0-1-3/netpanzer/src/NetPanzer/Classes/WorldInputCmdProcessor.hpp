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
#include "ArrayUtil/Timer.hpp"

class WorldInputCmdProcessor
{
protected:
    iRect world_win;

    bool left_button_hold_action_complete;

    bool selection_box_active;
    iXY box_press, box_release;

    short outpost_goal_selection; //number of the outpost, -1 if none
    iXY output_pos_press;

    iXY right_mouse_scroll_pos;
    iXY right_mouse_scrolled_pos;
    bool right_mouse_scroll;
    bool selectBoundBoxUnits( void );

    bool local_key_table[256];
    bool manual_control_state;
    bool previous_manual_control_state;
    bool manual_fire_state;

    SelectionList selection_group_lists[10];
    SelectionList working_list;
    unsigned long current_selection_list_index;
    unsigned int current_selection_list_bits;
    SelectionList target_list;

    void initializeSelectionLists( void );
    void switchSelectionList( unsigned long new_list_index );
    void setSelectionList( unsigned long new_list_index );
    void cycleSelectedUnits( unsigned long new_list_index );

    unsigned char getCursorStatus( iXY &loc );
    void setMouseCursor( unsigned char world_cursor_status );

    void getManualControlStatus( void );

    void sendManualMoveCommand( unsigned char orientation,
                                bool start_stop );
    void sendManualFireCommand( iXY &world_pos );
    void sendMoveCommand( iXY &world_pos );
    void sendAttackCommand( iXY &world_pos );
    void sendAllianceRequest( iXY &world_pos, bool make_break );

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

    bool getConsoleInputString( char *input_string );

    static short selected_objective_id;

public:

    WorldInputCmdProcessor();

    void process( void );

    void inFocus( void );

    void updateScrollStatus( const iXY &mouse_pos );

    void updateControls( void );

    void closeSelectionBox( void );

    // ** methods for external input control
    bool isUnitSelected( void );
    bool selectUnits( iRect bound_box );
    bool isValidMoveLocation( iXY location );
    bool sendMoveCommandExternal( iXY location );
    void    deselectUnitsExternal( void );

    void startChatMessageAll( void );
    void startChatMessageAllie( void );

    void createGroup( unsigned long group );
    void activateGroup( unsigned long group );

    static bool  isObjectiveSelected( void );
    static char *   getSelectedObjectiveName( void );
    static iXY getSelectedObjectiveWorldPos( void );
};

extern WorldInputCmdProcessor COMMAND_PROCESSOR;

#endif // ** _WORLDINPUTCMDPROCESSOR_HPP
