/*
Copyright (C) 2003 Ivo Danihelka <ivo@danihelka.net>
 
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
#ifndef __LOADINGVIEW_HPP__
#define __LOADINGVIEW_HPP__

#include "Views/Components/View.hpp"
#include "2D/Surface.hpp"

//---------------------------------------------------------------------------
class LoadingView : public View
{
public:
    virtual ~LoadingView() {}

    virtual void init() = 0;
    virtual void update(const char *text) = 0;
    virtual void scroll() = 0;
    void scrollAndUpdate(const char *text)
    {
        scroll();
        update(text);
    }
    void scrollDirect()
    {
        scroll();
        blitToScreen();
    }
    void scrollAndUpdateDirect(const char *text)
    {
        scroll();
        update(text);
        blitToScreen();
    }
    void updateDirect(const char *text)
    {
        update(text);
        blitToScreen();
    }

    virtual void reset() = 0;

    virtual void open() = 0;
    virtual void close() = 0;
    virtual void toggleGameView() = 0;
    virtual void toggleMainMenu() = 0;
private:
    virtual void blitToScreen() {};
};

#endif // end __LOADINGVIEW_HPP__
