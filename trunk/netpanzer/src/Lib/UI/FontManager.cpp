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
#include <config.h>

#include "FontManager.hpp"
#include "Util/Log.hpp"


namespace UI{
    
    FontManager::FontManager(void){
        if(!TTF_WasInit())
            TTF_Init();
    }

    FontManager::~FontManager(void){
        std::map<std::string, TTF_Font * >::iterator i;

        for(i = fontCollection.begin(); i != fontCollection.end(); i++)
            TTF_CloseFont((*i).second);
    }

    void FontManager::loadFont(std::string name, const char * fileName, int ptSize){
        TTF_Font * font = TTF_OpenFont(fileName, ptSize);
        if(!font){
            LOG(("Warning: in FontManager::loadFont : %s", TTF_GetError()));
        }else{
            fontCollection[name] = font;
        }
    }

    TTF_Font * FontManager::getFont(const std::string & name){

        // std::cerr << "getFont(" << name << ")" << std::endl;

        if(fontCollection.find(name) != fontCollection.end()){
            return fontCollection[name];
        }else{
            return 0;
        }
    }
}
