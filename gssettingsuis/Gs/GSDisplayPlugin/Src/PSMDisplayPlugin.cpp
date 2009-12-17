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
* Description:  Stub imlementation
*
*/


// INCLUDE FILES
#include <psmsettingsprovider.h>
#include "PSMDisplayPlugin.h"
#include "GSDisplayPluginModel.h"
#include "GsLogger.h"
#include <psmsrvdomaincrkeys.h>
#include <featmgr.h>
#include <settingsinternalcrkeys.h>

#ifndef __WINS__
#include <power_save_display_mode.h>
#endif

// CONSTANT DEFINITIONS
const TUint32 KPSMDisplayPluginStorageId = 0x2000B591;

enum TPsmDisplayPluginKeys
    {
    EContrast = 1,
    EBacklightPeriod,
    EBrightness,
    EAmbientLightSensor,
    EScreenSaverPeriod,
    EAutomaticBrightnessControl
    };
//
// ----------------------------------------------------------------------------------
// CPSMDisplayPlugin::CPSMDisplayPlugin()
// ----------------------------------------------------------------------------------
//
CPSMDisplayPlugin::CPSMDisplayPlugin( TPsmPluginCTorParams& aInitParams ) :
    CPsmPluginBase( aInitParams )
	{
	}

// -----------------------------------------------------------------------------
// CPSMDisplayPlugin::ConstructL(
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPSMDisplayPlugin::ConstructL()
    {
    iModel = CGSDisplayPluginModel::NewL( EFalse );
#ifndef __WINS__ 
    iDisplayModel = CPowerSaveDisplayMode::NewL();
#endif
    }

//
// ----------------------------------------------------------------------------------
// CPSMDisplayPlugin::NewL()
// ----------------------------------------------------------------------------------
//
// Two-phased constructor.
CPSMDisplayPlugin* CPSMDisplayPlugin::NewL( TPsmPluginCTorParams& aInitParams )
	{
	CPSMDisplayPlugin* self = new ( ELeave ) CPSMDisplayPlugin( aInitParams );

	CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
	}

// ----------------------------------------------------------------------------------
// CPSMDisplayPlugin::~CPSMDisplayPlugin()
// ----------------------------------------------------------------------------------
//
// Destructor.
CPSMDisplayPlugin::~CPSMDisplayPlugin()
	{
    __GSLOGSTRING( "[CPSMDisplayPlugin::~CPSMDisplayPlugin]" );
    if( iModel ) delete iModel;

#ifndef __WINS__
    if ( iDisplayModel )
        {
        delete iDisplayModel;
        }
#endif
    
	}

// ----------------------------------------------------------------------------------
// CPSMDisplayPlugin::NotifyModeChange()
// ----------------------------------------------------------------------------------
//
void CPSMDisplayPlugin::NotifyModeChange( const TInt aMode )
    {
    TInt err = KErrNone;
    TRAP( err, DoModeChangeL( aMode ) );
    __GSLOGSTRING2( "[CPSMDisplayPlugin::NotifyModeChange]: Mode:%d Err:%d", aMode, err );

    }


// ----------------------------------------------------------------------------------
// CPSMDisplayPlugin::NotifyModeChange()
// ----------------------------------------------------------------------------------
//
void CPSMDisplayPlugin::DoModeChangeL( const TInt aMode )
    {
    RConfigInfoArray infoArray;

    TPsmsrvConfigInfo info1;
    info1.iConfigId = EContrast;
    info1.iConfigType = EConfigTypeInt;
    info1.iIntValue = iModel->ContrastL();
    infoArray.Append( info1 );

    TPsmsrvConfigInfo info2;
    info2.iConfigId = EBacklightPeriod;
    info2.iConfigType = EConfigTypeInt;
    info2.iIntValue = iModel->BacklightPeriodL();
    infoArray.Append( info2 );

    TPsmsrvConfigInfo info3;
    info3.iConfigId = EBrightness;
    info3.iConfigType = EConfigTypeInt;
    info3.iIntValue = iModel->BrightnessL();
    infoArray.Append( info3 );
    
    TPsmsrvConfigInfo info4;
    info4.iConfigId = EAmbientLightSensor;
    info4.iConfigType = EConfigTypeInt;
    info4.iIntValue = iModel->AmbientLightSensorL();
    infoArray.Append( info4 );
    
    TPsmsrvConfigInfo info5;
    info5.iConfigId = EScreenSaverPeriod;
    info5.iConfigType = EConfigTypeInt;
    info5.iIntValue = iModel->ScreenSaverPeriodL();
    infoArray.Append( info5 );

#ifndef __WINS__
    TPsmsrvConfigInfo info6;
    info6.iConfigId = EAutomaticBrightnessControl;
    info6.iConfigType = EConfigTypeInt;
    info6.iIntValue = iDisplayModel->GetPowerSaveLevel();
    infoArray.Append( info6 );
#endif

    __GSLOGSTRING1( "[CPSMDisplayPlugin::NotifyModeChangeL] Switching to mode:%d", aMode );

    // Print old values from array
    __GSLOGSTRING1( "[CPSMDisplayPlugin::NotifyModeChangeL]: oldValue info1: %d", infoArray[0].iIntValue );
    __GSLOGSTRING1( "[CPSMDisplayPlugin::NotifyModeChangeL]: oldValue info2: %d", infoArray[1].iIntValue );
    __GSLOGSTRING1( "[CPSMDisplayPlugin::NotifyModeChangeL]: oldValue info3: %d", infoArray[2].iIntValue );   
    __GSLOGSTRING1( "[CPSMDisplayPlugin::NotifyModeChangeL]: oldValue info4: %d", infoArray[3].iIntValue );   
    __GSLOGSTRING1( "[CPSMDisplayPlugin::NotifyModeChangeL]: oldValue info5: %d", infoArray[4].iIntValue );
#ifndef __WINS__
    __GSLOGSTRING1( "[CPSMDisplayPlugin::NotifyModeChangeL]: oldValue info6: %d", infoArray[5].iIntValue );
#endif

    iSettingsProvider.BackupAndGetSettingsL( infoArray, KPSMDisplayPluginStorageId );

    // Print new values from array
    __GSLOGSTRING1( "[CPSMDisplayPlugin::NotifyModeChangeL]: newValue info1: %d", infoArray[0].iIntValue );
    __GSLOGSTRING1( "[CPSMDisplayPlugin::NotifyModeChangeL]: newValue info2: %d", infoArray[1].iIntValue );
    __GSLOGSTRING1( "[CPSMDisplayPlugin::NotifyModeChangeL]: newValue info3: %d", infoArray[2].iIntValue );
    __GSLOGSTRING1( "[CPSMDisplayPlugin::NotifyModeChangeL]: newValue info4: %d", infoArray[3].iIntValue );   
    __GSLOGSTRING1( "[CPSMDisplayPlugin::NotifyModeChangeL]: newValue info5: %d", infoArray[4].iIntValue );
#ifndef __WINS__
    __GSLOGSTRING1( "[CPSMDisplayPlugin::NotifyModeChangeL]: newValue info6: %d", infoArray[5].iIntValue );
#endif
    
    if ( iModel->ContrastL ( )!= KErrNotSupported )
        {
        iModel->SetContrastL ( infoArray[0].iIntValue );
        }

    if ( FeatureManager::FeatureSupported ( KFeatureIdBrightnessControl ) )
        {
        iModel->SetBrightnessL ( infoArray[2].iIntValue );
        }

    if ( iModel->CheckScreenSaverTimeoutSupportL() )
        {
        iModel->SetScreenSaverPeriodL( infoArray[4].iIntValue );
        }

    iModel->SetBacklightPeriodL( infoArray[1].iIntValue );
    iModel->SetAmbientLightSensorL( infoArray[3].iIntValue );
 
#ifndef __WINS__
    if ( FeatureManager::FeatureSupported( KFeatureIdFfAbcAgressiveUi ) )
        {
        iDisplayModel->SetPowerSaveLevel( 
              (CPowerSaveDisplayMode::TPowerSaveLevel)infoArray[5].iIntValue );
        __GSLOGSTRING1( "[CPSMDisplayPlugin::NotifyModeChangeL]: level: %d", 
                iDisplayModel->GetPowerSaveLevel() );
        }
#endif
    
    infoArray.Reset();
    }


//End of file
