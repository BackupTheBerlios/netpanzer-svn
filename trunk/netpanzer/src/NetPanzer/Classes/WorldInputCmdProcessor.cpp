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
#include <sstream>
#include "WorldInputCmdProcessor.hpp"

#include "MouseInterface.hpp"
#include "KeyboardInterface.hpp"

#include "WorldViewInterface.hpp"
#include "MapInterface.hpp"
#include "UnitInterface.hpp"
#include "ObjectiveInterface.hpp"
#include "PlayerInterface.hpp"
#include "ChatInterface.hpp"

#include "UnitMessageTypes.hpp"
#include "TerminalNetMesg.hpp"
#include "PlayerNetMessage.hpp"
#include "Client.hpp"
#include "NetMessageEncoder.hpp"
#include "PlacementMatrix.hpp"
#include "System/Sound.hpp"
#include "ScreenSurface.hpp"
#include "Util/Log.hpp"

#include "GameConfig.hpp"

#include "VehicleSelectionView.hpp"
#include "Desktop.hpp"

#include "NetworkState.hpp"

#include "ConsoleInterface.hpp"

WorldInputCmdProcessor COMMAND_PROCESSOR;

enum { _cursor_regular,
       _cursor_move,
       _cursor_blocked,
       _cursor_player_unit,
       _cursor_enemy_unit,
       _cursor_make_allie,
       _cursor_break_allie
     };

enum { _keyboard_input_mode_command,
       _keyboard_input_mode_chat_mesg,
       _keyboard_input_mode_allie_mesg,
     };

short WorldInputCmdProcessor::selected_objective_id = 0;

WorldInputCmdProcessor::WorldInputCmdProcessor()
{
    initializeSelectionLists();

    memset( local_key_table, 0, 256 );

    keyboard_input_mode = _keyboard_input_mode_command;

    selection_box_active = false;
    outpost_goal_selection = -1;
    previous_manual_control_state = false;
    manual_control_state = false;
    manual_fire_state = false;

    left_button_hold_action_complete = false;
    right_mouse_scroll = false;
}

void WorldInputCmdProcessor::initializeSelectionLists()
{
    /* empty */
}

void WorldInputCmdProcessor::switchSelectionList( unsigned long new_list_index )
{
    working_list.copyList( selection_group_lists[ new_list_index ] );
    working_list.select();
}

void WorldInputCmdProcessor::setSelectionList( unsigned long new_list_index )
{
    selection_group_lists[ new_list_index ].copyList( working_list );
    working_list.select();
}

void WorldInputCmdProcessor::cycleSelectedUnits( unsigned long new_list_index )
{
    if ( current_selection_list_index != new_list_index ) {
        working_list.copyList( selection_group_lists[ new_list_index ] );
        current_selection_list_index = new_list_index;
    }

    working_list.cycleNextUnit();
}

void WorldInputCmdProcessor::updateScrollStatus(const iXY &mouse_pos )
{
    iXY screen_size;
    double time_slice;
    long  scroll_increment;
    float scroll_rate;

    screen_size = screen->getPix();
    time_slice = TimerInterface::getTimeSlice();
    scroll_rate = gameconfig->scrollrate;

    scroll_increment = (long) ( scroll_rate * time_slice );

    if(right_mouse_scroll) {
        int x,y;
        int buttons=SDL_GetMouseState(&x,&y);
        if(!(buttons&SDL_BUTTON(SDL_BUTTON_RIGHT))) {
            // sometimes the winning page or something comes up
            //  as you're holding down the right mouse button
            //  and the UP message doesn't come through
            right_mouse_scroll=false;
        }
        else if(mouse_pos.x!=right_mouse_scroll_pos.x || mouse_pos.y!=right_mouse_scroll_pos.y) {
            // we're holding down the right mouse button, and mouse has moved
            int x_move=mouse_pos.x-right_mouse_scroll_pos.x;
            int y_move=mouse_pos.y-right_mouse_scroll_pos.y;
            SDL_WarpMouse(right_mouse_scroll_pos.x,right_mouse_scroll_pos.y);

            WorldViewInterface::scroll_right(x_move*4);
            WorldViewInterface::scroll_down(y_move*4);
            right_mouse_scrolled_pos.x+=x_move;
            right_mouse_scrolled_pos.y+=y_move;
        }
        return;
    }

    if(((bool)gameconfig->fullscreen)!=true) {
        // don't do border scrolling on windowed mode because
        //  the window isn't always on the edge of the screen.
        return;
    }

    if(mouse_pos.x >= (screen_size.x - 1)) {
        WorldViewInterface::scroll_right(scroll_increment);
    }

    if( mouse_pos.x < 1) {
        WorldViewInterface::scroll_left(scroll_increment);
    }

    if( mouse_pos.y >= (screen_size.y - 1)) {
        WorldViewInterface::scroll_down(scroll_increment);
    }

    if( mouse_pos.y < 1) {
        WorldViewInterface::scroll_up(scroll_increment);
    }
}


unsigned char WorldInputCmdProcessor::getCursorStatus( iXY &loc )
{
    iXY map_loc;
    unsigned char unit_loc_status;

    if( (manual_control_state == true) || (manual_fire_state == true) ) {
        return ( _cursor_enemy_unit );
    }

    MapInterface::pointXYtoMapXY( loc, &map_loc );

    if ( MapInterface::getMovementValue( map_loc ) >= 0xFF ) {
        return( _cursor_blocked );
    }

    unit_loc_status = UnitInterface::quearyUnitLocationStatus( loc );
    if ( unit_loc_status == _unit_player ) {
        return( _cursor_player_unit );
    } else
        if ( (unit_loc_status == _unit_enemy) && KeyboardInterface::getKeyState(
                    SDLK_a ) ) {
            return ( _cursor_make_allie );
        } else
            if (  (unit_loc_status == _unit_enemy) && working_list.isSelected() ) {
                return ( _cursor_enemy_unit );
            } else
                if ( (unit_loc_status == _unit_allied) && KeyboardInterface::getKeyState(
                            SDLK_a ) ) {
                    return ( _cursor_break_allie );
                }

    if ( working_list.isSelected() )
        return( _cursor_move );

    return( _cursor_regular );
}

void WorldInputCmdProcessor::
setMouseCursor( unsigned char world_cursor_status )
{
    // XXX yet another abstraction here? probably convert the cursor types to
    // simple strings (which are the names of the cursor images at the same
    // time) and get rid of all this code here and in MouseInterface
    switch( world_cursor_status ) {
    case _cursor_regular :
        MouseInterface::setCursor("default.bmp");
        break;

    case _cursor_move :
        MouseInterface::setCursor("move.bmp");
        break;

    case _cursor_blocked :
        MouseInterface::setCursor("noentry.bmp");
        break;

    case _cursor_player_unit :
        MouseInterface::setCursor("select.bmp");
        break;

    case _cursor_enemy_unit :
        MouseInterface::setCursor("target.bmp");
        break;

    case _cursor_make_allie :
        MouseInterface::setCursor("allie.bmp");
        break;

    case _cursor_break_allie :
        MouseInterface::setCursor("breakallie.bmp");
        break;
    } // ** switch
}

void WorldInputCmdProcessor::getManualControlStatus( void )
{
    if ( KeyboardInterface::getKeyState( SDLK_LCTRL ) ||
            KeyboardInterface::getKeyState( SDLK_RCTRL )
       ) {
        manual_fire_state = true;
    } else {
        manual_fire_state = false;
    }

}

void WorldInputCmdProcessor::evaluateKeyCommands( void )
{
    if ( (KeyboardInterface::getKeyPressed( SDLK_o ) == true) ) {
        toggleDisplayOutpostNames();
    }

    if ( (KeyboardInterface::getKeyPressed( SDLK_f ) == true) ) {
        gameconfig->drawunitflags.toggle();
    }
    if ( (KeyboardInterface::getKeyPressed( SDLK_d ) == true) ) {
        gameconfig->drawunitdamage.toggle();
    }

    if ( (KeyboardInterface::getKeyPressed( SDLK_RETURN ) == true)
            && (KeyboardInterface::getKeyState( SDLK_LALT ) == false)
            && (KeyboardInterface::getKeyState( SDLK_RALT ) == false)) {
        setKeyboardInputModeChatMesg();
    }

    if ( ( KeyboardInterface::getKeyState( SDLK_LCTRL ) ||
            KeyboardInterface::getKeyState( SDLK_RCTRL ) )
            && (KeyboardInterface::getKeyPressed( SDLK_a ) == true) ) {
        setKeyboardInputModeAllieChatMesg();
    }

}

void WorldInputCmdProcessor::evaluateGroupingKeys( void )
{
    bool alt_status = false;
    bool ctrl_status = false;

    if( (KeyboardInterface::getKeyState( SDLK_LCTRL ) == true) ||
            (KeyboardInterface::getKeyState( SDLK_RCTRL ) == true)
      ) {
        ctrl_status = true;
    }

    if( (KeyboardInterface::getKeyState( SDLK_LALT ) == true) ||
            (KeyboardInterface::getKeyState( SDLK_RALT ) == true)
      ) {
        alt_status = true;
    }
    unsigned selected_bits=0;
    int released=0;
    for(int key_code=SDLK_0;  key_code<=SDLK_9; key_code++) {
        unsigned int b=1 << (key_code-SDLK_0);
        if ( (KeyboardInterface::getKeyState( key_code ) == true) ) {
            selected_bits|=b;
        }
        else if(current_selection_list_bits&b) {
            // we've released a key
            released++;
        }
    }
    if(released==0 && selected_bits>0 && selected_bits!=current_selection_list_bits) {
        // we've pressed down a number key
        if(ctrl_status != true && alt_status != true) {
            working_list.unGroup();
        }
        for(int key_code=SDLK_0;  key_code<=SDLK_9; key_code++) {
            if ( (KeyboardInterface::getKeyState( key_code ) != true) ) {
                continue;
            }
            int n=key_code-SDLK_0;
            if(ctrl_status == true) {
                setSelectionList(n);
                std::stringstream s;
                s << "Group " << n << " Created";
                ConsoleInterface::postMessage( s.str().c_str() );
                continue;
            }
            if(alt_status == true) {
                cycleSelectedUnits(n);
                continue;
            }
            working_list.addList( selection_group_lists[ n ] );
        }
        if(alt_status != true) {
            working_list.select();
        }
    }
    current_selection_list_bits=selected_bits;

#if 0
    //*********************************************************
    if ( (KeyboardInterface::getKeyState( SDLK_1 ) == true) &&
            (KeyboardInterface::getPrevKeyState( SDLK_1) == false)  ) {
        if ( shift_status == true ) {
            setSelectionList(1);
            ConsoleInterface::postMessage( "Group 1 Created" );
        } else
            if ( alt_status == true ) {
                switchSelectionList(1);
            } else {
                cycleSelectedUnits(1);
            }
    } // ** if

    //*********************************************************
    if ( (KeyboardInterface::getKeyState( SDLK_2 ) == true) &&
            (KeyboardInterface::getPrevKeyState( SDLK_2) == false)    ) {
        if ( shift_status == true ) {
            setSelectionList(2);
            ConsoleInterface::postMessage( "Group 2 Created" );
        } else
            if ( alt_status == true ) {
                switchSelectionList(2);
            } else {
                cycleSelectedUnits(2);
            }
    } // ** if

    //*********************************************************
    if ( (KeyboardInterface::getKeyState( SDLK_3 ) == true) &&
            (KeyboardInterface::getPrevKeyState( SDLK_3) == false)    ) {
        if ( shift_status == true ) {
            setSelectionList(3);
            ConsoleInterface::postMessage( "Group 3 Created" );
        } else
            if ( alt_status == true ) {
                switchSelectionList(3);
            } else {
                cycleSelectedUnits(3);
            }
    } // ** if

    //*********************************************************
    if ( (KeyboardInterface::getKeyState( SDLK_4 ) == true) &&
            (KeyboardInterface::getPrevKeyState( SDLK_4) == false)    ) {
        if ( shift_status == true ) {
            setSelectionList(4);
            ConsoleInterface::postMessage( "Group 4 Created" );
        } else
            if ( alt_status == true ) {
                switchSelectionList(4);
            } else {
                cycleSelectedUnits(4);
            }
    } // ** if

    //*********************************************************
    if ( (KeyboardInterface::getKeyState( SDLK_5 ) == true) &&
            (KeyboardInterface::getPrevKeyState( SDLK_5) == false)    ) {
        if ( shift_status == true ) {
            setSelectionList(5);
            ConsoleInterface::postMessage( "Group 5 Created" );
        } else
            if ( alt_status == true ) {
                switchSelectionList(5);
            } else {
                cycleSelectedUnits(5);
            }
    } // ** if

    //*********************************************************
    if ( (KeyboardInterface::getKeyState( SDLK_6 ) == true) &&
            (KeyboardInterface::getPrevKeyState( SDLK_6) == false)    ) {
        if ( shift_status == true ) {
            setSelectionList(6);
            ConsoleInterface::postMessage( "Group 6 Created" );
        } else
            if ( alt_status == true ) {
                switchSelectionList(6);
            } else {
                cycleSelectedUnits(6);
            }
    } // ** if

    //*********************************************************
    if ( (KeyboardInterface::getKeyState( SDLK_7 ) == true) &&
            (KeyboardInterface::getPrevKeyState( SDLK_7) == false)    ) {
        if ( shift_status == true ) {
            setSelectionList(7);
            ConsoleInterface::postMessage( "Group 7 Created" );
        } else
            if ( alt_status == true ) {
                switchSelectionList(7);
            } else {
                cycleSelectedUnits(7);
            }
    } // ** if

    //*********************************************************
    if ( (KeyboardInterface::getKeyState( SDLK_8 ) == true) &&
            (KeyboardInterface::getPrevKeyState( SDLK_8) == false)    ) {
        if ( shift_status == true ) {
            setSelectionList(8);
            ConsoleInterface::postMessage( "Group 8 Created" );
        } else
            if ( alt_status == true ) {
                switchSelectionList(8);
            } else {
                cycleSelectedUnits(8);
            }
    } // ** if

    //*********************************************************
    if ( (KeyboardInterface::getKeyState( SDLK_9 ) == true) &&
            (KeyboardInterface::getPrevKeyState( SDLK_9) == false) ) {
        if ( shift_status == true ) {
            setSelectionList(9);
            ConsoleInterface::postMessage( "Group 9 Created" );
        } else
            if ( alt_status == true ) {
                switchSelectionList(9);
            } else {
                cycleSelectedUnits(9);
            }
    } // ** if

    //*********************************************************
    if ( (KeyboardInterface::getKeyState( SDLK_0 ) == true) &&
            (KeyboardInterface::getPrevKeyState( SDLK_0) == false) ) {
        if ( shift_status == true ) {
            setSelectionList(0);
            ConsoleInterface::postMessage( "Group 0 Created" );
        } else
            if ( alt_status == true ) {
                switchSelectionList(0);
            } else {
                cycleSelectedUnits(0);
            }
    } // ** if
#endif

}

void WorldInputCmdProcessor::keyboardInputModeCommand( void )
{
    getManualControlStatus();

    evaluateGroupingKeys();

    evaluateKeyCommands();

    previous_manual_control_state = manual_control_state;
}

void WorldInputCmdProcessor::setKeyboardInputModeChatMesg( void )
{
    ConsoleInterface::setInputStringStatus( true );
    ConsoleInterface::resetInputString( "Message All: " );
    ChatInterface::setMessageScopeAll();
    KeyboardInterface::flushCharBuffer();
    enter_key_hit_count = 1;
    keyboard_input_mode = _keyboard_input_mode_chat_mesg;
}

void WorldInputCmdProcessor::keyboardInputModeChatMesg( void )
{
    char chat_string[256];
    if ( getConsoleInputString( chat_string ) == true ) {
        keyboard_input_mode = _keyboard_input_mode_command;
        ConsoleInterface::setInputStringStatus( false );
        ChatInterface::sendCurrentMessage( chat_string );
    }
}

void WorldInputCmdProcessor::setKeyboardInputModeAllieChatMesg( void )
{
    ConsoleInterface::setInputStringStatus( true );
    ConsoleInterface::resetInputString( "Message Allies : " );
    ChatInterface::setMessageScopeAllies();
    KeyboardInterface::flushCharBuffer();
    enter_key_hit_count = 1;
    keyboard_input_mode = _keyboard_input_mode_allie_mesg;
}

void WorldInputCmdProcessor::keyboardInputModeAllieChatMesg( void )
{
    char chat_string[256];
    if ( getConsoleInputString( chat_string ) == true ) {
        keyboard_input_mode = _keyboard_input_mode_command;
        ConsoleInterface::setInputStringStatus( false );
        ChatInterface::sendCurrentMessage( chat_string );
    }
}


void WorldInputCmdProcessor::evaluateKeyboardEvents( void )
{
    switch( keyboard_input_mode ) {
    case _keyboard_input_mode_command :
        keyboardInputModeCommand();
        break;

    case _keyboard_input_mode_chat_mesg :
        keyboardInputModeChatMesg();
        break;

    case _keyboard_input_mode_allie_mesg :
        keyboardInputModeAllieChatMesg();
        break;
    }
}

bool WorldInputCmdProcessor::selectBoundBoxUnits( void )
{
    bool select_success;
    long x,y;

    if ( box_press.x > box_release.x ) {
        x = box_press.x;
        box_press.x = box_release.x;
        box_release.x = x;
    }

    if ( box_press.y > box_release.y ) {
        y = box_press.y;
        box_press.y = box_release.y;
        box_release.y = y;
    }

    select_success = working_list.selectBounded( iRect( box_press, box_release ) );

    if ( select_success == false ) {
        iXY box_size;
        box_size = box_release - box_press;
        if ( (box_size.x > 40) || (box_size.y > 40) )
            select_success = true;

        return( select_success );
    } else {
        current_selection_list_index = 0xFFFF;
        current_selection_list_bits=0;
        return( select_success );
    }

}

void WorldInputCmdProcessor::evaluateMouseEvents( void )
{
    iXY world_pos;
    iXY mouse_pos;

    MouseInterface::getMousePosition( &mouse_pos.x, &mouse_pos.y );
    WorldViewInterface::getViewWindow( &world_win );

    WorldViewInterface::clientXYtoWorldXY( world_win, mouse_pos, &world_pos );
    setMouseCursor( getCursorStatus( world_pos ) );

    //updateScrollStatus( mouse_pos );

    if ( (MouseInterface::buttonHeld( _LEFT_BUTTON_MASK ) == true)
	 && (manual_control_state == false) && (manual_fire_state == false)
	 ) {
        
      /*        Objective *objective;
        PlayerID player_id;
	
        player_id = PlayerInterface::getLocalPlayerID();
        unsigned char status = ObjectiveInterface::quearyObjectiveLocationStatus( world_pos, player_id, &objective );
        if(status != _player_occupied_objective_found){*/
        if(outpost_goal_selection == -1){
            if ( selection_box_active == false ) {
                selection_box_active = true;
            }
        }
    } else {
        if ( selection_box_active == true ) {
            selection_box_active = false;
            box_release = world_pos;
            left_button_hold_action_complete = selectBoundBoxUnits();
        }else{
          
        }
    }

    while( !MouseInterface::event_queue.empty() ) {
        MouseEvent event = MouseInterface::event_queue.front();
        MouseInterface::event_queue.pop_front();

        if( event.button == MouseInterface::left_button )
            evalLeftMButtonEvents( event );

        if( event.button == MouseInterface::right_button )
            evalRightMButtonEvents( event );
    }

    left_button_hold_action_complete = false;
}


void WorldInputCmdProcessor::evalLeftMButtonEvents( MouseEvent &event )
{
    iXY world_pos;
    unsigned char click_status;

    if ( (manual_control_state == true) ||
            KeyboardInterface::getKeyState( SDLK_LCTRL ) ||
            KeyboardInterface::getKeyState( SDLK_RCTRL )
       ) {

        if (event.event == MouseEvent::EVENT_DOWN )
            //if (event.event == _event_mbutton_click )
        {
            WorldViewInterface::clientXYtoWorldXY( world_win, event.down_pos, &world_pos );
            sendManualFireCommand( world_pos );
        }

    } else {
        if ( event.event == MouseEvent::EVENT_DOWN ) {
	    WorldViewInterface::clientXYtoWorldXY( world_win, event.down_pos, &world_pos );
            Objective *objective;
            PlayerID player_id;
	
            player_id = PlayerInterface::getLocalPlayerID();
            click_status = ObjectiveInterface::quearyObjectiveLocationStatus( world_pos, player_id, &objective );
	
            if ( (click_status == _player_occupied_objective_found) ) {
                box_press = world_pos;
                box_release = world_pos;
                                
                if ( outpost_goal_selection == -1){
                    outpost_goal_selection = objective->objective_state.ID;
                    output_pos_press = objective->objective_state.location;
                }
            
            }else{
                if ( selection_box_active == false) {
                    WorldViewInterface::clientXYtoWorldXY( world_win, event.down_pos, &world_pos );
                    box_press = world_pos;
                    box_release = world_pos;
                }else{
              
                }
            }
        } // ** _event_mbutton_down

        if ( event.event == MouseEvent::EVENT_UP ) {
            if (outpost_goal_selection != -1 ){
                iXY temp;
                MouseInterface::getMousePosition( &temp.x, &temp.y );
                WorldViewInterface::clientXYtoWorldXY( world_win, temp, &world_pos );

                Objective *objective;
                PlayerID player_id = PlayerInterface::getLocalPlayerID();
                int cs = ObjectiveInterface::quearyObjectiveLocationStatus( world_pos, player_id, &objective );

                if ( (cs == _player_occupied_objective_found) 
                    && outpost_goal_selection == objective->objective_state.ID
                    ) {
                    // we've let go of the mouse on the building so we're
                    //  not changing the spawn point
                    selected_objective_id = CURRENT_SELECTED_OUTPOST_ID = objective->objective_state.ID;
                    activateVehicleSelectionView( selected_objective_id );
                }
                else {
                    TerminalOutpostOutputLocRequest term_mesg;

                    term_mesg.output_loc_request.set( outpost_goal_selection,
                                                      world_pos);

                    CLIENT->sendMessage( &term_mesg, sizeof(TerminalOutpostOutputLocRequest), 0 );

                    if ( NetworkState::status == _network_state_client ) {
                    
                        ObjectiveInterface::sendMessage( &(term_mesg.output_loc_request) );
                    }
                }
                outpost_goal_selection = -1;
            }
        }

        if ( (event.event == MouseEvent::EVENT_CLICK) &&
                (left_button_hold_action_complete == false) ) {
            WorldViewInterface::clientXYtoWorldXY( world_win, event.down_pos, &world_pos );
            click_status = getCursorStatus( world_pos );



	    switch ( click_status ) {
            case _cursor_player_unit : {
                    if( (KeyboardInterface::getKeyState( SDLK_LSHIFT ) == true) ||
                            (KeyboardInterface::getKeyState( SDLK_RSHIFT ) == true)
                      ) {
                        working_list.addUnit( world_pos );
                    } else {
                        working_list.selectUnit( world_pos );
                    }

                    current_selection_list_bits=0;
                    current_selection_list_index = 0xFFFF;
                    if (working_list.unit_list.size() > 0) {
                        UnitBase *unit = UnitInterface::getUnit(working_list.unit_list[0]);
                        unit->soundSelected();
                    }

                }
                break;

            case _cursor_move:
            case _cursor_blocked: {
                    if(outpost_goal_selection == -1){
                        sendMoveCommand( world_pos );
                    }
                }
                break;

            case _cursor_enemy_unit : {
                    sendAttackCommand( world_pos );
                }
                break;

            case _cursor_make_allie : {
                    sendAllianceRequest( world_pos, true );
                }
                break;

            case _cursor_break_allie : {
                    sendAllianceRequest( world_pos, false );
                }
                break;

            } // ** switch

        }  // ** if _event_mbutton_click

    } // ** else  manual_control_state == false;

} // ** evalLeftMButtonEvents


void WorldInputCmdProcessor::evalRightMButtonEvents( MouseEvent &event )
{
    iXY world_pos;
    unsigned char click_status;

    Objective *objective;
    PlayerID player_id;

    if (event.event == MouseEvent::EVENT_DOWN ) {
        right_mouse_scroll=true;
        right_mouse_scroll_pos=event.down_pos;
        right_mouse_scrolled_pos.x=right_mouse_scrolled_pos.y=0;
    }
    if (right_mouse_scroll && event.event == MouseEvent::EVENT_UP ) {
        right_mouse_scroll=false;
        if(right_mouse_scrolled_pos.x==0 && right_mouse_scrolled_pos.y==0) {
            // simple right click on the same position
            working_list.unGroup();
        }
        return;
    }

//    if (event.event == MouseEvent::EVENT_CLICK ) {
//    }  // ** _event_mbutton_click


    if (event.event == MouseEvent::EVENT_DBCLICK ) {
        WorldViewInterface::clientXYtoWorldXY( world_win, event.down_pos, &world_pos );
        player_id = PlayerInterface::getLocalPlayerID();
        click_status = ObjectiveInterface::quearyObjectiveLocationStatus( world_pos, player_id, &objective );

        if ( (click_status == _player_occupied_objective_found) ) {
            selected_objective_id = CURRENT_SELECTED_OUTPOST_ID = objective->objective_state.ID;
            activateVehicleSelectionView( selected_objective_id );
        }
    }


}

void WorldInputCmdProcessor::sendMoveCommand( iXY &world_pos )
{
    iXY map_pos;
    PlacementMatrix matrix;

    unsigned long id_list_index;
    size_t id_list_size;
    UnitBase *unit_ptr;

    TerminalUnitCmdRequest comm_mesg;
    MultiMessage *encode_message;


    id_list_size = working_list.unit_list.size();

    if ( id_list_size == 0 )
        return;

    MapInterface::pointXYtoMapXY( world_pos, &map_pos );
    matrix.reset( map_pos );

    PUBLIC_MESSAGE_ENCODER.resetEncoder();

    for( id_list_index = 0; id_list_index < id_list_size; id_list_index++ ) {
        unit_ptr = UnitInterface::getUnit(working_list.unit_list[ id_list_index ]);
        if ( unit_ptr != 0 ) {
            if ( unit_ptr->unit_state.select == true ) {
                matrix.getNextEmptyLoc( &map_pos );
                comm_mesg.comm_request.setHeader( unit_ptr->unit_id, _umesg_flag_unique );
                comm_mesg.comm_request.setMoveToLoc( map_pos );

                while( PUBLIC_MESSAGE_ENCODER.encodeMessage( &comm_mesg, sizeof(TerminalUnitCmdRequest), &encode_message ) ) {
                    CLIENT->sendMessage( encode_message, encode_message->realSize(), 0 );
                    PUBLIC_MESSAGE_ENCODER.resetEncoder();
                }
                //CLIENT->sendMessage( &comm_mesg, sizeof(TerminalUnitCmdRequest), 0 );

            } // ** if
        } // ** if unit_ptr->isValid() == true
    } // ** for

    PUBLIC_MESSAGE_ENCODER.getEncodeMessage( &encode_message );
    if ( encode_message != 0 ) {
        CLIENT->sendMessage( encode_message, encode_message->realSize(), 0 );
    } // ** if

    //sfx
    sound->playUnitSound(working_list.getHeadUnitType() );
    sound->playSound("move");
}

void WorldInputCmdProcessor::sendAttackCommand( iXY &world_pos )
{
    TerminalUnitCmdRequest comm_mesg;
    MultiMessage *encode_message;

    UnitBase *target_ptr;

    size_t id_list_index;
    size_t id_list_size;
    UnitBase *unit_ptr;

    if ( working_list.isSelected() == true ) {
        target_list.selectTarget( world_pos );

        target_ptr = UnitInterface::getUnit( target_list.unit_list[0] );

        id_list_size = working_list.unit_list.size();

        if ( id_list_size == 0 )
            return;

        PUBLIC_MESSAGE_ENCODER.resetEncoder();

        for( id_list_index = 0; id_list_index < id_list_size; id_list_index++ ) {
            unit_ptr = UnitInterface::getUnit( working_list.unit_list[ id_list_index ] );
            if ( unit_ptr != 0 ) {
                if ( unit_ptr->unit_state.select == true ) {
                    comm_mesg.comm_request.setHeader( unit_ptr->unit_id, _umesg_flag_unique );
                    comm_mesg.comm_request.setTargetUnit( target_ptr->unit_id );

                    while( PUBLIC_MESSAGE_ENCODER.encodeMessage( &comm_mesg, sizeof(TerminalUnitCmdRequest), &encode_message ) ) {
                        CLIENT->sendMessage( encode_message, encode_message->realSize(), 0 );
                        PUBLIC_MESSAGE_ENCODER.resetEncoder();
                    }
                    //CLIENT->sendMessage( &comm_mesg, sizeof(TerminalUnitCmdRequest), 0 );
                } // ** if
            } // ** if

        } // ** for

        PUBLIC_MESSAGE_ENCODER.getEncodeMessage( &encode_message );
        if ( encode_message != 0 ) {
            CLIENT->sendMessage( encode_message, encode_message->realSize(), 0 );
        } // ** if

        //sfx
        sound->playSound("target");
    } // ** if
}

void WorldInputCmdProcessor::sendManualMoveCommand( unsigned char orientation,
        bool start_stop )
{
    TerminalUnitCmdRequest comm_mesg;
    size_t id_list_index;
    size_t id_list_size;
    UnitBase *unit_ptr;

    if ( working_list.unit_list.size() > 0 ) {
        id_list_size = working_list.unit_list.size();

        PUBLIC_MESSAGE_ENCODER.resetEncoder();

        for( id_list_index = 0; id_list_index < id_list_size; id_list_index++ ) {
            unit_ptr = UnitInterface::getUnit( working_list.unit_list[ id_list_index ] );
            if ( unit_ptr != 0 ) {
                if ( unit_ptr->unit_state.select == true ) {
                    comm_mesg.comm_request.setHeader( unit_ptr->unit_id, _umesg_flag_unique );
                    if ( start_stop == true ) {
                        comm_mesg.comm_request.setStartManualMove( orientation );
                    } else {
                        comm_mesg.comm_request.setStopManualMove();
                    }

                    //CLIENT->sendMessage( &comm_mesg, sizeof(TerminalUnitCmdRequest), 0 );
                    PUBLIC_MESSAGE_ENCODER.encodeMessage( &comm_mesg, sizeof(TerminalUnitCmdRequest) );

                } // ** if
            } // ** if unit_ptr != 0

        }  // ** for

        PUBLIC_MESSAGE_ENCODER.sendEncodedMessage();
    } // ** if containsItems() > 0


}

void WorldInputCmdProcessor::sendManualFireCommand( iXY &world_pos )
{
    TerminalUnitCmdRequest comm_mesg;
    MultiMessage *encode_message;

    size_t id_list_index;
    size_t id_list_size;
    UnitBase *unit_ptr;

    if ( working_list.unit_list.size() > 0 ) {
        id_list_size = working_list.unit_list.size();

        PUBLIC_MESSAGE_ENCODER.resetEncoder();

        for( id_list_index = 0; id_list_index < id_list_size; id_list_index++ ) {
            unit_ptr = UnitInterface::getUnit( working_list.unit_list[ id_list_index ] );

            if ( unit_ptr != 0 ) {
                if ( unit_ptr->unit_state.select == true ) {
                    comm_mesg.comm_request.setHeader( unit_ptr->unit_id, _umesg_flag_unique );
                    comm_mesg.comm_request.setManualFire( world_pos );

                    while( PUBLIC_MESSAGE_ENCODER.encodeMessage( &comm_mesg, sizeof(TerminalUnitCmdRequest), &encode_message ) ) {
                        CLIENT->sendMessage( encode_message, encode_message->realSize(), 0 );
                        PUBLIC_MESSAGE_ENCODER.resetEncoder();
                    }
                    //CLIENT->sendMessage( &comm_mesg, sizeof(TerminalUnitCmdRequest), 0 );
                } // ** if
            } // ** if

        }  // ** for

        PUBLIC_MESSAGE_ENCODER.getEncodeMessage( &encode_message );
        if ( encode_message != 0 ) {
            CLIENT->sendMessage( encode_message, encode_message->realSize(), 0 );
        } // ** if
        // SFX
        sound->playSound("target");
    } // ** if containsItems() > 0

}

void WorldInputCmdProcessor::sendAllianceRequest( iXY &world_pos, bool make_break )
{
    UnitBase *target_ptr;

    target_list.selectTarget( world_pos );

    if ( target_list.isSelected() == true ) {
        target_ptr = UnitInterface::getUnit( target_list.unit_list[0] );

        PlayerAllianceRequest  allie_request;
        allie_request.allie_by_player_index = PlayerInterface::getLocalPlayerIndex();
        allie_request.allie_with_player_index = target_ptr->unit_id.getPlayer();

        if ( make_break ) {
            allie_request.alliance_request_type = _player_make_alliance;
        } else {
            allie_request.alliance_request_type = _player_break_alliance;
        }

        CLIENT->sendMessage( &allie_request, sizeof(PlayerAllianceRequest), 0 );
    }
}


void WorldInputCmdProcessor::process( void )
{
    evaluateKeyboardEvents();
    evaluateMouseEvents();

    working_list.validateList();
}

bool WorldInputCmdProcessor::getConsoleInputString( char *input_string )
{
    int key_char;
    while (KeyboardInterface::getChar(key_char)) {
        // Check for extended code.
        if (key_char == 0) {
            if (KeyboardInterface::getChar(key_char)) {
                ConsoleInterface::addExtendedChar(key_char);
                if ((key_char == SDLK_RETURN) ) {
                    enter_key_hit_count++;
                    if (enter_key_hit_count == 2) {
                        ConsoleInterface::getInputString( input_string );
                        return( true );
                    }
                }
            }
        } else {
            ConsoleInterface::addChar(key_char);
        }

    }
    return( false );
}




void WorldInputCmdProcessor::inFocus( void )
{
    iXY world_pos;
    iXY mouse_pos;

    MouseInterface::getMousePosition( &mouse_pos.x, &mouse_pos.y );

    WorldViewInterface::getViewWindow( &world_win );
    WorldViewInterface::clientXYtoWorldXY( world_win, mouse_pos, &world_pos );

    selection_box_active = false;
    box_press = world_pos;
    box_release = world_pos;
}

void WorldInputCmdProcessor::updateControls( void )
{
    iXY client_pos;
    iXY mouse_pos;

    if ( selection_box_active == true ) {
        MouseInterface::getMousePosition( &mouse_pos.x, &mouse_pos.y );

        WorldViewInterface::getViewWindow( &world_win );
        WorldViewInterface::worldXYtoClientXY( world_win, box_press, &client_pos );

        if ( client_pos.x < mouse_pos.x ) {
            screen->drawHLine( client_pos.x, client_pos.y, mouse_pos.x, 252 );
            screen->drawHLine( client_pos.x, mouse_pos.y, mouse_pos.x, 252 );
        } else {
            screen->drawHLine( mouse_pos.x, client_pos.y, client_pos.x, 252 );
            screen->drawHLine( mouse_pos.x, mouse_pos.y, client_pos.x, 252 );
        }

        if ( client_pos.y < mouse_pos.y ) {
            screen->drawVLine( client_pos.x, client_pos.y, mouse_pos.y, 252 );
            screen->drawVLine( mouse_pos.x, client_pos.y, mouse_pos.y, 252 );
        } else {
            screen->drawVLine( client_pos.x, mouse_pos.y, client_pos.y, 252 );
            screen->drawVLine( mouse_pos.x, mouse_pos.y, client_pos.y, 252 );
        }

    }

    if (outpost_goal_selection != -1){
        MouseInterface::getMousePosition( &mouse_pos.x, &mouse_pos.y );

        WorldViewInterface::getViewWindow( &world_win );
        WorldViewInterface::worldXYtoClientXY( world_win, output_pos_press, &client_pos );

        screen->drawLine(client_pos.x, client_pos.y, mouse_pos.x, mouse_pos.y,252 );
    }
}

void WorldInputCmdProcessor::closeSelectionBox( void )
{
    iXY world_pos;
    iXY mouse_pos;

    MouseInterface::getMousePosition( &mouse_pos.x, &mouse_pos.y );

    WorldViewInterface::getViewWindow( &world_win );
    WorldViewInterface::clientXYtoWorldXY( world_win, mouse_pos, &world_pos );

    if ( selection_box_active == true ) {
        selection_box_active = false;
        box_release = world_pos;
        left_button_hold_action_complete = selectBoundBoxUnits();
    } else {
        selection_box_active = false;
        box_press = world_pos;
        box_release = world_pos;
    }
}


bool WorldInputCmdProcessor::isUnitSelected()
{
    return working_list.unit_list.size() > 0;
}

bool WorldInputCmdProcessor::selectUnits( iRect bound_box )
{
    long x,y;

    if ( bound_box.min.x > bound_box.max.x ) {
        x = bound_box.min.x;
        bound_box.min.x = bound_box.max.x;
        bound_box.max.x = x;
    }

    if ( bound_box.min.y > bound_box.max.y ) {
        y = bound_box.min.y;
        bound_box.min.y = bound_box.max.y;
        bound_box.max.y = y;
    }

    return( working_list.selectBounded( bound_box ) );
}

bool WorldInputCmdProcessor::isValidMoveLocation( iXY location )
{
    return( true );
}

bool WorldInputCmdProcessor::sendMoveCommandExternal( iXY location )
{
    sendMoveCommand( location );
    return( true );
}

void  WorldInputCmdProcessor::deselectUnitsExternal( void )
{
    working_list.unGroup();
}

void WorldInputCmdProcessor::startChatMessageAll( void )
{
    setKeyboardInputModeChatMesg();
}

void WorldInputCmdProcessor::startChatMessageAllie( void )
{
    setKeyboardInputModeAllieChatMesg();
}

void WorldInputCmdProcessor::createGroup( unsigned long group )
{
    assert( group < 10 );
    setSelectionList( group );
    ConsoleInterface::postMessage( "Group %lu Created", group );
}

void WorldInputCmdProcessor::activateGroup( unsigned long group )
{
    assert( group < 10 );
    switchSelectionList( group );
}

bool WorldInputCmdProcessor::isObjectiveSelected( void )
{
    if (Desktop::getVisible("VehicleSelectionView") == true ) {
        return( true );
    }

    return( false );
}

char *  WorldInputCmdProcessor::getSelectedObjectiveName( void )
{
    ObjectiveState *objective_state;

    objective_state = ObjectiveInterface::getObjectiveState( selected_objective_id );

    return( objective_state->name );
}

iXY WorldInputCmdProcessor::getSelectedObjectiveWorldPos( void )
{
    ObjectiveState *objective_state;

    objective_state = ObjectiveInterface::getObjectiveState( selected_objective_id );

    return( objective_state->location );
}
