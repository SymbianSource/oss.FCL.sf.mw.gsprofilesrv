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
*     CFLDPlayerBase is used to play sound files. It takes into account
*     the current volume and ringing type settings.
*
*
*/


// CLASS HEADER
#include "CFLDPlayerBase.h"

// INTERNAL INCLUDES
#include "ProfileEngineInternalCRKeys.h"

// EXTERNAL INCLUDES
#include <aknnotewrappers.h> // CAknErrorNote
#include <StringLoader.h>
#include <FileList.rsg>
#include <centralrepository.h>
#include <hwrmvibrasdkcrkeys.h>
#include <e32property.h>
#include <ctsydomainpskeys.h>

// CONSTANTS
namespace
	{
	const TInt KMinVolumeLevel( 1 );
	const TInt KMaxVolumeLevel( 10 );
	}

// ============================ MEMBER FUNCTIONS ===============================

// Destructor
CFLDPlayerBase::~CFLDPlayerBase()
    {
    CCoeEnv::Static()->RemoveForegroundObserver( *this );

    iApaSession.Close();

	if( iVibraNotifyHandler )
		{
		iVibraNotifyHandler->StopListening();
		}

	if( iTypeNotifyHandler )
		{
		iTypeNotifyHandler->StopListening();
		}

	if( iVolumeNotifyHandler )
		{
		iVolumeNotifyHandler->StopListening();
		}

    delete iVibraNotifyHandler;
	delete iTypeNotifyHandler;
    delete iVolumeNotifyHandler;

    delete iSession;
    delete iVibraSession;

    delete iErrorMessage;
	iErrorMessage = NULL;
    }

// -----------------------------------------------------------------------------
// CFLDPlayerBase::CFLDPlayerBase
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CFLDPlayerBase::CFLDPlayerBase( TBool aShowErrorMsgs )
    :   i3dEffects( ETrue ),
        iShowErrorMsgs( aShowErrorMsgs )
    {
    }

// -----------------------------------------------------------------------------
// CFLDPlayerBase::BaseConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CFLDPlayerBase::BaseConstructL()
    {
	CCoeEnv* coeEnv = CCoeEnv::Static();

    coeEnv->AddForegroundObserverL( *this );

    // Read a text string from resource file
    if ( iShowErrorMsgs )
        {
		iErrorMessage = StringLoader::LoadL( R_FLD_QTN_FILE_FORMAT_ERROR );
        }

    // Get initial settings from active profile
    // init cenrep connection
    iSession = CRepository::NewL(KCRUidProfileEngine);
    iVibraSession = CRepository::NewL(KCRUidVibraCtrl);

    iVolumeNotifyHandler = CCenRepNotifyHandler::NewL(*this,
                                                     *iSession,
                                                     CCenRepNotifyHandler::EIntKey,
                                                     KProEngActiveRingingVolume);

	iTypeNotifyHandler = CCenRepNotifyHandler::NewL(*this,
                                                     *iSession,
                                                     CCenRepNotifyHandler::EIntKey,
                                                     KProEngActiveRingingType);

	iVibraNotifyHandler = CCenRepNotifyHandler::NewL(*this,
                                                     *iVibraSession,
                                                     CCenRepNotifyHandler::EIntKey,
                                                     KVibraCtrlProfileVibraEnabled);


	User::LeaveIfError( iSession->Get( KProEngActiveRingingVolume, iRingingVolume ) );
    User::LeaveIfError( iSession->Get( KProEngActiveRingingType, iRingingType ) );
    User::LeaveIfError( iVibraSession->Get( KVibraCtrlProfileVibraEnabled, iVibra ) );

    User::LeaveIfError( iSession->Get( KProEngDefaultRingingTone, iDefaultTone ) );

    iVolumeNotifyHandler->StartListeningL();
    iTypeNotifyHandler->StartListeningL();
    iVibraNotifyHandler->StartListeningL();

    User::LeaveIfError( iApaSession.Connect() );
    }

// -----------------------------------------------------------------------------
// CFLDPlayerBase::SetVolume()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDPlayerBase::SetVolume( TInt aVolume )
    {
   iRingingVolume = aVolume;
        	
	// CCenrepNotifyHandler doesn't allow to stop only iVolumeNotifyHandler
	// but we must stop all the handlers in correct order
	if( iVibraNotifyHandler )
		{
		iVibraNotifyHandler->StopListening();
		}

	if( iTypeNotifyHandler )
		{
		iTypeNotifyHandler->StopListening();
		}

	if( iVolumeNotifyHandler )
		{
		iVolumeNotifyHandler->StopListening();
		delete iVolumeNotifyHandler;
        iVolumeNotifyHandler = NULL;
		}
	
	// Start handlers again which were stopped
	// No possibility to handle leave here any way
	if( iTypeNotifyHandler )
		{
        TRAP_IGNORE( iTypeNotifyHandler->StartListeningL() );
        }
	if( iVibraNotifyHandler )
		{
        TRAP_IGNORE( iVibraNotifyHandler->StartListeningL() );
        }
    }

// -----------------------------------------------------------------------------
// CFLDPlayerBase::SetRingingType()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDPlayerBase::SetRingingType( TInt aRingingType )
    {
    iRingingType = aRingingType;
	if( iVibraNotifyHandler )
		{
		iVibraNotifyHandler->StopListening();
		}

	if( iTypeNotifyHandler )
		{
		iTypeNotifyHandler->StopListening();
		delete iTypeNotifyHandler;
        iTypeNotifyHandler = NULL;
		}

	if( iVolumeNotifyHandler )
		{
		iVolumeNotifyHandler->StopListening();
		}
	
	if( iVolumeNotifyHandler )
		{
        TRAP_IGNORE( iVolumeNotifyHandler->StartListeningL() );
        }
        
	if( iVibraNotifyHandler )
		{
        TRAP_IGNORE( iVibraNotifyHandler->StartListeningL() );
        }
    }

// -----------------------------------------------------------------------------
// CFLDPlayerBase::SetVibra()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDPlayerBase::SetVibra( TBool aVibra )
    {
    iVibra = aVibra;
	if( iVibraNotifyHandler )
		{
		iVibraNotifyHandler->StopListening();
		delete iVibraNotifyHandler; 
        iVibraNotifyHandler = NULL; 
		}

	if( iTypeNotifyHandler )
		{
		iTypeNotifyHandler->StopListening();
		}

	if( iVolumeNotifyHandler )
		{
		iVolumeNotifyHandler->StopListening();
		}
	
	if( iVolumeNotifyHandler )
		{
        TRAP_IGNORE( iVolumeNotifyHandler->StartListeningL() );
        }
	if( iTypeNotifyHandler )
		{
        TRAP_IGNORE( iTypeNotifyHandler->StartListeningL() );
        }
    }

// -----------------------------------------------------------------------------
// CFLDPlayerBase::Set3dEffects()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDPlayerBase::Set3dEffects( TBool a3dEffects )
	{
	i3dEffects = a3dEffects;
	}

// -----------------------------------------------------------------------------
// CFLDPlayerBase::BaseConvertVolume()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CFLDPlayerBase::BaseConvertVolume( TInt aVolume, TInt aMaxVolume )
    {
    TInt result( aMaxVolume * aVolume / KMaxVolumeLevel );
    // if user has selected minimum volume level
    // set HW volume 1
    if ( aVolume == KMinVolumeLevel && result == 0 )
        {
        result = 1;
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CFLDPlayerBase::DisplayErrorNoteL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDPlayerBase::DisplayErrorNoteL()
    {
    if( iShowErrorMsgs )
        {
        CAknErrorNote* note = new( ELeave ) CAknErrorNote( ETrue );
        note->ExecuteLD( *iErrorMessage );
        }
    }

// -----------------------------------------------------------------------------
// CFLDPlayerBase::DataType()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CFLDPlayerBase::DataType( const TDesC& aFileName, TDataType& aDataType )
    {
   	TUid dummyUid( KNullUid );
	return iApaSession.AppForDocument( aFileName, dummyUid, aDataType );
    }

// -----------------------------------------------------------------------------
// CFLDPlayerBase::HandleNotifyInt()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDPlayerBase::HandleNotifyInt(TUint32 aId, TInt aNewValue)
    {

   	if ( aId == KProEngActiveRingingVolume )
   		{
   		iRingingVolume = aNewValue;
   		}
   	else if ( aId == KProEngActiveRingingType )
		{
   		iRingingType = aNewValue;
   		}
   	else if ( aId == KVibraCtrlProfileVibraEnabled )
   		{
   		iVibra = aNewValue;
   		}

    }

// -----------------------------------------------------------------------------
// CFLDPlayerBase::HandleLosingForeground()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDPlayerBase::HandleLosingForeground()
    {
    iFocusLost = ETrue;
    Cancel();
    }

// -----------------------------------------------------------------------------
// CFLDPlayerBase::HandleGainingForeground()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDPlayerBase::HandleGainingForeground()
    {
    iFocusLost = EFalse;
    }

// -----------------------------------------------------------------------------
// CFLDPlayerBase::IsCallOngoing()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CFLDPlayerBase::IsCallOngoing() const
    {
    // Get call status
    TBool res = EFalse;
    TInt err = KErrNone;
    TInt callState = EPSCTsyCallStateUninitialized; 

    err = RProperty::Get( KPSUidCtsyCallInformation, KCTsyCallState, callState );
    if( !err &&
         ( callState != EPSCTsyCallStateUninitialized && 
           callState != EPSCTsyCallStateNone && 
           callState != EPSCTsyCallStateDisconnecting ) ) 
        {
        // There was call ongoing
        res = ETrue;
        }

    return res;
    }


//  End of File

