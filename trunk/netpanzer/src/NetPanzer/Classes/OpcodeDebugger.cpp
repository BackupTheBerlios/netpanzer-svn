#include <config.h>

#include <string>
#include <sstream>
#include <stdio.h>
#include <unistd.h>

#include "OpcodeDebugger.hpp"
#include "UnitOpcodes.hpp"

void OpcodeDebugger::logOpcode(std::ostream& log, UnitOpcode* opcode)
{
    log << "PL:" << (int) opcode->player_index
        << " UNIT:" << (int) opcode->unit_index
        << " FL:" << (int) opcode->flags << " ";
    switch(opcode->opcode) {
        case _UNIT_OPCODE_MOVE:
        {
            MoveOpcode* movecode = (MoveOpcode*) opcode;
            log << "move: S:" << movecode->square
                << " dX:" << (int) movecode->loc_x_offset
                << " dY:" << (int) movecode->loc_y_offset;
            break;
        }
        case _UNIT_OPCODE_TURRET_TRACK_POINT:
        {
            TurretTrackPointOpcode* trackcode =(TurretTrackPointOpcode*) opcode;
            log << "ttrackp: x:" << trackcode->x
                << " y:" << trackcode->y
                << " A:" << (int) trackcode->activate;
            break;
        }         
        case _UNIT_OPCODE_TURRET_TRACK_TARGET:
        {
            TurretTrackTargetOpcode* tracktcode
                = (TurretTrackTargetOpcode*) opcode;
            log << "ttrack: PL:" << tracktcode->target_player_index
                << " U:" << tracktcode->target_unit_index
                << " A:" << (int) tracktcode->activate;
            break;
        }
        case _UNIT_OPCODE_FIRE_WEAPON:
        {
            FireWeaponOpcode* weaponcode = (FireWeaponOpcode*) opcode;
            log << "fire: X:" << weaponcode->x
                << " Y:" << weaponcode->y;
            break;
        }
        case _UNIT_OPCODE_SYNC_UNIT:
        {
            //SyncUnitOpcode* synccode = (SyncUnitOpcode*) opcode;
            log << "sync";
            break;
        }
        case _UNIT_OPCODE_UPDATE_STATE:
        {
            UpdateStateUnitOpcode* updatecode = 
                (UpdateStateUnitOpcode*) opcode;
            log << "upd: HP:" << (int) updatecode->hit_points;
            break;
        }
        case _UNIT_OPCODE_DESTRUCT:
        {
            /*DestructUnitOpcode* destructcode =
                (DestructUnitOpcode*) opcode;*/
            log << "destruct";
            break;
        }
    }
}

