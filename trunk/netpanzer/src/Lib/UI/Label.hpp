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

#include "Component.hpp"
#include "FontManager.hpp"
#include "2D/Color.hpp"
#include "2D/Palette.hpp"

#include <string>

#ifndef __UI_Label_hpp__
#define __UI_Label_hpp__

namespace UI{

    typedef enum
    {
        TOP=1,
        V_CENTER=2,
        BOTTOM=4} VERTICAL_ALIGN;

    typedef enum
    {
        LEFT=8,
        H_CENTER=16,
        RIGHT=32} HORIZONTAL_ALIGN;
   

    class Label : public Component{
    protected:
        std::string text;
        int alignment;
        SDL_Surface * textSurface;
        FontManager * fontManager;
        SDL_Color textColor;
        iXY textPosition;
        

        void updateTextSurface(void);
        SDL_Color transformColor(Color c);

    public:
        Label(std::string text, iRect area, FontManager *fm, int alignment = TOP|LEFT): Component(area){
            fontManager = fm;
            this->alignment = alignment;
            textColor = transformColor(::Color::white);
            textSurface = 0;
            setText(text);
        }

        void setText(std::string text);
        const std::string &  getText(void) const;

        void setTextColor(Color col);

        void setAlignment(int align){
            alignment = align;
            updateTextSurface();
        }

        void draw(Painter & painter);
    };
}
        
#endif
