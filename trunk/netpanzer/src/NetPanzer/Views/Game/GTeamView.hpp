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

#ifndef GTEAMVIEW_HPP
#define GTEAMVIEW_HPP

#include <vector>
#include "GameTemplateView.hpp"
#include "2D/Surface.hpp"
#include "Views/Components/tButton.hpp"

class PlayerState;

class GTeamView : public GameTemplateView
{
private:
    bool loaded;

    iRect firstrect;
    iRect secondrect;
    std::vector<const PlayerState*> states;
    void drawTeams(Surface &dest);
    tButton * changebutton;

public:
    GTeamView();
    virtual ~GTeamView()
    {}

    virtual void doActivate();
    virtual void doDeactivate();
    virtual void doDraw(Surface &windowArea, Surface &clientArea);
    virtual void checkResolution(iXY oldResolution, iXY newResolution);
    virtual void processEvents();
    virtual void onComponentClicked(Component* c);
    virtual void resize(const iXY &size);

    void init();
};

#endif // GTEAMVIEW_HPP
