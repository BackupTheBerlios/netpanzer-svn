#ifndef __UILIB_SOUND_HPP__
#define __UILIB_SOUND_HPP__

/** This is an interface to sound functions in the game */
class Sound
{
public:
	enum Event{
		_selected, _deselected, _move_to, _target_enemy, _no_line_of_fire,
		_under_attack, _fire_gun, _hit_target, _miss_target, _scream,
		_random_battle, _blow_up_tank
	};
	
	virtual bool initialize() = 0;
	virtual void shutdown() = 0;

	virtual void PlayTankIdle() = 0;
	virtual void StopTankIdle() = 0;
	virtual void PlayMenuSound() = 0;
	virtual void PlayAttackWarning() = 0;
	virtual void PlayPowerUpSound() = 0;
	virtual void PlayUnitSound(int unit_type) = 0;
	virtual void PlayUnitVoice(int unit_type, Event event) = 0;
	virtual void PlayAmbientSound(int unit_type, Event event, long distance)=0;
};

extern Sound* sound;

#endif

