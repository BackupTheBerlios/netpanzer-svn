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
#ifndef __SFX_H
#define __SFX_H

#define		MAX_NUM_SFX		47
#define  AMBIENT_START   24


//this enum is used to index the correct pointer to the secondary
//direct sound buffer that's supposed to be played. these constants
//have to by in synch with the ORDER the source wave files are
//loaded or the system might play the wrong sounds--
enum SoundBuffer {
  //special sounds
  _tanks_idle,
  _menu_sound,
  _attack_warning,
  _heavy,
  _medium,
  _light,
  _multi,
  //voice response
		_ht_reporting,
  _ht_awaiting,
		_ht_ready,
		_mt_reporting,
  _mt_awaiting,
		_mt_ready,
		_lt_reporting,
  _lt_awaiting,
		_lt_ready,
		_mu_reporting,
  _mu_awaiting,
		_mu_ready,
//		_hv_reporting,
//		_hv_ready,
//		_ho_reporting,
//		_ho_ready,
//		_rt_reporting,
//		_rt_ready,
		_moving_out,
		_yes_sir,
		_affirmative,
		_target_confirmed,
//		_target_obscured,
//		_no_target_sir,
//		_taking_heavy_fire,
//		_under_attack,
//		_taking_hits,
//		_enemy_vehicles,
//		_take_it_easy,
		_scream1,
		_scream2,
		_oh_god_oh_god,
//		_background,
		//vehicles firing
		_ht_fire,
		_mt_fire,
		_lt_fire,
		_mu_fire,
		_hv_fire,
		_ho_fire,
		//non-lethal hits
		_hit1,
		_hit2,
		_hit3,
		_hit4,
		_hit5,
		_hit6,
		//explosions
		_explosion1,
		_explosion2,
		_explosion3,
		_explosion4,
		_explosion5,
		_explosion6,
		//misses
		//shells in flight
		_shell1,
//		_shell2,
//		_shell3,
		//heavy machine guns
		_mg1,
//		_mg2,
//		_mg3
		//distant battle
//		_db1,
//		_db2,
//		_db3,
        _sfx_powerup
};


#endif
