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
* Description:  Container for the Display sub-folder
*
*/



// INCLUDE FILES
#include "GSDisplayPluginContainer.h"
#include "GSDisplayPluginModel.h"
#include "GsDisplayPlugin.hrh"
#include "GSDisplayPlugin.h"

#include <bldvariant.hrh>
#include <aknlists.h>
#include <eikfrlb.h>
#include <aknslider.h>
#include <gulicon.h>
#include <StringLoader.h>
#include <featmgr.h>
#include <AknUtils.h>
#include <csxhelp/cp.hlp.hrh>
#include <gsfwviewuids.h>     // for KUidGS
#include <GsDisplayPluginRsc.rsg>
#include <gslistbox.h>
#include <AknsConstants.h>    //for determining skin change
#include <AknDef.hrh>
#include <AknIconArray.h>

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS
const   TInt    KGSBufSize128 = 128;
const   TInt    KGSNumberOfIcons = 3;
const   TInt    KGSContrastIndex = 0;
const   TInt    KGSBrightnessIndex = 1;
const   TInt    KGSAmbientLightSensorIndex = 2;
const   TInt    KGSSliderIndexMin = 0;
const   TInt    KGSBrightnessMax = 31;
const   TInt    KGSALSMax = 5;

// MODULE DATA STRUCTURES
// current ordering is based on the resource structure.
enum TZoomLevel
    {
    EZoomLargeInUi,
    EZoomNormalInUi,
    EZoomSmallInUi
    };

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ==============================

// ========================== MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSDisplayPluginContainer::ConstructL(const TRect& aRect)
// Symbian OS two phased constructor
//
// ---------------------------------------------------------------------------
//
void CGSDisplayPluginContainer::ConstructL( const TRect& aRect )
    {
    iListBox = new ( ELeave ) CAknSettingStyleListBox;
    iModel = CGSDisplayPluginModel::NewL();
    iModel->SetOwner( this );

    BaseConstructL( aRect, R_GS_DISPLAY_VIEW_TITLE, R_DISPL_LBX );
    }

// ---------------------------------------------------------------------------
// CGSDisplayPluginContainer::~CGSDisplayPluginContainer()
//
// Destructor
// ---------------------------------------------------------------------------
//
CGSDisplayPluginContainer::~CGSDisplayPluginContainer()
    {
    if ( FeatureManager::FeatureSupported ( KFeatureIdUiZoom ) )
        {
        delete iDispTxtSizeItems;
        }

    if ( iWelcomeItems )
        {
        delete iWelcomeItems;
        }

    if ( iLedItems )
        {
        delete iLedItems;
        }
    
    if ( iOperatorLogoItems)
        {
        delete iOperatorLogoItems;
        iOperatorLogoItems = NULL;
        }

    if ( iModel )
        {
        delete iModel;
        iModel = NULL;
        }

    if ( iListboxItemArray )
        {
        delete iListboxItemArray;
        }
    }


// ---------------------------------------------------------------------------
// CGSDisplayPluginContainer::CGSDisplayPluginContainer()
//
// Constructor
// ---------------------------------------------------------------------------
//
CGSDisplayPluginContainer::CGSDisplayPluginContainer()
    {
    }

// ---------------------------------------------------------------------------
// CGSDisplayPluginContainer::ConstructListBoxL()
// Destructor
//
// ---------------------------------------------------------------------------
//
void CGSDisplayPluginContainer::ConstructListBoxL( TInt aResLbxId )
    {
    iListBox->ConstructL( this, EAknListBoxSelectionList );
    iListboxItemArray =
        CGSListBoxItemTextArray::NewL( aResLbxId, *iListBox, *iCoeEnv );
    iListBox->Model()->SetItemTextArray( iListboxItemArray );
    iListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );


    if ( FeatureManager::FeatureSupported ( KFeatureIdUiZoom ) )
        {
        iDispTxtSizeItems = iCoeEnv->ReadDesC16ArrayResourceL(
                            R_DISPLAY_TEXT_SIZE_SETTING_PAGE_LBX );
        }

    iWelcomeItems = iCoeEnv->ReadDesC16ArrayResourceL(
                            R_WELCOME_NOTE_SETTING_PAGE_LBX );
    iLedItems = iCoeEnv->ReadDesC16ArrayResourceL(
                            R_POWER_SAVE_LED_SETTING_PAGE_LBX );
    iOperatorLogoItems = iCoeEnv->ReadDesC16ArrayResourceL(
                            R_OPLOGO_ON_OFF_SETTING_PAGE_LBX );


    CArrayPtr<CGulIcon>* icons =
            new( ELeave ) CAknIconArray( KGSNumberOfIcons );
    CleanupStack::PushL( icons );

    CGulIcon* icon = CGulIcon::NewL();
    CleanupStack::PushL( icon );

    CGulIcon* icon2 = CGulIcon::NewL();
    CleanupStack::PushL( icon2 );

    CGulIcon* icon3 = CGulIcon::NewL();
    CleanupStack::PushL( icon3 );

    icons->AppendL( icon );
    icons->AppendL( icon2 );
    icons->AppendL( icon3 );

    CEikFormattedCellListBox* listbox =
              static_cast<CEikFormattedCellListBox*>( iListBox );
    listbox->ItemDrawer()->FormattedCellData()->SetIconArray( icons );
    CleanupStack::Pop( 4, icons ); // icons, icon & icon2 & icon3
    iIconArray = listbox->ItemDrawer()->FormattedCellData()->IconArray();

    CreateListBoxItemsL();
    }

// ---------------------------------------------------------------------------
// CGSDisplayPluginContainer::CreateListBoxItemsL()
//
//
// ---------------------------------------------------------------------------
//
void CGSDisplayPluginContainer::CreateListBoxItemsL()
    {
    if( iModel->ContrastL() != KErrNotSupported )
        {
        MakeContrastItemL();
        }

    TInt pslnStatus = iModel->BackgroundImage()->GetPlnsStatus();

    if ( FeatureManager::FeatureSupported( KFeatureIdBrightnessControl) )
        {
        MakeBrightnessItemL();
        }

    if( iModel->CheckScreenSaverTimeoutSupportL() )
        {
        MakeSSPItemL();
        }

    MakeBacklightItemL();

    if ( FeatureManager::FeatureSupported( KFeatureIdAmbientLightSensor ) )
        {
        MakeAmbientLightSensorItemL();
        }

    if( iModel->IsUWNoteSupportedL() )
        {
        MakeWNIItemL();
        }

    if ( FeatureManager::FeatureSupported ( KFeatureIdUiZoom ) )
        {
        MakeDisplayTextSizeItemL();
        }

    if ( iModel->CheckPowerSaveLedSupportL() )
        {
        MakePowerSaveLedItemL();
        }
    
    if ( iModel->ShowOperatorLogoSettingL() )
        {
        // Make item to show operator logo
        MakeOperatorLogoItemL();
        }
    }

// ---------------------------------------------------------------------------
// CGSDisplayPluginContainer::UpdateListBoxL(TInt aItemId)
//
//
// ---------------------------------------------------------------------------
//
void CGSDisplayPluginContainer::UpdateListBoxL( TInt aFeatureId )
    {
    switch ( aFeatureId )
        {
        case EGSSettIdContrast:
            if( iModel->ContrastL() != KErrNotSupported )
                {
                MakeContrastItemL();
                }
            break;
        case EGSSettIdBrightness:
            if ( FeatureManager::FeatureSupported(
                             KFeatureIdBrightnessControl ) )
                {
                MakeBrightnessItemL();
                }
            break;
        case EGSSettIdSSP:
            if( iModel->CheckScreenSaverTimeoutSupportL() )
                {
                MakeSSPItemL();
                }
            break;
        case EGSSettIdBacklight:
            MakeBacklightItemL();
            break;
        case EGSSettIdAmbientLightSensor:
            if ( FeatureManager::FeatureSupported(
                              KFeatureIdAmbientLightSensor ) )
                {
                MakeAmbientLightSensorItemL();
                }
            break;
        case EGSSettIdWNI:
            if( iModel->IsUWNoteSupportedL() )
                {
                MakeWNIItemL();
                }
            break;
        case EGSSettIdDisplayTextSize:
            if ( FeatureManager::FeatureSupported ( KFeatureIdUiZoom ) )
                {
                MakeDisplayTextSizeItemL();
                }
            break;
        case EGSSettIdPowerSaveLed:
            if ( iModel->CheckPowerSaveLedSupportL() )
                {
                MakePowerSaveLedItemL();
                }
            break;
        case EGSSettIdOperatorLogo:
            if ( iModel->ShowOperatorLogoSettingL() )
                {
                // Update operator logo settings
                MakeOperatorLogoItemL();
                }
        default:
            break;
        }

    iListBox->HandleItemAdditionL();
    }

// ---------------------------------------------------------------------------
// CGSDisplayPluginContainer::MakeContrastItemL()
//
//
// ---------------------------------------------------------------------------
//
void CGSDisplayPluginContainer::MakeContrastItemL()
    {
    TInt currentPos = iModel->ContrastL();

    CreateBitmapL( currentPos, R_CONTRAST_SLIDER, KGSContrastIndex );

    // And add to listbox
    iListboxItemArray->SetItemVisibilityL( EGSSettIdContrast,
        CGSListBoxItemTextArray::EVisible );
    }


// ---------------------------------------------------------------------------
// CGSDisplayPluginContainer::MakeBrightnessItemL()
//
//
// ---------------------------------------------------------------------------
//
void CGSDisplayPluginContainer::MakeBrightnessItemL()
    {
    TInt currentPos = iModel->BrightnessL();

    if ( currentPos > KGSSliderIndexMin &&
         currentPos <= KGSBrightnessMax )
        {
        CreateBitmapL( currentPos, R_BRIGHTNESS_SLIDER, KGSBrightnessIndex );

        // And add to listbox
        iListboxItemArray->SetItemVisibilityL( EGSSettIdBrightness,
                           CGSListBoxItemTextArray::EVisible );
        }
    }


// ---------------------------------------------------------------------------
// CGSDisplayPluginContainer::MakeAmbientLightSensorItemL()
//
//
// ---------------------------------------------------------------------------
//
void CGSDisplayPluginContainer::MakeAmbientLightSensorItemL()
    {
    TInt currentPos = iModel->AmbientLightSensorL();

    if ( currentPos > KGSSliderIndexMin &&
         currentPos <= KGSALSMax )
        {
        CreateBitmapL( currentPos, R_AMBIENT_LIGHT_SENSOR_SLIDER,
                       KGSAmbientLightSensorIndex );

        // And add to listbox
        iListboxItemArray->SetItemVisibilityL( EGSSettIdAmbientLightSensor,
                           CGSListBoxItemTextArray::EVisible );

        }
    }


// ---------------------------------------------------------------------------
// CGSDisplayPluginContainer::MakeSSPItemL()
//
//
// ---------------------------------------------------------------------------
//
void CGSDisplayPluginContainer::MakeSSPItemL()
    {
    TInt timeOut = iModel->ScreenSaverAndKeyguardPeriodL();
    HBufC* dynamicText = CAknSlider::CreateValueTextInHBufCL( timeOut,
                                     R_SETTING_SCREEN_SAVER_SLIDER );

    // for A&H number conversion
    TPtr bufPtr = dynamicText->Des();
    if( AknTextUtils::DigitModeQuery( AknTextUtils::EDigitModeShownToUser ) )
        {
        AknTextUtils::LanguageSpecificNumberConversion( bufPtr );
        }

    // Finally, set the dynamic text
    iListboxItemArray->SetDynamicTextL( EGSSettIdSSP, dynamicText->Des() );

    // And add to listbox
    iListboxItemArray->SetItemVisibilityL( EGSSettIdSSP,
                       CGSListBoxItemTextArray::EVisible );

    delete dynamicText;
    }


// ---------------------------------------------------------------------------
// CGSDisplayPluginContainer::MakeBackLightItemL()
//
// Backlight timeout item
// ---------------------------------------------------------------------------
//
void CGSDisplayPluginContainer::MakeBacklightItemL()
    {
    TInt timeOut = iModel->BacklightPeriodL();
    HBufC* dynamicText = CAknSlider::CreateValueTextInHBufCL( timeOut,
                                     R_SETTING_BACKLIGHT_SLIDER );

    // for A&H number conversion
    TPtr bufPtr = dynamicText->Des();
    if( AknTextUtils::DigitModeQuery( AknTextUtils::EDigitModeShownToUser ) )
        {
        AknTextUtils::LanguageSpecificNumberConversion( bufPtr );
        }

    // Finally, set the dynamic text
    iListboxItemArray->SetDynamicTextL( EGSSettIdBacklight,
                                        dynamicText->Des() );

    // And add to listbox
    iListboxItemArray->SetItemVisibilityL( EGSSettIdBacklight,
                       CGSListBoxItemTextArray::EVisible );

    delete dynamicText;
    }


// ---------------------------------------------------------------------------
// CGSDisplayPluginContainer::MakeDisplayTextSizeItemL()
//
// Display text size item
// ---------------------------------------------------------------------------
//
void CGSDisplayPluginContainer::MakeDisplayTextSizeItemL()
    {
    HBufC* dynamicText = HBufC::NewLC( KGSBufSize128 );
    TPtr ptrBuffer ( dynamicText->Des() );

    TInt textSize = iModel->DisplayTextSizeL();
    // value is changed through the below method
    MapZoomLevelToUi( textSize );

    ptrBuffer = ( *iDispTxtSizeItems )[ textSize ];

    // Finally, set the dynamic text
    iListboxItemArray->SetDynamicTextL( EGSSettIdDisplayTextSize, ptrBuffer );
    CleanupStack::PopAndDestroy( dynamicText );

    // And add to listbox
    iListboxItemArray->SetItemVisibilityL( EGSSettIdDisplayTextSize,
                       CGSListBoxItemTextArray::EVisible );
    }


// ---------------------------------------------------------------------------
// CGSDisplayPluginContainer::MakeWNIItemL()
//
// Welcome note item
// ---------------------------------------------------------------------------
//
void CGSDisplayPluginContainer::MakeWNIItemL()
    {
    HBufC* dynamicText = HBufC::NewLC( KGSBufSize128 );
    TPtr ptrBuffer ( dynamicText->Des() );

    const TInt welcomeNoteTypeIndex = iModel->WelcomeNoteTypeL();
    ptrBuffer = ( *iWelcomeItems )[ welcomeNoteTypeIndex ];

    // Finally, set the dynamic text
    iListboxItemArray->SetDynamicTextL( EGSSettIdWNI, ptrBuffer );
    CleanupStack::PopAndDestroy( dynamicText );

    // And add to listbox
    iListboxItemArray->SetItemVisibilityL( EGSSettIdWNI,
                       CGSListBoxItemTextArray::EVisible );
    }


// ---------------------------------------------------------------------------
// CGSDisplayPluginContainer::MakePowerSaveLedItemL()
//
//
// ---------------------------------------------------------------------------
//
void CGSDisplayPluginContainer::MakePowerSaveLedItemL()
    {

    HBufC* dynamicText = HBufC::NewLC( KGSBufSize128 );
    TPtr ptrBuffer ( dynamicText->Des() );

    TInt objectSel = iModel->PowerSaveLedL();
    ptrBuffer = ( *iLedItems )[ objectSel ];

    // Finally, set the dynamic text
    iListboxItemArray->SetDynamicTextL( EGSSettIdPowerSaveLed, ptrBuffer );
    CleanupStack::PopAndDestroy( dynamicText );

    // And add to listbox
    iListboxItemArray->SetItemVisibilityL( EGSSettIdPowerSaveLed,
        CGSListBoxItemTextArray::EVisible );
    }


// ---------------------------------------------------------------------------
// CGSDisplayPluginContainer::MakeOperatorLogoItemL()
//
//
// ---------------------------------------------------------------------------
//
void CGSDisplayPluginContainer::MakeOperatorLogoItemL()
    {
    HBufC* dynamicText = HBufC::NewLC( KGSBufSize128 );
    TPtr ptrBuffer ( dynamicText->Des() );
    
    TInt selectedItem = iModel->OperatorLogoL();
    ptrBuffer = ( *iOperatorLogoItems )[ 1 - selectedItem ];
    
    iListboxItemArray->SetDynamicTextL( EGSSettIdOperatorLogo, ptrBuffer );
    CleanupStack::PopAndDestroy( dynamicText );

    iListboxItemArray->SetItemVisibilityL( EGSSettIdOperatorLogo, 
            CGSListBoxItemTextArray::EVisible );
    }

// ---------------------------------------------------------------------------
// CGSDisplayPluginContainer::CreateBitmapL()
//
//
// ---------------------------------------------------------------------------
//
void CGSDisplayPluginContainer::CreateBitmapL( TInt aValue, TInt aResourceId,
                                               TInt aIndex )
    {
    CGulIcon* icon =
            CAknSlider::CreateSetStyleListBoxIconL( aValue, aResourceId );

    if( icon )
        {
        delete iIconArray->At( aIndex );
        iIconArray->At( aIndex ) = icon;
        }
    }

// ---------------------------------------------------------------------------
// CGSDisplayPluginContainer::CurrentFeatureId()
//
// ---------------------------------------------------------------------------
//
TInt CGSDisplayPluginContainer::CurrentFeatureId( ) const
    {
    return iListboxItemArray->CurrentFeature( );
    }


// ---------------------------------------------------------------------------
// CGSDisplayPluginContainer::GetHelpContext() const
// Gets Help
//
// ---------------------------------------------------------------------------
//
void CGSDisplayPluginContainer::GetHelpContext( TCoeHelpContext& aContext ) const
    {
    aContext.iMajor = KUidGS;
    aContext.iContext = KCP_HLP_DISPLAY;
    }

// ---------------------------------------------------------------------------
// CGSDisplayPluginContainer::HandleResourceChange()
//
// Handling changing of the skin
// ---------------------------------------------------------------------------
//
void CGSDisplayPluginContainer::HandleResourceChange( TInt aType )
    {
    CCoeControl::HandleResourceChange( aType );
    if ( aType == KAknsMessageSkinChange ||
         aType == KEikDynamicLayoutVariantSwitch )
        {
        TRAP_IGNORE 
            (
            if( iModel->ContrastL() != KErrNotSupported )
                {
                MakeContrastItemL();
                }
            );

        if ( FeatureManager::FeatureSupported( KFeatureIdBrightnessControl) )
            {
            TRAP_IGNORE( MakeBrightnessItemL() );
            }

        if ( FeatureManager::FeatureSupported( KFeatureIdAmbientLightSensor ) )
            {
            TRAP_IGNORE( MakeAmbientLightSensorItemL() );
            }
        }
    if ( aType == KEikDynamicLayoutVariantSwitch )
        {
        TRect mainPaneRect;
        AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EMainPane,
                                           mainPaneRect);
        SetRect( mainPaneRect );
        }
    }

// ---------------------------------------------------------------------------
// CGSDisplayPluginContainer::MapZoomLevelToUi()
//
// Converts the value from Model's Avkon to setting page item index
// ---------------------------------------------------------------------------
//
void CGSDisplayPluginContainer::MapZoomLevelToUi( TInt& aCurrentZoom )
    {
    TInt textSize = EZoomNormalInUi;

    switch( aCurrentZoom )
        {
        case EAknUiZoomSmall:
            textSize = EZoomSmallInUi;
            break;
        case EAknUiZoomLarge:
            textSize = EZoomLargeInUi;
            break;
        case EAknUiZoomNormal:
        default:
            textSize = EZoomNormalInUi;
            break;
        }

    aCurrentZoom = textSize;
    }


// ---------------------------------------------------------------------------
// CGSDisplayPluginContainer::MapZoomLevelFromUi()
//
// Converts the value from setting page item index to Model's Avkon value
// ---------------------------------------------------------------------------
//
void CGSDisplayPluginContainer::MapZoomLevelFromUi ( TInt& aCurrentZoom )
    {
    TInt textSize = 0;

    switch( aCurrentZoom )
        {
        case EZoomSmallInUi:
            textSize = EAknUiZoomSmall;
            break;
        case EZoomLargeInUi:
            textSize = EAknUiZoomLarge;
            break;
        case EZoomNormalInUi:
        default:
            textSize = EAknUiZoomNormal;
            break;
        }

    aCurrentZoom = textSize;
    }

// ---------------------------------------------------------------------------
// CGSDisplayPluginContainer::Model()
//
// Return its member variable iModel.
// ---------------------------------------------------------------------------
//
CGSDisplayPluginModel* CGSDisplayPluginContainer::Model()
    {
    return iModel;
    }

void CGSDisplayPluginContainer::CloseDialog()
	{
	CAknViewAppUi* appUi = static_cast<CAknViewAppUi*>( CCoeEnv::Static()->AppUi() );
	CGSDisplayPlugin* view = static_cast<CGSDisplayPlugin*>( appUi->View(KGSDisplayPluginUid) );
	view->CloseDialog();
	}

// ========================== OTHER EXPORTED FUNCTIONS =========================

// End of File
