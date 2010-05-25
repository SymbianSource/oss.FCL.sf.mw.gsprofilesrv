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
* Description:    Cpp file for plugin.
*
*/


// Includes
#include "GSPrslnPlugin.h"
#include "GSLogger.h"

#include <gsprslnpluginrsc.rsg>
#include <GSPrslnPlugin.mbg>

// Includes from GS framework:
#include <GSFWViewUIDs.h>
#include <GSPrivatePluginProviderIds.h>
#include <GSMainView.h>
#include <csxhelp/cp.hlp.hrh>

#include <AknViewAppUi.h>
#include <ConeResLoader.h>
#include <barsread.h> // For TResourceReader
#include <StringLoader.h>
#include <FeatMgr.h>

#include <eikbtgpc.h>

#ifndef RD_CONTROL_PANEL
    #include <GSTabHelper.h>
#endif //RD_CONTROL_PANEL

// Constants
// Active Idle UID.
const TUid KScutActiveIdleUid       = { 0x102750F0 };

// ========================= MEMBER FUNCTIONS ================================


// ---------------------------------------------------------------------------
// CGSPrslnPlugin::CGSPrslnPlugin()
//
//
// ---------------------------------------------------------------------------
//
CGSPrslnPlugin::CGSPrslnPlugin()
    : CGSParentPlugin(), iResourceLoader( *iCoeEnv ), iCbaConstructed( EFalse )
    {
    __GSLOGSTRING( "[CGSPrslnPlugin] CGSPrslnPlugin()" );

    }


// ---------------------------------------------------------------------------
// CGSPrslnPlugin::~CGSPrslnPlugin()
//
//
// ---------------------------------------------------------------------------
//
CGSPrslnPlugin::~CGSPrslnPlugin()
    {
    iResourceLoader.Close();
    
    #ifndef RD_CONTROL_PANEL
        delete iTabHelper;
    #endif //RD_CONTROL_PANEL
    __GSLOGSTRING( "[CGSPrslnPlugin] ~CGSPrslnPlugin()" );
    }


// ---------------------------------------------------------------------------
// CGSPrslnPlugin::ConstructL()
//
//
// ---------------------------------------------------------------------------
//
void CGSPrslnPlugin::ConstructL()
    {
    __GSLOGSTRING( "[CGSPrslnPlugin] ConstructL()" );
    OpenLocalizedResourceFileL( KGSPrslnPluginResourceFileName,
                                iResourceLoader );
    BaseConstructL( R_GS_PRSLN_VIEW, R_GS_PRSLN_VIEW_TITLE );
    
    #ifndef RD_CONTROL_PANEL
        iTabHelper = CGSTabHelper::NewL();
    #endif //RD_CONTROL_PANEL
    }


// ---------------------------------------------------------------------------
// CGSPrslnPlugin::NewL()
//
//
// ---------------------------------------------------------------------------
//
CGSPrslnPlugin* CGSPrslnPlugin::NewL( TAny* /*aInitParams*/ )
	{
	__GSLOGSTRING( "[CGSPrslnPlugin] NewL()" );
	CGSPrslnPlugin* self = new(ELeave) CGSPrslnPlugin();
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
TUid CGSPrslnPlugin::Id() const
    {
    __GSLOGSTRING1( "[CGSPrslnPlugin] Id():0x%X", KGSPrslnPluginUid.iUid );
    return KGSPrslnPluginUid;
    }


// ---------------------------------------------------------------------------
// CGSParentPlugin::DoActivateL()
//
//
// ---------------------------------------------------------------------------
//
void CGSPrslnPlugin::DoActivateL
    ( const TVwsViewId& aPrevViewId,
      TUid aCustomMessageId,
      const TDesC8& aCustomMessage )
    {
    
    CEikButtonGroupContainer* cba = Cba();

    if ( aCustomMessageId == KScutActiveIdleUid && !iCbaConstructed )
        {
        cba->SetCommandSetL( R_GS_PSLN_SOFTKEYS_OPTIONS_EXIT_OPEN );
        cba->DrawDeferred();
        }
    iCbaConstructed = ETrue;
        
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
    }


// ---------------------------------------------------------------------------
// CGSParentPlugin::DoDeactivate()
//
//
// ---------------------------------------------------------------------------
//
void CGSPrslnPlugin::DoDeactivate()
    {
    CGSParentPlugin::DoDeactivate();
    
    #ifndef RD_CONTROL_PANEL
        iTabHelper->RemoveTabGroup();
    #endif //RD_CONTROL_PANEL
    }


// ========================= From CGSParentPlugin =====================


// ---------------------------------------------------------------------------
// CGSPrslnPlugin::UpperLevelViewUid()
//
//
// ---------------------------------------------------------------------------
//
TUid CGSPrslnPlugin::UpperLevelViewUid()
    {
    #ifdef RD_CONTROL_PANEL
        return KGSMainViewUid;
    #else //RD_CONTROL_PANEL
        return KGSGenPluginUid;
    #endif //RD_CONTROL_PANEL
    }


// -----------------------------------------------------------------------------
// CGSPrslnPlugin::GetHelpContext()
//
//
// -----------------------------------------------------------------------------
//
void CGSPrslnPlugin::GetHelpContext( TCoeHelpContext& aContext )
    {
    aContext.iMajor = KUidGS;
    aContext.iContext = KCP_HLP_PERSONAL;
    }


// ========================= From CGSPluginInterface ==================


// ---------------------------------------------------------------------------
// CGSPrslnPlugin::GetCaptionL()
//
//
// ---------------------------------------------------------------------------
//
void CGSPrslnPlugin::GetCaptionL( TDes& aCaption ) const
    {
    // The resource file is already opened by iResourceLoader.
    HBufC* result = StringLoader::LoadL( R_GS_PRSLN_VIEW_CAPTION );
    aCaption.Copy( *result );
    delete result;
    }


// ---------------------------------------------------------------------------
// CGSPrslnPlugin::CreateIconL
//
// Return the icon, if has one.
// ---------------------------------------------------------------------------
//
CGulIcon* CGSPrslnPlugin::CreateIconL( const TUid aIconType )
    {
    //EMbm<Mbm_file_name><Bitmap_name>
    CGulIcon* icon;
    TParse* fp = new( ELeave ) TParse();
    CleanupStack::PushL( fp );
    fp->Set( KGSPrslnPluginIconDirAndName, &KDC_BITMAP_DIR, NULL );

    if( aIconType == KGSIconTypeLbxItem )
        {
        icon = AknsUtils::CreateGulIconL(
        AknsUtils::SkinInstance(),
        KAknsIIDQgnPropCpPerso,
        fp->FullName(),
        EMbmGsprslnpluginQgn_prop_cp_perso,
        EMbmGsprslnpluginQgn_prop_cp_perso_mask );
        }
       else
        {
        icon = CGSPluginInterface::CreateIconL( aIconType );
        }

    CleanupStack::PopAndDestroy( fp );

    return icon;
    }

// ---------------------------------------------------------------------------
// CGSPrslnPlugin::PluginProviderCategory()
//
//
// ---------------------------------------------------------------------------
//
TInt CGSPrslnPlugin::PluginProviderCategory() const
    {
    return KGSPluginProviderInternal;
    }


// ---------------------------------------------------------------------------
// CGSPrslnPlugin::ListBoxType()
//
//
// ---------------------------------------------------------------------------
//
TGSListboxTypes CGSPrslnPlugin::ListBoxType()
    {
    return EGSListBoxTypeSingleLarge;
    }


// ----------------------------------------------------------------------------
// CGSPrslnPlugin::DynInitMenuPaneL()
// 
// Display the dynamic menu
// ----------------------------------------------------------------------------
void CGSPrslnPlugin::DynInitMenuPaneL( TInt aResourceId,
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
