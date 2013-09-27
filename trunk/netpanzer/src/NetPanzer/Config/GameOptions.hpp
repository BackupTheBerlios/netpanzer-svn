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
 * Created on September 19, 2013, 10:22 AM
 */

#ifndef GAMEOPTIONS_HPP
#define	GAMEOPTIONS_HPP

class GameOptions
{
public:
    class Outposts
    {
        public:
            Outposts();
            ~Outposts();
            
            inline bool enabled() const { return enable; }
            inline int getCaptureMode() const { return captureMode; }
            inline int getPlayerLimit() const { return playerLimit; }
            
            inline void setEnabled(const bool v) { enable = v; }
            inline void setCaptureMode(const int v) { captureMode = v; }
            inline void setPlayerLimit(const int v) { playerLimit = v; }
            
        private:
            bool enable;
            int captureMode; // 0=no capture, 1= normal, 2= new
            int playerLimit; // 0 = nolimit, else max bases per player
                
    };
    
    class Units
    {
        
    };
    
    GameOptions();
    ~GameOptions();
    
    Outposts outposts;
    
    
private:
    GameOptions(const GameOptions&);
    
    

};

#endif	/* GAMEOPTIONS_HPP */

