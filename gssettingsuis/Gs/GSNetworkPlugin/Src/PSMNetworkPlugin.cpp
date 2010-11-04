/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Stub imlementation
*
*/


// INCLUDE FILES
#include <psmsettingsprovider.h>
#include <psmsrvdomaincrkeys.h>
#include <featmgr.h>
#include <CoreApplicationUIsSDKCRKeys.h> // KCRUidCoreApplicationUIs, TCoreAppUIsNetworkConnectionAllowed
#include "PSMNetworkPlugin.h"
#include "GSNetworkPluginModel.h"
#include "GsLogger.h"

// CONSTANT DEFINITIONS
const TUint32 KPSMNetworkPluginStorageId = 0x2000B593;
// default value for network mode change key
const TInt KPowersavingNetworkmodeNoChanged = 0; 

enum TPSMNetworkPluginKeys
    {
    ENetworkMode = 1,
    };

//
// ----------------------------------------------------------------------------------
// CPSMNetworkPlugin::CPSMNetworkPlugin()
// ----------------------------------------------------------------------------------
//
CPSMNetworkPlugin::CPSMNetworkPlugin( TPsmPluginCTorParams& aInitParams ) :
    CPsmPluginBase( aInitParams )
	{
	}

// -----------------------------------------------------------------------------
// CPSMNetworkPlugin::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPSMNetworkPlugin::ConstructL()
    {
    __GSLOGSTRING( "[GS]-->[CPSMNetworkPlugin::ConstructL]" );
    
    iModel = CGSNetworkPluginModel::NewL( NULL,NULL );
    iPsmRepository = CRepository::NewL( KCRUidPowerSaveMode );
    // Read from CenRep so iPsmMode gets correct init value
    TInt psmMode;
    iPsmRepository->Get( KPsmCurrentMode, psmMode );
    iPsmMode = ( TPsmsrvMode )psmMode;
    
    __GSLOGSTRING( "[GS]<--[CPSMNetworkPlugin::ConstructL]" );
    }

//
// ----------------------------------------------------------------------------------
// CPSMNetworkPlugin::NewL()
// ----------------------------------------------------------------------------------
//
// Two-phased constructor.
CPSMNetworkPlugin* CPSMNetworkPlugin::NewL( TPsmPluginCTorParams& aInitParams )
	{
	CPSMNetworkPlugin* self = new ( ELeave ) CPSMNetworkPlugin( aInitParams );

	CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
	}

// ----------------------------------------------------------------------------------
// CPSMNetworkPlugin::~CPSMNetworkPlugin()
// ----------------------------------------------------------------------------------
//
// Destructor.
CPSMNetworkPlugin::~CPSMNetworkPlugin()
	{
    __GSLOGSTRING( "[CPSMNetworkPlugin::~CPSMNetworkPlugin]" );
    delete iModel;
    iModel = NULL;
    delete iPsmRepository;
    iPsmRepository = NULL;
	}

// ---------------------------------------------------------
// CPSMNetworkPlugin::IsPhoneOfflineL
//
// Checks if phone is in offline mode or not.
// Return ETrue if phone is in offline mode.
// Return EFalse if phone is not in offline mode.
// ---------------------------------------------------------
//
TBool CPSMNetworkPlugin::IsPhoneOfflineL() const
    {
    if ( FeatureManager::FeatureSupported( KFeatureIdOfflineMode ) )
        {
        CRepository* repository = CRepository::NewLC( KCRUidCoreApplicationUIs );
        TInt connAllowed = 1;
        repository->Get( KCoreAppUIsNetworkConnectionAllowed, connAllowed );
        CleanupStack::PopAndDestroy();  // repository
        if ( !connAllowed )
            {
            return ETrue;
            }
        }
    return EFalse;
    }

// ----------------------------------------------------------------------------------
// CPSMNetworkPlugin::NotifyModeChange()
// ----------------------------------------------------------------------------------
//
void CPSMNetworkPlugin::NotifyModeChange( const TInt aMode )
    {
    TInt err = KErrNone;
    TRAP( err, DoModeChangeL( aMode ) );
    if ( KErrNone != err)
        {}
    __GSLOGSTRING2( "[CPSMNetworkPlugin::NotifyModeChange]: Mode:%d Err:%d", aMode, err );
    }


// ----------------------------------------------------------------------------------
// CPSMNetworkPlugin::NotifyModeChange()
// ----------------------------------------------------------------------------------
//
void CPSMNetworkPlugin::DoModeChangeL( const TInt aMode )
    {
	TPsmsrvMode newMode = ( TPsmsrvMode )aMode;
    if ( !IsPhoneOfflineL() && 
         FeatureManager::FeatureSupported( KFeatureIdProtocolWcdma ) &&
         iModel->IsNetworkModeVisible() && IsChangeNetworkModeL ( iPsmMode, newMode ) )
        {
        RConfigInfoArray infoArray;
    
        TPsmsrvConfigInfo info1;
        info1.iConfigId = ENetworkMode;
        info1.iConfigType = EConfigTypeInt;
        info1.iIntValue = iModel->GetNetworkMode();
        infoArray.Append( info1 );
        
        __GSLOGSTRING1( "[CPSMNetworkPlugin::NotifyModeChangeL] Switching to mode:%d", aMode );
    
        __GSLOGSTRING1( "[CPSMNetworkPlugin::NotifyModeChangeL]: oldValue info1: %d", infoArray[0].iIntValue );
    
        iSettingsProvider.BackupAndGetSettingsL( infoArray, KPSMNetworkPluginStorageId );
        
        __GSLOGSTRING1( "[CPSMNetworkPlugin::NotifyModeChangeL]: newValue info1: %d", infoArray[0].iIntValue );
    
        // Don't change the network mode if there is ongoing phone call
        // since this will disconnect it
        if ( !iModel->IsCallActive() )
            {
            iModel->SetNetworkModeL ( infoArray[0].iIntValue  );
            }
        
        infoArray.Reset();       
        }
    }

// ----------------------------------------------------------------------------------
// CPSMNetworkPlugin::IsChangeNetworkMode
// ----------------------------------------------------------------------------------
//
TBool CPSMNetworkPlugin::IsChangeNetworkModeL( TPsmsrvMode& aOldMode, TPsmsrvMode aNewMode )
    {
    // get the value of network mode change key  
    TInt modeChange = iModel->GetPsmNetworkModeChangeL();    
    TPsmsrvMode oldMode = aOldMode;
    aOldMode = aNewMode;
    if ( ( oldMode == EPsmsrvModeNormal && aNewMode == EPsmsrvPartialMode )
         || ( oldMode == EPsmsrvModePowerSave && aNewMode == EPsmsrvPartialMode )
         || ( KPowersavingNetworkmodeNoChanged == modeChange ) )
        {
        return EFalse;
        }
    else
        {
        return ETrue;
        }
    }

//End of File


