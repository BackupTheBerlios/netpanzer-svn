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

#include "PowerUps/PowerUpInterface.hpp"

#include <stdlib.h>

#include "Util/NTimer.hpp"

#include "Interfaces/GameConfig.hpp"

#include "Interfaces/MapInterface.hpp"
#include "Interfaces/PlayerInterface.hpp"

#include "PowerUps/BonusUnitPowerUp.hpp"
#include "PowerUps/UnitPowerUp.hpp"
#include "PowerUps/EnemyRadarPowerUp.hpp"

#include "Util/Log.hpp"
#include "Classes/Network/NetworkState.hpp"
#include "Classes/Network/NetworkServer.hpp"
#include "Classes/Network/PowerUpNetMessage.hpp"

// 4 minutes in miliseconds
#define TIME_RANGE_TO_REGEN (4 * 60 * 1000)

// 1 minute in miliseconds
#define MINIMUM_TIME_TO_REGEN (1 * 60 * 1000)

static PowerUp* powerups = 0;
static unsigned int num_powerups = 0;
static PowerUpID nextid = 0;

static NTimer regen_timer;

static unsigned int power_up_limit;
static Uint32 power_up_regen_time_upper_bound = TIME_RANGE_TO_REGEN;
static Uint32 power_up_regen_time_lower_bound = MINIMUM_TIME_TO_REGEN;

enum { _powerup_bonus_units,
       _powerup_unit,
       _powerup_enemy_radar
     };

int  powerup_probability_table[3] = { _powerup_unit,
                                      _powerup_bonus_units,
                                      _powerup_enemy_radar
                                    };

static void setPowerUpLimits()
{
    int active_players;

    active_players = PlayerInterface::getActivePlayerCount();

    // Magic Number 0.10 = 1/10 
    int player_factor = int(( 0.10 ) * active_players);
    
    // Magic Number 0.0000625 = 1/16000
    unsigned int power_limit = (unsigned int)( ( 0.0000625 ) * ( (MapInterface::getWidth() * MapInterface::getHeight()) ) );
    power_limit = power_limit + (power_limit * player_factor);

    power_up_limit = power_limit;
    power_up_regen_time_upper_bound =  TIME_RANGE_TO_REGEN;
    power_up_regen_time_lower_bound =  MINIMUM_TIME_TO_REGEN;
}

static PowerUp* addNewPowerUp(const iXY& map_pos, PowerUpID id, int type)
{
    PowerUp* power_up = 0;
    switch( type )
    {
        case _powerup_bonus_units :
            power_up = new BonusUnitPowerUp( map_pos, type );
            break;

        case _powerup_unit :
            power_up = new UnitPowerUp( map_pos, type );
            break;

        case _powerup_enemy_radar :
            power_up = new EnemyRadarPowerUp( map_pos, type );
            break;

        default:
            LOGGER.info("Unknown powerup type?!?");
    }

    if ( power_up )
    {
        power_up->ID = id;
        power_up->next = powerups;
        powerups = power_up;
        ++num_powerups;
    }

    return power_up;
}

static PowerUp* deleteRemovePowerUp(PowerUp* previous_power_up, const PowerUp* power_up)
{
    PowerUp* next_power_up = power_up->next;
    if ( ! previous_power_up )
    {
        powerups = next_power_up;
    }
    else
    {
        previous_power_up->next = next_power_up;
    }

    delete power_up;
    --num_powerups;
    return next_power_up;
}

static void generatePowerUp()
{
    if( num_powerups < power_up_limit )
    {
        iXY loc;
        unsigned int map_size_x = MapInterface::getWidth();
        unsigned int map_size_y = MapInterface::getHeight();

        do
        {
            loc.x = rand() % map_size_x;
            loc.y = rand() % map_size_y;
        } while( MapInterface::getMovementValue( loc ) == 0xFF );

        int powerup_type = powerup_probability_table[ rand() % 3 ];

        PowerUp* power_up = addNewPowerUp(loc, nextid++, powerup_type);

        if ( power_up )
        {
            PowerUpCreateMesg create_mesg;
            create_mesg.set(power_up->map_loc, power_up->ID, power_up->type);
            NetworkServer::broadcastMessage(&create_mesg, sizeof(create_mesg));
        }
        else
        {
            LOGGER.warning("Couldn't create powerup");
        }

        int next_regen_interval = rand() % power_up_regen_time_upper_bound;
        next_regen_interval += power_up_regen_time_lower_bound;

        regen_timer.setTimeOut( next_regen_interval );
        regen_timer.reset();

        setPowerUpLimits();
    }
}

void PowerUpInterface::initialize( void )
{
    PowerUp::POWERUP_ANIM.loadPNGSheet("powerups/Bolt.png", 127, 127, 36);
    PowerUp::POWERUP_ANIM.setColorkey();
    PowerUp::POWERUP_ANIM.setOffsetCenter();
    PowerUp::POWERUP_ANIM.setFPS( 15 );
    Surface t;
    t.loadPNGSheet("powerups/BoltS.png", 127, 127, 36);
    PowerUp::POWERUP_ANIM_SHADOW.createShadow(t);
    PowerUp::POWERUP_ANIM_SHADOW.setOffsetCenter();
}

void PowerUpInterface::resetLogic( void )
{
    PowerUp *del = 0;
    while ( powerups )
    {
        del = powerups;
        powerups = powerups->next;
        delete del;
    }
    num_powerups = 0;

    if( gameconfig->powerups == false )
    {
        return;
    }

    setPowerUpLimits();

    regen_timer.setTimeOut( power_up_regen_time_upper_bound );
    regen_timer.reset();

    if ( NetworkState::status == _network_state_server )
    {
        generatePowerUp();
        generatePowerUp();
        generatePowerUp();
    }
}

void PowerUpInterface::updateState()
{
    if( gameconfig->powerups == false )
    {
        return;
    }

    if ( NetworkState::status == _network_state_server )
    {
        if( regen_timer.isTimeOut() )
        {
            generatePowerUp();
        }
    }

    PowerUp* powerup = powerups;
    PowerUp* previous_power_up = 0;
    while ( powerup )
    {
        if ( powerup->life_cycle_state == _power_up_lifecycle_state_inactive )
        {
            powerup = deleteRemovePowerUp(previous_power_up, powerup);
            // previous remains;
        }
        else
        {
            if ( NetworkState::status == _network_state_server )
            {
                if ( UnitInterface::unitOccupiesLoc(powerup->map_loc) )
                {
                    Unit * unit = UnitBucketArray::queryUnitAtMapLoc(powerup->map_loc);
                    if ( unit )
                    {
                        powerup->onHit( unit );
                    }
                }
            }
            previous_power_up = powerup;
            powerup = powerup->next;
        }
    }
 }

void PowerUpInterface::offloadGraphics( SpriteSorter &sorter )
{
    if( gameconfig->powerups == false )
    {
        return;
    }

    PowerUp* powerup = powerups;
    while ( powerup )
    {
        powerup->sprite.nextFrame();
        powerup->sprite_shadow.setFrame( powerup->sprite.getCurFrame() );

        sorter.addSprite( &powerup->sprite );
        sorter.addSprite( &powerup->sprite_shadow );

        powerup = powerup->next;
    }
}

static void netMessagePowerUpCreate(const NetMessage* message)
{
    PowerUp *power_up = 0;
    PowerUpCreateMesg *create_mesg;

    create_mesg = (PowerUpCreateMesg *) message;

    power_up = addNewPowerUp(iXY(create_mesg->getLocX(), create_mesg->getLocY()),
                             create_mesg->getID(),
                             create_mesg->getType());
}

static void netMessagePowerUpHit(const NetMessage* message)
{
    PowerUpHitMesg *hit_mesg = (PowerUpHitMesg *) message;

    PowerUp* powerup = powerups;
    while ( powerup )
    {
        if ( powerup->ID == hit_mesg->getID() )
        {
            powerup->onHitMessage( hit_mesg );
            break;
        }
        powerup = powerup->next;
    }
}

void PowerUpInterface::processNetMessages(const NetMessage* message )
{
    switch(message->message_id)
    {
        case _net_message_id_powerup_create:
            netMessagePowerUpCreate(message);
            break;

        case _net_message_id_powerup_hit:
            netMessagePowerUpHit(message);
            break;

        default:
            LOGGER.warning("Unknown PowerUpMessage type (id %d-%d)",
                    message->message_class, message->message_id);
            break;
    }
}

void PowerUpInterface::syncPowerUps( ClientSocket * client )
{
    PowerUp* powerup = powerups;
    while ( powerup )
    {
        if ( powerup->life_cycle_state != _power_up_lifecycle_state_inactive )
        {
            PowerUpCreateMesg create_mesg;
            create_mesg.set(powerup->map_loc, powerup->ID, powerup->type);
            client->sendMessage( &create_mesg, sizeof(create_mesg));
        }
        powerup = powerup->next;
    }
}

