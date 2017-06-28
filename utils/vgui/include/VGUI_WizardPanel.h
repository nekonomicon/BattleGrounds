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


#ifndef VGUI_WIZARDPANEL_H
#define VGUI_WIZARDPANEL_H

#include<VGUI_Panel.h>
#include<VGUI_Dar.h>

namespace vgui
{

class ActionSignal;

class VGUIAPI WizardPanel : public Panel
{

public:

	class VGUIAPI WizardPage : public Panel
	{
	
	friend class WizardPanel;
	
	private:

		WizardPage* _backWizardPage;
		WizardPage* _nextWizardPage;
		bool        _backButtonEnabled;
		bool        _nextButtonEnabled;
		bool        _finishedButtonEnabled;
		bool        _cancelButtonEnabled;
		bool        _backButtonVisible;
		bool        _nextButtonVisible;
		bool        _finishedButtonVisible;
		bool        _cancelButtonVisible;
		char*       _backButtonText;
		char*       _nextButtonText;
		char*       _finishedButtonText;
		char*       _cancelButtonText;
		Dar<ActionSignal*> _switchingToBackPageSignalDar;
		Dar<ActionSignal*> _switchingToNextPageSignalDar;
		char*       _title;
		Panel*      _wantedFocus;

	private:
		
		virtual void fireSwitchingToBackPageSignals();
		virtual void fireSwitchingToNextPageSignals();
		virtual void init();

	public:

		WizardPage();
		WizardPage(int wide,int tall);

	public:

		virtual void        setBackWizardPage(WizardPage* backWizardPage);
		virtual void        setNextWizardPage(WizardPage* nextWizardPage);
		virtual WizardPage* getBackWizardPage();
		virtual WizardPage* getNextWizardPage();

		virtual bool        isBackButtonEnabled();
		virtual bool        isNextButtonEnabled();
		virtual bool        isFinishedButtonEnabled();
		virtual bool        isCancelButtonEnabled();
		virtual void        setBackButtonEnabled(bool state);
		virtual void        setNextButtonEnabled(bool state);
		virtual void        setFinishedButtonEnabled(bool state);
		virtual void        setCancelButtonEnabled(bool state);

		virtual bool        isBackButtonVisible();
		virtual bool        isNextButtonVisible();
		virtual bool        isFinishedButtonVisible();
		virtual bool        isCancelButtonVisible();
		virtual void        setBackButtonVisible(bool state);
		virtual void        setNextButtonVisible(bool state);
		virtual void        setFinishedButtonVisible(bool state);
		virtual void        setCancelButtonVisible(bool state);

		virtual void        getBackButtonText(char* text,int textLen);
		virtual void        getNextButtonText(char* text,int textLen);
		virtual void        getFinishedButtonText(char* text,int textLen);
		virtual void        getCancelButtonText(char* text,int textLen);
        virtual void        setBackButtonText(const char* text);
		virtual void        setNextButtonText(const char* text);
		virtual void        setFinishedButtonText(const char* text);
		virtual void        setCancelButtonText(const char* text);

		virtual void		setWantedFocus(Panel* panel);
		virtual Panel*		getWantedFocus();

		virtual void        addSwitchingToBackPageSignal(ActionSignal* s);
		virtual void        addSwitchingToNextPageSignal(ActionSignal* s);

		virtual void        setTitle(const char* title);
		virtual void        getTitle(char* buf,int bufLen);

	};

private:

	Button*     _backButton;
	Button*     _nextButton;
	Button*     _finishedButton;
	Button*     _cancelButton;
	WizardPage* _currentWizardPage;
	Dar<ActionSignal*> _pageChangedActionSignalDar;

private:

	virtual void fireFinishedActionSignal();
	virtual void fireCancelledActionSignal();
	virtual void firePageChangedActionSignal();

protected:

	virtual void performLayout();

public:

	WizardPanel(int x,int y,int wide,int tall);

public:

	virtual void setCurrentWizardPage(WizardPage* currentWizardPage);
	virtual void addFinishedActionSignal(ActionSignal* s);
	virtual void addCancelledActionSignal(ActionSignal* s);
	virtual void addPageChangedActionSignal(ActionSignal* s);
	virtual void doBack();
	virtual void doNext();
	virtual void getCurrentWizardPageTitle(char* buf,int bufLen);
	virtual WizardPage* getCurrentWizardPage();

};

}


#endif

