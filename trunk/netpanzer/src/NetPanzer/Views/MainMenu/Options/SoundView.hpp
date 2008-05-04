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
#ifndef __SoundView_hpp__
#define __SoundView_hpp__

#include "Views/Components/View.hpp"
#include "Views/MainMenu/OptionsTemplateView.hpp"
#include "Views/Components/CheckBox.hpp"
#include "Views/Components/StateChangedCallback.hpp"

/////////////////////////////////////////////////////////////////////////////
// forward declarations
/////////////////////////////////////////////////////////////////////////////

class Surface;

/////////////////////////////////////////////////////////////////////////////
// types
/////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
class SoundView : public OptionsTemplateView, public StateChangedCallback
{
protected:
    virtual void loadTitleSurface();
    CheckBox * checkBoxSoundEnabled;
    CheckBox * checkBoxMusicEnabled;

public:
    SoundView();

    virtual void doDraw(Surface &windowArea, Surface &clientArea);
    virtual void initButtons();
    virtual void stateChanged(Component* source);
}; // end SoundView

#endif // end __SoundView_hpp__
