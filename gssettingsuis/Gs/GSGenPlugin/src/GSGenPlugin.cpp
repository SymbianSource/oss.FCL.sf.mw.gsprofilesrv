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
* Description:  GSGenPlugin source code.
*
*/


// Includes
#include "GSGenPlugin.h"
#include "GsLogger.h"
#include <gsfwviewuids.h>
#include <gsgenplugin.mbg> // Icons
#include <GSGenPluginRsc.rsg>
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
// CGSGenPlugin::CGSGenPlugin()
//
//
// ---------------------------------------------------------------------------
//
CGSGenPlugin::CGSGenPlugin()
    : CGSParentPlugin(), iResourceLoader( *iCoeEnv )
    {
    __GSLOGSTRING( "[CGSGenPlugin] CGSGenPlugin()" );

    }


// ---------------------------------------------------------------------------
// CGSGenPlugin::~CGSGenPlugin()
//
//
// ---------------------------------------------------------------------------
//
CGSGenPlugin::~CGSGenPlugin()
    {
    __GSLOGSTRING( "[CGSGenPlugin] ~CGSGenPlugin()|->" );
    FeatureManager::UnInitializeLib();
    iResourceLoader.Close();
    
    #ifndef RD_CONTROL_PANEL
        if ( iTabHelper )
            {
            delete iTabHelper;
            }
    #endif //RD_CONTROL_PANEL
    __GSLOGSTRING( "[CGSGenPlugin] ~CGSGenPlugin()-|" );
    }


// ---------------------------------------------------------------------------
// CGSGenPlugin::ConstructL()
//
//
// ---------------------------------------------------------------------------
//
void CGSGenPlugin::ConstructL()
    {
    __GSLOGSTRING( "[CGSGenPlugin] ConstructL()|->" );
    FeatureManager::InitializeLibL();
    OpenLocalizedResourceFileL( KGSGenPluginResourceFileName,
                                iResourceLoader );
    BaseConstructL( R_GS_GEN_VIEW, R_GS_GEN_VIEW_TITLE );
    
    #ifndef RD_CONTROL_PANEL
        iTabHelper = CGSTabHelper::NewL();
    #endif //RD_CONTROL_PANEL
    __GSLOGSTRING( "[CGSGenPlugin] ConstructL()-|" );
    }


// ---------------------------------------------------------------------------
// CGSGenPlugin::NewL()
//
//
// ---------------------------------------------------------------------------
//
CGSGenPlugin* CGSGenPlugin::NewL( TAny* /*aInitParams*/ )
    {
    __GSLOGSTRING( "[CGSGenPlugin] NewL()" );

    CGSGenPlugin* self = new(ELeave) CGSGenPlugin();
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
TUid CGSGenPlugin::Id() const
    {
    __GSLOGSTRING1( "[CGSGenPlugin] Id():0x%X", KGSGenPluginUid.iUid );
    return KGSGenPluginUid;
    }


// ---------------------------------------------------------------------------
// CGSParentPlugin::DoActivateL()
//
//
// ---------------------------------------------------------------------------
//
void CGSGenPlugin::DoActivateL
    ( const TVwsViewId& aPrevViewId,
      TUid aCustomMessageId,
      const TDesC8& aCustomMessage )
    {
    __GSLOGSTRING( "[CGSGenPlugin] DoActivateL()|->" );
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
    __GSLOGSTRING( "[CGSGenPlugin] DoActivateL()-|" );
    }


// ---------------------------------------------------------------------------
// CGSParentPlugin::DoDeactivate()
//
//
// ---------------------------------------------------------------------------
//
void CGSGenPlugin::DoDeactivate()
    {
    __GSLOGSTRING( "[CGSGenPlugin] DoDeactivate()|->" );
    CGSParentPlugin::DoDeactivate();
    
    #ifndef RD_CONTROL_PANEL
        iTabHelper->RemoveTabGroup();
    #endif //RD_CONTROL_PANEL
    __GSLOGSTRING( "[CGSGenPlugin] DoDeactivate()-|" );
    }


// ========================= From CGSParentPlugin =====================


// ---------------------------------------------------------------------------
// CGSGenPlugin::UpperLevelViewUid()
//
//
// ---------------------------------------------------------------------------
//
TUid CGSGenPlugin::UpperLevelViewUid()
    {
    return KGSMainViewUid;
    }


// ========================= From CGSPluginInterface ==================


// ---------------------------------------------------------------------------
// CGSGenPlugin::GetCaptionL()
//
//
// ---------------------------------------------------------------------------
//
void CGSGenPlugin::GetCaptionL( TDes& aCaption ) const
    {
    __GSLOGSTRING( "[CGSGenPlugin] GetCaptionL()|->" );
    // The resource file is already opened by iResourceLoader.
    HBufC* result = StringLoader::LoadL( R_GS_GEN_VIEW_CAPTION );
    aCaption.Copy( *result );
    delete result;
    __GSLOGSTRING( "[CGSGenPlugin] GetCaptionL()-|" );
    }


// ---------------------------------------------------------------------------
// CGSGenPlugin::PluginProviderCategory()
//
//
// ---------------------------------------------------------------------------
//
TInt CGSGenPlugin::PluginProviderCategory() const
    {
    return KGSPluginProviderInternal;
    }


// -----------------------------------------------------------------------------
// CGSGenPlugin::GetHelpContext()
//
//
// -----------------------------------------------------------------------------
//
void CGSGenPlugin::GetHelpContext( TCoeHelpContext& aContext )
    {
    aContext.iMajor = KUidGS;
    aContext.iContext = KCP_HLP_DEVICE;
    }


// -----------------------------------------------------------------------------
// CGSGenPlugin::CreateIconL()
//
//
// -----------------------------------------------------------------------------
//
CGulIcon* CGSGenPlugin::CreateIconL( const TUid aIconType )
    {
    //EMbm<Mbm_file_name><Bitmap_name>
    CGulIcon* icon;
    TParse* fp = new( ELeave ) TParse();
    CleanupStack::PushL( fp );
    fp->Set( KGSGenPluginIconDirAndName, &KDC_BITMAP_DIR, NULL );

    if( aIconType == KGSIconTypeLbxItem )
        {
        icon = AknsUtils::CreateGulIconL(
        AknsUtils::SkinInstance(),
        KAknsIIDQgnPropCpDev,
        fp->FullName(),
        EMbmGsgenpluginQgn_prop_cp_dev,
        EMbmGsgenpluginQgn_prop_cp_dev_mask );
        }
       else
        {
        icon = CGSPluginInterface::CreateIconL( aIconType );
        }

    CleanupStack::PopAndDestroy( fp );

    return icon;
    }


// ----------------------------------------------------------------------------
// CGSGenPlugin::DynInitMenuPaneL()
// 
// Display the dynamic menu
// ----------------------------------------------------------------------------
void CGSGenPlugin::DynInitMenuPaneL( TInt aResourceId,
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
