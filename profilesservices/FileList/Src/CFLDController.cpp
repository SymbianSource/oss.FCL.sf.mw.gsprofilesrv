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
*     CFLDController observes the list box and invokes a MFLDFileProcessor
*     if the timer expires.
*
*
*/


//  CLASS HEADER
#include "CFLDController.h"

//	INTERNAL INCLUDES
#include "MFLDFileObserver.h"
#include "CFLDRingingTonePlayer.h"

#ifdef RD_VIDEO_AS_RINGING_TONE
	#include "CFLDVideoPlayer.h"
	#include "CFLDSoftKeyChanger.h"
#endif

//	EXTERNAL INCLUDES
#include <bautils.h>
#include <data_caging_path_literals.hrh>
#include <CLFContentListing.hrh>
#include <pathinfo.h>	// For PathInfo

#include <caf/caf.h>
#include <Oma2Agent.h>


// CONSTANTS
namespace
	{
	_LIT( KFLDResourceFileName, "filelist.rsc" );
	}

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CFLDController::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CFLDController* CFLDController::NewL( TBool aShowErrorMsgs,
 TTimeIntervalMicroSeconds32 aDelay )
    {
    CFLDController* self = CFLDController::NewLC( aShowErrorMsgs, aDelay );
    CleanupStack::Pop( self ); // self

    return self;
    }

// -----------------------------------------------------------------------------
// CFLDController::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CFLDController* CFLDController::NewLC( TBool aShowErrorMsgs,
 TTimeIntervalMicroSeconds32 aDelay )
    {
    CFLDController* self = new( ELeave ) CFLDController( aShowErrorMsgs, aDelay );
    CleanupStack::PushL( self );

    self->ConstructL( );
    return self;
    }

// Destructor
EXPORT_C CFLDController::~CFLDController()
    {
    iResourceLoader.Close();
    
	Release();

	delete iTimer;
	delete iAudioProcessor;

#ifdef RD_VIDEO_AS_RINGING_TONE
	delete iSoftKeyChanger;
    delete iVideoProcessor;
#endif
    }

// -----------------------------------------------------------------------------
// CFLDController::Release
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDController::Release()
	{
	iTimer->Cancel();
	iAudioProcessor->Cancel();

#ifdef RD_VIDEO_AS_RINGING_TONE
	iVideoProcessor->Cancel();
#endif
	}

// -----------------------------------------------------------------------------
// CFLDController::CFLDFileListModel
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CFLDController::CFLDController( TBool aShowErrorMsgs,
 TTimeIntervalMicroSeconds32 aDelay )
    :  iDelay( aDelay ),
    iShowErrorMsgs( aShowErrorMsgs ),
    iResourceLoader( *( CCoeEnv::Static() ) )
    {
    }

// -----------------------------------------------------------------------------
// CFLDController::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CFLDController::ConstructL()
    {
    TFileName* fn = new (ELeave) TFileName
		( TParsePtrC( PathInfo::RomRootPath() ).Drive() );
	CleanupStack::PushL( fn );

	fn->Append( KDC_RESOURCE_FILES_DIR );
	fn->Append( KFLDResourceFileName );
	iResourceLoader.OpenL( *fn );
	CleanupStack::PopAndDestroy( fn );
		
    iTimer = CPeriodic::NewL( EPriorityNormal );
    iAudioProcessor = CFLDRingingTonePlayer::NewL( iShowErrorMsgs );
    
#ifdef RD_VIDEO_AS_RINGING_TONE
	iVideoProcessor = CFLDVideoPlayer::NewL( iShowErrorMsgs );
    iSoftKeyChanger = CFLDSoftKeyChanger::NewL();
#endif
    }

// -----------------------------------------------------------------------------
// CFLDController::CompleteConstructionL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CFLDController::CompleteConstructionL( RWindow& aWindow )
	{
	iWindow = &aWindow;
	}

// -----------------------------------------------------------------------------
// CFLDController::SetDelay()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CFLDController::SetDelay( TTimeIntervalMicroSeconds32 aDelay )
    {
    iDelay = aDelay;
    }


// -----------------------------------------------------------------------------
// CFLDController::SetVolume()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CFLDController::SetVolume( TInt aVolume )
    {
   	iRingingVolume = aVolume;
   	iAudioProcessor->SetVolume( iRingingVolume );

#ifdef RD_VIDEO_AS_RINGING_TONE
 	iVideoProcessor->SetVolume( iRingingVolume );
#endif
    }

// -----------------------------------------------------------------------------
// CFLDController::SetRingingType()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CFLDController::SetRingingType( TInt aRingingType )
    {
  	iRingingType = aRingingType;
 	iAudioProcessor->SetRingingType( iRingingType );

#ifdef RD_VIDEO_AS_RINGING_TONE
 	iVideoProcessor->SetRingingType( iRingingType );
#endif
    }

// -----------------------------------------------------------------------------
// CFLDController::SetVibra()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CFLDController::SetVibra( TBool aVibra )
    {
    iVibra = aVibra;
    iAudioProcessor->SetVibra( iVibra );

#ifdef RD_VIDEO_AS_RINGING_TONE
   	iVideoProcessor->SetVibra( iVibra );
#endif
    }

// -----------------------------------------------------------------------------
// CFLDController::SetFileObserver()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CFLDController::SetFileObserver( MFLDFileObserver* aFileObserver )
	{
	iFileObserver = aFileObserver;
	}

// -----------------------------------------------------------------------------
// CFLDController::Set3dEffects()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CFLDController::Set3dEffects( TBool a3dEffects )
	{
	i3dEffects = a3dEffects;
	iAudioProcessor->Set3dEffects( i3dEffects );
	}
	
// -----------------------------------------------------------------------------
// CFLDController::HandleSoftKeyState()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDController::HandleSoftKeyState( TFileListSoftKeyState& aSoftKeyState )
	{
#ifdef RD_VIDEO_AS_RINGING_TONE
	aSoftKeyState = iSoftKeyChanger->SoftKeyState();
#else
	aSoftKeyState = EToneSelectionSoftKeyState;	// Just remove compiler warnings
#endif
	}

// -----------------------------------------------------------------------------
// CFLDController::HandleFileListBoxEventL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CFLDController::HandleFileListBoxEventL( TFileListBoxEvent aEvent,
                                                   const TDesC& aFileName )
    {
    iCurrentFile = aFileName;

    switch( aEvent )
    	{
    	// User changes focus in tonelist
    	case EFocusChanged:
    		{
			Release();
#ifdef RD_VIDEO_AS_RINGING_TONE
			iSoftKeyChanger->RestoreOldToneSelectionSoftKeysL();
#endif

    		if( iDelay >= TTimeIntervalMicroSeconds32( 0 ) )
    			{
    			iTimer->Start( iDelay, iDelay, TCallBack( HandleTimerTickL, this ) ); // CSI: 10 # iTimer is cancelled in Release()
    			}
    		break;
    		}

		// Some other key was pressed than softkeys
    	case EOtherKeyEvent:
    		{
			Release();
#ifdef RD_VIDEO_AS_RINGING_TONE
			iSoftKeyChanger->RestoreOldToneSelectionSoftKeysL();
#endif
    		break;
    		}

		// User selects the video or audio as ringingtone in tonelist
		case EListBoxClosed:
			{
			Release();

    		if( iDelay >= TTimeIntervalMicroSeconds32( 0 ) )
    			{
    			iTimer->Start( iDelay, iDelay, TCallBack( HandleTimerTickL, this ) ); // CSI: 10 # iTimer is cancelled in Release()
    			}
    		break;
			}

#ifdef RD_VIDEO_AS_RINGING_TONE
		// Video has been focused for 1s and preview cba is selected
    	case EVideoPreview:
    		{
			Release();
			iSoftKeyChanger->ChangeNewSoftKeysPreviewL();
			if( iRingingType != ERingingTypeBeepOnce )
				{
				iVideoProcessor->ProcessFileL( iCurrentFile, iWindow );
				}
			else
				{
				// BeebOnce
				iAudioProcessor->ProcessFileL( iCurrentFile, NULL );
				iVideoProcessor->ProcessFileL( iCurrentFile, iWindow );
				}
			iSoftKeyChanger->ChangeNewSoftKeysPreviewSelectL();
			break;
    		}


		// Video is running in preview window and user has selected it
    	case EVideoPreviewSelected:
    		{
    		// Cleanup is done in destructor
    		break;
    		}

		// Video is running in preview window and user has canceled it
    	case EVideoPreviewCanceled:
    		{
            // Only cancel and restore softkeys if video is being previewed
            if( iSoftKeyChanger->SoftKeyState() == EPreviewSelectSoftKeyState )
                {
                Release();
                iSoftKeyChanger->RestoreOldToneSelectionSoftKeysL();
                }
			break;
    		}
#endif

    	default:
    		{
    		break;
    		}

    	};
    }

// -----------------------------------------------------------------------------
// CFLDController::HandleTimerTickL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CFLDController::HandleTimerTickL( TAny* aPtr )
    {
    CFLDController* controller = reinterpret_cast< CFLDController* >( aPtr );
    controller->iTimer->Cancel();

#ifdef RD_VIDEO_AS_RINGING_TONE
	TBool isVideo( EFalse );
	if( controller->iFileObserver )
		{
		if( controller->iFileObserver->MediaFileType(
		 controller->iCurrentFile ) == ECLFMediaTypeVideo )
			{
			isVideo = ETrue;
			}
		}

	if( isVideo )
		{
		// File is video
		// If it is OMA DRM 2 protected, it cannot be previewed in FileList (yet)
		using namespace ContentAccess;
	   	CContent* content = CContent::NewLC( controller->iCurrentFile,
	   										 EContentShareReadWrite );
	   	TInt deliveryMethod = 0;
	   	content->GetAttribute( EDeliveryMethod, deliveryMethod );
	   	CleanupStack::PopAndDestroy(); // content
	   	if( deliveryMethod != EOmaDrm2 )
	   		{
	   		// Change softkeys for preview
		 	controller->iSoftKeyChanger->ChangeNewSoftKeysPreviewL();
	   		}
		}
	 else
	 	{
	 	// File is audio
 		if( ( controller->iFileObserver &&
			!controller->iFileObserver->IsFileValidL(
        	controller->iCurrentFile, MFLDFileObserver::EPlay ) ) )
			{
			// If there is a file observer and the file is not valid,
			// don't call ProcessFileL.
			return 0;
			}
		controller->iAudioProcessor->ProcessFileL( controller->iCurrentFile, NULL );
	 	}
#else
	if( ( controller->iFileObserver &&
		!controller->iFileObserver->IsFileValidL(
        controller->iCurrentFile, MFLDFileObserver::EPlay ) ) )
		{
		// If there is a file observer and the file is not valid,
		// don't call ProcessFileL.
		return 0;
		}
	controller->iAudioProcessor->ProcessFileL( controller->iCurrentFile, NULL );
#endif

    return 0;
    }

//  End of File
