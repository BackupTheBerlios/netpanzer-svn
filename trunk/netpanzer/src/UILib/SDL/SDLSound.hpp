#ifndef __UILIB_SDLSOUND_HPP__
#define __UILIB_SDLSOUND_HPP__

#include "UILib/Sound.hpp"

class SDLSound : public Sound
{
public:
	SDLSound();
	virtual ~SDLSound();

	virtual bool initialize();
	virtual void shutdown();
                                                                            
	virtual void PlayTankIdle();
	virtual void StopTankIdle();
	virtual void PlayMenuSound();
	virtual void PlayAttackWarning();
	virtual void PlayPowerUpSound();
	virtual void PlayUnitSound(int unit_type);
	virtual void PlayUnitVoice(int unit_type, Event event);
	virtual void PlayAmbientSound(int unit_type, Event event, long distance);
};

#endif

