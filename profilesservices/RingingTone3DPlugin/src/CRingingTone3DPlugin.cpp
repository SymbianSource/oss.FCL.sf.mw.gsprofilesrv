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
* Description:  Plug-In that implements C3DRingingToneInterface.
*
*/


// INCLUDE FILES
#include "CRingingTone3DPlugin.h"
#include "tdaudioconstants.h"
#include "RingingTone3DPluginDebug.h"

#include <EnvironmentalReverbUtility.h>       // CEnvironmentalReverbUtility
#include <SourceDopplerBase.h>                // CSourceDoppler
#include <SourceLocationBase.h>               // CSourceLocation
#include <ListenerLocationBase.h>             // CListenerLocation

#ifndef SPP_NSS_3D_RINGING_TONE_STW_DISABLED
#include <StereoWideningBase.h>               // CStereoWidening
#endif


#include <MProfileEngine.h>
#include <MProfile.h>
#include <MProfileTones.h>
#include <MProfileExtraSettings.h>
#include <MProfile3DToneSettings.h>
#include <MProfileName.h>
#include <ProfileInternal.hrh> // for TProfile3DToneEffect and TProfile3DToneEcho


// CONSTANTS
const TUint32 KDopplerFactor = 100;




// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CRingingTone3DPlugin::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CRingingTone3DPlugin* CRingingTone3DPlugin::NewL( TAny* aParameter )
    {
    // aParameter comes from REComSession::CreateImplementationL
    // (TAny *aConstructionParameters)
    
    CRingingTone3DPlugin* self = new ( ELeave ) CRingingTone3DPlugin();
    CleanupStack::PushL( self );
    self->ConstructL( aParameter );
    CleanupStack::Pop();
    
    return self;
    }

// Destructor
CRingingTone3DPlugin::~CRingingTone3DPlugin()
    {
    
    // Stop() may leave because of CDrmPlayerUtility error
    // (CMMFDrmAudioPlayerUtility::IsValidCustomCommandDestination
    // uses CleanupClosePushL).
    TRAP_IGNORE( Stop() );
    
    delete iTimer;
    delete iParser;
    }
    
    
// -----------------------------------------------------------------------------
// CRingingTone3DPlugin::Stop
// Stop the playback.
// -----------------------------------------------------------------------------
//
void CRingingTone3DPlugin::Stop()
    {
    FLOG( _L("CRingingTone3DPlugin::Stop START") );
    
    // Stop the playback and timer.
    if ( iTimer )
        {
        iTimer->Cancel();    
        }
    
    StopRingingTone();

    delete iReverbEffect;
    iReverbEffect = NULL;
    
    delete iDopplerEffect;
    iDopplerEffect = NULL;
    
    delete iListener;
    iListener = NULL;
    
    delete iSource;
    iSource = NULL;
    
#ifdef __STEREO_WIDENING_EFFECT_UI     
    delete iStereoWidening;
    iStereoWidening = NULL;
#endif    
    
    FLOG( _L("CRingingTone3DPlugin::Stop END") );
    }


// -----------------------------------------------------------------------------
// CRingingTone3DPlugin::CRingingTone3DPlugin
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CRingingTone3DPlugin::CRingingTone3DPlugin()
    {
    iPlaying = EFalse;
    iPlayerType = KErrNotFound;
    iProfileId = KErrNotFound;
 
    iProfile3DToneEffect = KErrNotFound;
    iProfile3DToneEcho = KErrNotFound;
    iRingingTone = KNullDesC;
    }


// -----------------------------------------------------------------------------
// CRingingTone3DPlugin::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CRingingTone3DPlugin::ConstructL( TAny* aParameter )
    {
    FLOG( _L("CRingingTone3DPlugin::ConstructL START") );

    iTimer = C3DAudioTimeOutTimer::NewL( EPriorityHigh, this );
    iParser = C3DAudioXMLParser::NewL();

    FLOG( _L("CRingingTone3DPlugin::ConstructL END") ); 
    }


// -----------------------------------------------------------------------------
// CRingingTone3DPlugin::Init
// 
// -----------------------------------------------------------------------------
//
void CRingingTone3DPlugin::Init( CMdaAudioPlayerUtility* aSamplePlayer )
    {
    FLOG( _L("CRingingTone3DPlugin::InitL START") );
    
    __ASSERT_ALWAYS( aSamplePlayer, Panic(KErrArgument) );
    
    iAudioPlayerUtility = aSamplePlayer;
    iPlayerType = EMdaAudioPlayerUtility;
  
#ifdef _DEBUG			                
    FlogPlayer( iPlayerType );
#endif        
    
    FLOG( _L("CRingingTone3DPlugin::InitL END") );
    }
    
    
// -----------------------------------------------------------------------------
// CRingingTone3DPlugin::Init
// 
// -----------------------------------------------------------------------------
//
void CRingingTone3DPlugin::Init( CMdaAudioToneUtility* aSamplePlayer )
    {
    FLOG( _L("CRingingTone3DPlugin::InitL START") );
    
    __ASSERT_ALWAYS( aSamplePlayer, Panic(KErrArgument) );
    
    iAudioToneUtility = aSamplePlayer;
    iPlayerType = EMdaAudioToneUtility;

#ifdef _DEBUG			                
    FlogPlayer( iPlayerType );
#endif        
    
    FLOG( _L("CRingingTone3DPlugin::InitL END") );
    }


// -----------------------------------------------------------------------------
// CRingingTone3DPlugin::Init
// 
// -----------------------------------------------------------------------------
//
void CRingingTone3DPlugin::Init( CDrmPlayerUtility* aSamplePlayer )
    {
    //FLOG( _L("CRingingTone3DPlugin::InitL START") );
    
    __ASSERT_ALWAYS( aSamplePlayer, Panic(KErrArgument) );
    
    iDrmPlayerUtility = aSamplePlayer;
    iPlayerType = EDrmPlayerUtility;

#ifdef _DEBUG			                
    FlogPlayer( iPlayerType );
#endif        
    
    //FLOG( _L("CRingingTone3DPlugin::InitL END") );
    }


// -----------------------------------------------------------------------------
// CRingingTone3DPlugin::Init
// 
// -----------------------------------------------------------------------------
//
void CRingingTone3DPlugin::Init( CVideoPlayerUtility* aSamplePlayer )
    {
    FLOG( _L("CRingingTone3DPlugin::InitL START") );
    
    __ASSERT_ALWAYS( aSamplePlayer, Panic(KErrArgument) );
    
    iVideoPlayerUtility = aSamplePlayer;
    iPlayerType = EVideoPlayerUtility;

#ifdef _DEBUG			                
    FlogPlayer( iPlayerType );
#endif        
    
    FLOG( _L("CRingingTone3DPlugin::InitL END") );
    }


// -----------------------------------------------------------------------------
// CRingingTone3DPlugin::SetAttr (from C3DRingingToneInterface.h)
// 
// -----------------------------------------------------------------------------
//
TInt CRingingTone3DPlugin::SetAttr( T3DRingingToneInterfaceAttr aAttributeKey,
                                    TInt aValue )
    {
    switch (aAttributeKey)
        {
        case E3DRTIAttrProfileId:
            {
            iProfileId = aValue;
            break;
            }
        case E3DRTIAttr3DEffect:
            {
            iProfile3DToneEffect = aValue;
            break;
            }
        case E3DRTIAttr3DEcho:
            {
            iProfile3DToneEcho = aValue;
            break;
            }
        default:
            {
            break;
            }
        }
        
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CRingingTone3DPlugin::SetAttr  (from C3DRingingToneInterface.h)
// 
// -----------------------------------------------------------------------------
//
TInt CRingingTone3DPlugin::SetAttr( T3DRingingToneInterfaceAttr aAttributeKey,
                                    const TDesC& aValue )
    {
    switch (aAttributeKey)
        {
        case E3DRTIAttr3DRingingTone:
            {
            if ( aValue.Length() < iRingingTone.MaxLength() )
                {
                iRingingTone = aValue;    
                }
            break;
            }
        default:
            {
            break;
            }
        }
        
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CRingingTone3DPlugin::SetAttr  (from C3DRingingToneInterface.h)
// 
// -----------------------------------------------------------------------------
//
TInt CRingingTone3DPlugin::SetAttr( TInt aAttributeKey, TAny* aValue )
    {
    switch (aAttributeKey)
        {
        case E3DRTIAttrAudioPlayerUtility:
            {
            Init( ( CMdaAudioPlayerUtility* )aValue );
            break;
            }
        case E3DRTIAttrAudioToneUtility:
            {
            Init( ( CMdaAudioToneUtility* )aValue );
            break;
            }
        case E3DRTIAttrDrmPlayerUtility:
            {
            Init( ( CDrmPlayerUtility* )aValue );
            break;
            }
        case E3DRTIAttrVideoPlayerUtility:
            {
            Init( ( CVideoPlayerUtility* )aValue );
            break;
            }

        default:
            {
            break;
            }
        }
        
    return KErrNone;    
    }


// -----------------------------------------------------------------------------
// CRingingTone3DPlugin::PlayL
// Start the playback, initialize first in case there has been changes.
// -----------------------------------------------------------------------------
//
void CRingingTone3DPlugin::PlayL()
    {
    TInt err = KErrNone;
    
    TRAP( err, DoPlayL() );
    
    if ( err != KErrNone )
        {
        // 3D play failed - play normal ringing tone
        UnInitialize();
        PlayRingingTone();
        }
    }


// -----------------------------------------------------------------------------
// CRingingTone3DPlugin::DoPlayL
//
// Start the playback, initialize first in case there has been changes.
// -----------------------------------------------------------------------------
//
void CRingingTone3DPlugin::DoPlayL()
    {
    FLOG( _L("CRingingTone3DPlugin::DoPlayL START") );
    
    __ASSERT_ALWAYS( iPlayerType != KErrNotFound, Panic(KErrNotReady) );
    
    TInt err = KErrNone;

    if ( iTimer )
        {
        iTimer->Cancel();    
        }
    else
        {
        User::Leave( KErrNoMemory );  // ConstructL has failed
        }
    
    TRAP( err, InitializeL() );  // Initialize the plug-in

    if ( err != KErrNone )
        {
        FLOG( _L("### CRingingTone3DPlugin::InitializeL failed (%d) ###"), err);
        User::Leave(err);
        }
        
    if ( iProfile3DToneEffect == EProfile3DEffectOff )
        {
        PlayRingingTone();  // play normal ringing tone
        
        FLOG( _L("CRingingTone3DPlugin::DoPlayL END") );
        return;
        }
  

#ifdef __STEREO_WIDENING_EFFECT_UI
    if ( iStereoWidening )
        {
        if ( !iStereoWidening->IsEnabled() )
            {
            FLOG( _L("### CStereoWidening not enabled ###") );
            
            TRAP( err, iStereoWidening->EnableL() );
            if ( err != KErrNone )
                {
                FLOG( _L("### CStereoWidening::EnableL failed (%d) ###"), err);
                User::Leave( err );    
                }
            }

        // play normal ringing tone with stereowidening
        PlayRingingTone();  
        
        FLOG( _L("CRingingTone3DPlugin::DoPlayL END") );
        return;
        }
#endif


    iCurrentUpdate = KErrNotFound;

    SetNext3DPatternPositionL();
        
    PlayRingingTone();  // Start playing 3D ringing tone.
    
    FLOG( _L("CRingingTone3DPlugin::DoPlayL END") );      
    }

    
// -----------------------------------------------------------------------------
// CRingingTone3DPlugin::TimerExpiredL
// Callback method for timer expiration.
// Used to update the location and doppler-effects.
// -----------------------------------------------------------------------------
//
void CRingingTone3DPlugin::TimerExpiredL()
    {
    //FLOG( _L("CRingingTone3DPlugin::TimerExpiredL START") );
    
    if ( iCurrentUpdate % 10 == 0 )
        {
        FLOG( _L("CRingingTone3DPlugin::TimerExpiredL called (count = %d)"), iCurrentUpdate);
        }

    SetNext3DPatternPositionL();
        
    //FLOG( _L("CRingingTone3DPlugin::TimerExpiredL END") );
    }



// -----------------------------------------------------------------------------
// CRingingTone3DPlugin::SetNext3DPatternPositionL
//
// Used to update the location and doppler-effects.
// -----------------------------------------------------------------------------
//
void CRingingTone3DPlugin::SetNext3DPatternPositionL()
    {
    //FLOG( _L("CRingingTone3DPlugin::SetNext3DPatternPositionL START") );
    
    TInt err = KErrNone;
    
    if ( !iSource || !iListener )
        {
        FLOG( _L("### CSourceLocation or CListenerLocation or C3DAudioPattern is NULL ###") );
        User::Leave( KErrDied );
        }
    
    if ( iCurrentUpdate == KErrNotFound )
        {
        iCurrentUpdate = 0;  // first position
        }
    else
        {
        iCurrentUpdate++;  // next position
        }
    
    // iCurrentUpdate out of bounds. Start from the beginning.
    if ( iCurrentUpdate >= iParser->CountL() )
        {
        iCurrentUpdate = 0;
        }
 

    //
    // update doppler-effect
    //
    if ( iDoppler  && iDopplerEffect )
        {
        // Current doppler-velocity.
        T3DVelocity velocity = iParser->VelocityL( iCurrentUpdate );
        
        // Update the velocity and apply.
        TRAP( err, iDopplerEffect->SetCartesianVelocityL( ( TInt32& )velocity.iXVector, 
                                                          ( TInt32& )velocity.iYVector, 
                                                          ( TInt32& )velocity.iZVector ) );
        if ( err != KErrNone )
            {
            FLOG( _L("### CSourceDoppler::SetCartesianVelocityL failed (%d) ###"), err);
            User::Leave( err );
            }                                                            

        if ( !iDopplerEffect->IsEnabled() )
            {
            FLOG( _L("### CSourceDoppler not enabled ###") );
            
            TRAP( err, iDopplerEffect->EnableL() );

            if ( err != KErrNone )
                {
                FLOG( _L("### CSourceDoppler::EnableL failed (%d) ###"), err);
                User::Leave( err );
                }
            }
        
        TRAP( err, iDopplerEffect->ApplyL() );
        if ( err != KErrNone )
            {
            FLOG( _L("### CSourceDoppler::ApplyL failed (%d) ###"), err);
            User::Leave( err );
            }
            
#ifdef _DEBUG			                
        //FlogVelocityL( velocity, iCurrentUpdate );
#endif        
        
        }
        
        
    //
    // set new 3D pattern position
    //
    T3DPosition position = iParser->PositionL( iCurrentUpdate );
    
    TRAP( err, iSource->SetLocationCartesianL( ( TInt32& )position.iX, 
                                               ( TInt32& )position.iY, 
                                               ( TInt32& )position.iZ ) );
    if ( err != KErrNone )
        {
        FLOG( _L("### CSourceLocation::SetLocationCartesianL failed (%d) ###"), err);
        User::Leave( err );
        }
        
    if ( !iSource->IsEnabled() )
        {
        FLOG( _L("### CSourceLocation not enabled ###") );

        TRAP( err, iSource->EnableL() );
        if ( err != KErrNone )
            {
            FLOG( _L("### CSourceLocation::EnableL failed (%d) ###"), err);
            User::Leave( err );
            }          
        }
    
    TRAP( err, iSource->ApplyL() );    
    if ( err != KErrNone )
        {
        FLOG( _L("### CSourceLocation::ApplyL failed (%d) ###"), err);
        User::Leave( err );
        }          
      

#ifdef _DEBUG			                
    //FlogPositionL( position, iCurrentUpdate );
#endif        

      
    //
    // if reverb is created and not enabled try to enable it.
    //
    if ( iReverbEffect )
        {
        TBool enabled = iReverbEffect->EnvironmentalReverb().IsEnabled();
        if ( !enabled )
            {
            FLOG( _L("### CEnvironmentalReverb not enabled ###") );

            TRAP( err, iReverbEffect->EnvironmentalReverb().EnableL() );
            if ( err != KErrNone )
                {
                FLOG( _L("### CEnvironmentalReverb::EnableL failed (%d) ###"), err);
                User::Leave( err );
                }          
            }
        }
  
  
    //
    // if listener location is created and not enabled try to enable it
    //
    if ( !iListener->IsEnabled() )
        {
        FLOG( _L("### CListnerLocation not enabled ###") );
        
        TRAP( err, iListener->EnableL() );
        if ( err != KErrNone )
            {
            FLOG( _L("### CListnerLocation::EnableL failed (%d) ###"), err);
            User::Leave( err );
            }
        }        


    // Get the update time and calculate it according to velocity set.
    TInt dTime = iParser->TimeL( iCurrentUpdate ) * 1000;
    
    CalculateDTime( dTime );
    
#ifdef _DEBUG			                
    //FlogTimeL( dTime, iCurrentUpdate );
#endif        

    // Start the timer if pattern changes.
    if ( dTime != 0 )
        {
        iTimer->After( dTime );    
        }
        
    //FLOG( _L("CRingingTone3DPlugin::SetNext3DPatternPositionL END") );
    }


// -----------------------------------------------------------------------------
// CRingingTone3DPlugin::CalculateDTime
// Calculate the time spent in current update, depending on the velocity set.
// -----------------------------------------------------------------------------
void CRingingTone3DPlugin::CalculateDTime( TInt& aTime )
    {
    // Velocity is set to zero, so no movement will appear.    
    if ( iVelocity == 0 )
        {
        aTime = 0;
        }
    // Velocity is under five (default), so the updates take more time.
    else if ( iVelocity < KMotionPatternDefaultVelocity )
        {
        aTime = aTime + ( ( aTime / 5 ) * ( KMotionPatternDefaultVelocity - iVelocity ) );
        }
    // Velocity is over five (default), so the updates take less time.
    else if ( iVelocity > KMotionPatternDefaultVelocity )
        {
        aTime = aTime - ( ( aTime / 6 ) * ( iVelocity - KMotionPatternDefaultVelocity ) );
        }
    }


// -----------------------------------------------------------------------------
// CRingingTone3DPlugin::InitializeL()
// Initialize the plugin to current settings.
// -----------------------------------------------------------------------------
//    
void CRingingTone3DPlugin::UnInitialize()
    {
    if ( iReverbEffect )
        {
        TRAP_IGNORE( iReverbEffect->DisableEnvironmentalReverbL() );
        }
    if ( iListener )
        {
        TRAP_IGNORE( iListener->DisableL() );
        }
    if ( iSource )
        {
        TRAP_IGNORE( iSource->DisableL() );
        }
        
    if ( iDopplerEffect )
        {
        TRAP_IGNORE( iDopplerEffect->DisableL() );
        }
    }


// -----------------------------------------------------------------------------
// CRingingTone3DPlugin::InitializeL()
// Initialize the plugin to current settings.
// -----------------------------------------------------------------------------
//
void CRingingTone3DPlugin::InitializeL()
    {
    FLOG( _L("CRingingTone3DPlugin::InitializeL START") );
    
    if ( iProfile3DToneEffect == KErrNotFound || iProfile3DToneEcho == KErrNotFound )
        {
        // profile 3D settings not available - get them from active profile 
        ReadActiveProfileL();
        }
 
    // if ringing tone has been set (SetAttr), check its validity
    if ( !CheckRingingTone() ) 
        {
        FLOG( _L("### invalid 3D ringing tone ###") );
        FLOG( _L("CRingingTone3DPlugin::InitializeL END") );
        User::Leave(KErrNotFound);
        }

    if ( iProfile3DToneEffect == EProfile3DEffectOff )
        {
        FLOG( _L("3D ringing tones not enabled") );
        FLOG( _L("CRingingTone3DPlugin::InitializeL END") );
        return;
        }

#ifdef _DEBUG
    Check3DPatternL(iProfile3DToneEffect);
    Check3DEchoL(iProfile3DToneEcho);
#endif    
        

#ifdef __STEREO_WIDENING_EFFECT_UI
    if ( iProfile3DToneEffect == EProfile3DEffectStereoWidening )        
        {
        const TInt KStereoWideningLevel = 50;
        
        if ( !iStereoWidening )
            {
            iStereoWidening = StereoWideningL();    
            }
                 
        iStereoWidening->SetStereoWideningLevelL( KStereoWideningLevel );
        iStereoWidening->EnableL();
        
        FLOG( _L("CStereoWidening used as 3D effect") );
        FLOG( _L("CRingingTone3DPlugin::InitializeL END") );
        return;
        }
#endif

    //
    // parse 3D pattern
    //
    TInt err = KErrNone;
    TRAP( err ,iParser->ParseL( iProfile3DToneEffect ) );
    if ( err != KErrNone )
        {
        FLOG( _L("### C3DAudioXMLParser::ParseL failed (err=%d, pattern=%d) ###"), err, iProfile3DToneEffect);
        User::Leave( err );
        }
      
    
#ifdef _DEBUG                         
    C3DAudioXMLParser::FlogPubSubL( iProfile3DToneEcho );
#endif

    iVelocity = iParser->Attribute( C3DAudioXMLParser::EKeyVelocity );
    iDoppler = iParser->Attribute( C3DAudioXMLParser::EKeyDopplerUsed );
    
    if ( iProfile3DToneEcho != EProfile3DEchoOff )
        {
        TInt err = KErrNone;
        
        if ( !iReverbEffect ) 
            {
            TRAP( err, iReverbEffect = EnvironmentalReverbUtilityL());
            if ( err != KErrNone )
                {
                FLOG( _L("### CEnvironmentalReverbUtility::NewL failed (%d) ###"), err);
                User::Leave(err);    
                }
            }

        // Apply the preset. This also enables reverb effect.
        TRAP( err, iReverbEffect->ApplyPresetL( iProfile3DToneEcho ) );
        if ( err != KErrNone )
            {
            FLOG( _L("### CEnvironmentalReverbUtility::ApplyPresetL failed (%d) ###"), err);
            User::Leave(err);    
            }
        }
               
                
    // SourceLocation and ListenerLocation are not created.
    if ( !iSource ) 
        {
        iSource = SourceLocationL();
        }
           
    if ( !iListener ) 
        {
        iListener = ListenerLocationL();
        }

    // Veriable to set the location.
    TInt32 zeroInt = 0;
                         
    // Set both locations to (0, 0, 0) and enable them.
    iListener->SetLocationCartesianL( zeroInt, zeroInt, zeroInt );
    iListener->EnableL();
                
    iSource->SetLocationCartesianL( zeroInt, zeroInt, zeroInt );
    iSource->EnableL();

    // Doppler is enabled
    if ( iDoppler )
        {
        // The effect is not created
        if ( !iDopplerEffect ) 
            {
            iDopplerEffect = SourceDopplerL();
            }
                    
        TInt maxFactor = iDopplerEffect->FactorMax();
        if ( KDopplerFactor > maxFactor )
            {
            iDopplerEffect->SetFactorL( maxFactor );
            FLOG( _L("### CSourceDoppler::SetFactorL: max factor (%d) overflow ###"), maxFactor );  
            }
        else
            {
            // Set doppler factor.
            iDopplerEffect->SetFactorL( KDopplerFactor );
            }
                  
        // Enable the effect.                
        iDopplerEffect->EnableL();
        }
    

    FLOG( _L("CRingingTone3DPlugin::InitializeL END") );
    }



// -----------------------------------------------------------------------------
// CRingingTone3DPlugin::EnvironmentalReverbUtilityL()
//
// -----------------------------------------------------------------------------
//    
CEnvironmentalReverbUtility* CRingingTone3DPlugin::EnvironmentalReverbUtilityL()
    {
    CEnvironmentalReverbUtility* ret = NULL;
    
    switch ( iPlayerType )
        {
        case EMdaAudioPlayerUtility:
            {
            ret = CEnvironmentalReverbUtility::NewL( *iAudioPlayerUtility );
            break;
            }
        case EMdaAudioToneUtility:
            {
            ret = CEnvironmentalReverbUtility::NewL( *iAudioToneUtility );
            break;
            }
        case EDrmPlayerUtility:
            {
            ret = CEnvironmentalReverbUtility::NewL( *iDrmPlayerUtility );
            break;
            }
        case EVideoPlayerUtility:
            {
            ret = CEnvironmentalReverbUtility::NewL( *iVideoPlayerUtility );
            break;
            }
        default:
            {
            Panic(KErrArgument);
            break;
            }
        }
        
    return ret;
    }


// -----------------------------------------------------------------------------
// CRingingTone3DPlugin::SourceLocationL()
//
// -----------------------------------------------------------------------------
//    
CSourceLocation* CRingingTone3DPlugin::SourceLocationL()
    {
    CSourceLocation* ret = NULL;
    
    switch ( iPlayerType )
        {
        case EMdaAudioPlayerUtility:
            {
            ret = CSourceLocation::NewL( *iAudioPlayerUtility );
            break;
            }
        case EMdaAudioToneUtility:
            {
            ret = CSourceLocation::NewL( *iAudioToneUtility );
            break;
            }
        case EDrmPlayerUtility:
            {
            ret = CSourceLocation::NewL( *iDrmPlayerUtility );
            break;
            }
        case EVideoPlayerUtility:
            {
            ret = CSourceLocation::NewL( *iVideoPlayerUtility );
            break;
            }
        default:
            {
            Panic(KErrArgument);
            break;
            }
        }
        
    return ret;
    }


// -----------------------------------------------------------------------------
// CRingingTone3DPlugin::ListenerLocationL()
//
// -----------------------------------------------------------------------------
//    
CListenerLocation* CRingingTone3DPlugin::ListenerLocationL()
    {
    CListenerLocation* ret = NULL;
    
    switch ( iPlayerType )
        {
        case EMdaAudioPlayerUtility:
            {
            ret = CListenerLocation::NewL( *iAudioPlayerUtility );
            break;
            }
        case EMdaAudioToneUtility:
            {
            ret = CListenerLocation::NewL( *iAudioToneUtility );
            break;
            }
        case EDrmPlayerUtility:
            {
            ret = CListenerLocation::NewL( *iDrmPlayerUtility );
            break;
            }
        case EVideoPlayerUtility:
            {
            ret = CListenerLocation::NewL( *iVideoPlayerUtility );
            break;
            }
        default:
            {
            Panic(KErrArgument);
            break;
            }
        }
        
    return ret;
    }



// -----------------------------------------------------------------------------
// CRingingTone3DPlugin::SourceDopplerL()
//
// -----------------------------------------------------------------------------
//    
CSourceDoppler* CRingingTone3DPlugin::SourceDopplerL()
    {
    CSourceDoppler* ret = NULL;
    
    switch ( iPlayerType )
        {
        case EMdaAudioPlayerUtility:
            {
            ret = CSourceDoppler::NewL( *iAudioPlayerUtility );
            break;
            }
        case EMdaAudioToneUtility:
            {
            ret = CSourceDoppler::NewL( *iAudioToneUtility );
            break;
            }
        case EDrmPlayerUtility:
            {
            ret = CSourceDoppler::NewL( *iDrmPlayerUtility );
            break;
            }
        case EVideoPlayerUtility:
            {
            ret = CSourceDoppler::NewL( *iVideoPlayerUtility );
            break;
            }
        default:
            {
            Panic(KErrArgument);
            break;
            }
        }
        
    return ret;
    }



#ifdef __STEREO_WIDENING_EFFECT_UI
// -----------------------------------------------------------------------------
// CRingingTone3DPlugin::StereoWideningL()
//
// -----------------------------------------------------------------------------
//    
CStereoWidening* CRingingTone3DPlugin::StereoWideningL()
    {
    CStereoWidening* ret = NULL;
    
    switch ( iPlayerType )
        {
        case EMdaAudioPlayerUtility:
            {
            ret = CStereoWidening::NewL( *iAudioPlayerUtility );
            break;
            }
        case EMdaAudioToneUtility:
            {
            ret = CStereoWidening::NewL( *iAudioToneUtility );
            break;
            }
        case EDrmPlayerUtility:
            {
            ret = CStereoWidening::NewL( *iDrmPlayerUtility );
            break;
            }
        case EVideoPlayerUtility:
            {
            ret = CStereoWidening::NewL( *iVideoPlayerUtility );
            break;
            }
        default:
            {
            Panic(KErrArgument);
            break;
            }
        }
        
    return ret;
    }
#endif


// -----------------------------------------------------------------------------
// CRingingTone3DPlugin::PlayRingingTone()
//
// -----------------------------------------------------------------------------
//    
void CRingingTone3DPlugin::PlayRingingTone()
    {
    if ( iPlaying )
        {
        return;
        }
    iPlaying = ETrue;
    
    switch ( iPlayerType )
        {
        case EMdaAudioPlayerUtility:
            {
            iAudioPlayerUtility->Play();
            break;
            }
        case EMdaAudioToneUtility:
            {
            iAudioToneUtility->Play();
            break;
            }
        case EDrmPlayerUtility:
            {
            iDrmPlayerUtility->Play();
            break;
            }
        case EVideoPlayerUtility:
            {
            iVideoPlayerUtility->Play();
            break;
            }
        default:
            {
            break;
            }
        }
    }


// -----------------------------------------------------------------------------
// CRingingTone3DPlugin::StopRingingTone()
//
// -----------------------------------------------------------------------------
//    
void CRingingTone3DPlugin::StopRingingTone()
    {
    if ( !iPlaying )
        {
        return;
        }
    iPlaying = EFalse;
        
    switch ( iPlayerType )
        {
        case EMdaAudioPlayerUtility:
            {
            iAudioPlayerUtility->Stop();
            break;
            }
        case EMdaAudioToneUtility:
            {
            iAudioToneUtility->CancelPlay();
            break;
            }
        case EDrmPlayerUtility:
            {
            iDrmPlayerUtility->Stop();
            break;
            }
        case EVideoPlayerUtility:
            {
            iVideoPlayerUtility->Stop();
            break;
            }
        default:
            {
            Panic(KErrArgument);
            break;
            }
        }
        
    FLOG( _L("CRingingTone3DPlugin::StopRingingTone: ringing stopped") );
    }


// -----------------------------------------------------------------------------
// CRingingTone3DPlugin::Panic()
//
// -----------------------------------------------------------------------------
//    
void CRingingTone3DPlugin::Panic(TInt aReason)
    {
	_LIT(KPanicCategory,"RingingTone3DAudioPlugin");
	
	User::Panic(KPanicCategory, aReason); 
    }


// -----------------------------------------------------------------------------
// CRingingTone3DPlugin::ReadActiveProfileL()
// -----------------------------------------------------------------------------
//    
void CRingingTone3DPlugin::ReadActiveProfileL()
    {
    MProfileEngine* profileEngine = CreateProfileEngineL();
    CleanupReleasePushL( *profileEngine );
    
    MProfile* activeProfile  = profileEngine->ActiveProfileL();
    CleanupReleasePushL( *activeProfile );
        
#ifdef _DEBUG        
    FlogProfileL(activeProfile);
#endif        

    //const MProfileTones& profileTones = activeProfile->ProfileTones();
    //iRingingTone = profileTones.RingingTone1();
      
    const MProfileExtraSettings& extra = activeProfile->ProfileExtraSettings();
    const MProfile3DToneSettings& threeD = extra.Profile3DToneSettings();
    
    iProfile3DToneEffect = threeD.Effect();
    iProfile3DToneEcho = threeD.Echo();
   
    CleanupStack::PopAndDestroy(activeProfile); 
    CleanupStack::PopAndDestroy( profileEngine );
    }


// -----------------------------------------------------------------------------
// CRingingTone3DPlugin::CheckRingingTone()
// -----------------------------------------------------------------------------
//    
TBool CRingingTone3DPlugin::CheckRingingTone()
    {
    if ( iRingingTone.Length() == 0 )
        {
        return ETrue; // nothing to check
        }
        
    TParsePtr fileParser( iRingingTone );
    TPtrC extPtr = fileParser.Ext();
    
    // Ringing tone is valid for 3D-playback, i.e not a .rng-tone or "No ringing tone".
    // Or the user has set the RingingToneInUse flag to EFalse in CR.
    if ( extPtr.Compare( KRngType ) == 0 || iRingingTone.Compare( KNoToneFileName ) == 0 ) 
        {
        return EFalse;
        }
        
    return ETrue;
    }



#ifdef _DEBUG

// -----------------------------------------------------------------------------
// CRingingTone3DPlugin::Check3DPatternL()
//
// -----------------------------------------------------------------------------
//    
void CRingingTone3DPlugin::Check3DPatternL(TInt aPattern)
    {
    
    if ( aPattern != EProfile3DEffectOff && aPattern != EProfile3DEffectCircle &&
         aPattern != EProfile3DEffectFlyby && aPattern != EProfile3DEffectZigZag &&
         aPattern != EProfile3DEffectRandomWalk && aPattern != EProfile3DEffectChaos &&
         aPattern != EProfile3DEffectStereoWidening )
        {
        User::Leave(KErrArgument);
        }
    }

// -----------------------------------------------------------------------------
// CRingingTone3DPlugin::Check3DEchoL()
//
// -----------------------------------------------------------------------------
//    
void CRingingTone3DPlugin::Check3DEchoL(TInt aEcho)
    {
    
    if ( aEcho != EProfile3DEchoOff && aEcho != EProfile3DEchoLivingRoom &&
         aEcho != EProfile3DEchoCave && aEcho != EProfile3DEchoStoneCorridor &&
         aEcho != EProfile3DEchoForest && aEcho != EProfile3DEchoSewerPipe &&
         aEcho != EProfile3DEchoUnderwater )
        {
        User::Leave(KErrArgument);
        }
    }


// -----------------------------------------------------------------------------
// CRingingTone3DPlugin::FlogRepository()
//
// -----------------------------------------------------------------------------
//    
void CRingingTone3DPlugin::FlogRepositoryL(CRepository* aRepository, TInt aEcho)
    {
    FLOG( _L("------ start repository data ------") );
    
    TInt num = 0;
    TBuf<128> buf;
    
    User::LeaveIfError( aRepository->Get( E3DAudioRepositoryPattern, num ) );
    C3DAudioXMLParser::GetPatternString(buf, num);
    FLOG( _L("pattern: %S (%d)"), &buf, num );
    
    User::LeaveIfError( aRepository->Get( E3DAudioRepositoryReverb, num ) );
    C3DAudioXMLParser::GetEchoString(buf, num);
    
    C3DAudioXMLParser::GetEchoString(buf, aEcho);
    FLOG( _L("echo: %S (%d)"), &buf, aEcho );
 
    User::LeaveIfError( aRepository->Get( E3DAudioRepositoryVelocity, num ) );
    FLOG( _L("velocity: %d"), num );
    
    User::LeaveIfError( aRepository->Get( E3DAudioRepositoryDoppler, num ) );
    FLOG( _L("doppler: %d"), num );
    
    User::LeaveIfError( aRepository->Get( E3DAudioRepositoryBlockCount, num ) );
    FLOG( _L("block count: %d"), num );
    
    FLOG( _L("------ end repository data ------") );
    }


// -----------------------------------------------------------------------------
// CRingingTone3DPlugin::FlogProfile()
//
// -----------------------------------------------------------------------------
//    
void CRingingTone3DPlugin::FlogProfileL(MProfile* aProfile)
    {
    FLOG( _L("------ start profile data ------") );
    
    TInt num = 0;
    TBuf<128> buf;
    
    const MProfileName& name = aProfile->ProfileName();
    buf = name.Name();
    FLOG( _L("profile name: %S"), &buf );
    
    const MProfileTones& profileTones = aProfile->ProfileTones();
    TFileName ringingTone = profileTones.RingingTone1();
    FLOG( _L("ringing tone: %S"), &ringingTone );
  
    const MProfileExtraSettings& extra = aProfile->ProfileExtraSettings();
    const MProfile3DToneSettings& threeD = extra.Profile3DToneSettings();
    
    num = threeD.Effect();
    C3DAudioXMLParser::GetPatternString(buf, num);
    FLOG( _L("pattern: %S (%d)"), &buf, num );
    
    num = threeD.Echo();
    C3DAudioXMLParser::GetEchoString(buf, num);
    FLOG( _L("echo: %S (%d)"), &buf, num );

    
    FLOG( _L("------ end profile data ------") );
    }


// -----------------------------------------------------------------------------
// CRingingTone3DPlugin::FlogPlayer()
//
// -----------------------------------------------------------------------------
//    
void CRingingTone3DPlugin::FlogPlayer(TInt aPlayer)
    {
    TBuf<128> buf;
    
  	switch ( aPlayer )
		{
		case EMdaAudioPlayerUtility:
			buf = _L("EMdaAudioPlayerUtility");
			break;
		case EMdaAudioToneUtility:
			buf = _L("EMdaAudioToneUtility");
			break;
		case EDrmPlayerUtility:
			buf = _L("EDrmPlayerUtility");
			break;
		case EVideoPlayerUtility:
			buf = _L("EVideoPlayerUtility");
			break;
			
		default:
			buf = _L("### unknown player ###");
			break;
		}

    
    FLOG( _L("player type: %S"), &buf );
    }


// -----------------------------------------------------------------------------
// CRingingTone3DPlugin::FlogPositionL()
//
// -----------------------------------------------------------------------------
//    
void CRingingTone3DPlugin::FlogPositionL(T3DPosition& aPosition, TInt aIndex)
    {
    TBuf<128> buf;
    TInt x = aPosition.iX;
    TInt y = aPosition.iY;
    TInt z = aPosition.iZ;
    
    buf.Format(_L("%d:  position x=%d y=%d z=%d"), aIndex, x,y,z);
    FLOG(buf);
    }


// -----------------------------------------------------------------------------
// CRingingTone3DPlugin::FlogVelocityL()
//
// -----------------------------------------------------------------------------
//    
void CRingingTone3DPlugin::FlogVelocityL(T3DVelocity& aVelocity, TInt aIndex)
    {
    TBuf<128> buf;
    
    TInt x = aVelocity.iXVector;
    TInt y = aVelocity.iYVector;
    TInt z = aVelocity.iZVector;
    
    buf.Format(_L("%d:  velocity x=%d y=%d z=%d"), aIndex, x,y,z);
    FLOG(buf);
    }

// -----------------------------------------------------------------------------
// CRingingTone3DPlugin::FlogTimeL()
//
// -----------------------------------------------------------------------------
//    
void CRingingTone3DPlugin::FlogTimeL(TInt aTime, TInt aIndex)
    {
    TBuf<128> buf;
  
    buf.Format(_L("%d:  time = %d"), aIndex, aTime);
    FLOG(buf);
    }

#endif   // _DEBUG






//  End of File  
