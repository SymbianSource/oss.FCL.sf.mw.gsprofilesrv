/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
*        Model/Engine for Network Settings Plugin
*
*/


// INCLUDE FILES
#include "GSNetworkPluginModel.h"
#include "GSNetworkPlugin.h"
#include "GsNetworkPlugin.hrh"
#include "GsLogger.h"      //for logging traces
#include <featmgr.h>
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
#include "SettingsPrivateCRKeys.h"
#include <LogsDomainCRKeys.h>
#include <PsetContainer.h>
#include <PsetSAObserver.h>  //ongoing call check
#include "GSNetworkDebugHelper.h"
#include <gsnetworkpluginrsc.rsg>           //for resource IDs
#include <mmtsy_names.h>
//CONSTANTS
// default value for autolock period
// default value for Sat operations


_LIT( KGSNameOfClass, "CGSNetworkPluginModel" );

// Warning disabled as this constant is used inside __ASSERT_DEBUG
#pragma diag_suppress 177
_LIT( KGSNetworkPluginModelAssertName, "CGSNetworkPluginModel::CheckState" );


// ================= MEMBER FUNCTIONS =======================

// ----------------------------------------------------------------------------
// CGSNetworkPluginModel::NewL
//
// Symbian OS two-phased constructor, overloaded version
// ----------------------------------------------------------------------------
//
CGSNetworkPluginModel* CGSNetworkPluginModel::NewL(
        CGSNetworkPluginContainer* aContainer,
        CGSNetworkPlugin* aPlugin )
    {
    CGSNetworkPluginModel* self = new( ELeave ) CGSNetworkPluginModel(
            aContainer,
            aPlugin );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ----------------------------------------------------------------------------
// CGSNetworkPluginModel::CGSNetworkPluginModel
//
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CGSNetworkPluginModel::CGSNetworkPluginModel(
        CGSNetworkPluginContainer* aContainer,
        CGSNetworkPlugin* aPlugin )
    : iNetworkMode( KGSNetworkModeCapsNotUpdated ),
      iContainer( aContainer ), 
      iPlugin( aPlugin ) 
    {
    }


// ----------------------------------------------------------------------------
// CGSNetworkPluginModel::ConstructL
//
// EPOC default constructor can leave.
// ----------------------------------------------------------------------------
//
void CGSNetworkPluginModel::ConstructL()
    {
    ConstructBaseItemsL();
    }

// ----------------------------------------------------------------------------
// CGSNetworkPluginModel::ConstructBaseItemsL
//
// Constructing base items
// ----------------------------------------------------------------------------
//
void CGSNetworkPluginModel::ConstructBaseItemsL()
    {
    FeatureManager::InitializeLibL();
    InitializeCentralRepositoryL();
    SetCSPActiveL( ETrue );
    iSettingsContainer = CPsetContainer::NewL();
    User::LeaveIfError( iGSVariationRepository->Get(
            KSettingsVariationFlags, iLocalVariationValues ) );
#ifndef __WINS__
    //This is currently not supported by emulator
    CreatePhoneSettingsEngineL();
#endif //__WINS__
    }


// ----------------------------------------------------------------------------
// CGSNetworkPluginModel::~CGSNetworkPluginModel
//
// Destructor
// ----------------------------------------------------------------------------
//
CGSNetworkPluginModel::~CGSNetworkPluginModel()
    {
    UninitializeCentralRepository();

    if ( GetCSPStatus() )
        {
        TRAP_IGNORE( SetCSPActiveL( EFalse ) );
        }

    if ( iPhoneSettingsEngine )
        {
        delete iPhoneSettingsEngine;
        }

    if( iSettingsContainer )
        {
        delete iSettingsContainer;
        }

    FeatureManager::UnInitializeLib();

    if ( iContainer )
        {
        iContainer = NULL;
        }
    }

// ----------------------------------------------------------------------------
// CGSNetworkPluginModel::InitializeCentralRepositoryL
//
// Creating and setting keys for the Central Repository
// ----------------------------------------------------------------------------
//
void CGSNetworkPluginModel::InitializeCentralRepositoryL()
    {
    iNetworkRepository = CRepository::NewL( KCRUidNetworkSettings );
    iGSVariationRepository = CRepository::NewL( KCRUidSettingsVariation );
    }

// ----------------------------------------------------------------------------
// CGSNetworkPluginModel::UninitializeCentralRepositoryL
//
// Removes Central Repository objects
// ----------------------------------------------------------------------------
//
void CGSNetworkPluginModel::UninitializeCentralRepository()
    {
    if ( iNetworkRepository )
        {
        delete iNetworkRepository;
        iNetworkRepository = NULL;
        }
    if ( iGSVariationRepository )
        {
        delete iGSVariationRepository;
        iGSVariationRepository = NULL;
        }
    }

// ----------------------------------------------------------------------------
// CGSNetworkPluginModel::GetCSPStatus
//
// Get Customer Service Profile status
// ----------------------------------------------------------------------------
//
TBool CGSNetworkPluginModel::GetCSPStatus()
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
// CGSNetworkPluginModel::SetCSPActiveL
//
// Set Customer Service Profile active
// ----------------------------------------------------------------------------
//
void CGSNetworkPluginModel::SetCSPActiveL( TBool aValue )
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
// CGSNetworkPluginModel::IsNetworkModeVisible
//
// Get network mode status value from shared data.
// ----------------------------------------------------------------------------
//
TBool CGSNetworkPluginModel::IsNetworkModeVisible()
    {
    TBool nwModeAvailable = EFalse;
    TInt value;

    iNetworkRepository->Get( KSettingsNetworkMode, value );

    switch ( value )
        {
        case 1:
            nwModeAvailable = ETrue;
            break;
        case 0:
            nwModeAvailable = EFalse;
            break;
        default:
            break;
        }

    return nwModeAvailable;

    }

// ----------------------------------------------------------------------------
// CGSNetworkPluginModel::IsSettingSupported
//
// Check if a given setting is supported
// ----------------------------------------------------------------------------
//
TBool CGSNetworkPluginModel::IsSettingSupported( TInt aSettingNumber )
    {
    __ASSERT_ALWAYS( iCSP != NULL, User::Panic( KGSNameOfClass,
                                   EGSTelPluinModelPanicNullPtr ) );
    TBool settingSupported = EFalse;
    TInt retVal = KErrNone;
    switch ( aSettingNumber )
        {
        case EGSCSPCallForward:
            retVal = iCSP->IsCFSupported( settingSupported );
            break;
        case EGSCSPCallBarring:
            retVal = iCSP->IsCBSupported( settingSupported );
            break;
        case EGSCSPCallWaiting:
            retVal = iCSP->IsCWSupported( settingSupported );
            break;
        case EGSCSPAlternateLine:
            retVal = iCSP->IsALSSupported( settingSupported );
            break;
        case EGSCSPManualNetworkSelection:
            retVal = iCSP->IsNetworkSelectionSupported( settingSupported );
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
// CGSNetworkPluginModel::GetMCNSettingValue
// Gets MCN setting value
//
// ----------------------------------------------------------------------------
//
TBool CGSNetworkPluginModel::GetMCNSettingValue( TInt& aId )
    {
    TInt ret = KErrNone;
    ret = iNetworkRepository->Get( KSettingsMcnDisplay, aId );
    return ret;
    }

// ----------------------------------------------------------------------------
// CGSNetworkPluginModel::SetMCNSettingValue
// Sets MCN setting value
//
// ----------------------------------------------------------------------------
//
TBool CGSNetworkPluginModel::SetMCNSettingValue( TInt& aId )
    {
    TInt ret = KErrNone;
    ret = iNetworkRepository->Set( KSettingsMcnDisplay, aId );
    return ret;
    }

// ----------------------------------------------------------------------------
// CGSNetworkPluginModel::GetSupportedNetworksL
//
// Get supported network mode list
// ----------------------------------------------------------------------------
//
TInt CGSNetworkPluginModel::GetSupportedNetworksL()
    {
    TInt supportedNetworks = KGSSettingOff;
    User::LeaveIfError( iNetworkRepository->
                Get( KSettingsNetworkModeList, supportedNetworks ) );

    return supportedNetworks;
    }

// ---------------------------------------------------------
// CGSNetworkPluginModel::GraphicalNetworkListSupportedL
//
// Check if graphical network list is supported.
// ---------------------------------------------------------
//
TBool CGSNetworkPluginModel::GraphicalNetworkListSupportedL()
    {
    TBool supported = EFalse;

    if ( iLocalVariationValues & EGSConfig2G3GNetworkIcon )
        {
        supported = ETrue;
        }

    return supported;
    }

// ---------------------------------------------------------
// CGSTelPluginModel::AutomaticNetworkSearchSupportedL
//
// Check if automatic to automatic network search is supported.
// ---------------------------------------------------------
//
TBool CGSNetworkPluginModel::AutomaticNetworkSearchSupportedL()
    {
    TBool supported = EFalse;

    if ( iLocalVariationValues & EGSConfigNoAutoToAutoNetworkSearch )
        {
        supported = ETrue;
        }
    return supported;
    }


// ---------------------------------------------------------------------------
//
// Sets network mode to member variable.
//
// ---------------------------------------------------------------------------
//
void CGSNetworkPluginModel::CreatePhoneSettingsEngineL()
    {
    __GSLOGSTRING("[GS]--> CGSNetworkPluginModel::CreatePhoneSettingsEngineL");
    //initialize the phone settings object for network mode
    if ( !iPhoneSettingsEngine )
        {
        iPhoneSettingsEngine = iSettingsContainer->CreateNetworkModeObjectL( *this );
        }
    iPhoneSettingsEngine->SetNetworkModeObserver( *this );
    //CPSMNetworkPlugin will init iPlugin NULL.
    if ( iPlugin == NULL )
    	{
    	// get the current net mode synchronously
    	StartSynGetCurrentNetworkModeSelectionL();
    	}
    else
    	{
    	// get the current net mode asynchronously
    	StartAsynGetCurrentNetworkModeSelectionL();
    	}
    
    __GSLOGSTRING("[GS] <--CGSNetworkPluginModel::CreatePhoneSettingsEngineL");
    }


// ---------------------------------------------------------------------------
// CGSNetworkPluginModel::HandleNetworkSystemModeEventsL
//
// Handles Network system mode list fetching from CustomAPI.
// Handles Network system mode setting to CustomAPI.
// Handles current network system mode fetching from CustomAPI.
//
// ---------------------------------------------------------------------------
//
void CGSNetworkPluginModel::HandleNetworkSystemModeEventsL(
            const MPsetNetworkModeObserver::TServiceRequest aRequest,
            const TUint32 aNetworkModeCaps )
    {
#ifdef _DEBUG
    TBuf<KGSNetworkModeDebugStrLen> requestBuf;
    GSNetworkDebugHelper::NetworkModeRequestToDes( aRequest, requestBuf );
    __GSLOGSTRING1( "[CGSNetworkPluginModel::HandleNetworkSystemModeEventsL] %S ",
                    &requestBuf );
#endif // _DEBUG

    PrintState();

    switch ( aRequest )
        {
        case MPsetNetworkModeObserver::EServiceRequestGetCurrentNetworkMode:
            {
            TBool networkModeChanged = EFalse;
            if( iNetworkMode != aNetworkModeCaps )
                {
                networkModeChanged = ETrue;
                iNetworkMode = aNetworkModeCaps;
                }
            if( networkModeChanged )
                {
                if ( iContainer )
                    {
                    iContainer->UpdateListBoxL( EGSNetworkModeItemId, 0 );
                    }
                // Will update nw setting page in case it is open.
                if ( iPlugin )
                    {
                    iPlugin->UpdateNetworkSettingPageL();
                    }
                }
            }
            break;
        case MPsetNetworkModeObserver::EServiceRequestSetSelectedNetworkMode:
            {

            /**
             * Setting network mode succeeded: update actual mode:
             */
            iNetworkMode = iRequestedNetworkMode;
            
            /**
             * iNetworkMode should not be updated in case of error.
             * ->update only in this success case.
             *
             * Problem: Currently unclear if aNetworkModeCaps contains 
             * valid cata in this EServiceRequestSetSelectedNetworkMode case.
             * So, cannot update iNetworkMode from aNetworkModeCaps here.
             * Instead use iRequestedNetworkMode manually.
             */

            if( iContainer )
                {
                iContainer->HandleNetworkModeChangeL();
                }
            break;
            }
        default:
            break;
        }
    PrintState();
    }


// ---------------------------------------------------------------------------
// CGSNetworkPluginModel::HandleNetworkErrorL
//
// From MPsetNetworkModeObserver. This is called if 
// EServiceRequestSetSelectedNetworkMode fails. No need to handle other cases.
// ---------------------------------------------------------------------------
//
void CGSNetworkPluginModel::HandleNetworkErrorL(
            const MPsetNetworkModeObserver::TServiceRequest aRequest,
            const TInt aError )
    {
#ifdef _DEBUG
    TBuf<KGSNetworkModeDebugStrLen> requestBuf;
    GSNetworkDebugHelper::NetworkModeRequestToDes( aRequest, requestBuf );
    __GSLOGSTRING2( "[CGSNetworkPluginModel::HandleNetworkErrorL] %S error:%d",
                    &requestBuf, aError );
#endif // _DEBUG

    /**
     * Handles failing when setting network mode.
     * Requested mode is stored into a member and actual network mode is updated only
     * when success is reported in HandleNetworkSystemModeEventsL/
     * EServiceRequestSetSelectedNetworkMode.
     */
    switch( aRequest )
        {
        case MPsetNetworkModeObserver::EServiceRequestGetCurrentNetworkMode:
            break;
        case MPsetNetworkModeObserver::EServiceRequestSetSelectedNetworkMode:
            // Hould display error note, iNetworkMode value is still correct as
            // it is not updated in case request failed.
            if ( iPlugin )
                {
                iPlugin->ShowNoteL( R_NO_NETWORK_ACCESS, KGSErrorNote );   
                }
            break;
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// CGSNetworkPluginModel::GetNetworkSelectionMode
//
// Gets network selection mode (automatic/manual)
// ---------------------------------------------------------------------------
//
MPsetNetworkSelect::TSelectMode CGSNetworkPluginModel::GetNetworkSelectionMode()
    {
    MPsetNetworkSelect::TSelectMode mode =
            MPsetNetworkSelect::ENetSelectModeAutomatic;
#ifndef __WINS__
    iPhoneSettingsEngine->GetNetworkSelectMode( mode );
#endif //__WINS__

    return mode;
    }

// ---------------------------------------------------------------------------
// CGSNetworkPluginModel::StartAsynGetCurrentNetworkModeSelectionL
//
// ---------------------------------------------------------------------------
//
void CGSNetworkPluginModel::StartAsynGetCurrentNetworkModeSelectionL()
    {
    if ( iPhoneSettingsEngine )
        {
        iPhoneSettingsEngine->GetCurrentNetworkModeSelectionL();   
        }
    }

// ---------------------------------------------------------------------------
// CGSNetworkPluginModel::StartSynGetCurrentNetworkModeSelectionL
//
// ---------------------------------------------------------------------------
//
void CGSNetworkPluginModel::StartSynGetCurrentNetworkModeSelectionL()
    {
	// Connects to Etel and loads Tsy module
	RMobilePhone phone;
    RTelServer   server;
    // Custom phone.
    RMmCustomAPI customPhone;
    
    TInt err = KErrNone;
	for ( TInt a = 0; a < KPSetRetryCount; a++ )
		{
		err = server.Connect();

		if ( err == KErrNone )
			{
			break;
			}

		User::After( KPSetRetryTimeout );
		}

	if ( err != KErrNone )
		{
		User::Leave( err );
		}
	else
		{
		server.LoadPhoneModule( KMmTsyModuleName );
		}
	User::LeaveIfError( server.SetExtendedErrorGranularity( RTelServer::EErrorExtended ) );

	TInt numPhones;

	User::LeaveIfError( server.EnumeratePhones( numPhones ) );
	if ( !numPhones )
		{
		User::Leave( KErrGeneral );
		}

	//match phone name to correct one
	RTelServer::TPhoneInfo phoneInfo;
	TName matchTsyName;
	TInt i = 0;
	for (; i < numPhones; i++)
		{
		User::LeaveIfError(server.GetTsyName(i, matchTsyName));
		if (matchTsyName.CompareF(KMmTsyModuleName) == 0)
			{
			User::LeaveIfError(server.GetPhoneInfo(i, phoneInfo));
			break;
			}
		}
	if (i == numPhones)
		{
		User::Leave(KErrGeneral);
		}

	//open phone subsession
	User::LeaveIfError( phone.Open( server, phoneInfo.iName ) );
	User::LeaveIfError( customPhone.Open( phone ) );
	TUint32 currentNetworkModes;
	customPhone.GetCurrentSystemNetworkModes( currentNetworkModes );
	iNetworkMode = (TInt)currentNetworkModes;
	customPhone.Close();
	phone.Close();
	server.Close();	
	}

// ---------------------------------------------------------------------------
// CGSNetworkPluginModel::GetNetworkMode
//
// ---------------------------------------------------------------------------
//
TUint32 CGSNetworkPluginModel::GetNetworkMode()
    {
    /*
     * This does not actually ask for the mode from phone engine as
     * iPhoneSettingsEngine->GetCurrentNetworkModeSelectionL();
     * Has allready been called in constructor and afterwards we will receive
     * notifications if mode changes.
     */
    return iNetworkMode;
    }


void CGSNetworkPluginModel::SetNetworkModeL( TUint32 aNetworkMode )
    {
    __GSLOGSTRING1( "[GS]--> [CGSNetworkPluginModel::SetNetworkModeL]:%d", aNetworkMode );
    
    // Store requested mode but iNetworkMode should not updated unless request
    // succeeds.
    iRequestedNetworkMode = aNetworkMode;
    if( iPhoneSettingsEngine )
        {
        // Cancel the ongoing operation, otherwise, the set network mode operation will
        // leave with error code: KErrInUse. 
        if ( iPhoneSettingsEngine->IsActive() )
            {
            iPhoneSettingsEngine->Cancel();
            }
        iPhoneSettingsEngine->SetNetworkModeSelectionL( aNetworkMode );
        }
    
    __GSLOGSTRING1( "[GS]<-- [CGSNetworkPluginModel::SetNetworkModeL]:%d", aNetworkMode );
    }


// ---------------------------------------------------------------------------
// CGSNetworkPluginModel::PrintState
//
// ---------------------------------------------------------------------------
void CGSNetworkPluginModel::PrintState()
    {
#ifdef _DEBUG
    TBuf<KGSNetworkModeDebugStrLen> networkModeCaps;
    GSNetworkDebugHelper::NwCapsToDes( iNetworkMode, networkModeCaps );
    __GSLOGSTRING1( "[CGSNetworkPluginModel::PrintState] iNetworkMode:  %S ",
                    &networkModeCaps );
#endif // _DEBUG
    }

// ---------------------------------------------------------------------------
// CGSNetworkPluginModel::CheckState
// For debugging
// ---------------------------------------------------------------------------
void CGSNetworkPluginModel::CheckState( TGSNetworkModeItems aMode )
    {
#ifdef _DEBUG
    TBuf<KGSNetworkModeDebugStrLen> networkModeCaps;
    GSNetworkDebugHelper::NwCapsToDes( iNetworkMode, networkModeCaps );
    TBuf<KGSNetworkModeDebugStrLen> networkMode;
    GSNetworkDebugHelper::NwToDes( aMode, networkMode );

    __GSLOGSTRING2( "[CGSNetworkPluginModel::CheckState] %S <-> %S",
                    &networkModeCaps, &networkMode );

    switch ( iNetworkMode )
        {
        case RMmCustomAPI::KCapsNetworkModeDual:
            __ASSERT_DEBUG(
                aMode == EGSNetworkModeDualmode,
                User::Panic( KGSNetworkPluginModelAssertName, KErrArgument ) );
            break;
        case RMmCustomAPI::KCapsNetworkModeUmts:
            __ASSERT_DEBUG(
                aMode == EGSNetworkModeUMTS,
                User::Panic( KGSNetworkPluginModelAssertName, KErrArgument ) );
            break;
        case RMmCustomAPI::KCapsNetworkModeGsm:
            __ASSERT_DEBUG(
                aMode == EGSNetworkModeGSM,
                User::Panic( KGSNetworkPluginModelAssertName, KErrArgument ) );
            break;
        case KGSNetworkModeCapsNotUpdated:
            // Not updated yet so cannot check state.
            break;
        default:
            __ASSERT_DEBUG(
                EFalse,
                User::Panic( KGSNetworkPluginModelAssertName, KErrArgument ) );
            break;
        }

#endif // _DEBUG
    }



TBool CGSNetworkPluginModel::IsCallActive()
    {
    TBool callActive = EFalse;

#ifndef __WINS__
//This is currently not supported by emulator
    
    if ( iPhoneSettingsEngine->IsCallActive() != CPsetSAObserver::EPSetNoCallsActive )
        {
        callActive = ETrue;
        }
    
#endif //__WINS__
    
    return callActive;
    }

//  End of File
