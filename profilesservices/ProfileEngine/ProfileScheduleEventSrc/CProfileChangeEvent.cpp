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
* Description:  Handles reactivation of the previous profile when the scheduler
*                runs.
*
*/


// INCLUDE FILES
#include    "CProfileChangeEvent.h"

#include    <e32svr.h>
#include    <MProfileEngine.h>
#include    <AknGlobalNote.h>
#include    <data_caging_path_literals.hrh>
#include    <centralrepository.h>
#include    <barsc.h>
#include    <bautils.h>

#include	<SecondaryDisplay/SecondaryDisplayProfilesAPI.h>
#include	<aknSDData.h>
#include	<featmgr.h>

#include    <profilescheduleevent.rsg>
#include    "ProfileEnginePrivateCRKeys.h"
#include    <ProfileEngineConstants.h>
#include    "ProfilesDebug.h"


// CONSTANTS
namespace
    {
    _LIT( KProfileScheduleEventResourceDriveAndDir, "Z:ProfileScheduleEvent.rsc" );
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileChangeEvent::CProfileChangeEvent
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CProfileChangeEvent::CProfileChangeEvent()
    {
    }

// -----------------------------------------------------------------------------
// CProfileChangeEvent::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProfileChangeEvent::ConstructL()
    {
    FeatureManager::InitializeLibL();
    iCoverDisplay = FeatureManager::FeatureSupported( KFeatureIdCoverDisplay );
    
    
    User::LeaveIfError( iFs.Connect() );
    // Open/Create mutex
    TInt error( KErrNotFound );
    while( error == KErrNotFound )
        {
        error = iMutex.CreateGlobal( KProfileMutexName );
        if( error != KErrAlreadyExists )
            {
            break;
            }
        error = iMutex.OpenGlobal( KProfileMutexName );
        }
    User::LeaveIfError( error );
    iProfileEngine = CreateProfileEngineL( &iFs );
    iGlobalNote = CAknGlobalNote::NewL();
    ReadResourcesL();
    }

// -----------------------------------------------------------------------------
// CProfileChangeEvent::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProfileChangeEvent* CProfileChangeEvent::NewL()
    {
    CProfileChangeEvent* self = CProfileChangeEvent::NewLC();
    CleanupStack::Pop();

    return self;
    }

// -----------------------------------------------------------------------------
// CProfileChangeEvent::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProfileChangeEvent* CProfileChangeEvent::NewLC()
    {
    CProfileChangeEvent* self = new( ELeave ) CProfileChangeEvent;

    CleanupStack::PushL( self );
    self->ConstructL();

    return self;
    }


// Destructor
CProfileChangeEvent::~CProfileChangeEvent()
    {
    delete iNoteText;
    delete iGlobalNote;
    if( iProfileEngine )
        {
        iProfileEngine->Release();
        }
    iFs.Close();
    iMutex.Close();
    
    FeatureManager::UnInitializeLib();
    }


// -----------------------------------------------------------------------------
// CProfileChangeEvent::ExecuteL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileChangeEvent::ExecuteL()
    {
	PRODEBUG( " CProfileChangeEvent:ExecuteL" );
    CProfileChangeEvent* profileChangeEvent = CProfileChangeEvent::NewLC();
	PRODEBUG( " CProfileChangeEvent:ExecuteL 2" );

    profileChangeEvent->DoProfileExpirationL();

    CleanupStack::PopAndDestroy(); // profileChangeEvent
	PRODEBUG( " CProfileChangeEvent:ExecuteL end" );
    }

// -----------------------------------------------------------------------------
// CProfileChangeEvent::DoProfileExpirationL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileChangeEvent::DoProfileExpirationL()
    {
    iMutex.Wait();
    TRAPD( err,
        if( ReadProfileIdL() )
            {
            ShowNoteL();
            ChangeProfileL();
            }
         );
    iMutex.Signal();
    User::LeaveIfError( err );
    }

// -----------------------------------------------------------------------------
// CProfileChangeEvent::ChangeProfileL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileChangeEvent::ChangeProfileL()
    {
	PRODEBUG( " CProfileChangeEvent:ChangeProfileL" );

    iProfileEngine->SetActiveProfileL( iPreviousId );

    }

// -----------------------------------------------------------------------------
// CProfileChangeEvent::ReadProfileIdL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CProfileChangeEvent::ReadProfileIdL()
    {
	PRODEBUG( " CProfileChangeEvent:ReadProfileIdL" );
    CRepository* cenrep = CRepository::NewLC( KCRUidProfileEngine );
    User::LeaveIfError( cenrep->Get( KProEngPreviousActiveId, iPreviousId ) );
    CleanupStack::PopAndDestroy( cenrep );
    // If someone called SetActiveProfileL() right after scheduler started the
    // execution of this dll, the previous id would be resetted to KErrNotFound
    return ( iPreviousId > KErrNotFound );
    }

// -----------------------------------------------------------------------------
// CProfileChangeEvent::ShowNoteL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileChangeEvent::ShowNoteL()
    {
	PRODEBUG1( " CProfileChangeEvent:ShowNoteL(%S)", iNoteText );
	// Set secondary display data if cover ui is supported
	if ( iCoverDisplay )
		{
		CAknSDData* sd = CAknSDData::NewL(KCatProfiles,
							ECmdShowTimedProfileExpiryNote, KNullDesC8);
		iGlobalNote->SetSecondaryDisplayData( sd ); // ownership transferred
		}
    iGlobalNote->ShowNoteL( EAknGlobalConfirmationNote, *iNoteText );
    }

// -----------------------------------------------------------------------------
// CProfileChangeEvent::ReadResourcesL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileChangeEvent::ReadResourcesL()
    {
	PRODEBUG( " CProfileChangeEvent:ReadResourcesL" );
    TParse* fp = new ( ELeave ) TParse();
    fp->Set( KProfileScheduleEventResourceDriveAndDir,
             &KDC_RESOURCE_FILES_DIR, NULL );
    iResourceFile.Copy( fp->FullName() );
    delete fp;

	PRODEBUG( " CProfileChangeEvent:ReadResourcesL 2" );
    BaflUtils::NearestLanguageFile( iFs, iResourceFile );
	PRODEBUG1( " CProfileChangeEvent:ReadResourcesL 3 iResourceFile=%S", &iResourceFile );

    RResourceFile resourceFile;
    resourceFile.OpenL( iFs, iResourceFile );
    CleanupClosePushL( resourceFile );

	PRODEBUG( " CProfileChangeEvent:ReadResourcesL 4" );
    resourceFile.ConfirmSignatureL();
	PRODEBUG( " CProfileChangeEvent:ReadResourcesL 5" );

    HBufC8* dataBuffer = resourceFile.AllocReadLC( R_PROFILES_CONFIRM_TIMED_EXPIRED );

	PRODEBUG( " CProfileChangeEvent:ReadResourcesL 6" );
    TResourceReader rsReader;
    rsReader.SetBuffer( dataBuffer );
    iNoteText = rsReader.ReadHBufCL();
	PRODEBUG( " CProfileChangeEvent:ReadResourcesL 7" );

    CleanupStack::PopAndDestroy( 2 ); // dataBuffer, resourceFile
	PRODEBUG( " CProfileChangeEvent:ReadResourcesL 8" );
    }

//  End of File

