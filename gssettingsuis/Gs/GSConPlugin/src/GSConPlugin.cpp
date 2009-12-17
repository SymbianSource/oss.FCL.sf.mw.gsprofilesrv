/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  GSConPlugin implementation.
*
*/


// Includes
#include "GSConPlugin.h"
#include "GsLogger.h"

#include <gsconplugin.mbg> // Icons
#include <GSConPluginRsc.rsg>
#include <gsfwviewuids.h>
#include <gsmainview.h>
#include <gsplugininterface.h>
#include <gsprivatepluginproviderids.h>
#include <data_caging_path_literals.hrh>
#include <aknViewAppUi.h>
#include <barsread.h> // For TResourceReader
#include <ConeResLoader.h>
#include <StringLoader.h>
#include <csxhelp/cp.hlp.hrh>
#include <featmgr.h>
#include <eikbtgpc.h>

#ifndef RD_CONTROL_PANEL
    #include <GSTabHelper.h>
#endif //RD_CONTROL_PANEL

// Constants

// ========================= MEMBER FUNCTIONS ================================


// ---------------------------------------------------------------------------
// CGSConPlugin::CGSConPlugin()
//
//
// ---------------------------------------------------------------------------
//
CGSConPlugin::CGSConPlugin()
    : CGSParentPlugin(), iResources( *iCoeEnv )
    {
    __GSLOGSTRING( "[CGSConPlugin] CGSConPlugin()" );

    }


// ---------------------------------------------------------------------------
// CGSConPlugin::~CGSConPlugin()
//
//
// ---------------------------------------------------------------------------
//
CGSConPlugin::~CGSConPlugin()
    {
    FeatureManager::UnInitializeLib();
    iResources.Close();

    #ifndef RD_CONTROL_PANEL
        delete iTabHelper;
    #endif //RD_CONTROL_PANEL
    __GSLOGSTRING( "[CGSConPlugin] ~CGSConPlugin()" );
    }


// ---------------------------------------------------------------------------
// CGSConPlugin::ConstructL()
//
//
// ---------------------------------------------------------------------------
//
void CGSConPlugin::ConstructL()
    {
    __GSLOGSTRING( "[CGSConPlugin] ConstructL()" );
    FeatureManager::InitializeLibL();
    OpenLocalizedResourceFileL( KGSConPluginResourceFileName, iResources );
    BaseConstructL( R_GS_CON_VIEW, R_GS_CON_VIEW_TITLE );

    #ifndef RD_CONTROL_PANEL
        iTabHelper = CGSTabHelper::NewL();
    #endif //RD_CONTROL_PANEL
    }


// ---------------------------------------------------------------------------
// CGSConPlugin::NewL()
//
//
// ---------------------------------------------------------------------------
//
CGSConPlugin* CGSConPlugin::NewL( TAny* /*aInitParams*/ )
    {
    __GSLOGSTRING( "[CGSConPlugin] NewL()" );

    CGSConPlugin* self = new(ELeave) CGSConPlugin();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CGSConPlugin::Id()
//
//
// ---------------------------------------------------------------------------
//
TUid CGSConPlugin::Id() const
    {
    __GSLOGSTRING1( "[CGSConPlugin] Id():0x%X", KGSConPluginUid.iUid );
    return KGSConPluginUid;
    }


// ---------------------------------------------------------------------------
// CGSConPlugin::DoActivateL()
//
//
// ---------------------------------------------------------------------------
//
void CGSConPlugin::DoActivateL( const TVwsViewId& aPrevViewId,
                                TUid aCustomMessageId,
                                const TDesC8& aCustomMessage )
    {
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
    
		// If AppUid is different or placeholderview is used, this view has been launched from outside GS
		if ( iPrevViewId.iAppUid != KUidGS || iPrevViewId.iViewUid == TUid::Uid(0x0DEADBED ) )
			{
			CEikButtonGroupContainer* cbaGroup = Cba();
			if(cbaGroup)
				{
				HBufC* rightSKText = StringLoader::LoadLC (R_GS_CBA_EXIT);
				TPtr rskPtr = rightSKText->Des();
				cbaGroup->SetCommandL(2,EAknSoftkeyExit,*rightSKText);
				CleanupStack::PopAndDestroy(rightSKText);
				}
		}
    
    }


// ---------------------------------------------------------------------------
// CGSParentPlugin::DoDeactivate()
//
//
// ---------------------------------------------------------------------------
void CGSConPlugin::DoDeactivate()
    {
    CGSParentPlugin::DoDeactivate();

    #ifndef RD_CONTROL_PANEL
        iTabHelper->RemoveTabGroup();
    #endif //RD_CONTROL_PANEL
    }


// ========================= From CGSParentPlugin =====================


// ---------------------------------------------------------------------------
// CGSConPlugin::UpperLevelViewUid()
//
//
// ---------------------------------------------------------------------------
//
TUid CGSConPlugin::UpperLevelViewUid()
    {
    return KGSMainViewUid;
    }


// ========================= From CGSPluginInterface ==================


// ---------------------------------------------------------------------------
// CGSConPlugin::GetCaptionL()
//
//
// ---------------------------------------------------------------------------
//
void CGSConPlugin::GetCaptionL( TDes& aCaption ) const
    {
    // The resource file is already opened.
    HBufC* result = StringLoader::LoadL( R_GS_CON_VIEW_CAPTION );
    aCaption.Copy( *result );
    delete result;
    }


// ---------------------------------------------------------------------------
// CGSConPlugin::PluginProviderCategory()
//
//
// ---------------------------------------------------------------------------
//
TInt CGSConPlugin::PluginProviderCategory() const
    {
    return KGSPluginProviderInternal;
    }


// -----------------------------------------------------------------------------
// CGSConPlugin::GetHelpContext()
//
//
// -----------------------------------------------------------------------------
//
void CGSConPlugin::GetHelpContext( TCoeHelpContext& aContext )
    {
    aContext.iMajor = KUidGS;
    aContext.iContext = KCP_HLP_CONNECTIVITY;
    }


// ---------------------------------------------------------------------------
// CGSConPlugin::CreateIconL
//
//
// ---------------------------------------------------------------------------
//
CGulIcon* CGSConPlugin::CreateIconL( const TUid aIconType )
    {
    //EMbm<Mbm_file_name><Bitmap_name>
    CGulIcon* icon;
    TParse* fp = new( ELeave ) TParse();
    CleanupStack::PushL( fp );
    fp->Set( KGSConPluginIconDirAndName, &KDC_BITMAP_DIR, NULL );

    if( aIconType == KGSIconTypeLbxItem )
        {
        icon = AknsUtils::CreateGulIconL(
        AknsUtils::SkinInstance(),
        KAknsIIDQgnPropCpConn,
        fp->FullName(),
        EMbmGsconpluginQgn_prop_cp_conn,
        EMbmGsconpluginQgn_prop_cp_conn_mask );
        }
    else
        {
        icon = CGSPluginInterface::CreateIconL( aIconType );
        }

    CleanupStack::PopAndDestroy( fp );

    return icon;
    }

// ---------------------------------------------------------------------------
// CGSConPlugin::ListBoxType()
//
// ---------------------------------------------------------------------------
//
TGSListboxTypes CGSConPlugin::ListBoxType()
    {
    return EGSListBoxTypeDoubleLarge;
    }


// End of file
