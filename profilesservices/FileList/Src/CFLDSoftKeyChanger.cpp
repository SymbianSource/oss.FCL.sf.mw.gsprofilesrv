/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* 	CFLDSoftKeyChanger changes softkeys into video preview layout and
* 	video preview select layout. Allows also to restore old layout.
*
*
*/

//  CLASS HEADER
#include "CFLDSoftKeyChanger.h"

//	EXTERNAL INCLUDES
#include <f32file.h> 	// For TParsePtrC
#include <StringLoader.h>
#include <eikbtgpc.h> 	// For CEikButtonGroupContainer
#include <filelist.rsg>	// For R_FLD_SOFTKEYS_PREVIEW_CANCEL
#include <avkon.rsg>	// For R_AVKON_SOFTKEYS_SELECT_CANCEL
#include <avkon.hrh>	// For EAknSoftkeySelect

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CFLDSoftKeyChanger::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CFLDSoftKeyChanger* CFLDSoftKeyChanger::NewL()
    {
    CFLDSoftKeyChanger* self = CFLDSoftKeyChanger::NewLC();
    CleanupStack::Pop( self ); // self

    return self;
    }

// -----------------------------------------------------------------------------
// CFLDSoftKeyChanger::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CFLDSoftKeyChanger* CFLDSoftKeyChanger::NewLC()
    {
    CFLDSoftKeyChanger* self = new( ELeave ) CFLDSoftKeyChanger();
    CleanupStack::PushL( self );

    self->ConstructL( );
    return self;
    }

// Destructor
CFLDSoftKeyChanger::~CFLDSoftKeyChanger()
    {
    delete iSoftKeySelect;
    delete iSoftKeyPreview;
    delete iSoftKeyCancel;
    delete iSoftKeyBack;
    }


// -----------------------------------------------------------------------------
// CFLDSoftKeyChanger::CFLDSoftKeyChanger
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CFLDSoftKeyChanger::CFLDSoftKeyChanger()
	: iSoftKeyState( MFLDFileListBoxObserver::EToneSelectionSoftKeyState )
    {
    }

// -----------------------------------------------------------------------------
// CFLDSoftKeyChanger::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CFLDSoftKeyChanger::ConstructL()
    {
    iSoftKeySelect = StringLoader::LoadL( R_FLD_QTN_TEXT_SOFTKEY_PREVIEW_SELECT );
    iSoftKeyPreview = StringLoader::LoadL( R_FLD_QTN_TEXT_SOFTKEY_PREVIEW );
    iSoftKeyCancel = StringLoader::LoadL( R_FLD_QTN_TEXT_SOFTKEY_CANCEL );
    iSoftKeyBack = StringLoader::LoadL( R_FLD_QTN_TEXT_SOFTKEY_BACK );
    }

// -----------------------------------------------------------------------------
// CFLDSoftKeyChanger::ChangeLeftSoftKeyL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDSoftKeyChanger::ChangeLeftSoftKeyL( const TDesC& aSoftKeyLabel ) const
	{
	CEikButtonGroupContainer* cba = CEikButtonGroupContainer::Current();
 	CleanupStack::PushL( cba );
	cba->SetCommandL( EAknSoftkeySelect, aSoftKeyLabel );
	CleanupStack::Pop( cba );
	cba->DrawDeferred();
	}

// -----------------------------------------------------------------------------
// CFLDSoftKeyChanger::ChangeRightSoftKeyL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDSoftKeyChanger::ChangeRightSoftKeyL( const TDesC& aSoftKeyLabel ) const
	{
	CEikButtonGroupContainer* cba = CEikButtonGroupContainer::Current();
 	CleanupStack::PushL( cba );
	cba->SetCommandL( EAknSoftkeyCancel, aSoftKeyLabel );
	CleanupStack::Pop( cba );
	cba->DrawDeferred();
	}


// -----------------------------------------------------------------------------
// CFLDSoftKeyChanger::ChangeNewSoftKeysPreviewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDSoftKeyChanger::ChangeNewSoftKeysPreviewL()
	{
	if (iSoftKeyState == MFLDFileListBoxObserver::EPreviewSoftKeyState )
		{
		return;
		}
		
    CFLDSoftKeyChanger::ChangeLeftSoftKeyL( iSoftKeyPreview->Des() );
    CFLDSoftKeyChanger::ChangeRightSoftKeyL( iSoftKeyCancel->Des() );
    
	iSoftKeyState = MFLDFileListBoxObserver::EPreviewSoftKeyState;
    }

// -----------------------------------------------------------------------------
// CFLDSoftKeyChanger::ChangeNewSoftKeysPreviewSelectL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDSoftKeyChanger::ChangeNewSoftKeysPreviewSelectL()
	{
	if (iSoftKeyState == MFLDFileListBoxObserver::EPreviewSelectSoftKeyState )
		{
		return;
		}
		
    CFLDSoftKeyChanger::ChangeLeftSoftKeyL( iSoftKeySelect->Des() );
    CFLDSoftKeyChanger::ChangeRightSoftKeyL( iSoftKeyBack->Des() );
  
	iSoftKeyState = MFLDFileListBoxObserver::EPreviewSelectSoftKeyState;
	}

// -----------------------------------------------------------------------------
// CFLDSoftKeyChanger::RestoreOldSoftKeysL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDSoftKeyChanger::RestoreOldToneSelectionSoftKeysL()
	{
	if (iSoftKeyState == MFLDFileListBoxObserver::EToneSelectionSoftKeyState )
		{
		return;
		}
		
    CFLDSoftKeyChanger::ChangeLeftSoftKeyL( iSoftKeySelect->Des() );
    CFLDSoftKeyChanger::ChangeRightSoftKeyL( iSoftKeyCancel->Des() );

	iSoftKeyState = MFLDFileListBoxObserver::EToneSelectionSoftKeyState;
	}

// -----------------------------------------------------------------------------
// CFLDSoftKeyChanger::SoftKeyState
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MFLDFileListBoxObserver::TFileListSoftKeyState
 CFLDSoftKeyChanger::SoftKeyState()	const
	{
	return iSoftKeyState;
	}

// End of File
