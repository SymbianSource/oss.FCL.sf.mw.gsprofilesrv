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
*     CFLDRingingTonePlayer is used to play sound files. It takes into account
*     the current volume and ringing type settings.
*
*
*/


// CLASS HEADER
#include "CFLDRingingTonePlayer.h"

// EXTERNAL INCLUDES
#include <AudioPreference.h>
#include <mda/common/resource.h>
#include <c3dringingtoneinterface.h>		// C3DRingingToneInterface
#include <ScreensaverInternalPSKeys.h>

// CONSTANTS
namespace
	{
	const TInt KAscendingVolumeInterval( 3000000 ); // 3 seconds
	const TInt KToneInterval( 1000000 ); // 1 second pause between tones

    _LIT( KDataTypeNone, "" );
	_LIT( KFileListRngMimeType, "application/vnd.nokia.ringing-tone" );
	_LIT8( KFileListBeepSequence, "\x00\x11\x06\x0A\x08\x73\x0A\x40\x28\x0A\xF7\
	\x05\xFC\x40\x64\x0A\x08\x40\x32\x0A\xF7\x06\x0B" ); //sequence for playing a beep once sound
	}

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CFLDRingingTonePlayer::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CFLDRingingTonePlayer* CFLDRingingTonePlayer::NewL( TBool aShowErrorMsgs )
    {
    CFLDRingingTonePlayer* self = new( ELeave ) CFLDRingingTonePlayer( aShowErrorMsgs );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self ); // self
    return self;
    }

// Destructor
CFLDRingingTonePlayer::~CFLDRingingTonePlayer()
    {
    Cancel(); // Stops possible playback

	delete iAudioPlayer;
	delete iTonePlayer;
	
	delete i3dRingingTonePlugin;
    }

// -----------------------------------------------------------------------------
// CFLDRingingTonePlayer::CFLDRingingTonePlayer
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CFLDRingingTonePlayer::CFLDRingingTonePlayer( TBool aShowErrorMsgs )
    :   CFLDPlayerBase( aShowErrorMsgs ),
        iAudioPlayerStatus( EAudioPlayerNotCreated ),
		iTonePlayerStatus( EAudioPlayerNotCreated )
    {
    i3dEffects = ETrue;
    i3DEffect = KErrNotFound;
    i3DEcho = KErrNotFound;
    }

// -----------------------------------------------------------------------------
// CFLDRingingTonePlayer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CFLDRingingTonePlayer::ConstructL()
    {
    BaseConstructL();

   	// To allow/not allow screensaver
   	// Errors ignored, no actions needed if API is not available
    iPropScreenSaver.Attach( KPSUidScreenSaver, KScreenSaverAllowScreenSaver );

    }

// -----------------------------------------------------------------------------
// CFLDRingingTonePlayer::ProcessFileL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDRingingTonePlayer::ProcessFileL( const TDesC& aFileName, RWindow* /*aWindow*/ )
    {
    if ( aFileName == KNullDesC || iFocusLost )
        {
        return;
        }

    Cancel(); // Stops possible playback

	TDataType dataType;
	TInt error( DataType( aFileName, dataType ) );
    if( ( error != KErrNotFound ) && ( error != KErrNone ) )
        {
        User::Leave( error );
        }

   	// Do not allow screen saver while playing
    // Errors ignored, no actions needed if API is not available	   	
   	iPropScreenSaver.Set( KPSUidScreenSaver,
			KScreenSaverAllowScreenSaver, EFLScreenSaverNotAllowed );

	delete iTonePlayer;
    iTonePlayer = NULL;
	
    delete iAudioPlayer;
    iAudioPlayer = NULL;

	if ( iRingingType == ERingingTypeBeepOnce )
		{
		// Active profile ringing tone is set to Beep Once
		// Don't initialize a FileSequence but use DesSequence instead
        iTonePlayer = CMdaAudioToneUtility::NewL( *this );
		iTonePlayer->PrepareToPlayDesSequence( KFileListBeepSequence() );
		iTonePlayerStatus = EAudioPlayerInitializing;
		}
    else
		{
		TPtrC ptr( aFileName );
        if( error )
            {
            ptr.Set( iDefaultTone );
            }
        if( dataType.Des().CompareF( KFileListRngMimeType ) == 0 )
            {
            //Ringingtone is a RNG-file
            iTonePlayer = CMdaAudioToneUtility::NewL( *this );
            iTonePlayer->PrepareToPlayFileSequence( ptr );
            iTonePlayerStatus = EAudioPlayerInitializing;
            }
        // If the mime type is NULL, the file can't be play.
        else if ( dataType.Des().CompareF( KDataTypeNone ) == 0 )
            {
            return;
            }
        else
            {
            iAudioPlayer = CDrmPlayerUtility::NewFilePlayerL(
             	ptr, *this, KAudioPriorityRingingTonePreview,
                TMdaPriorityPreference ( 
                iVibra ? KAudioPrefRingFilePreviewVibra :
                KAudioPrefRingFilePreview ) );
	
            iAudioPlayerStatus = EAudioPlayerInitializing;
            }
		}
    }

// -----------------------------------------------------------------------------
// CFLDRingingTonePlayer::Cancel()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDRingingTonePlayer::Cancel()
    {
   	if ( iAudioPlayerStatus == EAudioPlayerPlayingWith3DEffect )
   	    {
   	    i3dRingingTonePlugin->Stop();
   	    // plugin calls AudioPlayer->Stop()
	    iAudioPlayer->Close();
	    delete iAudioPlayer;
	    iAudioPlayer = NULL;
   	    }
    
   	if ( iAudioPlayerStatus == EAudioPlayerPlaying )
        {
        if( iAudioPlayer )
	        {
    	    iAudioPlayer->Stop();
    	    iAudioPlayer->Close();
    	    delete iAudioPlayer;
    	    iAudioPlayer = NULL;
	        }
        }

    iAudioPlayerStatus = EAudioPlayerReady;

	if ( iTonePlayerStatus == EAudioPlayerPlaying )
        {
        if( iTonePlayer )
	        {
    	    iTonePlayer->CancelPlay();
    	    delete iTonePlayer;
    	    iTonePlayer = NULL;
	        }
        }
    iTonePlayerStatus = EAudioPlayerReady;

   	// Allow screen saver, unless there's a call ongoing
   	if( !IsCallOngoing() )
   		{
	    // Errors ignored, no actions needed if API is not available	   	
	   	iPropScreenSaver.Set( KPSUidScreenSaver,
	    		KScreenSaverAllowScreenSaver, EFLScreenSaverAllowed );
   		}

}

// -----------------------------------------------------------------------------
// CFLDRingingTonePlayer::DoSetRingingType()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDRingingTonePlayer::DoSetRingingType( TInt aRingingType )
    {
    if( iAudioPlayerStatus == EAudioPlayerInitialized )
        {
        switch( aRingingType )
            {
			// Fall through
            case ERingingTypeRinging:
            case ERingingTypeSilent:
                {
                iAudioPlayer->SetRepeats(
                 KMdaRepeatForever, TTimeIntervalMicroSeconds( KToneInterval ) );
                break;
                }
            case ERingingTypeAscending:
                {
                iAudioPlayer->SetRepeats(
                	KMdaRepeatForever, TTimeIntervalMicroSeconds( KToneInterval ) );
                TInt volRamp = KAscendingVolumeInterval * iRingingVolume;
                iAudioPlayer->SetVolumeRamp( TTimeIntervalMicroSeconds( volRamp ) );
				break;
                }
            case ERingingTypeRingOnce:
                {
                iAudioPlayer->SetRepeats(
                 0, TTimeIntervalMicroSeconds( KToneInterval ) );
                break;
                }

            default:
                {
                break;
                }
            }
        }
    else if( iTonePlayerStatus == EAudioPlayerInitialized )
        {
        switch( aRingingType )
            {
			// Fall through
            case ERingingTypeRinging:
			case ERingingTypeSilent:
                {
                iTonePlayer->SetRepeats(
                 KMdaRepeatForever, TTimeIntervalMicroSeconds( KToneInterval ) );
                break;
                }
            case ERingingTypeAscending:
                {
                iTonePlayer->SetRepeats(
                 KMdaRepeatForever, TTimeIntervalMicroSeconds( KToneInterval ) );
                TInt volRamp = KAscendingVolumeInterval * iRingingVolume;
                iTonePlayer->SetVolumeRamp( TTimeIntervalMicroSeconds( volRamp ) );
                break;
                }
			// Fall through
            case ERingingTypeRingOnce:
			case ERingingTypeBeepOnce:
                {
                iTonePlayer->SetRepeats(
                 0, TTimeIntervalMicroSeconds( KToneInterval ) );
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
// CFLDRingingTonePlayer::ConvertVolume()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CFLDRingingTonePlayer::ConvertVolume( TInt aVolume )
    {
    TInt result( 0 );
    if ( iAudioPlayerStatus == EAudioPlayerInitialized ||
        iAudioPlayerStatus == EAudioPlayerPlaying )
        {
        result = BaseConvertVolume( aVolume, iAudioPlayer->MaxVolume() );
        }
    else if( iTonePlayerStatus == EAudioPlayerInitialized ||
        iTonePlayerStatus == EAudioPlayerPlaying )
        {
        result = BaseConvertVolume( aVolume, iTonePlayer->MaxVolume() );
        }
	//if user has selected silent ringing type, set volume off
	if( iRingingType == ERingingTypeSilent )
		{
		result = 0;
		}

    return result;
    }

// -----------------------------------------------------------------------------
// CFLDRingingTonePlayer::MdapcInitComplete()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDRingingTonePlayer::MdapcInitComplete(
    TInt aError, const TTimeIntervalMicroSeconds& /* aDuration */ )
    {
    if ( !aError && iAudioPlayerStatus == EAudioPlayerInitializing )
        {
        iAudioPlayerStatus = EAudioPlayerInitialized;
        DoSetRingingType( iRingingType );
        iAudioPlayer->SetVolume( ConvertVolume( iRingingVolume ) );
		iAudioPlayer->SetPriority( KAudioPriorityPreview,
            TMdaPriorityPreference( iVibra ? KAudioPrefRingFilePreviewVibra :
                                             KAudioPrefRingFilePreview ) );
                                             
        if ( iRingingType == ERingingTypeBeepOnce )
            {
            return; // beeb once not handled here
            }
        
        if( !i3dEffects )
            {
            iAudioPlayer->Play();
         	iAudioPlayerStatus = EAudioPlayerPlaying;
         	return;
            }

        if ( !i3dRingingTonePlugin )
            {
            TUid emptyUid = { 0 };
            TRAPD( err, i3dRingingTonePlugin = C3DRingingToneInterface::NewL( emptyUid ) );
            if ( err != KErrNone || !i3dRingingTonePlugin )
                {
           	    iAudioPlayer->Play();
          	    iAudioPlayerStatus = EAudioPlayerPlaying;
           	    return;
                }
            }
        if ( i3DEffect != KErrNotFound && i3DEcho != KErrNotFound )
            {
            i3dRingingTonePlugin->SetAttr( E3DRTIAttr3DEffect, i3DEffect );
            i3dRingingTonePlugin->SetAttr( E3DRTIAttr3DEcho, i3DEcho );
            }
            
        i3dRingingTonePlugin->SetAttr( E3DRTIAttrDrmPlayerUtility, iAudioPlayer );
        TRAP_IGNORE( i3dRingingTonePlugin->PlayL() );
        iAudioPlayerStatus = EAudioPlayerPlayingWith3DEffect;
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
// CFLDRingingTonePlayer::MdapcPlayComplete()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDRingingTonePlayer::MdapcPlayComplete( TInt aError )
    {
    if ( ( aError == KErrNotSupported ) || ( aError == KErrCorrupt ) )
        {
        // Don't care about leave, if the note can't be displayed.
        TRAP_IGNORE( DisplayErrorNoteL() );
        }

	Cancel();
    }

// -----------------------------------------------------------------------------
// CFLDRingingTonePlayer::MatoPrepareComplete()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDRingingTonePlayer::MatoPrepareComplete( TInt aError )
    {
    if ( !aError && iTonePlayerStatus == EAudioPlayerInitializing )
        {
        iTonePlayerStatus = EAudioPlayerInitialized;
        DoSetRingingType( iRingingType );
        iTonePlayer->SetVolume( ConvertVolume( iRingingVolume ) );
        iTonePlayer->SetPriority( KAudioPriorityPreview,
            TMdaPriorityPreference( iVibra ? KAudioPrefRingFilePreviewVibra :
                                             KAudioPrefRingFilePreview ) );
        iTonePlayer->Play();
        iTonePlayerStatus = EAudioPlayerPlaying;
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
// CFLDRingingTonePlayer::MatoPlayComplete()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDRingingTonePlayer::MatoPlayComplete( TInt aError )
    {
    if ( ( aError == KErrNotSupported ) || ( aError == KErrCorrupt ) )
        {
        // Don't care about leave, if the note can't be displayed.
        TRAP_IGNORE( DisplayErrorNoteL() );
        }

	Cancel();
    }


// -----------------------------------------------------------------------------
// CFLDRingingTonePlayer::Set3dEffects()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDRingingTonePlayer::Set3dEffects( TBool a3dEffects )
	{
	const TInt K3DEffectBase = 1000;
    const TInt K3DEchoBase =   2000;
    
	i3dEffects = a3dEffects;
	
	if ( a3dEffects >= K3DEffectBase && a3dEffects < K3DEchoBase )
	    {
	    i3DEffect = a3dEffects - K3DEffectBase;
	    return;
	    }
	
	if ( a3dEffects >= K3DEchoBase )
	    {
	    i3DEcho = a3dEffects - K3DEchoBase;
	    return;
	    }
	}


//  End of File
