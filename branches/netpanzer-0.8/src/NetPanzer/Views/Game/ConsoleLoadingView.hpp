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
#ifndef __CONSOLELOADINGVIEW_HPP__
#define __CONSOLELOADINGVIEW_HPP__

#include "View.hpp"
#include "2D/Surface.hpp"
#include "LoadingView.hpp"

//---------------------------------------------------------------------------
class ConsoleLoadingView : public LoadingView
{
public:
    virtual void init() {};
    virtual void update(const char *text);
    virtual void scroll() {};

    virtual void reset() {};

    virtual void open() {};
    virtual void close() {};
    virtual void toggleGameView();
    virtual void toggleMainMenu();
private:
    virtual void blitToScreen() { }
};

#endif
