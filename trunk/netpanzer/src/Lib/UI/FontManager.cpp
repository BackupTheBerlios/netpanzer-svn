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

#include <memory>

#include "FontManager.hpp"
#include "Util/FileSystem.hpp"
#include "Util/Exception.hpp"
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
        try {
	    // XXX memory leak. We need to leave the file open as long as the
	    // font is open. Ugly hack for now
            ReadFile* file (FileSystem::openRead(fileName));
                
            TTF_Font * font = TTF_OpenFontRW(file->getSDLRWOps(), true, ptSize);
            if(font == 0)
                throw Exception("Couldn't load font: %s.", TTF_GetError());
            
            fontCollection[name] = font;
        } catch(std::exception& e) {
            LOGGER.info("Couldn't load font '%s': %s.", fileName, e.what());
        }
    }

    TTF_Font * FontManager::getFont(const std::string & name){
        if(fontCollection.find(name) != fontCollection.end()){
            return fontCollection[name];
        }else{
            return 0;
        }
    }
}
