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

#include "Views/Components/Button.hpp"
#include "2D/Color.hpp"
#include "2D/Palette.hpp"
#include "Util/Log.hpp"
#include "Util/FileSystem.hpp"
#include "Util/Exception.hpp"

#include "SDL_image.h"

#include <string>
#include <memory>

namespace UI
{
    Button::Button(const char* backgroundFileNameUp,
            const char* backgroundFileNameDown, const std::string& text,
            iRect area, FontManager* fm)
        : Component(area)
    {
        initialize(backgroundFileNameUp, backgroundFileNameDown);
        textLabel = new Label(text, area, fm, V_CENTER|H_CENTER);
        setName(text);
    }

    Button::Button(const char* backgroundFileNameUp,
            const char* backgroundFileNameDown, iRect area, FontManager * fm)
        : Component(area)
    {
        (void) fm;
        initialize(backgroundFileNameUp, backgroundFileNameDown);
        textLabel = 0;
        setName("unknown");
    }

    Button::~Button(void){
        if(bgSurfaceUp)
            SDL_FreeSurface(bgSurfaceUp);
        if(bgSurfaceDown)
            SDL_FreeSurface(bgSurfaceDown);
                
    }

    void Button::initialize(const char * backgroundFileNameUp, const char * backgroundFileNameDown){
        clickState = false;
        try{
            filesystem::ReadFile* fileDown 
                = filesystem::openRead(backgroundFileNameDown);
            SDL_RWops * t = fileDown->getSDLRWOps();
            bgSurfaceDown = IMG_Load_RW(t, false);
            free(t);
            if(bgSurfaceDown == 0)
                throw Exception("Couldn't load image: %s.", IMG_GetError());
            delete fileDown;
        }catch(std::exception& e) {
            LOGGER.info("Couldn't load image '%s': %s.", backgroundFileNameDown, e.what());
            bgSurfaceDown = 0;
        }

        try{
            filesystem::ReadFile* fileUp 
                = filesystem::openRead(backgroundFileNameUp);
            SDL_RWops * t = fileUp->getSDLRWOps();
            bgSurfaceUp = IMG_Load_RW(t, false);
            free(t);
            if(bgSurfaceUp == 0)
                throw Exception("Couldn't load image: %s.", IMG_GetError());
            delete fileUp;
        }catch(std::exception& e) {
            LOGGER.info("Couldn't load image '%s': %s.", backgroundFileNameUp, e.what());
            bgSurfaceUp = 0;
        }
    }

    void Button::draw(Painter & painter)
    {
        //LOG(("DrawButton"));
        painter.setBrushColor(::Color::white);
        if(!clickState && bgSurfaceUp)
            painter.drawImage(bgSurfaceUp, area.min);
        else if(clickState && bgSurfaceDown)
            painter.drawImage(bgSurfaceDown, area.min);

        if(textLabel)
            textLabel->draw(painter);
    }

    void Button::mouseEntered(MouseEventParameter param) {
        (void) param;
        // LOG(("%s : Mouse Entered", getName().c_str()));
    }


    void Button::mouseExited(MouseEventParameter param) {
        (void) param;
        //LOG(("%s : Mouse Exited", getName().c_str()));
        clickState = false;
    }
    
    void Button::mousePressed(MouseEventParameter param){
        //LOG(("%s : Mouse pressed", getName().c_str()));
        if(param.getButtons() & SDL_BUTTON_LEFT)
            clickState = true;

    }
    void Button::mouseReleased(MouseEventParameter param){
        if(clickState){
            clickState = false;
            //LOG(("%s : Click", getName().c_str()));
            std::list<ButtonCallback *>::iterator i;
            for(i = callbacks.begin(); i != callbacks.end(); i++)
                (*i)->buttonPressed(param, this);
        }
    }
}
