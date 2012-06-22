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

#include "Component.hpp"
#include "2D/Color.hpp"


//--------------------------------------------------------------------------
class tBitButton : public Component
{
protected:
    enum BState
    {
        NORMAL =   0,
        OVER   =   1,
        PRESSED=   2,
        DISABLE=   3
    } ;
    Surface bimage;
    BState bstate;
    void render();
    void resetState()
    {
        bstate = NORMAL;
        dirty = true;
    }

public:
    tBitButton(const std::string &cname);

    virtual ~tBitButton() {}

    static tBitButton * createButton(const std::string cname,const Surface &Bitmap, const iXY loc);

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
        
    void setImage(const Surface &s)
    {
        if ( s.getNumFrames() )
        {
            bimage.copy(s);
            setSize(bimage.getWidth(), bimage.getHeight());
        }
        dirty = true;
    }

    virtual void actionPerformed(const mMouseEvent &me);
}; // end Button

