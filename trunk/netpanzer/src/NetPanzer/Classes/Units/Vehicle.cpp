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

#include <string.h>
#include "Log.hpp"
#include "Vehicle.hpp"
#include "MapInterface.hpp"
#include "PathScheduler.hpp"
#include "ProjectileInterface.hpp"
#include "NetworkState.hpp"
#include "Sound.hpp"

// NOTE: Temp unit new sprites put in
#include "GameConfig.hpp"

#include "codewiz.hpp"
#include "ConsoleInterface.hpp"
#include "ParticleInterface.hpp"

enum{ _rotate_and_move, _rotate_stop_move };

Vehicle::Vehicle(iXY initial_loc)
    : path(1000)
{
    smolderWait    = 0.0f;
    smolderWaitMin = 0.0f;

    iXY loc;
    MapInterface::mapXYtoPointXY( initial_loc, &loc );
    unit_state.location = loc;
    markUnitLoc( initial_loc );
    fsm_timer.changeRate( 10 );
    unit_state_timer.changeRate( 10 );
    setAiFsmDefendHold();
    reload_counter = 0;
    critical_ai_section = false;
    ai_fsm_transition_complete = false;
    pending_AI_comm = false;
    unit_state.lifecycle_state = _UNIT_LIFECYCLE_ACTIVE;
    external_ai_event = _external_event_null;
    memset( fsm_active_list, 0, sizeof( bool ) * 7 );

    opcode_queue.initialize( 25 );
    move_opcode_queue.initialize( 25 );

    in_sync_flag = true;

    body_anim_shadow.setDrawModeBlend(&Palette::colorTableDarkenALot);
    turret_anim_shadow.setDrawModeBlend(&Palette::colorTableDarkenALot);

}

void Vehicle::updateUnitStateProperties( void )
{
    if ( reload_counter < unit_state.reload_time )
        reload_counter++;

    if ( (unit_state.lifecycle_state == _UNIT_LIFECYCLE_PENDING_DESTRUCT) &&
            (ai_command_state == _ai_command_idle)
       ) {
        unit_state.lifecycle_state = _UNIT_LIFECYCLE_DESTROYED;
        death_counter = 0;
    }

    if ( unit_state.lifecycle_state == _UNIT_LIFECYCLE_DESTROYED ) {
        if ( death_counter == 5 ) {
            unit_state.lifecycle_state = _UNIT_LIFECYCLE_INACTIVE;

            DestructUnitOpcode destruct_opcode;
            destruct_opcode.unit_index = unit_id.getIndex();
            destruct_opcode.player_index = unit_id.getPlayer();
            sendOpcode( &destruct_opcode );

        } else
            death_counter++;
    }
}



void Vehicle::orientationToOffset( unsigned short orientation, signed char *offset_x, signed char *offset_y )
{
    switch ( orientation ) {
    case 0: {
            *offset_x =  1; *offset_y =  0;
        }
        break;
    case 1: {
            *offset_x =  1; *offset_y = -1;
        }
        break;
    case 2: {
            *offset_x =  0; *offset_y = -1;
        }
        break;
    case 3: {
            *offset_x = -1; *offset_y = -1;
        }
        break;
    case 4: {
            *offset_x = -1; *offset_y =  0;
        }
        break;
    case 5: {
            *offset_x = -1; *offset_y =  1;
        }
        break;
    case 6: {
            *offset_x =  0; *offset_y =  1;
        }
        break;
    case 7: {
            *offset_x =  1; *offset_y =  1;
        }
        break;
    }

}


unsigned short Vehicle::mapXYtoOrientation( unsigned long square, long *goal_angle )
{
    iXY current_loc, next_loc;

    MapInterface::pointXYtoMapXY( unit_state.location, &current_loc );
    MapInterface::offsetToMapXY( square, &next_loc );

    if ( (next_loc.x > current_loc.x)  &&  (next_loc.y == current_loc.y) ) {
        *goal_angle = 0;
        return( 0 );
    }

    if ( (next_loc.x > current_loc.x)  &&  (next_loc.y < current_loc.y) ) {
        *goal_angle = 5;
        return( 1 );
    }

    if ( (next_loc.x == current_loc.x)  &&  (next_loc.y < current_loc.y) ) {
        *goal_angle = 9;
        return( 2 );
    }

    if ( (next_loc.x < current_loc.x)  &&  (next_loc.y < current_loc.y)   ) {
        *goal_angle = 14;
        return( 3 );
    }

    if ( (next_loc.x < current_loc.x)  &&  (next_loc.y  == current_loc.y)  ) {
        *goal_angle = 18;
        return( 4 );
    }

    if ( (next_loc.x < current_loc.x)  &&  (next_loc.y > current_loc.y) ) {
        *goal_angle = 23;
        return( 5 );
    }

    if ( (next_loc.x == current_loc.x)  &&  (next_loc.y > current_loc.y) ) {
        *goal_angle = 27;
        return( 6 );
    }

    if ( (next_loc.x > current_loc.x)  &&  (next_loc.y > current_loc.y) ) {
        *goal_angle = 32;
        return( 7 );
    }

    return( 0xFFFF );


}

unsigned short Vehicle::shortestRotation( AngleInt &angle, long goal_angle, long *delta )
{
    long delta_plus, delta_minus;

    if( goal_angle > angle.angle_int )
        delta_plus  = ( goal_angle ) - (angle.angle_int) ;
    else
        delta_plus  = (angle.angle_limit + goal_angle) - angle.angle_int;

    if ( goal_angle > angle.angle_int )
        delta_minus = angle.angle_limit - (goal_angle  -  angle.angle_int) ;
    else
        delta_minus = (angle.angle_int ) - ( goal_angle );

    if (delta_minus > delta_plus) {
        *delta = delta_plus;
        return( _rotate_pos );
    } else {
        *delta = delta_minus;
        return( _rotate_neg );
    }
}


void Vehicle::locationOffset( unsigned long square, iXY &offset )
{
    iXY square_map_loc;
    iXY unit_map_loc;

    MapInterface::offsetToMapXY( square, &square_map_loc );
    MapInterface::pointXYtoMapXY( unit_state.location, &unit_map_loc );

    offset = unit_map_loc - square_map_loc ;
}

void Vehicle::setFsmBodyRotate( long goal_angle, unsigned short rotation )
{
    fsmBodyRotate_rotation = rotation;
    fsmBodyRotate_goal_angle = goal_angle;
}

bool Vehicle::fsmBodyRotate( void )
{
    if( unit_state.body_angle.angle_int != fsmBodyRotate_goal_angle ) {
        if ( fsmBodyRotate_rotation == _rotate_pos ) {
            ++unit_state.body_angle;
        } else {
            --unit_state.body_angle;
        }  // ** else turn_rotation **
    } else {
        return( true );   // goal direction achieved
    }

    return( false );
}

void Vehicle::setFsmTurretRotate( long goal_angle, unsigned short rotation )
{
    fsmTurretRotate_rotation = rotation;
    fsmTurretRotate_goal_angle = goal_angle;
}

bool Vehicle::fsmTurretRotate( void )
{
    if( unit_state.turret_angle.angle_int != fsmTurretRotate_goal_angle ) {
        if ( fsmTurretRotate_rotation == _rotate_pos ) {
            ++unit_state.turret_angle;
        } else {
            --unit_state.turret_angle;
        }  // ** else turn_rotation **
    } else {
        return( true );   // goal direction achieved
    }

    return( false );
}


void Vehicle::setFsmMove( unsigned short orientation )
{
    fsm_timer.changeRate( unit_state.speed_rate );
    orientationToOffset( orientation, &fsmMove_offset_x, &fsmMove_offset_y );
    fsmMove_moves_counter = 0;
    fsmMove_moves_per_square = 32 / unit_state.speed_factor;
    /*
    fsmMove_moves_per_square = 32;
    interpolation_speed = unit_state.speed_rate * unit_state.speed_factor;
    if ( NetworkState::status == _network_state_client )
     { interpolation_speed += 2; } 
    fsmMove_first_stamp = true;
    */
}

bool Vehicle::fsmMove( void )
{
    /*
    long move_offset;

    if( fsmMove_first_stamp == true )
       {
        start_move_stamp = now();
        fsmMove_first_stamp = false;
       }

    end_move_stamp = now();

    move_offset = (end_move_stamp - start_move_stamp) * interpolation_speed;  

    if( (move_offset + fsmMove_moves_counter) > fsmMove_moves_per_square )
     {
      move_offset = fsmMove_moves_per_square - fsmMove_moves_counter;
     }
    */

    if ( fsmMove_moves_counter < fsmMove_moves_per_square ) {
        /*
        fsmMove_moves_counter += move_offset;

        unit_state.location.x = unit_state.location.x + ( move_offset * fsmMove_offset_x );

        unit_state.location.y = unit_state.location.y + ( move_offset * fsmMove_offset_y );
        */

        fsmMove_moves_counter++;

        unit_state.location.x = unit_state.location.x + ( unit_state.speed_factor * fsmMove_offset_x );

        unit_state.location.y = unit_state.location.y + ( unit_state.speed_factor * fsmMove_offset_y );

        ParticleInterface::addMoveDirtPuff(unit_state);
        //start_move_stamp = now();
    }

    if( fsmMove_moves_counter >= fsmMove_moves_per_square) {
        //fsmMove_first_stamp = true;
        fsm_timer.changeRate( 10 );
        return( true );
    }

    return( false );
}

void Vehicle::setFsmMoveMapSquare( unsigned long square )
{
    long goal_angle;
    long delta;
    unsigned short rotation;

    unit_state.orientation = mapXYtoOrientation( square, &goal_angle );
    rotation = shortestRotation( unit_state.body_angle, goal_angle, &delta );

    setFsmBodyRotate( goal_angle, rotation );
    setFsmMove( unit_state.orientation );

    if ( delta <= 9 )
        fsmMoveMapSquare_movement_type = _rotate_and_move;
    else
        fsmMoveMapSquare_movement_type = _rotate_stop_move;

    fsm_active_list[ _control_move_map_square ] = true;
    critical_ai_section = true;

    if ( NetworkState::status == _network_state_server ) {
        iXY loc_offset;

        move_opcode.opcode = _UNIT_OPCODE_MOVE;
        move_opcode.unit_index = unit_id.getIndex();
        move_opcode.player_index = unit_id.getPlayer();
        move_opcode.square = square;
        locationOffset( square, loc_offset );
        move_opcode.loc_x_offset = (signed char) loc_offset.x;
        move_opcode.loc_y_offset = (signed char) loc_offset.y;
        if ( move_opcode_sent == true )
            sendOpcodeNG( &move_opcode );

    }

}




bool Vehicle::fsmMoveMapSquare( void )
{
    if ( move_opcode_sent == false && NetworkState::status == _network_state_server )
        if ( opcode_move_timer.count() ) {
            sendOpcodeNG( &move_opcode );
            move_opcode_sent = true;
        }

    switch( fsmMoveMapSquare_movement_type ) {
    case  _rotate_stop_move : {
            if ( fsmBodyRotate() )
                if ( fsmMove() ) {
                    fsm_active_list[ _control_move_map_square ] = false;
                    critical_ai_section = false;

                    if ( move_opcode_sent == false && NetworkState::status == _network_state_server ) {
                        sendOpcodeNG( &move_opcode );
                        move_opcode_sent = true;
                    }


                    return( true );
                }
        }
        break;

    case _rotate_and_move : {
            fsmBodyRotate();
            if ( fsmMove() ) {
                fsm_active_list[ _control_move_map_square ] = false;
                critical_ai_section = false;

                if ( move_opcode_sent == false && NetworkState::status == _network_state_server ) {
                    sendOpcodeNG( &move_opcode );
                    move_opcode_sent = true;
                }


                return( true );
            }
        }
        break;
    } // ** switch

    return( false );
}


void Vehicle::setFsmTurretTrackPoint( iXY &target )
{
    iXY direction_vector;

    fsmTurretTrackPoint_target = target;
    direction_vector = fsmTurretTrackPoint_target - unit_state.location;
    fsmTurretTrackPoint_target_angle.set( direction_vector );

    fsmTurretTrackPoint_on_target = false;

    fsm_active_list[ _control_turret_track_point ] = true;

    if ( NetworkState::status == _network_state_server ) {
        TurretTrackPointOpcode track_point_opcode;
        track_point_opcode.opcode = _UNIT_OPCODE_TURRET_TRACK_POINT;
        track_point_opcode.unit_index = unit_id.getIndex();
        track_point_opcode.player_index = unit_id.getPlayer();
        track_point_opcode.x = (unsigned short) target.x;
        track_point_opcode.y = (unsigned short) target.y;
        track_point_opcode.activate = true;
        sendOpcode( &track_point_opcode );
    }

}

void Vehicle::clearFsmTurretTrackPoint( void )
{
    fsm_active_list[ _control_turret_track_point ] = false;

    fsmTurretTrackPoint_on_target = false;

    if ( NetworkState::status == _network_state_server ) {
        TurretTrackPointOpcode track_point_opcode;
        track_point_opcode.opcode = _UNIT_OPCODE_TURRET_TRACK_POINT;
        track_point_opcode.unit_index = unit_id.getIndex();
        track_point_opcode.player_index = unit_id.getPlayer();
        track_point_opcode.activate = false;
        sendOpcode( &track_point_opcode );
    }

}

void Vehicle::syncFsmTurretTrackPoint( void )
{
    if ( fsm_active_list[ _control_turret_track_point ] == true ) {
        TurretTrackPointOpcode track_point_opcode;
        track_point_opcode.flags = _unit_opcode_flag_sync;
        track_point_opcode.opcode = _UNIT_OPCODE_TURRET_TRACK_POINT;
        track_point_opcode.unit_index = unit_id.getIndex();
        track_point_opcode.player_index = unit_id.getPlayer();
        track_point_opcode.x = (unsigned short) fsmTurretTrackPoint_target.x;
        track_point_opcode.y = (unsigned short) fsmTurretTrackPoint_target.y;
        track_point_opcode.activate = true;

        sendOpcode( &track_point_opcode );
    }
}

void Vehicle::fsmTurretTrackPoint( void )
{
    long goal_angle;
    long delta;
    unsigned short rotation;
    iXY direction_vector;

    direction_vector = fsmTurretTrackPoint_target - unit_state.location;
    fsmTurretTrackPoint_target_angle.set( direction_vector );

    goal_angle = fsmTurretTrackPoint_target_angle.DegreesInt( 10 );
    rotation = shortestRotation( unit_state.turret_angle, goal_angle, &delta );
    setFsmTurretRotate( goal_angle, rotation );

    if (  fsmTurretRotate() == true )
        fsmTurretTrackPoint_on_target = true;
    else
        fsmTurretTrackPoint_on_target = false;

}

void Vehicle::setFsmTurretTrackTarget( UnitID &target_id )
{
    fsmTurretTrackTarget_target_id = target_id;
    fsm_active_list[ _control_turret_track_target ] = true;

    if ( NetworkState::status == _network_state_server ) {
        TurretTrackTargetOpcode track_target_opcode;
        track_target_opcode.opcode  = _UNIT_OPCODE_TURRET_TRACK_TARGET;
        track_target_opcode.unit_index = unit_id.getIndex();
        track_target_opcode.player_index = unit_id.getPlayer();
        track_target_opcode.target_player_index = target_id.getPlayer();
        track_target_opcode.target_unit_index = target_id.getIndex();
        track_target_opcode.activate = true;
        sendOpcode( &track_target_opcode );
    }

}

void Vehicle::clearFsmTurretTrackTarget( void )
{
    fsm_active_list[ _control_turret_track_target ] = false;

    if ( NetworkState::status == _network_state_server ) {
        TurretTrackTargetOpcode track_target_opcode;
        track_target_opcode.opcode  = _UNIT_OPCODE_TURRET_TRACK_TARGET;
        track_target_opcode.unit_index = unit_id.getIndex();
        track_target_opcode.player_index = unit_id.getPlayer();
        track_target_opcode.activate = false;
        sendOpcode( &track_target_opcode );
    }

}

void Vehicle::syncFsmTurretTrackTarget( void )
{
    if ( fsm_active_list[ _control_turret_track_target ] == true ) {
        TurretTrackTargetOpcode track_target_opcode;
        track_target_opcode.opcode  = _UNIT_OPCODE_TURRET_TRACK_TARGET;
        track_target_opcode.flags = _unit_opcode_flag_sync;
        track_target_opcode.unit_index = unit_id.getIndex();
        track_target_opcode.player_index = unit_id.getPlayer();
        track_target_opcode.target_player_index = fsmTurretTrackTarget_target_id.getPlayer();
        track_target_opcode.target_unit_index = fsmTurretTrackTarget_target_id.getIndex();
        track_target_opcode.activate = true;
        sendOpcode( &track_target_opcode );
    }

}


void Vehicle::fsmTurretTrackTarget( void )
{
    long goal_angle;
    long delta;
    unsigned short rotation;
    UnitBase *target_unit_ptr;
    iXY direction_vector;

    target_unit_ptr = getUnit( fsmTurretTrackTarget_target_id );

    if ( target_unit_ptr != 0 ) {
        direction_vector = target_unit_ptr->unit_state.location - unit_state.location;
        Angle float_angle( direction_vector );

        goal_angle = float_angle.DegreesInt( 10 );
        rotation = shortestRotation( unit_state.turret_angle, goal_angle, &delta );
        setFsmTurretRotate( goal_angle, rotation );
    }

    if ( fsmTurretRotate() == true ) {
        fsmTurretTrackTarget_on_target = true;
    } else {
        fsmTurretTrackTarget_on_target = false;
    }
}



void Vehicle::setFsmGunneryLocation( iXY &target )
{
    if ( fsm_active_list[ _control_gunnery_target ] == true ) {
        clearFsmGunneryTarget();
    }

    fsmGunneryLocation_target = target;
    setFsmTurretTrackPoint( target );
    fsm_active_list[ _control_gunnery_location ] = true;
}

void Vehicle::clearFsmGunneryLocation( void )
{
    fsm_active_list[ _control_gunnery_location ] = false;
    clearFsmTurretTrackPoint();
}

void Vehicle::fsmGunneryLocation( void )
{
    iXY range_vector;

    range_vector = fsmGunneryLocation_target - unit_state.location;

    if ( (range_vector.mag2() < unit_state.weapon_range) &&
            (fsmTurretTrackPoint_on_target == true) &&
            (reload_counter >= unit_state.reload_time)
       ) {
        fireWeapon( fsmGunneryLocation_target );
        clearFsmGunneryLocation();
    }

}

void Vehicle::setFsmGunneryTarget( UnitID &target_id )
{
    if ( fsm_active_list[ _control_gunnery_location ] == true ) {
        clearFsmGunneryLocation();
    }

    if ( fsm_active_list[ _control_turret_track_point ] == true ) {
        clearFsmTurretTrackPoint();
    }

    fsmGunneryTarget_target_id = target_id;
    setFsmTurretTrackTarget( target_id );
    fsm_active_list[ _control_gunnery_target ] = true;
}

void Vehicle::clearFsmGunneryTarget( void )
{
    fsm_active_list[ _control_gunnery_target ] = false;
    clearFsmTurretTrackTarget();
}

void Vehicle::fsmGunneryTarget( void )
{
    UnitBase *target_unit_ptr;
    UnitState *target_unit_state;
    iXY range_vector;

    target_unit_ptr = getUnit( fsmGunneryTarget_target_id );
    if ( target_unit_ptr == 0 ) {
        clearFsmGunneryTarget();
    } else {
        target_unit_state = &(target_unit_ptr->unit_state);

        if ( target_unit_state->lifecycle_state == _UNIT_LIFECYCLE_DESTROYED ) {
            clearFsmGunneryTarget();
        } // ** if
        else {
            range_vector = target_unit_state->location - unit_state.location;

            if ( (range_vector.mag2() < unit_state.weapon_range) &&
                    (fsmTurretTrackTarget_on_target == true) &&
                    (reload_counter == unit_state.reload_time )
               ) {
                fireWeapon( target_unit_state->location );
            }

        }

    }


}

void Vehicle::aiFsmIdle( void )
{
    if ( pending_AI_comm == true ) {
        ai_fsm_transition_complete = true;
    }

}

bool Vehicle::ruleMoveToLoc_GoalReached( void )
{
    iXY map_loc;
    MapInterface::pointXYtoMapXY( unit_state.location, &map_loc );
    if ( map_loc == aiFsmMoveToLoc_goal )
        return( true );

    return( false );
}


void Vehicle::aiFsmMoveToLoc( void )
{
    bool end_cycle = false;

    do {
        switch ( aiFsmMoveToLoc_state ) {

            // *************************************************************
        case _aiFsmMoveToLoc_path_generate : {
                // QueryPath: Has a path been generated for unit ?
                path_generated = PathScheduler::queryPath( unit_id );


                if ( external_ai_event == _external_event_pending_unit_destruct  ) {
                    // External Event: This unit is about to be deleted
                    // Action : Exit fsm gracefully
                    aiFsmMoveToLoc_OnExitCleanUp();

                    iXY current_map_loc;
                    MapInterface::pointXYtoMapXY( unit_state.location, &current_map_loc );
                    unmarkUnitLoc( current_map_loc );

                    external_ai_event = _external_event_null;
                    ai_command_state = _ai_command_idle;
                    end_cycle = true;
                } else
                    if ( pending_AI_comm == true ) {
                        // External Event: A new AI command is pending
                        // Action: Allow command transition to occur
                        ai_fsm_transition_complete = true;
                        aiFsmMoveToLoc_OnExitCleanUp();
                        end_cycle = true;
                    } else
                        if ( path_generated == true ) {
                            // Rule QueryPath: is true move to next state
                            //LOG( ("Path Successfully Generated") );
                            aiFsmMoveToLoc_state = _aiFsmMoveToLoc_check_goal;
                        } else
                            end_cycle = true;

            }
            break;

            // *************************************************************

        case _aiFsmMoveToLoc_check_goal : {

                //  GoalReached: Has the goal been reached ?
                if ( ruleMoveToLoc_GoalReached() == true ) {
                    // Rule GoalReached : is true
                    // Action : Exit fsm
                    aiFsmMoveToLoc_prev_loc = unit_state.location;
                    MapInterface::pointXYtoMapXY( aiFsmMoveToLoc_prev_loc, &aiFsmMoveToLoc_prev_loc );
                    markUnitLoc( aiFsmMoveToLoc_prev_loc );

                    aiFsmMoveToLoc_OnExitCleanUp();
                    setAiFsmDefendHold();

                    end_cycle = true;
                } else {
                    if ( aiFsmMoveToLoc_path_not_finished == false ) {
                        //  Rule: GoalReached is false AND Unit is at the end of path
                        //  Action : Request path generation to goal
                        iXY start;
                        PathRequest path_request;

                        //LOG( ("Incomplete Path -- Regenerating Path") );
                        MapInterface::pointXYtoMapXY( unit_state.location, &start );
                        path_request.set( unit_id, start, aiFsmMoveToLoc_goal, 0,  &path, _path_request_full );
                        PathScheduler::requestPath( path_request );
                        aiFsmMoveToLoc_path_not_finished = true;
                        aiFsmMoveToLoc_state = _aiFsmMoveToLoc_path_generate;
                    } else {
                        // Rule GoalReached: is false
                        // Action: Get next move;
                        aiFsmMoveToLoc_state = _aiFsmMoveToLoc_next_move;
                    }
                }
            }
            break;

            // *************************************************************

        case _aiFsmMoveToLoc_next_move : {

                // CurrentPathComplete: is Unit at the end of the current path
                aiFsmMoveToLoc_path_not_finished = path.popFirst( &aiFsmMoveToLoc_next_square );
                MapInterface::offsetToMapXY( aiFsmMoveToLoc_next_square, &aiFsmMoveToLoc_next_loc );


                if ( !aiFsmMoveToLoc_path_not_finished ) {
                    // Rule: CurrentPathComplete is true
                    // Action : check if unit is at the goal
                    aiFsmMoveToLoc_state = _aiFsmMoveToLoc_check_goal;
                } else {
                    // Rule: CurrentPathComplete is false
                    // Action: Check if next location is empty
                    aiFsmMoveToLoc_prev_loc = unit_state.location;
                    MapInterface::pointXYtoMapXY( aiFsmMoveToLoc_prev_loc, &aiFsmMoveToLoc_prev_loc );
                    markUnitLoc( aiFsmMoveToLoc_prev_loc );

                    aiFsmMoveToLoc_wait_timer.changePeriod( 0.8f );
                    aiFsmMoveToLoc_state = _aiFsmMoveToLoc_wait_clear_loc;
                }

            }
            break;

            // *************************************************************

        case _aiFsmMoveToLoc_wait_clear_loc : {

                if ( external_ai_event == _external_event_pending_unit_destruct  ) {
                    // External Event: This unit is about to be deleted
                    // Action : Exit fsm gracefully
                    aiFsmMoveToLoc_OnExitCleanUp();
                    unmarkUnitLoc( aiFsmMoveToLoc_prev_loc );
                    external_ai_event = _external_event_null;
                    ai_command_state = _ai_command_idle;
                    end_cycle = true;
                } else
                    if ( pending_AI_comm == true ) {
                        // External Event: A new AI command is pending
                        // Action: Allow command transition to occur
                        ai_fsm_transition_complete = true;
                        aiFsmMoveToLoc_OnExitCleanUp();
                        end_cycle = true;
                    } else  // NextSquareOccupied: does the next square contain a abstruction
                        if ( unitOccupiesLoc( aiFsmMoveToLoc_next_loc ) == true ) {
                            // Rule: NextSquareOccupied is true
                            // Action: Check Wait Timer

                            if ( aiFsmMoveToLoc_wait_timer.count() ) {
                                // Rule: NextSquareOccupied is true AND WaitTimer is finished
                                // Action: Preform path update

                                if ( aiFsmMoveToLoc_next_loc == aiFsmMoveToLoc_goal ) {

                                    unit_placement_matrix.reset( aiFsmMoveToLoc_goal );
                                    unit_placement_matrix.getNextEmptyLoc( &aiFsmMoveToLoc_goal );

                                    PathRequest path_request;
                                    path_request.set( unit_id, aiFsmMoveToLoc_prev_loc, aiFsmMoveToLoc_goal, 0, &path, _path_request_full );
                                    PathScheduler::requestPath( path_request );
                                } else {
                                    PathRequest path_request;
                                    path_request.set( unit_id, aiFsmMoveToLoc_prev_loc, aiFsmMoveToLoc_goal, 0, &path, _path_request_update );
                                    PathScheduler::requestPath( path_request );
                                }

                                aiFsmMoveToLoc_state = _aiFsmMoveToLoc_path_generate;
                            }

                            end_cycle = true;
                        } else {
                            // Rule: NextSquareOccupied is false
                            // Action: Begin move to next square
                            markUnitLoc( aiFsmMoveToLoc_next_loc );
                            setFsmMoveMapSquare( aiFsmMoveToLoc_next_square );

                            aiFsmMoveToLoc_state = _aiFsmMoveToLoc_move_wait;
                        }

            }
            break;

            // *************************************************************

        case _aiFsmMoveToLoc_move_wait : {
                // MoveFinished : has the low level fsm finshed
                if ( fsm_active_list[ _control_move_map_square ] == false ) {
                    // Rule: MoveFinished is true
                    // Action: Check for a pending transition
                    unmarkUnitLoc( aiFsmMoveToLoc_prev_loc );
                    aiFsmMoveToLoc_state = _aiFsmMoveToLoc_check_fsm_transition;
                    end_cycle = true;
                } else {
                    end_cycle = true;
                } // ** else

            }
            break;

            // *************************************************************

        case _aiFsmMoveToLoc_check_fsm_transition : {
                if ( external_ai_event == _external_event_pending_unit_destruct  ) {
                    // External Event: This unit is about to be deleted
                    // Action : Exit fsm gracefully
                    aiFsmMoveToLoc_OnExitCleanUp();
                    unmarkUnitLoc( aiFsmMoveToLoc_next_loc );
                    external_ai_event = _external_event_null;
                    ai_command_state = _ai_command_idle;
                    end_cycle = true;
                } else
                    if ( pending_AI_comm == true ) {
                        // External Event: A new AI command is pending
                        // Action: Allow command transition to occur
                        ai_fsm_transition_complete = true;
                        aiFsmMoveToLoc_OnExitCleanUp();
                        end_cycle = true;
                    } else {
                        aiFsmMoveToLoc_state = _aiFsmMoveToLoc_check_goal;
                    }

            }
            break;


        } // ** switch

    } while ( end_cycle == false );

}

void Vehicle::aiFsmMoveToLoc_OnExitCleanUp( void )
{
    PathScheduler::killRequest( unit_id );
    clearFsmTurretTrackPoint();
}

void Vehicle::aiFsmAttackUnit( void )
{
    bool end_cycle = false;

    UnitBase *target_unit_ptr;
    UnitState *target_unit_state = 0;
    iXY range_vector;

    target_unit_ptr = getUnit( aiFsmAttackUnit_target_ID );
    if ( target_unit_ptr == 0 ) {
        aiFsmAttackUnit_target_destroyed = true;
    } else {
        target_unit_state = &(target_unit_ptr->unit_state);

        if ( target_unit_state->lifecycle_state == _UNIT_LIFECYCLE_DESTROYED ) {
            aiFsmAttackUnit_target_destroyed = true;

            if ( aiFsmAttackUnit_state !=  _aiFsmAttackUnit_move_wait ) {
                setAiFsmDefendHold();
                aiFsmAttackUnit_OnExitCleanUp();
                return;
            } // ** if

        } // ** if
    } // ** if

    do {
        switch ( aiFsmAttackUnit_state ) {
            // *************************************************************

        case _aiFsmAttackUnit_path_generate : {
                // QueryPath: Has a path been generated for unit ?
                path_generated = PathScheduler::queryPath( unit_id );

                if ( external_ai_event == _external_event_pending_unit_destruct  ) {
                    // External Event: This unit is about to be deleted
                    // Action : Exit fsm gracefully
                    aiFsmAttackUnit_OnExitCleanUp();

                    iXY current_map_loc;
                    MapInterface::pointXYtoMapXY( unit_state.location, &current_map_loc );
                    unmarkUnitLoc( current_map_loc );

                    external_ai_event = _external_event_null;
                    ai_command_state = _ai_command_idle;
                    end_cycle = true;
                } else
                    if ( pending_AI_comm == true ) {
                        // External Event: A new AI command is pending
                        // Action: Allow command transition to occur
                        ai_fsm_transition_complete = true;
                        aiFsmAttackUnit_OnExitCleanUp();
                        end_cycle = true;
                    } else
                        if ( path_generated == true ) {
                            // Rule QueryPath: is true move to next state
                            aiFsmAttackUnit_state = _aiFsmAttackUnit_range_check;
                        } else
                            end_cycle = true;

            }
            break;
            // *************************************************************

        case _aiFsmAttackUnit_range_check : {

                // RangeVector: the absolute distance between unit and target
                range_vector = target_unit_state->location - unit_state.location;

                if ( range_vector.mag2() < unit_state.weapon_range ) {
                    // Rule: RangeVector < WeaponRange, unit is in range
                    // Action: Remain in position
                    aiFsmAttackUnit_prev_loc = unit_state.location;
                    MapInterface::pointXYtoMapXY( aiFsmAttackUnit_prev_loc, &aiFsmAttackUnit_prev_loc );
                    markUnitLoc( aiFsmAttackUnit_prev_loc );
                    aiFsmAttackUnit_state = _aiFsmAttackUnit_idle;
                    end_cycle = true;
                } else {
                    if ( aiFsmAttackUnit_path_not_finished == false ) {
                        iXY start;
                        MapInterface::pointXYtoMapXY( unit_state.location, &start );

                        PathRequest path_request;
                        path_request.set( unit_id, start, aiFsmAttackUnit_target_goal_loc, 0, &path, _path_request_full );
                        PathScheduler::requestPath( path_request );

                        aiFsmAttackUnit_path_not_finished = true;
                        aiFsmAttackUnit_state = _aiFsmAttackUnit_path_generate;
                    } else {
                        // Rule: RangeVector > WeaponRange
                        // Action: Get next move
                        aiFsmAttackUnit_state = _aiFsmAttackUnit_next_move;
                    }
                }
            }
            break;
            // *************************************************************

        case _aiFsmAttackUnit_idle : {
                range_vector = target_unit_state->location - unit_state.location;

                if ( external_ai_event == _external_event_pending_unit_destruct  ) {
                    // External Event: This unit is about to be deleted
                    // Action : Exit fsm gracefully
                    aiFsmAttackUnit_OnExitCleanUp();
                    unmarkUnitLoc( aiFsmAttackUnit_prev_loc );
                    external_ai_event = _external_event_null;
                    ai_command_state = _ai_command_idle;
                    end_cycle = true;
                } else
                    if ( pending_AI_comm == true ) {
                        // External Event: A new AI command is pending
                        // Action: Allow command transition to occur
                        ai_fsm_transition_complete = true;
                        aiFsmAttackUnit_OnExitCleanUp();
                        end_cycle = true;
                    } else
                        if ( range_vector.mag2() < unit_state.weapon_range ) {
                            // Rule: RangeVector < WeaponRange, unit is in range
                            // Action: Remain in position
                            end_cycle = true;
                        } else {
                            // Rule: RangeVector > WeaponRange
                            // Action: Get next move
                            aiFsmAttackUnit_state = _aiFsmAttackUnit_next_move;
                        }

            }
            break;

            // *************************************************************

        case _aiFsmAttackUnit_next_move : {
                // CurrentPathComplete: is Unit at the end of the current path
                aiFsmAttackUnit_path_not_finished = path.popFirst( &aiFsmAttackUnit_next_square );
                MapInterface::offsetToMapXY( aiFsmAttackUnit_next_square, &aiFsmAttackUnit_next_loc );

                if ( !aiFsmAttackUnit_path_not_finished ) {
                    // Rule: CurrentPathComplete is true
                    // Action : check if unit is at the goal
                    aiFsmAttackUnit_state = _aiFsmAttackUnit_range_check;
                } else {
                    // Rule: CurrentPathComplete is false
                    // Action: Check if next location is empty
                    aiFsmAttackUnit_prev_loc = unit_state.location;
                    MapInterface::pointXYtoMapXY( aiFsmAttackUnit_prev_loc, &aiFsmAttackUnit_prev_loc );
                    markUnitLoc( aiFsmAttackUnit_prev_loc );
                    aiFsmAttackUnit_wait_timer.changePeriod( 0.8f );
                    aiFsmAttackUnit_state = _aiFsmAttackUnit_wait_clear_loc;
                }

            }
            break;

            // *************************************************************

        case _aiFsmAttackUnit_wait_clear_loc : {
                if ( external_ai_event == _external_event_pending_unit_destruct  ) {
                    // External Event: This unit is about to be deleted
                    // Action : Exit fsm gracefully
                    aiFsmAttackUnit_OnExitCleanUp();
                    unmarkUnitLoc( aiFsmAttackUnit_prev_loc );
                    external_ai_event = _external_event_null;
                    ai_command_state = _ai_command_idle;
                    end_cycle = true;
                } else
                    if ( pending_AI_comm == true ) {
                        // External Event: A new AI command is pending
                        // Action: Allow command transition to occur
                        ai_fsm_transition_complete = true;
                        aiFsmAttackUnit_OnExitCleanUp();
                        end_cycle = true;
                    } else
                        if ( unitOccupiesLoc( aiFsmAttackUnit_next_loc ) == true ) {
                            if ( aiFsmAttackUnit_wait_timer.count() ) {

                                if ( aiFsmAttackUnit_next_loc == aiFsmAttackUnit_target_goal_loc ) {
                                    unit_placement_matrix.reset( aiFsmAttackUnit_target_goal_loc );
                                    unit_placement_matrix.getNextEmptyLoc( &aiFsmAttackUnit_target_goal_loc );

                                    PathRequest path_request;
                                    path_request.set( unit_id, aiFsmAttackUnit_prev_loc, aiFsmAttackUnit_target_goal_loc, 0, &path, _path_request_full );
                                    PathScheduler::requestPath( path_request );

                                } else {
                                    PathRequest path_request;
                                    path_request.set( unit_id, aiFsmAttackUnit_prev_loc, aiFsmAttackUnit_target_goal_loc, 0, &path, _path_request_update );
                                    PathScheduler::requestPath( path_request );
                                }


                                aiFsmAttackUnit_state = _aiFsmAttackUnit_path_generate;
                            }

                            end_cycle = true;
                        } else {
                            markUnitLoc( aiFsmAttackUnit_next_loc );
                            setFsmMoveMapSquare( aiFsmAttackUnit_next_square );
                            aiFsmAttackUnit_state = _aiFsmAttackUnit_move_wait;
                        }

            }
            break;

            // *************************************************************

        case _aiFsmAttackUnit_move_wait : {
                if ( fsm_active_list[ _control_move_map_square ] == false ) {
                    unmarkUnitLoc( aiFsmAttackUnit_prev_loc );
                    aiFsmAttackUnit_state = _aiFsmAttackUnit_check_fsm_transition;
                    end_cycle = true;
                } else {
                    end_cycle = true;
                } // ** else

            }
            break;

            // *************************************************************

        case _aiFsmAttackUnit_check_fsm_transition : {
                if ( external_ai_event == _external_event_pending_unit_destruct  ) {
                    // External Event: This unit is about to be deleted
                    // Action : Exit fsm gracefully
                    aiFsmAttackUnit_OnExitCleanUp();
                    unmarkUnitLoc( aiFsmAttackUnit_next_loc );
                    external_ai_event = _external_event_null;
                    ai_command_state = _ai_command_idle;
                    end_cycle = true;
                } else
                    if ( pending_AI_comm == true ) {
                        ai_fsm_transition_complete = true;
                        aiFsmAttackUnit_OnExitCleanUp();
                        end_cycle = true;
                    } else
                        if (  aiFsmAttackUnit_target_destroyed == true ) {
                            setAiFsmDefendHold();
                            aiFsmAttackUnit_OnExitCleanUp();
                            end_cycle = true;
                        } else {

                            aiFsmAttackUnit_state = _aiFsmAttackUnit_check_path_deviation;
                        }

            }
            break;
            // *************************************************************

        case _aiFsmAttackUnit_check_path_deviation : {
                iXY deviation_vector;
                iXY goal_point_loc;

                MapInterface::mapXYtoPointXY( aiFsmAttackUnit_target_goal_loc, &goal_point_loc );
                deviation_vector = target_unit_state->location - goal_point_loc;

                if ( deviation_vector.mag2() > unit_state.weapon_range ) {
                    MapInterface::pointXYtoMapXY( target_unit_state->location, &aiFsmAttackUnit_target_goal_loc );

                    PathRequest path_request;
                    path_request.set( unit_id, aiFsmAttackUnit_next_loc, aiFsmAttackUnit_target_goal_loc, 0, &path, _path_request_full );
                    PathScheduler::requestPath( path_request );

                    aiFsmAttackUnit_state = _aiFsmAttackUnit_path_generate;
                    end_cycle = true;
                } else {
                    aiFsmAttackUnit_state = _aiFsmAttackUnit_range_check;
                }

            }
            break;

        } // ** switch

    } while ( end_cycle == false );

}

void Vehicle::aiFsmAttackUnit_OnExitCleanUp( void )
{
    PathScheduler::killRequest( unit_id );
    clearFsmGunneryTarget();
}

void Vehicle::setAiFsmDefendHold( void )
{
    ai_command_state = _ai_command_defend_hold;
    aiFsmDefendHold_state = _aiFsmDefendHold_search_for_enemy;
    aiFsmDefendHold_search_timer.changePeriod( .5 );
}

void Vehicle::aiFsmDefendHold( void )
{
    bool end_cycle = false;

    UnitBase  *target_unit_ptr;
    UnitState *target_unit_state;

    iXY range_vector;

    do {
        switch ( aiFsmDefendHold_state ) {
        case _aiFsmDefendHold_search_for_enemy : {
                if ( external_ai_event == _external_event_pending_unit_destruct  ) {
                    // External Event: This unit is about to be deleted
                    // Action : Exit fsm gracefully
                    external_ai_event = _external_event_null;
                    ai_command_state = _ai_command_idle;
                    end_cycle = true;
                } else
                    if ( pending_AI_comm == true ) {
                        // External Event: A new AI command is pending
                        // Action: Allow command transition to occur
                        ai_fsm_transition_complete = true;
                        end_cycle = true;
                    } else
                        if ( aiFsmDefendHold_search_timer.count() ) {
                            if ( quearyClosestEnemyUnit( &target_unit_ptr, unit_state.location, unit_id.getPlayer() ) ) {
                                target_unit_state = &(target_unit_ptr->unit_state);
                                range_vector = target_unit_state->location - unit_state.location;
                                if ( range_vector.mag2() <= unit_state.defend_range ) {
                                    aiFsmDefendHold_target_ID = target_unit_ptr->unit_id;
                                    setFsmGunneryTarget( aiFsmDefendHold_target_ID );
                                    aiFsmDefendHold_state = _aiFsmDefendHold_attack_enemy;
                                    end_cycle = true;
                                } else {
                                    end_cycle = true;
                                }
                            } // **  quearyClosestEnemyUnit
                            else {
                                end_cycle = true;
                            }

                        } // ** aiFsmDefendHold_search_timer.count()
                        else {
                            end_cycle = true;
                        }
            }
            break;

        case _aiFsmDefendHold_attack_enemy : {
                if ( external_ai_event == _external_event_pending_unit_destruct  ) {
                    // External Event: This unit is about to be deleted
                    // Action : Exit fsm gracefully
                    clearFsmGunneryTarget();
                    external_ai_event = _external_event_null;
                    ai_command_state = _ai_command_idle;
                    end_cycle = true;
                } else
                    if ( pending_AI_comm == true ) {
                        // External Event: A new AI command is pending
                        // Action: Allow command transition to occur
                        clearFsmGunneryTarget();
                        ai_fsm_transition_complete = true;
                        end_cycle = true;
                    } else {
                        target_unit_ptr = getUnit( aiFsmDefendHold_target_ID );
                        if ( target_unit_ptr == 0 ) {
                            clearFsmGunneryTarget();
                            aiFsmDefendHold_state = _aiFsmDefendHold_search_for_enemy;
                            end_cycle = true;
                        } else {
                            target_unit_state = &(target_unit_ptr->unit_state);
                            if ( target_unit_state->lifecycle_state == _UNIT_LIFECYCLE_DESTROYED ) {
                                clearFsmGunneryTarget();
                                aiFsmDefendHold_state = _aiFsmDefendHold_search_for_enemy;
                                end_cycle = true;
                            } // ** if
                            else {
                                range_vector = target_unit_state->location - unit_state.location;
                                if ( range_vector.mag2() > unit_state.defend_range ) {
                                    clearFsmGunneryTarget();
                                    aiFsmDefendHold_state = _aiFsmDefendHold_search_for_enemy;
                                    end_cycle = true;
                                }
                            } // else
                        } // ** else
                    } // ** else

                end_cycle = true;
            }
            break;
        } // ** switch

    } while ( end_cycle == false );

}





void Vehicle::aiFsmManualMove( void )
{
    bool end_cycle = false;
    signed char offset_x, offset_y;
    unsigned long next_square;

    do {
        switch( aiFsmManualMove_state ) {
        case _aiFsmManualMove_next_move : {
                aiFsmManualMove_prev_loc = aiFsmManualMove_next_loc;
                orientationToOffset( aiFsmManualMove_move_orientation, &offset_x, &offset_y );
                aiFsmManualMove_next_loc.x += offset_x;
                aiFsmManualMove_next_loc.y += offset_y;
                MapInterface::mapXYtoOffset( aiFsmManualMove_next_loc, &next_square );

                if ( MapInterface::getMovementValue( aiFsmManualMove_next_loc ) == 0xFF ) {
                    setAiFsmDefendHold();
                    end_cycle = true;
                } else
                    if( unitOccupiesLoc( aiFsmManualMove_next_loc ) == true ) {
                        setAiFsmDefendHold();
                        end_cycle = true;
                    } else {
                        markUnitLoc( aiFsmManualMove_next_loc );
                        setFsmMoveMapSquare( next_square );
                        aiFsmManualMove_state = _aiFsmManualMove_move_wait;
                    }
            }
            break;

            // *************************************************************

        case _aiFsmManualMove_move_wait : {
                if ( fsm_active_list[ _control_move_map_square ] == false ) {
                    unmarkUnitLoc( aiFsmManualMove_prev_loc );
                    aiFsmManualMove_state = _aiFsmManualMove_check_fsm_transition;
                    end_cycle = true;
                } else {
                    end_cycle = true;
                } // ** else
            }
            break;

            // *************************************************************

        case _aiFsmManualMove_check_fsm_transition : {
                if ( external_ai_event == _external_event_pending_unit_destruct  ) {
                    // External Event: This unit is about to be deleted
                    // Action : Exit fsm gracefully
                    unmarkUnitLoc( aiFsmManualMove_next_loc );
                    external_ai_event = _external_event_null;
                    ai_command_state = _ai_command_idle;
                    end_cycle = true;
                } else
                    if ( pending_AI_comm == true ) {
                        ai_fsm_transition_complete = true;
                        end_cycle = true;
                    } else {
                        aiFsmManualMove_state = _aiFsmManualMove_next_move;
                    }
            }
            break;

        } // ** switch
    } while ( end_cycle == false );

}

void Vehicle::fireWeapon( iXY &target_loc )
{
    reload_counter = 0;

    unsigned short projectile_type = launchProjectile();
    ProjectileInterface::newProjectile(projectile_type, unit_state.unit_type, unit_id,
                                       unit_state.damage_factor,
                                       unit_state.location,
                                       target_loc
                                      );

    if ( NetworkState::status == _network_state_server ) {
        FireWeaponOpcode fire_opcode;
        fire_opcode.opcode = _UNIT_OPCODE_FIRE_WEAPON;
        fire_opcode.unit_index = unit_id.getIndex();
        fire_opcode.player_index = unit_id.getPlayer();
        fire_opcode.x = target_loc.x;
        fire_opcode.y = target_loc.y;
        sendOpcodeNG( &fire_opcode );
    }
}

//-----------------------------------------------------------------
void Vehicle::soundSelected()
{
    sound->playSound("yessir");
}

void Vehicle::accessThreatLevels( void )
{
    if ( unit_state.threat_level == _threat_level_under_attack ) {
        if( threat_level_under_attack_timer.count() ) {
            unit_state.threat_level = _threat_level_all_clear;
        }
    }

}

void Vehicle::updateFsmState( void )
{
    if ( fsm_timer.count() ) {
        if ( fsm_active_list[ _control_move_map_square ] == true )
            fsmMoveMapSquare();

        if ( fsm_active_list[ _control_turret_track_point ] == true )
            fsmTurretTrackPoint();

        if ( fsm_active_list[ _control_turret_track_target ] == true )
            fsmTurretTrackTarget();

        if ( fsm_active_list[ _control_gunnery_location ] == true )
            fsmGunneryLocation();

        if ( fsm_active_list[ _control_gunnery_target ] == true )
            fsmGunneryTarget();
    }
}


void Vehicle::updateAIState( void )
{
    switch( ai_command_state ) {
    case _ai_command_idle :
        aiFsmIdle();
        break;

    case _ai_command_defend_hold :
        aiFsmDefendHold();
        break;

    case _ai_command_move_to_loc :
        aiFsmMoveToLoc();
        break;

    case _ai_command_attack_unit :
        aiFsmAttackUnit();
        break;

    case _ai_command_manual_move :
        aiFsmManualMove();
        break;
    }
}


void Vehicle::checkPendingAICommStatus( void )
{
    if ( (pending_AI_comm == true) &&
            (ai_fsm_transition_complete == true) ) {
        pending_AI_comm = false;
        ai_fsm_transition_complete = false;

        if ( unit_state.lifecycle_state == _UNIT_LIFECYCLE_ACTIVE ) {
            switch( pending_AI_comm_mesg.command ) {
            case _command_move_to_loc :
                setCommandMoveToLoc( &pending_AI_comm_mesg );
                break;

            case _command_attack_unit :
                setCommandAttackUnit( &pending_AI_comm_mesg );
                break;

            case _command_start_manual_move :
                setCommandManualMove( &pending_AI_comm_mesg  );
                break;

            case _command_stop_manual_move :
                setCommandManualMove( &pending_AI_comm_mesg  );
                break;
            } // ** switch

        } // ** unit_state.lifecycle_state == _UNIT_LIFECYCLE_ACTIVE

    } // ** if

}

void Vehicle::setCommandMoveToLoc( UMesgAICommand *message  )
{
    iXY start;

    if ( fsm_active_list[ _control_gunnery_location ] == true ) {
        clearFsmGunneryLocation();
    }

    aiFsmMoveToLoc_goal = message->goal_loc;
    ai_command_state = _ai_command_move_to_loc;
    aiFsmMoveToLoc_state = _aiFsmMoveToLoc_path_generate;
    aiFsmMoveToLoc_path_not_finished = true;

    opcode_move_timer.changePeriod( 0.10f );
    move_opcode_sent = false;
    //move_opcode_sent = true;

    MapInterface::pointXYtoMapXY( unit_state.location, &start );

    //LOG( ("UnitID %d, %d : Start %d, %d : Goal %d, %d", unit_id.getPlayer(), unit_id.getIndex(),
    //                                                    start.x, start.y,
    //                                                    aiFsmMoveToLoc_goal.x, aiFsmMoveToLoc_goal.y ) );

    PathRequest path_request;
    path_request.set( unit_id, start, aiFsmMoveToLoc_goal, 0, &path, _path_request_full );
    PathScheduler::requestPath( path_request );

    iXY target;
    MapInterface::mapXYtoPointXY( aiFsmMoveToLoc_goal, &target);
    setFsmTurretTrackPoint( target );
}

void Vehicle::setCommandAttackUnit( UMesgAICommand *message )
{
    iXY start;
    UnitBase *target_unit_ptr;
    UnitState *target_unit_state;

    aiFsmAttackUnit_target_ID = message->target_id;

    target_unit_ptr = getUnit( aiFsmAttackUnit_target_ID );
    if ( target_unit_ptr == 0 )
        return;

    target_unit_state = &(target_unit_ptr->unit_state);

    ai_command_state = _ai_command_attack_unit;

    aiFsmAttackUnit_state = _aiFsmAttackUnit_path_generate;
    aiFsmAttackUnit_path_not_finished = true;
    aiFsmAttackUnit_target_destroyed = false;

    MapInterface::pointXYtoMapXY( unit_state.location, &start );
    MapInterface::pointXYtoMapXY( target_unit_state->location, &aiFsmAttackUnit_target_goal_loc );

    PathRequest path_request;
    path_request.set( unit_id, start, aiFsmAttackUnit_target_goal_loc, 0, &path, _path_request_full );
    PathScheduler::requestPath( path_request );

    opcode_move_timer.changePeriod( 0.10f );
    move_opcode_sent = false;
    //move_opcode_sent = true;

    setFsmGunneryTarget( aiFsmAttackUnit_target_ID );
}

void Vehicle::setCommandManualMove( UMesgAICommand *message )
{
    if ( message->command == _command_start_manual_move ) {
        aiFsmManualMove_move_orientation = message->manual_move_orientation;
        MapInterface::pointXYtoMapXY( unit_state.location, &aiFsmManualMove_next_loc );
        aiFsmManualMove_state = _aiFsmManualMove_next_move;
        ai_command_state = _ai_command_manual_move;
    } else
        if ( message->command == _command_stop_manual_move ) {
            ai_command_state = _ai_command_idle;
        }
}

void Vehicle::setCommandManualFire( UMesgAICommand *message )
{
    setFsmGunneryLocation( message->target_loc );
}

void Vehicle::messageAICommand( UnitMessage *message )
{
    UMesgAICommand *command_mesg;

    command_mesg = (UMesgAICommand *) message;

    if ( unit_state.lifecycle_state == _UNIT_LIFECYCLE_ACTIVE ) {
        if ( command_mesg->command == _command_manual_fire ) {
            setCommandManualFire( command_mesg );
        } else {
            memmove( &pending_AI_comm_mesg, command_mesg, sizeof( UMesgAICommand) );
            pending_AI_comm = true;
        }
    }

}


void Vehicle::messageWeaponHit( UnitMessage *message )
{
    UMesgWeaponHit *weapon_hit;

    weapon_hit = (UMesgWeaponHit *) message;

    if ( unit_state.bounds( weapon_hit->hit_location ) == true ) {
        unit_state.hit_points -= weapon_hit->damage_factor;

        unit_state.threat_level = _threat_level_under_attack;
        threat_level_under_attack_timer.changePeriod( GameConfig::getAttackNotificationTime() );

        UpdateStateUnitOpcode update_state_opcode;

        update_state_opcode.player_index = unit_id.getPlayer();
        update_state_opcode.unit_index = unit_id.getIndex();
        update_state_opcode.hit_points = unit_state.hit_points;
        sendOpcode( &update_state_opcode );


        if ( unit_state.hit_points <= 0 ) {
            unit_state.lifecycle_state = _UNIT_LIFECYCLE_PENDING_DESTRUCT;
            external_ai_event = _external_event_pending_unit_destruct;

            UMesgEndLifeCycleUpdate lifecycle_update;
            lifecycle_update.set( unit_id, weapon_hit->owner_id, unit_state.unit_type );
            sendMessage( &lifecycle_update );

            // ** Note: Temp
            iXY current_map_loc;
            MapInterface::pointXYtoMapXY( unit_state.location, &current_map_loc );
            unmarkUnitLoc( current_map_loc );
        }

    }
}

void Vehicle::messageSelectBoxUpdate( UnitMessage *message )
{
    UMesgUpdateSelectBoxInfo *select_box_update;

    select_box_update = (UMesgUpdateSelectBoxInfo *) message;

    switch ( select_box_update->request_type ) {
    case _select_box_allie_visibility : {
            select_info_box.setAllieIcon( select_box_update->allie_flag_visiblity );
        }
        break;

    case _select_box_flag_visiblity : {
            select_info_box.setFlagIcon( select_box_update->flag_visiblity );
        }
        break;

    case _select_box_is_allied : {
            select_info_box.setAllieState( select_box_update->allied_state );
        }
        break;

    } // ** switch

}

void Vehicle::messageSelfDestruct( UnitMessage *message )
{
    unit_state.lifecycle_state = _UNIT_LIFECYCLE_PENDING_DESTRUCT;
    external_ai_event = _external_event_pending_unit_destruct;

    // ** Note: Temp
    iXY current_map_loc;
    MapInterface::pointXYtoMapXY( unit_state.location, &current_map_loc );
    unmarkUnitLoc( current_map_loc );
}


void Vehicle::processMessage( UnitMessage *message )
{
    if ( unit_state.lifecycle_state == _UNIT_LIFECYCLE_ACTIVE ) {
        switch( message->message_id ) {
        case _umesg_ai_command :
            messageAICommand( message );
            break;

        case _umesg_weapon_hit :
            messageWeaponHit( message );
            break;

        case _umesg_update_select_box_info :
            messageSelectBoxUpdate( message );
            break;

        case _umesg_self_destruct:
            messageSelfDestruct( message );
            break;

        } // ** switch
    }
}

void Vehicle::unitOpcodeMove( UnitOpcodeStruct *opcode )
{
    MoveOpcode *move_opcode;

    if ( ( (in_sync_flag == false) && !(opcode->flags & _unit_opcode_flag_sync) ) ||
            ( (in_sync_flag == true) && (opcode->flags & _unit_opcode_flag_sync) )
       )
        return;

    move_opcode = (MoveOpcode *) opcode;

    iXY sync_loc;
    iXY current_loc;

    MapInterface::offsetToMapXY( move_opcode->square, &sync_loc );
    sync_loc.x = sync_loc.x + move_opcode->loc_x_offset;
    sync_loc.y = sync_loc.y + move_opcode->loc_y_offset;

    MapInterface::pointXYtoMapXY( unit_state.location, &current_loc );

    if ( current_loc != sync_loc ) {
        MapInterface::mapXYtoPointXY( sync_loc, &(unit_state.location) );
    }

    setFsmMoveMapSquare( move_opcode->square );
}

void Vehicle::unitOpcodeTrackPoint( UnitOpcodeStruct *opcode )
{
    TurretTrackPointOpcode *track_point_opcode;

    if ( ( (in_sync_flag == false) && !(opcode->flags & _unit_opcode_flag_sync) ) ||
            ( (in_sync_flag == true) && (opcode->flags & _unit_opcode_flag_sync) )
       )
        return;

    track_point_opcode = (TurretTrackPointOpcode *) opcode;

    if ( track_point_opcode->activate == true ) {
        iXY target;
        target.x = track_point_opcode->x;
        target.y = track_point_opcode->y;
        setFsmTurretTrackPoint( target );
    } else {
        fsm_active_list[ _control_turret_track_point ] = false;
    }

}

void Vehicle::unitOpcodeTrackTarget( UnitOpcodeStruct *opcode )
{
    TurretTrackTargetOpcode *track_target_opcode;

    if ( ( (in_sync_flag == false) && !(opcode->flags & _unit_opcode_flag_sync) ) ||
            ( (in_sync_flag == true) && (opcode->flags & _unit_opcode_flag_sync) )
       )
        return;

    track_target_opcode = (TurretTrackTargetOpcode *) opcode;

    if ( track_target_opcode->activate == true ) {
        UnitID target_id;

        target_id.set( track_target_opcode->target_player_index,
                       track_target_opcode->target_unit_index
                     );

        setFsmTurretTrackTarget( target_id );
    } else {
        fsm_active_list[ _control_turret_track_target ] = false;
    }

}

void Vehicle::unitOpcodeFireWeapon( UnitOpcodeStruct *opcode )
{
    iXY target_loc;

    if ( ( (in_sync_flag == false) && !(opcode->flags & _unit_opcode_flag_sync) ) ||
            ( (in_sync_flag == true) && (opcode->flags & _unit_opcode_flag_sync) )
       )
        return;

    FireWeaponOpcode *fire_weapon;

    fire_weapon = (FireWeaponOpcode *) opcode;

    target_loc.x = fire_weapon->x;
    target_loc.y = fire_weapon->y;

    fireWeapon( target_loc );
}

void Vehicle::unitOpcodeSync( UnitOpcodeStruct *opcode )
{
    in_sync_flag = true;
}

void Vehicle::unitOpcodeUpdateState( UnitOpcodeStruct *opcode )
{
    UpdateStateUnitOpcode *update_state_opcode;

    update_state_opcode = (UpdateStateUnitOpcode *) opcode;

    unit_state.hit_points = update_state_opcode->hit_points;

    unit_state.threat_level = _threat_level_under_attack;
    threat_level_under_attack_timer.changePeriod( 30 );
}

void Vehicle::unitOpcodeDestruct( UnitOpcodeStruct *opcode )
{
    unit_state.lifecycle_state = _UNIT_LIFECYCLE_INACTIVE;
}

void Vehicle::syncUnit( void )
{
    syncFsmTurretTrackPoint();
    syncFsmTurretTrackTarget();

    SyncUnitOpcode sync_opcode;

    sync_opcode.opcode = _UNIT_OPCODE_SYNC_UNIT;
    sync_opcode.unit_index = unit_id.getIndex();
    sync_opcode.player_index = unit_id.getPlayer();
    sendOpcode( &sync_opcode );
}


void Vehicle::processMoveOpcodeQueue( void )
{
    UnitOpcodeStruct opcode;

    if ( move_opcode_queue.isReady() == true ) {
        if( fsm_active_list[ _control_move_map_square ] == false ) {
            if ( move_opcode_queue.itemCount() >= 3 ) {
                for( int i = 0; i < 2; i++ ) {
                    move_opcode_queue.dequeue();
                }
                //ConsoleInterface::postMessage( "Move Opcode Queue Adjusted" );
            }
            opcode = move_opcode_queue.dequeue();
            unitOpcodeMove( &opcode );
        }
    }

}

void Vehicle::processOpcodeQueue( void )
{
    UnitOpcodeStruct opcode;

    processMoveOpcodeQueue();

    if ( opcode_queue.isReady() ) {
        opcode = opcode_queue.getFirst();

        switch( opcode.opcode ) {

        case _UNIT_OPCODE_TURRET_TRACK_POINT : {
                unitOpcodeTrackPoint( &opcode );
                opcode_queue.dequeue();
            }
            break;

        case _UNIT_OPCODE_TURRET_TRACK_TARGET : {
                unitOpcodeTrackTarget( &opcode );
                opcode_queue.dequeue();
            }
            break;

        case _UNIT_OPCODE_FIRE_WEAPON : {
                unitOpcodeFireWeapon( &opcode );
                opcode_queue.dequeue();
            }
            break;

        case _UNIT_OPCODE_SYNC_UNIT : {
                unitOpcodeSync( &opcode );
                opcode_queue.dequeue();
            }
            break;

        case _UNIT_OPCODE_UPDATE_STATE : {
                unitOpcodeUpdateState( &opcode );
                opcode_queue.dequeue();
            }
            break;

        case _UNIT_OPCODE_DESTRUCT : {
                unitOpcodeDestruct( &opcode );
                opcode_queue.dequeue();
            }
            break;

        } // ** switch

    } // ** if opcode_queue.isReady()
}


void Vehicle::evalCommandOpcode( UnitOpcodeStruct *opcode )
{

    if ( opcode->opcode == _UNIT_OPCODE_MOVE ) {
        if ( move_opcode_queue.enqueue( *opcode ) == false ) {
            LOG( ("ERROR : Move Opcode Queue Overwrite, Unit %d, %d", unit_id.getPlayer(), unit_id.getIndex() ) );
        }
    } else
        if ( opcode_queue.enqueue( *opcode ) == false ) {
            LOG( ("ERROR : Opcode Queue Overwrite, Unit %d, %d", unit_id.getPlayer(), unit_id.getIndex() ) );
        }

}


void Vehicle::updateState( void )
{
    updateFsmState();

    if ( NetworkState::status == _network_state_server ) {
        if( unit_state_timer.count() )
            updateUnitStateProperties();

        updateAIState();
        checkPendingAICommStatus();
    } else {
        processOpcodeQueue();
    }

    accessThreatLevels();

    if( unit_state.hit_points < unit_state.max_hit_points ) {
        smolderWait += TimerInterface::getTimeSlice();

        int intPercent = unit_state.percentDamageInt();

        if (intPercent > 50 && (rand() % 100) < intPercent) {
            if (smolderWait > smolderWaitMin) {
                float percent = unit_state.percentDamageFloat();

                smolderWaitMin  = (float(100 - percent) / 100.0) + (float(100 - percent) / 100.0) * 0.3;

                ParticleInterface::addUnitDamagePuffParticle(unit_state);

                smolderWait = 0.0f;
            }
        }
    }
}


void Vehicle::offloadGraphics( SpriteSorter &sorter )
{
    body_anim_shadow.setWorldPos( unit_state.location);

    if ( sorter.cullSprite( body_anim_shadow ) == false ) {
        // Body
        body_anim.setWorldPos( unit_state.location );
        body_anim.setFrame( unit_state.body_angle.angle_int );

        // Turret
        turret_anim.setWorldPos( unit_state.location );
        turret_anim.setFrame( unit_state.turret_angle.angle_int );

        // Body Shadow
        body_anim_shadow.setFrame( unit_state.body_angle.angle_int );

        // Turret Shadow
        turret_anim_shadow.setWorldPos( unit_state.location );
        turret_anim_shadow.setFrame( unit_state.turret_angle.angle_int );

        select_info_box.setBoxState( unit_state.select );

        //Added layer selection to the selection box info.
        select_info_box.setAttrib( unit_state.location, GameConfig::getUnitInfoDrawLayer() );
        select_info_box.setHitPoints( unit_state.hit_points );

        sorter.forceAddSprite( &body_anim_shadow );
    }

    //sorter.addSprite( &body_anim_shadow );
}
