/* 
 * File:   OptionsLayer.hpp
 * Author: krom
 *
 * Created on February 22, 2013, 3:33 PM
 */

#ifndef OPTIONSLAYER_HPP
#define	OPTIONSLAYER_HPP

#include "2D/ComponentLayer.hpp"
#include "ModalMessageLayer.hpp"

class CheckBox;
class Choice;
class Slider;
class Label;

class ModalMessageLayer;

class OptionsLayer : public ComponentLayer
{
public:
    OptionsLayer();
    ~OptionsLayer();

private:
    void recalculateComponentLocations();
    void handleComponentEvents();

    ModalMessageLayer * message_layer;
    
    Component * area;

    Component * separator_visual;
    Label     * label_screenres;
    Label     * label_minimapunitsize;
    Choice    * choice_screenres;
    Choice    * choice_minimapunitsize;
    CheckBox  * checkbox_fullscreen;
    CheckBox  * checkbox_showshadows;
    CheckBox  * checkbox_blendsmoke;
    
    Component * separator_interface;
    Label     * label_scrollrate;
    Slider    * slider_scrollrate;
    Label     * label_scrollrate_value;

    Component * separator_sound;
    Label     * label_sound;
    CheckBox  * checkbox_sound;
    Slider    * slider_soundvolume;
    Label     * label_soundvolume_value;
    Label     * label_music;
    CheckBox  * checkbox_music;
    Slider    * slider_musicvolume;
    Label     * label_musicvolume_value;

#ifdef _WIN32
    CheckBox * checkbox_usedirectx;
#endif

};

#endif	/* OPTIONSLAYER_HPP */

