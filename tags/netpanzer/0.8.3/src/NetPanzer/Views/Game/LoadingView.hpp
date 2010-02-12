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
#include "Views/Components/Desktop.hpp"
#include "2D/Surface.hpp"
#include "Util/Log.hpp"
#include "Interfaces/GameManager.hpp"
#include <list>
#include <string>

using namespace std;

#define LINELIMIT 100

//---------------------------------------------------------------------------
class LoadingView : public View
{
public:
    static void update(const string text)
    {
        LOGGER.info("Loading: %s", text.c_str());
        lines.pop_back();
        lines.push_back(text);
        dirty=true;
    }
    
    static void append(const string text)
    {
        LOGGER.info("Loading: %s", text.c_str());
        if ( lines.size() >= LINELIMIT )
        {
            lines.pop_front();
        }
        lines.push_back(text);
        dirty=true;
    }
    
    LoadingView()
    {
        init();
    }
    
    static void loadFinish()
    {
        Desktop::setVisibilityAllWindows(false);
        GameManager::loadPalette("netp");
        GameManager::setNetPanzerGameOptions();
        Desktop::setVisibility("MiniMapView", true);
        Desktop::setVisibility("GameView", true);
    }

    static void loadError()
    {
        GameManager::loadPalette("netpmenu");
        Desktop::setVisibilityAllWindows(false);
        Desktop::setVisibility("MainView", true);

    }
    
    static void show()
    {
        Desktop::setVisibilityAllWindows(false);
        Desktop::setVisibility("LoadingView", true);
    }
    
    static void hide()
    {
        Desktop::setVisibility("LoadingView", false);
    }

    virtual ~LoadingView() {}
    
    virtual void init();
    virtual void doActivate();
    virtual void doDeactivate();
    
    void doDraw(Surface &viewArea, Surface &clientArea);
    void render();
    
private:
    static list<string> lines;
    static bool dirty;
    
    Surface backgroundSurface;
    Surface surface;
};

#endif // end __LOADINGVIEW_HPP__
