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
*	Creates a video player and starts video preview
*
*
*/


// CLASS HEADER
#include "CFLDVideoPlayer.h"

// EXTERNAL INCLUDES
#include <AknUtils.h> // For AknLayoutUtils
#include <AudioPreference.h> // For KAudioPriorityPreview
#include <ScreensaverInternalPSKeys.h>

// CONSTANTS
namespace
	{
	const TInt KAscendingVolumeRampInterval( 3000000 ); // 3 seconds
	const TInt KPhoneVideoVolumeRampStep = 1;
	// Used to reset inactivity timer so that backlight stays on
	const TInt KResetInactivityTimerDelay = 2000000;   // 2 seconds
	}

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CFLDVideoPlayer::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CFLDVideoPlayer* CFLDVideoPlayer::NewL( TBool aShowErrorMsgs )
    {
    CFLDVideoPlayer* self = new( ELeave ) CFLDVideoPlayer( aShowErrorMsgs );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self ); // self
    return self;
    }

// Destructor
CFLDVideoPlayer::~CFLDVideoPlayer()
    {
    Cancel();

    delete iBacklightTimer;

    if( iVolumeRampTimer )
	    {
    	delete iVolumeRampTimer;
	    }
	
    if( iVideoPlayer )
		{
		delete iVideoPlayer;
		}
    }

// -----------------------------------------------------------------------------
// CFLDVideoPlayer::CFLDVideoPlayer
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CFLDVideoPlayer::CFLDVideoPlayer( TBool aShowErrorMsgs )
    : CFLDPlayerBase( aShowErrorMsgs ),
      iVideoPlayerStatus( EVideoPlayerNotCreated )
    {
    }

// -----------------------------------------------------------------------------
// CFLDVideoPlayer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CFLDVideoPlayer::ConstructL()
    {
    BaseConstructL();

   	// To allow/not allow screensaver
   	// Errors ignored, no actions needed if API is not available
    iPropScreenSaver.Attach( KPSUidScreenSaver, KScreenSaverAllowScreenSaver );

    // To keep backlight on while a video is being previewed
    iBacklightTimer = CPeriodic::NewL( EPriorityLow );
    }

// -----------------------------------------------------------------------------
// CFLDVideoPlayer::ProcessFileL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDVideoPlayer::ProcessFileL( const TDesC& aFileName, RWindow* aWindow )
	{
	iWindow = aWindow;
	
	if ( aFileName == KNullDesC || iFocusLost || !iWindow )
        {
        return;
        }

    Cancel(); // Stops possible playback
    
   	// Do not allow screen saver while playing
    // Errors ignored, no actions needed if API is not available	   	
   	iPropScreenSaver.Set( KPSUidScreenSaver,
			KScreenSaverAllowScreenSaver, EFLScreenSaverNotAllowed );

    iBacklightTimer->Cancel(); // Just in case
    // Disable backlight turn off during video preview
    iBacklightTimer->Start( KResetInactivityTimerDelay,
                            KResetInactivityTimerDelay,
                            TCallBack( DoResetInactivityTimer, NULL ) );

   	// Screen and clip rectangles to window dimensions
    TPoint wndPosition( iWindow->AbsPosition() );
    TSize wndSize( iWindow->Size() );

	// iY and iHeight should be even numbers
	if( wndPosition.iY % 2 )
		{
		wndPosition.iY = wndPosition.iY + 1;
		wndSize.iHeight = wndSize.iHeight - 1;
		}
	if( wndSize.iHeight % 2 )
		{
		wndSize.iHeight = wndSize.iHeight - 1;
		}

    TRect wndRect( wndPosition, wndSize );

	if( iVideoPlayer )
		{
		delete iVideoPlayer;
		iVideoPlayer = NULL;
		}
                                            		
    iVideoPlayer =
		 CVideoPlayerUtility::NewL (*this, KAudioPriorityPreview,
	 	TMdaPriorityPreference( iVibra ? KAudioPrefRingFilePreviewVibra :
                                             KAudioPrefRingFilePreview ),
        CCoeEnv::Static()->WsSession(),
		*(CCoeEnv::Static()->ScreenDevice()),
		*iWindow,
		wndRect,
		wndRect);
		
	iVideoPlayerStatus = EVideoPlayerReady;     
	

	TDataType dataType;
	TInt error( DataType( aFileName, dataType ) );
    if( ( error != KErrNotFound ) && ( error != KErrNone ) )
        {
        User::Leave( error );
        }

	TPtrC ptr( aFileName );
	if( error )
        {
        ptr.Set( iDefaultTone );
        }

	iVideoPlayer->OpenFileL( ptr );
	iVideoPlayerStatus = EVideoPlayerInitializing;
	}

// -----------------------------------------------------------------------------
// CFLDVideoPlayer::Cancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDVideoPlayer::Cancel()
    {
    iBacklightTimer->Cancel();
    
    if( iVolumeRampTimer )
	    {
    	iVolumeRampTimer->Cancel();
	    }
	    
	if( iVideoPlayer )
		{
		iVideoPlayer->Stop();
		iVideoPlayer->Close();
		iVideoPlayerStatus = EVideoPlayerReady;
		delete iVideoPlayer;
		iVideoPlayer = NULL;
		}

   	// Allow screen saver, unless there's a call ongoing
   	if( !IsCallOngoing() )
   		{
	    // Errors ignored, no actions needed if API is not available	   	
	   	iPropScreenSaver.Set( KPSUidScreenSaver,
	    		KScreenSaverAllowScreenSaver, EFLScreenSaverAllowed );
   		}

	}

// -----------------------------------------------------------------------------
// CFLDVideoPlayer::DoResetInactivityTimer()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CFLDVideoPlayer::DoResetInactivityTimer( TAny* /*aObject*/ )
    {
    User::ResetInactivityTime();
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CFLDVideoPlayer::DoSetRingingType()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDVideoPlayer::DoSetRingingType( TInt aRingingType )
    {
    if( iVideoPlayerStatus == EVideoPlayerInitialized )
        {
        switch( aRingingType )
            {
			// Fall through
            case ERingingTypeRinging:
            case ERingingTypeSilent:
            case ERingingTypeRingOnce:
                {
                break;
                }
            case ERingingTypeAscending:
                {
				if ( !iVolumeRampTimer )
    				{
        			iVolumeRampTimer = CPeriodic::New( CActive::EPriorityStandard );
            		}

        		if ( iVolumeRampTimer && !iVolumeRampTimer->IsActive() )
        			{
            		TCallBack cb( VolumeRampTimerCallback, this );
        		    iRampedVolume = KFLDMinVolumeLevel;
            		iVolumeRampTimer->Start(
             			KAscendingVolumeRampInterval, KAscendingVolumeRampInterval, cb );
            		}
				break;
                }
             
            default:
                {
                break;
                }
            }
        }
    }

// -----------------------------------------------------------------------------
// CFLDVideoPlayer::DoVolumeRamp
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CFLDVideoPlayer::DoVolumeRamp()
    {
    if ( iRampedVolume < iRingingVolume )
        {
        iRampedVolume = iRampedVolume + KPhoneVideoVolumeRampStep;
        if ( iRampedVolume >= iRingingVolume )
            {
            // target volume level reached
            iRampedVolume = iRingingVolume;
            iVolumeRampTimer->Cancel();
            }
        }
        
    TRAP_IGNORE( iVideoPlayer->SetVolumeL( ConvertVolume( iRampedVolume ) ) );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CFLDVideoPlayer::VolumeRampTimerCallback
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CFLDVideoPlayer::VolumeRampTimerCallback( TAny* aObj )
    {
    return static_cast<CFLDVideoPlayer*>( aObj )->DoVolumeRamp();
    }

// -----------------------------------------------------------------------------
// CFLDVideoPlayer::ConvertVolume()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CFLDVideoPlayer::ConvertVolume( TInt aVolume )
    {
    TInt result( 0 );
    TBool audioEnabled( EFalse );

	TRAP_IGNORE( audioEnabled = iVideoPlayer->AudioEnabledL() );

	if( audioEnabled )
		{
	    if ( iVideoPlayerStatus == EVideoPlayerInitialized ||
    	    iVideoPlayerStatus == EVideoPlayerPlaying )
        	{
            result = BaseConvertVolume( aVolume, iVideoPlayer->MaxVolume() );

			//if user has selected silent ringing type
			// or beeb once, set volume off
			if( ( iRingingType == ERingingTypeSilent ) ||
			 ( iRingingType == ERingingTypeBeepOnce ) )
				{
				result = 0;
				}
        	}
		}

    return result;
    }

// -----------------------------------------------------------------------------
// CFLDVideoPlayer::MvpuoOpenComplete
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDVideoPlayer::MvpuoOpenComplete(TInt aError)
	{
	if ( !aError && iVideoPlayerStatus == EVideoPlayerInitializing )
		{
		iVideoPlayer->Prepare();
		}
	else
		{
		Cancel();
        if ( ( aError == KErrNotSupported ) || ( aError == KErrCorrupt ) )
            {
            // Don't care about leave, if the note can't be displayed.
            TRAP_IGNORE( DisplayErrorNoteL() );
            }
		}
	}
// -----------------------------------------------------------------------------
// CFLDVideoPlayer::MvpuoFrameReady
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDVideoPlayer::MvpuoFrameReady(CFbsBitmap& /*aFrame*/,TInt /*aError*/)
	{

	}

// -----------------------------------------------------------------------------
// CFLDVideoPlayer::MvpuoEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDVideoPlayer::MvpuoEvent(const TMMFEvent& /*aEvent*/)
	{

	}
// -----------------------------------------------------------------------------
// CFLDVideoPlayer::MvpuoPrepareComplete
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDVideoPlayer::MvpuoPrepareComplete(TInt aError)
	{
	if ( !aError && iVideoPlayerStatus == EVideoPlayerInitializing )
        {
        iVideoPlayerStatus = EVideoPlayerInitialized;
        DoSetRingingType( iRingingType );

		TInt startVolume( KFLDMinVolumeLevel );
		if( iRingingType != ERingingTypeAscending )
			{
			startVolume = ConvertVolume( iRingingVolume );
			}
		else
			{
			// Ascending starts from minimum volume level
			startVolume = ConvertVolume( KFLDMinVolumeLevel );
			}			
		
        TRAP_IGNORE( iVideoPlayer->SetVolumeL( startVolume ) );

		// Unfortunately SetPriorityL uses always priority/preference
		// settings which are given in videoPlayer constructor and ONLY
		// after that sets SetPriorityL parameter to its member data
		// which leads to a situation that we need to make SetPriorityL
		// call twice to make new settings effect.
        TRAP_IGNORE( iVideoPlayer->SetPriorityL( KAudioPriorityPreview,
        	TMdaPriorityPreference( iVibra ? KAudioPrefRingFilePreviewVibra :
                                             KAudioPrefRingFilePreview ) ) );
		TRAP_IGNORE( iVideoPlayer->SetPriorityL( KAudioPriorityPreview,
        	TMdaPriorityPreference( iVibra ? KAudioPrefRingFilePreviewVibra :
                                             KAudioPrefRingFilePreview ) ) );                                             
 		iVideoPlayer->Play();
		iVideoPlayerStatus = EVideoPlayerPlaying;
		}
	else
		{
		 Cancel();
        if ( ( aError == KErrNotSupported ) || ( aError == KErrCorrupt ) )
            {
            // Don't care about leave, if the note can't be displayed.
            TRAP_IGNORE( DisplayErrorNoteL() );
            }
		}
	}

// -----------------------------------------------------------------------------
// CFLDVideoPlayer::MvpuoPlayComplete
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDVideoPlayer::MvpuoPlayComplete(TInt aError)
	{
	if ( !aError && iVideoPlayerStatus == EVideoPlayerPlaying )
		{
		if( iRingingType != ERingingTypeRingOnce )
			{
			iVideoPlayer->Play();
			}
		else
			{
			Cancel();
			iVideoPlayerStatus = EVideoPlayerReady;
			}
		}
	else
		{
		Cancel();
        if ( ( aError == KErrNotSupported ) || ( aError == KErrCorrupt ) )
            {
            // Don't care about leave, if the note can't be displayed.
            TRAP_IGNORE( DisplayErrorNoteL() );
            }
		}
	}

// -----------------------------------------------------------------------------
// CFLDVideoPlayer::VideoResolution
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TSize CFLDVideoPlayer::VideoFrameSize() const
    {
    // Original implementation taken from phone application

    TSize frameSize( 0,0 );

    TRAPD( err, iVideoPlayer->VideoFrameSizeL( frameSize ) );

    if ( err != KErrNone )
        {
        return TSize(0,0);
        }

    return frameSize;
    }

// -----------------------------------------------------------------------------
// CFLDVideoPlayer::AdjustToWindow
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDVideoPlayer::AdjustToWindow( RWindow& aDisplayWindow )
    {
	// Original implementation taken from phone application

    // Get video frame dimensions
    TSize frameSize( VideoFrameSize() );

    if ( frameSize.iWidth == 0 || frameSize.iHeight == 0  )
        {
        return;
        }

    // Get display dimensions
    TPoint displayPosition( aDisplayWindow.AbsPosition() );
    TSize  displaySize( aDisplayWindow.Size() );
    TRect  displayRect( displayPosition, displaySize );


    // To cover display by video:
    // 1) Video is scaled to be >= display size
    // 2) If scaled size > display, then video is cropped
    // Assumption is that video can be scaled to 50,150 or 200
    // percent from its original size (can't be scaled freely).

    /////////////////////////////
    // Calculate scaling factor
    /////////////////////////////
    TInt dScaleFactor(100); // use integer arithmetic

    TInt xDelta( displaySize.iWidth - frameSize.iWidth );
    TInt yDelta( displaySize.iHeight - frameSize.iHeight );

    if ( xDelta == 0 && yDelta == 0 )
        {
        // correct size, scaling not needed
        }
    else if ( xDelta < 0 && yDelta == 0 )
        {
        // wide, but cannot downscale -> just crop
        }
    else if ( yDelta < 0 && xDelta == 0 )
        {
        // tall, but cannot downscale -> just crop
        }
    else if ( xDelta > 0 && yDelta > 0 )
        {
        // small, narrow and flat  -> enlarge
        TInt xProp( (100 * displaySize.iWidth) / frameSize.iWidth );
        TInt yProp( (100 * displaySize.iHeight) / frameSize.iHeight );

        dScaleFactor = xProp > yProp ? xProp : yProp;
        }
    else if ( xDelta < 0 && yDelta < 0 )
        {
        // large, wide and tall -> downscale
        TInt xProp( ( 100 * displaySize.iWidth) / frameSize.iWidth );
        TInt yProp( ( 100 * displaySize.iHeight) / frameSize.iHeight );

        dScaleFactor = xProp > yProp ? xProp : yProp;
        }
    else if ( xDelta > 0 && yDelta <= 0 )
        {
        // narrow -> enlarge
        dScaleFactor = (100 * displaySize.iWidth) / frameSize.iWidth;
        }
    else if ( yDelta > 0 && xDelta <= 0 )
        {
        // flat  -> enlarge
        dScaleFactor = (100 * displaySize.iHeight) / frameSize.iHeight;
        }
    else
        {
        // do nothing
        }

    // Convert to float: 0.5, 1.5, 2.0 ..
    TInt scaleFactor( dScaleFactor / 100 );
    TInt remainder( dScaleFactor % 100 );
    TReal32 fScaleFactor = (TReal) scaleFactor ;

    if ( scaleFactor > 0 ) // upscale
        {
        if ( remainder > 50 )
            {
            fScaleFactor = fScaleFactor + 1.0;
            }
        else if ( remainder > 0 )
            {
            fScaleFactor = fScaleFactor + 0.5;
            }
        else // 0
            {
            }
        }
    else // downscale
        {
        if ( remainder > 50 )
            {
            fScaleFactor = 1.0;
            }
        else
            {
            fScaleFactor = 0.5;
            }
        }

    ////////////////////////////////////////////////
    // Calculate scaled frame size (virtual canvas)
    ////////////////////////////////////////////////
    TReal32 canvasWidth = fScaleFactor * (TReal32)frameSize.iWidth;
    TReal32 canvasHeight = fScaleFactor * (TReal32)frameSize.iHeight;
    TSize canvasSize( (TInt)canvasWidth, (TInt)canvasHeight );

    ////////////////////////////////////////////////
    // Crop by centering displayRect to canvasRect
    ////////////////////////////////////////////////
    TRect canvasRect( displayPosition, canvasSize );
    TInt offsetX = (displaySize.iWidth - canvasSize.iWidth) / 2;
    TInt offsetY = (displaySize.iHeight - canvasSize.iHeight) / 2;
    canvasRect.Move( offsetX, offsetY );

    ////////////////////////////////////////////////
    // Update settings to player
    ////////////////////////////////////////////////
    TRAP_IGNORE( iVideoPlayer->SetDisplayWindowL(
           CCoeEnv::Static()->WsSession(),
           *CCoeEnv::Static()->ScreenDevice(),
           aDisplayWindow,
           canvasRect,
           displayRect ) );

    }

//  End of File
