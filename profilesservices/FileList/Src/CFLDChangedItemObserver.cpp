/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Implementation of the CFLDChangedItemObserver.
*
*/



// CLASS HEADER
#include "CFLDChangedItemObserver.h"

// INTERNAL INCLUDES
#include "CFLDFileListModel.h"
#include "CFLDWaitNote.h"
#include "CFLDPopupList.h"
#include "CFLDController.h"

const TInt KDefaultDelay( 3000000 );
// EXTERNAL INCLUDES

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CFLDChangedItemObserver::CFLDChangedItemObserver
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CFLDChangedItemObserver::CFLDChangedItemObserver()
	: iModel( NULL )
    {
    }

// -----------------------------------------------------------------------------
// CFLDChangedItemObserver::CFLDChangedItemObserver
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CFLDChangedItemObserver::CFLDChangedItemObserver(CFLDWaitNote* aWaitNote)
	: iModel( NULL ),
	iDelay(KDefaultDelay)
    {
    }

// -----------------------------------------------------------------------------
// CFLDChangedItemObserver::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CFLDChangedItemObserver* CFLDChangedItemObserver::NewL()
    {
    CFLDChangedItemObserver* self = new (ELeave) CFLDChangedItemObserver();
    return self;
    }

// -----------------------------------------------------------------------------
// CFLDChangedItemObserver::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CFLDChangedItemObserver* CFLDChangedItemObserver::NewL(CFLDWaitNote* aWaitNote)
	{
	CFLDChangedItemObserver* self = new (ELeave) CFLDChangedItemObserver(aWaitNote);
	CleanupStack::PushL( self );
	self->ConstructL(aWaitNote);
	CleanupStack::Pop( self );
	return self;
	
	}

// -----------------------------------------------------------------------------
// CFLDChangedItemObserver::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CFLDChangedItemObserver::ConstructL( CFLDWaitNote* aWaitNote)
	{
	iTimer = CPeriodic::NewL( EPriorityNormal );
	iWaitNote = aWaitNote;
	}

// Destructor
CFLDChangedItemObserver::~CFLDChangedItemObserver()
    {
    if (iTimer)
    	{
    	  iTimer->Cancel();
    	  delete iTimer;
    	}
  
    }

// -----------------------------------------------------------------------------
// CFLDFileListModel::SetFileListModel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDChangedItemObserver::SetFileListModel( CFLDFileListModel* aModel )
	{
	iModel = aModel;	
	}

// -----------------------------------------------------------------------------
// CFLDFileListModel::HandleItemChangeL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDChangedItemObserver::HandleItemChangeL( const TArray<TCLFItemId>& /*aItemIDArray*/ )
	{
	if( iModel )
		{
		TRAP_IGNORE( static_cast<CFLDController*>(iModel->GetPopupList()->GetFileListBoxObserver())->Release());
		}
	
	if( iWaitNote && !iWaitNote->IsRunning() )
		{
		iWaitNote->OpenWaitNoteL();
		}

	
    if (iModel)
    	{
    	iTimer->Cancel();
		iTimer->Start( iDelay, iDelay, TCallBack( HandleModelRefreshL, this ) ); // CSI: 10 # iTimer is cancelled in Release()
		
		// When video screen is closed,
		// listbox must be forced to be refreshed
		// ( otherwise scrollbar is not redrawn)
		iModel->GetPopupList()->ListBox()->SetDimmed( EFalse );
		iModel->GetPopupList()->ListBox()->ScrollBarFrame()->VerticalScrollBar()->SetDimmed( EFalse );
		iModel->GetPopupList()->DrawNow( CEikonEnv::Static()->EikAppUi()->ApplicationRect() );
		}
	}

// -----------------------------------------------------------------------------
// CFLDFileListModel::HandleModelRefreshL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CFLDChangedItemObserver::HandleModelRefreshL( TAny* aPtr )
	{
	CFLDChangedItemObserver* observer= reinterpret_cast< CFLDChangedItemObserver* >( aPtr );
	observer->iTimer->Cancel();
	observer->iModel->RefreshEntryListL();
	
	return 0;
	}

// -----------------------------------------------------------------------------
// CFLDFileListModel::HandleError
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDChangedItemObserver::HandleError( TInt /*aError*/ )
	{
	}
//  End of File
