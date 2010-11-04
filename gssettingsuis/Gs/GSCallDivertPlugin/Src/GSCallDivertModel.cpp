/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Model for call divert functionality. Tries to encapsulate
*                 most of hw/engine related functionality from UI components.
*
*/




// User includes:
#include "featmgr.h"
#include "GSCallDivertModel.h"
#include "GSCallDivertView.h"
#include "GsLogger.h"
#include "GSPhoneSettingConstants.h"


// System includes:
#include <CPbkContactEngine.h>
#include <CPhCltUssd.h>
#include <MPsetDivertObs.h>
#include <PhCltTypes.h>
#include <PsetContainer.h>
#include <PsuiContainer.h>
#include <RPhCltServer.h>
#include <RSSSettings.h>
#include <centralrepository.h>
#include <NumberGroupingCRKeys.h>
#include <NumberGrouping.h>

// Constants for restricted call divert
const TInt KMaxLengthSendString = 40;
_LIT( KOne, "1" );
_LIT( KAsterisk, "*" );
_LIT( KHashMark, "#" );
_LIT( KOneHundredTwenty, "120" );

// Class neme descriptor for panic descriptions
_LIT( KGSCallDivertModel, "CGSCallDivertModel" );


// ---------------------------------------------------------------------------
// CGSCallDivertModel::CGSCallDivertModel
// ---------------------------------------------------------------------------
CGSCallDivertModel::CGSCallDivertModel()
{
}

// ---------------------------------------------------------------------------
// CGSCallDivertModel::NewL
// ---------------------------------------------------------------------------
CGSCallDivertModel* CGSCallDivertModel::NewL()
{
    CGSCallDivertModel* self = new( ELeave ) CGSCallDivertModel;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
}

// ---------------------------------------------------------------------------
// CGSCallDivertModel::ConstructL
// ---------------------------------------------------------------------------
void CGSCallDivertModel::ConstructL()
{
    iContactEngine = CPbkContactEngine::NewL();
    if( FeatureManager::FeatureSupported( KFeatureIdRestrictedCallDivert ) )
        {
        iBasicClient = CPhCltUssd::NewL();
        }
    iSettings = CPsetContainer::NewL();
    iObsContainer = CPsuiContainer::NewL();
    iCfObserver = iObsContainer->CreateCFObsL();
    iForward = iSettings->CreateCFObjectL( *iCfObserver );
    iSsSettings = new (ELeave) RSSSettings;
    User::LeaveIfError( iSsSettings->Open() );
}

// ---------------------------------------------------------------------------
// CGSCallDivertModel::~CGSCallDivertModel
// ---------------------------------------------------------------------------
CGSCallDivertModel::~CGSCallDivertModel()
{
    // These objects have to be deleted in specific order or otherwise a access
    // violation will follow. So, if changing order, be very careful and test
    // thoroughly.
    delete iContactEngine;
    delete iObsContainer;
    delete iCfObserver;
    delete iForward;
    delete iSettings;
    if (FeatureManager::FeatureSupported ( KFeatureIdRestrictedCallDivert ) )
        {
        delete iBasicClient;
        }
    if( iSsSettings )
        {
        iSsSettings->Close ( );
        delete iSsSettings;
        }
    if( iNumberGroupingRepository )
        {
        delete iNumberGroupingRepository;
        iNumberGroupingRepository = NULL;
        }
}

// ---------------------------------------------------------------------------
// CGSCallDivertModel::SendCallDivertRequestL
// ---------------------------------------------------------------------------
void CGSCallDivertModel::SendCallDivertRequestL(
    TCallDivertSetting& aCallDivertSetting,
    RMobilePhone::TMobilePhoneServiceAction aInfo,
    TBool& aVmbxDivert,
    TInt aCommand,
    TGSCallDivertServices aDivertService )
    {
    // Set Delay time if necessary.
    TInt retVal = KCFNoNeedToGetTimeValue;

    //User is doing activation
    if ( aCallDivertSetting.iSetting == EActivateDiverting )
        {
        if ( aCallDivertSetting.iNumber == KNullDesC )
            {
            return; //User started to activate, but cancelled.
            }
        //Delay time needs to be set for no answer, not available.
        if ( aDivertService == ECFDivertWhenNoAnswer ||
             aDivertService ==  ECFDivertWhenNotAvailable )
            {
            retVal = iDivertView->CreateTimeListL();
            }
        }
    if( FeatureManager::FeatureSupported( KFeatureIdRestrictedCallDivert ) )
        {
        // Always ask for duration if it's restricted call divert
        retVal = iDivertView->CreateTimeListL();
        }
    //Call Phonesettings if all values have been set.
    if ( retVal )
        {
        if( FeatureManager::FeatureSupported( KFeatureIdRestrictedCallDivert ) )
            {
            __GSLOGSTRING(  "[CGSCallDivertModel::SendCallDivertRequestL] -- Sending restricted call divert request: --" );
            __GSLOGSTRING1( "[CGSCallDivertModel::SendCallDivertRequestL] aCallDivertSetting.iNumber:%S",   &aCallDivertSetting.iNumber );
            __GSLOGSTRING1( "[CGSCallDivertModel::SendCallDivertRequestL] iDivertServiceGroup:  %d",        iDivertServiceGroup );

            ActivateRestrictedCallDivertL( aCallDivertSetting.iNumber );
            }
        else
            {
            __GSLOGSTRING(  "[CGSCallDivertModel::SendCallDivertRequestL] -- Sending call divert request: --" );
            __GSLOGSTRING1( "[CGSCallDivertModel::SendCallDivertRequestL] aCallDivertSetting.iNumber:%S",   &aCallDivertSetting.iNumber );
            __GSLOGSTRING1( "[CGSCallDivertModel::SendCallDivertRequestL] aInfo:                %d",        aInfo );
            __GSLOGSTRING1( "[CGSCallDivertModel::SendCallDivertRequestL] aVmbxDivert:          %d",        aVmbxDivert );
            __GSLOGSTRING1( "[CGSCallDivertModel::SendCallDivertRequestL] aCommand:             %d",        aCommand );
            __GSLOGSTRING1( "[CGSCallDivertModel::SendCallDivertRequestL] aDivertService:       %d",        aDivertService );
            __GSLOGSTRING1( "[CGSCallDivertModel::SendCallDivertRequestL] iDivertServiceGroup:  %d",        iDivertServiceGroup );

            // Make a call to PhoneSettings.
            EngineOperatorL( aCallDivertSetting, aInfo, aVmbxDivert, aCommand, aDivertService, iDivertServiceGroup );
            }
        }
    }


// ---------------------------------------------------------------------------
// CGSCallDivertModel::EngineOperatorL
//
// Seems to be a blocking function...
// Handles the engine operating, arguments tell what to call and
// the number if needed.
// ---------------------------------------------------------------------------
void CGSCallDivertModel::EngineOperatorL(
    TCallDivertSetting& aDivert,
    const RMobilePhone::TMobilePhoneServiceAction& aInfo,
    TBool& aVmbxValidNumber,
    TInt aCommand,
    TGSCallDivertServices aDivertService,
    TGSCallDivertServiceGroups aDivertServiceGroup )
    {
    __GSLOGSTRING("[CGSCallDivertModel::EngineOperatorL] Start...");

    TBasicServiceGroups bsc = EAllTeleAndBearer;
    aDivert.iNoReplyTimer = 0;

    if ( aCommand == ECFCmdCancelAll )
        {
        aDivert.iCondition = EDivertConditionAllCalls;
        aDivert.iSetting = EEraseDiverting;
        aDivert.iStatus = EDivertingStatusUnknown;
        aDivert.iServiceGroup = EServiceGroupAllTeleservices;
        aDivert.iNumber.Zero();
        iForward->SetDivertingL( aDivert, bsc );

        __GSLOGSTRING("[CGSCallDivertModel::EngineOperatorL] ECFCmdCancelAll");
        return;
        }

    SetDivertParametersL( aDivert, bsc, aDivertService, aDivertServiceGroup );
    __GSLOGSTRING1("[CGSCallDivertModel::EngineOperatorL] ServiceGroup: %d", bsc);

    switch ( aInfo )
        {
        case RMobilePhone::EServiceActionUnspecified:
            iForward->GetDivertingStatusL( aDivert.iServiceGroup,
                                           aDivert.iCondition,
                                           bsc );
            break;
        case RMobilePhone::EServiceActionActivate:
            aDivert.iSetting = EActivateDiverting;
            iForward->SetDivertingL( aDivert, bsc, aVmbxValidNumber );
            break;
        case RMobilePhone::EServiceActionErase:
            aDivert.iSetting = EEraseDiverting;
            iForward->SetDivertingL( aDivert, bsc );
            break;
        case RMobilePhone::EServiceActionDeactivate:
            aDivert.iSetting = ECancelDiverting;
            iForward->SetDivertingL( aDivert, bsc );
            break;
        case RMobilePhone::EServiceActionRegister:
            aDivert.iSetting = ERegisterDiverting;
            iForward->SetDivertingL( aDivert, bsc, aVmbxValidNumber );
            break;
        default:
            Panic ( KGSCallDivertModel, EInvalidDivertAction );
            break;
        }
    __GSLOGSTRING("[CGSCallDivertModel::EngineOperatorL] End.");
    }

// ---------------------------------------------------------------------------
// CGSCallDivertModel::VoiceMailActivationL
//
// Handles activation attempts to voicemail box.
// ---------------------------------------------------------------------------
TInt CGSCallDivertModel::VoiceMailActivationL( TDes& aTelNumber )
    {
    __GSLOGSTRING1("[CGSCallDivertModel::VoiceMailActivationL] aTelNumber: %S", &aTelNumber);
    TRAPD( err, iForward->VoiceMailQueryL( aTelNumber ) );
    if ( err )
        {
    	__GSLOGSTRING1("[CGSCallDivertModel::VoiceMailActivationL] err=%d", err);
    	// If application is killed via FSW, contact selection query
    	// returns KLeaveExit. This should propagate to "root"
    	// application where it reaches the active scheduler loop 
    	// and thus allows proper application exit when the loop exits.
    	// Therefore propagate the error to caller and do not ignore it.
    	if ( KLeaveExit == err )
    		{
    		__GSLOGSTRING("[CGSCallDivertModel::VoiceMailActivationL] KLeaveExit");
    		User::Leave( err );
    		}
        return KErrCancel;
        }
    //Vmbx number can be "invalid" from CF point of view, because they are allowed to
    //contain special characters. These must be removed before making a divert to the
    //vmbx number.
    //E.g. number +35850888#45 becomes +35850888 (end is dropped).
    TInt noOfInvalidChars = KCFInvalidChars().Length();

    for ( TInt index = 0; index < noOfInvalidChars; index++ )
        {
        RemoveSpecialChar( aTelNumber, KCFInvalidChars().Mid( index, 1 ) );
        }
    __GSLOGSTRING1("[CGSCallDivertModel::VoiceMailActivationL] aTelNumber: %S", &aTelNumber);
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CGSCallDivertModel::VideoMailActivationL
//
// Handles activation attempts to videomail box.
// ---------------------------------------------------------------------------
TInt CGSCallDivertModel::VideoMailActivationL( TDes& aTelNumber )
    {
    __GSLOGSTRING1("[CGSCallDivertModel::VideoMailActivationL] aTelNumber: %S", &aTelNumber);
    // Trapped to return KErrCancel in all cases in order to provide same
    // behaviour as in VoiceMailActivationL method (Both methods can be used
    // in the same manner)
    TRAPD( err, iForward->VideoMailQueryL( aTelNumber ) );
    if ( err )
        {
        return KErrCancel;
        }
    //Vmbx number can be "invalid" from CF point of view, because they are allowed to
    //contain special characters. These must be removed before making a divert to the
    //vmbx number.
    //E.g. number +35850888#45 becomes +35850888 (end is dropped).
    TInt noOfInvalidChars = KCFInvalidChars().Length();
    TPtrC invalidChars( KCFInvalidChars );
    for ( TInt index = 0; index < noOfInvalidChars; index++ )
        {
        RemoveSpecialChar( aTelNumber, TPtrC( &invalidChars[ index ] ) );
        }
    __GSLOGSTRING1("[CGSCallDivertModel::VideoMailActivationL] aTelNumber: %S", &aTelNumber);
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CGSCallDivertModel::ActivateRestrictedCallDivertL
//
// Handles attempts to activate call divert in restricted mode
// ---------------------------------------------------------------------------
void CGSCallDivertModel::ActivateRestrictedCallDivertL(
    TTelNumber& aDivertNumber )
    {
    if( FeatureManager::FeatureSupported( KFeatureIdRestrictedCallDivert ) )
        {
        //Send Number and ringing time to network by using USSD.
        HBufC* sendString = HBufC::NewLC( KMaxLengthSendString );
        TPtr ptr = sendString->Des();

        //Start call forwarding has following syntax:
        //*120*SI1*SI2*SI3 # SEND(Start & Set CF number & Set the time (sec.))
        //where: S1 = 1
        //SI2:(CF number)
        //SI3: 0 - 120(ringing time (sec.))
        ptr.Append( KAsterisk );
        ptr.Append( KOneHundredTwenty );
        ptr.Append( KAsterisk );
        ptr.Append( KOne );
        ptr.Append( KAsterisk );
        ptr.Append( aDivertNumber );
        ptr.Append( KAsterisk );
        ptr.AppendNum( iForward->GetTimerValueL() );
        ptr.Append( KHashMark );

        iBasicClient->SendUssd( *sendString );
        CleanupStack::PopAndDestroy( sendString );
        }
    }


// ---------------------------------------------------------------------------
// CGSCallDivertModel::CheckStatusRestrictedCallDivertL
//
// Handles attempts to check divert status in restricted mode
// ---------------------------------------------------------------------------
void CGSCallDivertModel::CheckStatusRestrictedCallDivertL()
    {
    if( FeatureManager::FeatureSupported( KFeatureIdRestrictedCallDivert ) )
        {
        //Check call forwarding: * # 120*SI1 # SEND
        HBufC* sendString = HBufC::NewLC( KMaxLengthSendString );
        TPtr ptr = sendString->Des();
        ptr.Append( KAsterisk );
        ptr.Append( KHashMark );
        ptr.Append( KOneHundredTwenty );
        ptr.Append( KAsterisk );
        ptr.Append( KOne );
        ptr.Append( KHashMark );
        iBasicClient->SendUssd( *sendString );
        CleanupStack::PopAndDestroy( sendString );
        }
    }


// ---------------------------------------------------------------------------
// CGSCallDivertModel::CancelRestrictedCallDivertL
//
// Handles attempts to cancel call divert in restricted mode
// ---------------------------------------------------------------------------
void CGSCallDivertModel::CancelRestrictedCallDivertL()
    {
    if( FeatureManager::FeatureSupported( KFeatureIdRestrictedCallDivert ) )
        {
        //Cancel call forwarding: # 120*SI1 # SEND
        HBufC* sendString = HBufC::NewLC( KMaxLengthSendString );
        TPtr ptr = sendString->Des();
        ptr.Append( KHashMark );
        ptr.Append( KOneHundredTwenty );
        ptr.Append( KAsterisk );
        ptr.Append( KOne );
        ptr.Append( KHashMark );
        iBasicClient->SendUssd( *sendString );
        CleanupStack::PopAndDestroy( sendString );
        }
    }


// ---------------------------------------------------------------------------
// CGSCallDivertModel::SetDivertParametersL
//
// Sets parameters for EngineOperator function
// ---------------------------------------------------------------------------
void CGSCallDivertModel::SetDivertParametersL(
        TCallDivertSetting& aDivert,
        TBasicServiceGroups& aBsc,
        TGSCallDivertServices aDivertService,
        TGSCallDivertServiceGroups aDivertServiceGroup )
    {
    aDivert.iStatus = EDivertingStatusUnknown;
     //complains for ambiguous definition of EUnknown
     //need to correct in PhoneSettings.
    aBsc = ( TBasicServiceGroups )-1;

    if ( aDivertServiceGroup == EGSSettIdVoiceDivert )
        {
        aDivert.iServiceGroup = EServiceGroupVoice;
        }
    else
        {
        aDivert.iServiceGroup = EServiceGroupData;
        aBsc = EAllBearer;
        }

    switch ( aDivertService )
        {
        case ECFDivertVoiceCalls:
            {
            aDivert.iCondition = EDivertConditionUnconditional;
            TInt alsLine = ESSSettingsAlsNotSupported;
            TInt alsError = iSsSettings->Get( ESSSettingsAls, alsLine );
            if ( alsLine == ESSSettingsAlsNotSupported || ( alsLine == ESSSettingsAlsPrimary ) )
                {
                // Etelephony only activates voice service nothing else or causes
                // voice service status request.
                aBsc = ETelephony;
                }
            else // ESSSettingsAlsAlternate
                {
                // EAltTele only activates alternate service nothing else or causes
                // alternate service status request.
                aBsc = EAltTele;
                }
            break;
            }
        case ECFDivertWhenBusy:
            aDivert.iCondition = EDivertConditionBusy;
            break;
        case ECFDivertWhenNoAnswer:
            aDivert.iCondition = EDivertConditionNoReply;
            aDivert.iNoReplyTimer = iForward->GetTimerValueL();
            break;
        case ECFDivertWhenOutOfReach:
            aDivert.iCondition = EDivertConditionNotReachable;
            break;
        case ECFDivertWhenNotAvailable:
            aDivert.iCondition = EDivertConditionAllConditionalCases;
            aDivert.iNoReplyTimer = iForward->GetTimerValueL();
            break;
        case ECFDivertVideoCalls:
            aBsc = EAllBearer;
            aDivert.iCondition = EDivertConditionUnconditional;
            break;
        default:
            Panic ( KGSCallDivertModel, EInvalidCurrentItem );
            break;
        }
    }


// ---------------------------------------------------------------------------
// CGSCallDivertModel::MatchNumberToContactLC
//
// Returns the contact name matching the number, or the number if
// no contact name is found.
// Pushes the found (i.e. returned) name into Cleanupstack.
// @see CContactDatabase::MatchPhoneNumberL(const TDesC&,const TInt)
// ---------------------------------------------------------------------------
HBufC* CGSCallDivertModel::MatchNumberToContactLC( const TDesC& aTelNum )
    {
    CContactIdArray* idArray = iContactEngine->MatchPhoneNumberL(
        aTelNum,
        KCFCharactersToMatch );
    CleanupStack::PushL( idArray );
    HBufC* contactTitle = NULL;
    // fill data only if only single match is found
    TInt matches = idArray->Count();
    TBool conflict = EFalse;
    if ( matches >= KCFSingleContact )
        {
        //several matches, have to check that id is the same
        if ( matches != KCFSingleContact )
            {
            for ( TInt i = 1 ; i < matches ; i++ )
                {
                //all have to have same ID
                if ( (*idArray)[i] != (*idArray)[0] )
                    {
                    conflict = ETrue;
                    break; //stop comparing
                    }
                }
            }
        //Contact names are only shown if single match with same ID.
        if ( !conflict )
            {
            CPbkContactItem* pbkItem =
                iContactEngine->ReadContactLC( ( *idArray )[0] );
            contactTitle = iContactEngine->GetContactTitleL( *pbkItem );
            CleanupStack::PopAndDestroy( pbkItem );
            }
        }
    CleanupStack::PopAndDestroy( idArray );
    CleanupStack::PushL( contactTitle );
    if( contactTitle )
        {
        __GSLOGSTRING1("[CGSCallDivertModel::MatchNumberToContactLC] contactTitle: %S", &contactTitle);
        }
    return contactTitle;
    }


// ---------------------------------------------------------------------------
// Removes a special character from vmbx number.
// ---------------------------------------------------------------------------
void CGSCallDivertModel::RemoveSpecialChar( TDes& aVmbxNumber,
                                            const TDesC& aSpecChar )
    {
    TInt location = aVmbxNumber.Find( aSpecChar );
    if ( location != KErrNotFound )
        {
        aVmbxNumber = aVmbxNumber.Left( location );
        }
    }


// ---------------------------------------------------------------------------
// CGSCallDivertModel::DivertHandler
// ---------------------------------------------------------------------------
CPsetCallDiverting* CGSCallDivertModel::DivertHandler()
    {
    return iForward;
    }

// ---------------------------------------------------------------------------
// CGSCallDivertModel::SetDivertView
// ---------------------------------------------------------------------------
void CGSCallDivertModel::SetDivertView( CGSCallDivertView* aDivertView )
    {
    iDivertView = aDivertView;
    }

// ---------------------------------------------------------------------------
// CGSCallDivertModel::CGSCallDivertModel
// ---------------------------------------------------------------------------
TGSCallDivertServiceGroups CGSCallDivertModel::DivertServiceGroup()
    {
    return iDivertServiceGroup;
    }

// ---------------------------------------------------------------------------
// CGSCallDivertModel::SetDivertServiceGroup
// ---------------------------------------------------------------------------
void CGSCallDivertModel::SetDivertServiceGroup(
    TGSCallDivertServiceGroups aDivertServiceGroup )
    {
    iDivertServiceGroup = aDivertServiceGroup;
    __GSLOGSTRING1( "[CGSCallDivertModel::SetDivertServiceGroup] aDivertServiceGroup:%d ",
                    aDivertServiceGroup )
    }



// ---------------------------------------------------------------------------
// CGSCallDivertModel::NumberGroupingSupportedL
//
// ---------------------------------------------------------------------------
TBool CGSCallDivertModel::NumberGroupingSupportedL()
    {
    TInt numberGroupingVal;
    TBool retVal = EFalse;
    if( !iNumberGroupingRepository )
        {
        iNumberGroupingRepository = CRepository::NewL( KCRUidNumberGrouping );
        }
    iNumberGroupingRepository->Get( KNumberGrouping, numberGroupingVal );
    switch( numberGroupingVal )
        {
        case CPNGNumberGrouping::ENumberGroupingDisabled:
            retVal = EFalse;
            break;
        case CPNGNumberGrouping::ENumberGroupingEnabled:
            retVal = ETrue;
            break;
        default:
            retVal = EFalse;
            break;
        }
    return retVal;
    }

