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
#include <windows.h>

#include "DSound.hpp"
#include "sfx.h"
#include "wavfile.h"
#include "wave.h"
#include "stdio.h"

DirectSound dsound;

//defines for volume adjustments--
#define DSBVOLUME_75PERCENT  (long)-500
#define DSBVOLUME_50PERCENT (long)-750
#define DSBVOLUME_25PERCENT (long)-1000
#define DSBVOLUME_5PERCENT   (long)-1500
#define SOUND_TOO_FAR_AWAY  666



DirectSound::DirectSound()
{
 Enabled = false;
}
//////////////////////////////////
//////////////////////////////////


HRESULT DirectSound::Initialize(HWND hWnd)
{
    HRESULT hr;
	short index;

	for(index = 0; index < MAX_NUM_SFX; index++)
	{
		m_pDSBuffer[index] = 0;
  PreviousVolume[index] = DSBVOLUME_MAX;

  /*      if(index >= AMBIENT_START)
        {
		    m_pDSBufferDup1[index] = 0;
		    m_pDSBufferDup2[index] = 0;
        } */

	}

	m_lpDirectSound = 0;
	
	ZeroMemory(&m_DSCaps, sizeof(DSCAPS));

	RandomCount = 0;
	BattleSoundIndex = 0;
 UnitIndex = 0;
 VoiceIndex = 0;
	//DupBufferIndex = 0;
 AmbientIndex = 0;

 hr = InitDirectSound(hWnd);

 if(hr == false)
 {
  Enabled = false;
  return false;
 }

 hr = InitSoundEngine(hWnd);

 if(hr == false)
 {
  Enabled = false;
  return false;
 }

 Enabled = true;
 return true;

}
//////////////////////////////////
//////////////////////////////////


HRESULT DirectSound::InitDirectSound(HWND hWnd)
{

	//here's where i want to create a directsound object--
	HRESULT	hr;
	
	hr = DirectSoundCreate(0, &m_lpDirectSound, 0);

	if(hr != DS_OK)
	{
		if(hr == DSERR_ALLOCATED)
			MessageBox(hWnd, "DirectSound is in use by another application. You can still play netPanzer, but there will be no sound.", "Error", MB_OK);
		
		if((hr == DSERR_INVALIDPARAM) || (hr == DSERR_NOAGGREGATION))
			MessageBox(hWnd, "DirectSound can't initialize. You can still play netPanzer, but there won't be any sound.", "Error", MB_OK);
		
		if(hr == DSERR_NODRIVER)
			MessageBox(hWnd, "There is no DirectSound compatible driver. You can still play netPanzer, but there won't be any sound.", "Error", MB_OK);

		if(hr == DSERR_OUTOFMEMORY)
			MessageBox(hWnd, "Not enough memory to run DirectSound. You can still play netPanzer, but there won't be any sound.", "Error", MB_OK);

		ShutDownDirectSound();
		return false;
	}

	//according to spec, the next thing i have to do is set
	//the 'cooperative level'. Okay--
	hr = m_lpDirectSound -> SetCooperativeLevel( hWnd, DSSCL_NORMAL);

	if(hr != DS_OK)
	{
		MessageBox(hWnd, "DirectSound initialization failed. You can still play netPanzer, but there won't be any sound.", "Error", MB_OK);
  		ShutDownDirectSound();
        return false;
	}

	//i don't have to get device caps, but i will just for the hell
	//of it; besides, i might want to add a button that displays
	//the info--
	m_DSCaps.dwSize = sizeof(DSCAPS);
	hr = m_lpDirectSound -> GetCaps( &m_DSCaps);

	if(hr != DS_OK)
	{
		MessageBox(hWnd, "DirectSound can't get sound card info. You can still play netPanzer, but there won't be any sound.", "Error", MB_OK);
		ShutDownDirectSound();
		return false;
	}

	return true;
}
//////////////////////////////////
//////////////////////////////////


HRESULT DirectSound::InitSoundEngine(HWND hWnd)
{

	//ugh. loop through all the buffers. there are x of them. make
	//the buffer and save its pointer in sfx_pointers_list--
	//char string[20];
	short index;

	HRESULT hr;
	
	//declare all the pointers and variables and set them to 0--
	//for loading the .wav file--
	UINT	cbSize[MAX_NUM_SFX];
	DWORD	cSamples[MAX_NUM_SFX];
	BYTE	*pbData1[MAX_NUM_SFX];

	//for making the secondary buffer call--
	void	*pbData[MAX_NUM_SFX];
	void    *pbData2[MAX_NUM_SFX];
	DWORD   dwLength[MAX_NUM_SFX];
	DWORD   dwLength2[MAX_NUM_SFX];

	//set all these guys to 0--
	for(index = 0; index < MAX_NUM_SFX; index++)
	{
		cbSize[index] = 0;
		cSamples[index] = 0;
		pbData1[index] = 0;

		pbData[index] = 0;
		pbData2[index] = 0;
		dwLength[index] = 0;
		dwLength2[index] = 0;
	}


	//loop through all the sfx--
	for(index = 0; index < MAX_NUM_SFX; index++)
	{
		//get the filename for the specified filename--
		//string = filenames[index];

		//let's load the specified .wav--
		char file_path[80];
        sprintf( file_path, ".\\sfx\\%s", filenames[index] );

        if(WaveLoadFile( file_path, &cbSize[index], &cSamples[index], &m_pWaveFormatEx[index], &pbData1[index]) != 0)
		{
			MessageBox(hWnd, "DirectSound couldn't initialize completely. You can still play netPanzer, but there will be no sound.", "Error", MB_OK);
			ShutDownDirectSound();
			return false;
		}

		//now make a secondary direct sound buffer for it-- 
	    memset(&m_DSBufferDesc[index], 0, sizeof(DSBUFFERDESC));
	    m_DSBufferDesc[index].dwSize = sizeof(DSBUFFERDESC);

	    m_DSBufferDesc[index].dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRLVOLUME;

		m_DSBufferDesc[index].dwBufferBytes = cbSize[index];
	    m_DSBufferDesc[index].lpwfxFormat = m_pWaveFormatEx[index];

		hr = m_lpDirectSound -> CreateSoundBuffer( &m_DSBufferDesc[index], &m_pDSBuffer[index], 0);

		if(hr == DS_OK)
		{
			//now lock this stuff down and copy the memory to it--
			m_pDSBuffer[index]->Lock( 0, cbSize[index], &pbData[index], &dwLength[index], &pbData2[index], &dwLength2[index], 0L);

			memcpy(pbData[index], pbData1[index], cbSize[index]);

		    // Ok, now unlock the buffer--
			m_pDSBuffer[index]->Unlock( pbData[index], cbSize[index], 0, 0);

		    pbData[index] = 0;

			//make duplicate buffers for the ambient sound effects. i have to hard code this
			//for now (less than 16 is not ambient) make sure to change the value in the
			//OnCancel function so that it matches this one (else i'll generate a
			//protection fault--
/*			if(index >= AMBIENT_START )
			{
				hr = m_lpDirectSound -> DuplicateSoundBuffer( m_pDSBuffer[index], &(m_pDSBufferDup1[index]));
				hr = m_lpDirectSound -> DuplicateSoundBuffer( m_pDSBuffer[index], &(m_pDSBufferDup2[index]));

				if(hr != DS_OK)
				{
				MessageBox(hWnd, "DirectSound couldn't initialize completely. You can still play netPanzer, but there will be no sound.", "Error", MB_OK);
				ShutDownDirectSound();
				return false;
				}

			}
	*/		

		}
		else
		{
			MessageBox(hWnd, "DirectSound couldn't initialize completely. You can still play netPanzer, but there will be no sound.", "Error", MB_OK);
			ShutDownDirectSound();
			return false;
		} 
	}

	//Do the distance sfx buffers here--


	return true;

}
//////////////////////////////////
//////////////////////////////////





void DirectSound::ShutDownDirectSound()
{
	//release the direct sound buffers--
	short index;
	
	for(index = 0; index < MAX_NUM_SFX; index++)
	{
		if(m_pDSBuffer[index] != 0)
		{
			m_pDSBuffer[index] -> Release();
			m_pDSBuffer[index] = 0;
		}

		//this if statement needs to match the one in
		//the initialize function--
/*		if( index >= AMBIENT_START)
		{
			if(m_pDSBufferDup1[index] != 0) m_pDSBufferDup1[index] = 0;
			if(m_pDSBufferDup2[index] != 0) m_pDSBufferDup2[index] = 0;
		}
*/
	}

/*	if(m_pDSDistantSFX1 != 0)
	{
		m_pDSDistantSFX1 -> Release();
		m_pDSDistantSFX1 = 0;
	}
	if(m_pDSDistantSFX2 != 0)
	{
		m_pDSDistantSFX2 -> Release();
		m_pDSDistantSFX2 = 0;
	}
	if(m_pDSDistantSFX3 != 0)
	{
		m_pDSDistantSFX3 -> Release();
		m_pDSDistantSFX3 = 0;
	}
	*/

}
//////////////////////////////////
//////////////////////////////////


void DirectSound::PlayTankIdle()
{
    HRESULT hr;
    
    if ( Enabled == false )
     return;
    
    if(m_pDSBuffer[0] != 0)
    {
        hr = m_pDSBuffer[0] -> Play(0,0,DSBPLAY_LOOPING);
        if(hr == DSERR_BUFFERLOST) m_pDSBuffer[0] = 0;
    }

}
//////////////////////////////////
//////////////////////////////////


void DirectSound::StopTankIdle()
{

 if ( Enabled == false )
  return;

   StopUnitBuffer(0);

}
//////////////////////////////////
//////////////////////////////////


void DirectSound::PlayMenuSound()
{
    HRESULT hr;
    
    if ( Enabled == false )
     return;
    
    if(m_pDSBuffer[1] != 0)
    {
        hr = m_pDSBuffer[1] -> Play(0,0,0);
        if(hr == DSERR_BUFFERLOST) m_pDSBuffer[1] = 0;
    }

}
//////////////////////////////////
//////////////////////////////////


void DirectSound::PlayAttackWarning()
{
    HRESULT hr;
    
    if ( Enabled == false )
     return;
    
    if(m_pDSBuffer[2] != 0)
    {
        hr = m_pDSBuffer[2] -> Play(0,0,0);
        if(hr == DSERR_BUFFERLOST) m_pDSBuffer[2] = 0;
    }

}

void DirectSound::PlayPowerUpSound()
 {
    HRESULT hr;
    
    if ( Enabled == false )
     return;
    
    
   if(m_pDSBuffer[_sfx_powerup] != 0)
    {
     hr = m_pDSBuffer[_sfx_powerup] -> Play(0,0,0);
     if(hr == DSERR_BUFFERLOST) m_pDSBuffer[_sfx_powerup] = 0;
    }

 }
//////////////////////////////////
//////////////////////////////////



void DirectSound::PlayUnitSound(int unit_type)
{
	int buffer;

    if ( Enabled == false )
     return;

    if((unit_type == _unit_type_humvee) || (unit_type == _unit_type_hover_craft)) return;
 
    buffer = GetUnitBuffer();
    if((buffer < 0) || (buffer >= MAX_NUM_SFX)) return;
    else PlayUnitBuffer(buffer);


}
//////////////////////////////////
//////////////////////////////////


void DirectSound::PlayUnitVoice(int unit_type, Event event)
{
	int buffer;

    if ( Enabled == false )
     return;

    buffer = GetVoiceBuffer(unit_type, event);

    if((buffer < 0) || (buffer >= MAX_NUM_SFX)) return;
    else PlayVoiceBuffer(buffer);

}
//////////////////////////////////
//////////////////////////////////


void DirectSound::PlayAmbientSound(int unit_type, Event event, long distance)
{
	int buffer;
 long volume;

    if ( Enabled == false )
     return;

    buffer = GetAmbientBuffer(unit_type, event);
    volume = GetSoundVolume(distance);

    if( volume == SOUND_TOO_FAR_AWAY) return;

    if((buffer < 0) || (buffer >= MAX_NUM_SFX)) return;
    else PlayAmbientBuffer(buffer, volume);


}
//////////////////////////////////
//////////////////////////////////


void DirectSound::PlayUnitBuffer(int buffer)
{
    HRESULT hr;

    if((buffer < 0) || (buffer >= MAX_NUM_SFX)) return;
    if(m_pDSBuffer[buffer] != 0)
    {
        hr = m_pDSBuffer[buffer] -> Play(0,0,0);
        if(hr == DSERR_BUFFERLOST) m_pDSBuffer[buffer] = 0;
    }

}
//////////////////////////////////
//////////////////////////////////


void DirectSound::StopUnitBuffer(int buffer)
{

    if((buffer < 0) || (buffer >= MAX_NUM_SFX)) return;
    if(m_pDSBuffer[buffer] != 0)
    {
        m_pDSBuffer[buffer]-> Stop();
        m_pDSBuffer[buffer]-> SetCurrentPosition(0);
    }

}
//////////////////////////////////
//////////////////////////////////


void DirectSound::PlayVoiceBuffer(int buffer)
{
    HRESULT hr;

    if((buffer < 0) || (buffer >= MAX_NUM_SFX)) return;
    if(m_pDSBuffer[buffer] != 0)
    {
        hr = m_pDSBuffer[buffer] -> Play(0,0,0);
        if(hr == DSERR_BUFFERLOST) m_pDSBuffer[buffer] = 0;
    }

}
//////////////////////////////////
//////////////////////////////////


void DirectSound::PlayAmbientBuffer(int buffer, long volume)
{
    HRESULT hr;

    if((buffer < 0) || (buffer >= MAX_NUM_SFX)) return;


//	if(DupBufferIndex == 0)
//	{
        if(m_pDSBuffer[buffer] != 0)
        {

         if( PreviousVolume[buffer] != volume)
         {
           hr = m_pDSBuffer[buffer] -> SetVolume( volume);

           if( hr == DS_OK) PreviousVolume[buffer] = volume;
         }


		       hr = m_pDSBuffer[buffer] -> Play(0,0,0);
         if(hr == DSERR_BUFFERLOST)
         {
           m_pDSBuffer[buffer] = 0;
           //m_pDSBufferDup1[buffer] = 0;
           //m_pDSBufferDup2[buffer] = 0;
         }
        }//if m_pDSBuffer[buffer] != 0

// }//if DupBufferIndex == 0

//		DupBufferIndex = 1;
//	}
/*	else
	if(DupBufferIndex == 1)
	{
        if(m_pDSBufferDup1[buffer] != 0)
        {
		    hr = m_pDSBufferDup1[buffer] -> Play(0,0,0);

        }

		DupBufferIndex = 2;
	}
	else
    if(DupBufferIndex == 2)
	{
        if(m_pDSBufferDup2[buffer] != 0)
        {
		    hr = m_pDSBufferDup2[buffer] -> Play(0,0,0);
        }

		DupBufferIndex = 0;
	}
    else DupBufferIndex = 0;
*/
}
//////////////////////////////////
//////////////////////////////////


void DirectSound::RestoreBuffer(int buffer)
{
    HRESULT hr;
	UINT	cbSize = 0;
	DWORD	cSamples = 0;
	BYTE	*pbData1 = 0;

	void	*pbData = 0;
	void    *pbData2 = 0;
	DWORD   dwLength = 0;
	DWORD   dwLength2 = 0;


    hr = m_pDSBuffer[buffer] -> Restore();
    if(hr != DS_OK) m_pDSBuffer[buffer] = 0;

    //load the wave file--
    if(WaveLoadFile( filenames[buffer], &cbSize, &cSamples, &m_pWaveFormatEx[buffer], &pbData1) != 0)
        m_pDSBuffer[buffer] = 0;

    hr = m_pDSBuffer[buffer] -> Lock( 0, cbSize, &pbData, &dwLength, &pbData2, &dwLength2, 0L);
    if(hr != DS_OK) m_pDSBuffer[buffer] = 0;
    
    memcpy(pbData, pbData1, cbSize);

    hr = m_pDSBuffer[buffer]->Unlock( pbData, cbSize, 0, 0);
    if(hr != DS_OK) m_pDSBuffer[buffer] = 0;

    pbData = 0;

}
//////////////////////////////////
//////////////////////////////////

/*
void DirectSound::RestoreDup1(int buffer)
{
    HRESULT hr;

    hr = m_pDSBufferDup1[buffer] -> Restore();
    if(hr != DS_OK) m_pDSBufferDup1[buffer] = 0;

}
//////////////////////////////////
//////////////////////////////////


void DirectSound::RestoreDup2(int buffer)
{
    HRESULT hr;

    hr = m_pDSBufferDup2[buffer] -> Restore();
    if(hr != DS_OK) m_pDSBufferDup2[buffer] = 0;

}
//////////////////////////////////
//////////////////////////////////
*/

int DirectSound::GetUnitBuffer()
{
    if(UnitIndex > 3) UnitIndex = 0;
    
    if(UnitIndex == 0)
    {
        UnitIndex++;
        return _multi;
    }

    if(UnitIndex == 1)
    {
        UnitIndex++;
        return _heavy;
    }

    if(UnitIndex == 2)
    {
        UnitIndex++;
        return _medium;
    }

    if(UnitIndex == 3)
    {
        UnitIndex++;
        return _light;
    }

    return -1;
}
//////////////////////////////////
//////////////////////////////////


int DirectSound::GetVoiceBuffer(int unit_type, Event event)
{

	switch(unit_type)
	{
		case _unit_type_abrams: //heavy tank
			if(event == _selected)
			{
				if(VoiceIndex > 2) VoiceIndex = 0;
				else VoiceIndex++;
				
				if(VoiceIndex == 0) return _ht_reporting;
    if(VoiceIndex == 1) return _ht_awaiting;
				else return _ht_ready;
			}
            
            break;

         case _unit_type_leopard:
			if(event == _selected)
			{
				if(VoiceIndex > 2) VoiceIndex = 0;
				else VoiceIndex++;
				
				if(VoiceIndex == 0) return _mt_reporting;
    if(VoiceIndex == 1) return _mt_awaiting;
				else return _mt_ready;
			}
            
            break;
		
		case _unit_type_valentine: //light tank
			if(event == _selected)
			{
				if(VoiceIndex > 2) VoiceIndex = 0;
				else VoiceIndex++;
				
				if(VoiceIndex == 0) return _lt_reporting;
    if(VoiceIndex == 1) return _lt_awaiting;
				else return _lt_ready;
			}
            
            break;

		case _unit_type_hammerhead: //missle unit
			if(event == _selected)
			{
				if(VoiceIndex > 2) VoiceIndex = 0;
				else VoiceIndex++;
				
				if(VoiceIndex == 0) return _mu_reporting;
    if(VoiceIndex == 1) return _mu_awaiting;
				else return _mu_ready;
			}
            
            break;

		case _unit_type_humvee: //humvee
			if(event == _selected)
			{
				if(VoiceIndex > 2) VoiceIndex = 0;
				else VoiceIndex++;
				
				if(VoiceIndex == 0) return _yes_sir;
    if(VoiceIndex == 1) return _yes_sir;
				else return _yes_sir;
			}
            
            break;
/*
		case _unit_type_hover_craft: //hovercraft
			if(event == _selected)
			{
				if(VoiceIndex > 2) VoiceIndex = 0;
				else VoiceIndex++;
				
				if(VoiceIndex == 0) return _ht_reporting;
    if(VoiceIndex == 1) return _ht_awaiting;
				else return _ht_ready;
			}
            
            break;

		case _commkiller: //commkiller
			if(event == _selected)
			{
				if(VoiceIndex > 2) VoiceIndex = 0;
				else VoiceIndex++;
				
				if(VoiceIndex == 0) return _ht_reporting;
    if(VoiceIndex == 1) return _ht_awaiting;
				else return _ht_ready;
			}
            
            break;

		case _refueler: //refueler
			if(event == _selected)
			{
				if(VoiceIndex > 2) VoiceIndex = 0;
				else VoiceIndex++;
				
				if(VoiceIndex == 0) return _ht_reporting;
    if(VoiceIndex == 1) return _ht_awaiting;
				else return _ht_ready;
			}
            
            break;
*/
		default:

		break;

	}

    //catch non-selection events, which are the same for all
    //vehicles--
    if(event == _move_to)
    {
    	if(VoiceIndex > 2) VoiceIndex = 0;
		else VoiceIndex++;

        if(VoiceIndex == 0) return _moving_out;
        if(VoiceIndex == 1) return _affirmative;
		else return _yes_sir;
    }

    if(event == _target_enemy)
    {
	    if(VoiceIndex > 2) VoiceIndex = 0;
	    else VoiceIndex++;

        if(VoiceIndex == 0) return _affirmative;
        if(VoiceIndex == 1) return _yes_sir;
		else return _target_confirmed;
    }


	return -1;
}
//////////////////////////////////
//////////////////////////////////


int DirectSound::GetAmbientBuffer(int unit_type, Event event)
{

	switch(event)
	{

	case _fire_gun: //_fire_gun
		if(unit_type == _unit_type_abrams) return _ht_fire;
		if(unit_type == _unit_type_leopard) return _mt_fire;
		if(unit_type == _unit_type_valentine) return _lt_fire;
		if(unit_type == _unit_type_hammerhead) return _mu_fire;
		if(unit_type == _unit_type_humvee) return _lt_fire;
		if(unit_type == _unit_type_lynx) return _lt_fire;
		if(unit_type == _unit_type_m109) return _ht_fire;
		if(unit_type == _unit_type_spahpanzer) return _mt_fire;
		if(unit_type == _unit_type_scorpion) return _lt_fire;
		if(unit_type == _unit_type_archer) return _mu_fire;
		if(unit_type == _unit_type_hover_craft) return _ho_fire;
		break;
		
	case _hit_target: //_hit_target

		if(AmbientIndex > 5) AmbientIndex = 0;
		else AmbientIndex++;

		if(AmbientIndex == 0) return _hit1;
		if(AmbientIndex == 1) return _hit2;
		if(AmbientIndex == 2) return _hit3;
		if(AmbientIndex == 3) return _hit4;
		if(AmbientIndex == 4) return _hit5;
		if(AmbientIndex == 5) return _hit6;
		break;

	case _miss_target: //_miss_target

		break;

	case _scream: //_scream
		if(AmbientIndex > 5) AmbientIndex = 0;
		else AmbientIndex++;

		if(AmbientIndex == 0) return _scream1;
		if(AmbientIndex == 1) return _scream2;
		if(AmbientIndex == 2) return _oh_god_oh_god;
		if(AmbientIndex == 3) return _scream2;
		if(AmbientIndex == 4) return _scream1;
		if(AmbientIndex == 5) return _scream2;
		break;

	case _random_battle: //_random_battle
		if(RandomCount > 15)
		{
			if(BattleSoundIndex > 5) BattleSoundIndex = 0;
			else BattleSoundIndex++;

			if(BattleSoundIndex == 0)
            {
                RandomCount = 0;
                return _mg1;
            }
			if(BattleSoundIndex == 1)
            {
                RandomCount = 0;
                return _shell1;
            }
			if(BattleSoundIndex == 2)
            {
                RandomCount = 0;
                return _shell1;
            }
			if(BattleSoundIndex == 3)
            {
                RandomCount = 0;
                return _shell1;
            }
			if(BattleSoundIndex == 4)
            {
                RandomCount = 0;
                return _mg1;
            }
			if(BattleSoundIndex == 5)
            {
                RandomCount = 0;
                return _shell1;
            }

			RandomCount = 0;
		}
		else RandomCount++;
		
		break;

	case _blow_up_tank: //_blow_up_tank
		if(AmbientIndex > 5) AmbientIndex = 0;
		else AmbientIndex++;

		if(AmbientIndex == 0) return _explosion1;
		if(AmbientIndex == 1) return _explosion2;
		if(AmbientIndex == 2) return _explosion3;
		if(AmbientIndex == 3) return _explosion4;
		if(AmbientIndex == 4) return _explosion5;
		if(AmbientIndex == 5) return _explosion6;

		break;

	default:
		break;

	}

	//if we get to here no valid sound was returned so make sure
	//and return a -1 so nothing gets played--
	return -1;
}
//////////////////////////////////
//////////////////////////////////


long DirectSound::GetSoundVolume(long distance)
{

  if( distance < 0) distance = 0;

  //0 to 2 800x600 screen widths away--
  if( (distance < 640000) && (distance >= 0)) return DSBVOLUME_MAX;

  //2 to 4 800x600 screen widths away--
  if( (distance < 10240000) && (distance > 640000)) return DSBVOLUME_75PERCENT;

  //4 to 8 800x600 screen widths away--
  if( (distance < 40960000) && (distance > 10240000)) return DSBVOLUME_50PERCENT;

  //8 to 12 800x600 screen widths away--
  if( (distance < 92760000) && (distance > 40960000)) return DSBVOLUME_25PERCENT;

  //12 to 16 800x600 screen widths away--
  if( (distance < 163840000) && (distance > 92760000)) return DSBVOLUME_5PERCENT;

  //anything further away--
  return SOUND_TOO_FAR_AWAY;

  
}
//////////////////////////////////
//////////////////////////////////
