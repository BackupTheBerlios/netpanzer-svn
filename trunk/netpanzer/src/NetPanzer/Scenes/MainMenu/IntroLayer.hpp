/*
Copyright (C) 2013 Netpanzer Team. (www.netpanzer.org), Aaron Perez

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
 *
 * Created on March 4, 2013, 12:53 AM
 */

#ifndef INTROLAYER_HPP
#define	INTROLAYER_HPP

#include "2D/ComponentLayer.hpp"

class CheckBox;
class Choice;
class Slider;
class Label;
class Slider;
class InputField;

class IntroLayer : public ComponentLayer
{
public:
    IntroLayer();
    ~IntroLayer();
    
private:
    void recalculateComponentLocations();
    void handleComponentEvents();
    
    Label       * label;
    InputField  * input_field;
    
};

#endif	/* INTROLAYER_HPP */

