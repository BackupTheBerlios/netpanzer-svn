/*
Copyright (C) 2013 Netpanzer Team. (www.netpanzer.org), Aaron Perez

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
 *
 * Created on September 15, 2013, 11:59 PM
 */

#ifndef SOUNDCONFIG_HPP
#define	SOUNDCONFIG_HPP

#include "libjson/libjson.h"

class SoundConfig {
public:
    SoundConfig();
    ~SoundConfig();
    
    void load(const JSONNode& node);
    void save(JSONNode& node) const;
    
    inline bool useSound() const { return sound; }
    inline bool useMusic() const { return music; }
    inline unsigned getMusicVol() const { return musicvol; }
    inline bool useEffects() const { return effects; }
    inline unsigned getEffectsVol() const { return effectsvol; }
    
    inline void setSound(const bool v) { sound= v; }
    inline void setMusic(const bool v) { music= v; }
    inline void setMusicVol(const unsigned v) { musicvol= v; }
    inline void setEffects(const bool v) { effects= v; }
    inline void setEffectsVol(const unsigned v) { effectsvol= v; }
    
private:
    SoundConfig(const SoundConfig& );
    
    bool sound;
    bool music;
    unsigned musicvol;
    bool effects;
    unsigned effectsvol;

};

#endif	/* SOUNDCONFIG_HPP */

