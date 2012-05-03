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
#ifndef _TEAMMANAGER_HPP
#define _TEAMMANAGER_HPP

#include "Interfaces/Team.hpp"

class TeamManager
{
protected:
    static Team *Teams_lists;
    static Uint8 max_Teams;

public:
    static void initialize(const Uint8 _max_teams);
    static Uint8 getTeamColor(Uint8 team_id);
    static void reset();
    static void addPlayer(PlayerID player_id);
    static void addPlayerinTeam(PlayerID player_id, Uint8 team_id);
    static void removePlayer(PlayerID player_id, Uint8 team_id);
    
    static void cleanUp();

    static PlayerID getMaxteams(){return max_Teams;}
    static iXY getPlayerSpawnPoint(PlayerID player_id);

    static short getKills( Uint8 team_id );
    static short getLosses( Uint8 team_id );
    static short getObjectivesHeld( Uint8 team_id );
    static Uint8 getTeamWin();
    static const std::string& getTeamName( Uint8 team_id );
    static bool testRuleScoreLimit( long score_limit );
    static bool testRuleObjectiveRatio( float precentage );
    static void drawFlag( Uint8 team_id, Surface &dest, int x, int y);
    
    static void PlayerrequestchangeTeam(PlayerID player_id, Uint8 newteam);
    static void serverrequestchangeTeam(PlayerID player_id, Uint8 team_idx);
    static void PlayerchangeTeam(PlayerID player_id, Uint8 team_idx);
};

#endif // ** _TEAMMANAGER_HPP
