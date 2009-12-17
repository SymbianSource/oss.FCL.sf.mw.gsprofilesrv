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
* Description:  GSPDataPlugin source code.
*
*/


// Includes
#include "GSPDataPlugin.h"
#include "GsLogger.h"
#include <gsfwviewuids.h>
#include <gspdataplugin.mbg> // Icons
#include <GSPDataPluginRsc.rsg>
#include <gsmainview.h>
#include <gsprivatepluginproviderids.h>
#include <conset.hlp.hrh>

#include <aknViewAppUi.h>
#include <ConeResLoader.h>
#include <StringLoader.h>
#include <barsread.h> // For TResourceReader
#include <featmgr.h>


// Constants


// ========================= MEMBER FUNCTIONS ================================


// ---------------------------------------------------------------------------
// CGSPDataPlugin::CGSPDataPlugin()
//
//
// ---------------------------------------------------------------------------
//
CGSPDataPlugin::CGSPDataPlugin()
    : CGSParentPlugin(), iResourceLoader( *iCoeEnv ), iViewActive ( EFalse )
    {
    __GSLOGSTRING( "[CGSPDataPlugin] CGSPDataPlugin()" );

    }


// ---------------------------------------------------------------------------
// CGSPDataPlugin::~CGSPDataPlugin()
//
//
// ---------------------------------------------------------------------------
//
CGSPDataPlugin::~CGSPDataPlugin()
    {
    __GSLOGSTRING( "[CGSPDataPlugin] ~CGSPDataPlugin()|->" );
    iResourceLoader.Close();
    __GSLOGSTRING( "[CGSPDataPlugin] ~CGSPDataPlugin()-|" );
    }


// ---------------------------------------------------------------------------
// CGSPDataPlugin::ConstructL()
//
//
// ---------------------------------------------------------------------------
//
void CGSPDataPlugin::ConstructL()
    {
    __GSLOGSTRING( "[CGSPDataPlugin] ConstructL()|->" );
    OpenLocalizedResourceFileL( KPDataResourceFileName, iResourceLoader );                                
    BaseConstructL( R_GS_PDATA_VIEW, R_GS_GPRS_VIEW_TITLE );
    __GSLOGSTRING( "[CGSPDataPlugin] ConstructL()-|" );
    }


// ---------------------------------------------------------------------------
// CGSPDataPlugin::NewL()
//
//
// ---------------------------------------------------------------------------
//
CGSPDataPlugin* CGSPDataPlugin::NewL( TAny* /*aInitParams*/ )
    {
    __GSLOGSTRING( "[CGSPDataPlugin] NewL()" );

    CGSPDataPlugin* self = new(ELeave) CGSPDataPlugin();
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
TUid CGSPDataPlugin::Id() const
    {
    return KGSPDataPluginUid;
    }


// ---------------------------------------------------------------------------
// CGSParentPlugin::DoActivateL()
//
//
// ---------------------------------------------------------------------------
//
void CGSPDataPlugin::DoActivateL
    ( const TVwsViewId& aPrevViewId,
      TUid aCustomMessageId,
      const TDesC8& aCustomMessage )
    {
    __GSLOGSTRING( "[CGSPDataPlugin] DoActivateL()|->" );
    CGSParentPlugin::DoActivateL( aPrevViewId,
                                  aCustomMessageId,
                                  aCustomMessage );
    CGSMainView* parent =
        static_cast<CGSMainView*> ( AppUi()->View( KGSMainViewUid ) );
        
    iViewActive = ETrue;

    // Create tab group for this view (do only if casting parent succeeds):
    __GSLOGSTRING( "[CGSPDataPlugin] DoActivateL()-|" );
    }


// ---------------------------------------------------------------------------
// CGSParentPlugin::DoDeactivate()
//
//
// ---------------------------------------------------------------------------
//
void CGSPDataPlugin::DoDeactivate()
    {
    __GSLOGSTRING( "[CGSPDataPlugin] DoDeactivate()|->" );
    CGSParentPlugin::DoDeactivate();
    iViewActive = EFalse;
    __GSLOGSTRING( "[CGSPDataPlugin] DoDeactivate()-|" );
    }


// ========================= From CGSParentPlugin =====================


// ---------------------------------------------------------------------------
// CGSPDataPlugin::UpperLevelViewUid()
//
//
// ---------------------------------------------------------------------------
//
TUid CGSPDataPlugin::UpperLevelViewUid()
    {
    #ifdef RD_CONTROL_PANEL
        return KGSAdminPluginUid;
    #else //RD_CONTROL_PANEL
        return KGSConPluginUid;
    #endif //RD_CONTROL_PANEL
    }


// ========================= From CGSPluginInterface ==================


// ---------------------------------------------------------------------------
// CGSPDataPlugin::GetCaptionL()
//
//
// ---------------------------------------------------------------------------
//
void CGSPDataPlugin::GetCaptionL( TDes& aCaption ) const
    {
    __GSLOGSTRING( "[CGSPDataPlugin] GetCaptionL()|->" );
    HBufC* result = StringLoader::LoadL( R_GS_GPRS_PLUGIN_CAPTION );
    aCaption.Copy( *result );
    delete result;
    __GSLOGSTRING( "[CGSPDataPlugin] GetCaptionL()-|" );
    }


// ---------------------------------------------------------------------------
// CGSPDataPlugin::PluginProviderCategory()
//
//
// ---------------------------------------------------------------------------
//
TInt CGSPDataPlugin::PluginProviderCategory() const
    {
    return KGSPluginProviderInternal;
    }


// -----------------------------------------------------------------------------
// CGSPDataPlugin::GetHelpContext()
//
//
// -----------------------------------------------------------------------------
//
void CGSPDataPlugin::GetHelpContext( TCoeHelpContext& aContext )
    {
    aContext.iMajor = KUidGS;
    aContext.iContext = KSET_HLP_CONNEC_GPRS;
    }


// -----------------------------------------------------------------------------
// CGSPDataPlugin::CreateIconL()
//
//
// -----------------------------------------------------------------------------
//
CGulIcon* CGSPDataPlugin::CreateIconL( const TUid aIconType )
    {
    CGulIcon* icon;
    TParse* fp = new( ELeave ) TParse();
    CleanupStack::PushL( fp );
    fp->Set( KGSPDataPluginIconDirAndName, &KDC_BITMAP_DIR, NULL );

    if( aIconType == KGSIconTypeLbxItem )
        {
        icon = AknsUtils::CreateGulIconL(
        AknsUtils::SkinInstance(),
        KAknsIIDQgnPropSetConnGprs,
        fp->FullName(),
        EMbmGspdatapluginQgn_prop_set_conn_gprs,
        EMbmGspdatapluginQgn_prop_set_conn_gprs_mask );
        }
    else
        {
        icon = CGSPluginInterface::CreateIconL( aIconType );
        }

    CleanupStack::PopAndDestroy( fp );

    return icon;
    }

// ---------------------------------------------------------------------------
// CGSPDataPlugin::ListBoxType()
//
//
// ---------------------------------------------------------------------------
//

TGSListboxTypes CGSPDataPlugin::ListBoxType()
    {
    return EGSListBoxTypeSettings;
    }


// ----------------------------------------------------------------------------
// CGSPDataPlugin::DynInitMenuPaneL()
// 
// Display the dynamic menu
// ----------------------------------------------------------------------------
void CGSPDataPlugin::DynInitMenuPaneL( TInt aResourceId,
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


// ----------------------------------------------------------------------------
// CGSPDataPlugin::CustomOperationL()
//
//
// ----------------------------------------------------------------------------
TAny* CGSPDataPlugin::CustomOperationL( TAny* aParam1, TAny* /*aParam2*/ )
    {
    TGSCustomOperationType* operationType =
                    static_cast<TGSCustomOperationType*> ( aParam1 );
    switch ( *operationType )
        {
        case EGSCustomOperationViewActive:
            // Returning view status - ETrue if the view is active, EFalse if
            // it's not
            return &iViewActive;
        default:
            break;
        }
    return NULL;
    }
    
// End of File
