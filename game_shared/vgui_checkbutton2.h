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

#ifndef VGUI_CHECKBUTTON2_H
#define VGUI_CHECKBUTTON2_H
#ifdef _WIN32
#pragma once
#endif


#include "vgui_label.h"
#include "vgui_imagepanel.h"
#include "vgui_defaultinputsignal.h"


namespace vgui
{


class CCheckButton2;


class ICheckButton2Handler
{
public:
	virtual void	StateChanged(CCheckButton2 *pButton) = 0;
};


// VGUI checkbox class.
// - Provides access to the checkbox images.
// - Provides an easy callback mechanism for state changes.
// - Default background is invisible, and default text color is white.
class CCheckButton2 : public Panel, public CDefaultInputSignal
{
public:

				CCheckButton2();
				~CCheckButton2();
	
	// Initialize the button with these.
	void		SetImages(char const *pChecked, char const *pUnchecked);
	void		SetImages(Image *pChecked, Image *pUnchecked);		// If you use this, the button will never delete the images.
	void		DeleteImages();

	// The checkbox can be to the left or the right of the text (default is left).
	void		SetCheckboxLeft(bool bLeftAlign);
	bool		GetCheckboxLeft();
	
	// Set the label text.
	void		SetText(char const *pText, ...);
	void		SetTextColor(int r, int g, int b, int a);

	// You can register for change notification here.
	void		SetHandler(ICheckButton2Handler *pHandler);
	
	// Get/set the check state.
	bool		IsChecked();
	void		SetChecked(bool bChecked);



// Panel overrides.
public:

	virtual void	internalMousePressed(MouseCode code);	


protected:

	void			SetupControls();


// InputSignal overrides.
protected:
	virtual void mousePressed(MouseCode code,Panel* panel);


public:
	ICheckButton2Handler	*m_pHandler;

	bool		m_bCheckboxLeft;
	Label		m_Label;
	ImagePanel	m_CheckboxPanel;
	
	Image		*m_pChecked;
	Image		*m_pUnchecked;
	bool		m_bOwnImages;

	bool		m_bChecked;
};


}


#endif // VGUI_CHECKBUTTON2_H
