/* 
 * File:   MainMenuScene.hpp
 * Author: krom
 *
 * Created on February 16, 2013, 10:29 PM
 */

#ifndef MAINMENUSCENE_HPP
#define	MAINMENUSCENE_HPP

#include "2D/Scene.hpp"
#include "MainMenu/IntroLayer.hpp"

class MainMenuLayer;

class MainMenuScene : public Scene
{
public:
    MainMenuScene();
    ~MainMenuScene();
    
    void logic();
    
private:
    
    MainMenuLayer * menu_layer;
    
    void onBecomeActive();
};

#endif	/* MAINMENUSCENE_HPP */

