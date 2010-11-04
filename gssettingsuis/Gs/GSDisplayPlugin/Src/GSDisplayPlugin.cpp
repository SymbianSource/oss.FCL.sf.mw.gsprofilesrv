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
* Description:  View for Display sub-folder
*
*/


// INCLUDE FILES
#include "GSDisplayPlugin.h"

#ifdef FF_POWER_SAVE
    #include "GSDisplayPluginAO.h"
#endif // FF_POWER_SAVE

#include "GSDisplayPluginModel.h"
#include "GSDisplayPluginContainer.h"
#include "GsDisplayPlugin.hrh"
#include "GsLogger.h"

#include <coeaui.h>
#include <hlplch.h>             // For HlpLauncher
#include <gulicon.h>
#include <bautils.h>
#include <featmgr.h>
#include <StringLoader.h>
#include <aknnotedialog.h>
#include <AknWaitDialog.h>
#include <aknslider.h>
#include <aknradiobuttonsettingpage.h>
#include <aknslidersettingpage.h>
#include <aknnotewrappers.h>
#include <aknViewAppUi.h>
#include <AknsWallpaperUtils.h>
#include <AknWaitDialog.h>
#include <akntextsettingpage.h>
#include <telephonydomainpskeys.h>
#include <imageconversion.h>

#include <MGFetch.h>            // Image selection
#include <DRMCommon.h>
#include <gsdisplaypluginrsc.rsg>
#include <gsprivatepluginproviderids.h>
#include <gsmainview.h>
#include <gsbasecontainer.h>
#include <gscommon.hrh>
#include <gsdisplayplugin.mbg> // Icons

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES

// CONSTANTS
const TInt KGSImageWelcomeNote = 2;
// MACROS

// LOCAL CONSTANTS AND MACROS
//_LIT( KGSWelcomeNoteTextValue, "" );
//_LIT( KGSScreenSaverTextValue, "" );

_LIT( KGSDisplayPluginResourceFileName, "z:gsdisplaypluginrsc.rsc" );

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ==============================

// ========================= MEMBER FUNCTIONS =================================

// ----------------------------------------------------------------------------
// CGSDisplayPlugin::CGSDisplayPlugin()
//
// Constructor
// ----------------------------------------------------------------------------
//
CGSDisplayPlugin::CGSDisplayPlugin()
  : iUpdateSSText( EFalse ),
    iUpdateWNote( ETrue ),
    iReshowOprtSetPage( EFalse ),
    iResourceLoader( *iCoeEnv )
    {
    }


// ----------------------------------------------------------------------------
// CGSDisplayPlugin::NewL()
//
// Symbian OS default constructor
// ----------------------------------------------------------------------------
CGSDisplayPlugin* CGSDisplayPlugin::NewL( TAny* /*aInitParams*/ )
    {
    CGSDisplayPlugin* self = new( ELeave ) CGSDisplayPlugin();

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }


// ----------------------------------------------------------------------------
// CGSDisplayPlugin::ConstructL()
//
// Symbian OS two-phased constructor
// ----------------------------------------------------------------------------
void CGSDisplayPlugin::ConstructL()
    {
    FeatureManager::InitializeLibL();

    // Find the resource file
    TParse parse;
    parse.Set( KGSDisplayPluginResourceFileName,
               &KDC_RESOURCE_FILES_DIR, NULL );
    TFileName fileName( parse.FullName() );

    // Get language of resource file
    BaflUtils::NearestLanguageFile( iCoeEnv->FsSession(), fileName );

    // Open resource file
    iResourceLoader.OpenL( fileName );

    BaseConstructL( R_GS_DISPL_VIEW );

#ifdef FF_POWER_SAVE    
    iPsmActive = CGSDisplayPluginAO::NewL();
    iPsmActive->SetView( this );
#endif // FF_POWER_SAVE    
    }


// ----------------------------------------------------------------------------
// CGSDisplayPlugin::~CGSDisplayPlugin
//
// Destructor
// ----------------------------------------------------------------------------
CGSDisplayPlugin::~CGSDisplayPlugin()
    {
    CloseDialog();
    FeatureManager::UnInitializeLib();
    iResourceLoader.Close();

#ifdef FF_POWER_SAVE
    delete iPsmActive;
#endif // FF_POWER_SAVE
    
    if ( iImageHandler )
        {
        delete iImageHandler;
        }

    if ( iModel )
        {
        iModel = NULL;
        }
    }


// ----------------------------------------------------------------------------
// TUid CGSDisplayPlugin::Id()
//
// Returns plug-in's ID.
// ----------------------------------------------------------------------------
TUid CGSDisplayPlugin::Id() const
    {
    return KGSDisplayPluginUid;
    }

// ----------------------------------------------------------------------------
// CGSDisplayPlugin::HandleCommandL
//
// Handles commands directed to this class.
// ----------------------------------------------------------------------------
void CGSDisplayPlugin::HandleCommandL( TInt aCommand )
    {
    switch ( aCommand )
        {
        case EGSMSKCmdAppChange:
            if ( EGSSettIdOperatorLogo == Container()->CurrentFeatureId() )
                {
                ChangeOperatorLogoSettingL( EFalse );
                break;
                }
        case EGSCmdAppChange:
            HandleListBoxSelectionL();
            break;
        case EAknSoftkeyBack:
            #ifdef RD_CONTROL_PANEL
                iAppUi->ActivateLocalViewL( KGSGenPluginUid );
            #else //RD_CONTROL_PANEL
                iAppUi->ActivateLocalViewL( KGSPrslnPluginUid );
            #endif //RD_CONTROL_PANEL
            break;
        case EGSCmdShowPowerSaveLedSettingPage:
            ShowPowerSaveLedSettingPageL( ETrue );
            break;
        case EAknCmdHelp:
            {
            if( FeatureManager::FeatureSupported( KFeatureIdHelp ) )
                {
                HlpLauncher::LaunchHelpApplicationL(
                    iEikonEnv->WsSession(), iAppUi->AppHelpContextL() );
                }
            break;
            }
        default:
            iAppUi->HandleCommandL( aCommand );
            break;
        }
    }


// ----------------------------------------------------------------------------
// CGSDisplayPlugin::UpdateListBoxL
//
//
// ----------------------------------------------------------------------------
//
void CGSDisplayPlugin::UpdateListBoxL( TInt aItemId )
    {
    if( Container() )
        {
        Container()->UpdateListBoxL( aItemId );
        }
    }


// ----------------------------------------------------------------------------
// CGSDisplayPlugin::Container
//
// Return handle to container class.
// ----------------------------------------------------------------------------
//
CGSDisplayPluginContainer* CGSDisplayPlugin::Container()
    {
    return static_cast<CGSDisplayPluginContainer*>( iContainer );
    }


// ----------------------------------------------------------------------------
// CGSDisplayPlugin::NewContainerL()
// Creates new iContainer.
//
// ----------------------------------------------------------------------------
//
void CGSDisplayPlugin::NewContainerL()
    {
    // container takes a CAknView pointer
    iContainer = new( ELeave ) CGSDisplayPluginContainer( /* *this*/ );
    }


// ----------------------------------------------------------------------------
// CGSDisplayPlugin::DoActivateL
//
// First method called by the Avkon framwork to invoke a view.
// ----------------------------------------------------------------------------
//
void CGSDisplayPlugin::DoActivateL( const TVwsViewId& aPrevViewId,
                                    TUid aCustomMessageId,
                                    const TDesC8& aCustomMessage )
    {
    CGSBaseView::DoActivateL( aPrevViewId, aCustomMessageId, aCustomMessage );
    iModel = Container()->Model();
    }


// ----------------------------------------------------------------------------
// CGSDisplayPlugin::DoDeactivate
//
// Called by the Avkon view framework when closing.
// ----------------------------------------------------------------------------
//
void CGSDisplayPlugin::DoDeactivate()
    {
    CGSBaseView::DoDeactivate();
    }

// ----------------------------------------------------------------------------
// CGSDisplayPlugin::HandleListBoxSelectionL()
//
// Handles events raised through a rocker key
// ----------------------------------------------------------------------------
void CGSDisplayPlugin::HandleListBoxSelectionL()
    {
    const TInt currentFeatureId = Container()->CurrentFeatureId();
    
    switch ( currentFeatureId )
        {
        case EGSSettIdContrast:
            if( iModel->ContrastL() != KErrNotSupported )
                {
#ifdef FF_POWER_SAVE                    
                if ( iPsmActive->Mode() == EPsmsrvModePowerSave )
                    {
                    // If PSM is on, block setting:
                    DisplayBlockNoteL();
                    }
                else
                    {
#endif // FF_POWER_SAVE                    
                    ShowContrastSettingPageL();
#ifdef FF_POWER_SAVE
                    }
#endif // FF_POWER_SAVE
                }

            
            break;
        case EGSSettIdBrightness:
            if( FeatureManager::FeatureSupported( KFeatureIdBrightnessControl) )
                {
#ifdef FF_POWER_SAVE            
                if ( iPsmActive->Mode() == EPsmsrvModePowerSave )
                    {
                    // If PSM is on, block setting:
                    DisplayBlockNoteL();
                    }
                else
                    {
#endif // FF_POWER_SAVE                    
                    ShowBrightnessSettingPageL();
#ifdef FF_POWER_SAVE
                    }
#endif // FF_POWER_SAVE              
                }
            break;
        case EGSSettIdSSP:
#ifdef FF_POWER_SAVE            
            if ( iPsmActive->Mode() == EPsmsrvModePowerSave )
                {
                // If PSM is on, block setting:
                DisplayBlockNoteL();
                }
            else
                {
#endif // FF_POWER_SAVE 
                ShowScreenSaverPeriodSettingPageL();
#ifdef FF_POWER_SAVE
                }
#endif // FF_POWER_SAVE      
            break;
        case EGSSettIdBacklight:
#ifdef FF_POWER_SAVE            
            if ( iPsmActive->Mode() == EPsmsrvModePowerSave )
                {
                // If PSM is on, block setting:
                DisplayBlockNoteL();
                }
            else
                {
#endif // FF_POWER_SAVE                
                ShowBacklightSettingPageL ( );
#ifdef FF_POWER_SAVE
                }
#endif // FF_POWER_SAVE            
            break;
        case EGSSettIdAmbientLightSensor:
            {
#ifdef FF_POWER_SAVE            
            if ( iPsmActive->Mode() == EPsmsrvModePowerSave )
                {
                // If PSM is on, block setting:
                DisplayBlockNoteL();
                }
            else
                {
#endif // FF_POWER_SAVE                    
                ShowAmbientLightSensorSettingPageL();
#ifdef FF_POWER_SAVE
                }
#endif // FF_POWER_SAVE      
            }
            break;
        case EGSSettIdWNI:
            ShowWelcomeNoteSettingPageL();
            break;
        case EGSSettIdDisplayTextSize:
            if ( FeatureManager::FeatureSupported ( KFeatureIdUiZoom ) )
                {
                ShowDisplayTextSizeSettingPageL();
                }
            break;
        case EGSSettIdPowerSaveLed:
            {
#ifdef FF_POWER_SAVE            
            if ( iPsmActive->Mode() == EPsmsrvModePowerSave )
                {
                // If PSM is on, block setting:
                DisplayBlockNoteL();
                }
            else
                {
#endif // FF_POWER_SAVE                    
                ShowPowerSaveLedSettingPageL( EFalse );
#ifdef FF_POWER_SAVE
                }
#endif // FF_POWER_SAVE 
            }
            break;
        case EGSSettIdOperatorLogo:
        	do
        		{
        		iReshowOprtSetPage = EFalse;
        		ChangeOperatorLogoSettingL( ETrue );
        		}
        	while ( iReshowOprtSetPage );
            break;
        default:
            break;
        }
    }

// ========================= From CGSPluginInterface ==================

// ----------------------------------------------------------------------------
// CGSDisplayPlugin::GetCaption
//
// Return application/view caption.
// ----------------------------------------------------------------------------
//
void CGSDisplayPlugin::GetCaptionL( TDes& aCaption ) const
    {
    // the resource file is already opened.
    HBufC* result = StringLoader::LoadL( R_GS_DISPLAY_VIEW_CAPTION );

    aCaption.Copy( *result );
    delete result;
    }


// ----------------------------------------------------------------------------
// CGSDisplayPlugin::PluginProviderCategory
//
// A means to identify the location of this plug-in in the framework.
// ----------------------------------------------------------------------------
//
TInt CGSDisplayPlugin::PluginProviderCategory() const
    {
    //To identify internal plug-ins.
    return KGSPluginProviderInternal;
    }


// ---------------------------------------------------------------------------
// CGSDisplayPlugin::ShowContrastSettingPageL()
//
// Display the contrast setting page.
// ---------------------------------------------------------------------------
//
void CGSDisplayPlugin::ShowContrastSettingPageL()
    {
    TInt currentValue;
    TInt oldValue;
    oldValue = iModel->ContrastL();

    currentValue = oldValue;

    CAknSliderSettingPage* dlg = new( ELeave ) CAknSliderSettingPage(
                                 R_CONTRAST_SETTING_PAGE, currentValue );

    dlg->SetSettingId( KGSContrastSettingId );
    dlg->SetSettingPageObserver( this );

    if ( !dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged ) )
        {
#ifdef FF_POWER_SAVE
        if ( iPsmActive->Mode() != EPsmsrvModePowerSave )
            {
#endif //FF_POWER_SAVE
            iModel->SetContrastL( oldValue );
#ifdef FF_POWER_SAVE 
            }
#endif //FF_POWER_SAVE
        }
    UpdateListBoxL( EGSSettIdContrast );
    }


// ---------------------------------------------------------------------------
// CGSDisplayPlugin::ShowBrightnessSettingPageL()
//
// Display the brightness setting page.
// ---------------------------------------------------------------------------
//
void CGSDisplayPlugin::ShowBrightnessSettingPageL()
    {
    TInt currentValue;
    TInt oldValue = iModel->BrightnessL();

    currentValue = oldValue;

    CAknSliderSettingPage* dlg = new( ELeave ) CAknSliderSettingPage(
                                 R_BRIGHTNESS_SETTING_PAGE, currentValue );

    dlg->SetSettingId( KGSBrightnessSettingId );
    dlg->SetSettingPageObserver( this );

    if ( !dlg->ExecuteLD(CAknSettingPage::EUpdateWhenChanged ) )
        {
#ifdef FF_POWER_SAVE
        if ( iPsmActive->Mode() != EPsmsrvModePowerSave )
            {
#endif //FF_POWER_SAVE
            iModel->SetBrightnessL( oldValue );
#ifdef FF_POWER_SAVE 
            }
#endif //FF_POWER_SAVE
        }
    UpdateListBoxL( EGSSettIdBrightness );
    }

// ---------------------------------------------------------------------------
// CGSDisplayPlugin::ShowAmbientLightSensorSettingPageL()
//
// Display the ambient light sensor setting page.
// ---------------------------------------------------------------------------
//
void CGSDisplayPlugin::ShowAmbientLightSensorSettingPageL()
    {
    TInt currentValue;
    TInt oldValue = iModel->AmbientLightSensorL();

    currentValue = oldValue;

    CAknSliderSettingPage* dlg = new( ELeave ) CAknSliderSettingPage(
        R_AMBIENT_LIGHT_SENSOR_SETTING_PAGE, currentValue );

    dlg->SetSettingId( KGSAmbientLightSensorId );
    dlg->SetSettingPageObserver( this );

    if ( !dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged ) )
        {
#ifdef FF_POWER_SAVE
        if ( iPsmActive->Mode() != EPsmsrvModePowerSave )
            {
#endif //FF_POWER_SAVE
            iModel->SetAmbientLightSensorL( oldValue );
#ifdef FF_POWER_SAVE 
            }
#endif //FF_POWER_SAVE
        }
    UpdateListBoxL( EGSSettIdAmbientLightSensor );
    }


// ---------------------------------------------------------------------------
// CGSDisplayPlugin::ShowScreenSaverPeriodSettingPageL()
//
// Display the screen saver period (time out) setting page.
// ---------------------------------------------------------------------------
//
void CGSDisplayPlugin::ShowScreenSaverPeriodSettingPageL()
    {
    TInt sliderValue = iModel->ScreenSaverAndKeyguardPeriodL();

    CAknSettingPage* dlg = new( ELeave ) CAknSliderSettingPage(
                           R_SCREEN_SAVER_PERIOD_SETTING_PAGE, sliderValue );

    if ( dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged ) )
        {
#ifdef FF_POWER_SAVE         
        if ( iPsmActive->Mode() == EPsmsrvModePowerSave )
            {
            DisplayBlockNoteL();
            }
        else
            {
#endif //FF_POWER_SAVE
            iModel->SetScreenSaverAndKeyguardPeriodL( sliderValue );
#ifdef FF_POWER_SAVE 
            }
#endif //FF_POWER_SAVE
        UpdateListBoxL( EGSSettIdSSP );
        }
    }

// ---------------------------------------------------------------------------
// CGSDisplayPlugin::ShowBacklightSettingPageL()
//
// Display the backlight time-out setting page.
// ---------------------------------------------------------------------------
//
void CGSDisplayPlugin::ShowBacklightSettingPageL()
    {
    TInt sliderValue = iModel->BacklightPeriodL();

    CAknSettingPage* dlg = new( ELeave ) CAknSliderSettingPage(
                           R_BACKLIGHT_PERIOD_SETTING_PAGE, sliderValue );

    if ( dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged ) )
        {
#ifdef FF_POWER_SAVE 
        if ( iPsmActive->Mode() == EPsmsrvModePowerSave )
            {
            DisplayBlockNoteL();
            }
        else
            {
#endif //FF_POWER_SAVE
            iModel->SetBacklightPeriodL( sliderValue );
#ifdef FF_POWER_SAVE 
            }
#endif //FF_POWER_SAVE
        UpdateListBoxL( EGSSettIdBacklight );
        }
    }


// ---------------------------------------------------------------------------
// CGSDisplayPlugin::HandleSettingPageEventL
//
// Handle any setting page related events
// ---------------------------------------------------------------------------
//
void CGSDisplayPlugin::HandleSettingPageEventL( CAknSettingPage* aSettingPage,
                                     TAknSettingPageEvent aEventType )
    {
    TInt id = aSettingPage->SettingId();

    if ( aEventType == EEventSettingChanged )
        {
        switch ( id )
            {
            case KGSContrastSettingId:
                if( iModel->ContrastL() != KErrNotSupported )
                    {
#ifdef FF_POWER_SAVE 
                    if ( iPsmActive->Mode() == EPsmsrvModePowerSave )
                        {
                        DisplayBlockNoteL();
                        static_cast<CAknSliderSettingPage*>( aSettingPage )->
                            SliderControl()->SetValueL( iModel->ContrastL() );
                        }
                    else
                        {
#endif //FF_POWER_SAVE
                        iModel->SetContrastL( static_cast<CAknSliderSettingPage*>
                            ( aSettingPage )->SliderControl()->Value() );
#ifdef FF_POWER_SAVE 
                        }
#endif //FF_POWER_SAVE
                    }
                break;
            case KGSBrightnessSettingId:
                if( FeatureManager::FeatureSupported(
                                    KFeatureIdBrightnessControl) )
                    {
#ifdef FF_POWER_SAVE 
                    if ( iPsmActive->Mode() == EPsmsrvModePowerSave )
                        {
                        DisplayBlockNoteL();
                        static_cast<CAknSliderSettingPage*>( aSettingPage )->
                            SliderControl()->SetValueL( iModel->BrightnessL() );
                        }
                    else
                        {
#endif //FF_POWER_SAVE
                        iModel->SetBrightnessL( static_cast<CAknSliderSettingPage*>
                            ( aSettingPage )->SliderControl()->Value() );
#ifdef FF_POWER_SAVE 
                        }
#endif //FF_POWER_SAVE
                    }
                break;
            case KGSAmbientLightSensorId:
#ifdef FF_POWER_SAVE
                if ( iPsmActive->Mode() == EPsmsrvModePowerSave )
                    {
                    DisplayBlockNoteL();
                    static_cast<CAknSliderSettingPage*>( aSettingPage )->
                        SliderControl()->SetValueL( iModel->AmbientLightSensorL() );
                    }
                else
                    {
#endif //FF_POWER_SAVE
                    iModel->SetAmbientLightSensorL( static_cast<CAknSliderSettingPage*>
                         ( aSettingPage )->SliderControl()->Value() );
#ifdef FF_POWER_SAVE 
                    }
#endif //FF_POWER_SAVE
                break;
            default:
                break;
            }
        }

    if ( aEventType == EEventSettingOked )
        {
        TInt current;

        switch ( id )
            {
            case KGSWelcomeNoteSettingId:
                current =
                   static_cast<CAknRadioButtonSettingPage*> ( aSettingPage )->
                   ListBoxControl()->CurrentItemIndex();
                if ( current == KGSWNTextIndex )
                    {
                    ShowWelcomeNoteTextQueryL();
                    }
                else if ( current == KGSWNImageIndex )
                    {
                    aSettingPage->MakeVisible(EFalse); 
                    ShowWelcomeNoteImageListL();
                    }
                break;
            default:
                break;
            }
        }
    }


// ---------------------------------------------------------------------------
// CGSDisplayPlugin::VerifySelectionL()
//
// An overloaded method from MMGFetchVerifier interface class
// ---------------------------------------------------------------------------
//
TBool CGSDisplayPlugin::VerifySelectionL( const MDesCArray* aSelectedFiles )
    {
    const TBool KGSDrmProtectedContent = ETrue;

    TBool ret = ETrue;
    //
    if ( aSelectedFiles->MdcaCount() == 1 )
        {
        const TPtrC fileName( aSelectedFiles->MdcaPoint( 0 ) );

        // First, check if the selected file is DRM protected
        if  ( ret && CheckDRMProtectionL( fileName ) ==
                    KGSDrmProtectedContent )
            {
            // display the note to user
            // Show information note
            HBufC* prompt = iCoeEnv->AllocReadResourceLC(
                                          R_GS_DRM_NOT_ALLOWED );

            CAknInformationNote* note =
                new( ELeave ) CAknInformationNote( ETrue );
            note->ExecuteLD( *prompt );

            CleanupStack::PopAndDestroy( prompt );
            ret = EFalse;
            }

        // Next, check whether the image header is valid
        if  ( ret )
            {
            CImageDecoder* imageDecoder = NULL;
            TRAPD( err, imageDecoder = CImageDecoder::FileNewL(
                  iCoeEnv->FsSession(), fileName, ContentAccess::EPeek ) );

            if  ( err != KErrNone )
                {
                // Show information note
                HBufC* prompt = iCoeEnv->AllocReadResourceLC(
                                                  R_GS_IMAGE_CORRUPTED );
                CAknInformationNote* note =
                    new( ELeave ) CAknInformationNote( ETrue );
                note->ExecuteLD(*prompt);
                CleanupStack::PopAndDestroy( prompt );

                ret = EFalse;
                }
            delete imageDecoder;
            }
        }

    return ret;
    }


// ---------------------------------------------------------------------------
// CGSDisplayPlugin::ShowWelcomeNoteSettingPageL()
//
// Display welcome note setting page
// ---------------------------------------------------------------------------
//
void CGSDisplayPlugin::ShowWelcomeNoteSettingPageL()
    {
    iUpdateWNote = ETrue;

    CDesCArrayFlat* items = iCoeEnv->ReadDesC16ArrayResourceL(
                                        R_WELCOME_NOTE_SETTING_PAGE_LBX );
    CleanupStack::PushL( items );

    TInt currentItem = iModel->WelcomeNoteTypeL();
    iWelcomeNoteType = currentItem;

    CAknSettingPage* dlg = new( ELeave ) CAknRadioButtonSettingPage(
                                  R_WELCOME_NOTE_SETTING_PAGE,
                                  currentItem, items );

    dlg->SetSettingId( KGSWelcomeNoteSettingId );
    dlg->SetSettingPageObserver(this);
    if ( dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged )
         && iUpdateWNote && currentItem != KGSImageWelcomeNote
         )
        {
        iModel->SetWelcomeNoteTypeL( currentItem );
        UpdateListBoxL( EGSSettIdWNI );
        }

    CleanupStack::PopAndDestroy( items );
    }

// ---------------------------------------------------------------------------
// CGSDisplayPlugin::ShowWelcomeNoteTextQueryL()
//
// Display welcome note text input dialog
// ---------------------------------------------------------------------------
//
void CGSDisplayPlugin::ShowWelcomeNoteTextQueryL()
    {
    TBuf<KGSMaxWNText> note;
    iModel->WelcomeNoteTextL( note );

    CAknTextQueryDialog* dlg = new ( ELeave ) CAknTextQueryDialog( note );
    if( dlg->ExecuteLD( R_WELCOME_NOTE_TEXT_QUERY ) )
        {
        iModel->SetWelcomeNoteTextL( note );
        iUpdateWNote = ETrue;
        }
    else
        {
        iUpdateWNote = EFalse;
        }
   }

// ---------------------------------------------------------------------------
// CGSDisplayPlugin::ShowWelcomeNoteImageListL()
//
// Display welcome note image list from media gallery
// ---------------------------------------------------------------------------
//
void CGSDisplayPlugin::ShowWelcomeNoteImageListL()
    {
    // If the MGFetch already launched, return.
    if ( iLaunchedImageFetch )
        {
        return;
        }
    CDesCArray* selectedFiles = new ( ELeave ) CDesCArrayFlat( 1 );
    CleanupStack::PushL( selectedFiles );

    iLaunchedImageFetch = ETrue;
    TBool ret = MGFetch::RunL( *selectedFiles, EImageFile, EFalse, this );
    iLaunchedImageFetch = EFalse;

    if ( ret && selectedFiles->MdcaCount() == 1 )
        {
        iImageHandler = CGSAsyncImageHandling::NewL( iCoeEnv->FsSession(),
                                               this, KGSWelcomeNoteImgPath );

        TPtrC imagePath = selectedFiles->MdcaPoint( 0 );

        //attempting to create private directory
        RFs fs;
        User::LeaveIfError( fs.Connect() );
        CleanupClosePushL( fs );
        TInt ret = fs.CreatePrivatePath( 2 );
        CleanupStack::PopAndDestroy(); //fs

        TRAPD( error,
               iModel->BackgroundImage()->SetWelcomeNoteImageL( imagePath, *iImageHandler )
             );

        if ( error == KErrNone )
            {
            // Show wait note
            ShowImageLoadWaitNoteL();
            }
        else
            {
            HandleImageErrorsL( error );
            }
        }
    else
        {
        iUpdateWNote = EFalse;
        }

    CleanupStack::PopAndDestroy( selectedFiles );
    }


// ---------------------------------------------------------------------------
// CGSDisplayPlugin::ShowDisplayTextSizeSettingPageL()
//
// Display display text size setting page
// ---------------------------------------------------------------------------
//
void CGSDisplayPlugin::ShowDisplayTextSizeSettingPageL()
    {
    CDesCArrayFlat* items = iCoeEnv->ReadDesC16ArrayResourceL(
                                     R_DISPLAY_TEXT_SIZE_SETTING_PAGE_LBX );
    CleanupStack::PushL( items );

    TInt currentItem = iModel->DisplayTextSizeL();
    // get the UI index from container
    Container()->MapZoomLevelToUi( currentItem );
    // store the current value for comparison
    TInt oldItem = currentItem;

    CAknSettingPage* dlg = new( ELeave ) CAknRadioButtonSettingPage(
                                  R_DISPLAY_TEXT_SIZE_SETTING_PAGE,
                                  currentItem, items );

    if ( dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged ) )
        {
        if( currentItem != oldItem )
            {
            // get the Avkon value from UI index
            Container()->MapZoomLevelFromUi( currentItem );
            // set Avkon value to CenRep. As an additional check, confirm
            // from feature manager that zooming is supported.
            if ( FeatureManager::FeatureSupported ( KFeatureIdUiZoom ) )
                {
                iModel->SetDisplayTextSizeL( currentItem );
                }
             // now inform all open apps of the switch
             // note that app needs software event capability to call this API
            TWsEvent event;
            event.SetType( KEikDynamicLayoutVariantSwitch );
            iEikonEnv->WsSession().SendEventToAllWindowGroups( event );
            }

        UpdateListBoxL( EGSSettIdDisplayTextSize );
        }

    CleanupStack::PopAndDestroy( items );
    }


// ---------------------------------------------------------------------------
// CGSDisplayPlugin::ShowPowerSaveLedSettingPageL()
//
// Display the screen saver object setting page.
// ---------------------------------------------------------------------------
//
void CGSDisplayPlugin::ShowPowerSaveLedSettingPageL( TBool aSettingPage )
    {

    CDesCArrayFlat* items =
        iCoeEnv->ReadDesC16ArrayResourceL( R_POWER_SAVE_LED_SETTING_PAGE_LBX );
    CleanupStack::PushL( items );

    TInt currentItem = iModel->PowerSaveLedL();

    if( aSettingPage )
        {
        CAknRadioButtonSettingPage* dlg =
            new ( ELeave ) CAknRadioButtonSettingPage(
                                   R_POWER_SAVE_LED_SETTING_PAGE,
                                   currentItem, items );

        if ( dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged ) )
            {
            iModel->SetPowerSaveLedL( currentItem );
            UpdateListBoxL( EGSSettIdPowerSaveLed );
            }
        }
    else
        {
        iModel->SetPowerSaveLedL( !currentItem );
        UpdateListBoxL( EGSSettIdPowerSaveLed );
        }
    CleanupStack::PopAndDestroy( items );
    }


// ---------------------------------------------------------------------------
// CGSDisplayPlugin::ImageHandlingComplete()
//
// Method derived from MGSSyncImageHandlingObserver
// Run when the asynchronous image converting is complete
// ---------------------------------------------------------------------------
//
void CGSDisplayPlugin::ImageHandlingCompleteL( TInt aError )
    {
    if ( aError != KErrNone )
        {
        HandleImageErrorsL( aError );
        }
    else
        {
        iModel->SetWelcomeNoteTypeL( KGSImageWelcomeNote );
        UpdateListBoxL( EGSSettIdWNI );
        }
    HideImageLoadWaitNoteL();

    if( iImageHandler )
        {
        delete iImageHandler;
        iImageHandler = NULL;
        }
    }


// ---------------------------------------------------------------------------
// CGSDisplayPlugin::ShowImageLoadWaitNoteL()
//
// Display image load wait note dialog.
// ---------------------------------------------------------------------------
//
void CGSDisplayPlugin::ShowImageLoadWaitNoteL()
    {
    if ( !iWaitDialog )
        {
        iWaitDialog = new( ELeave ) CAknWaitDialog(
                      ( REINTERPRET_CAST(CEikDialog**,
                      &iWaitDialog ) ), ETrue );

        iWaitDialog->ExecuteDlgLD( CAknNoteDialog::ENoTone,
                                   R_IMAGE_LOAD_WAIT_NOTE );
        }
    }

// ---------------------------------------------------------------------------
// CGSDisplayPlugin::HideImageLoadWaitNoteL()
//
// Hide image load wait note dialog.
// ---------------------------------------------------------------------------
//
void CGSDisplayPlugin::HideImageLoadWaitNoteL()
    {
    if ( iWaitDialog )
        {
        iWaitDialog->ProcessFinishedL(); // deletes the dialog
        iWaitDialog = NULL;
        }
    }


// ---------------------------------------------------------------------------
// CGSDisplayPlugin::CheckDRMProtectionL
//
// Check if the selected image file is DRM protected.
// ---------------------------------------------------------------------------
//
TBool CGSDisplayPlugin::CheckDRMProtectionL( const TDesC& aOriginalFileName )
    {
    TBool isProtected( EFalse );
    DRMCommon* drmClient = DRMCommon::NewL();
    CleanupStack::PushL( drmClient );
    TInt error = drmClient->Connect();
    if ( error != DRMCommon::EOk )
        {
        User::Leave( KErrCorrupt );
        }

    if ( aOriginalFileName != KNullDesC )
        {
        error = drmClient->IsProtectedFile( aOriginalFileName, isProtected );
        if ( error != DRMCommon::EOk )
            {
            User::Leave( KErrCorrupt );
            }
        }

    CleanupStack::PopAndDestroy( drmClient );
    return isProtected;
    }

// ---------------------------------------------------------------------------
// CGSDisplayPlugin::HandleImageErrorsL
//
// Prompt image related errors to the user
// ---------------------------------------------------------------------------
//
void CGSDisplayPlugin::HandleImageErrorsL( TInt aError )
    {
    if ( aError )
        {
        TInt resourceId;
        switch( aError )
            {
            case KErrNotSupported:
            case KErrUnderflow:
                // Image is corrupted or in wrong format
                resourceId = R_GS_IMAGE_CORRUPTED;
                break;
            case KErrDiskFull:
            case KErrNoMemory:
                // Image is too large
                resourceId = R_GS_IMAGE_TOO_LARGE;
                break;
            default:
                // Better to give some error message than result in CONE5 panic:
                resourceId = R_GS_IMAGE_CORRUPTED;
                //User::LeaveIfError( aError );
                break;
            }

        // Show information note
        HBufC* prompt = iCoeEnv->AllocReadResourceLC( resourceId );
        CAknInformationNote* note = new( ELeave ) CAknInformationNote( ETrue );
        note->ExecuteLD( *prompt );

        // Restoring previous welcome note value
        iModel->SetWelcomeNoteTypeL( iWelcomeNoteType );
        UpdateListBoxL( EGSSettIdWNI );
        CleanupStack::PopAndDestroy( prompt );
        }
    }


// ----------------------------------------------------------------------------
// CGSDisplayPlugin::DynInitMenuPaneL()
//
// Display the dynamic menu
// ----------------------------------------------------------------------------
void CGSDisplayPlugin::DynInitMenuPaneL( TInt aResourceId,
                                         CEikMenuPane* aMenuPane )
    {
    // show or hide the 'help' menu item when supported
    if( aResourceId == R_GS_MENU_ITEM_HELP )
        {
        User::LeaveIfNull( aMenuPane );

        if ( FeatureManager::FeatureSupported( KFeatureIdHelp ) )
            {
            aMenuPane->SetItemDimmed( EAknCmdHelp, EFalse );
            }
        else
            {
            aMenuPane->SetItemDimmed( EAknCmdHelp, ETrue );
            }
        }
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//
// ----------------------------------------------------------------------------------
// CGSDisplayPlugin::UpdateViewL()
// ----------------------------------------------------------------------------------
//
void CGSDisplayPlugin::UpdateViewL()
    {
    // Check what view is active, update if needed.
    // If displaying main view, update all PSM changed items.
    TVwsViewId activeView;
    CEikonEnv::Static()->EikAppUi()->GetActiveViewId( activeView );
//    if( activeView.iViewUid == Id() )
        {
        __GSLOGSTRING( "[CGSDisplayPlugin] I am active!!!" );        
        // Not too elegant - update only items that are affected by PSM
        UpdateListBoxL( EGSSettIdContrast );
        UpdateListBoxL( EGSSettIdBrightness );
        UpdateListBoxL( EGSSettIdAmbientLightSensor );
//        UpdateListBoxL( EGSSettIdDisplayTextSize );
        UpdateListBoxL( EGSSettIdSSP );
//        UpdateListBoxL( EGSSettIdSSO );
//        UpdateListBoxL( EGSSettIdWNI );
        UpdateListBoxL( EGSSettIdBacklight );
//        UpdateListBoxL( EGSSettIdPowerSaveLed );
        UpdateListBoxL( EGSSettIdOperatorLogo );
        }
    }

#ifdef FF_POWER_SAVE
// ----------------------------------------------------------------------------------
// CGSDisplayPlugin::DisplayBlockNoteL()
// ----------------------------------------------------------------------------------
void CGSDisplayPlugin::DisplayBlockNoteL()
    {
    HBufC* prompt = iCoeEnv->AllocReadResourceLC(
            R_GS_POWER_SAVING_PROTECTED_SETTINGS_INFONOTE );
    CAknInformationNote* note = new ( ELeave ) CAknInformationNote( ETrue );
    note->ExecuteLD( *prompt );
    CleanupStack::PopAndDestroy( prompt );
    }
#endif // FF_POWER_SAVE


// -----------------------------------------------------------------------------
// CGSDisplayPlugin::CreateIconL()
//
//
// -----------------------------------------------------------------------------
//
CGulIcon* CGSDisplayPlugin::CreateIconL( const TUid aIconType )
    {
    //EMbm<Mbm_file_name><Bitmap_name>
    CGulIcon* icon;

    if( aIconType == KGSIconTypeLbxItem )
        {
        icon = AknsUtils::CreateGulIconL(
        AknsUtils::SkinInstance(),
        KAknsIIDQgnPropCpDevDisp,
        KGSDefaultDisplayIconFileName,
        EMbmGsdisplaypluginQgn_prop_cp_dev_disp,
        EMbmGsdisplaypluginQgn_prop_cp_dev_disp_mask );
        }
       else
        {
        icon = CGSPluginInterface::CreateIconL( aIconType );
        }

    return icon;
    }
    

// -----------------------------------------------------------------------------
// CGSDisplayPlugin::ChangeOperatorLogoSettingL()
//
//
// -----------------------------------------------------------------------------
//
void CGSDisplayPlugin::ChangeOperatorLogoSettingL( TBool aUseSettingPage )
    {
    TInt currentValue = iModel->OperatorLogoL();
    TBool isValueUpdated = EFalse;

    if ( aUseSettingPage )
        {
        isValueUpdated = ShowOperatorLogoSettingPageL( currentValue );
        }
    else
        {
        if( currentValue == KGSSettingOff )
            {
            currentValue = KGSSettingOn;
            }
        else
            {
            currentValue = KGSSettingOff;
            }
        isValueUpdated = ETrue;
        }
    // Value has been changed -> store it:
    if ( isValueUpdated )
        {
        iModel->SetOperatorLogoL( currentValue );

        UpdateListBoxL( EGSSettIdOperatorLogo );
        }
    }


// -----------------------------------------------------------------------------
// CGSDisplayPlugin::ShowOperatorLogoSettingPageL()
//
// Note: Lbx index of each value is only internal to this function. The
// parameter given and modified is the actual operator logo value.
// -----------------------------------------------------------------------------
//
TBool CGSDisplayPlugin::ShowOperatorLogoSettingPageL(
    TInt& aOperatorLogoValue )
    {
    TBool isValueUpdated = EFalse;
    TInt previousValue = aOperatorLogoValue;
    TInt selectedIndex;

    // Match aOperatorLogoValue value to lbx item index:
    switch( aOperatorLogoValue )
        {
        case KGSSettingOn:
            selectedIndex = KGSOpLogoLbxIndexOn;
            break;
        case KGSSettingOff:
            selectedIndex = KGSOpLogoLbxIndexOff;
            break;
        }

    CDesCArrayFlat* items =
        iCoeEnv->ReadDesC16ArrayResourceL( R_OPLOGO_ON_OFF_SETTING_PAGE_LBX );
    CleanupStack::PushL( items );

    if ( !iOprtLogoSettingPage )
    	{
    	iOprtLogoSettingPage = new (ELeave) CAknRadioButtonSettingPage(
            R_OPERATOR_LOGO_SETTING_PAGE,
            selectedIndex,
            items );
    	}

    TBool ret = iOprtLogoSettingPage->ExecuteLD( CAknSettingPage::EUpdateWhenChanged );
    iOprtLogoSettingPage = NULL;
    CleanupStack::PopAndDestroy( items );

     // Match selected lbx item index to aOperatorLogoValue:
    switch( selectedIndex )
        {
        case KGSOpLogoLbxIndexOn:
            aOperatorLogoValue = KGSSettingOn;
            break;
        case KGSOpLogoLbxIndexOff:
            aOperatorLogoValue = KGSSettingOff;
            break;
        }

    if( aOperatorLogoValue != previousValue )
        {
        isValueUpdated = ETrue;
        }
    return isValueUpdated;
    }

// -----------------------------------------------------------------------------
// CGSDisplayPlugin::CloseDialog
//
//
// -----------------------------------------------------------------------------
//
void CGSDisplayPlugin::CloseDialog()
    {
    //Send ESC key sequence to setting dialog, so that it closes itself.
    if ( iOprtLogoSettingPage )
        {
        iReshowOprtSetPage = ETrue;
        TKeyEvent event;
        event.iCode = EKeyEscape;
        event.iScanCode = EStdKeyEscape;
        event.iRepeats = 0;
        TRAP_IGNORE( iCoeEnv->SimulateKeyEventL( event, EEventKeyDown );
                     iCoeEnv->SimulateKeyEventL( event, EEventKey );
                     iCoeEnv->SimulateKeyEventL( event, EEventKeyUp ); );
        }
    }
// End of File


