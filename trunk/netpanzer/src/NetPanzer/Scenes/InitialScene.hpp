/* 
 * File:   InitialScene.hpp
 * Author: krom
 *
 * Created on February 15, 2013, 11:03 AM
 */

#ifndef INITIALSCENE_HPP
#define	INITIALSCENE_HPP

#include "2D/Scene.hpp"

class InitialScene : public Scene
{
public:
    InitialScene();
    ~InitialScene();
    
    void logic();
    
};

#endif	/* INITIALSCENE_HPP */

