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

#ifndef TABLE_HPP
#define	TABLE_HPP

#include "Views/Components/Component.hpp"
#include "2D/TextRenderer.hpp"
#include "Types/UString.hpp"
#include "ArrayUtil/PtrArray.hpp"

class VScrollBar;

class Table : public Component
{
public:
    /**
     * Interface for painting rows. DataSource shall return subclasses of
     * RowPainter to draw the table data
     */
    class RowPainter
    {
        public:
            RowPainter() {}
            virtual ~RowPainter() {}
            
            /**
             * Paints the 'rect' especified area of the especified column cell
             * on the destination surface at the specified location.
             * @param dest Surface where the cell will be drawn.
             * @param x X destination of the painting
             * @param y Y destinatino of the painting
             * @param rect subarea to paint of the cell
             * @param column Column number to paint
             */
            virtual void paintCell(Surface& dest, const int x, const int y, const iRect& rect, const unsigned column) const = 0;
    };
    
    
    /**
     * DataSource interface for tables
     */
    class DataSource
    {
        public:
            DataSource();
            virtual ~DataSource();
            
            /**
             * <p>Sets the display Table to use and also sets the DataSource in the
             * Table to <b>this</b>.</p>
             * <p><b>Note: don't set two DataSources to the same table at the
             * same time</b></p>
             * @param table The Table
             */
            void setTable(Table * table);
            
            /**
             * Implementations shall return the number of rows in this DataSource.
             * @return Number of rows in this DataSource
             */
            virtual unsigned getRowCount() const = 0;
            
            /**
             * Implementations shall return the height needed for painting a
             * row. Note that the Table only reads this value on
             * <b>setDataSource</b>.
             * @return Height
             */
            virtual unsigned getRowHeight() const = 0;
            
            /**
             * @return RowPainter for this DataSource
             */
            virtual RowPainter * newRowPainter() const = 0;
            
            /**
             * Configures the row painter to show
             * @param painter The RowPainter to configure
             * @param row The row number with the data to paint
             */
            virtual void configureRowPainter(RowPainter * painter, const unsigned row) const = 0;
            
        protected:
            /**
             * Subclasses may use this to notify the table that the data has 
             * been modified. Use it when the whole data is different.
             */
            void notifyDataChanged() { if ( table ) table->onDataChanged(); }
            
            
        private:
            friend class Table;
            Table * table;
    };
    
    /**
     * Creates a default table
     * @param numColumns Maximum number of columns the table may have
     * @param intercolumnWidth Separation space in pixels between columns, defaults to 1
     */
    Table(const unsigned numColumns, const int intercolumnWidth = 1);
    ~Table();
    
    void draw(Surface& dest) const;
    
    /**
     * Adds a new column definition to the table
     * @param text Text to show in the column header
     * @param width Width of the column
     */
    void addColumn(const UString& text, const unsigned width);
    
    void setSize(const int x, const int y);
    void setLocation(const int x, const int y);
    
private:
    friend class Table::DataSource;
    
    /**
     * Draws the table header and returns the new Y position to continue drawing
     * @param dest Surface to draw on
     * @return new Y position to continue drawing
     */
    int drawHeader(Surface& dest) const;

    /**
     * Sets the data source. <b>Only used by DataSource itself</b>
     * @param ds The DataSource
     */
    void setDataSource( const DataSource * ds);
    
    void onDataChanged();
    void onSelectStart();
    void onSelectStop();
    void onHoverStart();
    void onHoverStop();
    void onPointerMove(int rel_x, int rel_y);
    
    void logic();
    
    /** Structure for holding data of a column definition */
    struct ColumnDef
    {
        ColumnDef(const UString& text, const int width) : textRender(text), width(width) {}
        TextRenderer textRender;
        unsigned width;
    };
    
    /** Maximum number of columns the table may have */
    unsigned numColumns;
    
    /** Separation space in pixels between columns */
    int intercolumnWidth;
    
    /** Location of the first row on the whole scrollable view */
    int firstRowTotalPosition; // @todo find a better name for this
    
    /** cached first row draw offset */
    int firstRowOffset;
    
    /** cached first row drawing height */
    int firstRowHeight;
    
    /** cached last row height */
    int lastRowHeight;
    
    /** Height of one row, updated from the DataSource */
    int rowHeight;
    
    /** Flag to notify the logic() code it needs to do some recalculation */
    bool dirty;
    
    /** DataSource for this table */
    const DataSource * ds;

    /** List of defined columns */
    PtrArray<ColumnDef> columnDef;
    
    /** List of visible RowPainters */
    PtrArray<RowPainter> painters;
    
    /** The scroll bar */
    VScrollBar * scrollbar;
    
    /** flag if the mouse is over the scrollbar */
    bool hoveringScrollbar;
    
    /** flag if the mouse is pressing the scrollbar */
    bool selectingScrollbar;
    
    /** Scrollbar will send the change event here */
    ComponentEvents myEvents;
};

#endif	/* TABLE_HPP */

