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
#include "EnemyRadarPowerUp.hpp"

#include <stdlib.h>
#include "Structs/UnitTypes.hpp"
#include "Interfaces/UnitInterface.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "Interfaces/MapInterface.hpp"
#include "Interfaces/MiniMapInterface.hpp"
#include "Interfaces/ConsoleInterface.hpp"

#include "Interfaces/Server.hpp"
#include "Classes/Network/NetworkServer.hpp"
#include "Classes/Network/NetworkState.hpp"
#include "Classes/Network/UnitNetMessage.hpp"
#include "Classes/Network/PowerUpNetMessage.hpp"

#include "System/Sound.hpp"


SpritePacked ENEMY_RADAR_POWERUP_ANIM;
SpritePacked ENEMY_RADAR_POWERUP_ANIM_SHADOW;

EnemyRadarPowerUp::EnemyRadarPowerUp(iXY map_loc, int type)
        : PowerUp( map_loc, type )
{
    enemy_radar_animation.setData( ENEMY_RADAR_POWERUP_ANIM );
    enemy_radar_animation.setAttrib( powerup_state.world_loc, iXY(0,0), 5 );

    enemy_radar_animation_shadow.setData( ENEMY_RADAR_POWERUP_ANIM_SHADOW );
    enemy_radar_animation_shadow.setAttrib( powerup_state.world_loc, iXY(0,0), 4 );
    enemy_radar_animation_shadow.setDrawModeBlend(&Palette::colorTableDarkenALot);

}

void
EnemyRadarPowerUp::setRadar(UnitID &unit_id)
{
    sound->playPowerUpSound();

    UnitBase* unit = UnitInterface::getUnit(unit_id);

    if(unit->player == PlayerInterface::getLocalPlayer()) {
        MiniMapInterface::setShowEnemyRadar( 180 );
        ConsoleInterface::postMessage(Color::unitAqua, "YOU GOT AN ENEMY RADAR POWERUP" );
    }

    PowerUpHitMesg hit_mesg;
    hit_mesg.set(powerup_state.ID, unit->player->getID());
    SERVER->sendMessage( &hit_mesg, sizeof( PowerUpHitMesg ));

    powerup_state.life_cycle_state = _power_up_lifecycle_state_inactive;
}

void
EnemyRadarPowerUp::updateState()
{
    UnitID unit_id;

    if ( NetworkState::status == _network_state_server ) {
        if ( powerup_state.life_cycle_state == _power_up_lifecycle_state_active ) {
            if( isPowerUpHit( &unit_id ) == true ) {
                setRadar( unit_id );
            }
        }
    }
}

void
EnemyRadarPowerUp::offloadGraphics( SpriteSorter &sorter )
{
    enemy_radar_animation.nextFrame();
    enemy_radar_animation_shadow.setFrame( enemy_radar_animation.getCurFrame() );

    sorter.addSprite( &enemy_radar_animation );
    sorter.addSprite( &enemy_radar_animation_shadow );

}

void
EnemyRadarPowerUp::onHit( PowerUpHitMesg *message  )
{
    sound->playPowerUpSound();
    PlayerID local_player_id;

    local_player_id = PlayerInterface::getLocalPlayerID();

    if( local_player_id.getIndex() == message->getPlayerID() ) {
        MiniMapInterface::setShowEnemyRadar( 180 );
        ConsoleInterface::postMessage(Color::unitAqua, "YOU GOT AN ENEMY RADAR POWERUP" );
    }

    powerup_state.life_cycle_state = _power_up_lifecycle_state_inactive;
}
