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
#include <vector>

#include "UnitState.hpp"
#include "Util/Timer.hpp"
#include "Classes/UnitMessageTypes.hpp"
#include "Units/UnitOpcodes.hpp"
#include "Classes/AI/PathList.hpp"

#include "Classes/Sprite.hpp"
#include "Classes/SelectionBoxSprite.hpp"
#include "Classes/SpriteSorter.hpp"

class PlayerState;
class UnitInterface;

class Unit
{
public:
    PlayerState* player;
    UnitID       id;
    UnitState    unit_state;
    bool         in_sync_flag;

    Unit(PlayerState* ownplayer, UnitInterface * unit_manager, unsigned char utype, UnitID uid, iXY initial_loc);

    void soundSelected();

    void updateState();

    void processMessage(const UnitMessage* message);

    void evalCommandOpcode(const UnitOpcode* opcode);

    void syncUnit();

    UnitID getID() const
    {
        return id;
    }

    float smolderWait;
    float smolderWaitMin;

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
    bool path_generated;
    bool critical_ai_section;
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
    void    setFsmBodyRotate( long goal_angle, unsigned short rotation );
    bool fsmBodyRotate();

    unsigned short fsmTurretRotate_rotation;
    long           fsmTurretRotate_goal_angle;
    void    setFsmTurretRotate( long goal_angle, unsigned short rotation );
    bool fsmTurretRotate();

    float interpolation_speed;
    TimeStamp start_move_stamp;
    TimeStamp end_move_stamp;
    bool     fsmMove_first_stamp;
    signed char fsmMove_offset_x;
    signed char fsmMove_offset_y;
    unsigned char fsmMove_moves_counter;
    unsigned char fsmMove_moves_per_square;
    void setFsmMove( unsigned short orientation );
    bool fsmMove();

    MoveOpcode move_opcode;
    Timer opcode_move_timer;
    bool move_opcode_sent;
    unsigned char fsmMoveMapSquare_movement_type;
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
    void setFsmGunneryLocation(const iXY& target );
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


    unsigned char aiFsmManualMove_move_orientation;
    unsigned char aiFsmManualMove_state;
    iXY aiFsmManualMove_next_loc;
    iXY aiFsmManualMove_prev_loc;
    void aiFsmManualMove();

    void fireWeapon( iXY &target_loc );
    unsigned short launchProjectile();

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
    UMesgAICommand pending_AI_comm_mesg;
    bool	     pending_AI_comm;
    void checkPendingAICommStatus();

    void setCommandMoveToLoc(const UMesgAICommand* message);
    void setCommandAttackUnit(const UMesgAICommand* message);
    void setCommandManualFire(const UMesgAICommand* message);

    void messageAICommand(const UnitMessage* message);
    void messageWeaponHit(const UnitMessage* message);
    void messageSelectBoxUpdate(const UnitMessage* message);
    void messageSelfDestruct(const UnitMessage* message);
    
    void setUnitProperties( unsigned char utype );

private:
    friend class UnitInterface;
    UnitInterface * my_manager;

    void setID(UnitID id)
    {
        this->id = id;
    }

};


#endif // ** _VEHICLE_HPP
