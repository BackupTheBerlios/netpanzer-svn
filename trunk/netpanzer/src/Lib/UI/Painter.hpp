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


#ifndef __UI_Painter_hpp__
#define __UI_Painter_hpp__

#include <stack>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "Types/iXY.hpp"
#include "Types/iRect.hpp"

#include "Color.hpp"
#include "FontManager.hpp"

namespace UI {

    class Painter
    {
    public:
        Painter(SDL_Surface* surface, FontManager * fm);
        virtual ~Painter();

        void setSurface(SDL_Surface* surface);

        void setBrushColor(Color color);
        void setFillColor(Color color);

        TTF_Font * getCurrentFont(void);

        void drawLine(iXY from, iXY to);
        void drawRect(iRect rect);
        void fillRect(iRect rect);

        //should be used for dynamic text only.
        //static text should be render on a buffer and blitted
        void drawString(iXY pos, const std::string & s, const std::string & font);

        void drawImage(SDL_Surface* surface, iXY pos);
        //void drawScaledImage(Surface* surface, iRect area);

        // the following 2 functions are used by the window to transform the
        // coordinate system, so that the components can use their normal
        // coordinates
        void pushTransform(iXY translate);

        void popTransform();
        
    private:
        void putPixelAbsolute(size_t x, size_t y, Color color);

        
        class Transform {
        public:
            Transform()
            {}
            
            Transform(const iXY& newtranslation)
                : translation(newtranslation)
            { }
           
            // I think rotation isn't really needed yet...
            iXY translation;

            Transform & operator+=(const Transform & p){
                translation += p.translation;
                return *this;
            }

            Transform & operator-=(const Transform & p){
                translation -= p.translation;
                return *this;
            }

            void apply(iXY& pos)
            {
                pos += translation;
            }

            void apply(iRect& rect)
            {
                rect.min += translation;
                rect.max += translation;
            }
        };
        
        std::stack<Transform> transforms;
        Transform currentTransform;

        Color brushColor;
        Color fillColor;

        FontManager * fontManager;
        
        SDL_Surface * drawingSurface;
    };
}

#endif
