/*
    Copyright 2001 to 2004. The Battle Grounds Team and Contributors

    This file is part of the Battle Grounds Modification for Half-Life.

    The Battle Grounds Modification for Half-Life is free software;
    you can redistribute it and/or modify it under the terms of the
    GNU Lesser General Public License as published by the Free
    Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.

    The Battle Grounds Modification for Half-Life is distributed in
    the hope that it will be useful, but WITHOUT ANY WARRANTY; without
    even the implied warranty of MERCHANTABILITY or FITNESS FOR A
    PARTICULAR PURPOSE.  See the GNU Lesser General Public License
    for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with The Battle Grounds Modification for Half-Life;
    if not, write to the Free Software Foundation, Inc., 59 Temple Place,
    Suite 330, Boston, MA  02111-1307  USA

    You must obey the GNU Lesser General Public License in all respects for
    all of the code used other than code distributed with the Half-Life
    SDK developed by Valve.  If you modify this file, you may extend this
    exception to your version of the file, but you are not obligated to do so.
    If you do not wish to do so, delete this exception statement from your
    version.
*/


#ifndef VGUI_TABLEPANEL_H
#define VGUI_TABLEPANEL_H

#include<VGUI.h>
#include<VGUI_Panel.h>
#include<VGUI_Dar.h>

namespace vgui
{

class HeaderPanel;

class VGUIAPI TablePanel : public Panel
{
friend class FooVGuiTablePanelHandler;
private:
	vgui::Dar<int> _columnDar;
	bool           _gridVisible[2];
	int            _gridWide;
	int            _gridTall;
	int            _selectedCell[2];
	int            _mouseOverCell[2];
	int            _editableCell[2];
	Panel*         _fakeInputPanel;
	bool           _columnSelectionEnabled;
	bool           _rowSelectionEnabled;
	bool           _cellSelectionEnabled;
	Panel*         _editableCellPanel;
	int            _virtualSize[2];
	bool           _cellEditingEnabled;
public:
	TablePanel(int x,int y,int wide,int tall,int columnCount);
public:
	virtual void   setCellEditingEnabled(bool state);
	virtual void   setColumnCount(int columnCount);
	virtual void   setGridVisible(bool horizontal,bool vertical);
	virtual void   setGridSize(int gridWide,int gridTall);
	virtual int    getColumnCount();
	virtual void   setColumnExtents(int column,int x0,int x1);
	virtual void   setSelectedCell(int column,int row);
	virtual void   getSelectedCell(int& column,int& row);
	virtual void   setHeaderPanel(HeaderPanel* header);
	virtual void   setColumnSelectionEnabled(bool state);
	virtual void   setRowSelectionEnabled(bool state);
	virtual void   setCellSectionEnabled(bool state);
	virtual void   setEditableCell(int column,int row);
	virtual void   stopCellEditing();
	virtual void   getVirtualSize(int& wide,int& tall);
	virtual int    getRowCount()=0;
	virtual int    getCellTall(int row)=0;
	virtual Panel* getCellRenderer(int column,int row,bool columnSelected,bool rowSelected,bool cellSelected)=0;
	virtual Panel* startCellEditing(int column,int row)=0;
protected:
	virtual void   paint();
	virtual Panel* isWithinTraverse(int x,int y);
private:
	virtual void privateMousePressed(MouseCode code,Panel* panel);
	virtual void privateMouseDoublePressed(MouseCode code,Panel* panel);
	virtual void privateKeyTyped(KeyCode code,Panel* panel);
};

}

#endif