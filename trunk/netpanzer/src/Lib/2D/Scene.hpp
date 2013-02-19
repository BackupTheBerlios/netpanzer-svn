/* 
 * File:   Scene.hpp
 * Author: krom
 *
 * Created on February 15, 2013, 12:55 AM
 */

#ifndef SCENE_HPP
#define	SCENE_HPP

class Layer;

class Scene
{
private:
    Layer * layers;
    
protected:
    bool addLayer(Layer * layer);
    
    virtual void handleMouseLocation(int x, int y) {}
    virtual void handleMouseDown(int x, int y, int button) {}
    virtual void handleMouseUp(int x, int y, int button) {}
    
public:
    Scene() : layers(0) {}
    virtual ~Scene() {}
    
    void draw() const;
    
    void input();
    
    virtual void logic() = 0;
    
};


#endif	/* SCENE_HPP */

