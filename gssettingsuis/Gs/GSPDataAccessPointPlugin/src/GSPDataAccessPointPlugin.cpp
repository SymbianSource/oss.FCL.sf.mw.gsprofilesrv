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
* Description:  GSPDataAccessPointPlugin implementation.
*
*/


// User includes
#include    "GSPDataAccessPointPlugin.h"
#include    "GSPDataAccessPointModel.h"
#include    <gsparentplugin.h>
#include    <gscommon.hrh>
#include    <gspdataaccesspointpluginrsc.rsg> // GUI Resource
#include    <gsprivatepluginproviderids.h>
#include    <gsfwviewuids.h>

// System includes
#include    <aknnotewrappers.h>
#include    <aknradiobuttonsettingpage.h>
#include    <akntextsettingpage.h>
#include    <aknViewAppUi.h>
#include    <bautils.h>
#include    <featmgr.h>
#include    <hlplch.h> // HlpLauncher
#include    <StringLoader.h>
#include    <telephonydomainpskeys.h>
#include    <apgtask.h>

// Constants
const TInt KGSBufSize128 = 128;
// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSPDataAccessPointPlugin::CGSPDataAccessPointPlugin()
// Constructor
//
// ---------------------------------------------------------------------------
//
CGSPDataAccessPointPlugin::CGSPDataAccessPointPlugin()
    : iResources( *iCoeEnv )
    {
    }


// ---------------------------------------------------------------------------
// CGSPDataAccessPointPlugin::~CGSPDataAccessPointPlugin()
// Destructor
//
// ---------------------------------------------------------------------------
//
CGSPDataAccessPointPlugin::~CGSPDataAccessPointPlugin()
    {
    iResources.Close();

    if( iModel )
        {
        delete iModel;
        }
    }


// ---------------------------------------------------------------------------
// CGSPDataAccessPointPlugin::ConstructL(const TRect& aRect)
// Symbian OS two-phased constructor
//
// ---------------------------------------------------------------------------
//
void CGSPDataAccessPointPlugin::ConstructL()
    {
    iModel = CGSPDataAccessPointModel::NewL();
    OpenLocalizedResourceFileL( KPDataAccessPointResourceFileName, iResources );
    BaseConstructL( R_GS_PDATAACCESSPOINT_VIEW );
    }


// ---------------------------------------------------------------------------
// CGSPDataAccessPointPlugin::NewL()
// Static constructor
//
// ---------------------------------------------------------------------------
//
CGSPDataAccessPointPlugin* CGSPDataAccessPointPlugin::NewL( TAny* /*aInitParams*/ )
    {
    CGSPDataAccessPointPlugin* self = new( ELeave ) CGSPDataAccessPointPlugin();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CGSPDataAccessPointPlugin::Id
//
//
// -----------------------------------------------------------------------------
//
TUid CGSPDataAccessPointPlugin::Id() const
    {
    return KGSPDataAccessPointPluginUID;
    }

// -----------------------------------------------------------------------------
// CGSPDataAccessPointPlugin::DoActivateL
//
//
// -----------------------------------------------------------------------------
//
void CGSPDataAccessPointPlugin::DoActivateL( const TVwsViewId& /*aPrevViewId*/,
                                  TUid /*aCustomMessageId*/,
                                  const TDesC8& /*aCustomMessage*/ )
    {
    }


// -----------------------------------------------------------------------------
// CGSPDataAccessPointPlugin::DoDeactivate
//
//
// -----------------------------------------------------------------------------
//
void CGSPDataAccessPointPlugin::DoDeactivate()
    {
    }


// ========================= From CGSPluginInterface ==================


// -----------------------------------------------------------------------------
// CGSPDataAccessPointPlugin::GetCaptionL
//
//
// -----------------------------------------------------------------------------
//
void CGSPDataAccessPointPlugin::GetCaptionL( TDes& aCaption ) const
    {
    HBufC* result = StringLoader::LoadL( R_GS_PDATAACCESSPOINT_PLUGIN_CAPTION );
    aCaption.Copy( *result );
    delete result;
    }


// -----------------------------------------------------------------------------
// CGSPDataAccessPointPlugin::PluginProviderCategory
//
//
// -----------------------------------------------------------------------------
//
TInt CGSPDataAccessPointPlugin::PluginProviderCategory() const
    {
    return KGSPluginProviderInternal;
    }


// -----------------------------------------------------------------------------
// CGSPDataAccessPointPlugin::ItemType()
//
//
// -----------------------------------------------------------------------------
//
TGSListboxItemTypes CGSPDataAccessPointPlugin::ItemType()
    {
    return EGSItemTypeSettingDialog;
    }


// -----------------------------------------------------------------------------
// CGSPDataAccessPointPlugin::GetValue()
//
//
// -----------------------------------------------------------------------------
//
void CGSPDataAccessPointPlugin::GetValue( const TGSPluginValueKeys aKey,
                                    TDes& aValue )
    {
    TRAPD( err, GetValueL( aValue ) );
    if ( err != KErrNone )
        {
        CGSPluginInterface::GetValue( aKey, aValue );
        }
    }


// -----------------------------------------------------------------------------
// CGSPDataAccessPointPlugin::GetValueL()
//
// Leaving version of GetValue()
// -----------------------------------------------------------------------------
//
void CGSPDataAccessPointPlugin::GetValueL( TDes& aValue )
    {
    HBufC* name = HBufC::NewLC( KGSBufSize128 );
    TPtr ptrBuffer ( name->Des() );
    HBufC* dynamicText = name;
    if ( iModel->GetDialupAPNameL( ptrBuffer ) != KErrNone
         || ptrBuffer.Length() == 0 )
        {
        dynamicText = iEikonEnv->AllocReadResourceL( R_DIALUP_AP_NAME_NONE );
        }
    TPtr bufPtr( dynamicText->Des() );
		aValue.Copy( *dynamicText );
    CleanupStack::PopAndDestroy( name );
    }


// -----------------------------------------------------------------------------
// CGSPDataAccessPointPlugin::HandleSelection()
//
//
// -----------------------------------------------------------------------------
//
void CGSPDataAccessPointPlugin::HandleSelection(
    const TGSSelectionTypes aSelectionType )
    {
    switch( aSelectionType )
        {
        case EGSSelectionBySelectionKey:
        case EGSSelectionByMenu:
            TRAP_IGNORE( ShowAccessPointSettingPageL() );
            break;
        }

    }


// -----------------------------------------------------------------------------
// CGSPDataAccessPointPlugin::ShowAccessPointSettingPageL()
//
//
// -----------------------------------------------------------------------------
//
void CGSPDataAccessPointPlugin::ShowAccessPointSettingPageL()
    {
		TBuf<KGSMaxDAPName> name;
    //returns error code, no need to check the value
    //(it doesn't matter if 'name' is empty)
    iModel->GetDialupAPNameL( name );

    CAknTextSettingPage* dlg = new( ELeave ) CAknTextSettingPage(
        R_DIALUP_AP_NAME_SETTING_PAGE,
        name,
        CAknTextSettingPage::EZeroLengthAllowed );

    if ( dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged ) )
        {
        iModel->SetDialupAPNameL( name );
        // Update AI item's value to lbx:
    	CGSParentPlugin* parent = static_cast<CGSParentPlugin*>(
            AppUi()->View( KGSPDataPluginUid ) );// This is the parent plugin
        parent->UpdateView();
        }
    }


// -----------------------------------------------------------------------------
// CGSPDataAccessPointPlugin::OpenLocalizedResourceFileL()
//
//
// -----------------------------------------------------------------------------
//
void CGSPDataAccessPointPlugin::OpenLocalizedResourceFileL(
    const TDesC& aResourceFileName,
    RConeResourceLoader& aResourceLoader )
    {
    RFs fsSession;
    User::LeaveIfError( fsSession.Connect() );

    // Find the resource file:
    TParse parse;
    parse.Set( aResourceFileName, &KDC_RESOURCE_FILES_DIR, NULL );
    TFileName fileName( parse.FullName() );

    // Get language of resource file:
    BaflUtils::NearestLanguageFile( fsSession, fileName );

    // Open resource file:
    aResourceLoader.OpenL( fileName );

    // If leave occurs before this, close is called automatically when the
    // thread exits.
    fsSession.Close();
    }


// -----------------------------------------------------------------------------
// CGSPDataAccessPointPlugin::TGSMenuActivationItems()
//
//
// -----------------------------------------------------------------------------
//
TGSMenuActivationItems CGSPDataAccessPointPlugin::MenuActivationItem()
    {
    return EGSMenuActivationItemChange;
    }

// End of file
