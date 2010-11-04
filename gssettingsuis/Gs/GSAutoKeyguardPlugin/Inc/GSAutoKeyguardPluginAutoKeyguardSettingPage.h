/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*		Autokeyguard period settingpage     
*
*
*/


#ifndef     __CAUTOKEYGUARDSETTINGPAGE_H
#define     __CAUTOKEYGUARDSETTINGPAGE_H

#include    <aknradiobuttonsettingpage.h>
#include    <eikmobs.h>

class CAutoKeyguardSettingPage : public CAknRadioButtonSettingPage
	{
	public:
		/**
		* C++ default constructor.
		*/
		CAutoKeyguardSettingPage(TInt aResourceId, TInt& aCurrentSelectionItem, CDesCArrayFlat* aItemArray, TTime* aAutoKeyguardValue, TTime aMaxPeriod);
		/**
		* Symbian OS constructor.
		*/
		void ConstructL();
	protected:
		/**
		* Process ui commands
		* from CAknRadioButtonSettingPage
		* @param aCommandId TInt 
		*/
		void ProcessCommandL(TInt aCommandId);
		
		/** 
		 * From CAknRadioButtonSettingPage
		 * @param	listbox generating the event
		 * @param	type of listbox event
		 */
		void HandleListBoxEventL(CEikListBox* aListBox,
				MEikListBoxObserver::TListBoxEvent aEventType);
	private:
		/** 
		 * Handle GSAutoKeyguard settings
		 */
		void ExecuteAutoKeyguardSettingsL();
		
	private: // data
		CEikButtonGroupContainer* iPopoutCba;
		//Not owned!
		TTime* iAutoKeyguardValue;
		TTime iMaxPeriod;
	};

#endif // CAUTOKEYGUARDSETTINGPAGE_H

// End of file
