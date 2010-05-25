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
* Description:  GSSynchronisationPlugin source code.
*
*/


// Includes
#include "GSSynchronisationPlugin.h"
#include "GsLogger.h"
#include <gsfwviewuids.h>
#include <gssynchronisationplugin.mbg> // Icons
#include <gssynchronisationpluginrsc.rsg>
#include <gsmainview.h>
#include <gsprivatepluginproviderids.h>
#include <gstabhelper.h>
#include <csxhelp/cp.hlp.hrh>


#include <aknViewAppUi.h>
#include <ConeResLoader.h>
#include <StringLoader.h>
#include <barsread.h> // For TResourceReader


// Constants


// ========================= MEMBER FUNCTIONS ================================


// ---------------------------------------------------------------------------
// CGSSynchronisationPlugin::CGSSynchronisationPlugin()
//
//
// ---------------------------------------------------------------------------
//
CGSSynchronisationPlugin::CGSSynchronisationPlugin()
    : CGSParentPlugin(), iResourceLoader( *iCoeEnv )
    {
    __GSLOGSTRING( "[CGSSynchronisationPlugin] CGSSynchronisationPlugin()" );

    }


// ---------------------------------------------------------------------------
// CGSSynchronisationPlugin::~CGSSynchronisationPlugin()
//
//
// ---------------------------------------------------------------------------
//
CGSSynchronisationPlugin::~CGSSynchronisationPlugin()
    {
    __GSLOGSTRING( "[CGSSynchronisationPlugin] ~CGSSynchronisationPlugin()|->" );
    iResourceLoader.Close();
    __GSLOGSTRING( "[CGSSynchronisationPlugin] ~CGSSynchronisationPlugin()-|" );
    }


// ---------------------------------------------------------------------------
// CGSSynchronisationPlugin::ConstructL()
//
//
// ---------------------------------------------------------------------------
//
void CGSSynchronisationPlugin::ConstructL()
    {
    __GSLOGSTRING( "[CGSSynchronisationPlugin] ConstructL()|->" );
    OpenLocalizedResourceFileL( KGSSynchronisationPluginResourceFileName,
                                iResourceLoader );
    BaseConstructL( R_GS_SYNCHRONISATION_VIEW, R_GS_SYNCHRONISATION_VIEW_TITLE );
    __GSLOGSTRING( "[CGSSynchronisationPlugin] ConstructL()-|" );
    }


// ---------------------------------------------------------------------------
// CGSSynchronisationPlugin::NewL()
//
//
// ---------------------------------------------------------------------------
//
CGSSynchronisationPlugin* CGSSynchronisationPlugin::NewL( TAny* /*aInitParams*/ )
    {
    __GSLOGSTRING( "[CGSSynchronisationPlugin] NewL()" );

    CGSSynchronisationPlugin* self = new(ELeave) CGSSynchronisationPlugin();
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
TUid CGSSynchronisationPlugin::Id() const
    {
    __GSLOGSTRING1( "[CGSSynchronisationPlugin] Id():0x%X", KGSSynchronisationPluginUid.iUid );
    return KGSSynchronisationPluginUid;
    }


// ---------------------------------------------------------------------------
// CGSParentPlugin::DoActivateL()
//
//
// ---------------------------------------------------------------------------
//
void CGSSynchronisationPlugin::DoActivateL
    ( const TVwsViewId& aPrevViewId,
      TUid aCustomMessageId,
      const TDesC8& aCustomMessage )
    {
    __GSLOGSTRING( "[CGSSynchronisationPlugin] DoActivateL()|->" );
    CGSParentPlugin::DoActivateL( aPrevViewId,
                                  aCustomMessageId,
                                  aCustomMessage );

    __GSLOGSTRING( "[CGSSynchronisationPlugin] DoActivateL()-|" );
    }


// ---------------------------------------------------------------------------
// CGSParentPlugin::DoDeactivate()
//
//
// ---------------------------------------------------------------------------
//
void CGSSynchronisationPlugin::DoDeactivate()
    {
    __GSLOGSTRING( "[CGSSynchronisationPlugin] DoDeactivate()|->" );
    CGSParentPlugin::DoDeactivate();
    __GSLOGSTRING( "[CGSSynchronisationPlugin] DoDeactivate()-|" );
    }


// ========================= From CGSParentPlugin =====================


// ---------------------------------------------------------------------------
// CGSSynchronisationPlugin::UpperLevelViewUid()
//
//
// ---------------------------------------------------------------------------
//
TUid CGSSynchronisationPlugin::UpperLevelViewUid()
    {
    return KGSConPluginUid;
    }


// ========================= From CGSPluginInterface ==================


// ---------------------------------------------------------------------------
// CGSSynchronisationPlugin::GetCaptionL()
//
//
// ---------------------------------------------------------------------------
//
void CGSSynchronisationPlugin::GetCaptionL( TDes& aCaption ) const
    {
    __GSLOGSTRING( "[CGSSynchronisationPlugin] GetCaptionL()|->" );
    // The resource file is already opened by iResourceLoader.
    HBufC* result = StringLoader::LoadL( R_GS_SYNCHRONISATION_VIEW_CAPTION );
    aCaption.Copy( *result );
    delete result;
    __GSLOGSTRING( "[CGSSynchronisationPlugin] GetCaptionL()-|" );
    }


// ---------------------------------------------------------------------------
// CGSSynchronisationPlugin::PluginProviderCategory()
//
//
// ---------------------------------------------------------------------------
//
TInt CGSSynchronisationPlugin::PluginProviderCategory() const
    {
    return KGSPluginProviderInternal;
    }


// -----------------------------------------------------------------------------
// CGSSynchronisationPlugin::GetHelpContext()
//
//
// -----------------------------------------------------------------------------
//
void CGSSynchronisationPlugin::GetHelpContext( TCoeHelpContext& aContext )
    {
    aContext.iMajor = KUidGS;
    aContext.iContext = KCP_HLP_SYNCHRONISATION;
    }


// -----------------------------------------------------------------------------
// CGSSynchronisationPlugin::CreateIconL()
//
//
// -----------------------------------------------------------------------------
//
CGulIcon* CGSSynchronisationPlugin::CreateIconL( const TUid aIconType )
    {
    //EMbm<Mbm_file_name><Bitmap_name>
    CGulIcon* icon;
    TParse* fp = new( ELeave ) TParse();
    CleanupStack::PushL( fp );
    fp->Set( KGSSynchronisationPluginIconDirAndName, &KDC_BITMAP_DIR, NULL );

    if( aIconType == KGSIconTypeLbxItem )
        {
        icon = AknsUtils::CreateGulIconL(
        AknsUtils::SkinInstance(),
        KAknsIIDQgnPropCpConnSync,
        fp->FullName(),
        EMbmGssynchronisationpluginQgn_prop_cp_conn_sync,
        EMbmGssynchronisationpluginQgn_prop_cp_conn_sync_mask );
        }
       else
        {
        icon = CGSPluginInterface::CreateIconL( aIconType );
        }

    CleanupStack::PopAndDestroy( fp );

    return icon;
    }

// End of File
