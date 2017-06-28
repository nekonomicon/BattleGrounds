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

//========= Copyright © 1996-2001, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#ifndef VGUI_GRID_H
#define VGUI_GRID_H
#ifdef _WIN32
#pragma once
#endif


#include "vgui_panel.h"


namespace vgui
{

// The grid control simply manages a grid of panels. You can adjust column sizes and spacings and 
// configure and fill the panels however you want.
// To use this control, call SetDimensions, SetSpacing and fill the controls.
class CGrid : public Panel
{
public:
						CGrid();
	virtual				~CGrid();

	bool				SetDimensions(int xCols, int yRows);		// Set how many columns and rows in the grid.
	void				Term();

	Panel*				GetEntry(int x, int y);						// Get the panel associated with a grid entry.
	bool				SetEntry(int x, int y, Panel *pPanel);
	
	int					GetXSpacing();
	int					GetYSpacing();
	void				SetSpacing(int xSpacing, int ySpacing);		// Set spacing between rows and columns.

	bool				SetColumnWidth(int iColumn, int width);		// Set a column's width.
	bool				SetRowHeight(int iRow, int height);			// Set a row's height.

	int					GetColumnWidth(int iColumn);
	int					GetRowHeight(int iRow);

	int					CalcFitColumnWidth(int iColumn);			// Returns the maximum width of all panels in the column.
	int					CalcFitRowHeight(int iRow);					// Returns the maximum height of all panels in the row.

	int					CalcDrawHeight();							// Returns how many pixels high the grid control should be
																	// for all of its contents to be visible (based on its row heights
																	// and y spacing).

	void				AutoSetRowHeights();						// Just does SetRowHeight(iRow, CalcFitRowHeight(iRow)) for all rows.

	bool				GetEntryBox(								// Returns the bounding box for the specified entry.
		int col, int row, int &x, int &y, int &w, int &h);

	bool				CopyColumnWidths(CGrid *pOther);			// Copy the column widths from the other grid. Fails if the 
																	// column count is different.

	void				RepositionContents();						// Sets the size and position of all the grid entries based
																	// on current spacings and row/column widths.
																	// You usually only want to call this while setting up the control
																	// if you want to get the position or dimensions of the child  
																	// controls. This will set them.

	void				SetRowUnderline(int row, bool enabled, int offset, int r, int g, int b, int a);	// sets underline color for a particular row

	// returns the true if found, false otherwise
	bool					getCellAtPoint(int worldX, int worldY, int &row, int &col);

// Panel overrides.
public:
	
	virtual void		paint();
	virtual void		paintBackground();

protected:

	class CGridEntry
	{
	public:
				CGridEntry();
				~CGridEntry();
		
		Panel	*m_pPanel;

		bool	m_bUnderline;
		short	m_UnderlineColor[4];
		int		m_iUnderlineOffset;
	};

	void				Clear();
	CGridEntry*			GridEntry(int x, int y);

	void				CalcColOffsets(int iStart);
	void				CalcRowOffsets(int iStart);


protected:

	bool		m_bDirty;	// Set when controls will need to be repositioned.

	int			m_xCols;
	int			m_yRows;
	
	int			m_xSpacing;
	int			m_ySpacing;

	int			*m_Widths;
	int			*m_Heights;
	int			*m_ColOffsets;
	int			*m_RowOffsets;
	
	CGridEntry	*m_GridEntries;
	
};

};


#endif // VGUI_GRID_H
