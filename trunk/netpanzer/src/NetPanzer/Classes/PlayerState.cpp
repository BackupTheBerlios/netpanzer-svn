/*
Copyright (C) 1998 Pyrosoft Inc. (www.pyrosoftgames.com), Matthew Bogue

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
#include <config.h>
#include "Util/Endian.hpp"
#include "PlayerState.hpp"

uint16_t NetworkPlayerState::getPlayerIndex(void)
{
    return ltoh16(playerindex_id);
}

//If you modify this array, also modify the constant above
uint8_t *playerColorArray[] = {
	&Color::red,
	&Color::green,
	&Color::brown,
	&Color::yellow,
	&Color::cyan,
	&Color::gray,
	&Color::magenta,
	&Color::pink,
	&Color::unitAqua,
	&Color::unitYellow,
	&Color::unitRed,
	&Color::unitBlue,
	&Color::unitDarkBlue,
	&Color::unitLightGreen,
	&Color::unitGreen,
	&Color::unitBlueGray,
	&Color::unitDarkRed,
	&Color::unitBlack,
	&Color::unitDarkGreen,
	&Color::unitWhite,
	&Color::unitLightOrange,
	&Color::unitOrange,
	&Color::unitGray,
	&Color::unitDarkGray,
	&Color::tan,
	&Color::chartreuse,
	&Color::cobaltGreen,
	&Color::emeraldGreen,
	&Color::forestGreen,
	&Color::darkOliveGreen,
	&Color::terreVerte,
	&Color::darkBlue,
	&Color::darkBrown,
	&Color::darkCyan,
	&Color::darkGray,
	&Color::darkGreen,
	&Color::darkMagenta,
	&Color::darkOrange,
	&Color::darkPink,
	&Color::darkRed,
	&Color::darkYellow,
	&Color::lightBlue,
	&Color::lightBrown,
	&Color::lightCyan,
	&Color::lightGray,
	&Color::lightGreen,
	&Color::lightMagenta,
	&Color::lightOrange,
	&Color::lightPink,
	&Color::lightRed,
	&Color::lightYellow,
	&Color::gray32,
	&Color::gray64,
	&Color::gray96,
	&Color::gray128,
	&Color::gray160,
	&Color::gray192,
	&Color::gray224
};

static const size_t playerColorCount 
    = sizeof(playerColorArray) / sizeof(uint8_t*);

void PlayerState::setColor( uint32_t index ) {
	colorIndex = index % playerColorCount;
//	printf( "%s got colorIndex: %d!\n", name, colorIndex );
}

uint8_t PlayerState::getColor() {
    assert(colorIndex < playerColorCount);
    return ( *playerColorArray[ colorIndex ] );
}

PlayerState::PlayerState()
    : flag(0), status(0), kills(0), kill_points(0), losses(0),
      loss_points(0), total(0), objectives_held(0), stats_locked(false),
      colorIndex(0)
{
}

PlayerState::PlayerState(const PlayerState& other)
    : name(other.name), flag(other.flag), status(other.status),
      kills(other.kills), kill_points(other.kill_points),
      losses(other.losses), loss_points(other.loss_points),
      total(other.total), objectives_held(other.objectives_held),
      stats_locked(other.stats_locked), unit_config(other.unit_config)
{
}

void PlayerState::operator= (const PlayerState& other)
{
    name = other.name;
    flag = other.flag;
    status = other.status;
    kills = other.kills;
    kill_points = other.kill_points;
    losses = other.losses;
    loss_points = other.loss_points;
    total = other.total;
    objectives_held = other.objectives_held;
    stats_locked = other.stats_locked;
    unit_config = other.unit_config;
}

void PlayerState::setName(const std::string& newname)
{
    name = newname;
}

void PlayerState::setID( PlayerID player_id )
{
    ID = player_id;
}

void PlayerState::resetStats()
{
    kills  = 0;
    kill_points = 0;
    losses = 0;
    loss_points = 0;
    total = 0;
    objectives_held = 0;
    stats_locked = false;
}

const std::string& PlayerState::getName() const
{
    return name;
}

void PlayerState::lockStats()
{
    stats_locked = true;
}

void PlayerState::unlockStats()
{
    stats_locked = false;
}

short PlayerState::getKills()
{
    return( kills );
}

short PlayerState::getLosses()
{
    return( losses );
}

void PlayerState::incKills( unsigned short unit_type )
{
    if ( stats_locked == true ) return;
    kills++;
    kill_points += 2 * unit_config.getUnitPointValue( (unsigned char)  unit_type ) ;
}

void PlayerState::decKills(unsigned short unit_type)
{
    (void) unit_type;
    if ( stats_locked == true ) return;
    kills--;
}

void PlayerState::incLosses( unsigned short unit_type )
{
    if ( stats_locked == true ) return;
    losses++;
    loss_points += unit_config.getUnitPointValue( (unsigned char) unit_type );
}

void PlayerState::decLosses( unsigned short unit_type )
{
    (void) unit_type;
    if ( stats_locked == true ) return;
    losses--;
}

void PlayerState::incObjectivesHeld()
{
    if ( stats_locked == true ) return;
    objectives_held++;
}

void PlayerState::decObjectivesHeld()
{
    if ( stats_locked == true ) return;
    objectives_held++;
}

short PlayerState::getObjectivesHeld()
{
    return( objectives_held );
}

void PlayerState::setObjectivesHeld( short objectives )
{
    if ( stats_locked == true ) return;
    objectives_held = objectives;
}

void PlayerState::setID( unsigned short index, SocketClient::ID networkid  )
{
    ID.setIndex(index);
    ID.setNetworkID(networkid);
}

void PlayerState::setID( unsigned short index )
{
    ID.setIndex( index );
}

void PlayerState::setID( SocketClient::ID networkid )
{
    ID.setNetworkID(networkid);
}

PlayerID PlayerState::getPlayerID()
{
    return( ID );
}

void PlayerState::setStatus( unsigned char status )
{
    PlayerState::status = status;
}

unsigned char PlayerState::getStatus()
{
    return( status );
}

void PlayerState::setFlag(unsigned char flag)
{
    PlayerState::flag = flag;
}

unsigned char PlayerState::getFlag()
{
    return flag;
}

short PlayerState::getTotal()
{
    return( kill_points - loss_points );
}

NetworkPlayerState PlayerState::getNetworkPlayerState()
{
    return NetworkPlayerState(name, flag, ID.getIndex(), status,
            kills, kill_points, losses, loss_points, total,
            objectives_held, colorIndex);
}

void PlayerState::setFromNetworkPlayerState(NetworkPlayerState* state)
{
    state->name[63] = '\0';
    name = state->name;
    flag = state->flag;
    ID.setIndex(ltoh16(state->playerindex_id));
    status = state->status;
    kills = ltoh16(state->kills);
    kill_points = ltoh16(state->kill_points);
    losses = ltoh16(state->losses);
    loss_points = ltoh16(state->loss_points);
    total = ltoh16(state->total);
    objectives_held = ltoh16(state->objectives_held);
    colorIndex = ltoh32(state->colorIndex);
}
