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
#ifndef _VEHICLE_HPP
#define _VEHICLE_HPP
#include <string>

#include "Units/UnitBase.hpp"
#include "Util/Timer.hpp"
#include "Units/UnitOpcodes.hpp"
#include "Classes/AI/PathList.hpp"

#include "Classes/Sprite.hpp"
#include "Classes/SelectionBoxSprite.hpp"

enum { _control_idle,
       _control_move,
       _control_move_map_square,
       _control_turret_track_point,
       _control_turret_track_target,
       _control_gunnery_location,
       _control_gunnery_target    };

enum { _ai_command_idle,
       _ai_command_move_to_loc,
       _ai_command_attack_unit,
       _ai_command_defend_hold };

enum { _aiFsmMoveToLoc_path_generate,
       _aiFsmMoveToLoc_check_goal,
       _aiFsmMoveToLoc_next_move,
       _aiFsmMoveToLoc_move_wait,
       _aiFsmMoveToLoc_wait_clear_loc,
       _aiFsmMoveToLoc_check_fsm_transition };

enum { _aiFsmAttackUnit_path_generate,
       _aiFsmAttackUnit_range_check,
       _aiFsmAttackUnit_idle,
       _aiFsmAttackUnit_next_move,
       _aiFsmAttackUnit_move_wait,
       _aiFsmAttackUnit_wait_clear_loc,
       _aiFsmAttackUnit_check_fsm_transition,
       _aiFsmAttackUnit_check_path_deviation };

enum { _aiFsmDefendHold_search_for_enemy,
       _aiFsmDefendHold_attack_enemy };

enum { _external_event_null,
       _external_event_pending_unit_destruct };

enum { _rotate_pos, _rotate_neg };

class Vehicle : public UnitBase
{
protected:
    SpritePacked body_anim;
    SpritePacked turret_anim;

    SpritePacked body_anim_shadow;
    SpritePacked turret_anim_shadow;

    UnitSelectionBox select_info_box;
    
    std::string soundSelect;
    std::string fireSound;
    unsigned short weaponType;

    Timer unit_state_timer;
    Timer fsm_timer;
    bool fsm_active_list[ 7 ];

    PathList path;
    bool ai_fsm_transition_complete;

    unsigned short reload_counter;
    unsigned short death_counter;
    void updateUnitStateProperties();

    unsigned char ai_command_state;
    unsigned char external_ai_event;


    unsigned short shortestRotation( AngleInt &angle, long goal_angle, long *delta );
    unsigned short mapXYtoOrientation( unsigned long square, long *goal_angle  );
    void orientationToOffset( unsigned short orientation, signed char *offset_x, signed char *offset_y );
    void locationOffset( unsigned long square, iXY &offset );

    // ** FSMs and AI FSMs
    unsigned short fsmBodyRotate_rotation;
    long           fsmBodyRotate_goal_angle;
    void setFsmBodyRotate( long goal_angle, unsigned short rotation );
    bool fsmBodyRotate();

    unsigned short fsmTurretRotate_rotation;
    long           fsmTurretRotate_goal_angle;
    void setFsmTurretRotate( long goal_angle, unsigned short rotation );
    bool fsmTurretRotate();

    signed char     fsmMove_offset_x;
    signed char     fsmMove_offset_y;
    unsigned char   fsmMove_moves_counter;
    unsigned char   fsmMove_moves_per_square;
    void setFsmMove( unsigned short orientation );
    bool fsmMove();

    MoveOpcode      move_opcode;
    Timer           opcode_move_timer;
    bool            move_opcode_sent;
    unsigned char   fsmMoveMapSquare_movement_type;
    void setFsmMoveMapSquare( unsigned long square );
    bool fsmMoveMapSquare();

    iXY fsmTurretTrackPoint_target;
    Angle fsmTurretTrackPoint_target_angle;
    bool fsmTurretTrackPoint_on_target;
    void setFsmTurretTrackPoint(const iXY& target );
    void clearFsmTurretTrackPoint();
    void syncFsmTurretTrackPoint();
    void fsmTurretTrackPoint();

    UnitID fsmTurretTrackTarget_target_id;
    bool fsmTurretTrackTarget_on_target;
    void setFsmTurretTrackTarget(UnitID target_id);
    void clearFsmTurretTrackTarget();
    void syncFsmTurretTrackTarget();
    void fsmTurretTrackTarget();

    iXY fsmGunneryLocation_target;
    void clearFsmGunneryLocation();
    void fsmGunneryLocation();

    UnitID fsmGunneryTarget_target_id;
    void setFsmGunneryTarget(UnitID target_id);
    void clearFsmGunneryTarget();
    void fsmGunneryTarget();

    void aiFsmIdle();

    unsigned char aiFsmDefendHold_state;
    Timer	  aiFsmDefendHold_search_timer;
    UnitID  aiFsmDefendHold_target_ID;
    void setAiFsmDefendHold();
    void aiFsmDefendHold();

    iXY aiFsmMoveToLoc_goal;
    unsigned char aiFsmMoveToLoc_state;
    unsigned long aiFsmMoveToLoc_next_square;
    iXY aiFsmMoveToLoc_next_loc;
    iXY aiFsmMoveToLoc_prev_loc;
    Timer	   aiFsmMoveToLoc_wait_timer;
    bool  aiFsmMoveToLoc_path_not_finished;
    bool ruleMoveToLoc_GoalReached();
    void aiFsmMoveToLoc_OnExitCleanUp();
    void aiFsmMoveToLoc();

    UnitID   aiFsmAttackUnit_target_ID;
    iXY aiFsmAttackUnit_target_goal_loc;
    unsigned char aiFsmAttackUnit_state;
    unsigned long aiFsmAttackUnit_next_square;
    iXY aiFsmAttackUnit_next_loc;
    iXY aiFsmAttackUnit_prev_loc;
    Timer	   aiFsmAttackUnit_wait_timer;
    bool  aiFsmAttackUnit_path_not_finished;
    bool  aiFsmAttackUnit_target_destroyed;
    void aiFsmAttackUnit_OnExitCleanUp();
    void aiFsmAttackUnit();

    void fireWeapon( iXY &target_loc );
    virtual unsigned short launchProjectile();
    virtual void soundSelected();

    TimerFrameBase threat_level_under_attack_timer;
    void accessThreatLevels();

    void updateFsmState();
    void updateAIState();

    // ** Opcode Functions
    void unitOpcodeMove(const UnitOpcode* opcode );
    void unitOpcodeTrackPoint(const UnitOpcode* opcode );
    void unitOpcodeTrackTarget(const UnitOpcode* opcode );
    void unitOpcodeFireWeapon(const UnitOpcode* opcode );
    void unitOpcodeSync(const UnitOpcode* opcode );
    void unitOpcodeUpdateState(const UnitOpcode* opcode );
    void unitOpcodeDestruct(const UnitOpcode* opcode );

    UnitOpcodeQueue opcode_queue;
    UnitOpcodeQueue move_opcode_queue;
    void processMoveOpcodeQueue();
    void processOpcodeQueue();

    // ** Message Handlers
    
    iXY             pending_moveToLocation;
    UnitID          pending_enemyUnit;
    unsigned char   pending_command; 
    bool	    pending_AI_comm;
    void checkPendingAICommStatus();

    void setCommandMoveToLoc();
    void setCommandAttackUnit();

    bool weaponHit(const UnitID from_unit, const Uint16 damage_factor);
    
    void setUnitProperties( unsigned char utype );

    void moveToLoc(const iXY& loc);
    void attackUnit(const UnitID unit_id);
    void manualShoot(const iXY& loc);
    void selfDestruct();
    
public:
    Vehicle(PlayerState* player, unsigned char utype, UnitID id, iXY initial_loc);

    virtual void updateState();

    virtual void evalCommandOpcode(const UnitOpcode* opcode);

    virtual void syncUnit();

    virtual void offloadGraphics( SpriteSorter &sorter );

    float smolderWait;
    float smolderWaitMin;
};


#endif // ** _VEHICLE_HPP
