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
* Description:   Class for preview audio and video
*
*/




// INCLUDE FILES

#include "mediafilepreview.h"
#include "mediafiledialogutils.h"
#include "mediafilelist.h"
#include "mediafilelistdebug.h"

#include <ScreensaverInternalPSKeys.h>   // KPSUidScreenSaver
#include <AudioPreference.h>             // KAudioPriorityPreview
#include <c3dringingtoneinterface.h>     // C3DRingingToneInterface

#include <ctsydomainpskeys.h>            // for phone call states

// for reading active profile settings
#include <MProfileEngine.h>
#include <MProfile.h>
#include <MProfileTones.h>
#include "TProfileToneSettings.h"
#include <MProfileExtraSettings.h>
#include <MProfile3DToneSettings.h>
#include <ProfileInternal.hrh>
#include <ProfileEngineDomainCRKeys.h>   // KProEngDefaultRingingTone


/******************************************************************************
 * class CMFPreviewHandlerBase
 ******************************************************************************/


// -----------------------------------------------------------------------------
// CMFPreviewHandlerBase::CMFPreviewHandlerBase
// 
// -----------------------------------------------------------------------------
//
CMFPreviewHandlerBase::CMFPreviewHandlerBase()
	{
    iMediaType = KErrNotFound;
    iRingingVolume = KErrNotFound;
    iRingingType = KErrNotFound;
    iVibra = KErrNotFound;
    i3DEffect = KErrNotFound;
    i3DEcho = KErrNotFound;
    iFileSize = KErrNotFound;
	
	iFullName = NULL;
	
	iActiveProfileRead = EFalse;
	iPlayerStatus = EPlayerNotCreated;
	
    iObserver = NULL;
    }


// -----------------------------------------------------------------------------
// CMFPreviewHandlerBase::CMFPreviewHandlerBase
//
// -----------------------------------------------------------------------------
//
void CMFPreviewHandlerBase::ConstructL()
    {
   	// To allow/not allow screensaver
   	// Errors ignored, no actions needed if API is not available
    iProperty.Attach( KPSUidScreenSaver, KScreenSaverAllowScreenSaver );
    
    User::LeaveIfError( iApaSession.Connect() );
    
    TRAP_IGNORE( ReadDefaultToneL() );
    // To keep backlight on while a video is being previewed
    iBacklightTimer = CPeriodic::NewL( EPriorityLow );
    }


// ----------------------------------------------------------------------------
// Destructor
//
// ----------------------------------------------------------------------------
//
CMFPreviewHandlerBase::~CMFPreviewHandlerBase()
    {
    delete iFullName;
    iProperty.Close();
    
    iApaSession.Close();
    }


// -----------------------------------------------------------------------------
// CMFPreviewHandlerBase::SetAttr
// 
// -----------------------------------------------------------------------------
void CMFPreviewHandlerBase::SetAttrL( TInt aAttr, TInt aValue )
	{
	switch ( aAttr )
	    {
	    case CMediaFileList::EAttrVolume:
	        {
	        iRingingVolume = aValue;
	        break;
	        }
	    case CMediaFileList::EAttrRingingType:
	        {
	        iRingingType = aValue;
	        break;
	        }
	    case CMediaFileList::EAttrVibra:
	        {
	        iVibra = aValue;
	        break;
	        }
	    case CMediaFileList::EAttr3DEffect:
	        {
	        i3DEffect = aValue;
	        break;
	        }
	    case CMediaFileList::EAttr3DEcho:
	        {
	        i3DEcho = aValue;
	        break;
	        }
	    case CMediaFileList::EAttrFileSize:
	        {
	        iFileSize = aValue;
	        break;
	        }
	    default:
	        {
	        break;
	        }
	    }
	}


// -----------------------------------------------------------------------------
// CMFPreviewHandlerBase::Attr
// 
// -----------------------------------------------------------------------------
TInt CMFPreviewHandlerBase::Attr( TInt aAttr )
    {
    TInt ret = KErrNotFound;
    
    switch ( aAttr )
        {
        case CMediaFileList::EAttrVolume:
            {
            ret = iRingingVolume;
            break;
            }
        case CMediaFileList::EAttrRingingType:
            {
            ret = iRingingType;
            break;
            }
        case CMediaFileList::EAttrVibra:
            {
            ret = iVibra;
            break;
            }
        case CMediaFileList::EAttr3DEffect:
            {
            ret = i3DEffect;
            break;
            }
        case CMediaFileList::EAttr3DEcho:
            {
            ret = i3DEcho;
            break;
            }
        case CMediaFileList::EAttrFileSize:
            {
            ret = iFileSize;
            break;
            }
        default:
            {
            break;
            }
        }
    
    return ret;
    }


// -----------------------------------------------------------------------------
// CMFPreviewHandlerBase::SetAttrL
// 
// -----------------------------------------------------------------------------
void CMFPreviewHandlerBase::SetAttrL( TInt aAttr, const TDesC& aValue )
	{
    if ( aAttr == TMFDialogUtil::EAttrFileName )
	    {
	    delete iFullName;
	    iFullName = NULL;
	    iFullName = aValue.AllocL();
	    }
	}


// -----------------------------------------------------------------------------
// CMFPreviewHandlerBase::SetAttr
// 
// -----------------------------------------------------------------------------
//
void CMFPreviewHandlerBase::SetAttrL( TInt aAttr, TAny* aValue )
    {
	if ( aAttr == TMFDialogUtil::EAttrDrawingWindow )
	    {
	    iWindow = static_cast<RWindow*>( aValue );
	    }
    }


// -----------------------------------------------------------------------------
// CMFPreviewHandlerBase::SetObserver
// 
// -----------------------------------------------------------------------------
//
void CMFPreviewHandlerBase::SetObserver( MPreviewHandlerObserver* aObserver )
    {
    iObserver = aObserver;
    }
    

// -----------------------------------------------------------------------------
// CMFPreviewHandlerBase::IsCallOngoing
//
// -----------------------------------------------------------------------------
//
TBool CMFPreviewHandlerBase::IsCallOngoing()
    {
    TInt err = KErrNone;
    TInt callState = EPSCTsyCallStateUninitialized; 

    err = RProperty::Get( KPSUidCtsyCallInformation, KCTsyCallState, callState );
    if ( err == KErrNone )
        {
        if( callState != EPSCTsyCallStateUninitialized && 
            callState != EPSCTsyCallStateNone && 
            callState != EPSCTsyCallStateDisconnecting ) 
            {
            // There was call ongoing
            return ETrue;
            }
        }

    return EFalse;
    }


// -----------------------------------------------------------------------------
// CMFPreviewHandlerBase::RingingVolume
//
// -----------------------------------------------------------------------------
//
TInt CMFPreviewHandlerBase::RingingVolume()
    {
    const TInt KDefaultVolumeLevel = 7; // see profile.hrh for volume levels
    
    if ( iRingingVolume != KErrNotFound )
        {
        return iRingingVolume;
        }
        
    if ( iActiveProfileRead )
        {
        return iActiveProfileRingingVolume;
        }

    return KDefaultVolumeLevel;
    }


// -----------------------------------------------------------------------------
// CMFPreviewHandlerBase::RingingType
//
// -----------------------------------------------------------------------------
//
TInt CMFPreviewHandlerBase::RingingType()
    {
    if ( iRingingType != KErrNotFound )
        {
        return iRingingType;
        }
        
    if ( iActiveProfileRead )
        {
        return iActiveProfileRingingType;
        }

    return ERingingTypeRinging;
    }


// -----------------------------------------------------------------------------
// CMFPreviewHandlerBase::Vibra
//
// -----------------------------------------------------------------------------
//
TInt CMFPreviewHandlerBase::Vibra()
    {
    if ( iVibra != KErrNotFound )
        {
        return iVibra;
        }
        
    if ( iActiveProfileRead )
        {
        return iActiveProfileVibra;
        }
    
    return 0;  // in case of error vibra is off
    }


// -----------------------------------------------------------------------------
// CMFPreviewHandlerBase::Echo3D
//
// -----------------------------------------------------------------------------
//
TInt CMFPreviewHandlerBase::Echo3D()
    {
    if ( i3DEcho != KErrNotFound )
        {
        return i3DEcho;
        }
    
    if ( iActiveProfileRead )
        {
        return iActiveProfile3DEcho;
        }

    return EProfile3DEchoOff;  // from ProfileInternal.hrh
    }


// -----------------------------------------------------------------------------
// CMFPreviewHandlerBase::Effect3D
//
// -----------------------------------------------------------------------------
//
TInt CMFPreviewHandlerBase::Effect3D()
    {
    if ( i3DEffect != KErrNotFound )
        {
        return i3DEffect;
        }
    
    if ( iActiveProfileRead )
        {
        return iActiveProfile3DEffect;
        }

    return EProfile3DEffectOff;
    }


// -----------------------------------------------------------------------------
// CMFPreviewHandlerBase::ConvertVolume
//
// -----------------------------------------------------------------------------
//
TInt CMFPreviewHandlerBase::ConvertVolume( TInt aVolume, TInt aMaxVolume )
    {
  	const TInt KMinVolumeLevel = 1;
    const TInt KMaxVolumeLevel = 10;
    	
    TInt result = aMaxVolume * aVolume / KMaxVolumeLevel;
    
    // if user has selected minimum volume level set HW volume 1
    if ( aVolume == KMinVolumeLevel && result == 0 )
        {
        result = 1;
        }

    return result;
    }


// -----------------------------------------------------------------------------
// CMFPreviewHandlerBase::ReadActiveProfileL
// -----------------------------------------------------------------------------
//    
void CMFPreviewHandlerBase::ReadActiveProfileL()
    {
    iActiveProfileRead = EFalse;
    
    MProfileEngine* profileEngine = CreateProfileEngineL();
    CleanupReleasePushL( *profileEngine );
    
    MProfile* activeProfile  = profileEngine->ActiveProfileL();
    CleanupReleasePushL( *activeProfile );
        
    const MProfileTones& profileTones = activeProfile->ProfileTones();
     
    const TProfileToneSettings& toneSettings = profileTones.ToneSettings();
    iActiveProfileVibra = toneSettings.iVibratingAlert;
    iActiveProfileRingingVolume = toneSettings.iRingingVolume;
    iActiveProfileRingingType = toneSettings.iRingingType;
          
    const MProfileExtraSettings& extra = activeProfile->ProfileExtraSettings();
    const MProfile3DToneSettings& threeD = extra.Profile3DToneSettings();
    
    iActiveProfile3DEffect = threeD.Effect();
    iActiveProfile3DEcho = threeD.Echo();
   
    CleanupStack::PopAndDestroy( activeProfile ); 
    CleanupStack::PopAndDestroy( profileEngine );
    
    iActiveProfileRead = ETrue;
    }


// -----------------------------------------------------------------------------
// CMFPreviewHandlerBase::ReadDefaultToneL
// -----------------------------------------------------------------------------
//    
void CMFPreviewHandlerBase::ReadDefaultToneL()
    {
    CRepository* cenrep = CRepository::NewLC( KCRUidProfileEngine );

    User::LeaveIfError( cenrep->Get( KProEngDefaultRingingTone, iDefaultTone ) );
    CleanupStack::PopAndDestroy( cenrep );
    }


// -----------------------------------------------------------------------------
// CMFPreviewHandlerBase::EnableScreenSaver
//
// -----------------------------------------------------------------------------
//
void CMFPreviewHandlerBase::EnableScreenSaver( TBool aEnable )
    {
    if ( aEnable )
        {
   	    // allow screen saver, unless there's a call ongoing
   	    if( !IsCallOngoing() )
   		    {
	        // ignore error, no actions needed if API is not available	   	
	   	    iProperty.Set( KPSUidScreenSaver, KScreenSaverAllowScreenSaver,
	   	                   EFLScreenSaverAllowed );
   		    }
        }
    else
        {
   	    iProperty.Set( KPSUidScreenSaver,
			       KScreenSaverAllowScreenSaver, EFLScreenSaverNotAllowed );
        }
    }


// -----------------------------------------------------------------------------
// CMFPreviewHandlerBase::GetDataType
//
// -----------------------------------------------------------------------------
//
TInt CMFPreviewHandlerBase::GetDataType( const TDesC& aFileName, TDataType& aDataType )
    {
   	TUid dummyUid( KNullUid );
	return iApaSession.AppForDocument( aFileName, dummyUid, aDataType );
    }


// -----------------------------------------------------------------------------
// CMFPreviewHandlerBase::Panic
//
// -----------------------------------------------------------------------------
//
void CMFPreviewHandlerBase::Panic( TInt aReason ) 
    {
	_LIT( KPanicCategory, "CMFPreviewHandlerBase" );
	
	User::Panic( KPanicCategory, aReason ); 
    }


// -----------------------------------------------------------------------------
// CMFPreviewHandlerBase::DoResetInactivityTimer()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CMFPreviewHandlerBase::DoResetInactivityTimer( TAny* /*aObject*/ )
    {
    User::ResetInactivityTime();
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMFPreviewHandlerBase::DisableBackLight()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CMFPreviewHandlerBase::DisableBackLight()
	{
		const TInt KResetInactivityTimerDelay = 2000000;
    iBacklightTimer->Cancel(); // Just in case
    // Disable backlight turn off during video preview
    iBacklightTimer->Start( KResetInactivityTimerDelay,
                            KResetInactivityTimerDelay,
                            TCallBack( DoResetInactivityTimer, NULL ) );
	
	}


/******************************************************************************
 * class CMFVideoPreviewHandler
 ******************************************************************************/



// -----------------------------------------------------------------------------
// CMFVideoPreviewHandler::NewL
//
// -----------------------------------------------------------------------------
CMFVideoPreviewHandler* CMFVideoPreviewHandler::NewL()
    {
    CMFVideoPreviewHandler* self = new (ELeave) CMFVideoPreviewHandler();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }


// -----------------------------------------------------------------------------
// CMFVideoPreviewHandler::CMFVideoPreviewHandler
// 
// -----------------------------------------------------------------------------
//
CMFVideoPreviewHandler::CMFVideoPreviewHandler()
	{
	iPlayerStatus = EPlayerNotCreated;
    }


// -----------------------------------------------------------------------------
// CMFVideoPreviewHandler::ConstructL
//
// -----------------------------------------------------------------------------
//
void CMFVideoPreviewHandler::ConstructL()
    {
    CMFPreviewHandlerBase::ConstructL();
    }


// ----------------------------------------------------------------------------
// Destructor
//
// ----------------------------------------------------------------------------
//
CMFVideoPreviewHandler::~CMFVideoPreviewHandler()
    {
    Cancel();
    
    if( iVolumeRampTimer )
	    {
    	delete iVolumeRampTimer;
	    }
	    
	if( iVideoPlayer )
		{
		delete iVideoPlayer;
    	}
	
	if ( iBacklightTimer )
		{
		delete iBacklightTimer;
		iBacklightTimer = NULL;
		}
    }


// -----------------------------------------------------------------------------
// CMFVideoPreviewHandler::IsPlaying
// 
// -----------------------------------------------------------------------------
TBool CMFVideoPreviewHandler::IsPlaying()
    {
    if ( iPlayerStatus == EPlayerNotCreated )
        {
        return EFalse;
        }
    
    return ETrue;
    }
    

// -----------------------------------------------------------------------------
// CMFVideoPreviewHandler::SetDisplayWindowL
// 
// -----------------------------------------------------------------------------
void CMFVideoPreviewHandler::SetDisplayWindowL()
    {
    if ( !iVideoPlayer )
        {
        return;
        }
    
    if ( !iWindow )
        {
        return;  // no screen for showing video
        }

    // Screen and clip rectangles to window dimensions
    TPoint wndPosition( iWindow->AbsPosition() );
    TSize wndSize( iWindow->Size() );

    TRect wndRect( wndPosition, wndSize );

    iVideoPlayer->SetDisplayWindowL( CCoeEnv::Static()->WsSession(),
                                     *(CCoeEnv::Static()->ScreenDevice()),
                                     *iWindow,
                                     wndRect,
                                     wndRect );
    }

// -----------------------------------------------------------------------------
// CMFVideoPreviewHandler::PlayL
// 
// -----------------------------------------------------------------------------
void CMFVideoPreviewHandler::PlayL()
	{
    Cancel(); // stop previous playing
    
    if ( !iFullName || iFullName->Des().Length() == 0 )
        {
        User::Leave( KErrNotFound );
        }

	if ( !iWindow )
	    {
	    return;  // no screen for showing video
        }
    
	TRAP_IGNORE( ReadActiveProfileL() );
	
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
 

	// check video file
	TDataType dataType;
	TInt err = GetDataType( iFullName->Des(), dataType );
    if ( err != KErrNone )
        {
        User::Leave( err );
        }

    TInt vibra = Vibra();
    TMdaPriorityPreference pref = (TMdaPriorityPreference) KAudioPrefRingFilePreview;       		
    if ( vibra )
        {
        pref = (TMdaPriorityPreference) KAudioPrefRingFilePreviewVibra;
        }
        
	delete iVideoPlayer;
	iVideoPlayer = NULL;

    iVideoPlayer = CVideoPlayerUtility::NewL ( *this, KAudioPriorityPreview, pref,
                                               CCoeEnv::Static()->WsSession(),
		                                       *(CCoeEnv::Static()->ScreenDevice()),
		                                       *iWindow,
		                                       wndRect,
		                                       wndRect );
		
    iVideoPlayer->OpenFileL( iFullName->Des() );

	iPlayerStatus = EPlayerInitializing;
	   	
   	EnableScreenSaver( EFalse ); // do not allow screen saver while playing
   	DisableBackLight();

#ifdef _DEBUG
    User::InfoPrint( iFullName->Des() );
    FLOG( _L("CMFVideoPreviewHandler::PlayL  '%S'"), &iFullName->Des() );
#endif

	}


// -----------------------------------------------------------------------------
// CMFVideoPreviewHandler::Stop
// 
// -----------------------------------------------------------------------------
void CMFVideoPreviewHandler::Stop()
	{
	Cancel();
	}

    
// -----------------------------------------------------------------------------
// CMFVideoPreviewHandler::Cancel
//
// -----------------------------------------------------------------------------
//
void CMFVideoPreviewHandler::Cancel()
    {
   	TBool isPlaying = EFalse;
   	
    if( iVolumeRampTimer )
	    {
    	iVolumeRampTimer->Cancel();
	    }
	    
	if( iVideoPlayer )
		{
		isPlaying = ETrue;
		
		if ( iPlayerStatus == EPlayerPlaying )
		    {
		    iVideoPlayer->Stop();    
		    }
		iVideoPlayer->Close();

		delete iVideoPlayer;
		iVideoPlayer = NULL;
    	}

   	if ( isPlaying )
   	    {
   	    User::InfoPrint( _L("cancel video") );

   	    EnableScreenSaver( ETrue );
   	    iBacklightTimer->Cancel(); 
   	    }
   	    
   	iPlayerStatus = EPlayerNotCreated;
	}


// -----------------------------------------------------------------------------
// CMFVideoPreviewHandler::SetRingingType
//
// -----------------------------------------------------------------------------
//
void CMFVideoPreviewHandler::SetRingingType( TInt aRingingType )
    {
    const TInt KMinVolumeLevel = 1;
   	const TInt KAscendingVolumeRampInterval = 3000000; // 3 seconds

    if ( aRingingType == ERingingTypeAscending )
        {
        if ( !iVolumeRampTimer )
            {
            iVolumeRampTimer = CPeriodic::New( CActive::EPriorityStandard );
            }

        if ( iVolumeRampTimer && !iVolumeRampTimer->IsActive() )
            {
            TCallBack cb( VolumeRampTimerCallback, this );
            iRampedVolume = KMinVolumeLevel;
            iVolumeRampTimer->Start( KAscendingVolumeRampInterval, 
                                     KAscendingVolumeRampInterval, cb );
            }
        }
    }


// -----------------------------------------------------------------------------
// CMFVideoPreviewHandler::ConvertVolume
//
// -----------------------------------------------------------------------------
//
TInt CMFVideoPreviewHandler::ConvertVolume( TInt aVolume )
    {
    if ( !iVideoPlayer )
        {
        return 0;
        }
    
    TInt result = 0;
    TBool audioEnabled = EFalse;

	TRAP_IGNORE( audioEnabled = iVideoPlayer->AudioEnabledL() );

	if ( audioEnabled )
		{
        result = CMFPreviewHandlerBase::ConvertVolume( aVolume, iVideoPlayer->MaxVolume() );

        // if user has selected silent ringing type or beeb once, set volume off
     	TInt ringingType = RingingType();
		if ( ringingType == ERingingTypeSilent || ringingType == ERingingTypeBeepOnce )
			{
			result = 0;
			}
		}

    return result;
    }



// -----------------------------------------------------------------------------
// CMFVideoPreviewHandler::VolumeRampTimerCallback
//
// -----------------------------------------------------------------------------
//
TInt CMFVideoPreviewHandler::VolumeRampTimerCallback( TAny* aObj )
    {
    return static_cast<CMFVideoPreviewHandler*>( aObj )->VolumeRamp();
    }


// -----------------------------------------------------------------------------
// CMFVideoPreviewHandler::VolumeRamp
//
// -----------------------------------------------------------------------------
//
TInt CMFVideoPreviewHandler::VolumeRamp()
    {
	const TInt KPhoneVideoVolumeRampStep = 1;
	    
	TInt ringingVolume = RingingVolume();
    if ( iRampedVolume < ringingVolume )
        {
        iRampedVolume = iRampedVolume + KPhoneVideoVolumeRampStep;
        if ( iRampedVolume >= ringingVolume )
            {
            // target volume level reached
            iRampedVolume = ringingVolume;
            iVolumeRampTimer->Cancel();
            }
        }
        
    TRAP_IGNORE( iVideoPlayer->SetVolumeL( ConvertVolume( iRampedVolume ) ) );

    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CMFVideoPreviewHandler::MvpuoOpenComplete (from MVidePlayerUtilityObserver)
//
// -----------------------------------------------------------------------------
//
void CMFVideoPreviewHandler::MvpuoOpenComplete( TInt aError )
	{
    __ASSERT_DEBUG( iPlayerStatus == EPlayerInitializing, Panic( KErrGeneral ) );
    
    if ( aError )
        {
        Cancel();
        
        if ( iObserver )
            {
            TInt event = MPreviewHandlerObserver::EPreviewError;
            TRAP_IGNORE( iObserver->HandlePreviewEventL( event, aError ) );
            }
        }
	else
	    {
	    iVideoPlayer->Prepare();    
	    }
    }
	
	
// -----------------------------------------------------------------------------
// CMFVideoPreviewHandler::MvpuoPrepareComplete (from MVidePlayerUtilityObserver)
//
// -----------------------------------------------------------------------------
//
void CMFVideoPreviewHandler::MvpuoPrepareComplete( TInt aError )
	{
	const TInt KMinVolumeLevel = 1;
	
    __ASSERT_DEBUG( iPlayerStatus == EPlayerInitializing, Panic( KErrGeneral ) );
    
    if ( aError != KErrNone )
        {
        Cancel();

        if ( iObserver )
            {
            TInt event = MPreviewHandlerObserver::EPreviewError;
            TRAP_IGNORE( iObserver->HandlePreviewEventL( event, aError ) );
            }
        return;
        }
    	
    iPlayerStatus = EPlayerInitialized;
    
    TInt ringingType = RingingType();
    TInt ringingVolume = RingingVolume();
    TInt vibra = Vibra();

    
    SetRingingType( ringingType );

    TInt startVolume = KMinVolumeLevel;
	if ( ringingType != ERingingTypeAscending )
		{
		startVolume = ConvertVolume( ringingVolume );
		}
	else
		{
		// Ascending starts from minimum volume level
		startVolume = ConvertVolume( KMinVolumeLevel );
		}			
    TRAP_IGNORE( iVideoPlayer->SetVolumeL( startVolume ) );
    
    TMdaPriorityPreference pref = (TMdaPriorityPreference) KAudioPrefRingFilePreview;       		
    if ( vibra )
        {
        pref = (TMdaPriorityPreference) KAudioPrefRingFilePreviewVibra;
        }

	// Unfortunately SetPriorityL uses always priority/preference
	// settings which are given in videoPlayer constructor and ONLY
	// after that sets SetPriorityL parameter to its member data
	// which leads to a situation that we need to make SetPriorityL
	// call twice to make new settings effect.
    TRAP_IGNORE( iVideoPlayer->SetPriorityL( KAudioPriorityPreview, pref ) );
    TRAP_IGNORE( iVideoPlayer->SetPriorityL( KAudioPriorityPreview, pref ) );
                                             
 	iVideoPlayer->Play();
	iPlayerStatus = EPlayerPlaying;
	}


// -----------------------------------------------------------------------------
// CMFVideoPreviewHandler::MvpuoPlayComplete (from MVidePlayerUtilityObserver)
//
// -----------------------------------------------------------------------------
//
void CMFVideoPreviewHandler::MvpuoPlayComplete( TInt aError )
	{
    __ASSERT_DEBUG( iPlayerStatus == EPlayerPlaying, Panic( KErrGeneral ) );
    
    if ( aError != KErrNone )
        {
        Cancel();
        
        if ( iObserver )
            {
            TInt event = MPreviewHandlerObserver::EVideoPreviewComplete;
            TRAP_IGNORE( iObserver->HandlePreviewEventL( event, aError ) );
            }
        return;
        }
	
	TInt ringingType = RingingType();
    if( ringingType != ERingingTypeRingOnce )
		{
		iVideoPlayer->Play();
		}
	else
		{
		Cancel();
        if ( iObserver )
            {
            TInt event = MPreviewHandlerObserver::EVideoPreviewComplete;
            TRAP_IGNORE( iObserver->HandlePreviewEventL( event, aError ) );
            }
		}
	}


// -----------------------------------------------------------------------------
// CMFVideoPreviewHandler::MvpuoFrameReady (from MVidePlayerUtilityObserver)
//
// -----------------------------------------------------------------------------
//
void CMFVideoPreviewHandler::MvpuoFrameReady( CFbsBitmap& /*aFrame*/,TInt /*aError*/ )
    {
    }


// -----------------------------------------------------------------------------
// CMFVideoPreviewHandler::MvpuoEvent (from MVidePlayerUtilityObserver)
//
// -----------------------------------------------------------------------------
//
void CMFVideoPreviewHandler::MvpuoEvent( const TMMFEvent& /*aEvent*/ )
    {
    }


// -----------------------------------------------------------------------------
// CMFVideoPreviewHandler::GetRectData
// 
// -----------------------------------------------------------------------------
void CMFVideoPreviewHandler::GetRectData( TRect aRect, TDes& aText )
    {
    _LIT( KFormat, "top left (%d,%d)  bottom right (%d,%d)" );
    
    TPoint p1 = aRect.iTl;
    TPoint p2 = aRect.iBr;
    
    aText.Format( KFormat, p1.iX, p1.iY, p2.iX, p2.iY );
    }




/******************************************************************************
 * class CMFAudioPreviewHandler
 ******************************************************************************/



// -----------------------------------------------------------------------------
// CMFAudioPreviewHandler::NewL
//
// -----------------------------------------------------------------------------
CMFAudioPreviewHandler* CMFAudioPreviewHandler::NewL()
    {
    CMFAudioPreviewHandler* self = new (ELeave) CMFAudioPreviewHandler();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }


// -----------------------------------------------------------------------------
// CMFAudioPreviewHandler::CMFAudioPreviewHandler
// 
// -----------------------------------------------------------------------------
//
CMFAudioPreviewHandler::CMFAudioPreviewHandler()
	{
	iAudioPlayerStatus = EPlayerNotCreated;
	iTonePlayerStatus = EPlayerNotCreated;
    }


// -----------------------------------------------------------------------------
// CMFAudioPreviewHandler::CMFAudioPreviewHandler
//
// -----------------------------------------------------------------------------
//
void CMFAudioPreviewHandler::ConstructL()
    {
    CMFPreviewHandlerBase::ConstructL();
    }


// ----------------------------------------------------------------------------
// Destructor
//
// ----------------------------------------------------------------------------
//
CMFAudioPreviewHandler::~CMFAudioPreviewHandler()
    {
    Cancel();
    
    delete iAudioPlayer;
    delete iTonePlayer;
    delete i3dRingingTonePlugin;
    }


// -----------------------------------------------------------------------------
// CMFAudioPreviewHandler::IsPlaying
// 
// -----------------------------------------------------------------------------
TBool CMFAudioPreviewHandler::IsPlaying()
    {
    if ( iAudioPlayerStatus != EPlayerNotCreated )
        {
        return ETrue;
        }
    
    if ( iTonePlayerStatus != EPlayerNotCreated )
        {
        return ETrue;
        }

    return EFalse;
    }


// -----------------------------------------------------------------------------
// CMFAudioPreviewHandler::PlayL
// 
// -----------------------------------------------------------------------------
void CMFAudioPreviewHandler::PlayL()
	{
	//sequence for playing a beep once sound
	_LIT8( KFileListBeepSequence, "\x00\x11\x06\x0A\x08\x73\x0A\x40\x28\x0A\xF7\
	\x05\xFC\x40\x64\x0A\x08\x40\x32\x0A\xF7\x06\x0B" ); 

	// rng mime type
	_LIT( KFileListRngMimeType, "application/vnd.nokia.ringing-tone" );
	
	Cancel(); // stop previous play
	
    if ( !iFullName || iFullName->Des().Length() == 0 )
        {
        User::Leave( KErrNotFound );
        }

	TRAP_IGNORE( ReadActiveProfileL() );

	TPtrC fileName( iFullName->Des() );
	TDataType dataType;
	TInt err = GetDataType( fileName, dataType );
    if ( err == KErrNotFound )
        {
        fileName.Set( iDefaultTone );
        if ( fileName.Length() == 0 )
             {
             User::Leave( KErrNotFound );
             }
        }
    else if ( err != KErrNone )
        {
        User::Leave( err );
        }
 	    
    TBool mimeTypeRng = EFalse;
    
    if ( err == KErrNone )
        {
        if( dataType.Des().CompareF( KFileListRngMimeType ) == 0 )
            {
            mimeTypeRng = ETrue;
            }
        }

	TInt ringingType = RingingType();
	if ( ringingType == ERingingTypeBeepOnce )
		{
		// Active profile ringing tone is set to Beep Once
		// Don't initialize a FileSequence but use DesSequence instead
 		iTonePlayer = CMdaAudioToneUtility::NewL( *this );
		iTonePlayer->PrepareToPlayDesSequence( KFileListBeepSequence() );
		iTonePlayerStatus = EPlayerInitializing;
		}
    else
		{
        if( mimeTypeRng )
            {
            //Ringingtone is a RNG-file
            iTonePlayer = CMdaAudioToneUtility::NewL( *this );
            iTonePlayer->PrepareToPlayFileSequence( fileName );
            iTonePlayerStatus = EPlayerInitializing;
            }
        else
            {
            delete iAudioPlayer;
            iAudioPlayer = NULL;

            iAudioPlayer = CDrmPlayerUtility::NewFilePlayerL(
             	fileName, *this, KAudioPriorityRingingTonePreview,
             	( TMdaPriorityPreference )KAudioPrefRingFilePreview );
	
            iAudioPlayerStatus = EPlayerInitializing;
            }
		}
		
    EnableScreenSaver( EFalse );
    DisableBackLight();
	
#ifdef _DEBUG
    User::InfoPrint( iFullName->Des() );
    FLOG( _L("CMFAudioPreviewHandler::PlayL  '%S'"), &iFullName->Des() );
#endif

    }


// -----------------------------------------------------------------------------
// CMFAudioPreviewHandler::Stop
// 
// -----------------------------------------------------------------------------
void CMFAudioPreviewHandler::Stop()
	{
	Cancel();
	}


// -----------------------------------------------------------------------------
// CMFAudioPreviewHandler::ConvertVolume
//
// -----------------------------------------------------------------------------
//
TInt CMFAudioPreviewHandler::ConvertVolume( TInt aVolume )
    {
    TInt result = 0;
    if ( iAudioPlayer )
        {
        result = CMFPreviewHandlerBase::ConvertVolume( aVolume, iAudioPlayer->MaxVolume() );
        }
    else if ( iTonePlayer )
        {
        result = CMFPreviewHandlerBase::ConvertVolume( aVolume, iTonePlayer->MaxVolume() );
        }
        
    //if user has selected silent ringing type, set volume off
    TInt ringingType = RingingType();
    if( ringingType == ERingingTypeSilent )
        {
        result = 0;
        }

    return result;
    }


// -----------------------------------------------------------------------------
// CMFAudioPreviewHandler::SetToneRingingType
//
// -----------------------------------------------------------------------------
//
void CMFAudioPreviewHandler::SetToneRingingType( TInt aRingingType )
    {
    const TInt KToneInterval = 1000000; // 1 second pause between tones
    const TInt KAscendingVolumeInterval = 3000000; // 3 seconds
    
    if ( !iTonePlayer )
        {
        return;
        }
    
    __ASSERT_DEBUG( iTonePlayerStatus == EPlayerInitialized, Panic( KErrGeneral ) );


    TInt ringingVolume = RingingVolume();

    switch( aRingingType )
        {
    	case ERingingTypeRinging:
		case ERingingTypeSilent:
            {
            iTonePlayer->SetRepeats( KMdaAudioToneRepeatForever,
                                     TTimeIntervalMicroSeconds( KToneInterval ) );
            break;
            }
        case ERingingTypeAscending:
            {
            iTonePlayer->SetRepeats( KMdaAudioToneRepeatForever,
                                     TTimeIntervalMicroSeconds( KToneInterval ) );
            
            TInt volRamp = KAscendingVolumeInterval * ringingVolume;
            iTonePlayer->SetVolumeRamp( TTimeIntervalMicroSeconds( volRamp ) );
            break;
            }
	    case ERingingTypeRingOnce:
		case ERingingTypeBeepOnce:
            {
            iTonePlayer->SetRepeats( 0, TTimeIntervalMicroSeconds( KToneInterval ) );
            break;
            }
	    default:
            {
            break;
            }
        }
    }


// -----------------------------------------------------------------------------
// CMFAudioPreviewHandler::SetAudioRingingType
//
// -----------------------------------------------------------------------------
//
void CMFAudioPreviewHandler::SetAudioRingingType( TInt aRingingType )
    {
    const TInt KToneInterval = 1000000; // 1 second pause between tones
    const TInt KAscendingVolumeInterval = 3000000; // 3 seconds
    
    if ( !iAudioPlayer )
        {
        return;
        }

    __ASSERT_DEBUG( iAudioPlayerStatus == EPlayerInitialized, Panic( KErrGeneral ) );


    TInt ringingVolume = RingingVolume();
        
    switch( aRingingType )
        {
        case ERingingTypeRinging:
        case ERingingTypeSilent:
            {
            iAudioPlayer->SetRepeats( KMdaAudioToneRepeatForever,
                                      TTimeIntervalMicroSeconds( KToneInterval ) );
            break;
            }
        case ERingingTypeAscending:
            {
            iAudioPlayer->SetRepeats( KMdaAudioToneRepeatForever,
                                      TTimeIntervalMicroSeconds( KToneInterval ) );
            TInt volRamp = KAscendingVolumeInterval * ringingVolume;
            iAudioPlayer->SetVolumeRamp( TTimeIntervalMicroSeconds( volRamp ) );
		    break;
            }
        case ERingingTypeRingOnce:
            {
            iAudioPlayer->SetRepeats( 0, TTimeIntervalMicroSeconds( KToneInterval ) );
            break;
            }

        default:
            {
            break;
            }
        }
    }


// -----------------------------------------------------------------------------
// CMFAudioPreviewHandler::Cancel
//
// -----------------------------------------------------------------------------
//
void CMFAudioPreviewHandler::Cancel()
    {
   	TBool isPlaying = EFalse;
   	
   	if ( iAudioPlayer )
   	    {
   	    isPlaying = ETrue;
   	    if ( iAudioPlayerStatus == EPlayerPlayingWith3DEffect )
   	        {
   	        i3dRingingTonePlugin->Stop();
   	        // plugin calls AudioPlayer->Stop()
	        iAudioPlayer->Close();
   	        }
     	if ( iAudioPlayerStatus == EPlayerPlaying )
            {
      	    iAudioPlayer->Stop();
    	    iAudioPlayer->Close();
            }
        
        delete iAudioPlayer;
        iAudioPlayer = NULL;
        iAudioPlayerStatus = EPlayerNotCreated;
  	    }
    
	if ( iTonePlayer )
        {
        isPlaying = ETrue;
        if ( iTonePlayerStatus == EPlayerPlaying )
            {
    	    iTonePlayer->CancelPlay();
	        }
        
        delete iTonePlayer;
        iTonePlayer = NULL;
        iTonePlayerStatus = EPlayerNotCreated;
        }
        

   	if ( isPlaying )
   	    {
   	    //User::InfoPrint(_L("cancel"));
   	    EnableScreenSaver( ETrue );
   	    iBacklightTimer->Cancel();
   	    }
    }


// -----------------------------------------------------------------------------
// CMFAudioPreviewHandler::MatoPlayComplete (from MMdaAudioToneObserver)
//
// -----------------------------------------------------------------------------
//
void CMFAudioPreviewHandler::MatoPlayComplete( TInt aError )
    {
    if ( iObserver )
        {
        TInt event = MPreviewHandlerObserver::EAudioPreviewComplete;
        TRAP_IGNORE( iObserver->HandlePreviewEventL( event, aError ) );
        }
    
	Cancel();
    }


// -----------------------------------------------------------------------------
// CMFAudioPreviewHandler::MatoPrepareComplete (from MMdaAudioToneObserver)
//
// -----------------------------------------------------------------------------
//
void CMFAudioPreviewHandler::MatoPrepareComplete( TInt aError )
    {
    __ASSERT_DEBUG( iTonePlayerStatus == EPlayerInitializing, Panic( KErrGeneral ) );

    if ( aError != KErrNone )
        {
        Cancel();
        
        if ( iObserver )
            {
            TInt event = MPreviewHandlerObserver::EPreviewError;
            TRAP_IGNORE( iObserver->HandlePreviewEventL( event, aError ) );
            }
        
        return;
        }

    TInt ringingVolume = RingingVolume();
    TInt ringingType = RingingType();
    TInt vibra = Vibra();
       
    iTonePlayerStatus = EPlayerInitialized;
    SetToneRingingType( ringingType );
    iTonePlayer->SetVolume( ConvertVolume( ringingVolume ) );
    
    TMdaPriorityPreference pref = (TMdaPriorityPreference) KAudioPrefRingFilePreview;               
    if ( vibra )
        {
        pref = (TMdaPriorityPreference) KAudioPrefRingFilePreviewVibra;
        }
    iTonePlayer->SetPriority( KAudioPriorityPreview, pref );
                                             
    iTonePlayer->Play();
    iTonePlayerStatus = EPlayerPlaying;
    }


// -----------------------------------------------------------------------------
// CMFAudioPreviewHandler::MdapcInitComplete (from MDrmAudioPlayerCallback)
// 
// -----------------------------------------------------------------------------
//
void CMFAudioPreviewHandler::MdapcInitComplete( TInt aError, 
                        const TTimeIntervalMicroSeconds& /* aDuration */ )
    {
    __ASSERT_DEBUG( iAudioPlayer, Panic( KErrGeneral ) );

    if ( aError != KErrNone )
        {
        Cancel();

        if ( iObserver )
            {
            TInt event = MPreviewHandlerObserver::EPreviewError;
            TRAP_IGNORE( iObserver->HandlePreviewEventL( event, aError ) );
            }
        return;
        }
        

    TInt ringingVolume = RingingVolume();
    TInt ringingType = RingingType();
    TInt vibra = Vibra();
    TInt echo3D = Echo3D();
    TInt effect3D = Effect3D();

    __ASSERT_DEBUG( ringingType != ERingingTypeBeepOnce,
                     TMFDialogUtil::Panic( KErrGeneral ) );

    iAudioPlayerStatus = EPlayerInitialized;
    SetAudioRingingType( ringingType );
    iAudioPlayer->SetVolume( ConvertVolume( ringingVolume ) );
    
    TMdaPriorityPreference pref = (TMdaPriorityPreference) KAudioPrefRingFilePreview;               
    if ( vibra )
        {
        pref = (TMdaPriorityPreference) KAudioPrefRingFilePreviewVibra;
        }
    iAudioPlayer->SetPriority( KAudioPriorityPreview, pref );
     
    iAudioPlayerStatus = EPlayerPlaying;
    
    if ( effect3D == EProfile3DEffectOff )
        {
        iAudioPlayer->Play();  // 3D not used
       	return;
        }

    if ( !i3dRingingTonePlugin )
        {
        TUid emptyUid = { 0 };
        TRAPD( err, i3dRingingTonePlugin = C3DRingingToneInterface::NewL( emptyUid ) );
        if ( err != KErrNone || !i3dRingingTonePlugin )
            {
       	    iAudioPlayer->Play();
       	    return;
            }
        }       
   
    i3dRingingTonePlugin->SetAttr( E3DRTIAttr3DEffect, effect3D );
    i3dRingingTonePlugin->SetAttr( E3DRTIAttr3DEcho, echo3D );    
    i3dRingingTonePlugin->SetAttr( E3DRTIAttrDrmPlayerUtility, iAudioPlayer );
    TRAP_IGNORE( i3dRingingTonePlugin->PlayL() );

    iAudioPlayerStatus = EPlayerPlayingWith3DEffect;
    }


// -----------------------------------------------------------------------------
// CMFAudioPreviewHandler::MdapcPlayComplete (from MDrmAudioPlayerCallback)
//
// -----------------------------------------------------------------------------
//
void CMFAudioPreviewHandler::MdapcPlayComplete( TInt aError )
    {
    if ( iObserver )
        {
        TInt event = MPreviewHandlerObserver::EAudioPreviewComplete;
        TRAP_IGNORE( iObserver->HandlePreviewEventL( event, aError ) );
        }

	Cancel();
    }



//  End of File  
