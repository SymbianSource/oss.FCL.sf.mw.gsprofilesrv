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
* Description:  Implementation of CProEngActiveSettingsEventDelegate
*
*/



// INCLUDE FILES
#include    "CProEngActiveSettingsEventDelegate.h"
#include    <MProEngActiveSettingsObserver.h>

namespace
    {
    // CONSTANTS
    const TInt KProEngActiveId( 0x7E000000 );
    const TUint32 KProEngProfileIdMask( 0xFF000000 );
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProEngActiveSettingsEventDelegate::CProEngActiveSettingsEventDelegate
// -----------------------------------------------------------------------------
//
CProEngActiveSettingsEventDelegate::CProEngActiveSettingsEventDelegate(
        TUint32 aPartialCenRepKey,
        TUint32 aKeyMask,
        MProEngActiveSettingsObserver& aObserver )
    : CProEngCenRepObserverBase( aPartialCenRepKey, aKeyMask ),
      iObserver( aObserver )
    {
    }

// -----------------------------------------------------------------------------
// CProEngActiveSettingsEventDelegate::NewL
// -----------------------------------------------------------------------------
//
CProEngActiveSettingsEventDelegate* CProEngActiveSettingsEventDelegate::NewL(
          MProEngActiveSettingsObserver& aObserver )
    {
    CProEngActiveSettingsEventDelegate* self = new ( ELeave )
        CProEngActiveSettingsEventDelegate( KProEngActiveId,
                                     KProEngProfileIdMask, aObserver );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CProEngActiveSettingsEventDelegate::NotifyObserverL
// -----------------------------------------------------------------------------
//
void CProEngActiveSettingsEventDelegate::NotifyObserverL()
    {
    iObserver.HandleActiveSettingsModifiedL( );
    }

// -----------------------------------------------------------------------------
// CProEngActiveSettingsEventDelegate::NotifyError
// -----------------------------------------------------------------------------
//
void CProEngActiveSettingsEventDelegate::NotifyError( TInt aError )
    {
    iObserver.HandleActiveSettingsNotificationError( aError );
    }

//  End of File

