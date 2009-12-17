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
* Description:  Source file for CGSSecurityPlugin class
*
*/


// Includes
#include "GSSecurityPlugin.h"
#include "GsLogger.h"

#include <aknViewAppUi.h>
#include <ConeResLoader.h>
#include <barsread.h>       // For TResourceReader
#include <StringLoader.h>

// Includes from GS framework:
#include <gsfwviewuids.h>
#include <gsprivatepluginproviderids.h>

// Plugin includes:
#include <GSSecurityPluginRsc.rsg>
#include <gssecplugin.mbg>
#include <csxhelp/cp.hlp.hrh>
#include <featmgr.h>


// CONSTANTS
_LIT( KGSSecurityPluginResourceFileName, "z:GSSecurityPluginRsc.rsc" );

#ifdef __SCALABLE_ICONS
  // svg file
  _LIT( KGSSecPluginIconFileName, "\\resource\\apps\\GSSecPlugin.mif");
#else
  // bitmap
  _LIT( KGSSecPluginIconFileName, "\\resource\\apps\\GSSecPlugin.mbm");
#endif // __SCALABLE_ICONS

// ========================= MEMBER FUNCTIONS ================================


// ---------------------------------------------------------------------------
// CGSSecurityPlugin::CGSSecurityPlugin()
//
//
// ---------------------------------------------------------------------------
//
CGSSecurityPlugin::CGSSecurityPlugin()
    : CGSParentPlugin(), iResourceLoader( *iCoeEnv )
    {
    }


// ---------------------------------------------------------------------------
// CGSSecurityPlugin::~CGSSecurityPlugin()
//
//
// ---------------------------------------------------------------------------
//
CGSSecurityPlugin::~CGSSecurityPlugin()
    {
    iResourceLoader.Close();
    }


// ---------------------------------------------------------------------------
// CGSSecurityPlugin::ConstructL()
//
//
// ---------------------------------------------------------------------------
//
void CGSSecurityPlugin::ConstructL()
    {
    __GSLOGSTRING( "[CGSSecurityPlugin] ConstructL()|->" );
    OpenLocalizedResourceFileL( KGSSecurityPluginResourceFileName,
                                iResourceLoader );
    BaseConstructL( R_GS_SECURITY_VIEW, R_GS_SECURITY_VIEW_TITLE );
    __GSLOGSTRING( "[CGSSecurityPlugin] ConstructL()-|" );
    }


// ---------------------------------------------------------------------------
// CGSSecurityPlugin::NewL()
//
//
// ---------------------------------------------------------------------------
//
CGSSecurityPlugin* CGSSecurityPlugin::NewL( TAny* /*aInitParams*/ )
  {
  CGSSecurityPlugin* self = new( ELeave ) CGSSecurityPlugin();
  CleanupStack::PushL( self );
  self->ConstructL();
  CleanupStack::Pop( self );
  return self;
  }


// ---------------------------------------------------------------------------
// CGSParentPlugin::Id()
//
//
// ---------------------------------------------------------------------------
//
TUid CGSSecurityPlugin::Id() const
    {
    return KGSSecurityPluginUid;
    }


// ---------------------------------------------------------------------------
// CGSParentPlugin::DoActivateL()
//
//
// ---------------------------------------------------------------------------
//
void CGSSecurityPlugin::DoActivateL( const TVwsViewId& aPrevViewId,
                                     TUid aCustomMessageId,
                                     const TDesC8& aCustomMessage )
    {
    CGSParentPlugin::DoActivateL( aPrevViewId, aCustomMessageId,
                                  aCustomMessage );
    }


// ---------------------------------------------------------------------------
// CGSParentPlugin::DoDeactivate()
//
//
// ---------------------------------------------------------------------------
//
void CGSSecurityPlugin::DoDeactivate()
    {
    CGSParentPlugin::DoDeactivate();
    }


// ========================= From CGSParentPlugin =====================


// ---------------------------------------------------------------------------
// CGSSecurityPlugin::UpperLevelViewUid()
//
//
// ---------------------------------------------------------------------------
//
TUid CGSSecurityPlugin::UpperLevelViewUid()
    {
    #ifdef RD_CONTROL_PANEL
        return KGSDeviceManagementPluginUid;
    #else //RD_CONTROL_PANEL
        return KGSGenPluginUid;
    #endif //RD_CONTROL_PANEL
    }


// -----------------------------------------------------------------------------
// CGSSecurityPlugin::GetHelpContext()
//
//
// -----------------------------------------------------------------------------
//
void CGSSecurityPlugin::GetHelpContext( TCoeHelpContext& aContext )
    {
    aContext.iMajor = KUidGS;
    aContext.iContext = KCP_HLP_SECURITY;
    }


// ========================= From CGSPluginInterface ==================


// ---------------------------------------------------------------------------
// CGSSecurityPlugin::GetCaptionL()
//
//
// ---------------------------------------------------------------------------
//
void CGSSecurityPlugin::GetCaptionL( TDes& aCaption ) const
    {
    __GSLOGSTRING( "[CGSSecurityPlugin] GetCaptionL()|->" );
    // The resource file is already opened by iResourceLoader.
    HBufC* result = StringLoader::LoadL( R_GS_SECURITY_VIEW_CAPTION );
    aCaption.Copy( *result );
    delete result;
    __GSLOGSTRING( "[CGSSecurityPlugin] GetCaptionL()-|" );
    }


// ---------------------------------------------------------------------------
// CGSSecurityPlugin::PluginProviderCategory()
//
//
// ---------------------------------------------------------------------------
//
TInt CGSSecurityPlugin::PluginProviderCategory() const
    {
    return KGSPluginProviderInternal;
    }

// ---------------------------------------------------------------------------
// CGSSecurityPlugin::CreateIconL
//
// Return the icon, if has one.
// ---------------------------------------------------------------------------
//
CGulIcon* CGSSecurityPlugin::CreateIconL( const TUid aIconType )
    {
    __GSLOGSTRING( "[CGSSecurityPlugin] CreateIconL()|->" );
    //EMbm<Mbm_file_name><Bitmap_name>
    CGulIcon* icon;

    if( aIconType == KGSIconTypeLbxItem )
        {
        icon = AknsUtils::CreateGulIconL(
        AknsUtils::SkinInstance(),
        KAknsIIDQgnPropSetSecSub,
        KGSSecPluginIconFileName,
        EMbmGssecpluginQgn_prop_set_sec_sub,
        EMbmGssecpluginQgn_prop_set_sec_sub_mask );
        }
     else
        {
        icon = CGSPluginInterface::CreateIconL( aIconType );
        }

    __GSLOGSTRING( "[CGSSecurityPlugin] CreateIconL()-|" );
    return icon;
    }


// -----------------------------------------------------------------------------
// CGSSecurityPlugin::ListBoxType()
//
//
// -----------------------------------------------------------------------------
//
TGSListboxTypes CGSSecurityPlugin::ListBoxType()
    {
    return EGSListBoxTypeSettings;
    }


// ----------------------------------------------------------------------------
// CGSSecurityPlugin::DynInitMenuPaneL()
// 
// Display the dynamic menu
// ----------------------------------------------------------------------------
void CGSSecurityPlugin::DynInitMenuPaneL( TInt aResourceId,
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
