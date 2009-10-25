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

#include <sstream>
#include "WorldInputCmdProcessor.hpp"

#include "Interfaces/MouseInterface.hpp"
#include "Interfaces/KeyboardInterface.hpp"

#include "Interfaces/WorldViewInterface.hpp"
#include "Interfaces/MapInterface.hpp"
#include "Units/UnitInterface.hpp"
#include "Objectives/ObjectiveInterface.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "Interfaces/ChatInterface.hpp"

#include "Classes/UnitMessageTypes.hpp"
#include "Classes/Network/TerminalNetMesg.hpp"
#include "Classes/Network/PlayerNetMessage.hpp"
#include "Classes/Network/NetworkClient.hpp"
#include "Classes/Network/NetMessageEncoder.hpp"
#include "Classes/PlacementMatrix.hpp"
#include "System/Sound.hpp"
#include "Classes/ScreenSurface.hpp"
#include "Util/Log.hpp"
#include "Util/NTimer.hpp"

#include "Interfaces/GameConfig.hpp"

#include "Views/Game/VehicleSelectionView.hpp"
#include "Views/Components/Desktop.hpp"

#include "Classes/Network/NetworkState.hpp"

#include "Interfaces/ConsoleInterface.hpp"

#include "Units/Unit.hpp"

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
       _keyboard_input_mode_allie_mesg
     };

ObjectiveID WorldInputCmdProcessor::selected_objective_id = 0;

WorldInputCmdProcessor::WorldInputCmdProcessor()
{
    keyboard_input_mode = _keyboard_input_mode_command;

    selection_box_active = false;
    outpost_goal_selection = OBJECTIVE_NONE;
    manual_fire_state = false;

    right_mouse_scroll_moved = false;
}

void WorldInputCmdProcessor::setSelectionList(unsigned long new_list_index)
{
    selection_group_lists[new_list_index].copyList(working_list);
    working_list.select();
}

void
WorldInputCmdProcessor::cycleSelectedUnits(unsigned long new_list_index)
{
    if (current_selection_list_index != new_list_index) {
        working_list.copyList(selection_group_lists[new_list_index]);
        current_selection_list_index = new_list_index;
    }

    working_list.cycleNextUnit();
}

void
WorldInputCmdProcessor::updateScrollStatus(const iXY &mouse_pos)
{
    double time_slice;
    long  scroll_increment;
    float scroll_rate;

    time_slice = TimerInterface::getTimeSlice();
    scroll_rate = gameconfig->scrollrate;

    scroll_increment = (long) (scroll_rate * time_slice);

    if(((bool)gameconfig->fullscreen)!=true) {
        // don't do border scrolling on windowed mode because
        //  the window isn't always on the edge of the screen.
        return;
    }

    if((unsigned int)mouse_pos.x >= (screen->getWidth() - 1)) {
        WorldViewInterface::scroll_right(scroll_increment);
    }

    if( mouse_pos.x < 1) {
        WorldViewInterface::scroll_left(scroll_increment);
    }

    if( (unsigned int)mouse_pos.y >= (screen->getHeight() - 1)) {
        WorldViewInterface::scroll_down(scroll_increment);
    }

    if( mouse_pos.y < 1) {
        WorldViewInterface::scroll_up(scroll_increment);
    }
}


unsigned char
WorldInputCmdProcessor::getCursorStatus(const iXY& loc)
{
    iXY map_loc;
    
    if (selection_box_active)
        return _cursor_regular;

    if( manual_fire_state == true && working_list.isSelected() )
    {
        return  _cursor_enemy_unit;
    }

    MapInterface::pointXYtoMapXY( loc, &map_loc );

    if (MapInterface::getMovementValue(map_loc) >= 0xFF)
    {
        return _cursor_blocked;
    }
	
	//Unit * unit = UnitInterface::queryNonPlayerUnitAtWorld( loc, PlayerInterface::getLocalPlayerIndex() );
    Unit * unit = UnitInterface::queryUnitAtMapLoc( map_loc );
	if ( unit )
	{
        if ( unit->player->getID() == PlayerInterface::getLocalPlayerIndex() )
        {
            return _cursor_player_unit;
        }
        
		if ( ! PlayerInterface::isAllied(unit->player->getID(), PlayerInterface::getLocalPlayerIndex() ) )
		{
			if ( KeyboardInterface::getKeyState(SDLK_a) )
			{
				if ( PlayerInterface::isSingleAllied(PlayerInterface::getLocalPlayerIndex(), unit->player->getID() ) )
				{
					return _cursor_break_allie;
				}
				
				return _cursor_make_allie;
			}
			else if ( working_list.isSelected() )
			{
				return _cursor_enemy_unit;
			}
		}
		else
		{
			if ( KeyboardInterface::getKeyState(SDLK_a) )
			{
				return _cursor_break_allie;
			}
		}
		
	}
	
    if (working_list.isSelected())
        return _cursor_move;

    return _cursor_regular;
}

void
WorldInputCmdProcessor::setMouseCursor(unsigned char world_cursor_status)
{
    switch(world_cursor_status)
    {
        case _cursor_regular :
            MouseInterface::setCursor("default.png");
            break;

        case _cursor_move :
            MouseInterface::setCursor("move.png");
            break;

        case _cursor_blocked :
            MouseInterface::setCursor("noentry.png");
            break;

        case _cursor_player_unit :
            MouseInterface::setCursor("select.png");
            break;

        case _cursor_enemy_unit :
            MouseInterface::setCursor("target.png");
            break;

        case _cursor_make_allie :
            MouseInterface::setCursor("allie.png");
            break;

        case _cursor_break_allie :
            MouseInterface::setCursor("breakallie.png");
            break;
    }
}

void
WorldInputCmdProcessor::getManualControlStatus()
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

void
WorldInputCmdProcessor::evaluateKeyCommands()
{
    if ( (KeyboardInterface::getKeyPressed( SDLK_e ) == true) )
    {
       selectAllUnitsOnScreen ();
    }

    if ( (KeyboardInterface::getKeyPressed( SDLK_o ) == true) )
    {
        toggleDisplayOutpostNames();
    }

    if ( (KeyboardInterface::getKeyPressed( SDLK_f ) == true) )
    {
        gameconfig->drawunitflags.toggle();
    }

    if ( (KeyboardInterface::getKeyPressed( SDLK_d ) == true) )
    {
        gameconfig->drawunitdamage.toggle();
    }

    if ( (KeyboardInterface::getKeyPressed( SDLK_n ) == true) )
    {
        gameconfig->drawunitowner.toggle();
    }

    if ( (KeyboardInterface::getKeyPressed( SDLK_RETURN ) == true)
            && (KeyboardInterface::getKeyState( SDLK_LALT ) == false)
            && (KeyboardInterface::getKeyState( SDLK_RALT ) == false))
    {
        setKeyboardInputModeChatMesg();
    }

    if ( ( KeyboardInterface::getKeyState( SDLK_LCTRL ) ||
            KeyboardInterface::getKeyState( SDLK_RCTRL ) )
            && (KeyboardInterface::getKeyPressed( SDLK_a ) == true) )
    {
        setKeyboardInputModeAllieChatMesg();
    }

    //If space is pressed, jump to first currently attacked unit
    if( KeyboardInterface::getKeyState( SDLK_SPACE ) == true )
    {
        jumpLastAttackedUnit();
    }

    //If any of the four arrow-keys is pressed, scroll
    if( KeyboardInterface::getKeyState( SDLK_UP ) == true )
    {
    	WorldViewInterface::scroll_up( 15 );
    }
    else if( KeyboardInterface::getKeyState( SDLK_DOWN ) == true )
    {
    	WorldViewInterface::scroll_down( 15 );
    }
    
    if( KeyboardInterface::getKeyState( SDLK_RIGHT ) == true )
    {
    	WorldViewInterface::scroll_right( 15 );
    }
    else if( KeyboardInterface::getKeyState( SDLK_LEFT ) == true )
    {
    	WorldViewInterface::scroll_left( 15 );
    }

    if( KeyboardInterface::getKeyPressed( SDLK_z ) == true )
    {
    	SDL_SaveBMP(SDL_GetVideoSurface(), "/Users/krom/cap.bmp");
    }
}

void
WorldInputCmdProcessor::jumpLastAttackedUnit()
{
    const UnitInterface::Units& units = UnitInterface::getUnits();
    for(UnitInterface::Units::const_iterator i = units.begin();
            i != units.end(); ++i)
    {
        Unit* unit = i->second;
        if(unit->player != PlayerInterface::getLocalPlayer())
            continue;

        if (unit->unit_state.threat_level == _threat_level_under_attack)
        {
            WorldViewInterface::setCameraPosition(unit->unit_state.location);
	    break;
        }
    }
}

void
WorldInputCmdProcessor::evaluateGroupingKeys()
{
    bool alt_status = false;
    bool ctrl_status = false;

    if( (KeyboardInterface::getKeyState(SDLK_LCTRL) == true) ||
            (KeyboardInterface::getKeyState(SDLK_RCTRL) == true))
    {
        ctrl_status = true;
    }

    if( (KeyboardInterface::getKeyState( SDLK_LALT ) == true) ||
            (KeyboardInterface::getKeyState( SDLK_RALT ) == true)
      )
    {
        alt_status = true;
    }
    
    unsigned selected_bits=0;
    int released=0;
    for(int key_code=SDLK_0;  key_code<=SDLK_9; key_code++)
    {
        unsigned int b=1 << (key_code-SDLK_0);
        if ( (KeyboardInterface::getKeyState( key_code ) == true) )
        {
            selected_bits|=b;
        }
        else if(current_selection_list_bits&b)
        {
            // we've released a key
            released++;
        }
    }
    
    if(released==0 && selected_bits>0 && selected_bits!=current_selection_list_bits)
    {
        // we've pressed down a number key
        if(ctrl_status != true && alt_status != true &&
                !KeyboardInterface::getKeyState(SDLK_LSHIFT) &&
                !KeyboardInterface::getKeyState(SDLK_RSHIFT))
        {
            working_list.unGroup();
        }

        for(int key_code=SDLK_0;  key_code<=SDLK_9; key_code++)
        {
            if ( (KeyboardInterface::getKeyState( key_code ) != true) )
            {
                continue;
            }
            int n=key_code-SDLK_0;
            
            if(ctrl_status == true)
            {
                setSelectionList(n);
                std::stringstream s;
                s << "Group " << n << " Created";
                ConsoleInterface::postMessage(Color::brown, false, 0, s.str().c_str() );
                continue;
            }
            
            if(alt_status == true)
            {
                ChatInterface::sendQuickMessage(n);
                continue;
            }
            working_list.addList( selection_group_lists[ n ] );
        }
        if(alt_status != true) {
            working_list.select();
            if(ctrl_status != true) {
                if(now() - lastSelectTime < .4) {
                    centerSelectedUnits();
                }
                lastSelectTime = now();
            }
        }
    }
    current_selection_list_bits=selected_bits;
}

void
WorldInputCmdProcessor::keyboardInputModeCommand()
{
    getManualControlStatus();

    evaluateGroupingKeys();

    evaluateKeyCommands();
}

void
WorldInputCmdProcessor::setKeyboardInputModeChatMesg()
{
    ConsoleInterface::setInputStringStatus( true );
    ConsoleInterface::resetInputString( "Message All: " );
    ChatInterface::setMessageScopeAll();
    KeyboardInterface::flushCharBuffer();
    KeyboardInterface::setTextMode(true);
    enter_key_hit_count = 1;
    keyboard_input_mode = _keyboard_input_mode_chat_mesg;
}

void
WorldInputCmdProcessor::keyboardInputModeChatMesg()
{
    char chat_string[256];
    if (getConsoleInputString(chat_string)) {
        if(strcmp(chat_string, "") != 0)
            ChatInterface::sendCurrentMessage( chat_string );
        keyboard_input_mode = _keyboard_input_mode_command;
        ConsoleInterface::setInputStringStatus(false);             
    }
}

void
WorldInputCmdProcessor::setKeyboardInputModeAllieChatMesg()
{
    ConsoleInterface::setInputStringStatus( true );
    ConsoleInterface::resetInputString( "Message Allies : " );
    ChatInterface::setMessageScopeAllies();
    KeyboardInterface::flushCharBuffer();
    KeyboardInterface::setTextMode(true);
    enter_key_hit_count = 1;
    keyboard_input_mode = _keyboard_input_mode_allie_mesg;
}

void
WorldInputCmdProcessor::keyboardInputModeAllieChatMesg()
{
    char chat_string[256];
    if ( getConsoleInputString( chat_string ) == true ) {
        keyboard_input_mode = _keyboard_input_mode_command;
        ConsoleInterface::setInputStringStatus( false );
        ChatInterface::sendCurrentMessage( chat_string );
    }
}

void
WorldInputCmdProcessor::evaluateKeyboardEvents()
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

bool
WorldInputCmdProcessor::selectBoundBoxUnits()
{
    bool select_success;
    iRect r;
    
    if ( box_press.x > box_release.x ) {
        r.min.x=box_release.x;
        r.max.x=box_press.x;
    } else {
        r.min.x=box_press.x;
        r.max.x=box_release.x;
    }

    if ( box_press.y > box_release.y ) {
        r.min.y=box_release.y;
        r.max.y=box_press.y;
    } else {
        r.min.y=box_press.y;
        r.max.y=box_release.y;
    }

    bool addunits = false;
    if(KeyboardInterface::getKeyState(SDLK_LSHIFT) ||
            KeyboardInterface::getKeyState(SDLK_RSHIFT))
        addunits = true;

    select_success = working_list.selectBounded(r, addunits);

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

void
WorldInputCmdProcessor::evaluateMouseEvents()
{
    iXY world_pos;
    iXY mouse_pos;

    MouseInterface::getMousePosition( &mouse_pos.x, &mouse_pos.y );
    WorldViewInterface::getViewWindow( &world_win );

    WorldViewInterface::clientXYtoWorldXY( world_win, mouse_pos, &world_pos );
    setMouseCursor(getCursorStatus(world_pos));

    while( !MouseInterface::event_queue.empty() )
    {
        MouseEvent event = MouseInterface::event_queue.front();
        MouseInterface::event_queue.pop_front();


        if ( event.event == MouseEvent::EVENT_MOVE )
        {
            evalMouseMoveEvents(event);
        }
        else if( event.button == MouseInterface::left_button )
        {
            evalLeftMButtonEvents(event);
        }
        else if( event.button == MouseInterface::right_button )
        {
            evalRightMButtonEvents(event);
        }
    }
}

void
WorldInputCmdProcessor::evalMouseMoveEvents(const MouseEvent& event)
{
    iXY wpos;
    WorldViewInterface::clientXYtoWorldXY( world_win, event.pos, &wpos );

    if ( selection_box_active )
    {
        box_release = wpos;
        if(abs(box_release.x - box_press.x) > 3
                && abs(box_release.y - box_press.y) > 3)
        {
            selectBoundBoxUnits();
        }
    }

    if ( event.button & MouseInterface::right_button_mask )
    {
        WorldViewInterface::scroll_right(event.relpos.x*4);
        WorldViewInterface::scroll_down(event.relpos.y*4);
        right_mouse_scroll_moved = true;
    }
}

void
WorldInputCmdProcessor::evalLeftMButtonEvents(const MouseEvent &event)
{
    iXY world_pos;
    unsigned char click_status;

    WorldViewInterface::clientXYtoWorldXY(world_win, event.pos, &world_pos);

    if (event.event == MouseEvent::EVENT_DOWN)
    {
        if ( (!selection_box_active)
             && ( manual_fire_state == true
                 || KeyboardInterface::getKeyState( SDLK_LCTRL )
                 || KeyboardInterface::getKeyState( SDLK_RCTRL )
                )
           )
        {
            sendManualFireCommand( world_pos );
            return;
        }

        Objective *objective = 0;
        click_status = ObjectiveInterface::quearyObjectiveLocationStatus(
            world_pos, PlayerInterface::getLocalPlayerIndex(), &objective);
	
        if ( click_status == _player_occupied_objective_found )
        {
            selection_box_active = false;
            outpost_goal_selection = objective->objective_state.ID;
            output_pos_press = objective->objective_state.location; 
        }
        else
        {
            box_press = world_pos;
            box_release = world_pos;
            selection_box_active = true;
        }
    }
    else if(event.event == MouseEvent::EVENT_UP)
    {
        if (selection_box_active)
        {
            selection_box_active = false;
            box_release = world_pos;
            if(abs(box_release.x - box_press.x) > 3
		    && abs(box_release.y - box_press.y) > 3)
            {
                return;
            }
        }
        
        if (outpost_goal_selection != OBJECTIVE_NONE )
        {
            Objective *objective = 0;
            int cs = ObjectiveInterface::quearyObjectiveLocationStatus(
                    world_pos, PlayerInterface::getLocalPlayerIndex(),
                    &objective);
            
            if ( (cs == _player_occupied_objective_found) 
                    && outpost_goal_selection == objective->objective_state.ID
               )
            {
                // we've let go of the mouse on the building so we're
                //  not changing the spawn point
                selected_objective_id = objective->objective_state.ID;
                activateVehicleSelectionView( selected_objective_id );
            }
            else
            {
                TerminalOutpostOutputLocRequest term_mesg;
                
                term_mesg.output_loc_request.set( outpost_goal_selection,
                        world_pos);
                
                NetworkClient::sendMessage(&term_mesg, sizeof(TerminalOutpostOutputLocRequest));

                if ( NetworkState::status == _network_state_client )
                {    
                    ObjectiveInterface::sendMessage( &(term_mesg.output_loc_request) );
                }
            }
            outpost_goal_selection = OBJECTIVE_NONE;
            return;
        }
        
        click_status = getCursorStatus(world_pos);
        switch(click_status)
        {
            case _cursor_player_unit:
            {
                static NTimer dclick_timer(200);
                static int click_times = 0;
                bool addunits = false;
                if( (KeyboardInterface::getKeyState(SDLK_LSHIFT) == true) ||
                        (KeyboardInterface::getKeyState(SDLK_RSHIFT) == true))
                {
                    addunits = true;
                }
                if ( ! dclick_timer.isTimeOut() )
                {
                    if ( click_times )
                    {
                        iRect wr;
                        WorldViewInterface::getViewWindow(&wr);
                        working_list.selectBounded(wr, addunits);
                        click_times=0;
                    }
                    else
                    {
                        working_list.selectSameTypeVisible(world_pos,addunits);
                        dclick_timer.reset();
                        click_times++;
                    }
                    break;
                }
                else if (addunits)
                {
                    working_list.addUnit(world_pos);
                }
                else
                {
                    working_list.selectUnit(world_pos );
                }

                current_selection_list_bits=0;
                current_selection_list_index = 0xFFFF;
                if (working_list.unit_list.size() > 0)
                {
                    Unit *unit = UnitInterface::getUnit(
                            working_list.unit_list[0]);
                    if(unit)
                        unit->soundSelected();
                }
                dclick_timer.reset();
                click_times=0;
                break;
            }
            case _cursor_move:
            case _cursor_blocked:
                if(outpost_goal_selection == OBJECTIVE_NONE)
                    sendMoveCommand(world_pos);
                break;

            case _cursor_enemy_unit:
                 if ( manual_fire_state != true
                        && !KeyboardInterface::getKeyState( SDLK_LCTRL )
                        && !KeyboardInterface::getKeyState( SDLK_RCTRL )
                     )
                 {
                     sendAttackCommand(world_pos);
                 }
                break;

            case _cursor_make_allie:
                sendAllianceRequest(world_pos, true);
                break;

            case _cursor_break_allie:
                sendAllianceRequest(world_pos, false);
                break;
        }
    }
}

void
WorldInputCmdProcessor::evalRightMButtonEvents(const MouseEvent& event)
{
    static NTimer mtimer(150);
    if (event.event == MouseEvent::EVENT_DOWN )
    {
        MouseInterface::setGrabMode(true);
        right_mouse_scroll_moved = false;
        mtimer.reset();
    }
    
    if (event.event == MouseEvent::EVENT_UP )
    {
        MouseInterface::setGrabMode(false);
        if ( ! right_mouse_scroll_moved && mtimer.isTimeOut() )
        {
            // simple right click on the same position after timeout
            working_list.unGroup();
        }
    }
}

void
WorldInputCmdProcessor::sendMoveCommand(const iXY& world_pos)
{
    iXY map_pos;
    PlacementMatrix matrix;

    unsigned long id_list_index;
    size_t id_list_size;
    Unit *unit_ptr;

    TerminalUnitCmdRequest comm_mesg;

    id_list_size = working_list.unit_list.size();

    if ( id_list_size == 0 )
        return;

    MapInterface::pointXYtoMapXY( world_pos, &map_pos );
    matrix.reset( map_pos );

    NetMessageEncoder encoder(true);

    for( id_list_index = 0; id_list_index < id_list_size; id_list_index++ ) {
        unit_ptr = UnitInterface::getUnit(working_list.unit_list[ id_list_index ]);
        if ( unit_ptr != 0 ) {
            if ( unit_ptr->unit_state.select == true ) {
                matrix.getNextEmptyLoc( &map_pos );
                comm_mesg.comm_request.setHeader(unit_ptr->id);

                comm_mesg.comm_request.setMoveToLoc( map_pos );
                encoder.encodeMessage(&comm_mesg,
                        sizeof(TerminalUnitCmdRequest));
            }
        }
    }

    encoder.sendEncodedMessage();

    //sfx
    sound->playUnitSound(working_list.getHeadUnitType() );
    sound->playSound("move");
}

void
WorldInputCmdProcessor::sendAttackCommand(const iXY &world_pos)
{
    TerminalUnitCmdRequest comm_mesg;

    Unit *target_ptr;

    size_t id_list_index;
    size_t id_list_size;
    Unit *unit_ptr;

    if ( working_list.isSelected() == true )
    {
        target_ptr = UnitInterface::queryNonPlayerUnitAtWorld(world_pos,
                                        PlayerInterface::getLocalPlayerIndex());

        if ( ! target_ptr ) // there was nothing there
        {
            return;
        }

        id_list_size = working_list.unit_list.size();

        if ( id_list_size == 0 )
            return;

        NetMessageEncoder encoder(true);

        for( id_list_index = 0; id_list_index < id_list_size; id_list_index++ )
        {
            unit_ptr = UnitInterface::getUnit( working_list.unit_list[ id_list_index ] );
            if ( unit_ptr != 0 )
            {
                if ( unit_ptr->unit_state.select == true )
                {
                    comm_mesg.comm_request.setHeader(unit_ptr->id);
                    comm_mesg.comm_request.setTargetUnit(target_ptr->id);

                    encoder.encodeMessage(&comm_mesg,
                            sizeof(TerminalUnitCmdRequest));
                }
            }
        }

        encoder.sendEncodedMessage();

        //sfx
        sound->playSound("target");
    }
}

void
WorldInputCmdProcessor::sendManualFireCommand(const iXY &world_pos)
{
    TerminalUnitCmdRequest comm_mesg;

    size_t id_list_index;
    size_t id_list_size;
    Unit *unit_ptr;

    if ( working_list.unit_list.size() > 0 ) {
        id_list_size = working_list.unit_list.size();

        NetMessageEncoder encoder(true);

        for( id_list_index = 0; id_list_index < id_list_size; id_list_index++ ) {
            unit_ptr = UnitInterface::getUnit( working_list.unit_list[ id_list_index ] );

            if ( unit_ptr != 0 ) {
                if ( unit_ptr->unit_state.select == true ) {
                    comm_mesg.comm_request.setHeader(unit_ptr->id);
                    comm_mesg.comm_request.setManualFire(world_pos);

                    encoder.encodeMessage(&comm_mesg,
                            sizeof(TerminalUnitCmdRequest));
                }
            }
        }

        encoder.sendEncodedMessage();
        
        // SFX
        sound->playSound("target");
    } // ** if containsItems() > 0
}

void
WorldInputCmdProcessor::sendAllianceRequest(const iXY& world_pos, bool make_break)
{
    Unit *target_ptr = UnitInterface::queryNonPlayerUnitAtWorld(world_pos,
                                        PlayerInterface::getLocalPlayerIndex());

    if ( target_ptr )
    {
        Uint8 type;
        if ( make_break )
        {
            type = _player_make_alliance;
        }
        else
        {
            type = _player_break_alliance;
        }

        PlayerAllianceRequest allie_request;
        allie_request.set(PlayerInterface::getLocalPlayerIndex(),
                                            target_ptr->player->getID(), type);

        NetworkClient::sendMessage( &allie_request, sizeof(PlayerAllianceRequest));
    }
}

void
WorldInputCmdProcessor::process(bool handleMouse)
{
    evaluateKeyboardEvents();
    if ( handleMouse )
    {
        evaluateMouseEvents();
    }
    
    working_list.validateList();
}

bool
WorldInputCmdProcessor::getConsoleInputString(char *input_string)
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
			KeyboardInterface::setTextMode(false);
                        ConsoleInterface::getInputString( input_string );
                        return true;
                    }
                }
            }
        } else {
            ConsoleInterface::addChar(key_char);
        }

    }
    return false;
}

void
WorldInputCmdProcessor::inFocus()
{
    iXY world_pos;
    iXY mouse_pos;

    MouseInterface::getMousePosition( &mouse_pos.x, &mouse_pos.y );

    WorldViewInterface::getViewWindow( &world_win );
    WorldViewInterface::clientXYtoWorldXY( world_win, mouse_pos, &world_pos );

    selection_box_active = false;
}

void
WorldInputCmdProcessor::draw()
{
    if (selection_box_active == true && box_press != box_release)
    {
        WorldViewInterface::getViewWindow(&world_win);
        iXY box1, box2;
        WorldViewInterface::worldXYtoClientXY(world_win, box_press, &box1);
        WorldViewInterface::worldXYtoClientXY(world_win, box_release, &box2);
        --box2.x;
        box2.y-=3;

        screen->drawRect(iRect(box1, box2), Color::white);
    }

    if (outpost_goal_selection != OBJECTIVE_NONE)
    {
        iXY mouse_pos;
        MouseInterface::getMousePosition( &mouse_pos.x, &mouse_pos.y );

        WorldViewInterface::getViewWindow( &world_win );
        iXY pos;
        WorldViewInterface::worldXYtoClientXY(world_win, output_pos_press,&pos);
        screen->drawLine(pos, mouse_pos, Color::blue);
    }
}

void
WorldInputCmdProcessor::closeSelectionBox()
{
    iXY world_pos;
    iXY mouse_pos;

    MouseInterface::getMousePosition( &mouse_pos.x, &mouse_pos.y );

    WorldViewInterface::getViewWindow( &world_win );
    WorldViewInterface::clientXYtoWorldXY( world_win, mouse_pos, &world_pos );

    if (selection_box_active == true) {
        selection_box_active = false;
        box_release = world_pos;
        left_button_hold_action_complete = selectBoundBoxUnits();
    }
}

bool
WorldInputCmdProcessor::isObjectiveSelected()
{
    if (Desktop::getVisible("VehicleSelectionView") == true) {
        return true;
    }
                                                                                
    return false;
}
                                                                                
const char*
WorldInputCmdProcessor::getSelectedObjectiveName()
{
    ObjectiveState *objective_state;
                                                                                
    objective_state = ObjectiveInterface::getObjectiveState(selected_objective_id);
                                                                                
    return objective_state->name;
}

iXY
WorldInputCmdProcessor::getSelectedObjectiveWorldPos()
{
    ObjectiveState *objective_state;
                                                                                
    objective_state = ObjectiveInterface::getObjectiveState(selected_objective_id);
                                                                                
    return objective_state->location;
}

void
WorldInputCmdProcessor::centerSelectedUnits()
{
    /** When you want to center the camera on a group of units you have the
     * problem of where to center the camera. The following nice idea+patch is
     * from Christian Hausknecht <christian.hausknecht@gmx.de>.
     *
     * I check, in which direction most of the units in that group go, and
     * based upon that pick up the unit, which is the most advanced one in this
     * direction. So in most cases you will have a focus on the right end of a
     * group (if that group is in a very long queue passing the map).
     */
    
    Unit *maxyunit = 0;
    Unit *maxxunit = 0;
    Unit *minyunit = 0;
    Unit *minxunit = 0;

    // Direction initialize
    int direction[8];
    for(int i=0;i<8;++i)
        direction[i]=0;
    
    // Vote direction
    bool firstunit = true;
    for(unsigned int id_list_index = 0; id_list_index < working_list.unit_list.size(); id_list_index++) {
        Unit* unit_ptr = UnitInterface::getUnit(working_list.unit_list[id_list_index]);

        if(unit_ptr == 0)
            continue;
                
            // increment one direction
        direction[unit_ptr->unit_state.orientation]+=1;
            // initialize some pointers, get them from first unit
        if(firstunit) {
            maxyunit = unit_ptr;
            maxxunit = unit_ptr;
            minyunit = unit_ptr;
            minxunit = unit_ptr;
            firstunit = false;
        } // choose extremest in each direction
        else {
            if(maxyunit->unit_state.location.y < unit_ptr->unit_state.location.y)
                maxyunit=unit_ptr;
            if(maxxunit->unit_state.location.x < unit_ptr->unit_state.location.x)
                maxxunit=unit_ptr;
            if(minyunit->unit_state.location.y > unit_ptr->unit_state.location.y)
                minyunit=unit_ptr;
            if(minxunit->unit_state.location.x > unit_ptr->unit_state.location.x)
                minxunit=unit_ptr;
        }
    }
    
    // Index of chosen direction (which is most used by all units of a group)
    int preferred_direction = 0;
    int max = -1;
    for(int i=0;i<8;++i) {
        if(direction[i] > max) {
            max=direction[i];
            preferred_direction = i;
        }
    }
        
    // Chose Best unit correspondig to chosen direction
    Unit* unit = 0;
    switch(preferred_direction) {
      case 0:
          unit = maxxunit;
          break;
      case 1:
          if(direction[0]>direction[2])
              unit = maxxunit;
          else
              unit = minyunit;
          break;
      case 2:
          unit = minyunit;
          break;
      case 3:
          if(direction[4]>direction[2])
              unit = minxunit;
          else
              unit = minyunit;
          break;
      case 4:
          unit = minxunit;
          break;
      case 5:
          if(direction[4]>direction[6])
              unit = minxunit;
          else
              unit = maxyunit;
          break;
      case 6:
          unit = maxyunit;
          break;
      case 7:
          if(direction[6]>direction[0])
              unit = maxyunit;
          else
              unit = maxxunit;
          break;
      default:
          assert(false);
          break;
    }
    
    if(unit != 0)
        WorldViewInterface::setCameraPosition(unit->unit_state.location);
}

void
WorldInputCmdProcessor::selectAllUnitsOnScreen()
{
    iRect wr;
    WorldViewInterface::getViewWindow(&wr);

    bool addunits = false;
    if(    (KeyboardInterface::getKeyState(SDLK_LSHIFT) == true)
        || (KeyboardInterface::getKeyState(SDLK_RSHIFT) == true))
    {
        addunits = true;
    }

    working_list.selectBounded(wr, addunits);
}
