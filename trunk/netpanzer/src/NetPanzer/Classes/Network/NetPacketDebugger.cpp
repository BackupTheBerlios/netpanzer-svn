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
#include "ClientServerNetMessage.hpp"
#include "ConnectNetMessage.hpp"

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
            logClientServerMessage(*log, message);
            break;
        }
        case _net_message_class_system:
        {
            *log << "System";
            break;
        }
        case _net_message_class_connect:
        {
            logConnectMessage(*log, message);
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
    for (size_t i=sizeof(NetMessage); i<message->getSize(); ++i) {
        if ((i%4) == 0)
            *log << " ";
        log->width(2);
        *log << (int) data[i];
    }
    log->flags(std::ios::dec);
    *log << std::endl;
}

void NetPacketDebugger::logMultiMessage(std::ostream& log,
        NetMessage* message)
{
    MultiMessage* mmessage = (MultiMessage*) message;
    
    log << "multimessage:\n";
    size_t index = 0;
    for(int i=0; i<mmessage->message_count; i++) {
        if(index + mmessage->getHeaderSize() >= message->getSize()) {
            log << "****Incorrect multi message!!!\n";
            log << "Index: " << index << " HeaderSize: "
                << mmessage->getHeaderSize() << " MessageSize: "
                << message->getSize() << std::endl;
            return;
        }
        NetMessage* submessage = (NetMessage*) (mmessage->data + index);
        index += submessage->getSize();

        logMessage("  M", submessage);
    }
}

void NetPacketDebugger::logClientServerMessage(std::ostream& log,
        NetMessage* message)
{
    log << "Client/Server ";
    switch(message->message_id) {
        case _net_message_id_transport_client_accept:
            log << "(transport client accept)";
            break;
        case _net_message_id_client_connect_ack:
            log << "(client connect ack)";
            break;
        case _net_message_id_client_server_id:
            log << "(client server id)";
            break;
        case _net_message_id_client_keep_alive:
            log << "(client keep alive)";
            break;
        case _net_message_id_client_set_keepalive_state:
            log << "(client set keepalive state)";
            break;
        case _net_message_id_server_client_disconnect:
            log << "(server client disconnect)";
            break;
        case _net_message_id_server_keep_alive:
            log << "(server keep alive)";
            break;
        case _net_message_id_server_ping_request:
            log << "(server ping request)";
            break;
        case _net_message_id_client_ping_ack:
            log << "(client ping ack)";
            break;
        default:
            log << "(invalid messageid)";            
            break;                           }
}

void NetPacketDebugger::logConnectMessage(std::ostream& log,
        NetMessage* message)
{
    log << "Connect ";
    switch(message->message_id) {
        case _net_message_id_connect_join_game_request:
            log << "(join game request)";
            break;
        case _net_message_id_connect_join_game_request_ack:
            log << "(join game request ack)";
            break;
        case _net_message_id_client_start_connect:
            log << "(client start connect)";
            break;
        case _net_message_id_client_connect_request:
            log << "(client connect request)";
            break;
        case _net_message_id_client_connect_result:
            log << "(client connect result)";
            break;
        case _net_message_id_connect_client_settings:
            log << "(client settings)";
            break;
        case _net_message_id_client_connect_process_update:
            log << "(client connect process update)";
            break;
        case _net_message_id_client_connect_process_state_mesg:
            log << "(client connect process state message)";
            break;
        case _net_message_id_connect_server_game_setup:
            log << "(server game setup)";
            break;
        case _net_message_id_connect_client_game_setup_ping:
            log << "(client game setup ping)";
            break;
        case _net_message_id_connect_client_game_setup_ack:
            log << "(client game setup ack)";
            break;
        case _net_message_id_connect_client_abort:
            log << "(client abort)";
            break;
        case _net_message_id_connect_netPanzer_client_disconnect:
            log << "(netpnatzer client disconnect)";
            break;
        case _net_message_id_connect_netPanzer_server_disconnect:
            log << "(netpanzer server disconnect)";
            break;
        default:
            log << "(invalid messageid)";
            break;
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
            log << "move to:" 
                << cmd.getGoalLoc().x << "," << cmd.getGoalLoc().y;
            break;
        }
        case _command_attack_unit:
        {
            log << "Attack: " << cmd.getTargetUnitID();
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
            log << "MFire: " << cmd.getTargetLoc().x 
                << "," << cmd.getTargetLoc().y;
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
        = dataptr + (message->getSize() - UnitOpcodeMessage::getHeaderSize());
    while(dataptr < dataend) {
        log << "\n  ";
        OpcodeDebugger::logOpcode(log, (UnitOpcode*) dataptr);
        dataptr += sizeof(UnitOpcodeStruct);
    }
}

