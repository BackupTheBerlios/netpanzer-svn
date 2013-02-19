/* 
 * File:   ListBox.hpp
 * Author: krom
 *
 * Created on February 8, 2013, 5:59 PM
 */

#ifndef LISTBOX_HPP
#define	LISTBOX_HPP

#include "Component.hpp"
#include "Types/iRect.hpp"

class ListBoxDataSource
{
public:
    virtual ~ListBoxDataSource() {}
    
    virtual void getElementCount() = 0;
    virtual void paintRow(size_t row, Surface& dest) = 0;
    virtual void setSelectedRow(size_t row) = 0;
};

class ListBox : public Component
{
public:
    ListBox(ListBoxDataSource * data_source);
    ~ListBox();
    
    void draw(Surface& dest);
    
private:
    ListBoxDataSource * data_source;
    
    iRect rows_area;
    
};


#endif	/* LISTBOX_HPP */

