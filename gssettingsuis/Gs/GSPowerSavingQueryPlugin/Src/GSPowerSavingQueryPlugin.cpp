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
* Description:  View for Power saving query
*
*/


// INCLUDE FILES
#include "GSPowerSavingQueryPlugin.h"
#include "GSPowerSavingQueryPluginContainer.h"
#include "GSPowerSavingQueryPluginModel.h"
#include "GSPowerSavingQueryPlugin.hrh"
#include "GsLogger.h"
#include <GSPowerSavingQueryPlugin.mbg> // icons
#include <gspowersavingquerypluginrsc.rsg>
#include <gsfwviewuids.h>
#include <gsprivatepluginproviderids.h>
#include <aknradiobuttonsettingpage.h>
#include <aknViewAppUi.h>
#include <featmgr.h>
#include <StringLoader.h>
#include <bautils.h>
#include <coeaui.h>
#include <gulicon.h>
#include <hlplch.h>             // for hlplauncher
#include <gscommon.hrh>

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES

// CONSTANTS
const TInt KGSSettingOff = 0;
const TInt KGSSettingOn  = 1;
const TInt KGSIndexOff   = 1;
const TInt KGSIndexOn    = 0;

// MACROS

// LOCAL CONSTANTS AND MACROS

_LIT( KGSPowerSavingQueryPluginResourceFileName, "z:GSPowerSavingQueryPluginRsc.rsc" );

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ==============================

// ========================= MEMBER FUNCTIONS ================================

// ----------------------------------------------------------------------------
// CGSPowerSavingQueryPlugin::CGSPowerSavingQueryPlugin()
//
// Constructor
// ----------------------------------------------------------------------------
//
CGSPowerSavingQueryPlugin::CGSPowerSavingQueryPlugin()
  : iResourceLoader( *iCoeEnv )
    {
    }


// ---------------------------------------------------------------------------
// CGSPowerSavingQueryPlugin::NewL()
//
// Symbian OS default constructor
// ---------------------------------------------------------------------------
CGSPowerSavingQueryPlugin* CGSPowerSavingQueryPlugin::NewL( TAny* /*aInitParams*/ )
    {
    CGSPowerSavingQueryPlugin* self = new( ELeave ) CGSPowerSavingQueryPlugin();

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }



// ---------------------------------------------------------------------------
// CGSPowerSavingQueryPlugin::ConstructL()
//
// Symbian OS two-phased constructor
// ---------------------------------------------------------------------------
void CGSPowerSavingQueryPlugin::ConstructL()
    {
    FeatureManager::InitializeLibL();
    // Find the resource file
    TParse parse;
    parse.Set( KGSPowerSavingQueryPluginResourceFileName,
               &KDC_RESOURCE_FILES_DIR, NULL );
    TFileName fileName( parse.FullName() );

    // Get language of resource file
    BaflUtils::NearestLanguageFile( iCoeEnv->FsSession(), fileName );

    // Open resource file
    iResourceLoader.OpenL( fileName );

    BaseConstructL( R_GS_POWER_SAVING_QUERY_VIEW );
    }


// ----------------------------------------------------------------------------
// CGSPowerSavingQueryPlugin::~CGSPowerSavingQueryPlugin
//
// Destructor
// ----------------------------------------------------------------------------
CGSPowerSavingQueryPlugin::~CGSPowerSavingQueryPlugin()
    {
    FeatureManager::UnInitializeLib();
    iResourceLoader.Close();
    }


// ---------------------------------------------------------------------------
// TUid CGSPowerSavingQueryPlugin::Id()
//
// Returns view's ID.
// ---------------------------------------------------------------------------
TUid CGSPowerSavingQueryPlugin::Id() const
    {
    return KGSPowerSavingQueryPluginUid;
    }


// ========================= From CGSPluginInterface ==================

// ----------------------------------------------------------------------------
// CGSPowerSavingQueryPlugin::GetCaption
//
// Return application/view caption.
// ----------------------------------------------------------------------------
//
void CGSPowerSavingQueryPlugin::GetCaptionL( TDes& aCaption ) const
    {
    // the resource file is already opened.
    HBufC* result = StringLoader::LoadL( R_GS_POWER_SAVING_QUERY_VIEW_CAPTION );

    aCaption.Copy( *result );
    delete result;
    }


// ----------------------------------------------------------------------------
// CGSPowerSavingQueryPlugin::PluginProviderCategory
//
// A means to identify the location of this plug-in in the framework.
// ----------------------------------------------------------------------------
//
TInt CGSPowerSavingQueryPlugin::PluginProviderCategory() const
    {
    //To identify internal plug-ins.
    return KGSPluginProviderInternal;
    }


// ----------------------------------------------------------------------------
// CGSPowerSavingQueryPlugin::Visible
//
// Provides the visibility status of self to framework.
// ----------------------------------------------------------------------------
//
TBool CGSPowerSavingQueryPlugin::Visible() const
    {
    return ETrue;
    }


// ---------------------------------------------------------------------------
// CGSPowerSavingQueryPlugin::HandleCommandL(TInt aCommand)
//
// Handles commands directed to this class.
// ---------------------------------------------------------------------------
void CGSPowerSavingQueryPlugin::HandleCommandL( TInt aCommand )
    {
    switch ( aCommand )
        {
        case EGSMSKCmdAppChange:
            HandleListBoxSelectionL();
            break;
        case EGSCmdAppChange:
            ShowSettingPageL();
            break;
        case EAknSoftkeyBack:
            iAppUi->ActivateLocalViewL( KGSDeviceManagementPluginUid );
            break;
        default:
            iAppUi->HandleCommandL( aCommand );
            break;
        }
    }


// ---------------------------------------------------------------------------
// CGSPowerSavingQueryPlugin::UpdateListBoxL
//
// Updates listbox items.
// ---------------------------------------------------------------------------
//
void CGSPowerSavingQueryPlugin::UpdateListBoxL( TInt aItemId )
    {
    if( Container() )
        {
        Container()->UpdateListBoxL( aItemId );
        }
    }


// ----------------------------------------------------------------------------
// CGSPowerSavingQueryPlugin::Container
//
// Return handle to container class.
// ----------------------------------------------------------------------------
//
CGSPowerSavingQueryPluginContainer* CGSPowerSavingQueryPlugin::Container()
    {
    return static_cast<CGSPowerSavingQueryPluginContainer*>( iContainer );
    }


// ---------------------------------------------------------------------------
// CGSPowerSavingQueryPlugin::NewContainerL()
//
// Creates new iContainer.
// ---------------------------------------------------------------------------
//
void CGSPowerSavingQueryPlugin::NewContainerL()
    {
    iContainer = new( ELeave ) CGSPowerSavingQueryPluginContainer;
    }


// ---------------------------------------------------------------------------
// CGSPowerSavingQueryPlugin::DoActivateL()
//
// From CAknView 
// ---------------------------------------------------------------------------
//
void CGSPowerSavingQueryPlugin::DoActivateL( const TVwsViewId& aPrevViewId,
                  TUid aCustomMessageId,
                  const TDesC8& aCustomMessage )
    {
    __GSLOGSTRING("[CGSPowerSavingQueryPlugin]-->CGSPowerSavingQueryPlugin::DoActivateL");
    
    CGSBaseView::DoActivateL( aPrevViewId, aCustomMessageId, aCustomMessage );
    
    __GSLOGSTRING("[CGSPowerSavingQueryPlugin]<--CGSPowerSavingQueryPlugin::DoActivateL");
    }

// ---------------------------------------------------------------------------
// CGSPowerSavingQueryPlugin::DoActivateL()
//
// From CAknView
// ---------------------------------------------------------------------------
//
void CGSPowerSavingQueryPlugin::DoDeactivate()
    {
    CGSBaseView::DoDeactivate();
    }

// ---------------------------------------------------------------------------
// CGSPowerSavingQueryPlugin::HandleListBoxSelectionL()
//
// Handles events raised through a rocker key.
// ---------------------------------------------------------------------------
void CGSPowerSavingQueryPlugin::HandleListBoxSelectionL()
    {
    CGSPowerSavingQueryPluginModel* model = Container()->Model();
    
    TInt queryValue( model->PowerSavingQueryEnabled() );
    
    if ( EGSPowerSavingQueryOn == queryValue ) 
        {
        queryValue = EGSPowerSavingQueryOff;
        }
    else
        {
        queryValue = EGSPowerSavingQueryOn;
        }
    
    model->EnablePowerSavingQuery( queryValue );
    
    UpdateListBoxL( KGSSettIdPowerSavingQuery );
    }



// ---------------------------------------------------------------------------
// CGSPowerSavingQueryPlugin::ShowDialupAutodisconSettingPageL
//
// Display dialup auto disconnect setting page.
// ---------------------------------------------------------------------------
//
void CGSPowerSavingQueryPlugin::ShowSettingPageL()
    {
    TBool queryValue = Container()->Model()->PowerSavingQueryEnabled();
    
    SwitchOnOffValue( queryValue );

    CDesCArrayFlat* items = iCoeEnv->ReadDesC16ArrayResourceL(
            R_POWER_SAVING_QUERY_SETTING_PAGE_LBX );
    CleanupStack::PushL( items );

    CAknRadioButtonSettingPage* dlg = new( ELeave ) CAknRadioButtonSettingPage(
            R_POWER_SAVING_QUERY_SETTING_PAGE, queryValue, items );

    if ( dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged ) )
        {
        SwitchOnOffValue( queryValue );
        Container()->Model()->EnablePowerSavingQuery( queryValue );
        UpdateListBoxL( KGSSettIdPowerSavingQuery );
        }

    CleanupStack::PopAndDestroy( items );
    }

// -----------------------------------------------------------------------------
// CGSPowerSavingQueryPlugin::CreateIconL()
//
//
// -----------------------------------------------------------------------------
//
CGulIcon* CGSPowerSavingQueryPlugin::CreateIconL( const TUid aIconType )
    {
    CGulIcon* icon;
    TParse* fp = new( ELeave ) TParse();
    CleanupStack::PushL( fp );
    fp->Set( KGSPowerSavingQueryPluginIconDirAndName, &KDC_BITMAP_DIR, NULL );

    if( aIconType == KGSIconTypeLbxItem )
        {
        icon = AknsUtils::CreateGulIconL(
        AknsUtils::SkinInstance(),
        KAknsIIDQgnPropCpDevPsActivate,
        fp->FullName(),
        EMbmGspowersavingquerypluginQgn_prop_cp_dev_ps_activate,
        EMbmGspowersavingquerypluginQgn_prop_cp_dev_ps_activate_mask );
        }
    else
        {
        icon = CGSPluginInterface::CreateIconL( aIconType );
        }

    CleanupStack::PopAndDestroy( fp );

    return icon;
  
    }

// -----------------------------------------------------------------------------
// CGSPowerSavingQueryPlugin::SwitchOnOffValue()
//
// 
// -----------------------------------------------------------------------------
//
void CGSPowerSavingQueryPlugin::SwitchOnOffValue( TInt& aValue )
    {
    if ( aValue == KGSSettingOn )
        {
        aValue = KGSIndexOn;
        }
    else if ( aValue == KGSSettingOff )
        {
        aValue = KGSIndexOff;
        }
    }

// End of File
