/*Copyright (C) 1998 Pyrosoft Inc. (www.pyrosoftgames.com), Matthew Bogue
 
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

#ifndef __UI_Font_Manager_hpp__
#define __UI_Font_Manager_hpp__

#include <SDL_ttf.h>

#include <string>
#include <map>

namespace UI{
    class FontManager{

    public:

        FontManager(void);
        virtual ~FontManager(void);

        void loadFont(std::string name, const char * fileName, int ptSize);
        TTF_Font * getFont(const std::string & name);

        private:
        //TODO use a hash_map instead of map. hash_map doesn't seem to be standard
        std::map<std::string, TTF_Font * > fontCollection;
    };
}

#endif
