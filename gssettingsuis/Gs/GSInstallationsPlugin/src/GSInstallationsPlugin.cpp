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
* Description:  GSInstallationsPlugin source code.
*
*/


// Includes
#include "GSInstallationsPlugin.h"
#include "GsLogger.h"
#include <gsfwviewuids.h>
#include <GSInstallationsPlugin.mbg> // Icons
#include <GSInstallationsPluginRsc.rsg>
#include <gsmainview.h>
#include <gsprivatepluginproviderids.h>
#include <csxhelp/cp.hlp.hrh>
#include <aknViewAppUi.h>
#include <ConeResLoader.h>
#include <StringLoader.h>
#include <barsread.h> // For TResourceReader


// Constants


// ========================= MEMBER FUNCTIONS ================================


// ---------------------------------------------------------------------------
// CGSInstallationsPlugin::CGSInstallationsPlugin()
//
//
// ---------------------------------------------------------------------------
//
CGSInstallationsPlugin::CGSInstallationsPlugin()
    : CGSParentPlugin(), iResourceLoader( *iCoeEnv )
    {
    __GSLOGSTRING( "[CGSInstallationsPlugin] CGSInstallationsPlugin()" );

    }


// ---------------------------------------------------------------------------
// CGSInstallationsPlugin::~CGSInstallationsPlugin()
//
//
// ---------------------------------------------------------------------------
//
CGSInstallationsPlugin::~CGSInstallationsPlugin()
    {
    __GSLOGSTRING( "[CGSInstallationsPlugin] ~CGSInstallationsPlugin()|->" );
    iResourceLoader.Close();
    __GSLOGSTRING( "[CGSInstallationsPlugin] ~CGSInstallationsPlugin()-|" );
    }


// ---------------------------------------------------------------------------
// CGSInstallationsPlugin::ConstructL()
//
//
// ---------------------------------------------------------------------------
//
void CGSInstallationsPlugin::ConstructL()
    {
    __GSLOGSTRING( "[CGSInstallationsPlugin] ConstructL()|->" );
    OpenLocalizedResourceFileL( KGSInstallationsPluginResourceFileName,
                                iResourceLoader );
    BaseConstructL( R_GS_INSTALLATIONS_VIEW, R_GS_INSTALLATIONS_VIEW_TITLE );
    __GSLOGSTRING( "[CGSInstallationsPlugin] ConstructL()-|" );
    }


// ---------------------------------------------------------------------------
// CGSInstallationsPlugin::NewL()
//
//
// ---------------------------------------------------------------------------
//
CGSInstallationsPlugin* CGSInstallationsPlugin::NewL( TAny* /*aInitParams*/ )
    {
    __GSLOGSTRING( "[CGSInstallationsPlugin] NewL()" );

    CGSInstallationsPlugin* self = new(ELeave) CGSInstallationsPlugin();
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
TUid CGSInstallationsPlugin::Id() const
    {
    __GSLOGSTRING1( "[CGSInstallationsPlugin] Id():0x%X", KGSInstallationsPluginUid.iUid );
    return KGSInstallationsPluginUid;
    }


// ---------------------------------------------------------------------------
// CGSParentPlugin::DoActivateL()
//
//
// ---------------------------------------------------------------------------
//
void CGSInstallationsPlugin::DoActivateL
    ( const TVwsViewId& aPrevViewId,
      TUid aCustomMessageId,
      const TDesC8& aCustomMessage )
    {
    __GSLOGSTRING( "[CGSInstallationsPlugin] DoActivateL()|->" );
    CGSParentPlugin::DoActivateL( aPrevViewId,
                                  aCustomMessageId,
                                  aCustomMessage );
    CGSMainView* parent =
        static_cast<CGSMainView*> ( AppUi()->View( KGSMainViewUid ) );

    // Create tab group for this view (do only if casting parent succeeds):
    __GSLOGSTRING( "[CGSInstallationsPlugin] DoActivateL()-|" );
    }


// ---------------------------------------------------------------------------
// CGSParentPlugin::DoDeactivate()
//
//
// ---------------------------------------------------------------------------
//
void CGSInstallationsPlugin::DoDeactivate()
    {
    __GSLOGSTRING( "[CGSInstallationsPlugin] DoDeactivate()|->" );
    CGSParentPlugin::DoDeactivate();
    __GSLOGSTRING( "[CGSInstallationsPlugin] DoDeactivate()-|" );
    }


// ========================= From CGSParentPlugin =====================


// ---------------------------------------------------------------------------
// CGSInstallationsPlugin::UpperLevelViewUid()
//
//
// ---------------------------------------------------------------------------
//
TUid CGSInstallationsPlugin::UpperLevelViewUid()
    {
    return KGSMainViewUid;
    }


// ========================= From CGSPluginInterface ==================


// ---------------------------------------------------------------------------
// CGSInstallationsPlugin::GetCaptionL()
//
//
// ---------------------------------------------------------------------------
//
void CGSInstallationsPlugin::GetCaptionL( TDes& aCaption ) const
    {
    __GSLOGSTRING( "[CGSInstallationsPlugin] GetCaptionL()|->" );
    // The resource file is already opened by iResourceLoader.
    HBufC* result = StringLoader::LoadL( R_GS_INSTALLATIONS_VIEW_CAPTION );
    aCaption.Copy( *result );
    delete result;
    __GSLOGSTRING( "[CGSInstallationsPlugin] GetCaptionL()-|" );
    }


// ---------------------------------------------------------------------------
// CGSInstallationsPlugin::PluginProviderCategory()
//
//
// ---------------------------------------------------------------------------
//
TInt CGSInstallationsPlugin::PluginProviderCategory() const
    {
    return KGSPluginProviderInternal;
    }


// -----------------------------------------------------------------------------
// CGSInstallationsPlugin::GetHelpContext()
//
//
// -----------------------------------------------------------------------------
//
void CGSInstallationsPlugin::GetHelpContext( TCoeHelpContext& aContext )
    {
    aContext.iMajor = KUidGS;
    aContext.iContext = KCP_HLP_INSTALLATIONS;
    }


// -----------------------------------------------------------------------------
// CGSInstallationsPlugin::CreateIconL()
//
//
// -----------------------------------------------------------------------------
//
CGulIcon* CGSInstallationsPlugin::CreateIconL( const TUid aIconType )
    {

    //EMbm<Mbm_file_name><Bitmap_name>
    CGulIcon* icon;
    TParse* fp = new( ELeave ) TParse();
    CleanupStack::PushL( fp );
    fp->Set( KGSInstallationsPluginIconDirAndName, &KDC_BITMAP_DIR, NULL );

    if( aIconType == KGSIconTypeLbxItem )
        {
        icon = AknsUtils::CreateGulIconL(
        AknsUtils::SkinInstance(),
        KAknsIIDQgnPropCpInst,
        fp->FullName(),
        EMbmGsinstallationspluginQgn_prop_cp_inst,
        EMbmGsinstallationspluginQgn_prop_cp_inst_mask );
        }
       else
        {
        icon = CGSPluginInterface::CreateIconL( aIconType );
        }

    CleanupStack::PopAndDestroy( fp );

    return icon;
    }

// End of File
