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
#ifndef _PLAYERSTATE_HPP
#define _PLAYERSTATE_HPP

#include <string.h>
#include "PObject.hpp"
#include "PlayerID.hpp"
#include "PlayerUnitConfig.hpp"




enum { _player_state_free,
       _player_state_allocated,
       _player_state_connecting,
       _player_state_active
     };

class PlayerState : public PObject
 {
  protected:
   char name[64];
   unsigned char flag;
   PlayerID ID;
   unsigned char status;
   short kills;
   short kill_points;
   short losses;
   short loss_points;
   short total;
   short objectives_held;
   bool stats_locked;

  public:
   PlayerUnitConfig unit_config;

   void setName( const char *player_name )
    {
     strcpy( name, player_name );
    }

   void setID( PlayerID player_id )
    {
     ID = player_id;
    }

   void resetStats( void )
    {
     kills  = 0;
     kill_points = 0;
     losses = 0;
     loss_points = 0;
     total = 0;
     objectives_held = 0;
     stats_locked = false;
    }

   inline char * getName( void )
    {
     return( name );
    }

   inline void lockStats( void ) 
    { stats_locked = true; }

   inline void unlockStats( void ) 
    { stats_locked = false; }
   
   inline short getKills( void )
    { return( kills ); }

   inline short getLosses( void )
    { return( losses ); }
 
   inline void incKills( unsigned short unit_type )
    { 
     if ( stats_locked == true ) return;
     kills++; 
     kill_points += 2 * unit_config.getUnitPointValue( (unsigned char)  unit_type ) ;
    } 

   inline void decKills( unsigned short unit_type )
    { 
     if ( stats_locked == true ) return;
     kills--; 
    }
   
   inline void incLosses( unsigned short unit_type )
    { 
     if ( stats_locked == true ) return;
     losses++; 
     loss_points += unit_config.getUnitPointValue( (unsigned char) unit_type );
    }

   inline void decLosses( unsigned short unit_type )
    { 
     if ( stats_locked == true ) return;
     losses--; 
    }
 
   inline void incObjectivesHeld( void )
    { 
     if ( stats_locked == true ) return;
     objectives_held++; 
    }

   inline void decObjectivesHeld( void )
    { 
     if ( stats_locked == true ) return;
     objectives_held++; 
    }

   inline short getObjectivesHeld( void )
    { 
     return( objectives_held ); 
    }
   
   inline void setObjectivesHeld( short objectives )
    { 
     if ( stats_locked == true ) return;
     objectives_held = objectives; 
    }
   
   inline void setID( unsigned short index, DPID dpID  )
     {
      ID.set( index, dpID );
     }

   inline void setID( unsigned short index )
    {
     ID.setIndex( index );
    }
  
   inline void setID( DPID dpID )
    {
     ID.setDPID( dpID );
    }

   inline PlayerID getPlayerID( void )
    {
     return( ID );
    } 

   inline void setStatus( unsigned char status )
    {
     PlayerState::status = status;
    }

   inline unsigned char getStatus( void )
    {
     return( status );
    }

   inline void setFlag(unsigned char flag)
    {
     PlayerState::flag = flag;
    }
   
   inline unsigned char getFlag()
    {
     return flag;
    }   

   inline short getTotal()
    {
     return( kill_points - loss_points );
    }
 
 };

#endif // ** _PLAYERSTATE_HPP
