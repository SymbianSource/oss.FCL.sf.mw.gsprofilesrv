/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Display Settings model implementation.
*
*/


// INCLUDE FILES
#include "GSDisplayPluginModel.h"

#include <hal.h>
#include <e32math.h>
#include <featmgr.h>
#include <generalsettingsvariant.hrh>

#include <settingsinternalcrkeys.h>
#include <startupdomaincrkeys.h>
#include <ScreensaverInternalCRKeys.h>
#include <hwrmlightdomaincrkeys.h>
#include <coreapplicationuisdomainpskeys.h>
#include <AvkonInternalCRKeys.h> // for zooming
#include <AknDef.hrh>
#include "GsLogger.h"
#include "SettingsPrivateCRKeys.h"

// Used for OperatorLogo
#include <commdb.h>
#include <telephonydomainpskeys.h>
#include "GSDisplayPluginContainer.h"
#include "GsDisplayPlugin.hrh"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ==============================

// ========================= MEMBER FUNCTIONS =================================

// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::NewL
//
// Symbian OS two-phased constructor
// ----------------------------------------------------------------------------
//
CGSDisplayPluginModel* CGSDisplayPluginModel::NewL()
    {
    return CGSDisplayPluginModel::NewL( ETrue );
    }


// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::NewL
//
// Symbian OS two-phased constructor
// ----------------------------------------------------------------------------
//
CGSDisplayPluginModel* CGSDisplayPluginModel::NewL( TBool aInitBackgroundApi )
    {
    CGSDisplayPluginModel* self = new( ELeave ) CGSDisplayPluginModel;
    CleanupStack::PushL( self );
    self->ConstructL( aInitBackgroundApi );

    CleanupStack::Pop( self );
    return self;
    }


// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::CGSDisplayPluginModel
//
//
// C++ default constructor can NOT contain any code, that might leave.
// ----------------------------------------------------------------------------
//
CGSDisplayPluginModel::CGSDisplayPluginModel()
    {
    }


// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::ConstructL
//
// EPOC default constructor can leave.
// ----------------------------------------------------------------------------
//
void CGSDisplayPluginModel::ConstructL( TBool aInitBackgroundApi )
    {
    //__GSENGINELOGSTRING("CGSDisplayPluginModel::ConstructL begin");
    FeatureManager::InitializeLibL();
    
    InitializeCentralRepositoryL();

    //Background image DLL initializing
    if( aInitBackgroundApi ) iBackgroundApi = CGSBackgroundImage::NewL();

    User::LeaveIfError( iGSVariationRepository->Get(
                        KSettingsVariationFlags, iLocalVariationValues ) );
    }


// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::~CGSDisplayPluginModel
//
// Destructor
// ----------------------------------------------------------------------------
//
CGSDisplayPluginModel::~CGSDisplayPluginModel()
    {
    UninitializeCentralRepository();
    FeatureManager::UnInitializeLib();
    if( iBackgroundApi ) delete iBackgroundApi;
    }


// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::InitializeCentralRepositoryL
//
// Creating and setting keys for the Central Repository
// ----------------------------------------------------------------------------
//
void CGSDisplayPluginModel::InitializeCentralRepositoryL()
    {
    iStartupConfRepository = CRepository::NewL( KCRUidStartupConf );
    iScreensaverRepository = CRepository::NewL( KCRUidScreenSaver );
    iPersonalizationRepository =
            CRepository::NewL( KCRUidPersonalizationSettings );
    iLightRepository = CRepository::NewL( KCRUidLightSettings );
    iGSVariationRepository = CRepository::NewL( KCRUidSettingsVariation );
    iAvkonRepository = CRepository::NewL( KCRUidAvkon );
    
    // start listening to CenRep key changes for Operator Logo on/off
    iNotifyHandlerForOpLogo = CCenRepNotifyHandler::NewL( *this, 
                           *iPersonalizationRepository,
                           CCenRepNotifyHandler::EIntKey, 
                           KSettingsDisplayOperatorLogo );
    iNotifyHandlerForOpLogo->StartListeningL();
    iNotifyHandlerForOpLogoVisible = CCenRepNotifyHandler::NewL( *this, 
                           *iPersonalizationRepository,
                           CCenRepNotifyHandler::EIntKey, 
                           KSettingsShowOperatorLogoSetting );
    iNotifyHandlerForOpLogoVisible->StartListeningL();
    }

// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::UninitializeCentralRepositoryL
//
// Removes Central Repository objects
// ----------------------------------------------------------------------------
//
void CGSDisplayPluginModel::UninitializeCentralRepository()
    {
    if ( iStartupConfRepository )
        {
        delete iStartupConfRepository;
        iStartupConfRepository = NULL;
        }
    if ( iScreensaverRepository )
        {
        delete iScreensaverRepository;
        iScreensaverRepository = NULL;
        }
    if ( iNotifyHandlerForOpLogo )
        {
        iNotifyHandlerForOpLogo->StopListening();
        delete iNotifyHandlerForOpLogo;
        }
    if ( iNotifyHandlerForOpLogoVisible )
        {
        iNotifyHandlerForOpLogoVisible->StopListening();
        delete iNotifyHandlerForOpLogoVisible;
        }
    if ( iPersonalizationRepository )
        {
        delete iPersonalizationRepository;
        iPersonalizationRepository = NULL;
        }
    if ( iLightRepository )
        {
        delete iLightRepository;
        iLightRepository = NULL;
        }
    if ( iGSVariationRepository )
        {
        delete iGSVariationRepository;
        iGSVariationRepository = NULL;
        }
    if ( iAvkonRepository )
        {
        delete iAvkonRepository;
        iAvkonRepository = NULL;
        }
    }


// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::WelcomeNoteTypeL
//
// Returns user welcome note type.
// ----------------------------------------------------------------------------
//
TInt CGSDisplayPluginModel::WelcomeNoteTypeL()
    {
    TInt type;
    User::LeaveIfError( iStartupConfRepository->Get( KStartupWelcomeNoteType,
                                                     type ) );

    return type;
    }

// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::SetWelcomeNoteTypeL
//
// Sets user welcome note type.
// ----------------------------------------------------------------------------
//
void CGSDisplayPluginModel::SetWelcomeNoteTypeL( const TInt aType )
    {
    User::LeaveIfError( iStartupConfRepository->
                        Set( KStartupWelcomeNoteType, aType ) );

    if ( aType != KGSWelcomeNoteTypeImage )
        {
        // It doesn't matter much if the image deletion fails,
        // so no need to handle the error.
        TRAP_IGNORE( iBackgroundApi->DeleteImageL( KGSWelcomeNoteImgPath ) );
        }
    }

// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::GetWelcomeNoteTextL
//
// Reads welcome note text from shared data and returns it
// ----------------------------------------------------------------------------
//
void CGSDisplayPluginModel::WelcomeNoteTextL( TDes& aNote )
    {
    User::LeaveIfError( iStartupConfRepository->Get( KStartupWelcomeNoteText,
                                                     aNote ) );
    }

// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::SetWelcomeNoteTextL
//
// Writes welcome note text to shared data
// ----------------------------------------------------------------------------
//
void CGSDisplayPluginModel::SetWelcomeNoteTextL( const TDesC& aNote )
    {
    User::LeaveIfError( iStartupConfRepository->Set( KStartupWelcomeNoteText,
                                                     aNote ) );
    }

// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::ContrastL
//
// Returns contrast value.
// ----------------------------------------------------------------------------
//
TInt CGSDisplayPluginModel::ContrastL()
    {
    TInt contrast = 0;
#ifndef __WINS__
    if( HAL::Get( HAL::EDisplayContrast, contrast ) == KErrNotSupported )
        {
        return KErrNotSupported;
        }
#endif //__WINS__
    return contrast;
    }


// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::SetContrastL
//
// sets contrast value.
// ----------------------------------------------------------------------------
//
void CGSDisplayPluginModel::SetContrastL( const TInt aContrast )
    {
#ifndef __WINS__
    __GSLOGSTRING1("[CGSDisplayPluginModel::SetContrastL(%d)]", aContrast );
    User::LeaveIfError( HAL::Set( HAL::EDisplayContrast, aContrast ) );
#endif //__WINS__
    }


// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::ScreenSaverObjectL
//
// Returns screen saver mode.
// ----------------------------------------------------------------------------
//
TInt CGSDisplayPluginModel::ScreenSaverObjectL()
    {
    TInt object = KErrNone;

    User::LeaveIfError( iScreensaverRepository->Get( KScreenSaverObject,
                                                     object ) );

    return object;
    }

// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::SetScreenSaverObjectL
//
// Sets screen saver mode.
// ----------------------------------------------------------------------------
//
void CGSDisplayPluginModel::SetScreenSaverObjectL( const TInt aObject )
    {
    User::LeaveIfError( iScreensaverRepository->Set( KScreenSaverObject,
                                                     aObject ) );
    }


// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::GetScreenSaverTextL
//
// Reads screen saver text from shared data and returns it.
// ----------------------------------------------------------------------------
//
void CGSDisplayPluginModel::GetScreenSaverTextL( TDes& aText )
    {
    User::LeaveIfError( iScreensaverRepository->Get( KScreenSaverText,
                                                     aText ) );
    }

// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::SetScreenSaverTextL
//
// Writes screen saver text to shared data.
// ----------------------------------------------------------------------------
//
void CGSDisplayPluginModel::SetScreenSaverTextL( const TDesC& aText )
    {
    User::LeaveIfError( iScreensaverRepository->Set( KScreenSaverText,
                                                     aText ) );
    }


// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::ScreenSaverPeriodL
//
// Reads screen saver period from shared data and returns it.
// ----------------------------------------------------------------------------
//
TInt CGSDisplayPluginModel::ScreenSaverPeriodL()
    {
    TInt period = KGSSettingOff;
    User::LeaveIfError( iPersonalizationRepository->
            Get( KSettingsScreenSaverPeriod, period ) );

    return period;
    }

// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::SetScreenSaverPeriodL
//
// Writes screen saver text to shared data.
// ----------------------------------------------------------------------------
//
void CGSDisplayPluginModel::SetScreenSaverPeriodL( const TInt aPeriod )
    {
    User::LeaveIfError( iPersonalizationRepository->
            Set( KSettingsScreenSaverPeriod, aPeriod ) );
    }


// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::BacklightPeriodL
//
// Reads backlight timeout value from shared data and returns it.
// ----------------------------------------------------------------------------
//
TInt CGSDisplayPluginModel::BacklightPeriodL()
    {
    TInt period = KGSSettingOff;
    User::LeaveIfError( iLightRepository->
                Get( KDisplayLightsTimeout, period ) );

    return period;
    }

// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::SetBacklightPeriodL
//
// Writes new backlight timeout value to shared data.
// ----------------------------------------------------------------------------
//
void CGSDisplayPluginModel::SetBacklightPeriodL( const TInt aPeriod )
    {
    __GSLOGSTRING1("[CGSDisplayPluginModel::SetBacklightPeriodL(%d)]", aPeriod );
    User::LeaveIfError( iLightRepository->
            Set( KDisplayLightsTimeout, aPeriod ) );
    }


// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::AmbientLightSensorL
//
// Returns Ambient Light Sensor value.
// ----------------------------------------------------------------------------
//
TInt CGSDisplayPluginModel::AmbientLightSensorL()
    {
    TInt ret;
    User::LeaveIfError( iLightRepository->Get( KLightSensorSensitivity,
                                               ret ) );

    return ScaleLightSensorValues( ret, ETrue );
    }


// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::SetAmbientLightSensorL
//
// Sets Ambient Light Sensor value.
// ----------------------------------------------------------------------------
//
void CGSDisplayPluginModel::SetAmbientLightSensorL(
                            const TInt aSensorSensitivity )
    {
    TInt sensitivity;
    sensitivity = ScaleLightSensorValues( aSensorSensitivity, EFalse );

    User::LeaveIfError( iLightRepository->
        Set( KLightSensorSensitivity, sensitivity ) );
    }


// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::ScaleLightSensorValues
//
// Scaling Ambient Light Sensor values
// ----------------------------------------------------------------------------
//
TInt CGSDisplayPluginModel::ScaleLightSensorValues( TInt aValue,
                                                    TBool aDirection )
    {
    TInt coeff = 25;
    TInt ret = 0;

    if( aDirection ) //scaling to slider steps
        {
        ret = ( aValue / coeff ) + 1;
        }
    else //scaling to sensor percentage
        {
        ret = ( aValue - 1 ) * coeff;
        }

    return ret;
    }


// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::HandleNotifyInt
//
// Handle notification from MCenRepNotifyHandlerCallback
// ----------------------------------------------------------------------------
//
void CGSDisplayPluginModel::HandleNotifyInt( TUint32 aId, TInt /*aNewValue*/)
    {
    if ( aId == KSettingsDisplayOperatorLogo ||
         aId == KSettingsShowOperatorLogoSetting )
        {
        iContainer->UpdateListBoxL(EGSSettIdOperatorLogo);
        iContainer->CloseDialog();
        }
    }


// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::ScaleIntensityValues
//
// Converts the slider control steps to Intensity values
// ----------------------------------------------------------------------------
//
TInt CGSDisplayPluginModel::ScaleIntensityValues( TInt aValue, TBool aSample )
    {
    TReal sampledValue;
    TReal intResult;
    TReal sampleCoeff = 3.226; //100 by 31
    TInt aDecimalPlaces = 0;
    TInt32 result;

    if( aSample )
        {
        intResult = sampleCoeff * aValue;
        }
    else
        {
        intResult = aValue / sampleCoeff;
        }

    if( intResult > 1 )
        {
        Math::Round( sampledValue, intResult, aDecimalPlaces );
        Math::Int( result, sampledValue );
        }
    else
        {
        result = 1;
        }
    return result;
    }


// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::SetBackLightValueL
//
// ----------------------------------------------------------------------------
//
void CGSDisplayPluginModel::SetBackLightValueL()
    {
    iCoreAppProperty->Attach( KPSUidCoreApplicationUIs, KLightsControl );

    TLightsControl value = ELightsUninitialized;
    iCoreAppProperty->Set( KPSUidCoreApplicationUIs,
                           KLightsControl, value );

    iCoreAppProperty->Cancel();

    delete iCoreAppProperty;
    iCoreAppProperty = NULL;
    }

// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::IsUWNoteSupportedL
//
// Allow user welcome note item to be disabled, if required
// ----------------------------------------------------------------------------
//
TInt CGSDisplayPluginModel::IsUWNoteSupportedL()
    {
    TInt value = KGSSettingOff;
    User::LeaveIfError( iPersonalizationRepository->
            Get( KSettingsWelcomeNoteSupported, value ) );

    return value;
    }


// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::BrightnessL
//
// Returns brightness value from DosLights.
// ----------------------------------------------------------------------------
//
TInt CGSDisplayPluginModel::BrightnessL()
    {
    if( FeatureManager::FeatureSupported( KFeatureIdBrightnessControl ) )
        {
        TInt brightness;
        User::LeaveIfError( iLightRepository->Get( KLightIntensity,
                                                   brightness ) );

        return ScaleIntensityValues( brightness, EFalse );
        }
    else
        {
        return KErrNotSupported;
        }
    }


// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::SetBrightnessL
//
// Sets brightness value to DosLights.
// ----------------------------------------------------------------------------
//
void CGSDisplayPluginModel::SetBrightnessL( const TInt aBrightness )
    {
    if( FeatureManager::FeatureSupported( KFeatureIdBrightnessControl ) )
        {
        TInt brightness;

        brightness = ScaleIntensityValues( aBrightness, ETrue );
        __GSLOGSTRING1("[CGSDisplayPluginModel::SetBrightnessL(%d)]", aBrightness );
        User::LeaveIfError( iLightRepository->Set( KLightIntensity,
                                                   brightness ) );
        }
    }


// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::DisplayTextSizeL
//
// Return current display text size.
// ----------------------------------------------------------------------------
//
TInt CGSDisplayPluginModel::DisplayTextSizeL()
    {
    TInt textSize = EAknUiZoomNormal;

    if ( FeatureManager::FeatureSupported ( KFeatureIdUiZoom ) )
        {
        User::LeaveIfError( iAvkonRepository->Get( KAknGlobalUiZoom,
                                                   textSize ) );
        }

    return textSize;
    }


// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::SetDisplayTextSizeL
//
// Sets new display text size.
// ----------------------------------------------------------------------------
//
void CGSDisplayPluginModel::SetDisplayTextSizeL( TInt aSize )
    {
    // not checking for feature manager constant to avoid compiler
    // warning for the input parameter.
    User::LeaveIfError( iAvkonRepository->Set( KAknGlobalUiZoom, aSize ) );
    }


// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::PowerSaveLedL
//
// Returns Tracking Transaction state value.
// ----------------------------------------------------------------------------
//
TInt CGSDisplayPluginModel::PowerSaveLedL()
    {
    TInt ret = 0;
    User::LeaveIfError( iPersonalizationRepository->Get(
        KSettingsDisplayTurnoffTimeout, ret ) );
    return ret;
    }


// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::SetPowerSaveLedL
//
// Sets Tracking Transaction state value.
// ----------------------------------------------------------------------------
//
void CGSDisplayPluginModel::SetPowerSaveLedL( const TInt aPowerLedState )
    {
    User::LeaveIfError( iPersonalizationRepository->
        Set( KSettingsDisplayTurnoffTimeout, aPowerLedState ) );

    }


// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::CheckPowerSaveLedSupportL
//
// Checks local variation flag for supporting Power Save Led feature.
// ----------------------------------------------------------------------------
//
TInt CGSDisplayPluginModel::CheckPowerSaveLedSupportL()
    {
    TInt value = 0;
    User::LeaveIfError( iGSVariationRepository->Get(
                        KSettingsVariationFlags, value ) );

    TBool supported = EFalse;

    if ( value & EGSConfigPowerSaveLed )
        {
        supported = ETrue;
        }

    return supported;
    }


// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::BackgroundImage
// ----------------------------------------------------------------------------
//
CGSBackgroundImage* CGSDisplayPluginModel::BackgroundImage()
    {
    return iBackgroundApi;
    }


// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::CheckScreenSaverTimeoutSupportL
// 
// Checks ScreenSaver timeout support
// ----------------------------------------------------------------------------
//
TBool CGSDisplayPluginModel::CheckScreenSaverTimeoutSupportL()
    {
    TInt value = 0;
    User::LeaveIfError( iPersonalizationRepository->Get(
                        KSettingsScreensaverTimeoutItemVisibility, value ) );
    
    if ( value )
        {
        return ETrue;
        }
    else
        {
        return EFalse;
        }
    }


// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::ShowOperatorLogoSettingL
//
// Checks if Operator Logo item is to be made visible.
// ----------------------------------------------------------------------------
//
TBool CGSDisplayPluginModel::ShowOperatorLogoSettingL()
    {
    TInt opLogoSettingState = KGSSettingOff;
    TBool showSetting = EFalse;

    User::LeaveIfError( iPersonalizationRepository->
        Get( KSettingsShowOperatorLogoSetting, opLogoSettingState ) );
    switch( opLogoSettingState )
        {
        case KGSSettingOn:
            showSetting = ETrue;
            break;
        case KGSSettingOff:
            showSetting = EFalse;
        default:
            break;
        }
    return showSetting;
    }


// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::OperatorLogoL
//
// Returns operator logo value from shared data.
// ----------------------------------------------------------------------------
//
TInt CGSDisplayPluginModel::OperatorLogoL()
    {
    TInt opLogo = KGSSettingOff;
    User::LeaveIfError( iPersonalizationRepository->
        Get( KSettingsDisplayOperatorLogo, opLogo ) );
    return opLogo;
    }


// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::SetOperatorLogoL
//
// Sets operator logo value to shared data.
// ----------------------------------------------------------------------------
//
void CGSDisplayPluginModel::SetOperatorLogoL( TInt aOperatorLogo )
    {
    User::LeaveIfError( iPersonalizationRepository->
        Set( KSettingsDisplayOperatorLogo, aOperatorLogo ) );
    }

// ----------------------------------------------------------------------------
// CGSDisplayPluginModel::SetOwner
//
// Sets its owner.
// ----------------------------------------------------------------------------
//
void CGSDisplayPluginModel::SetOwner( CGSDisplayPluginContainer* aContainer)
    {
    iContainer = aContainer;
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

// End of File
