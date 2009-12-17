/*
* Copyright (c) 2006 - 2008 Nokia Corporation and/or its subsidiary(-ies).
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

#include <eikmenub.h>
#include <GSAutoKeyguardPluginRsc.rsg>
#include <e32base.h>
#include <AknQueryDialog.h>
#include "GSAutoKeyguardPluginAutoKeyguardTimeQuery.h"
#include "GSAutoKeyguardPluginAutoKeyguardSettingPage.h"

const TInt64 minPeriodInMicroseconds(5000000);

//
// ----------------------------------------------------------
// CAutoKeyguardSettingPage::CAutoKeyguardSettingPage()
// C++ default constructor
// ----------------------------------------------------------
// 
CAutoKeyguardSettingPage::CAutoKeyguardSettingPage(TInt aResourceId, 
                                                    TInt& aCurrentSelectionItem, 
                                                    CDesCArrayFlat* aItemArray, 
                                                    TTime* aAutoKeyguardValue,
                                                    TTime aMaxPeriod) : CAknRadioButtonSettingPage(aResourceId, aCurrentSelectionItem, aItemArray)					

	{	//c++ constructor may not contain any code which might leave
		iAutoKeyguardValue = aAutoKeyguardValue;
		iMaxPeriod = aMaxPeriod;
	}

//
// ----------------------------------------------------------
// CAutoKeyguardSettingPage::ConstructL()
// Symbian OS default constructor
// ----------------------------------------------------------
// 
void CAutoKeyguardSettingPage::ConstructL()
	{
	CAknRadioButtonSettingPage::ConstructL();
	const TSize screenSize = iCoeEnv->ScreenDevice()->SizeInPixels();
	}


//
// ----------------------------------------------------------
// CAutoKeyguardSettingPage::ProcessCommandL()
// 
// ----------------------------------------------------------
// 
void CAutoKeyguardSettingPage::ProcessCommandL(TInt aCommandId)
	{
	// Respond to softkey events

	switch (aCommandId)
		{
		case EAknSoftkeySelect:
		case EAknSoftkeyOk:
			ExecuteAutoKeyguardSettingsL();
			break;

		default:
			CAknSettingPage::ProcessCommandL(aCommandId);
			break;
		}

	}

//---------------------------------------------------------------------------------------
// CAutoKeyguardSettingPage::HandleListBoxEventL()
// 
//---------------------------------------------------------------------------------------
//
void CAutoKeyguardSettingPage::HandleListBoxEventL(CEikListBox* aListBox,
		MEikListBoxObserver::TListBoxEvent aEventType)
	{
	// Respond to double click events
	if ( aEventType == EEventEnterKeyPressed || aEventType == EEventItemSingleClicked )
		{
		ExecuteAutoKeyguardSettingsL();
		}
	}

//---------------------------------------------------------------------------------------
// CAutoKeyguardSettingPage::ExecuteAutoKeyguardSettingsL()
// 
//---------------------------------------------------------------------------------------
//
void CAutoKeyguardSettingPage::ExecuteAutoKeyguardSettingsL()
	{
	TInt cur = ListBoxControl()->CurrentItemIndex();
	
	// autoKeyguard off
	if (cur == 0)
		{
		*iAutoKeyguardValue = 0;
		AttemptExitL(ETrue);
		}
	// user defined
	if (cur == 1)
		{

		CAutoKeyguardTimeQuery* dlg = new (ELeave) CAutoKeyguardTimeQuery(*iAutoKeyguardValue, CAknQueryDialog::ENoTone);

		if(AknLayoutUtils::PenEnabled()) 
			{
			dlg->PrepareLC(R_AUTOKEYGUARD_TOUCH_TIME_SETTING_QUERY);
			}
		else
			{
			dlg->PrepareLC(R_AUTOKEYGUARD_TIME_SETTING_QUERY);
			}

		if(iMaxPeriod > 0)
			{
			TTime minimum(minPeriodInMicroseconds);
			dlg->SetMinimumAndMaximum(minimum, iMaxPeriod);
			}

		if(dlg->RunLD())
			{
			AttemptExitL(ETrue);
			}
		else
			AttemptExitL(EFalse);
		}
	}
// End of file

