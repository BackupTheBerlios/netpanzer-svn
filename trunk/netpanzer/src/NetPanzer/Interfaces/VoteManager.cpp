/*
Copyright (C) 2012 Netpanzer Team. (www.netpanzer.org), Laurent Jacques

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

#include "Interfaces/VoteManager.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "Interfaces/ChatInterface.hpp"
#include "Interfaces/GameControlRulesDaemon.hpp"
#include "Classes/Network/PlayerNetMessage.hpp"
#include "Classes/Network/NetworkServer.hpp"
#include "Classes/Network/NetworkClient.hpp"
#include "Classes/Network/NetworkState.hpp"
#include "Views/Components/Desktop.hpp"
#include "Views/Game/VoteBox.hpp"

bool   VoteManager::vote_in_progress = false;
Uint8  VoteManager::type_vote = 0;
bool  *VoteManager::player_vote = 0;
Uint8  VoteManager::vote_counter = 0;
Timer  VoteManager::voteTimer;

VoteBox *votebox;

const char *VoteStrings[2] = {"unknow vote", "Surrendering vote, you choose surrendering?\0"};

void VoteManager::netMessageVoteRequest(const NetMessage* message)
{
    const PlayerSendVote* voteplayer
    = (const PlayerSendVote *) message;

    if (vote_in_progress)
    {
        vote_counter--;
        player_vote[ voteplayer->getPlayerIndex() ] = voteplayer->player_vote;
        char buff[100];
        sprintf(buff, "player %s has voted, waiting %d votes",
                PlayerInterface::getPlayer(voteplayer->getPlayerIndex())->getName().c_str(),
                vote_counter);
        NPString buffstr = buff;
        ChatInterface::serversay(buffstr);
        if (vote_counter < 1) 
            checkPlayersVote();
    }
}

void VoteManager::startVote(Uint8 type)
{
    resetVote();
    vote_in_progress = true;
    type_vote = type;
}

void VoteManager::netMessageReceiveRequestVote(const NetMessage* message, PlayerID playerid)
{
    const PlayerVoteRequested* vote_request
    = (const PlayerVoteRequested *) message;

    if ( NetworkState::status == _network_state_server )
    {
        if (vote_in_progress) return;
        startVote(vote_request->vote_type);
        serverSendRequestVote();
        char buff[100];
        sprintf(buff, "Player %s request vote",
                PlayerInterface::getPlayer(playerid)->getName().c_str());
        NPString buffstr = buff;
        ChatInterface::serversay(buff);
    }
    else
    {
        startVote(vote_request->vote_type);
        votebox = new VoteBox(VoteStrings[type_vote]);
        Desktop::add(votebox);
        Desktop::setVisibility("votebox", true);
    }
}

void VoteManager::resetVote()
{
    vote_in_progress = false;
    type_vote = 0;
    delete[] player_vote;
    if (votebox)
    {
        Desktop::remove(votebox);
    }
    player_vote = new bool [PlayerInterface::getActivePlayerCount()];
    for ( int i = 0; i < PlayerInterface::getActivePlayerCount(); i++ )
    {
        player_vote[ i ] = false;
    }
    if ( NetworkState::status == _network_state_server )
        voteTimer.changePeriod(70);// more time for prevent some client retard (lag)
        else
        voteTimer.changePeriod(60);
    voteTimer.reset();
    vote_counter = PlayerInterface::getActivePlayerCount();
}

void VoteManager::playerSendRequestVote(Uint8 type)
{
    PlayerVoteRequested vote_request;
    vote_request.set(type);
    CLIENT->sendMessage( &vote_request, sizeof(PlayerVoteRequested));
}

void VoteManager::serverSendRequestVote()
{
    PlayerVoteRequested vote_request;
    vote_request.set(type_vote);
    SERVER->broadcastMessage( &vote_request, sizeof(PlayerVoteRequested));
}

void VoteManager::executeVoteAction()
{
    switch (type_vote)
    {
        case 1 :
            GameControlRulesDaemon::forceEndRound();
            break;
    }
}

void VoteManager::checkPlayersVote()
{

    ChatInterface::serversay("Vote is end");

    int yes_vote = 0, no_vote = 0;
    
    for ( int i = 0; i < PlayerInterface::getActivePlayerCount(); i++ )
    {
        if (player_vote[ i ] == true) 
            yes_vote++;
        else
            no_vote++;
    }
    char buff[100];
    sprintf(buff, " %d players has voted YES, %d has voted NO", yes_vote, no_vote);
    NPString buffstr = buff;
    ChatInterface::serversay(buffstr);
    vote_in_progress = false;
    if (yes_vote > no_vote) executeVoteAction();
}

void VoteManager::playerVote(bool responce)
{
    Desktop::setVisibility("votebox", false);
    Desktop::remove(votebox);

    PlayerSendVote vote_player;
    vote_player.set(PlayerInterface::getLocalPlayerIndex(), responce);
    CLIENT->sendMessage( &vote_player, sizeof(PlayerSendVote));
}

bool VoteManager::checkVoteTimer()
{
    if (vote_in_progress)
    {
        return voteTimer.count();
    }
    return false;
    //server need to check the timer for stop vote
    //if player crash or quit the game, vote don't ending
}

int VoteManager::getTimer()
{
    return (int) voteTimer.getTimeLeft();
}

