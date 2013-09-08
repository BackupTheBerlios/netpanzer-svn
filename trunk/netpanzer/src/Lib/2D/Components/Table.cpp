/*
Copyright (C) 2013 Netpanzer Team. (www.netpanzer.org), Aaron Perez

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 * 
 * Created on September 7, 2013, 12:54 PM
 */

#include "Table.hpp"
#include "VScrollBar.hpp"
#include "Actions/Action.hpp"
#include "2D/TextRenderingSystem.hpp"
#include "2D/Color.hpp"
#include "GameInput/InputManager.hpp"

#define REPEAT_INITIAL (500)
#define REPEAT_TIME (50)

struct Events
{
    enum
    {
        ScrollChange = 1
    };
};

Table::DataSource::DataSource()
{
    table = 0;
}

Table::DataSource::~DataSource()
{
    if ( table )
    {
        table->setDataSource(0);
    }
}

void Table::DataSource::setTable(Table* table)
{
    if ( table != this->table )
    {
        if ( this->table )
        {
            this->table->setDataSource(0);
        }
        
        this->table = table;
        
        if ( this->table )
        {
            this->table->setDataSource(this);
        }
    }
}

Table::Table(const unsigned numColumns, const int intercolumnWidth)
{
    this->numColumns = numColumns;
    this->intercolumnWidth = intercolumnWidth;
    dirty = true;

    scrollbar = new VScrollBar();
    hoveringScrollbar = false;
    selectingScrollbar = false;
    firstRowTotalPosition = 0;
    firstRowOffset = 0;
    firstRowHeight = 0;
    lastRowHeight = 0;

    scrollbar->setComponentEvents(&myEvents);
    scrollbar->setChangeEvent(Events::ScrollChange);
    scrollbar->setAutoRepeat(REPEAT_INITIAL, REPEAT_TIME);
}

Table::~Table()
{
    columnDef.deleteAll();
    painters.deleteAll();
    delete scrollbar;
}

void Table::setLocation(const int x, const int y)
{
    Component::setLocation(x, y);
    const int x_start = rect.getEndX() - scrollbar->getWidth();
    // @todo use headerHeight
    scrollbar->setLocation(x_start+1, y + TextRenderingSystem::line_height());
}

void Table::setSize(const int x, const int y)
{
    Component::setSize( x, y );
    // @todo use header height
    scrollbar->setVisibleHeight(y - TextRenderingSystem::line_height());
    const int x_start = rect.getEndX() - scrollbar->getWidth();
    scrollbar->setLocation(x_start+1, scrollbar->getLocationY());
}

void Table::addColumn(const UString& text, const unsigned width)
{
    const unsigned num = columnDef.size();
    if ( num < numColumns )
    {
        columnDef.push_back(new ColumnDef(text, width));
        if ( columnDef.size() == numColumns )
        {
            // @todo may need to do some recalculation
        }
    }
        
}

void Table::draw(Surface& dest) const
{
    dest.fillRect(rect, Color::orange);
    int y = drawHeader(dest);
    scrollbar->draw(dest);
    
    if ( ! painters.empty() )
    {
        iRect cellArea(0,firstRowOffset,0,firstRowHeight);
        
        int x = getLocationX();
        
        const RowPainter *p = painters[0];
        
        // draw first row, maybe cell are half drawn due to scrolling
        for ( int c = 0, ce = columnDef.size(); c < ce; c++ )
        {
            const int w = std::min(columnDef[c]->width, getWidth() - scrollbar->getWidth() - (x-getLocationX()));
            cellArea.setWidth(w);
            p->paintCell(dest, x, y, cellArea, c);
            x += columnDef[c]->width + intercolumnWidth;
        }
        
        y += firstRowHeight;
        cellArea.setLocationY(0);
        cellArea.setHeight(rowHeight);
        
        for ( int n = 1, ne = painters.size()-1; n < ne; n++ )
        {
            x = getLocationX();
            p = painters[n];
            for ( int c = 0, ce = columnDef.size(); c < ce; c++ )
            {
                const int w = std::min(columnDef[c]->width, getWidth() - scrollbar->getWidth() - (x-getLocationX()));
                cellArea.setWidth(w);
                p->paintCell(dest, x, y, cellArea, c);
                x += columnDef[c]->width + intercolumnWidth;
            }
            
            y += rowHeight;
        }

        // draw last row if needed
        if ( painters.size() >  1 )
        {
            x = getLocationX();
            cellArea.setHeight(lastRowHeight);
            p = painters.back();
            for ( int c = 0, ce = columnDef.size(); c < ce; c++ )
            {
                const int w = std::min(columnDef[c]->width, getWidth() - scrollbar->getWidth() - (x-getLocationX()));
                cellArea.setWidth(w);
                p->paintCell(dest, x, y, cellArea, c);
                x += columnDef[c]->width + intercolumnWidth;
            }
        }
    }
}

void Table::logic()
{
    scrollbar->logic();
    if ( dirty )
    {
        const int rowcount = ds->getRowCount();
        scrollbar->setTotalHeight(rowcount * rowHeight);
        if ( rowcount )
        {
            const int scrollpos = scrollbar->getValue();
            const int firstRow = scrollpos / rowHeight;
            const int froffset = scrollpos % rowHeight;
            
            // @todo save the header height somewhere, or the visible height
            const int endy = scrollpos + (getHeight() - TextRenderingSystem::line_height());
            
            int y = scrollpos - froffset;
            
            // @todo optimize this, don't delete all
            painters.deleteAll();
            for ( int n = firstRow, ne = rowcount; n < ne; n++ )
            {
                RowPainter * p = ds->newRowPainter();
                ds->configureRowPainter(p, n);
                painters.push_back(p);
                y += rowHeight;
                if ( y >= endy )
                    break;
            }
            
            lastRowHeight = rowHeight;
            if ( y > endy )
            {
                lastRowHeight -= y - endy;
            }
            
            firstRowHeight = rowHeight - froffset;
            firstRowOffset = froffset;

        }
        else
        {
            firstRowTotalPosition = 0;
            painters.deleteAll();
        }
    }
}

int Table::drawHeader(Surface& dest) const
{
    const int lineh = TextRenderingSystem::line_height();
    iRect textRect;
    iRect columnRect(rect);
    
    textRect.setHeight(lineh);
    columnRect.setHeight(lineh);
    
    for ( unsigned n = 0; n < columnDef.size(); n++ )
    {
        const int w = std::min(columnDef[n]->width, getWidth() - (columnRect.getLocationX()-getLocationX()));
        columnRect.setWidth( w );
        textRect.setWidth( w );
        dest.fillRect(columnRect, Color::darkGreen);
        columnDef[n]->textRender.drawPart(dest, columnRect.getLocationX(), columnRect.getLocationY(), textRect, Color::white);
        
        columnRect.translate(columnDef[n]->width + intercolumnWidth, 0);
        
        if ( columnRect.getLocationX() >= rect.getEndX() )
            break;
    }
    
    return columnRect.getLocationY() + lineh;
}

void Table::setDataSource(const DataSource* ds)
{
    if ( ds != this->ds )
    {
        this->ds = ds;
        if ( ds )
        {
            rowHeight = ds->getRowHeight();
            scrollbar->setStepSize(rowHeight);
        }
        dirty = true;
    }
}

void Table::onDataChanged()
{
    dirty = true;
}

void Table::onSelectStart()
{
    if ( hoveringScrollbar )
    {
        scrollbar->onSelectStart();
        selectingScrollbar = true;
    }
}

void Table::onSelectStop()
{
    if ( selectingScrollbar )
    {
        scrollbar->onSelectStop();
        selectingScrollbar = false;
        onPointerMove( 0, 0);
    }
}

void Table::onHoverStart()
{
    // unused
}

void Table::onHoverStop()
{
    if ( (! selectingScrollbar) && hoveringScrollbar )
    {
        scrollbar->onHoverStop();
        hoveringScrollbar = false;
    }
}

void Table::onPointerMove(int rel_x, int rel_y)
{
    const int mx = GameInput::InputManager::getMouseX();
    const int my = GameInput::InputManager::getMouseY();
    
    if ( ! selectingScrollbar )
    {
        if ( scrollbar->contains(mx, my) )
        {
            if ( ! hoveringScrollbar )
            {
                hoveringScrollbar = true;
                scrollbar->onHoverStart();
            }
            
            scrollbar->onPointerMove(rel_x, rel_y);
        }
        else
        {
            if ( hoveringScrollbar )
            {
                hoveringScrollbar = false;
                scrollbar->onHoverStop();
            }
        }
    }
    else
    {
        scrollbar->onPointerMove(rel_x, rel_y);
    }
}
