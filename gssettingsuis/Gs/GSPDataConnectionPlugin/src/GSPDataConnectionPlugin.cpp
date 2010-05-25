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
* Description:  GSPDataConnectionPlugin implementation.
*
*/


// User includes
#include    "GSPDataConnectionPlugin.h"
#include    "GSPDataConnectionModel.h"
#include    <gsparentplugin.h>
#include    <gscommon.hrh>
#include    <gspdataconnectionpluginrsc.rsg> // GUI Resource
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

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSPDataConnectionPlugin::CGSPDataConnectionPlugin()
// Constructor
//
// ---------------------------------------------------------------------------
//
CGSPDataConnectionPlugin::CGSPDataConnectionPlugin()
    : iResources( *iCoeEnv )
    {
    }


// ---------------------------------------------------------------------------
// CGSPDataConnectionPlugin::~CGSPDataConnectionPlugin()
// Destructor
//
// ---------------------------------------------------------------------------
//
CGSPDataConnectionPlugin::~CGSPDataConnectionPlugin()
    {
    iResources.Close();

    if( iModel )
        {
        delete iModel;
        }
    }


// ---------------------------------------------------------------------------
// CGSPDataConnectionPlugin::ConstructL(const TRect& aRect)
// Symbian OS two-phased constructor
//
// ---------------------------------------------------------------------------
//
void CGSPDataConnectionPlugin::ConstructL()
    {
    iModel = CGSPDataConnectionModel::NewL();
    OpenLocalizedResourceFileL( KPDataConnectionResourceFileName, iResources );
    BaseConstructL( R_GS_PDATACONNECTION_VIEW );
    }


// ---------------------------------------------------------------------------
// CGSPDataConnectionPlugin::NewL()
// Static constructor
//
// ---------------------------------------------------------------------------
//
CGSPDataConnectionPlugin* CGSPDataConnectionPlugin::NewL( TAny* /*aInitParams*/ )
    {
    CGSPDataConnectionPlugin* self = new( ELeave ) CGSPDataConnectionPlugin();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CGSPDataConnectionPlugin::Id
//
//
// -----------------------------------------------------------------------------
//
TUid CGSPDataConnectionPlugin::Id() const
    {
    return KGSPDataConnectionPluginUID;
    }

// -----------------------------------------------------------------------------
// CGSPDataConnectionPlugin::DoActivateL
//
//
// -----------------------------------------------------------------------------
//
void CGSPDataConnectionPlugin::DoActivateL( const TVwsViewId& /*aPrevViewId*/,
                                  TUid /*aCustomMessageId*/,
                                  const TDesC8& /*aCustomMessage*/ )
    {
    }


// -----------------------------------------------------------------------------
// CGSPDataConnectionPlugin::DoDeactivate
//
//
// -----------------------------------------------------------------------------
//
void CGSPDataConnectionPlugin::DoDeactivate()
    {
    }


// ========================= From CGSPluginInterface ==================


// -----------------------------------------------------------------------------
// CGSPDataConnectionPlugin::GetCaptionL
//
//
// -----------------------------------------------------------------------------
//
void CGSPDataConnectionPlugin::GetCaptionL( TDes& aCaption ) const
    {
    HBufC* result = StringLoader::LoadL( R_GS_PDATACONNECTION_PLUGIN_CAPTION );
    aCaption.Copy( *result );
    delete result;
    }


// -----------------------------------------------------------------------------
// CGSPDataConnectionPlugin::PluginProviderCategory
//
//
// -----------------------------------------------------------------------------
//
TInt CGSPDataConnectionPlugin::PluginProviderCategory() const
    {
    return KGSPluginProviderInternal;
    }


// -----------------------------------------------------------------------------
// CGSPDataConnectionPlugin::ItemType()
//
//
// -----------------------------------------------------------------------------
//
TGSListboxItemTypes CGSPDataConnectionPlugin::ItemType()
    {
    return EGSItemTypeSettingDialog;
    }


// -----------------------------------------------------------------------------
// CGSPDataConnectionPlugin::GetValue()
//
//
// -----------------------------------------------------------------------------
//
void CGSPDataConnectionPlugin::GetValue( const TGSPluginValueKeys aKey,
                                         TDes& aValue )
    {
    TRAPD( err, GetValueL( aValue ) );
    if ( err != KErrNone )
        {
        CGSPluginInterface::GetValue( aKey, aValue );
        }
    }


// -----------------------------------------------------------------------------
// CGSPDataConnectionPlugin::GetValueL()
//
// Leaving version of GetValue()
// -----------------------------------------------------------------------------
//
void CGSPDataConnectionPlugin::GetValueL( TDes& aValue )
    {
    TInt attachState = iModel->PDataAttachL();
    HBufC* result;

    switch( attachState )
        {
        case KAttachSettingWhenAvailable:
            result = StringLoader::LoadL( R_GS_PDATACONN_WHEN_ENABLE_TEXT );
            aValue.Copy( *result );
            delete result;
            break;

        case KAttachSettingWhenNeeded:
            result = StringLoader::LoadL( R_GS_PDATACONN_WHEN_NEEDED_TEXT );
            aValue.Copy( *result );
            delete result;
         default:
            break;
        }

    }


// -----------------------------------------------------------------------------
// CGSPDataConnectionPlugin::HandleSelection()
//
//
// -----------------------------------------------------------------------------
//
void CGSPDataConnectionPlugin::HandleSelection(
    const TGSSelectionTypes aSelectionType )
    {
    switch( aSelectionType )
        {
        case EGSSelectionBySelectionKey:
        case EGSSelectionByMenu:
            TRAP_IGNORE( SetAttachStateL() ); // Launch setting page to change value.
            break;
        default:
            break;
        }

    }


// -----------------------------------------------------------------------------
// CGSPDataConnectionPlugin::ShowAttachSettingPageL()
//
// Indexes of Attach on/off items in a listbox are not the same as aPDataConnectionState
// value. Therefore matching between these two is required.
// -----------------------------------------------------------------------------
//
TBool CGSPDataConnectionPlugin::ShowAttachSettingPageL( TInt& aPDataConnectionState )
    {
    TInt previousPDataConnectionState = aPDataConnectionState;
    TBool isValueUpdated = EFalse;
    
    CDesCArrayFlat* items = iCoeEnv->ReadDesC16ArrayResourceL( 
                            R_GS_PDATACONNECTION_SETTING_PAGE_LBX );
    CleanupStack::PushL( items );
    
    if( items->Count() != 2 )
        {
        CAknRadioButtonSettingPage* dlg =
            new (ELeave) CAknRadioButtonSettingPage
                                     ( R_GS_PDATACONNECTION_SETTING_PAGE,
                                       aPDataConnectionState,
                                       items );
        dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged );
        }
    else
        {
        if (aPDataConnectionState == 0)
            {
            aPDataConnectionState = 1;
            }
        else
            {
            aPDataConnectionState = 0;
            }
        }
    
    CleanupStack::PopAndDestroy( items );
    
    // Check if aPDataConnectionState has been changed:
    if( previousPDataConnectionState != aPDataConnectionState )
        {
        isValueUpdated = ETrue;
        }
    return isValueUpdated;
    }


// -----------------------------------------------------------------------------
// CGSPDataConnectionPlugin::SetAttachStateL()
//
//
// -----------------------------------------------------------------------------
//
void CGSPDataConnectionPlugin::SetAttachStateL()
    {
    // Get information of if there are open connections
    TInt PDataConnectionState = iModel->PDataAttachL();
    const TBool isValueUpdated = ShowAttachSettingPageL( PDataConnectionState );
    
    if ( isValueUpdated )
        {
        // Store updated Active Idle state:
        iModel->SetPDataAttachL( PDataConnectionState );
        // Update Attach item's value to lbx:
        CGSParentPlugin* parent = static_cast<CGSParentPlugin*>(
            AppUi()->View( KGSPDataPluginUid ) );// This is the parent plugin
        parent->UpdateView();
        }
    }


// -----------------------------------------------------------------------------
// CGSPDataConnectionPlugin::OpenLocalizedResourceFileL()
//
//
// -----------------------------------------------------------------------------
//
void CGSPDataConnectionPlugin::OpenLocalizedResourceFileL(
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
// CGSPDataConnectionPlugin::TGSMenuActivationItems()
//
//
// -----------------------------------------------------------------------------
//
TGSMenuActivationItems CGSPDataConnectionPlugin::MenuActivationItem()
    {
    return EGSMenuActivationItemChange;
    }


// ----------------------------------------------------------------------------
// CGSPDataConnectionPlugin
//
// Provides the visibility status of self to framework.
// ----------------------------------------------------------------------------
//
TBool CGSPDataConnectionPlugin::Visible() const
    {
    if ( FeatureManager::FeatureSupported( KFeatureIdIpPush ))
        {
        return EFalse;
        }
    return ETrue;
    }

// End of file
