/* 
 * File:   MainMenuScene.hpp
 * Author: krom
 *
 * Created on February 16, 2013, 10:29 PM
 */

#ifndef MAINMENUSCENE_HPP
#define	MAINMENUSCENE_HPP

#include "2D/Scene.hpp"

class MainMenuLayer;

class MainMenuScene : public Scene
{
public:
    MainMenuScene();
    ~MainMenuScene();
    
    void logic();
    
private:
    
    MainMenuLayer * menu_layer;
    
    void handleMouseLocation(int x, int y);
    void handleMouseDown(int x, int y, int button);
    void handleMouseUp(int x, int y, int button);
    
};

#endif	/* MAINMENUSCENE_HPP */

