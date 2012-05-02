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

#include "Interfaces/PlayerInterface.hpp"
#include "Interfaces/Team.hpp"
#include "Util/Log.hpp"
#include "2D/Surface.hpp"

void Team::initialize(const Uint8 id)
{
    teamID = id;
}

void Team::setName(const std::string& newname)
{
    if ( newname.length() > 25 )
    {
        name = newname.substr(0,25);
    }
    else
    {
        name = newname;
    }
}

const std::string& Team::getName() const
{
    return name;
}

void Team::addPlayer(PlayerID new_player)
{
    for ( PlayerID player_id = 0; player_id < PlayerInterface::getMaxPlayers(); ++player_id )
    {
        PlayerState* state = PlayerInterface::getPlayer(player_id);
        if (state->isActive())
        {
            if (state->getTeamID() == teamID
                && (player_id != new_player)) 
            {
                PlayerInterface::allyplayers( player_id, new_player);
            }
        }
    }
    PlayerInterface::getPlayer(new_player)->setTeamID(teamID);
}

void Team::removePlayer(PlayerID old_player)
{
    for ( PlayerID player_id = 0; player_id < PlayerInterface::getMaxPlayers(); ++player_id )
    {
        if (PlayerInterface::getPlayer(player_id)->isActive())
        {
            if (PlayerInterface::getPlayer(player_id)->getTeamID() == teamID) 
            {
                PlayerInterface::unallyplayers( old_player, player_id);
            }
        }
    }
    PlayerInterface::getPlayer(old_player)->setTeamID(0xFF);
}

void Team::cleanUp()
{
}

PlayerID Team::countPlayers() const
{
    PlayerID count = 0;
    PlayerID player_id;
    for ( player_id = 0; player_id < PlayerInterface::getMaxPlayers(); ++player_id )
    {
        if (PlayerInterface::getPlayer(player_id)->isActive())
        {
            if (PlayerInterface::getPlayer(player_id)->getTeamID() == teamID) 
            {
                count++;
            }
        }
    }
    return count;
}

void Team::loadFlag(const char *fileName)
{
    Flag.loadBMP(fileName);
}

void Team::drawFlag(Surface &dest, int x, int y) const
{
    Flag.bltTrans(dest, x, y);
}

short Team::getKills() const
{
    short TeamKills = 0;
    PlayerID player_id;
    
    for ( player_id = 0; player_id < PlayerInterface::getMaxPlayers(); ++player_id )
    {
        PlayerState* state = PlayerInterface::getPlayer(player_id);
        if (state->isActive())
        {
            if (state->getTeamID() == teamID) 
            {
                TeamKills += state->getKills();
            }
        }
    }
    return TeamKills;
}

short Team::getLosses() const
{
    long TeamLosses = 0;
    PlayerID player_id;
    
    for ( player_id = 0; player_id < PlayerInterface::getMaxPlayers(); ++player_id )
    {
        PlayerState* state = PlayerInterface::getPlayer(player_id);
        if (state->isActive())
        {
            if (state->getTeamID() == teamID) 
            {
                TeamLosses += state->getLosses();
            }
        }
    }
    return TeamLosses;
}

int Team::getTeamObjective() const
{
    long TeamObjective = 0;
    PlayerID player_id;
    
    for ( player_id = 0; player_id < PlayerInterface::getMaxPlayers(); ++player_id )
    {
        PlayerState* state = PlayerInterface::getPlayer(player_id);
        if (state->isActive())
        {
            if (state->getTeamID() == teamID) 
            {
                TeamObjective += state->getObjectivesHeld();
            }
        }
    }
    return TeamObjective;
}

