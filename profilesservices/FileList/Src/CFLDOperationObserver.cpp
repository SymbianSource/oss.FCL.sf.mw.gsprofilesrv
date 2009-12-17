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
* Description: Implementation of CFLDOperationObserver.
*
*/



// CLASS HEADER
#include "CFLDOperationObserver.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CFLDOperationObserver::CFLDOperationObserver
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CFLDOperationObserver::CFLDOperationObserver()
    {
    }

// -----------------------------------------------------------------------------
// CFLDOperationObserver::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CFLDOperationObserver* CFLDOperationObserver::NewL()
    {
    CFLDOperationObserver* self = new (ELeave) CFLDOperationObserver;
    return self;
    }

// Destructor
CFLDOperationObserver::~CFLDOperationObserver()
    {
    }

// -----------------------------------------------------------------------------
// CFLDOperationObserver::HandleOperationEventL
// Method to handle list model operation events.
// -----------------------------------------------------------------------------
//
void CFLDOperationObserver::HandleOperationEventL(
                                TCLFOperationEvent aOperationEvent,
                                TInt /*aError*/ )
    {
    // Waiting is stopped when an event for refresh completion is received
    if( aOperationEvent == ECLFRefreshComplete )
        {
        iRefreshOngoing = EFalse;

        if( iWait && iWait->IsStarted() )
            {
            iWait->AsyncStop();
            }
        }
    }

// -----------------------------------------------------------------------------
// CFLDOperationObserver::IsRefreshOngoing
// Check if there is a refresh operation is ongoing.
// -----------------------------------------------------------------------------
//
TBool CFLDOperationObserver::IsRefreshOngoing()
    {
    return iRefreshOngoing;
    }

// -----------------------------------------------------------------------------
// CFLDOperationObserver::PrepareForRefresh
// Prepare for a refresh operation.
// -----------------------------------------------------------------------------
//
void CFLDOperationObserver::PrepareForRefresh( CActiveSchedulerWait& aWait )
    {
    // Store the wait instance
    iWait = &aWait;

    // Refresh operation will be started in a moment
    iRefreshOngoing = ETrue;
    }    

//  End of File
