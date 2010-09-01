/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Waiting note for query
*
*/



// INCLUDE FILES
#include "mediafilewaitnote.h"
#include <mediafilelist.rsg>

#include <eikprogi.h>
#include <notecontrol.h>
#include <aknappui.h>   // macro iAvkonAppUi





/*******************************************************************************
 * class CMediaFileWaitNote
 *******************************************************************************/


// -----------------------------------------------------------------------------
// CMediaFileWaitNote::CMediaFileWaitNote
//
// -----------------------------------------------------------------------------
//
CMediaFileWaitNote::CMediaFileWaitNote( MMediaFileWaitNoteObserver* aObserver )
: iObserver( aObserver )
    {
    }


// -----------------------------------------------------------------------------
// CMediaFileWaitNote::NewL
//
// -----------------------------------------------------------------------------
//
CMediaFileWaitNote* CMediaFileWaitNote::NewL( MMediaFileWaitNoteObserver* aObserver )
    {
    CMediaFileWaitNote* self = new (ELeave) CMediaFileWaitNote( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return( self );
    }


// -----------------------------------------------------------------------------
// CMediaFileWaitNote::ConstructL
//
// -----------------------------------------------------------------------------
//
void CMediaFileWaitNote::ConstructL()
    {
    iObserverCallNeeded = ETrue;
    iDialogDismissed = EFalse;
    iLastText = KNullDesC;
	iAnimation = EFalse;
    iImageFile = KNullDesC;
    } 


// -----------------------------------------------------------------------------
// Destructor
//
// -----------------------------------------------------------------------------
//
CMediaFileWaitNote::~CMediaFileWaitNote()
    {
    delete iProgressDialog;
    }


// -----------------------------------------------------------------------------
// CMediaFileWaitNote::LaunchL
//
// -----------------------------------------------------------------------------
//
void CMediaFileWaitNote::LaunchL( const TDesC& aLabel )
    {
    if ( iProgressDialog )
        {
        // CAknNoteDialog::~CAknNoteDialog sets iProgressDialog to NULL after:
        // 1. CAknProgressDialog::ProcessFinishedL is called
        // 2. dialog is closed using Cancel button
        return;
        }
    
    iObserverCallNeeded = ETrue;
    iDialogDismissed = EFalse;
    
    iProgressDialog = new(ELeave) 
    CAknProgressDialog( reinterpret_cast<CEikDialog**>(&iProgressDialog), EFalse );
    
    iProgressDialog->PrepareLC( R_MEDIA_FILE_WAIT_NOTE );
    iProgressDialog->SetTextL( aLabel );
    
    if ( iImageFile.Length() > 0 )
        {
        CEikImage* image = TMFDialogUtil::CreateImageLC( iImageId, iImageFile,
                           iBitmapId,  iBitmapMaskId );
        iProgressDialog->SetImageL( image );
        CleanupStack::Pop( image );
        }

    // get reference to progress info bar
    //iProgressInfo = iProgressDialog->GetProgressInfoL();

    iProgressDialog->SetCallback( this );
    
    iProgressDialog->RunLD();
    }


// -----------------------------------------------------------------------------
// CMediaFileWaitNote::SetImageL
//
// -----------------------------------------------------------------------------
//
void CMediaFileWaitNote::SetImageL( TAknsItemID aId, const TDesC& aImageFile,
                                    TInt aBitmapId, TInt aBitmapMaskId )
	{
	if ( aImageFile.Length() > iImageFile.MaxLength() )
        {
        return;
        }
    iImageFile = aImageFile;
	iImageId = aId;
	iBitmapId = aBitmapId;
	iBitmapMaskId = aBitmapMaskId;
    }


// -----------------------------------------------------------------------------
// CMediaFileWaitNote::CancelProgressDialogL
//
// -----------------------------------------------------------------------------
//
void CMediaFileWaitNote::CancelProgressDialogL()
    {
    // this function is called from CMediaFileDialog so there is no need 
    // to notify observer (CMediaFileDialog)
    iObserverCallNeeded = EFalse;
    
    if ( iDialogDismissed )
        {
        return; // dialog is already closed
        }
    
	if ( !iProgressDialog )
		{
		return; // dialog is already closed
		}
		
	iProgressDialog->ProcessFinishedL();
    }


// -----------------------------------------------------------------------------
// CMediaFileWaitNote::SetTextL
//
// -----------------------------------------------------------------------------
//
void CMediaFileWaitNote::SetTextL( const TDesC& aText )
    {
    if ( !iProgressDialog )
        {
        return;
        }

    if ( iLastText.Compare( aText ) == 0 )
		{
		return;  // text has not changed
		}
    
    iProgressDialog->SetTextL( aText );
	TMFDialogUtil::StrCopy( iLastText, aText );
    }


// -----------------------------------------------------------------------------
// CMediaFileWaitNote::SetProgress
//
// -----------------------------------------------------------------------------
//
void CMediaFileWaitNote::SetProgress( TInt aValue )
    {
	if ( !iProgressDialog  || !iProgressInfo )
	    {
	    return;
	    }

	CEikProgressInfo::SInfo info = iProgressInfo->Info();

	TInt val = aValue;
	if ( val > info.iFinalValue )
		{
		val = info.iFinalValue;
		}
	
	iProgressInfo->SetAndDraw( val );
    }


// -----------------------------------------------------------------------------
// CMediaFileWaitNote::SetFinalProgress
//
// -----------------------------------------------------------------------------
//
void CMediaFileWaitNote::SetFinalProgress( TInt aFinalValue )
	{
    if ( !iProgressDialog  || !iProgressInfo )
        {
        return;
        }

    if ( aFinalValue < 0 )
    	{
    	iProgressInfo->SetFinalValue( 0 );
    	}
    else
    	{
    	iProgressInfo->SetFinalValue( aFinalValue );
    	}
    	
	iProgressInfo->SetAndDraw( 0 );
	}


// -----------------------------------------------------------------------------
// CMediaFileWaitNote::NoteControl
//
// -----------------------------------------------------------------------------
//
CAknNoteControl* CMediaFileWaitNote::NoteControl()
	{
	if ( !iProgressDialog )
	    {
	    return NULL;
	    }
    CAknNoteControl* note = STATIC_CAST(CAknNoteControl*, iProgressDialog->ControlOrNull(EGeneralNote));

    return note;
	}


// -----------------------------------------------------------------------------
// CMediaFileWaitNote::CancelAnimation
//
// -----------------------------------------------------------------------------
//
void CMediaFileWaitNote::CancelAnimation()
	{
    iAnimation = EFalse;
	CAknNoteControl* note = NoteControl();
	if ( note )
	    {
	    note->CancelAnimation();
	    }
    }


// ----------------------------------------------------------------------------
// CMediaFileWaitNote::StartAnimationL
//
// ----------------------------------------------------------------------------
//
void CMediaFileWaitNote::StartAnimationL()
	{
    if ( !iAnimation )
		{
	    CAknNoteControl* note = NoteControl();
	    if ( note )
	        {
	        note->StartAnimationL();
	        iAnimation = ETrue;
	        }
		}
    }


// -----------------------------------------------------------------------------
// CMediaFileWaitNote::DialogDismissedL (From MProgressDialogCallback)
//
// -----------------------------------------------------------------------------
//
void CMediaFileWaitNote::DialogDismissedL( TInt aButtonId )
    {
    iDialogDismissed = ETrue;
    
    if ( !iObserver )
		{
		return;
		}
	
    if ( !iObserverCallNeeded )
        {
        return;
        }
    
    TRAP_IGNORE( iObserver->HandleWaitNoteL( aButtonId ) );
    }


// -----------------------------------------------------------------------------
// CMediaFileWaitNote::HideButtonL
//
// -----------------------------------------------------------------------------
//
void CMediaFileWaitNote::HideButtonL()
    {
    const TInt KLastButtonIndex = 2;
    
   	if ( !iProgressDialog )
   	    {
   	    return;
   	    }
   	
    CEikButtonGroupContainer& container = iProgressDialog->ButtonGroupContainer();
	
	container.RemoveCommandFromStack( KLastButtonIndex, EAknSoftkeyCancel );
	container.AddCommandToStackL( KLastButtonIndex, EAknSoftkeyEmpty, KNullDesC );
	container.DrawDeferred();
    }



//  End of File  
