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
* Description:  GSDeviceManagementPlugin source code.
*
*/


// Includes
#include "GSDeviceManagementPlugin.h"
#include "GsLogger.h"
#include <gsfwviewuids.h>
#include <gsdevicemanagementplugin.mbg> // Icons
#include <GSDeviceManagementPluginRsc.rsg>
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
// CGSDeviceManagementPlugin::CGSDeviceManagementPlugin()
//
//
// ---------------------------------------------------------------------------
//
CGSDeviceManagementPlugin::CGSDeviceManagementPlugin()
    : CGSParentPlugin(), iResourceLoader( *iCoeEnv )
    {
    __GSLOGSTRING( "[CGSDeviceManagementPlugin] CGSDeviceManagementPlugin()" );

    }


// ---------------------------------------------------------------------------
// CGSDeviceManagementPlugin::~CGSDeviceManagementPlugin()
//
//
// ---------------------------------------------------------------------------
//
CGSDeviceManagementPlugin::~CGSDeviceManagementPlugin()
    {
    __GSLOGSTRING( "[CGSDeviceManagementPlugin] ~CGSDeviceManagementPlugin()|->" );
    iResourceLoader.Close();
    __GSLOGSTRING( "[CGSDeviceManagementPlugin] ~CGSDeviceManagementPlugin()-|" );
    }


// ---------------------------------------------------------------------------
// CGSDeviceManagementPlugin::ConstructL()
//
//
// ---------------------------------------------------------------------------
//
void CGSDeviceManagementPlugin::ConstructL()
    {
    __GSLOGSTRING( "[CGSDeviceManagementPlugin] ConstructL()|->" );
    OpenLocalizedResourceFileL( KGSDeviceManagementPluginResourceFileName,
                                iResourceLoader );
    BaseConstructL( R_GS_DEVICEMANAGEMENT_VIEW, R_GS_DEVICEMANAGEMENT_VIEW_TITLE );
    __GSLOGSTRING( "[CGSDeviceManagementPlugin] ConstructL()-|" );
    }


// ---------------------------------------------------------------------------
// CGSDeviceManagementPlugin::NewL()
//
//
// ---------------------------------------------------------------------------
//
CGSDeviceManagementPlugin* CGSDeviceManagementPlugin::NewL( TAny* /*aInitParams*/ )
    {
    __GSLOGSTRING( "[CGSDeviceManagementPlugin] NewL()" );

    CGSDeviceManagementPlugin* self = new(ELeave) CGSDeviceManagementPlugin();
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
TUid CGSDeviceManagementPlugin::Id() const
    {
    __GSLOGSTRING1( "[CGSDeviceManagementPlugin] Id():0x%X", KGSDeviceManagementPluginUid.iUid );
    return KGSDeviceManagementPluginUid;
    }


// ---------------------------------------------------------------------------
// CGSParentPlugin::DoActivateL()
//
//
// ---------------------------------------------------------------------------
//
void CGSDeviceManagementPlugin::DoActivateL
    ( const TVwsViewId& aPrevViewId,
      TUid aCustomMessageId,
      const TDesC8& aCustomMessage )
    {
    __GSLOGSTRING( "[CGSDeviceManagementPlugin] DoActivateL()|->" );
    CGSParentPlugin::DoActivateL( aPrevViewId,
                                  aCustomMessageId,
                                  aCustomMessage );
    CGSMainView* parent =
        static_cast<CGSMainView*> ( AppUi()->View( KGSMainViewUid ) );

    // Create tab group for this view (do only if casting parent succeeds):
    __GSLOGSTRING( "[CGSDeviceManagementPlugin] DoActivateL()-|" );
    }


// ---------------------------------------------------------------------------
// CGSParentPlugin::DoDeactivate()
//
//
// ---------------------------------------------------------------------------
//
void CGSDeviceManagementPlugin::DoDeactivate()
    {
    __GSLOGSTRING( "[CGSDeviceManagementPlugin] DoDeactivate()|->" );
    CGSParentPlugin::DoDeactivate();
    __GSLOGSTRING( "[CGSDeviceManagementPlugin] DoDeactivate()-|" );
    }


// ========================= From CGSParentPlugin =====================


// ---------------------------------------------------------------------------
// CGSDeviceManagementPlugin::UpperLevelViewUid()
//
//
// ---------------------------------------------------------------------------
//
TUid CGSDeviceManagementPlugin::UpperLevelViewUid()
    {
    return KGSGenPluginUid;
    }


// ========================= From CGSPluginInterface ==================


// ---------------------------------------------------------------------------
// CGSDeviceManagementPlugin::GetCaptionL()
//
//
// ---------------------------------------------------------------------------
//
void CGSDeviceManagementPlugin::GetCaptionL( TDes& aCaption ) const
    {
    __GSLOGSTRING( "[CGSDeviceManagementPlugin] GetCaptionL()|->" );
    // The resource file is already opened by iResourceLoader.
    HBufC* result = StringLoader::LoadL( R_GS_DEVICEMANAGEMENT_VIEW_CAPTION );
    aCaption.Copy( *result );
    delete result;
    __GSLOGSTRING( "[CGSDeviceManagementPlugin] GetCaptionL()-|" );
    }


// ---------------------------------------------------------------------------
// CGSDeviceManagementPlugin::PluginProviderCategory()
//
//
// ---------------------------------------------------------------------------
//
TInt CGSDeviceManagementPlugin::PluginProviderCategory() const
    {
    return KGSPluginProviderInternal;
    }


// -----------------------------------------------------------------------------
// CGSDeviceManagementPlugin::GetHelpContext()
//
//
// -----------------------------------------------------------------------------
//
void CGSDeviceManagementPlugin::GetHelpContext( TCoeHelpContext& aContext )
    {
    aContext.iMajor = KUidGS;
    aContext.iContext = KCP_HLP_DEVICE_MANAGEMENT;
    }


// -----------------------------------------------------------------------------
// CGSDeviceManagementPlugin::CreateIconL()
//
//
// -----------------------------------------------------------------------------
//
CGulIcon* CGSDeviceManagementPlugin::CreateIconL( const TUid aIconType )
    {
    //EMbm<Mbm_file_name><Bitmap_name>
    CGulIcon* icon;
    TParse* fp = new( ELeave ) TParse();
    CleanupStack::PushL( fp );
    fp->Set( KGSDeviceManagementPluginIconDirAndName, &KDC_BITMAP_DIR, NULL );

    if( aIconType == KGSIconTypeLbxItem )
        {
        icon = AknsUtils::CreateGulIconL(
        AknsUtils::SkinInstance(),
        KAknsIIDQgnPropCpDevMan,
        fp->FullName(),
        EMbmGsdevicemanagementpluginQgn_prop_cp_dev_man,
        EMbmGsdevicemanagementpluginQgn_prop_cp_dev_man_mask );
        }
       else
        {
        icon = CGSPluginInterface::CreateIconL( aIconType );
        }

    CleanupStack::PopAndDestroy( fp );

    return icon;
    }

// End of File
