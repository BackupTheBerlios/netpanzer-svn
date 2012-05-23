/*
Copyright (C) 2012 Netpanzer Team. (www.netpanzer.org), Laurent Jacques

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

#ifndef __tButton_hpp__
#define __tButton_hpp__

#include "Component.hpp"
#include "2D/Color.hpp"

typedef enum
{
    NORMAL =   0,
    OVER   =   1,
    PRESSED=   2,
    DISABLE=   3
} BState;

//--------------------------------------------------------------------------
class tButton : public Component
{
protected:
    std::string label;
    PIX textColors[4];
    Surface bimage;
    BState bstate;
    void render();
    void resetState()
    {
        bstate = NORMAL;
        dirty = true;
    }

public:

    tButton(const std::string &cname);

    virtual ~tButton() {}

    static tButton * createtButton(std::string cname,
                                       std::string label,
                                       iXY loc);
    static tButton * createtButton(std::string cname,
                                       std::string label,
                                       iXY loc, int with);

    void setTextColors( PIX normal, PIX over, PIX pressed, PIX disable)
    {
        textColors[NORMAL] = normal;
        textColors[OVER] = over;
        textColors[PRESSED] = pressed;
        textColors[DISABLE] = disable;
    }
    void Enable()
    {
        enabled = true;
        bstate = NORMAL;
        dirty = true;
    }
    void Disable()
    {
        enabled = false;
        bstate = DISABLE;
        dirty = true;
    }
    
    bool isEnable() const
    {
        return enabled;
    }
    
    void setLabel(const std::string& l)
    {
        label = l;
        dirty = true;
    }

    void setImage(const Surface &s)
    {
        if ( s.getNumFrames() )
        {
            bimage.copy(s);
            setSize(bimage.getWidth(), bimage.getHeight());
        }
        dirty = true;
    }
    const std::string& getLabel() const
    {
        return label;
    }

    virtual void actionPerformed(const mMouseEvent &me);
}; // end Button

#endif // end __tButton_hpp__
