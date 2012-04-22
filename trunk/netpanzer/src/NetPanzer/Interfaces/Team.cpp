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

void Team::initialize(const Uint8 _max_players)
{
    max_players = _max_players;

    player_lists = new PlayerID[max_players];

    for (PlayerID player_id = 0; player_id < max_players; ++player_id )
    {
        player_lists[ player_id ] = 0xFF;
    }
    //LOGGER.warning("Team::initialize(%d)", _max_players );
}

void Team::setName(const std::string& newname)
{
    if ( newname.length() > 63 )
    {
        name = newname.substr(0,63);
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

void Team::addPlayer(PlayerID player_id)
{
    for ( PlayerID _player_id = 0; _player_id < max_players; ++_player_id )
    {
        if (player_lists[ _player_id ] != 0xFF) 
        {
            //LOGGER.warning("ally %d with %d", _player_id, player_id);
            PlayerInterface::allyplayers( _player_id, player_id);
            PlayerInterface::allyplayers( player_id, _player_id);
        }
    }
    player_lists[ player_id ] = player_id;
    PlayerInterface::getPlayer(player_id)->setTeamID(teamID);
    //LOGGER.warning("Team::addPlayer %d", player_id);
}

void Team::removePlayer(PlayerID player_id)
{
   // LOGGER.warning("Team::removePlayer %d", player_id);
    player_lists[ player_id ] = 0xFF;
}

void Team::cleanUp()
{
    delete[] player_lists;
    player_lists = 0;
    max_players = 0;
}

PlayerID Team::countPlayers()
{
    PlayerID count = 0;
    PlayerID player_id;
    for ( player_id = 0; player_id < max_players; ++player_id )
    {
        if (player_lists[ player_id ] != 0xFF) count++;
    }
    return count;
}

void Team::setID( Uint8 _id )
{
    teamID = _id;
}

void Team::spawnTeam( const iXY &location )
{
    PlayerID player_id;
    for ( player_id = 0; player_id < max_players; ++player_id )
    {
        if (player_lists[ player_id ] != 0xFF) 
        {
            PlayerInterface::spawnPlayer(player_id, location);
        }
    }
}

void Team::spawnPlayer( PlayerID _player_id, const iXY &location )
{
    PlayerInterface::spawnPlayer(_player_id, location);
}

long Team::getTeamScore()
{
    long teamScore = 0;
    PlayerID player_id;
    
    for ( player_id = 0; player_id < max_players; ++player_id )
    {
        if (player_lists[ player_id ] != 0xFF) 
        {
            teamScore = PlayerInterface::getPlayer(player_id)->getKills();
        }
    }
    return teamScore;
}
