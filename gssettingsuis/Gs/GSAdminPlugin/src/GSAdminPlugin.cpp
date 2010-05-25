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
* Description:    Cpp file for plugin.
*
*/


// Includes
#include "GSAdminPlugin.h"
#include "GsLogger.h"

#include <gsadminpluginrsc.rsg>
#include <gsadminplugin.mbg>

// Includes from GS framework:
#include <gsfwviewuids.h>
#include <gsprivatepluginproviderids.h>
#include <gstabhelper.h>
#include <gsmainview.h>
#include <csxhelp/cp.hlp.hrh>

#include <aknViewAppUi.h>
#include <ConeResLoader.h>
#include <barsread.h> // For TResourceReader
#include <StringLoader.h>

// Constants


// ========================= MEMBER FUNCTIONS ================================


// ---------------------------------------------------------------------------
// CGSAdminPlugin::CGSAdminPlugin()
//
//
// ---------------------------------------------------------------------------
//
CGSAdminPlugin::CGSAdminPlugin()
    : CGSParentPlugin(), iResourceLoader( *iCoeEnv )
    {
    __GSLOGSTRING( "[CGSAdminPlugin] CGSAdminPlugin()" );

    }


// ---------------------------------------------------------------------------
// CGSAdminPlugin::~CGSAdminPlugin()
//
//
// ---------------------------------------------------------------------------
//
CGSAdminPlugin::~CGSAdminPlugin()
    {
    iResourceLoader.Close();
    __GSLOGSTRING( "[CGSAdminPlugin] ~CGSAdminPlugin()" );
    }


// ---------------------------------------------------------------------------
// CGSAdminPlugin::ConstructL()
//
//
// ---------------------------------------------------------------------------
//
void CGSAdminPlugin::ConstructL()
    {
    __GSLOGSTRING( "[CGSAdminPlugin] ConstructL()" );
    OpenLocalizedResourceFileL( KGSAdminPluginResourceFileName,
                                iResourceLoader );
    
    // Do not load content of this view untill/unless this view is activated.
    // This will save memory as well as optimize loading performance for other 
    // plugins.
    TBitFlags flags;
    flags.Set( EGSLoadChildrenOnActivation );
    SetOptionFlags( flags );
    
    BaseConstructL( R_GS_ADMIN_VIEW, R_GS_ADMIN_VIEW_TITLE );
    }


// ---------------------------------------------------------------------------
// CGSAdminPlugin::NewL()
//
//
// ---------------------------------------------------------------------------
//
CGSAdminPlugin* CGSAdminPlugin::NewL( TAny* /*aInitParams*/ )
	{
	__GSLOGSTRING( "[CGSAdminPlugin] NewL()" );
	CGSAdminPlugin* self = new(ELeave) CGSAdminPlugin();
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
TUid CGSAdminPlugin::Id() const
    {
    __GSLOGSTRING1( "[CGSAdminPlugin] Id():0x%X", KGSAdminPluginUid.iUid );
    return KGSAdminPluginUid;
    }


// ---------------------------------------------------------------------------
// CGSParentPlugin::DoActivateL()
//
//
// ---------------------------------------------------------------------------
//
void CGSAdminPlugin::DoActivateL
    ( const TVwsViewId& aPrevViewId,
      TUid aCustomMessageId,
      const TDesC8& aCustomMessage )
    {
    CGSParentPlugin::DoActivateL( aPrevViewId,
                                  aCustomMessageId,
                                  aCustomMessage );
    CGSMainView* parent =
        static_cast<CGSMainView*> ( AppUi()->View( KGSMainViewUid ) );
    }


// ---------------------------------------------------------------------------
// CGSParentPlugin::DoDeactivate()
//
//
// ---------------------------------------------------------------------------
//
void CGSAdminPlugin::DoDeactivate()
    {
    CGSParentPlugin::DoDeactivate();
    }


// ========================= From CGSParentPlugin =====================


// ---------------------------------------------------------------------------
// CGSAdminPlugin::UpperLevelViewUid()
//
//
// ---------------------------------------------------------------------------
//
TUid CGSAdminPlugin::UpperLevelViewUid()
    {
    return KGSConPluginUid;
    }


// -----------------------------------------------------------------------------
// CGSAdminPlugin::GetHelpContext()
//
//
// -----------------------------------------------------------------------------
//
void CGSAdminPlugin::GetHelpContext( TCoeHelpContext& aContext )
    {
    aContext.iMajor = KUidGS;
    aContext.iContext = KCP_HLP_ADMINISTRATIVE_SETTINGS;
    }


// ========================= From CGSPluginInterface ==================


// ---------------------------------------------------------------------------
// CGSAdminPlugin::GetCaptionL()
//
//
// ---------------------------------------------------------------------------
//
void CGSAdminPlugin::GetCaptionL( TDes& aCaption ) const
    {
    // The resource file is already opened by iResourceLoader.
    HBufC* result = StringLoader::LoadL( R_GS_ADMIN_VIEW_CAPTION );
    aCaption.Copy( *result );
    delete result;
    }


// ---------------------------------------------------------------------------
// CGSAdminPlugin::CreateIconL
//
// Return the icon, if has one.
// ---------------------------------------------------------------------------
//
CGulIcon* CGSAdminPlugin::CreateIconL( const TUid aIconType )
    {
    //EMbm<Mbm_file_name><Bitmap_name>
    CGulIcon* icon;
    TParse* fp = new( ELeave ) TParse();
    CleanupStack::PushL( fp );
    fp->Set( KGSAdminPluginIconDirAndName, &KDC_BITMAP_DIR, NULL );

    if( aIconType == KGSIconTypeLbxItem )
        {
        icon = AknsUtils::CreateGulIconL(
        AknsUtils::SkinInstance(),
        KAknsIIDQgnPropCpConnAdmin,
        fp->FullName(),
        EMbmGsadminpluginQgn_prop_cp_conn_admin,
        EMbmGsadminpluginQgn_prop_cp_conn_admin_mask );
        }
       else
        {
        icon = CGSPluginInterface::CreateIconL( aIconType );
        }

    CleanupStack::PopAndDestroy( fp );

    return icon;
    }

// ---------------------------------------------------------------------------
// CGSAdminPlugin::PluginProviderCategory()
//
//
// ---------------------------------------------------------------------------
//
TInt CGSAdminPlugin::PluginProviderCategory() const
    {
    return KGSPluginProviderInternal;
    }


// ---------------------------------------------------------------------------
// CGSAdminPlugin::ListBoxType()
//
//
// ---------------------------------------------------------------------------
//
TGSListboxTypes CGSAdminPlugin::ListBoxType()
    {
    return EGSListBoxTypeSettings;
    }


// End of File
