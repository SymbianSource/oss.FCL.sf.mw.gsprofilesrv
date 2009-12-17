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
* Description: Implementation of the CFLDWaitNote.
*
*/



// CLASS HEADER
#include "CFLDWaitNote.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CFLDWaitNote::CFLDWaitNote
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CFLDWaitNote::CFLDWaitNote( const TInt aWaitNoteResourceId )
	: iWaitNoteResourceId (aWaitNoteResourceId)
    {
    }

// -----------------------------------------------------------------------------
// CFLDWaitNote::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CFLDWaitNote* CFLDWaitNote::NewL( const TInt aWaitNoteResourceId )
    {
    CFLDWaitNote* self = new (ELeave) CFLDWaitNote( aWaitNoteResourceId );
    return self;
    }

// Destructor
CFLDWaitNote::~CFLDWaitNote()
    {
    if( iWaitDialog )
        {
        // Do not delete CAknWaitDialog object:
        // CAknWaitDialog::ProcessFinishedL should be used instead.
        TRAP_IGNORE( iWaitDialog->ProcessFinishedL() );
        }
    }

// -----------------------------------------------------------------------------
// CFLDWaitNote::DialogDismissedL
// Gets called when the dialog is dismissed/closed.
// -----------------------------------------------------------------------------
//
void CFLDWaitNote::DialogDismissedL( const TInt /*aButtonId*/ )
    {
    }

// -----------------------------------------------------------------------------
// CFLDWaitNote::CloseWaitNoteL
// Close the wait note dialog.
// -----------------------------------------------------------------------------
//
void CFLDWaitNote::CloseWaitNoteL()
    {
    // Close and delete the wait note dialog,
    // if it has not been dismissed yet
    if( iWaitDialog )
        {
        iWaitDialog->ProcessFinishedL();
        }
    }

// -----------------------------------------------------------------------------
// CFLDWaitNote::OpenWaitNoteL
// Open the wait note dialog.
// -----------------------------------------------------------------------------
//
void CFLDWaitNote::OpenWaitNoteL()
    {
    CloseWaitNoteL();

    // Create and view the wait note dialog
    iWaitDialog = new (ELeave) CAknWaitDialog(
        reinterpret_cast<CEikDialog**>( &iWaitDialog ) );
    iWaitDialog->SetCallback( this );
    iWaitDialog->ExecuteLD( iWaitNoteResourceId ); // CSI: 50 # Pointer to iWaitDialog variable is given to iWaitDialog itself, so won't dare to set it NULL below
    }

// -----------------------------------------------------------------------------
// CFLDWaitNote::IsRunning
// Gets called by the model to see if the wait note (refresh) is already running.
// -----------------------------------------------------------------------------
//
TBool CFLDWaitNote::IsRunning()
    {
    if( iWaitDialog == NULL )
        {
        return EFalse;
        }
    else
        {
        return ETrue;
        }
    }

//  End of File
