#include <config.h>

#include <unistd.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "NetPacket.hpp"
#include "UnitNetMessage.hpp"
#include "TerminalNetMesg.hpp"
#include "UnitOpcodes.hpp"

#include "NetPacketDebugger.hpp"
#include "OpcodeDebugger.hpp"

void NetPacketDebugger::logMessage(const char* domain, NetMessage* message)
{
    static std::ofstream* log = 0;
    if(log == 0) {
        std::stringstream filename;
        filename << "packets_" << getpid() << ".txt";
        log = new std::ofstream(filename.str().c_str());
    }

    *log << domain << ": (" << (int) message->message_class << "/"
                   << (int) message->message_id << ") ";
    switch(message->message_class) {
        case _net_message_class_multi:
        {
            logMultiMessage(*log, message);
            break;
        }
        case _net_message_class_client_server:
        {
            *log << "Client/Server";
            break;
        }
        case _net_message_class_system:
        {
            *log << "System";
            break;
        }
        case _net_message_class_connect:
        {
            *log << "Connect";
            break;
        }
        case _net_message_class_player:
        {
            *log << "Player";
            break;
        }
        case _net_message_class_unit:
        {
            logUnitMessage(*log, message);
            break;
        }
        case _net_message_class_terminal:
        {
            logTerminalMessage(*log, message);
            break;
        }
        case _net_message_class_objective:
        {
            *log << "Objective";
            break;
        }
        case _net_message_class_game_control:
        {
            *log << "Game Control";
            break;
        }
        case _net_message_class_powerup:
        {
            *log << "Powerup";
            break;
        }
        default:
        {
            *log << "?";
            break;
        }
    }
    *log << std::endl;

    log->flags(std::ios::hex);
    log->fill('0');
    uint8_t* data = (uint8_t *) message;
    for (int i=0; i<message->getsize(); i++) {
        if ((i%4) == 0)
            *log << " ";
        log->width(2);
        *log << (uint32_t)data[i];
    }
    log->flags(std::ios::dec);
    *log << std::endl;
}

void NetPacketDebugger::logMultiMessage(std::ostream& log,
        NetMessage* message)
{
    MultiMessage* mmessage = (MultiMessage*) message;
    
    uint8_t* dataptr = mmessage->data + 1;
    log << "multimessage:\n";
    for(int i=0;i<mmessage->message_count; i++) {
        NetMessage* submessage = (NetMessage*) dataptr;
        logMessage("  M", submessage);
        dataptr += submessage->getsize() + 1;
        if(dataptr - mmessage->data >= message->getsize()) {
            assert("Incorrect multi messge" == 0);
        }
    }
}

void NetPacketDebugger::logTerminalMessage(std::ostream& log,
        NetMessage* message)
{
    log << "Terminal/";
    switch(message->message_id) {
        case _net_message_id_term_unit_cmd:
        {
            TerminalUnitCmdRequest* unitcmd 
                = (TerminalUnitCmdRequest*) message;
            const UMesgAICommand& cmd = unitcmd->comm_request;
            log << "UnitCmd: ";
            logAICommand(log, cmd);
            break;
        }
        case _net_message_id_term_unit_gen:
        {
            log << "UnitGen: ";
            break;
        }
        case _net_message_id_term_output_loc:
        {
            log << "OutpLoc: ";
            break;
        }
        default:
        {
            log << "?";
            break;
        }
    }
}

void NetPacketDebugger::logAICommand(std::ostream& log,
        const UMesgAICommand& cmd)
{
    switch(cmd.command) {
        case _command_move_to_loc:
        {
            log << "move to:" << cmd.goal_loc_x << "," << cmd.goal_loc_y;
            break;
        }
        case _command_attack_unit:
        {
            log << "Attack: " << cmd.target_id.getUnique();
            break;
        }
        case _command_start_manual_move:
        {
            log << "mmove: O:" << cmd.manual_move_orientation;
            break;
        }
        case _command_stop_manual_move:
        {
            log << "stop mm";
            break;
        }
        case _command_manual_fire:
        {
            log << "MFire: " << cmd.target_loc_x << "," << cmd.target_loc_y;
            break;
        }
        default:
        {
            log << "?";
            break;
        }
    }
}

void NetPacketDebugger::logUnitMessage(std::ostream& log, NetMessage* message)
{
    log << "Unit/";
    switch(message->message_id) {
        case _net_message_id_opcode_mesg:
        {
            log << "Opcode";
            logUnitOpcodeMessage(log, message);
            break;
        }
        case _net_message_id_ini_sync_mesg:
        {
            log << "IniSync";
            break;
        }
        case _net_message_id_destroy_unit:
        {
            log << "Destroy";
            break;
        }
        case _net_message_id_create_unit:
        {
            log << "Create";
            break;
        }
        case _net_message_id_unit_sync_integrity_check:
        {
            log << "Sync";
            break;
        }
        default:
        {
            log << "?";
            break;
        }
    }
}

void NetPacketDebugger::logUnitOpcodeMessage(std::ostream& log,
        NetMessage* message)
{
    UnitOpcodeMessage* opcodes = (UnitOpcodeMessage*) message;
    uint8_t* dataptr = opcodes->data;
    uint8_t* dataend 
        = dataptr + opcodes->opcode_count * sizeof(UnitOpcodeStruct);
    while(dataptr < dataend) {
        log << "\n  ";
        OpcodeDebugger::logOpcode(log, (UnitOpcode*) dataptr);
        dataptr += sizeof(UnitOpcodeStruct);
    }
}

