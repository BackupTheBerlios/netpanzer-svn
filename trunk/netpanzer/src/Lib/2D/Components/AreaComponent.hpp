/* 
 * File:   AreaComponent.hpp
 * Author: krom
 *
 * Created on February 23, 2013, 1:32 PM
 */

#ifndef AREACOMPONENT_HPP
#define	AREACOMPONENT_HPP

#include "Views/Components/Component.hpp"

class AreaComponent : public Component
{
public:
    AreaComponent() : Component() {}
    AreaComponent(unsigned width, unsigned height) : Component() { setSize(width, height); }
    
private:
    void draw(Surface& dest) {}
    void render() {}
};

#endif	/* AREACOMPONENT_HPP */

