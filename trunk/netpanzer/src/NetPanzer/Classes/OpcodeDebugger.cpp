#include <config.h>

#include <string>
#include <sstream>
#include <stdio.h>
#include <unistd.h>

#include "OpcodeDebugger.hpp"
#include "UnitOpcodes.hpp"

void OpcodeDebugger::logOpcode(std::ostream& log, UnitOpcode* opcode)
{
    log << " UNIT:" << opcode->getUnitID()
        << " FL:" << (int) opcode->flags << " ";
    switch(opcode->opcode) {
        case _UNIT_OPCODE_MOVE:
        {
            MoveOpcode* movecode = (MoveOpcode*) opcode;
            log << "move: S:" << movecode->getSquare()
                << " dX:" << (int) movecode->loc_x_offset
                << " dY:" << (int) movecode->loc_y_offset;
            break;
        }
        case _UNIT_OPCODE_TURRET_TRACK_POINT:
        {
            TurretTrackPointOpcode* trackcode =(TurretTrackPointOpcode*) opcode;
            log << "ttrackp: x:" << trackcode->getTarget().x
                << " y:" << trackcode->getTarget().y
                << " A:" << (int) trackcode->activate;
            break;
        }         
        case _UNIT_OPCODE_TURRET_TRACK_TARGET:
        {
            TurretTrackTargetOpcode* tracktcode
                = (TurretTrackTargetOpcode*) opcode;
            log << "ttrack: U:" << tracktcode->getTargetUnitID()
                << " A:" << (int) tracktcode->activate;
            break;
        }
        case _UNIT_OPCODE_FIRE_WEAPON:
        {
            FireWeaponOpcode* weaponcode = (FireWeaponOpcode*) opcode;
            log << "fire: X:" << weaponcode->getTarget().x
                << " Y:" << weaponcode->getTarget().y;
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
        default:
        {
            log << "unknown";
            break;
        }
    }
}

