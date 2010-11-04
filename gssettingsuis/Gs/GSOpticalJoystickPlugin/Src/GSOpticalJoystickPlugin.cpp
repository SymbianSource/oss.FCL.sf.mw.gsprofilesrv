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
* Description:  View for Optical joystick
*
*/


// INCLUDE FILES
#include "GSOpticalJoystickPlugin.h"
#include "GSOpticalJoystickPluginContainer.h"
#include "GSOpticalJoystickPlugin.hrh"
#include "GsLogger.h"
#include <gsopticaljoystickplugin.mbg> // Icons
#include <gsopticaljoystickpluginrsc.rsg>
#include <gsfwviewuids.h>
#include <gsprivatepluginproviderids.h>

#include <AknQueryDialog.h>
#include <aknradiobuttonsettingpage.h>
#include <aknViewAppUi.h>
#include <featmgr.h>
#include <StringLoader.h>
#include <bautils.h>
#include <coeaui.h>
#include <e32svr.h>
#include <eikfrlbd.h>
#include <gulicon.h>
#include <hlplch.h>             // For HlpLauncher

#include <gscommon.hrh>

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

_LIT( KGSOpticalJoystickPluginResourceFileName, "z:gsopticaljoystickpluginrsc.rsc" );

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ==============================

// ========================= MEMBER FUNCTIONS ================================

// ----------------------------------------------------------------------------
// CGSOpticalJoystickPlugin::CGSOpticalJoystickPlugin()
//
// Constructor
// ----------------------------------------------------------------------------
//
CGSOpticalJoystickPlugin::CGSOpticalJoystickPlugin()
  : iResourceLoader( *iCoeEnv )
    {
    }


// ---------------------------------------------------------------------------
// CGSOpticalJoystickPlugin::NewL()
//
// Symbian OS default constructor
// ---------------------------------------------------------------------------
CGSOpticalJoystickPlugin* CGSOpticalJoystickPlugin::NewL( TAny* /*aInitParams*/ )
    {
    CGSOpticalJoystickPlugin* self = new( ELeave ) CGSOpticalJoystickPlugin ();

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }



// ---------------------------------------------------------------------------
// CGSOpticalJoystickPlugin::ConstructL()
//
// Symbian OS two-phased constructor
// ---------------------------------------------------------------------------
void CGSOpticalJoystickPlugin::ConstructL()
    {
    FeatureManager::InitializeLibL();
    // Find the resource file
    TParse parse;
    parse.Set( KGSOpticalJoystickPluginResourceFileName,
               &KDC_RESOURCE_FILES_DIR, NULL );
    TFileName fileName( parse.FullName() );

    // Get language of resource file
    BaflUtils::NearestLanguageFile( iCoeEnv->FsSession(), fileName );

    // Open resource file
    iResourceLoader.OpenL( fileName );

    BaseConstructL( R_GS_OPTICALJOYSTICK_VIEW );
    }


// ----------------------------------------------------------------------------
// CGSOpticalJoystickPlugin::~CGSOpticalJoystickPlugin
//
// Destructor
// ----------------------------------------------------------------------------
CGSOpticalJoystickPlugin::~CGSOpticalJoystickPlugin()
    {
    FeatureManager::UnInitializeLib();
    iResourceLoader.Close();
    }


// ---------------------------------------------------------------------------
// TUid CGSOpticalJoystickPlugin::Id()
//
// Returns view's ID.
// ---------------------------------------------------------------------------
TUid CGSOpticalJoystickPlugin::Id() const
    {
    return KGSOpticalJoystickPluginUid;
    }


// ========================= From CGSPluginInterface ==================

// ----------------------------------------------------------------------------
// CGSOpticalJoystickPlugin::GetCaption
//
// Return application/view caption.
// ----------------------------------------------------------------------------
//
void CGSOpticalJoystickPlugin::GetCaptionL( TDes& aCaption ) const
    {
    // the resource file is already opened.
    HBufC* result = StringLoader::LoadL( R_GS_OPTICALJOYSTICK_VIEW_CAPTION );

    aCaption.Copy( *result );
    delete result;
    }


// ----------------------------------------------------------------------------
// CGSOpticalJoystickPlugin::PluginProviderCategory
//
// A means to identify the location of this plug-in in the framework.
// ----------------------------------------------------------------------------
//
TInt CGSOpticalJoystickPlugin::PluginProviderCategory() const
    {
    //To identify internal plug-ins.
    return KGSPluginProviderInternal;
    }


// ----------------------------------------------------------------------------
// CGSOpticalJoystickPlugin::Visible
//
// Provides the visibility status of self to framework.
// ----------------------------------------------------------------------------
//
TBool CGSOpticalJoystickPlugin::Visible() const
    {
    if( FeatureManager::FeatureSupported( 193 /* KFeatureIdFfOfnFeature */) )
        {
        return ETrue;
        }
    else
        {
        return EFalse;
        }
    }


// ---------------------------------------------------------------------------
// CGSOpticalJoystickPlugin::HandleCommandL(TInt aCommand)
//
// Handles commands directed to this class.
// ---------------------------------------------------------------------------
void CGSOpticalJoystickPlugin::HandleCommandL( TInt aCommand )
    {
    switch ( aCommand )
        {
        case EGSMSKCmdAppChange:
        case EGSCmdAppChange:
            HandleListBoxSelectionL();
            break;
        case EAknSoftkeyBack:
            iAppUi->ActivateLocalViewL( KGSDeviceManagementPluginUid );
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


// ---------------------------------------------------------------------------
// CGSOpticalJoystickPlugin::UpdateListBoxL
//
// Updates listbox items.
// ---------------------------------------------------------------------------
//
void CGSOpticalJoystickPlugin::UpdateListBoxL( TInt aItemId )
    {
    if( Container() )
        {
        Container()->UpdateListBoxL( aItemId );
        }
    }


// ----------------------------------------------------------------------------
// CGSOpticalJoystickPlugin::Container
//
// Return handle to container class.
// ----------------------------------------------------------------------------
//
CGSOpticalJoystickPluginContainer* CGSOpticalJoystickPlugin::Container()
    {
    return static_cast<CGSOpticalJoystickPluginContainer*>( iContainer );
    }


// ---------------------------------------------------------------------------
// CGSOpticalJoystickPlugin::NewContainerL()
//
// Creates new iContainer.
// ---------------------------------------------------------------------------
//
void CGSOpticalJoystickPlugin::NewContainerL()
    {
    iContainer = new( ELeave ) CGSOpticalJoystickPluginContainer;
    }


// ----------------------------------------------------------------------------
// CGSOpticalJoystickPlugin::DoActivateL
//
// First method called by the Avkon framwork to invoke a view.
// ----------------------------------------------------------------------------
//
void CGSOpticalJoystickPlugin::DoActivateL( const TVwsViewId& aPrevViewId,
                                     TUid aCustomMessageId,
                                     const TDesC8& aCustomMessage )
    {
    CGSBaseView::DoActivateL( aPrevViewId, aCustomMessageId, aCustomMessage );
    }


// ----------------------------------------------------------------------------
// CGSOpticalJoystickPlugin::DoDeactivate
//
// Called by the Avkon view framework when closing.
// ----------------------------------------------------------------------------
//
void CGSOpticalJoystickPlugin::DoDeactivate()
    {
    CGSBaseView::DoDeactivate();
    }


// ---------------------------------------------------------------------------
// CGSOpticalJoystickPlugin::HandleListBoxSelectionL()
//
// Handles events raised through a rocker key.
// ---------------------------------------------------------------------------
void CGSOpticalJoystickPlugin::HandleListBoxSelectionL()
    {
    const TInt currentFeatureId = Container()->CurrentFeatureId();

    switch ( currentFeatureId )
        {
        case KGSSettIdOptJoyst:
            ShowSettingPageL();
            break;
       default:
            break;
        }
    }


// ---------------------------------------------------------------------------
// CGSOpticalJoystickPlugin::HandleSettingPageEventL
//
// Handle any setting page related events
// ---------------------------------------------------------------------------
//
void CGSOpticalJoystickPlugin::HandleSettingPageEventL( CAknSettingPage* /*aSettingPage*/,
                                              TAknSettingPageEvent /*aEventType*/ )
    {
    }

// ---------------------------------------------------------------------------
// CGSOpticalJoystickPlugin::ShowDialupAutodisconSettingPageL
//
// Display dialup auto disconnect setting page.
// ---------------------------------------------------------------------------
//
void CGSOpticalJoystickPlugin::ShowSettingPageL()
    {
    TInt currentValue; // This value should be got from Hardware
    TInt err = 0;
    
    Container()->GetOpticalJoystickStatus( currentValue );
    
    __GSLOGSTRING1("[CGSOpticalJoystickPlugin]--> ShowSettingPageL(), value: %d", currentValue );

    CDesCArrayFlat* items = iCoeEnv->ReadDesC16ArrayResourceL(
                                     R_OPTICALJOYSTICK_SETTING_PAGE_LBX );
    CleanupStack::PushL( items );

    CAknRadioButtonSettingPage* dlg = new( ELeave ) CAknRadioButtonSettingPage(
                                      R_OPTICALJOYSTICK_SETTING_PAGE,
                                      currentValue, items );

    dlg->SetSettingId( KGSOpticalJoystickSettingId );
    dlg->SetSettingPageObserver( this );

    if ( dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged ) )
        {
        Container()->SetOpticalJoystickStatusL( currentValue );

        __GSLOGSTRING1("[CGSOpticalJoystickPlugin]--> ShowSettingPageL(), value: %d", currentValue);

        UpdateListBoxL( KGSSettIdOptJoyst );
        }

    CleanupStack::PopAndDestroy( items );
    }

// -----------------------------------------------------------------------------
// CGSOpticalJoystickPlugin::CreateIconL()
//
//
// -----------------------------------------------------------------------------
//
CGulIcon* CGSOpticalJoystickPlugin::CreateIconL( const TUid aIconType )
    {
    //EMbm<Mbm_file_name><Bitmap_name>
    CGulIcon* icon;
    TParse* fp = new( ELeave ) TParse();
    CleanupStack::PushL( fp );
    fp->Set( KGSOpticalJoystickPluginIconDirAndName, &KDC_BITMAP_DIR, NULL );

    if( aIconType == KGSIconTypeLbxItem )
        {
        icon = AknsUtils::CreateGulIconL(
        AknsUtils::SkinInstance(),
        KAknsIIDQgnPropCpOptiJoystick,
        fp->FullName(),
        EMbmGsopticaljoystickpluginQgn_prop_cp_opti_joystick,
        EMbmGsopticaljoystickpluginQgn_prop_cp_opti_joystick_mask );
        }
    else
        {
        icon = CGSPluginInterface::CreateIconL( aIconType );
        }

    CleanupStack::PopAndDestroy( fp );

    return icon;
    }


// -----------------------------------------------------------------------------
// CGSOpticalJoystickPlugin::DynInitMenuPaneL()
//
// dynamically handle help item if not supported
// -----------------------------------------------------------------------------
//
void CGSOpticalJoystickPlugin::DynInitMenuPaneL( TInt aResourceId, 
                                          CEikMenuPane* aMenuPane )
    {   
    if( aResourceId == R_GS_MENU_ITEM_HELP )
        {
        User::LeaveIfNull( aMenuPane );
        
        if ( FeatureManager::FeatureSupported( KFeatureIdHelp ))
            {
            aMenuPane->SetItemDimmed( EAknCmdHelp, EFalse );
            }
        else
            {
            aMenuPane->SetItemDimmed( EAknCmdHelp, ETrue );
            }
        }
    }

// End of File
