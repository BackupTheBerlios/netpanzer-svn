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
#include "WorldInputCmdProcessor.hpp"

#include "MouseInterface.hpp"
#include "KeyboardInterface.hpp"
#include "KeyScanCodeDefs.h"

#include "WorldViewInterface.hpp"
#include "MapInterface.hpp"
#include "UnitInterface.hpp"
#include "ObjectiveInterface.hpp"
#include "PlayerInterface.hpp"
#include "ChatInterface.hpp"

#include "DDHardSurface.hpp"
#include "UnitMessageTypes.hpp"
#include "TerminalNetMesg.hpp"
#include "PlayerNetMessage.hpp"
#include "Client.hpp"
#include "NetMessageEncoder.hpp"
#include "PlacementMatrix.hpp"
#include "Sound.hpp"

#include "GameConfig.hpp"

#include "VehicleSelectionView.hpp"
#include "Desktop.hpp"

#include "NetworkState.hpp"

#include "codewiz.hpp"
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

bool WorldInputCmdProcessor::unit_flag_visibility_state = false;
short WorldInputCmdProcessor::selected_objective_id = 0;

WorldInputCmdProcessor::WorldInputCmdProcessor()
 {
  initializeSelectionLists();

  memset( local_key_table, 0, 256 );

  UnitInterface::resetUnitCycleIterator( &unit_cycle_search_iterator );

  keyboard_input_mode = _keyboard_input_mode_command;

  selection_box_active = false;
  previous_manual_control_state = false;
  manual_control_state = false;
  manual_fire_state = false;

  left_button_hold_action_complete = false;
 }

void WorldInputCmdProcessor::initializeSelectionLists( void )
 {
  unsigned long list_index;

  for( list_index = 0; list_index < 10; list_index++ )
   {
    selection_group_lists[ list_index ].initialize( 20, 10, 40 );
   } // ** for

  target_list.initialize( 20, 10, 40 );
  working_list.initialize( 20, 10, 40 );  
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
  if ( current_selection_list_index != new_list_index )
   {
    working_list.copyList( selection_group_lists[ new_list_index ] );
    current_selection_list_index = new_list_index;
   }
 
  working_list.cycleNextUnit(); 
 } 

void WorldInputCmdProcessor::updateScrollStatus(const PointXYi &mouse_pos )
{
  	PointXYi screen_size;
   	double time_slice;
	long  scroll_increment;
	float scroll_rate;

	screen_size = FRAME_BUFFER.getPix();
	time_slice = TimerInterface::getTimeSlice();
	scroll_rate = GameConfig::getScrollRate();
  
	scroll_increment = (long) ( scroll_rate * time_slice );
       
	if(mouse_pos.x >= (screen_size.x - 1))
	{
		WorldViewInterface::scroll_right(scroll_increment);
	}

   	if( mouse_pos.x < 1)
	{
		WorldViewInterface::scroll_left(scroll_increment);
  	}

	if( mouse_pos.y >= (screen_size.y - 1))
	{
		WorldViewInterface::scroll_down(scroll_increment);
  	}

	if( mouse_pos.y < 1)
	{
		WorldViewInterface::scroll_up(scroll_increment);
  	}
 }


unsigned char WorldInputCmdProcessor::getCursorStatus( PointXYi &loc )
 {
  PointXYi map_loc;
  unsigned char unit_loc_status;

  if( (manual_control_state == true) || (manual_fire_state == true) )
   { return ( _cursor_enemy_unit ); }
  
  MapInterface::pointXYtoMapXY( loc, &map_loc ); 

  if ( MapInterface::getMovementValue( map_loc ) >= 0xFF )
   { return( _cursor_blocked ); }
 
  unit_loc_status = UnitInterface::quearyUnitLocationStatus( loc );
  if ( unit_loc_status == _unit_player )
   { return( _cursor_player_unit ); }
  else
   if ( (unit_loc_status == _unit_enemy) && KeyboardInterface::getKeyState(
			   SDLK_a ) )
    { return ( _cursor_make_allie ); } 
   else 
    if (  (unit_loc_status == _unit_enemy) && working_list.isSelected() )
     { return ( _cursor_enemy_unit ); }
    else
     if ( (unit_loc_status == _unit_allied) && KeyboardInterface::getKeyState(
				 SDLK_a ) )
      { return ( _cursor_break_allie ); } 
  
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
  switch( world_cursor_status )
   {
    case _cursor_regular :
     MouseInterface::setCursor( MouseInterface::defaultcursor );
    break;

    case _cursor_move :
     MouseInterface::setCursor( MouseInterface::move );
    break;

    case _cursor_blocked :
     MouseInterface::setCursor( MouseInterface::noentry );
    break;

    case _cursor_player_unit :
     MouseInterface::setCursor( MouseInterface::select );
    break;

    case _cursor_enemy_unit :
     MouseInterface::setCursor( MouseInterface::target );
    break;
  
    case _cursor_make_allie :
     MouseInterface::setCursor( MouseInterface::allie );
    break;
     
    case _cursor_break_allie :
     MouseInterface::setCursor( MouseInterface::break_allie );
    break;

    break;
   } // ** switch

 
 }

void WorldInputCmdProcessor::cycleNextUnitAndChangeFocus( void )
 {
  PointXYi world_pos;

  world_pos = UnitInterface::unitPositionCycle( &unit_cycle_search_iterator );
  WorldViewInterface::setCameraPosition( world_pos );   
 } 

void WorldInputCmdProcessor::toggleUnitFlagVisibility( void )
 {
  if ( GameConfig::getDisplayUnitFlags() == true )
   { GameConfig::setDisplayUnitFlags( false ); }
  else
   { GameConfig::setDisplayUnitFlags( true ); } 
 }


void WorldInputCmdProcessor::getManualControlStatus( void )
 { 
  if ( KeyboardInterface::getKeyState( SDLK_LCTRL ) ||
       KeyboardInterface::getKeyState( SDLK_RCTRL )
     )
   {
    manual_fire_state = true;
   } 
  else
   {
    manual_fire_state = false;
   }

 }

void WorldInputCmdProcessor::evaluateKeyCommands( void )
 {
  //if ( (KeyboardInterface::getKeyPressed( SDLK_u ) == true) )
  // { cycleNextUnitAndChangeFocus(); }

  if ( (KeyboardInterface::getKeyPressed( SDLK_o ) == true) )
   { toggleDisplayOutpostNames();  }

  if ( (KeyboardInterface::getKeyPressed( SDLK_f ) == true) )
   { toggleUnitFlagVisibility();  }

  if ( (KeyboardInterface::getKeyPressed( SDLK_RETURN ) == true) )
   { setKeyboardInputModeChatMesg(); }

  if ( ( KeyboardInterface::getKeyState( SDLK_LCTRL ) ||
			  KeyboardInterface::getKeyState( SDLK_RCTRL ) )
       && (KeyboardInterface::getKeyPressed( SDLK_a ) == true) )
   { setKeyboardInputModeAllieChatMesg(); }

 }

void WorldInputCmdProcessor::evaluateGroupingKeys( void )
 {
  bool shift_status = false;
  bool alt_status = false;

  if( (KeyboardInterface::getKeyState( SDLK_LSHIFT ) == true) ||
      (KeyboardInterface::getKeyState( SDLK_RSHIFT ) == true)  
    )
   {
    shift_status = true;
   }        

  if( (KeyboardInterface::getKeyState( SDLK_LALT ) == true) )
   {
    alt_status = true;
   }        

  //*********************************************************
  if ( (KeyboardInterface::getKeyState( SDLK_1 ) == true) &&
       (KeyboardInterface::getPrevKeyState( SDLK_1) == false)  )
   {
    if ( shift_status == true )
     { 
      setSelectionList(1); 
      ConsoleInterface::postMessage( "Group 1 Created" );
     }
    else 
     if ( alt_status == true ) 
      { switchSelectionList(1); } 
     else 
      { cycleSelectedUnits(1); }
   } // ** if 
    
  //*********************************************************
  if ( (KeyboardInterface::getKeyState( SDLK_2 ) == true) &&
       (KeyboardInterface::getPrevKeyState( SDLK_2) == false)    )
   {
    if ( shift_status == true )
     { 
      setSelectionList(2); 
      ConsoleInterface::postMessage( "Group 2 Created" );
     }
    else 
     if ( alt_status == true ) 
      { switchSelectionList(2);  } 
     else 
      { cycleSelectedUnits(2); }
   } // ** if 

  //*********************************************************
  if ( (KeyboardInterface::getKeyState( SDLK_3 ) == true) &&
       (KeyboardInterface::getPrevKeyState( SDLK_3) == false)    )
   {
    if ( shift_status == true )
     { 
      setSelectionList(3);
      ConsoleInterface::postMessage( "Group 3 Created" );
     }
    else 
     if ( alt_status == true ) 
      { switchSelectionList(3);  } 
     else 
      { cycleSelectedUnits(3); }
   } // ** if 
 
  //*********************************************************
  if ( (KeyboardInterface::getKeyState( SDLK_4 ) == true) &&
       (KeyboardInterface::getPrevKeyState( SDLK_4) == false)    )
   {
    if ( shift_status == true )
     { 
      setSelectionList(4); 
      ConsoleInterface::postMessage( "Group 4 Created" );
     }
    else 
     if ( alt_status == true ) 
      { switchSelectionList(4);  } 
     else 
      { cycleSelectedUnits(4); }
   } // ** if 

  //*********************************************************
  if ( (KeyboardInterface::getKeyState( SDLK_5 ) == true) &&
       (KeyboardInterface::getPrevKeyState( SDLK_5) == false)    )
   {
    if ( shift_status == true )
     { 
      setSelectionList(5); 
      ConsoleInterface::postMessage( "Group 5 Created" );
     }
    else 
     if ( alt_status == true ) 
      { switchSelectionList(5);  } 
     else 
      { cycleSelectedUnits(5); }
   } // ** if 

  //*********************************************************
  if ( (KeyboardInterface::getKeyState( SDLK_6 ) == true) &&
       (KeyboardInterface::getPrevKeyState( SDLK_6) == false)    )
   {
    if ( shift_status == true )
     { 
      setSelectionList(6); 
      ConsoleInterface::postMessage( "Group 6 Created" );
     }
    else 
     if ( alt_status == true ) 
      { switchSelectionList(6);  } 
     else 
      { cycleSelectedUnits(6); }
   } // ** if 

  //*********************************************************
  if ( (KeyboardInterface::getKeyState( SDLK_7 ) == true) &&
       (KeyboardInterface::getPrevKeyState( SDLK_7) == false)    )
   {
    if ( shift_status == true )
     { 
      setSelectionList(7); 
      ConsoleInterface::postMessage( "Group 7 Created" );
     }
    else 
     if ( alt_status == true ) 
      { switchSelectionList(7);  } 
     else 
      { cycleSelectedUnits(7); }
   } // ** if 
 
  //*********************************************************
  if ( (KeyboardInterface::getKeyState( SDLK_8 ) == true) &&
       (KeyboardInterface::getPrevKeyState( SDLK_8) == false)    )
   {
    if ( shift_status == true )
     { 
      setSelectionList(8); 
      ConsoleInterface::postMessage( "Group 8 Created" );     
     }
    else 
     if ( alt_status == true ) 
      { switchSelectionList(8); } 
     else 
      { cycleSelectedUnits(8); }
   } // ** if 

  //*********************************************************
  if ( (KeyboardInterface::getKeyState( SDLK_9 ) == true) &&
       (KeyboardInterface::getPrevKeyState( SDLK_9) == false) )
   {
    if ( shift_status == true )
     { 
      setSelectionList(9); 
      ConsoleInterface::postMessage( "Group 9 Created" );
     }
    else 
     if ( alt_status == true ) 
      { switchSelectionList(9); } 
     else 
      { cycleSelectedUnits(9);  }
   } // ** if 

   //*********************************************************
  if ( (KeyboardInterface::getKeyState( SDLK_0 ) == true) &&
       (KeyboardInterface::getPrevKeyState( SDLK_0) == false) )
   {
    if ( shift_status == true )
     { 
      setSelectionList(0);
      ConsoleInterface::postMessage( "Group 0 Created" );
     }
    else 
     if ( alt_status == true ) 
      { switchSelectionList(0);  } 
     else 
      { cycleSelectedUnits(0); }
   } // ** if 
 
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
  if ( getConsoleInputString( chat_string ) == true )
   {
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
  if ( getConsoleInputString( chat_string ) == true )
   {
    keyboard_input_mode = _keyboard_input_mode_command;
    ConsoleInterface::setInputStringStatus( false );
    ChatInterface::sendCurrentMessage( chat_string );
   }
 }


void WorldInputCmdProcessor::evaluateKeyboardEvents( void )
 {
  switch( keyboard_input_mode )
   {
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
 
  if ( box_press.x > box_release.x )
   { 
	x = box_press.x; 
	box_press.x = box_release.x;
	box_release.x = x;
   }	  
	  
  if ( box_press.y > box_release.y )
   {
    y = box_press.y; 
	box_press.y = box_release.y;
	box_release.y = y;
   }
      
  select_success = working_list.selectBounded( Recti( box_press, box_release ) );
	 
  if ( select_success == false )
   {
	PointXYi box_size;
	box_size = box_release - box_press;
	if ( (box_size.x > 40) || (box_size.y > 40) )
	 select_success = true; 
   
    return( select_success );
   }
  else
   {
    current_selection_list_index = 0xFFFF;
    return( select_success );   
   }
       
 }

void WorldInputCmdProcessor::evaluateMouseEvents( void )
 {
  PointXYi world_pos;
  PointXYi mouse_pos;

  MouseInterface::getMousePosition( (long *)  &mouse_pos.x, (long *) &mouse_pos.y );

  WorldViewInterface::getViewWindow( &world_win );
  WorldViewInterface::clientXYtoWorldXY( world_win, mouse_pos, &world_pos );
                                                                                  
  setMouseCursor( getCursorStatus( world_pos ) );
 
  //updateScrollStatus( mouse_pos );
  
  if ( (MouseInterface::buttonHeld( _LEFT_BUTTON_MASK ) == true) 
       && (manual_control_state == false) && (manual_fire_state == false) 
     ) 
   {
    if ( selection_box_active == false )
     {
      selection_box_active = true;   
     }
   }
  else
   {
    if ( selection_box_active == true )
     {     
      selection_box_active = false;
      box_release = world_pos;
      left_button_hold_action_complete = selectBoundBoxUnits();
     }
   } 
 
  MouseEvent event;

  while( MouseInterface::event_queue.isReady() )
   {
    event = MouseInterface::event_queue.dequeue();

    if( event.button == MouseInterface::left_button )
	 evalLeftMButtonEvents( event );

	if( event.button == MouseInterface::right_button )
	 evalRightMButtonEvents( event );
   }

  left_button_hold_action_complete = false;
 }


void WorldInputCmdProcessor::evalLeftMButtonEvents( MouseEvent &event )
 {
  PointXYi world_pos;
  unsigned char click_status;

  if ( (manual_control_state == true) ||
       KeyboardInterface::getKeyState( SDLK_LCTRL ) ||
       KeyboardInterface::getKeyState( SDLK_RCTRL ) 
     )
   {
    
    if (event.event == MouseEvent::EVENT_DOWN )
    //if (event.event == _event_mbutton_click )
     {
      WorldViewInterface::clientXYtoWorldXY( world_win, event.down_pos, &world_pos );
      sendManualFireCommand( world_pos );
     }

   } 
  else
   {
    if ( event.event == MouseEvent::EVENT_DOWN )
     {
      if ( selection_box_active == false )
       {
         WorldViewInterface::clientXYtoWorldXY( world_win, event.down_pos, &world_pos );
         box_press = world_pos;
         box_release = world_pos;
       }
     } // ** _event_mbutton_down

  
    if ( (event.event == MouseEvent::EVENT_CLICK) &&
			(left_button_hold_action_complete == false) )
     {
      WorldViewInterface::clientXYtoWorldXY( world_win, event.down_pos, &world_pos );
      click_status = getCursorStatus( world_pos ); 

      switch ( click_status )
	   {
        case _cursor_player_unit :
         {
          if( (KeyboardInterface::getKeyState( SDLK_LSHIFT ) == true) ||
              (KeyboardInterface::getKeyState( SDLK_RSHIFT ) == true)  
            )
           { working_list.addUnit( world_pos ); }  
	      else
           { working_list.selectUnit( world_pos ); }
          
          current_selection_list_index = 0xFFFF;
          if ( working_list.unit_list.containsItems() > 0 )
           {
            sound->PlayUnitVoice( working_list.getHeadUnitType(), 
								  Sound::_selected);
           }

	     } break; 

	    case _cursor_move:
	     {
          sendMoveCommand( world_pos );
	     } break;

	    case _cursor_enemy_unit :
         {
          sendAttackCommand( world_pos );
         } break;
     
        case _cursor_make_allie :
         {
          sendAllianceRequest( world_pos, true ); 
         } break;
       
        case _cursor_break_allie :
         {
          sendAllianceRequest( world_pos, false ); 
         } break;
       
       } // ** switch 
    
     }  // ** if _event_mbutton_click
    
   } // ** else  manual_control_state == false; 

 } // ** evalLeftMButtonEvents


void WorldInputCmdProcessor::evalRightMButtonEvents( MouseEvent &event )
 {
  PointXYi world_pos;
  unsigned char click_status;
 
  Objective *objective;
  PlayerID player_id;


  if (event.event == MouseEvent::EVENT_CLICK )
   {   
       
    if ( (KeyboardInterface::getKeyState(SDLK_LCTRL) ||
		  KeyboardInterface::getKeyState(SDLK_RCTRL) ) ) 
     {
      WorldViewInterface::clientXYtoWorldXY( world_win, event.down_pos, &world_pos );
      player_id = PlayerInterface::getLocalPlayerID();   
      click_status = ObjectiveInterface::quearyObjectiveLocationStatus( world_pos, player_id, &objective ); 
    
      if ( (click_status == _player_occupied_objective_found) )
       {
        selected_objective_id = CURRENT_SELECTED_OUTPOST_ID = objective->objective_state.ID;
        activateVehicleSelectionView( selected_objective_id );
	   }  
      else 
        if ( (click_status == _enemy_occupied_objective_found) || 
             (click_status == _unoccupied_objective_found)
           )
         {
     
	     }
     }
    else
     { working_list.unGroup(); }
   }  // ** _event_mbutton_click
  
 
  if (event.event == MouseEvent::EVENT_DBCLICK )
   {
    WorldViewInterface::clientXYtoWorldXY( world_win, event.down_pos, &world_pos );
    player_id = PlayerInterface::getLocalPlayerID();   
    click_status = ObjectiveInterface::quearyObjectiveLocationStatus( world_pos, player_id, &objective ); 
 
    if ( (click_status == _player_occupied_objective_found) )
     {
      selected_objective_id = CURRENT_SELECTED_OUTPOST_ID = objective->objective_state.ID;
      activateVehicleSelectionView( selected_objective_id );
	 }      
   }
 
 
 }

void WorldInputCmdProcessor::sendMoveCommand( PointXYi &world_pos )
 {
  PointXYi map_pos;
  PlacementMatrix matrix;

  unsigned long id_list_index;
  unsigned long id_list_size;
  UnitBase *unit_ptr;

  TerminalUnitCmdRequest comm_mesg;
  MultiMessage *encode_message;

   
  id_list_size = working_list.unit_list.containsItems();
  
  if ( id_list_size == 0 )
   return;

  MapInterface::pointXYtoMapXY( world_pos, &map_pos );
  matrix.reset( map_pos );

  PUBLIC_MESSAGE_ENCODER.resetEncoder();
    
  for( id_list_index = 0; id_list_index < id_list_size; id_list_index++ )
   {
    unit_ptr = UnitInterface::getUnit( working_list.unit_list[ id_list_index ] );
	if ( unit_ptr != 0 )
     {
      if ( unit_ptr->unit_state.select == true )
       {
        matrix.getNextEmptyLoc( &map_pos );
	    comm_mesg.comm_request.setHeader( unit_ptr->unit_id, _umesg_flag_unique );
	    comm_mesg.comm_request.setMoveToLoc( map_pos ); 
        
        while( PUBLIC_MESSAGE_ENCODER.encodeMessage( &comm_mesg, sizeof(TerminalUnitCmdRequest), &encode_message ) )
	     {
          CLIENT->sendMessage( encode_message, encode_message->realSize(), 0 );   
          PUBLIC_MESSAGE_ENCODER.resetEncoder();
         }
        //CLIENT->sendMessage( &comm_mesg, sizeof(TerminalUnitCmdRequest), 0 );   

       } // ** if 
     } // ** if unit_ptr->isValid() == true    
   } // ** for
  
  PUBLIC_MESSAGE_ENCODER.getEncodeMessage( &encode_message ); 
  if ( encode_message != 0 )
   {
    CLIENT->sendMessage( encode_message, encode_message->realSize(), 0 );   
   } // ** if 

  //sfx
  sound->PlayUnitSound(working_list.getHeadUnitType() );
  sound->PlayUnitVoice(working_list.getHeadUnitType(), Sound::_move_to);  
 }

void WorldInputCmdProcessor::sendAttackCommand( PointXYi &world_pos )
 {
  TerminalUnitCmdRequest comm_mesg;
  MultiMessage *encode_message;

  UnitBase *target_ptr;

  unsigned long id_list_index;
  unsigned long id_list_size;
  UnitBase *unit_ptr;
   
  if ( working_list.isSelected() == true ) 
   {    
    target_list.selectTarget( world_pos );

    target_ptr = UnitInterface::getUnit( target_list.unit_list[0] );
  
    id_list_size = working_list.unit_list.containsItems();
    
    if ( id_list_size == 0 )
     return;
      
    PUBLIC_MESSAGE_ENCODER.resetEncoder();

	for( id_list_index = 0; id_list_index < id_list_size; id_list_index++ )
     {
      unit_ptr = UnitInterface::getUnit( working_list.unit_list[ id_list_index ] );
	  if ( unit_ptr != 0 )
       {
        if ( unit_ptr->unit_state.select == true )
         {
          comm_mesg.comm_request.setHeader( unit_ptr->unit_id, _umesg_flag_unique );
	      comm_mesg.comm_request.setTargetUnit( target_ptr->unit_id ); 

          while( PUBLIC_MESSAGE_ENCODER.encodeMessage( &comm_mesg, sizeof(TerminalUnitCmdRequest), &encode_message ) )
	       {
            CLIENT->sendMessage( encode_message, encode_message->realSize(), 0 );   
            PUBLIC_MESSAGE_ENCODER.resetEncoder();
           }
          //CLIENT->sendMessage( &comm_mesg, sizeof(TerminalUnitCmdRequest), 0 );   
         } // ** if 
       } // ** if
 
     } // ** for
    
    PUBLIC_MESSAGE_ENCODER.getEncodeMessage( &encode_message ); 
    if ( encode_message != 0 )
     {
      CLIENT->sendMessage( encode_message, encode_message->realSize(), 0 );   
     } // ** if 

    //sfx 
    sound->PlayUnitVoice(working_list.getHeadUnitType(), Sound::_target_enemy);
   } // ** if 
 }

void WorldInputCmdProcessor::sendManualMoveCommand( unsigned char orientation, 
                                                    bool start_stop )
 {
  TerminalUnitCmdRequest comm_mesg;
  unsigned long id_list_index;
  unsigned long id_list_size;
  UnitBase *unit_ptr;
  
  if ( working_list.unit_list.containsItems() > 0 )
   {
    id_list_size = working_list.unit_list.containsItems();

    PUBLIC_MESSAGE_ENCODER.resetEncoder();
    
    for( id_list_index = 0; id_list_index < id_list_size; id_list_index++ )
     {
	  unit_ptr = UnitInterface::getUnit( working_list.unit_list[ id_list_index ] );
      if ( unit_ptr != 0 )
       {
        if ( unit_ptr->unit_state.select == true )
         {
          comm_mesg.comm_request.setHeader( unit_ptr->unit_id, _umesg_flag_unique );
          if ( start_stop == true )
           {
            comm_mesg.comm_request.setStartManualMove( orientation );
           }
          else
           {
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

void WorldInputCmdProcessor::sendManualFireCommand( PointXYi &world_pos )
 {
  TerminalUnitCmdRequest comm_mesg;
  MultiMessage *encode_message;

  unsigned long id_list_index;
  unsigned long id_list_size;
  UnitBase *unit_ptr;
  
  if ( working_list.unit_list.containsItems() > 0 )
   {
    id_list_size = working_list.unit_list.containsItems();

    PUBLIC_MESSAGE_ENCODER.resetEncoder();

    for( id_list_index = 0; id_list_index < id_list_size; id_list_index++ )
     {
 	  unit_ptr = UnitInterface::getUnit( working_list.unit_list[ id_list_index ] );

      if ( unit_ptr != 0 )
       {
        if ( unit_ptr->unit_state.select == true )
         {
          comm_mesg.comm_request.setHeader( unit_ptr->unit_id, _umesg_flag_unique );
          comm_mesg.comm_request.setManualFire( world_pos );
                  
          while( PUBLIC_MESSAGE_ENCODER.encodeMessage( &comm_mesg, sizeof(TerminalUnitCmdRequest), &encode_message ) )
	       {
            CLIENT->sendMessage( encode_message, encode_message->realSize(), 0 );   
            PUBLIC_MESSAGE_ENCODER.resetEncoder();
           }
  	      //CLIENT->sendMessage( &comm_mesg, sizeof(TerminalUnitCmdRequest), 0 );   
         } // ** if 
       } // ** if 
 
     }  // ** for        
   
    PUBLIC_MESSAGE_ENCODER.getEncodeMessage( &encode_message ); 
    if ( encode_message != 0 )
     {
      CLIENT->sendMessage( encode_message, encode_message->realSize(), 0 );   
     } // ** if 
    // SFX
    sound->PlayUnitVoice(working_list.getHeadUnitType(), Sound::_target_enemy);
   } // ** if containsItems() > 0  

 }
 
void WorldInputCmdProcessor::sendAllianceRequest( PointXYi &world_pos, bool make_break )
 {
  UnitBase *target_ptr;

  target_list.selectTarget( world_pos );

  if ( target_list.isSelected() == true )
   {
    target_ptr = UnitInterface::getUnit( target_list.unit_list[0] );

    PlayerAllianceRequest  allie_request;
    allie_request.allie_by_player_index = PlayerInterface::getLocalPlayerIndex();
    allie_request.allie_with_player_index = target_ptr->unit_id.getPlayer();          

    if ( make_break )
     { allie_request.alliance_request_type = _player_make_alliance; }
    else
     { allie_request.alliance_request_type = _player_break_alliance; }

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
  char key_char;
  while (KeyboardInterface::getChar(&key_char))
   {
    // Check for extended code.
	if (key_char == 0)
	 {
	  if (KeyboardInterface::getChar(&key_char))
	   {
	    ConsoleInterface::addExtendedChar(key_char);
	    if ((key_char == SDLK_RETURN) )
         { 
          enter_key_hit_count++;
          if (enter_key_hit_count == 2) 
           {
            ConsoleInterface::getInputString( input_string );
            return( true ); 
           }
         }
       } 
     }
    else
	 {
	  ConsoleInterface::addChar(key_char);
	 }
   
   }  
  return( false );
 }




void WorldInputCmdProcessor::inFocus( void )
 {
  PointXYi world_pos;
  PointXYi mouse_pos;

  MouseInterface::getMousePosition( (long *)  &mouse_pos.x, (long *) &mouse_pos.y );

  WorldViewInterface::getViewWindow( &world_win );
  WorldViewInterface::clientXYtoWorldXY( world_win, mouse_pos, &world_pos );

  selection_box_active = false;  
  box_press = world_pos;
  box_release = world_pos; 
 }

void WorldInputCmdProcessor::updateControls( void )
 {
  PointXYi client_pos;
  PointXYi mouse_pos;

  if ( selection_box_active == true )
   {
    MouseInterface::getMousePosition( (long *) &mouse_pos.x, (long *) &mouse_pos.y );

    WorldViewInterface::getViewWindow( &world_win );
    WorldViewInterface::worldXYtoClientXY( world_win, box_press, &client_pos );
        
	if ( client_pos.x < mouse_pos.x )
	 {
	  FRAME_BUFFER.drawHLine( client_pos.x, client_pos.y, mouse_pos.x, 252 );
      FRAME_BUFFER.drawHLine( client_pos.x, mouse_pos.y, mouse_pos.x, 252 );
     }
	else
	 {
	  FRAME_BUFFER.drawHLine( mouse_pos.x, client_pos.y, client_pos.x, 252 );
      FRAME_BUFFER.drawHLine( mouse_pos.x, mouse_pos.y, client_pos.x, 252 );
	 }

	if ( client_pos.y < mouse_pos.y )
	 {
  	  FRAME_BUFFER.drawVLine( client_pos.x, client_pos.y, mouse_pos.y, 252 );
	  FRAME_BUFFER.drawVLine( mouse_pos.x, client_pos.y, mouse_pos.y, 252 );
     }
	else
	 {
  	  FRAME_BUFFER.drawVLine( client_pos.x, mouse_pos.y, client_pos.y, 252 );
	  FRAME_BUFFER.drawVLine( mouse_pos.x, mouse_pos.y, client_pos.y, 252 );
	 }
	
   }
 }

void WorldInputCmdProcessor::closeSelectionBox( void )
 {
  PointXYi world_pos;
  PointXYi mouse_pos;

  MouseInterface::getMousePosition( (long *)  &mouse_pos.x, (long *) &mouse_pos.y );

  WorldViewInterface::getViewWindow( &world_win );
  WorldViewInterface::clientXYtoWorldXY( world_win, mouse_pos, &world_pos );

  if ( selection_box_active == true )
   {     
    selection_box_active = false;
    box_release = world_pos;
    left_button_hold_action_complete = selectBoundBoxUnits();
   }
  else
   {
    selection_box_active = false;  
    box_press = world_pos;
    box_release = world_pos; 
   }
 }


bool WorldInputCmdProcessor::isUnitSelected( void )
 {
  if( working_list.unit_list.containsItems() > 0 )
   { return( true ); }
  else
   { return( false ); }
 }

bool WorldInputCmdProcessor::selectUnits( Recti bound_box )
 {
  long x,y;
 
  if ( bound_box.min.x > bound_box.max.x )
   { 
	x = bound_box.min.x; 
	bound_box.min.x = bound_box.max.x;
	bound_box.max.x = x;
   }	  
	  
  if ( bound_box.min.y > bound_box.max.y )
   {
    y = bound_box.min.y; 
	bound_box.min.y = bound_box.max.y;
	bound_box.max.y = y;
   }

  return( working_list.selectBounded( bound_box ) );
 }

bool WorldInputCmdProcessor::isValidMoveLocation( PointXYi location )
 {
  int click_status;
  
  click_status = getCursorStatus( location ); 
  if ( click_status != _cursor_move )
   { return( false ); }
    
  return( true ); 
 }

bool WorldInputCmdProcessor::sendMoveCommandExternal( PointXYi location )
 {
  int click_status;
  
  click_status = getCursorStatus( location ); 
  if ( click_status != _cursor_move )
   { return( false ); }
    
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
  ConsoleInterface::postMessage( "Group %d Created", group );
 }

void WorldInputCmdProcessor::activateGroup( unsigned long group ) 
 {
  assert( group < 10 );
  switchSelectionList( group );
 }

bool WorldInputCmdProcessor::isObjectiveSelected( void )
 {
  if (Desktop::getVisible("VehicleSelectionView") == true )
   {
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

PointXYi WorldInputCmdProcessor::getSelectedObjectiveWorldPos( void )
 {
  ObjectiveState *objective_state;

  objective_state = ObjectiveInterface::getObjectiveState( selected_objective_id );
  
  return( objective_state->location );
 }
