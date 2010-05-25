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
* Description:  GSAppsPlugin source code.
*
*/


// Includes
#include "GSAppsPlugin.h"
#include "GsLogger.h"
#include <aknViewAppUi.h>
#include <ConeResLoader.h>
#include <gsappspluginrsc.rsg>
#include <gsfwviewuids.h>
#include <gsmainview.h>
#include <gsprivatepluginproviderids.h>
#include <StringLoader.h>
#include <barsread.h> // For TResourceReader
#include <gsappsplugin.mbg> // Icons
#include <csxhelp/cp.hlp.hrh>
#include <featmgr.h>

#ifndef RD_CONTROL_PANEL
    #include <GSTabHelper.h>
    #include <GSTabbedViewOwner.h>
#endif //RD_CONTROL_PANEL

// Constants

// ========================= MEMBER FUNCTIONS ================================


// ---------------------------------------------------------------------------
// CGSAppsPlugin::CGSAppsPlugin()
// Constructor
//
// ---------------------------------------------------------------------------
//
CGSAppsPlugin::CGSAppsPlugin()
    : CGSParentPlugin(), iResources( *iCoeEnv )
    {
    __GSLOGSTRING( "[CGSAppsPlugin] CGSAppsPlugin()" );
    }


// ---------------------------------------------------------------------------
// CGSAppsPlugin::~CGSAppsPlugin()
// Destructor
//
// ---------------------------------------------------------------------------
//
CGSAppsPlugin::~CGSAppsPlugin()
    {
    FeatureManager::UnInitializeLib();
    iResources.Close();
    
    #ifndef RD_CONTROL_PANEL
        delete iTabHelper;
    #endif //RD_CONTROL_PANEL
    
    __GSLOGSTRING( "[CGSAppsPlugin] ~CGSAppsPlugin()" );
    }


// ---------------------------------------------------------------------------
// CGSAppsPlugin::ConstructL(const TRect& aRect)
// Symbian OS two-phased constructor
//
// ---------------------------------------------------------------------------
//
void CGSAppsPlugin::ConstructL()
    {
    FeatureManager::InitializeLibL();
    OpenLocalizedResourceFileL( KGSAppsPluginResourceFileName, iResources );
    
    // Do not load content of this view untill/unless this view is activated.
    // This will save memory as well as optimize loading performance for other 
    // plugins.
    TBitFlags flags;
    flags.Set( EGSLoadChildrenOnActivation );
    SetOptionFlags( flags );
    
    BaseConstructL( R_GS_APPLICATIONS_VIEW, R_GS_APPS_VIEW_TITLE );
    
    #ifndef RD_CONTROL_PANEL
        iTabHelper = CGSTabHelper::NewL();
    #endif //RD_CONTROL_PANEL
    }


// ---------------------------------------------------------------------------
// CGSAppsPlugin::NewL()
// Static constructor
//
// ---------------------------------------------------------------------------
//
CGSAppsPlugin* CGSAppsPlugin::NewL( TAny* /*aInitParams*/ )
	{
	CGSAppsPlugin* self = new(ELeave) CGSAppsPlugin();
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
	}


// -----------------------------------------------------------------------------
// CGSAppsPlugin::Id()
//
//
// -----------------------------------------------------------------------------
//
TUid CGSAppsPlugin::Id() const
    {
    return KGSAppsPluginUid;
    }


// -----------------------------------------------------------------------------
// CGSAppsPlugin::DoActivateL()
//
//
// -----------------------------------------------------------------------------
//
void CGSAppsPlugin::DoActivateL( const TVwsViewId& aPrevViewId,
                                 TUid aCustomMessageId,
                                 const TDesC8& aCustomMessage )
    {
    CGSParentPlugin::DoActivateL( aPrevViewId,
                                  aCustomMessageId,
                                  aCustomMessage );
    CGSMainView* parent =
        static_cast<CGSMainView*> ( AppUi()->View( KGSMainViewUid ) );

    // Create tab group for this view (do only if casting succeeds):
    #ifndef RD_CONTROL_PANEL
        if( parent )
            {
            iTabHelper->CreateTabGroupL( Id(), parent );
            }
    #endif //RD_CONTROL_PANEL
    }


// -----------------------------------------------------------------------------
// CGSAppsPlugin::DoDeactivate()
//
//
// -----------------------------------------------------------------------------
//
void CGSAppsPlugin::DoDeactivate()
    {
    CGSParentPlugin::DoDeactivate();
    
    #ifndef RD_CONTROL_PANEL
        iTabHelper->RemoveTabGroup();
    #endif //RD_CONTROL_PANEL
    }


// ========================= From CGSParentPlugin =====================


// -----------------------------------------------------------------------------
// CGSAppsPlugin::UpperLevelViewUid()
//
//
// -----------------------------------------------------------------------------
//
TUid CGSAppsPlugin::UpperLevelViewUid()
    {
    return KGSMainViewUid;
    }


// ========================= From CGSPluginInterface ==================


// -----------------------------------------------------------------------------
// CGSAppsPlugin::GetCaptionL()
//
//
// -----------------------------------------------------------------------------
//
void CGSAppsPlugin::GetCaptionL( TDes& aCaption ) const
    {
    // The resource file is already opened.
    HBufC* result = StringLoader::LoadL( R_GS_APPS_VIEW_CAPTION );
    aCaption.Copy( *result );
    delete result;
    }


// -----------------------------------------------------------------------------
// CGSAppsPlugin::PluginProviderCategory()
//
//
// -----------------------------------------------------------------------------
//
TInt CGSAppsPlugin::PluginProviderCategory() const
    {
    return KGSPluginProviderInternal;
    }


// -----------------------------------------------------------------------------
// CGSAppsPlugin::GetHelpContext()
//
//
// -----------------------------------------------------------------------------
//
void CGSAppsPlugin::GetHelpContext( TCoeHelpContext& aContext )
    {
    aContext.iMajor = KUidGS;
    aContext.iContext = KCP_HLP_APPILICATION_SETTINGS;
    }


// -----------------------------------------------------------------------------
// CGSAppsPlugin::CreateIconL()
//
//
// -----------------------------------------------------------------------------
//
CGulIcon* CGSAppsPlugin::CreateIconL( const TUid aIconType )
    {
    //EMbm<Mbm_file_name><Bitmap_name>
    CGulIcon* icon;
    TParse* fp = new( ELeave ) TParse();
    CleanupStack::PushL( fp );
    fp->Set( KGSAppsPluginIconDirAndName, &KDC_BITMAP_DIR, NULL );

    if( aIconType == KGSIconTypeLbxItem )
        {
        icon = AknsUtils::CreateGulIconL(
        AknsUtils::SkinInstance(),
        KAknsIIDQgnPropSetAppsSub,
        fp->FullName(),
        EMbmGsappspluginQgn_prop_set_apps_sub,
        EMbmGsappspluginQgn_prop_set_apps_sub_mask );
        }
    else if( aIconType == KGSIconTypeTab )
        {
        icon = AknsUtils::CreateGulIconL(
        AknsUtils::SkinInstance(),
        KAknsIIDQgnPropSetAppsTab4,
        fp->FullName(),
        EMbmGsappspluginQgn_prop_set_apps_tab4,
        EMbmGsappspluginQgn_prop_set_apps_tab4_mask );
        }
       else
        {
        icon = CGSPluginInterface::CreateIconL( aIconType );
        }

    CleanupStack::PopAndDestroy( fp );

    return icon;
    }


// ----------------------------------------------------------------------------
// CGSAppsPlugin::DynInitMenuPaneL()
// 
// Display the dynamic menu
// ----------------------------------------------------------------------------
void CGSAppsPlugin::DynInitMenuPaneL( TInt aResourceId,
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

// End of File
