/*
Copyright (C) 1998 Pyrosoft Inc. (www.pyrosoftgames.com), Matthew Bogue
 
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
#ifndef __ProgressView_hpp__
#define __ProgressView_hpp__

#include "View.hpp"
#include "Surface.hpp"

//---------------------------------------------------------------------------
class ProgressView : public View
{
protected:
    virtual void loadBackgroundSurface();

public:
    ProgressView();

    // Standard window related functions.
    virtual void doDraw(Surface &windowArea, Surface &clientArea);
    //virtual void lMouseDown(const iXY &p);
    virtual void doActivate();

    Surface background;
    Surface backgroundSurface;

    void init();
    void update(const char *text);
    void updateDirect(const char *text);

    void scroll();
    void scrollDirect();

    void scrollAndUpdate(const char *text)
    {
        scroll();
        update(text);
    }

    void scrollAndUpdateDirect(const char *text);

    void reset();

    void open();
    void close();
    void toggleGameView();
    void toggleMainMenu();

private:
    void blitToScreen();
}; // end ProgressView

extern ProgressView progressView;

#endif // end __ProgressView_hpp__
