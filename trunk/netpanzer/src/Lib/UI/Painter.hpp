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

#include "2D/RGBColor.hpp"
#include "2D/Surface.hpp"
#include "Types/iXY.hpp"

namespace UI {


    class Painter
    {
    public:
        Painter();
        virtual ~Painter();

        void setSurface(Surface * surface){
            drawingSurface = surface;
        }

        void setBrushColor(RGBColor color);
        void setFillColor(RGBColor color);

        void drawLine(iXY from, iXY to);
        void drawRect(iRect rect);
        void fillRect(iRect rect);
        //void drawImage(Surface* surface, iXY pos);
        //void drawScaledImage(Surface* surface, iRect area);

        // the following 2 functions are used by the window to transform the
        // coordinate system, so that the components can use their normal
        // coordinates
        void pushTransform(iXY translate);

        void popTransform();
        
    private:
        class Transform {
        public:
            iXY translation;
            // I think rotation isn't really needed yet...
            Transform & operator+=(const Transform & p){
                translation += p.translation;
                return *this;
            }

            Transform & operator-=(const Transform & p){
                translation -= p.translation;
                return *this;
            }
        };
        
        std::stack<Transform> transforms;
        Transform currentTransform;

        PIX brushColor;
        PIX fillColor;
        
        Surface * drawingSurface;

    };

}
#endif
