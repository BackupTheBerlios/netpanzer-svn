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
#include "Interfaces/MapInterface.hpp"
#include "Interfaces/TeamManager.hpp"
#include "Interfaces/Team.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "Classes/Network/NetworkClient.hpp"
#include "Classes/Network/PlayerNetMessage.hpp"
#include "Classes/Network/NetworkServer.hpp"
#include "Util/Log.hpp"
#include "Util/StringUtil.hpp"
#include "Objectives/ObjectiveInterface.hpp"
#include "Objectives/Objective.hpp"


Team       * TeamManager::Teams_lists = 0;
Uint8        TeamManager::max_Teams = 0;

void TeamManager::initialize(const Uint8 _max_teams)
{
    char txtBuf[256];
    Uint8 colorsteam[4] = {Color::yellow, 208, Color::green, Color::cyan};
    max_Teams = _max_teams;
    int team_id;

    delete[] Teams_lists;
    Teams_lists = new Team[max_Teams];

    std::vector<NPString> plist;
    NPString pl = *GameConfig::game_team_names;
    string_to_params(pl, plist);

    for ( team_id = 0; team_id < max_Teams; ++team_id )
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

    if ( (Teams_lists[newteam].countPlayers() < Teams_lists[current_team].countPlayers())
            && (Teams_lists[newteam].countPlayers() > 0))
    {
        Teams_lists[current_team].removePlayer(player_id);
        Teams_lists[newteam].addPlayer(player_id);
        PlayerTeamRequest Changeteam_request;

        Changeteam_request.set(player_id, newteam, change_team_Accepted);
        SERVER->broadcastMessage( &Changeteam_request, sizeof(PlayerTeamRequest));
    }
}

void TeamManager::PlayerchangeTeam(PlayerID player_id, Uint8 team_idx)
{
    Uint8 current_team = PlayerInterface::getPlayer(player_id)->getTeamID();
    Teams_lists[current_team].removePlayer(player_id);
    Teams_lists[team_idx].addPlayer(player_id);
    ConsoleInterface::postMessage(Color::yellow, false, 0,
                                  "%s has changed to team %s.",
                                  PlayerInterface::getPlayer(player_id)->getName().c_str(),
                                  Teams_lists[current_team].getName().c_str());
}





