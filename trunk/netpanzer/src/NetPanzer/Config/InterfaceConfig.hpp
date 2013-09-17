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
 * Created on September 15, 2013, 11:00 PM
 */

#ifndef INTERFACECONFIG_HPP
#define	INTERFACECONFIG_HPP

#include "Core/CoreTypes.hpp"

#include "libjson/libjson.h"

class InterfaceConfig
{
public:
    enum { _color_aqua,
       _color_blue,
       _color_dark_blue,
       _color_blue_gray,
       _color_red,
       _color_dark_red,
       _color_light_green,
       _color_green,
       _color_dark_green,
       _color_yellow,
       _color_light_orange,
       _color_orange,
       _color_black,
       _color_white,
       _color_gray,
       _color_dark_gray,
       _color_last
     };
     
     enum { _mini_map_unit_size_small,
       _mini_map_unit_size_large,
       _mini_map_unit_size_last
     };
     
     class MiniMap
     {
        public:
            MiniMap();
            ~MiniMap();
            
            void load(const JSONNode& node);
            void save(JSONNode& node) const;
            
            inline int getPlayerUnitColor() const { return playerunitcolor; }
            inline int getSelectedUnitColor() const { return selectedunitcolor; }
            inline int getAlliedUnitColor() const { return alliedunitcolor; }
            inline int getPlayerOutpostColor() const { return playeroutpostcolor; }
            inline int getAlliedOutpostColor() const { return alliedoutpostcolor; }
            inline int useBigUnitSize() const { return bigunitsize; }
            
            inline void setPlayerUnitColor(const int v) { playerunitcolor = v; }
            inline void setSelectedUnitColor(const int v) { selectedunitcolor = v; }
            inline void setAlliedUnitColor(const int v) { alliedunitcolor = v; }
            inline void setPlayerOutpostColor(const int v) { playeroutpostcolor = v; }
            inline void setAlliedOutpostColor(const int v) { alliedoutpostcolor = v; }
            inline void setBigUnitSize(const int v) { bigunitsize = v; }
            
        private:
            int playerunitcolor;
            int selectedunitcolor;
            int alliedunitcolor;
            int playeroutpostcolor;
            int alliedoutpostcolor;
            int bigunitsize;
     };

    
    InterfaceConfig();
    ~InterfaceConfig();
    
    void load(const JSONNode& node);
    void save(JSONNode& node) const;

    inline bool            showHealth() const { return show_health; }
    inline bool            showFlags() const { return show_flags; }
    inline bool            showNames() const { return show_names; }
    inline int             getAttackNotificationTime() const { return attacknotificationtime; }
    inline int             getVehicleSelectionColor() const { return vehicleselectioncolor; }
    inline int             getUnitInfoLayer() const { return unitinfodrawlayer; }
    inline int             getScrollRate() const { return scrollrate; }
    inline int             getRankPositionX() const { return rankposition_x; }
    inline int             getRankPositionY() const { return rankposition_y; }
    inline const NPString& getLanguage() const { return language; }
    
    inline void setShowHealth(const bool v) { show_health = v; }
    inline void setShowFlags(const bool v) { show_flags = v; }
    inline void setShowNames(const bool v) { show_names = v; }
    inline void setAttackNotificationTime(const int v) { attacknotificationtime = v; }
    inline void setVehicleSelectionColor(const int v) { vehicleselectioncolor = v; }
    inline void setUnitInfoLayer(const int v) { unitinfodrawlayer = v; }
    inline void setScrollRate(const int v) { scrollrate = v; }
    inline void setRankPositionX(const int v) { rankposition_x = v; }
    inline void setRankPositionY(const int v) { rankposition_y = v; }
    inline void setLanguage(const NPString& v) { language = v; }
    
    MiniMap minimap;
    
private:
    InterfaceConfig(const InterfaceConfig&);

    bool      show_health;
    bool      show_flags;
    bool      show_names;
    int       attacknotificationtime;
    int       vehicleselectioncolor;
    int       unitinfodrawlayer;
    int       scrollrate;
    int       rankposition_x;
    int       rankposition_y;
    NPString  language;
    
};

#endif	/* INTERFACECONFIG_HPP */

