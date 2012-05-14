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

#include "Interfaces/ConsoleInterface.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Interfaces/GameManager.hpp"
#include "Interfaces/MapInterface.hpp"
#include "Interfaces/TeamManager.hpp"
#include "Interfaces/Team.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "Interfaces/GameControlRulesDaemon.hpp"
#include "Classes/Network/NetworkClient.hpp"
#include "Classes/Network/PlayerNetMessage.hpp"
#include "Classes/Network/NetworkServer.hpp"
#include "Util/Log.hpp"
#include "Util/StringUtil.hpp"
#include "Objectives/ObjectiveInterface.hpp"
#include "Objectives/Objective.hpp"

Team       * TeamManager::Teams_lists = 0;
Uint8        TeamManager::max_Teams = 0;
static bool   * player_ready = 0;

static void resetPlayerReady()
{
    for ( int i = 0; i < PlayerInterface::getMaxPlayers(); i++ )
    {
        player_ready[ i ] = false;
    }
}

static bool isAllPlayersReady()
{
    for ( int i = 0; i < PlayerInterface::getMaxPlayers(); i++ )
    {
        if (PlayerInterface::getPlayer(i)->isActive())
        {
            if (player_ready[ i ] == false) return false;
        }
    }
    return true;
}

static void setReady(PlayerID player_id)
{
    player_ready[ player_id ] = true;
}

bool TeamManager::isPlayerReady(PlayerID player_id)
{
    return player_ready[ player_id ];
}

void TeamManager::initialize(const Uint8 _max_teams)
{
    char txtBuf[256];
    Uint8 colorsteam[4] = {Color::yellow, 208, Color::green, Color::cyan};
    max_Teams = _max_teams;

    delete[] Teams_lists;
    Teams_lists = new Team[max_Teams];
    delete[] player_ready;
    player_ready = new bool [PlayerInterface::getMaxPlayers()];

    std::vector<NPString> plist;
    NPString pl = *GameConfig::game_team_names;
    string_to_params(pl, plist);


    for ( int team_id = 0; team_id < max_Teams; ++team_id )
    {
        Teams_lists[ team_id ].initialize(team_id);
        if (team_id < (Uint8) plist.size()) Teams_lists[ team_id ].setName(plist[team_id]);
        snprintf(txtBuf, sizeof(txtBuf), "pics/default/team-%d.bmp", team_id+1);
        Teams_lists[ team_id ].loadFlag(txtBuf);
        Teams_lists[ team_id ].setColor(colorsteam[team_id]);
    }
}

Uint8 TeamManager::getTeamColor(Uint8 team_id)
{
    return Teams_lists[ team_id ].getColor();
}

void TeamManager::addPlayer(PlayerID player_id)
{
    int team_id;
    int lowTeam = 0, countPlayers = Teams_lists[ 0 ].countPlayers();

    for ( team_id = 0; team_id < max_Teams; ++team_id )
    {
        if (Teams_lists[ team_id ].countPlayers() < countPlayers)
        {
            countPlayers = Teams_lists[ team_id ].countPlayers();
            lowTeam = team_id;
        }
    }
    Teams_lists[ lowTeam ].addPlayer(player_id);
}

void TeamManager::BalancedTeam()
{
    int team_id;
    int lowTeam = 0, highTeam = 0;
    int minPlayers = Teams_lists[ 0 ].countPlayers(), maxPlayers = 0;

    for ( team_id = 0; team_id < max_Teams; ++team_id )
    {
        if (Teams_lists[ team_id ].countPlayers() < minPlayers)
        {
            minPlayers = Teams_lists[ team_id ].countPlayers();
            lowTeam = team_id;
        }
        if (Teams_lists[ team_id ].countPlayers() > maxPlayers)
        {
            maxPlayers = Teams_lists[ team_id ].countPlayers();
            highTeam = team_id;
        }
    }
    if ((maxPlayers - minPlayers) > 1)
    {
        serverrequestchangeTeam(Teams_lists[ highTeam ].getrandomplayer(), lowTeam);
    }
}

void TeamManager::reset()
{
    for ( PlayerID player_index = 0; player_index < PlayerInterface::getMaxPlayers()-1; ++player_index )
    {
        for ( PlayerID player_id = 1; player_id < PlayerInterface::getMaxPlayers(); ++player_id )
        {
            PlayerState* state_id = PlayerInterface::getPlayer(player_id);
            PlayerState* state_index = PlayerInterface::getPlayer(player_index);
            if (state_id->isActive() && state_index->isActive())
            {
                if (state_id->getTeamID() == state_index->getTeamID()
                        && (player_id != player_index))
                {
                    PlayerInterface::allyplayers( player_id, player_index);
                }
            }
        }
    }
    resetPlayerReady();
}

void TeamManager::addPlayerinTeam(PlayerID player_id, Uint8 team_id)
{
    Teams_lists[ team_id ].addPlayer(player_id);
}

void TeamManager::removePlayer(PlayerID player_id, Uint8 team_id)
{
    Teams_lists[ team_id ].removePlayer(player_id);
}

void TeamManager::cleanUp()
{
    Uint8 team_id;
    for ( team_id = 0; team_id < max_Teams; ++team_id )
    {
        Teams_lists[ team_id ].cleanUp();
    }
    delete[] Teams_lists;
    Teams_lists = 0;
    max_Teams = 0;
}

iXY TeamManager::getPlayerSpawnPoint(PlayerID player_id)
{
    Uint8 Team_id = PlayerInterface::getPlayer(player_id)->getTeamID();

    iXY spawn_point;
    switch (Team_id)
    {
    case 0:
        spawn_point = MapInterface::getMinSpawnPoint();
        break;
    case 1:
        spawn_point = MapInterface::getMaxSpawnPoint();
        break;
    default:
        spawn_point = MapInterface::getFreeSpawnPoint();
    }
    return spawn_point;
}

short TeamManager::getKills(  Uint8 team_id )
{
    return Teams_lists[team_id].getKills();
}

short TeamManager::getLosses(  Uint8 team_id )
{
    return Teams_lists[team_id].getLosses();
}

short TeamManager::getObjectivesHeld( Uint8 team_id )
{
    return Teams_lists[team_id].getTeamObjective();
}

Uint8 TeamManager::getTeamWin()
{
    long Score = 0;
    Uint8 teamWin = 0;
    unsigned char game_type = GameConfig::game_gametype;

    switch (game_type)
    {
    case _gametype_fraglimit:
    case _gametype_timelimit:
        for (Uint8 team_id = 0; team_id < max_Teams; ++team_id )
        {
            Score = Teams_lists[team_id].getKills();
            if (Score < Teams_lists[team_id].getKills())
            {
                teamWin = team_id;
                Score = Teams_lists[team_id].getKills();
            }
        }
        break;
    case _gametype_objective:
        for (Uint8 team_id = 0; team_id < max_Teams; ++team_id )
        {
            Score = Teams_lists[team_id].getTeamObjective();
            if (Score < Teams_lists[team_id].getTeamObjective())
            {
                teamWin = team_id;
                Score = Teams_lists[team_id].getTeamObjective();
            }
        }
        break;
    }
    return teamWin;
}

const std::string& TeamManager::getTeamName( Uint8 team_id )
{
    return Teams_lists[team_id].getName();
}

void TeamManager::drawFlag(Uint8 team_id, Surface& dest, int x, int y)
{
    Teams_lists[team_id].drawFlag(dest, x, y);
}

bool TeamManager::testRuleScoreLimit( long score_limit )
{
    for (Uint8 team_id = 0; team_id < max_Teams; ++team_id )
    {
        if ( Teams_lists[team_id].getKills() >= score_limit )
            return( true );
    }
    return( false );
}

bool TeamManager::testRuleObjectiveRatio( float precentage )
{
    ObjectiveID num_objectives = ObjectiveInterface::getObjectiveCount();

    int occupation_ratio = (int) ( ((float) num_objectives) * precentage  + 0.999);
    if (occupation_ratio == 0)
    {
        occupation_ratio = 1;
    }

    for (Uint8 team_id = 0; team_id < max_Teams; ++team_id )
    {
        if ( Teams_lists[team_id].getTeamObjective() >= occupation_ratio )
        {
            return true;
        }
    }
    return false;
}

void TeamManager::PlayerrequestchangeTeam(PlayerID player_id, Uint8 newteam)
{
    PlayerTeamRequest Changeteam_request;
    Changeteam_request.set(player_id, newteam, change_team_request);
    CLIENT->sendMessage( &Changeteam_request, sizeof(PlayerTeamRequest));
}

void TeamManager::serverrequestchangeTeam(PlayerID player_id, Uint8 newteam)
{
    Uint8 current_team = PlayerInterface::getPlayer(player_id)->getTeamID();

    if (GameControlRulesDaemon::getGameState() == _game_state_prepare_team)
    {
        if (((PlayerInterface::getMaxPlayers()/2) > Teams_lists[newteam].countPlayers()))
        {
            Teams_lists[current_team].removePlayer(player_id);
            Teams_lists[newteam].addPlayer(player_id);
            PlayerTeamRequest Changeteam_request;
            Changeteam_request.set(player_id, newteam, change_team_Accepted);
            SERVER->broadcastMessage( &Changeteam_request, sizeof(PlayerTeamRequest));
        }
    }
    else
    {
        if ( (Teams_lists[newteam].countPlayers() < Teams_lists[current_team].countPlayers())
                && ((PlayerInterface::getMaxPlayers()/2) > Teams_lists[newteam].countPlayers()))
        {
            Teams_lists[current_team].removePlayer(player_id);
            Teams_lists[newteam].addPlayer(player_id);
            PlayerTeamRequest Changeteam_request;
            Changeteam_request.set(player_id, newteam, change_team_Accepted);
            SERVER->broadcastMessage( &Changeteam_request, sizeof(PlayerTeamRequest));

            UnitInterface::destroyPlayerUnits(player_id);
            GameManager::spawnPlayer( player_id );
        }
    }
}

void TeamManager::PlayerchangeTeam(PlayerID player_id, Uint8 team_idx)
{
    Uint8 current_team = PlayerInterface::getPlayer(player_id)->getTeamID();

    Teams_lists[current_team].removePlayer(player_id);
    Teams_lists[team_idx].addPlayer(player_id);

    if (GameControlRulesDaemon::getGameState() != _game_state_prepare_team)
        ConsoleInterface::postMessage(Color::yellow, false, 0,
                                      "%s has changed to team %s.",
                                      PlayerInterface::getPlayer(player_id)->getName().c_str(),
                                      Teams_lists[current_team].getName().c_str());
}

void TeamManager::PlayerRequestReady(PlayerID player_id)
{
    PlayerReadyRequest Ready_request;
    Ready_request.set(player_id, ready_request);
    CLIENT->sendMessage( &Ready_request, sizeof(PlayerReadyRequest));
}

void TeamManager::ServerRequestReady(PlayerID player_id)
{
    setReady(player_id);
    PlayerReadyRequest Ready_request;
    Ready_request.set(player_id, ready_Accepted);
    SERVER->broadcastMessage( &Ready_request, sizeof(PlayerReadyRequest));
}

bool TeamManager::CheckisPlayerReady()
{
    if (PlayerInterface::getActivePlayerCount() > 1)
    {
        if (isAllPlayersReady())
        {
            return true;
        }
    }
    return false;
}

void TeamManager::PlayerRequestReadyAccepted(PlayerID player_id)
{
    setReady(player_id);
}

void TeamManager::SpawnTeams()
{
    for ( PlayerID player_id = 0; player_id < PlayerInterface::getMaxPlayers(); ++player_id )
    {
        PlayerState* state_id = PlayerInterface::getPlayer(player_id);
        if (state_id->isActive())
        {
            UnitInterface::destroyPlayerUnits(player_id);
            GameManager::spawnPlayer( player_id );
        }
    }
}

void TeamManager::netMessageChangeTeamRequest(const NetMessage* message)
{
    const PlayerTeamRequest* changeTeamRequest
    = (const PlayerTeamRequest *) message;
    
    switch(changeTeamRequest->request_type)
    {
    case change_team_request :
        serverrequestchangeTeam(changeTeamRequest->getPlayerIndex(),changeTeamRequest->gettoteamindex());
        break;
 
    case change_team_Accepted:
        PlayerchangeTeam(changeTeamRequest->getPlayerIndex(),changeTeamRequest->gettoteamindex());
        break;
    }
}
void TeamManager::netMessageReadyRequest(const NetMessage* message)
{
    const PlayerReadyRequest* ReadyRequest
    = (const PlayerReadyRequest *) message;
    
    switch(ReadyRequest->request_type)
    {
    case ready_request :
        ServerRequestReady(ReadyRequest->getPlayerIndex());
        break;
 
    case change_team_Accepted:
        PlayerRequestReadyAccepted(ReadyRequest->getPlayerIndex());
        break;
    }
    
}






