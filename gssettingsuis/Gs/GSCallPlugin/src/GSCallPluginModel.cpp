/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies).
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
*        Model/Engine for Call Settings Plugin
*
*/


// INCLUDE FILES
#include "GSCallPluginModel.h"
#include "GsLogger.h"      //for logging traces
#include "SettingsPrivateCRKeys.h"

#include <barsc.h>
#include <barsread.h>
#include <coecntrl.h>
#include <f32file.h>
#include <AknQueryDialog.h>
#include <s32file.h>
#include <featmgr.h>
#include <PsetCSP.h>
#include <generalsettingsvariant.hrh>
#include <PSVariables.h>  //PubSub 
#include <settingsinternalcrkeys.h>
#include <LogsDomainCRKeys.h>

#if defined(__VOIP) && defined(RD_VOIP_REL_2_2)
#include <spsettings.h>
#endif // __VOIP && RD_VOIP_REL_2_2
//CONSTANTS
// default value for autolock period
// default value for Sat operations

_LIT( KGSNameOfClass, "CGSCallPluginModel" );

// Max int size

//for switching values in SwitchValue()
const TInt KGSSettingOff = 0;
const TInt KGSSettingOn = 1;
const TInt KGSIndexOff = 1;
const TInt KGSIndexOn = 0;

// ================= MEMBER FUNCTIONS =======================

// ----------------------------------------------------------------------------
// CGSCallPluginModel::NewL
// 
// Symbian OS two-phased constructor
// ----------------------------------------------------------------------------
//
CGSCallPluginModel* CGSCallPluginModel::NewL()
    {
    CGSCallPluginModel* self = new( ELeave ) CGSCallPluginModel;
    CleanupStack::PushL( self );
    self->ConstructL();

    CleanupStack::Pop( self );
    return self;
    }


// ----------------------------------------------------------------------------
// CGSCallPluginModel::CGSCallPluginModel
// 
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CGSCallPluginModel::CGSCallPluginModel()
    {
    }


// ----------------------------------------------------------------------------
// CGSCallPluginModel::ConstructL
// 
// EPOC default constructor can leave.
// ----------------------------------------------------------------------------
//
void CGSCallPluginModel::ConstructL()
    {
    FeatureManager::InitializeLibL();

    InitializeCentralRepositoryL();

    iBackgroundApi = CGSBackgroundImage::NewL();

    SetCSPActiveL( ETrue );

    User::LeaveIfError( iGSVariationRepository->Get( 
                        KSettingsVariationFlags, iLocalVariationValues ) );
#if defined(__VOIP) && defined(RD_VOIP_REL_2_2)
	iSpSettings = CSPSettings::NewL();  
#endif // __VOIP && RD_VOIP_REL_2_2
    }


// ----------------------------------------------------------------------------
// CGSCallPluginModel::~CGSCallPluginModel
// 
// Destructor
// ----------------------------------------------------------------------------
//
CGSCallPluginModel::~CGSCallPluginModel()
    {
    FeatureManager::UnInitializeLib();
	UninitializeCentralRepository();
    
    if ( GetCSPStatus() )
        {
        TRAP_IGNORE( SetCSPActiveL( EFalse ) );
        }
    
    if ( iBackgroundApi )
        {
        delete iBackgroundApi;
        }
    
#if defined(__VOIP) && defined(RD_VOIP_REL_2_2)
    delete iSpSettings;
#endif // __VOIP && RD_VOIP_REL_2_2
    }

// ----------------------------------------------------------------------------
// CGSCallPluginModel::InitializeCentralRepositoryL
//
// Creating and setting keys for the Central Repository
// ----------------------------------------------------------------------------
//
void CGSCallPluginModel::InitializeCentralRepositoryL()
    {
    iTelephonyRepository = CRepository::NewL( KCRUidTelephonySettings );
    iCommonTelephonyRepository = 
            CRepository::NewL( KCRUidCommonTelephonySettings );
    iGSVariationRepository = CRepository::NewL( KCRUidSettingsVariation );
    iRichCallRepository = CRepository::NewL( KCRUidRichCallSettings );
    iLogsRepository = CRepository::NewL( KCRUidLogs );
    }

// ----------------------------------------------------------------------------
// CGSCallPluginModel::UninitializeCentralRepositoryL
//
// Removes Central Repository objects
// ----------------------------------------------------------------------------
//
void CGSCallPluginModel::UninitializeCentralRepository()
    {
    if ( iTelephonyRepository )
        {
        delete iTelephonyRepository;
        iTelephonyRepository = NULL;
        }
    if ( iCommonTelephonyRepository )
        {
        delete iCommonTelephonyRepository;
        iCommonTelephonyRepository = NULL;
        }
    if ( iGSVariationRepository )
        {
        delete iGSVariationRepository;
        iGSVariationRepository = NULL;
        }
    if ( iRichCallRepository )
        {
        delete iRichCallRepository;
        iRichCallRepository = NULL;
        }
    if ( iLogsRepository )
        {
        delete iLogsRepository;
        iLogsRepository = NULL;
        }
    }


// ----------------------------------------------------------------------------
// CGSCallPluginModel::GetCallSettingValue
// Gets one of the call related values (anykey answer, automatic redial,
// summary after call or one key dialing) from Central Repository (Working OK)
// ----------------------------------------------------------------------------
//
TInt CGSCallPluginModel::GetCallSettingValue( const TCallSettId aCallId,
                                            TInt& aId )
    {
    TInt ret = KErrNone;
    if ( aCallId == EGSSummaryAfterCall )
        {
        ret = 
            iCommonTelephonyRepository->Get( KSettingsSummaryAfterCall, aId );
        }
    else
        {        
        switch ( aCallId )
            {     
            case EGSOpeningSlideAnswerCall:
                ret = iTelephonyRepository->
                    Get( KSettingsOpeningSlideAnswer, aId );
                break;
            case EGSClosingSlideEndCall:
                ret = iTelephonyRepository->
                    Get( KSettingsClosingSlideEnd, aId );
                break;            
            case EGSSoftRejectDefault:
                ret = iTelephonyRepository->
                   Get( KSettingsSoftRejectDefaultInUse, aId );
                break;
            default:
                break;
            };
        }
    return ret;
    }

// ----------------------------------------------------------------------------
// CGSCallPluginModel::SetCallSettingValue
// Sets one of the call related values (anykey answer, automatic redial,
// summary after call or one key dialing) from Central Repository (working OK)
// ----------------------------------------------------------------------------
//
TInt CGSCallPluginModel::SetCallSettingValue( const TCallSettId aCallId,
                                            TInt aId )
    {
    TInt ret = KErrNone;
    if ( aCallId == EGSSummaryAfterCall )
        {
        ret = 
            iCommonTelephonyRepository->Set( KSettingsSummaryAfterCall, aId );
        }
    else
        {
        switch ( aCallId )
            {
            case EGSOpeningSlideAnswerCall:
                ret = iTelephonyRepository->
                    Set( KSettingsOpeningSlideAnswer, aId );
                break;
            case EGSClosingSlideEndCall:
                ret = iTelephonyRepository->
                    Set( KSettingsClosingSlideEnd, aId );
                break;    
            case EGSSoftRejectDefault:
                ret = iTelephonyRepository->
                    Set( KSettingsSoftRejectDefaultInUse, aId );
                break;
            default:
                break;
            };
        }
        
    return ret;
    }


// ----------------------------------------------------------------------------
// CGSCallPluginModel::GetCSPStatus
// 
// Get Customer Service Profile status
// ----------------------------------------------------------------------------
//
TBool CGSCallPluginModel::GetCSPStatus()
    {
    if ( iCSP )
        {
        return ETrue;
        }
    else
        {
        return EFalse;
        }    
    }

// ----------------------------------------------------------------------------
// CGSCallPluginModel::SetCSPActiveL
// 
// Set Customer Service Profile active
// ----------------------------------------------------------------------------
//
void CGSCallPluginModel::SetCSPActiveL( TBool aValue )
    {
    if ( aValue )
        {
        iCSP = CPsetCustomerServiceProfile::NewL();
        User::LeaveIfError( iCSP->OpenCSProfileL() );
        }
    else
        {        
        delete iCSP;
        iCSP = NULL;
        }
    }

// ----------------------------------------------------------------------------
// CGSCallPluginModel::IsSettingSupported
// 
// Check if a given setting is supported
// ----------------------------------------------------------------------------
//
TBool CGSCallPluginModel::IsSettingSupported( TInt aSettingNumber )
    {
    __ASSERT_ALWAYS( iCSP != NULL, User::Panic( KGSNameOfClass, 
                                   EGSCallPluinModelPanicNullPtr ) );
    TBool settingSupported = EFalse;
    TInt retVal = KErrNone;

    switch ( aSettingNumber )
        {
        case EGSCSPCallWaiting:
            retVal = iCSP->IsCWSupported( settingSupported );
            break;
        case EGSCSPAlternateLine:
            retVal = iCSP->IsALSSupported( settingSupported );
            break;
        default:
            break;
        }
    if ( retVal != KErrNone )
        {
        //if a CSP error occurs, by default service is available
        settingSupported = ETrue;
        }

    return settingSupported;
    }


// ----------------------------------------------------------------------------
// CGSCallPluginModel::GetSoftRejectText
// Gets soft reject text from shared data.
// ----------------------------------------------------------------------------
//
TBool CGSCallPluginModel::GetSoftRejectText( TDes& aText )
    {
    TInt ret = iTelephonyRepository->Get( KSettingsSoftRejectText, aText );
    return ret;
    }

// ----------------------------------------------------------------------------
// CGSCallPluginModel::SetSoftRejectText
// Sets user defined text to shared data for soft reject.
// ----------------------------------------------------------------------------
//
TBool CGSCallPluginModel::SetSoftRejectText( TDes& aText )
    {
    TInt ret = iTelephonyRepository->Set( KSettingsSoftRejectText, aText );
    return ret;
    }


// ----------------------------------------------------------------------------
// CGSCallPluginModel::VTStillImageL
// 
// VT Still Image mode.
// ----------------------------------------------------------------------------
//
TInt CGSCallPluginModel::VTStillImageL()
    {
    TInt StImg = KGSSettingOff;
    
    User::LeaveIfError( iTelephonyRepository->Get( KSettingsVTStillImage, 
                                                   StImg ) );
    return StImg;
    }

// ----------------------------------------------------------------------------
// CGSCallPluginModel::SetVTStillImageL
// 
// Sets VT Still Image mode to Shared Data
// ----------------------------------------------------------------------------
//
void CGSCallPluginModel::SetVTStillImageL( const TInt aValue )
    {
    User::LeaveIfError( iTelephonyRepository->Set( KSettingsVTStillImage, 
                                                   aValue ) );
    }


// ----------------------------------------------------------------------------
// CGSCallPluginModel::PrefixChangeModeL
// 
// Get the Japanese prefix change mode value from shared data.
// ----------------------------------------------------------------------------
//
TInt CGSCallPluginModel::PrefixChangeModeL()
    {
    TInt mode = KGSSettingOff;
    User::LeaveIfError( iTelephonyRepository->
                Get( KSettingsDialPrefixChangeMode, mode ) );
    SwitchValue( mode );
    return mode;
    }


// ----------------------------------------------------------------------------
// CGSCallPluginModel::SetPrefixChangeModeL
// 
// Set the Japanese prefix change mode value to shared data. CenRep OK
// ----------------------------------------------------------------------------
//
void CGSCallPluginModel::SetPrefixChangeModeL( TInt aMode )
    {
    SwitchValue( aMode );
    User::LeaveIfError( iTelephonyRepository->
                Set( KSettingsDialPrefixChangeMode, aMode ) );
    }


// ----------------------------------------------------------------------------
// CGSCallPluginModel::PrefixChangeDataL
// 
// Reads prefix change text from shared data and returns it
// ----------------------------------------------------------------------------
//
void CGSCallPluginModel::PrefixChangeDataL( TDes& aText )
    {
    User::LeaveIfError( iTelephonyRepository->
                Get( KSettingsDialPrefixText, aText ) );
    }

// ----------------------------------------------------------------------------
// CGSCallPluginModel::SetPrefixChangeDataL
// 
// Writes prefix change text to shared data
// ----------------------------------------------------------------------------
//
void CGSCallPluginModel::SetPrefixChangeDataL( const TDesC& aText )
    {
    User::LeaveIfError( iTelephonyRepository->
                Set( KSettingsDialPrefixText, aText ) );
    }


// ----------------------------------------------------------------------------
// CGSCallPluginModel::SwitchValue
// 
// Switching value from 1 to 0 and back
// ----------------------------------------------------------------------------
//
void CGSCallPluginModel::SwitchValue( TInt& aValue )
    {
    switch( aValue )   
        {
        case KGSSettingOff: //0
            aValue = KGSIndexOff; //1
            break;
        case KGSSettingOn: //1
            aValue = KGSIndexOn; //0
            break;
        default:
            aValue = KErrNotFound;
        };
    }


// ---------------------------------------------------------
// CGSCallPluginModel::RestrictedSendCallerIdSupportedL
// 
// Check if Restricted Send Caller ID is supported.
// ---------------------------------------------------------
//
TBool CGSCallPluginModel::RestrictedSendCallerIdSupportedL()
    {
    TBool supported = EFalse;    

    if ( iLocalVariationValues & EGSConfigRestrictedSendCallerId )
        {
        supported = ETrue;
        }

    return supported;

    }

// ---------------------------------------------------------
// CGSCallPluginModel::SecureSendCallerIdSupportedL
// 
// Check if Secure Send Caller ID is supported.
// ---------------------------------------------------------
//
TBool CGSCallPluginModel::SecureSendCallerIdSupportedL()
    {
    TBool supported = EFalse;    

    if ( iLocalVariationValues & EGSConfigSecureSendCallerId )
        {
        supported = ETrue;
        }

    return supported;

    }

// ----------------------------------------------------------------------------
// CGSCallPluginModel::GetVoIPCallSettingValue
// Gets one of the VoIP call related values (send my Int. call id, 
// Internet call waiting, preferred call type, do not disturb or 
// Internet call barring) from Central Repository
// ----------------------------------------------------------------------------
//
TInt CGSCallPluginModel::GetVoIPCallSettingValue( 
    const TGSVoIPSetting aVoipSettingId, TInt& aVoipId ) 
    {
    TInt ret( KErrNone );
   
    switch ( aVoipSettingId )
        {
        case EGSVoIPSendIntCallId:
            ret = iRichCallRepository->Get( KRCSEClir, aVoipId );
            break;
        case EGSVoIPCW:
            ret = iRichCallRepository->Get( KRCSPSCallWaiting, aVoipId );
            break;
        case EGSVoIPPreType:
            ret = iRichCallRepository->Get( KRCSEPreferredTelephony, aVoipId );
            break;
        case EGSVoIPDnd:
            ret = iRichCallRepository->Get( KRCSEDoNotDisturb, aVoipId );
            break;
        case EGSVoIPBarring:
            ret = iRichCallRepository->Get( KRCSEAnonymousCallBlockRule, aVoipId );
            break;
        default:
            break;
        }
    
    return ret;
    }

// ----------------------------------------------------------------------------
// CGSCallPluginModel::SetVoIPCallSettingValue
// Sets one of the VoIP call related values (send my Int. call id, 
// Internet call waiting, preferred call type, do not disturb or 
// Internet call barring) from Central Repository
// ----------------------------------------------------------------------------
//
TInt CGSCallPluginModel::SetVoIPCallSettingValue( 
    const TGSVoIPSetting aVoipSettingId, TInt aVoipId )
    {
    TInt ret( KErrNone );
    
    switch( aVoipSettingId )
        {
        case EGSVoIPSendIntCallId:
            ret = iRichCallRepository->Set( KRCSEClir, aVoipId );
            break;
        case EGSVoIPCW:
            ret = iRichCallRepository->Set( KRCSPSCallWaiting, aVoipId );
            break;
        case EGSVoIPPreType:
            ret = iRichCallRepository->Set( KRCSEPreferredTelephony, aVoipId );
            break;
        case EGSVoIPDnd:
            ret = iRichCallRepository->Set( KRCSEDoNotDisturb, aVoipId );
            break;
        case EGSVoIPBarring:
            ret = iRichCallRepository->Set( KRCSEAnonymousCallBlockRule, aVoipId );
            break;
        default:
            break;
        }
        
    return ret;    
    }
    
// ----------------------------------------------------------------------------
// CGSCallPluginModel::GetSCCPStatus
// Gets SCCP status from Central Repository
// ----------------------------------------------------------------------------
//
TInt CGSCallPluginModel::GetSCCPStatus( TInt& aStatus )
    {
    TInt ret( KErrNone );
    ret = iTelephonyRepository->Get( KSCCPinstallUpgrade, aStatus );
    return ret;
    }


// ----------------------------------------------------------------------------
// CGSCallPluginModel::CallDurationL
// 
// Get the Call duration setting value
// ----------------------------------------------------------------------------
//
TInt CGSCallPluginModel::CallDurationL()
    {
    TInt value = KGSSettingOff;
    User::LeaveIfError( iLogsRepository->
                Get( KLogsShowCallDuration, value ) );
    
	SwitchValue ( value );

    return value;
    }


// ----------------------------------------------------------------------------
// CGSCallPluginModel::SetCallDurationL
// 
// Set the Call duration setting value
// ----------------------------------------------------------------------------
//
void CGSCallPluginModel::SetCallDurationL( TInt aValue )
    {
    SwitchValue ( aValue );
    User::LeaveIfError( iLogsRepository->
                Set( KLogsShowCallDuration, aValue ) );
    }

// ---------------------------------------------------------
// CGSTelPluginModel::VoIPSupported
// 
// Check if VoIP is supported.
// ---------------------------------------------------------
//    
TBool CGSCallPluginModel::VoIPSupported()
    {
    TInt supported( KGSSettingOff );
#if defined(__VOIP) && defined(RD_VOIP_REL_2_2)
    if ( iSpSettings->IsFeatureSupported( ESupportVoIPFeature ) &&
        iSpSettings->IsFeatureSupported( ESupportVoIPSSFeature ) )
        {
        supported = KGSSettingOn;
        }
#else // __VOIP && RD_VOIP_REL_2_2
    if ( FeatureManager::FeatureSupported( KFeatureIdCommonVoip ) )
        {
        iTelephonyRepository->Get( KDynamicVoIP, supported );
        }
#endif // __VOIP && RD_VOIP_REL_2_2
    return KGSSettingOff != supported;
    }

// ----------------------------------------------------------------------------
// CGSCallPluginModel::LongPressCallKeyL
// 
// Get the Long Press Call Key setting value
// 0,1 --> Inactive (Default value is 1 and when RFS Default value is 0) 
// 2 --> Video call
// ----------------------------------------------------------------------------
//
TInt CGSCallPluginModel::LongPressCallKeyL()
    {
    TInt value = EGSLongPressCallKeyVoiceCall;
  #ifdef RD_VT_LONG_SEND_KEY
    User::LeaveIfError( iTelephonyRepository->
                Get( KSettingsUiLongCallKeyPress, value ) );
  #endif
    return value;
    }

// ----------------------------------------------------------------------------
// CGSCallPluginModel::SetLongPressCallKeyL
// 
// Set the Long Press Call Key setting value
// ----------------------------------------------------------------------------
//
void CGSCallPluginModel::SetLongPressCallKeyL( const TInt aValue )
    {
#ifdef RD_VT_LONG_SEND_KEY
    User::LeaveIfError( iTelephonyRepository->
                Set( KSettingsUiLongCallKeyPress, aValue ) );
#endif
    }

// ----------------------------------------------------------------------------
// CGSCallPluginModel::MapLongPressKeyCallKeyValue
// 
// Maps the Long Press Call Key setting value
// NOTE : EGSLongPressCallKeyVoiceCall is the default value normally
// NOTE: When Restore Factory Settings is done, the default value 
//       is set to EGSLongPressCallKeyInActive
// Three key values are used for a CenRep key though we have two
// two setting items because these values are also been used by
// CallUi and Phone Application.
// ----------------------------------------------------------------------------
//
void CGSCallPluginModel::MapLongPressKeyCallKeyValue( TInt &aValue )
    {
#ifdef RD_VT_LONG_SEND_KEY
  	switch ( aValue )
  		{
  		case EGSLongPressCallKeyVoiceCall:
  			 aValue = EGSLongPressCallKeyNotSet;
  			 break;
  		case EGSLongPressCallKeyVideoCall:
  		     aValue = EGSLongPressCallKeyVoiceCall;
  			 break;
  		default:
  			 break;
  		}
#endif
    }


// ----------------------------------------------------------------------------
// CGSCallPluginModel::OwnImageVtCallStatusL
// 
// Get the Own Image sending during vt call setting value
// 0,1 --> Inactive (Default value is 1 and when RFS Default value is 0) 
// 2 --> Video call
// ----------------------------------------------------------------------------
//
TInt CGSCallPluginModel::OwnImageVtCallStatusL()
    {
    TInt value;
    User::LeaveIfError( iTelephonyRepository->
                Get( KSettingsVTVideoSending, value ) );
    return value;
    }

// ----------------------------------------------------------------------------
// CGSCallPluginModel::SetOwnImageVtCallStatusL
// 
// Set the Long Press Call Key setting value
// ----------------------------------------------------------------------------
//
void CGSCallPluginModel::SetOwnImageVtCallStatusL( const TInt aValue )
    {
    User::LeaveIfError( iTelephonyRepository->
                Set( KSettingsVTVideoSending, aValue ) );
    }


// ----------------------------------------------------------------------------
// CGSCallPluginModel::SlideSettingsShown
// 
// Check if slide settings items should be shown
// ----------------------------------------------------------------------------
//
TBool CGSCallPluginModel::SlideSettingsShownL() const
    {
    TInt value( EGSNotShowSlideSettings );
    
    User::LeaveIfError( 
            iTelephonyRepository->Get( KSettingsShowSlideSettings,value ) );

    if ( EGSShowSlideSettings == value )
        {
        return ETrue;
        }
    else
        {
        return EFalse;
        }
    }

//  End of File
