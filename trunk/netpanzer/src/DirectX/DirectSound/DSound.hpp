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
#ifndef __DSOUND_HPP
#define __DSOUND_HPP

#include <windows.h>
#include "mmsystem.h"
#include "mmreg.h"
#include "msacm.h"
#include "dsound.h"

#include "PObject.hpp"
#include "UnitTypes.hpp"
#include "sfx.h"


////EXTERNALS/////////////////////////////////////////////////////
//extern "C" {
//
//	WaveLoadFile(char *,unsigned int *,unsigned long *,struct tWAVEFORMATEX ** ,unsigned char ** );
//
//}

//ENUMERATIONS//////////////////////////////////////////////////
/*
enum UnitType{ _light_tank,
               _medium_tank,
               _heavy_tank,
			   _missle_unit,
			   _humvee,
			   _hovercraft,
			   _commkiller,
			   _refueler,
			   _not_applicable};

enum UnitType{ _unit_type_valentine,
               _unit_type_leopard,
               _unit_type_abrams,
               _unit_type_hammerhead,
               _unit_type_humvee,
               _unit_type_lynx,
               _unit_type_m109,
               _unit_type_spahpanzer,
               _unit_type_scorpion,
               _unit_type_hover_craft,
               _unit_type_comm_killer,
               _unit_type_refueler,
               _unit_type_archer,	
               _not_applicable};
*/
enum Event{ _selected,
			_deselected,
			_move_to, 
			_target_enemy,
			_no_line_of_fire,
			_under_attack,
			_fire_gun,
			_hit_target,
			_miss_target,
			_scream,
			_random_battle,
			_blow_up_tank};


class DirectSound : public PObject
{
protected:

	short RandomCount;
	short BattleSoundIndex;
 short UnitIndex;
 short VoiceIndex;
 short AmbientIndex;
	//short DupBufferIndex;

 long PreviousVolume[MAX_NUM_SFX];

	LPDIRECTSOUND		m_lpDirectSound;

	LPDIRECTSOUNDBUFFER	m_pDSBuffer[MAX_NUM_SFX];
	//LPDIRECTSOUNDBUFFER m_pDSBufferDup1[MAX_NUM_SFX];
	//LPDIRECTSOUNDBUFFER m_pDSBufferDup2[MAX_NUM_SFX];

	DSBUFFERDESC		m_DSBufferDesc[MAX_NUM_SFX];
	DSCAPS				    m_DSCaps;

	WAVEFORMATEX		*m_pWaveFormatEx[MAX_NUM_SFX];


 HRESULT InitDirectSound(HWND hWnd);
	HRESULT InitSoundEngine(HWND hWnd);

	void PlayUnitBuffer(int buffer);
 void StopUnitBuffer(int buffer);
	void PlayVoiceBuffer(int buffer);
	void PlayAmbientBuffer(int buffer, long volume);

 void RestoreBuffer(int buffer);
 //void RestoreDup1(int buffer);
 //void RestoreDup2(int buffer);

	int GetUnitBuffer();
	int GetVoiceBuffer(int unit_type, Event event);
	int GetAmbientBuffer(int unit_type, Event event);
 long GetSoundVolume(long distance);


public:

 BOOL Enabled;

 DirectSound();    

 HRESULT Initialize(HWND hWnd);

 void ShutDownDirectSound();

 void PlayTankIdle();
 void StopTankIdle();
 void PlayMenuSound();
 void PlayAttackWarning();
 void PlayPowerUpSound();
 void PlayUnitSound(int unit_type);
 void PlayUnitVoice(int unit_type, Event event);
 void PlayAmbientSound(int unit_type, Event event, long distance);


};

extern DirectSound dsound;

#endif
