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
#include "json/json.h"

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

void SoundConfig::load(const Json::Value& node)
{
    setSound(node.get("enabled", true).asBool());
    setMusic(node.get("music", true).asBool());
    setMusicVol(node.get("musicvolume", 100).asInt());
    setEffects(node.get("effects", true).asBool());
    setEffectsVol(node.get("effectsvolume", 100).asInt());
}

void SoundConfig::save(Json::Value& node) const
{
    node["enable"] = useSound();
    node["music"] = useMusic();
    node["musicvolume"] = getMusicVol();
    node["effecs"] = useEffects();
    node["effectsvolume"] = getEffectsVol();
}
