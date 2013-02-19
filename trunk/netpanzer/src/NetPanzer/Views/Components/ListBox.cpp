
#include "ListBox.hpp"

ListBox::ListBox(ListBoxDataSource* data_source)
    : Component(), data_source(data_source)
{
    
}

ListBox::~ListBox()
{
    delete data_source;
    data_source = 0;
}

void ListBox::draw(Surface& dest)
{
    
}
