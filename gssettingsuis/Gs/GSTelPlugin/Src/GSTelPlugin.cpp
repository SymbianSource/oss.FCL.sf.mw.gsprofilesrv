/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  GSTelPlugin source code.
*
*/


// Includes
#include "GSTelPlugin.h"
#include "GsLogger.h"
#include <gsfwviewuids.h>
#include <gstelplugin.mbg> // Icons
#include <gstelpluginrsc.rsg>
#include <gsmainview.h>
#include <gsprivatepluginproviderids.h>
#include <csxhelp/cp.hlp.hrh>
#include <aknViewAppUi.h>
#include <ConeResLoader.h>
#include <StringLoader.h>
#include <barsread.h> // For TResourceReader
#include <featmgr.h>

#ifndef RD_CONTROL_PANEL
    #include <GSTabHelper.h>
#endif //RD_CONTROL_PANEL


// Constants


// ========================= MEMBER FUNCTIONS ================================


// ---------------------------------------------------------------------------
// CGSTelPlugin::CGSTelPlugin()
//
//
// ---------------------------------------------------------------------------
//
CGSTelPlugin::CGSTelPlugin()
    : CGSParentPlugin(), iResourceLoader( *iCoeEnv )
    {
    __GSLOGSTRING( "[CGSTelPlugin] CGSTelPlugin()" );

    }


// ---------------------------------------------------------------------------
// CGSTelPlugin::~CGSTelPlugin()
//
//
// ---------------------------------------------------------------------------
//
CGSTelPlugin::~CGSTelPlugin()
    {
    __GSLOGSTRING( "[CGSTelPlugin] ~CGSTelPlugin()|->" );
    iResourceLoader.Close();
    
    #ifndef RD_CONTROL_PANEL
        if ( iTabHelper )
            {
            delete iTabHelper;
            }
    #endif //RD_CONTROL_PANEL
    __GSLOGSTRING( "[CGSTelPlugin] ~CGSTelPlugin()-|" );
    }


// ---------------------------------------------------------------------------
// CGSTelPlugin::ConstructL()
//
//
// ---------------------------------------------------------------------------
//
void CGSTelPlugin::ConstructL()
    {
    __GSLOGSTRING( "[CGSTelPlugin] ConstructL()|->" );
    OpenLocalizedResourceFileL( KGSTelPluginResourceFileName,
                                iResourceLoader );
    BaseConstructL( R_GS_TEL_VIEW, R_GS_TEL_VIEW_TITLE );
    
    #ifndef RD_CONTROL_PANEL
        iTabHelper = CGSTabHelper::NewL();
    #endif //RD_CONTROL_PANEL
    __GSLOGSTRING( "[CGSTelPlugin] ConstructL()-|" );
    }


// ---------------------------------------------------------------------------
// CGSTelPlugin::NewL()
//
//
// ---------------------------------------------------------------------------
//
CGSTelPlugin* CGSTelPlugin::NewL( TAny* /*aInitParams*/ )
    {
    __GSLOGSTRING( "[CGSTelPlugin] NewL()" );

    CGSTelPlugin* self = new(ELeave) CGSTelPlugin();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }


// ---------------------------------------------------------------------------
// CGSParentPlugin::Id()
//
//
// ---------------------------------------------------------------------------
//
TUid CGSTelPlugin::Id() const
    {
    __GSLOGSTRING1( "[CGSTelPlugin] Id():0x%X", KGSTelPluginUid.iUid );
    return KGSTelPluginUid;
    }


// ---------------------------------------------------------------------------
// CGSParentPlugin::DoActivateL()
//
//
// ---------------------------------------------------------------------------
//
void CGSTelPlugin::DoActivateL
    ( const TVwsViewId& aPrevViewId,
      TUid aCustomMessageId,
      const TDesC8& aCustomMessage )
    {
    __GSLOGSTRING( "[CGSTelPlugin] DoActivateL()|->" );
    CGSParentPlugin::DoActivateL( aPrevViewId,
                                  aCustomMessageId,
                                  aCustomMessage );
    CGSMainView* parent =
        static_cast<CGSMainView*> ( AppUi()->View( KGSMainViewUid ) );

    #ifndef RD_CONTROL_PANEL
        // Create tab group for this view (do only if casting parent succeeds):
        if( parent )
            {
            iTabHelper->CreateTabGroupL( Id(), parent );
            }
    #endif //RD_CONTROL_PANEL
    __GSLOGSTRING( "[CGSTelPlugin] DoActivateL()-|" );
    }


// ---------------------------------------------------------------------------
// CGSParentPlugin::DoDeactivate()
//
//
// ---------------------------------------------------------------------------
//
void CGSTelPlugin::DoDeactivate()
    {
    __GSLOGSTRING( "[CGSTelPlugin] DoDeactivate()|->" );
    CGSParentPlugin::DoDeactivate();
    
    #ifndef RD_CONTROL_PANEL
        iTabHelper->RemoveTabGroup();
    #endif //RD_CONTROL_PANEL
    __GSLOGSTRING( "[CGSTelPlugin] DoDeactivate()-|" );
    }


// ========================= From CGSParentPlugin =====================


// ---------------------------------------------------------------------------
// CGSTelPlugin::UpperLevelViewUid()
//
//
// ---------------------------------------------------------------------------
//
TUid CGSTelPlugin::UpperLevelViewUid()
    {
    return KGSMainViewUid;
    }


// ========================= From CGSPluginInterface ==================


// ---------------------------------------------------------------------------
// CGSTelPlugin::GetCaptionL()
//
//
// ---------------------------------------------------------------------------
//
void CGSTelPlugin::GetCaptionL( TDes& aCaption ) const
    {
    __GSLOGSTRING( "[CGSTelPlugin] GetCaptionL()|->" );
    // The resource file is already opened by iResourceLoader.
    HBufC* result = StringLoader::LoadL( R_GS_TEL_VIEW_CAPTION );
    aCaption.Copy( *result );
    delete result;
    __GSLOGSTRING( "[CGSTelPlugin] GetCaptionL()-|" );
    }


// ---------------------------------------------------------------------------
// CGSTelPlugin::PluginProviderCategory()
//
//
// ---------------------------------------------------------------------------
//
TInt CGSTelPlugin::PluginProviderCategory() const
    {
    return KGSPluginProviderInternal;
    }


// -----------------------------------------------------------------------------
// CGSTelPlugin::GetHelpContext()
//
//
// -----------------------------------------------------------------------------
//
void CGSTelPlugin::GetHelpContext( TCoeHelpContext& aContext )
    {
    aContext.iMajor = KUidGS;
    aContext.iContext = KCP_HLP_TELEPHONY;
    }


// -----------------------------------------------------------------------------
// CGSTelPlugin::CreateIconL()
//
//
// -----------------------------------------------------------------------------
//
CGulIcon* CGSTelPlugin::CreateIconL( const TUid aIconType )
    {
    //EMbm<Mbm_file_name><Bitmap_name>
    CGulIcon* icon;

    if( aIconType == KGSIconTypeLbxItem )
        {
        icon = AknsUtils::CreateGulIconL(
        AknsUtils::SkinInstance(),
        KAknsIIDQgnPropCpTele,
        KGSDefaultTelIconFileName,
        EMbmGstelpluginQgn_prop_cp_tele,
        EMbmGstelpluginQgn_prop_cp_tele_mask );
        }
       else
        {
        icon = CGSPluginInterface::CreateIconL( aIconType );
        }

    return icon;
    }

// ----------------------------------------------------------------------------
// CGSTelPlugin::DynInitMenuPaneL()
// 
// Display the dynamic menu
// ----------------------------------------------------------------------------
void CGSTelPlugin::DynInitMenuPaneL( TInt aResourceId,
                                     CEikMenuPane* aMenuPane )
    {
    FeatureManager::InitializeLibL();
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
	
	FeatureManager::UnInitializeLib();
    }
// End of File
