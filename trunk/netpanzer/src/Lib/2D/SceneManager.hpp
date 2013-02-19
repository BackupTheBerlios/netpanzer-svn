/* 
 * File:   SceneManager.hpp
 * Author: krom
 *
 * Created on February 15, 2013, 10:40 AM
 */

#ifndef SCENEMANAGER_HPP
#define	SCENEMANAGER_HPP

class Scene;

class SceneManager
{
public:
    static bool init( Scene * first_scene );
    
    static bool run();
    
    static void endLoop();
    
    static void setNextScene( Scene * next_scene );
};


#endif	/* SCENEMANAGER_HPP */

