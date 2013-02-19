/* 
 * File:   Layer.hpp
 * Author: krom
 *
 * Created on February 14, 2013, 11:35 PM
 */

#ifndef LAYER_HPP
#define	LAYER_HPP

class Layer
{
private:
    friend class Scene;
    Layer * next;
    int z_index;
    
public:
    Layer(const int z_index) : z_index(z_index) {}
    virtual ~Layer() {}
    
    virtual void draw() = 0; // would like const
    
};

#endif	/* LAYER_HPP */
