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

#include "SoundConfig.hpp"
#include "ConfigGetter.hpp"

SoundConfig::SoundConfig()
 :  sound(true),
    music(true),
    musicvol(100),
    effects(true),
    effectsvol(100)
{
    
}

SoundConfig::~SoundConfig()
{
    
}

void SoundConfig::load(const JSONNode& node)
{
    ConfigGetter cg(node);
    
    setSound(cg.getBool("enabled", true));
    setMusic(cg.getBool("music", true));
    setMusicVol(cg.getInt("musicvolume", 0, 100, 100));
    setEffects(cg.getBool("effects", true));
    setEffectsVol(cg.getInt("effectsvolume", 0, 100, 100));
}

void SoundConfig::save(JSONNode& node) const
{
    node.push_back(JSONNode("enable", useSound()));
    node.push_back(JSONNode("music", useMusic()));
    node.push_back(JSONNode("musicvolume", getMusicVol()));
    node.push_back(JSONNode("effects", useEffects()));
    node.push_back(JSONNode("effectsvolume", getEffectsVol()));
}
