/*
* Copyright (c) 2002, 2003 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  View for Data call sub-folder
*
*/


// INCLUDE FILES
#include "GSDataCallPlugin.h"
#include "GSDataCallPluginModel.h"
#include "GSDataCallPluginContainer.h"
#include "GsDataCallPlugin.hrh"
#include <gsdatacallplugin.mbg> // Icons
#include <gsdatacallpluginrsc.rsg>
#include <gsfwviewuids.h>
#include <gsprivatepluginproviderids.h>

#include <AknQueryDialog.h>
#include <aknradiobuttonsettingpage.h>
#include <aknViewAppUi.h>
#include <featmgr.h>
#include <StringLoader.h>
#include <bautils.h>
#include <coeaui.h>
#include <eikfrlbd.h>
#include <gulicon.h>
#include <hlplch.h>             // For HlpLauncher

#include <gscommon.hrh>

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS
const TInt KGSAutodTimeIndex = 0;

_LIT( KGSDataCallPluginResourceFileName, "z:GSDataCallPluginRsc.rsc" );

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ==============================

// ========================= MEMBER FUNCTIONS ================================

// ----------------------------------------------------------------------------
// CGSDataCallPlugin::CGSDataCallPlugin()
//
// Constructor
// ----------------------------------------------------------------------------
//
CGSDataCallPlugin::CGSDataCallPlugin()
  : iResourceLoader( *iCoeEnv )
    {
    }


// ---------------------------------------------------------------------------
// CGSDataCallPlugin::NewL()
//
// Symbian OS default constructor
// ---------------------------------------------------------------------------
CGSDataCallPlugin* CGSDataCallPlugin::NewL( TAny* /*aInitParams*/ )
    {
    CGSDataCallPlugin* self = new( ELeave ) CGSDataCallPlugin ();

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }



// ---------------------------------------------------------------------------
// CGSDataCallPlugin::ConstructL()
//
// Symbian OS two-phased constructor
// ---------------------------------------------------------------------------
void CGSDataCallPlugin::ConstructL()
    {
    FeatureManager::InitializeLibL();
    // Find the resource file
    TParse parse;
    parse.Set( KGSDataCallPluginResourceFileName,
               &KDC_RESOURCE_FILES_DIR, NULL );
    TFileName fileName( parse.FullName() );

    // Get language of resource file
    BaflUtils::NearestLanguageFile( iCoeEnv->FsSession(), fileName );

    // Open resource file
    iResourceLoader.OpenL( fileName );

    iModel = CGSDataCallPluginModel::NewL();

    BaseConstructL( R_GS_DCALL_VIEW );
    }


// ----------------------------------------------------------------------------
// CGSDataCallPlugin::~CGSDataCallPlugin
//
// Destructor
// ----------------------------------------------------------------------------
CGSDataCallPlugin::~CGSDataCallPlugin()
    {
    FeatureManager::UnInitializeLib();
    iResourceLoader.Close();
    if ( iModel )
        {
        delete iModel;
        }
    }


// ---------------------------------------------------------------------------
// TUid CGSDataCallPlugin::Id()
//
// Returns view's ID.
// ---------------------------------------------------------------------------
TUid CGSDataCallPlugin::Id() const
    {
    return KGSDataCallPluginUid;
    }


// ========================= From CGSPluginInterface ==================

// ----------------------------------------------------------------------------
// CGSDataCallPlugin::GetCaption
//
// Return application/view caption.
// ----------------------------------------------------------------------------
//
void CGSDataCallPlugin::GetCaptionL( TDes& aCaption ) const
    {
    // the resource file is already opened.
    HBufC* result = StringLoader::LoadL( R_GS_DATACALL_VIEW_CAPTION );

    aCaption.Copy( *result );
    delete result;
    }


// ----------------------------------------------------------------------------
// CGSDataCallPlugin::PluginProviderCategory
//
// A means to identify the location of this plug-in in the framework.
// ----------------------------------------------------------------------------
//
TInt CGSDataCallPlugin::PluginProviderCategory() const
    {
    //To identify internal plug-ins.
    return KGSPluginProviderInternal;
    }


// ----------------------------------------------------------------------------
// CGSDataCallPlugin::Visible
//
// Provides the visibility status of self to framework.
// ----------------------------------------------------------------------------
//
TBool CGSDataCallPlugin::Visible() const
    {
    TBool visible = EFalse;

    if ( FeatureManager::FeatureSupported( KFeatureIdAppCsdSupport ) )
        {
        visible = ETrue;
        }

    return visible;
    }


// ---------------------------------------------------------------------------
// CGSDataCallPlugin::HandleCommandL(TInt aCommand)
//
// Handles commands directed to this class.
// ---------------------------------------------------------------------------
void CGSDataCallPlugin::HandleCommandL( TInt aCommand )
    {
    switch ( aCommand )
        {
        case EGSMSKCmdAppChange:
        case EGSCmdAppChange:
            HandleListBoxSelectionL();
            break;
        case EAknSoftkeyBack:
            #ifdef RD_CONTROL_PANEL
                iAppUi->ActivateLocalViewL( KGSAdminPluginUid );
            #else //RD_CONTROL_PANEL
                iAppUi->ActivateLocalViewL( KGSConPluginUid );
            #endif //RD_CONTROL_PANEL
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
// CGSDataCallPlugin::UpdateListBoxL
//
// Updates listbox items.
// ---------------------------------------------------------------------------
//
void CGSDataCallPlugin::UpdateListBoxL( TInt aItemId )
    {
    if( Container() )
        {
        Container()->UpdateListBoxL( aItemId );
        }
    }


// ----------------------------------------------------------------------------
// CGSDataCallPlugin::Container
//
// Return handle to container class.
// ----------------------------------------------------------------------------
//
CGSDataCallPluginContainer* CGSDataCallPlugin::Container()
    {
    return static_cast<CGSDataCallPluginContainer*>( iContainer );
    }


// ---------------------------------------------------------------------------
// CGSDataCallPlugin::NewContainerL()
//
// Creates new iContainer.
// ---------------------------------------------------------------------------
//
void CGSDataCallPlugin::NewContainerL()
    {
    iContainer = new( ELeave ) CGSDataCallPluginContainer;
    }


// ----------------------------------------------------------------------------
// CGSDataCallPlugin::DoActivateL
//
// First method called by the Avkon framwork to invoke a view.
// ----------------------------------------------------------------------------
//
void CGSDataCallPlugin::DoActivateL( const TVwsViewId& aPrevViewId,
                                     TUid aCustomMessageId,
                                     const TDesC8& aCustomMessage )
    {
    CGSBaseView::DoActivateL( aPrevViewId, aCustomMessageId, aCustomMessage );
    }


// ----------------------------------------------------------------------------
// CGSDataCallPlugin::DoDeactivate
//
// Called by the Avkon view framework when closing.
// ----------------------------------------------------------------------------
//
void CGSDataCallPlugin::DoDeactivate()
    {
    CGSBaseView::DoDeactivate();
    }


// ---------------------------------------------------------------------------
// CGSDataCallPlugin::HandleListBoxSelectionL()
//
// Handles events raised through a rocker key.
// ---------------------------------------------------------------------------
void CGSDataCallPlugin::HandleListBoxSelectionL()
    {
    const TInt currentFeatureId = Container()->CurrentFeatureId();

    switch ( currentFeatureId )
        {
        case KGSSettIdAutodiscon:
            ShowDialupAutodisconSettingPageL();
            break;
       default:
            break;
        }
    }


// ---------------------------------------------------------------------------
// CGSDataCallPlugin::HandleSettingPageEventL
//
// Handle any setting page related events
// ---------------------------------------------------------------------------
//
void CGSDataCallPlugin::HandleSettingPageEventL( CAknSettingPage* aSettingPage,
                                              TAknSettingPageEvent aEventType )
    {
    TInt settingId = aSettingPage->SettingId();

    if ( aEventType == EEventSettingOked )
        {
        TInt current;

        switch ( settingId )
            {
            case KGSAutodisconSettingId:
                current =
                  static_cast<CAknRadioButtonSettingPage*> ( aSettingPage )->
                    ListBoxControl()->CurrentItemIndex();
                if ( current == KGSAutodTimeIndex )
                    {
                    ShowAutodisconTimeQueryL();
                    }
                break;
            default:
                break;
            }
        }
    }



// ---------------------------------------------------------------------------
// CGSDataCallPlugin::ShowDialupAutodisconSettingPageL
//
// Display dialup auto disconnect setting page.
// ---------------------------------------------------------------------------
//
void CGSDataCallPlugin::ShowDialupAutodisconSettingPageL()
    {
    TInt value = iModel->AutodisconnectTimeL();
    TInt currentValue = 0; // magic number (user defined)

    if ( value == static_cast<TInt> ( KGSTimeUnlimited ) )
        {
        currentValue = 1;  // magic number (unlimited)
        }

    CDesCArrayFlat* items = iCoeEnv->ReadDesC16ArrayResourceL(
                                     R_AUTODISCON_TIME_SETTING_PAGE_LBX );
    CleanupStack::PushL( items );

    CAknRadioButtonSettingPage* dlg = new( ELeave ) CAknRadioButtonSettingPage(
                                      R_AUTODISCON_TIME_SETTING_PAGE,
                                      currentValue, items );

    dlg->SetSettingId( KGSAutodisconSettingId );
    dlg->SetSettingPageObserver( this );

    if ( dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged ) )
        {
        if ( currentValue == 1 )  // magic number (unlimited)
            {
            iModel->SetAutodisconnectTimeL( KGSTimeUnlimited );
            }

        UpdateListBoxL( KGSSettIdAutodiscon );
        }

    CleanupStack::PopAndDestroy( items );
    }

// ---------------------------------------------------------------------------
// CGSDataCallPlugin::ShowAutodisconTimeQueryL()
//
// Display auto disconnect time query dialog.
// ---------------------------------------------------------------------------
//
void CGSDataCallPlugin::ShowAutodisconTimeQueryL()
    {
    TInt time = iModel->AutodisconnectTimeL();
    if ( time == static_cast<TInt> ( KGSTimeUnlimited ) )
        {
        time = KGSZeroMinutes;
        }

    CAknNumberQueryDialog* dlg = new( ELeave ) CAknNumberQueryDialog( time );
    dlg->PrepareLC( R_AUTODISCON_TIME_NUMBER_QUERY );    
    CAknQueryControl *ctrl = ( CAknQueryControl* )dlg->Control( EGeneralQuery ); 
    CEikEdwin *edwin = ( CEikEdwin* )ctrl->ControlByLayoutOrNull( ENumberLayout );
    edwin->SetAknEditorNumericKeymap( EAknEditorPlainNumberModeKeymap );
    
    if( dlg->RunLD() )
        {
        if ( time == KGSZeroMinutes )
            {
            time = static_cast<TInt> ( KGSTimeUnlimited );
            }

        iModel->SetAutodisconnectTimeL( time );
        }
    }


// -----------------------------------------------------------------------------
// CGSDataCallPlugin::CreateIconL()
//
//
// -----------------------------------------------------------------------------
//
CGulIcon* CGSDataCallPlugin::CreateIconL( const TUid aIconType )
    {
    //EMbm<Mbm_file_name><Bitmap_name>
    CGulIcon* icon;
    TParse* fp = new( ELeave ) TParse();
    CleanupStack::PushL( fp );
    fp->Set( KGSDataCallPluginIconDirAndName, &KDC_BITMAP_DIR, NULL );

    if( aIconType == KGSIconTypeLbxItem )
        {
        icon = AknsUtils::CreateGulIconL(
        AknsUtils::SkinInstance(),
        KAknsIIDQgnPropSetConnData,
        fp->FullName(),
        EMbmGsdatacallpluginQgn_prop_set_conn_data,
        EMbmGsdatacallpluginQgn_prop_set_conn_data_mask );
        }
    else
        {
        icon = CGSPluginInterface::CreateIconL( aIconType );
        }

    CleanupStack::PopAndDestroy( fp );

    return icon;
    }


// -----------------------------------------------------------------------------
// CGSDataCallPlugin::DynInitMenuPaneL()
//
// dynamically handle help item if not supported
// -----------------------------------------------------------------------------
//
void CGSDataCallPlugin::DynInitMenuPaneL( TInt aResourceId, 
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
