/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of CProEngMasterSettingsEventDelegate
*
*/



// INCLUDE FILES
#include    "CProEngMasterSettingsEventDelegate.h"
#include    <MProEngMasterSettingsObserver.h>

namespace
    {
    // CONSTANTS
    const TInt KProEngMasterSettingsPartialKey( 0x80000200 );
    const TUint32 KProEngMasterSettingsMask( 0xFFFFFF00 );
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProEngMasterSettingsEventDelegate::CProEngMasterSettingsEventDelegate
// -----------------------------------------------------------------------------
//
CProEngMasterSettingsEventDelegate::CProEngMasterSettingsEventDelegate(
        TUint32 aPartialCenRepKey,
        TUint32 aKeyMask,
        MProEngMasterSettingsObserver& aObserver )
    : CProEngCenRepObserverBase( aPartialCenRepKey, aKeyMask ),
      iObserver( aObserver )
    {
    }

// -----------------------------------------------------------------------------
// CProEngMasterSettingsEventDelegate::NewL
// -----------------------------------------------------------------------------
//
CProEngMasterSettingsEventDelegate* CProEngMasterSettingsEventDelegate::NewL(
          MProEngMasterSettingsObserver& aObserver )
    {
    CProEngMasterSettingsEventDelegate* self = new ( ELeave )
        CProEngMasterSettingsEventDelegate( KProEngMasterSettingsPartialKey,
                                        KProEngMasterSettingsMask, aObserver );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CProEngMasterSettingsEventDelegate::NotifyObserverL
// -----------------------------------------------------------------------------
//
void CProEngMasterSettingsEventDelegate::NotifyObserverL()
    {
    iObserver.HandleMasterSettingsModifiedL( );
    }

// -----------------------------------------------------------------------------
// CProEngMasterSettingsEventDelegate::NotifyError
// -----------------------------------------------------------------------------
//
void CProEngMasterSettingsEventDelegate::NotifyError( TInt aError )
    {
    iObserver.HandleMasterSettingsNotificationError( aError );
    }

//  End of File

