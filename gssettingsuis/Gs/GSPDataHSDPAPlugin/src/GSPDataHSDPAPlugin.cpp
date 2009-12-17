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
* Description:  GSPDataHSDPAPlugin implementation.
*
*/


// User includes
#include    "GSPDataHSDPAPlugin.h"
#include    "GSPDataHSDPAModel.h"
#include    <gsparentplugin.h>
#include    <gscommon.hrh>
#include    <GSPDataHSDPAPluginRsc.rsg> // GUI Resource
#include    <gsprivatepluginproviderids.h>
#include    <gsfwviewuids.h>

// System includes
#include    <AknWaitDialog.h>
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
#include    <NetworkHandlingDomainPSKeys.h>
#include 	<CoreApplicationUIsSDKCRKeys.h>
#include 	<AknWaitDialog.h>
#include    <ProEngFactory.h>
#ifdef RD_STARTUP_CHANGE
#include 	<starterclient.h>
#endif
#include <centralrepository.h>
#include <settingsinternalcrkeys.h>
#include "SettingsPrivateCRKeys.h"

// Constants

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSPDataHSDPAPlugin::CGSPDataHSDPAPlugin()
// Constructor
//
// ---------------------------------------------------------------------------
//
CGSPDataHSDPAPlugin::CGSPDataHSDPAPlugin()
    : iResources( *iCoeEnv )
    {
    }

    
// ---------------------------------------------------------------------------
// CGSPDataHSDPAPlugin::~CGSPDataHSDPAPlugin()
// Destructor
//
// ---------------------------------------------------------------------------
//
CGSPDataHSDPAPlugin::~CGSPDataHSDPAPlugin()
    {
    FeatureManager::UnInitializeLib();
    if ( iNwModeListener )
        {
        delete iNwModeListener;
        }
        
    iResources.Close();

    if( iModel )
        {
        delete iModel;
        }
    if( iProfileEngine )
        {
        iProfileEngine->Release();
        iProfileEngine = NULL;        
        }
    if( iNotifyHandler )
        {
        delete iNotifyHandler;
        iNotifyHandler = NULL;
        }
    }


// ---------------------------------------------------------------------------
// CGSPDataHSDPAPlugin::ConstructL(const TRect& aRect)
// Symbian OS two-phased constructor
//
// ---------------------------------------------------------------------------
//
void CGSPDataHSDPAPlugin::ConstructL()
    {
    FeatureManager::InitializeLibL();
    iModel = CGSPDataHSDPAModel::NewL();
    OpenLocalizedResourceFileL( KPDataHSDPAResourceFileName, 
                                iResources );
    //PS listener initialization
    iNwModeListener = CGSPubSubsListener::NewL( 
                     KPSUidNetworkInfo,
                     KNWTelephonyNetworkMode, this );
    iNwModeListener->Get( iNetworkMode );
    
    iProfileEngine = CreateProfileEngineL();
    
    iNotifyHandler = ProEngFactory::NewNotifyHandlerL();    
    iNotifyHandler->RequestProfileActivationNotificationsL( *this );
    
    //Checking if HSDPA setting is suppose to be visible
    CRepository* hsdpaRepository = CRepository::NewL( KCRUidNetworkSettings );
    User::LeaveIfError( hsdpaRepository->Get( KSettingsHSDPAVisbility, 
                                        iHSDPASettingSupport ) );
    
    BaseConstructL( R_GS_PDATAHSDPA_VIEW );
    
    delete hsdpaRepository;
    }


// ---------------------------------------------------------------------------
// CGSPDataHSDPAPlugin::NewL()
// Static constructor
//
// ---------------------------------------------------------------------------
//
CGSPDataHSDPAPlugin* CGSPDataHSDPAPlugin::NewL( TAny* /*aInitParams*/ )
    {
    CGSPDataHSDPAPlugin* self = 
                                   new( ELeave ) CGSPDataHSDPAPlugin();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CGSPDataHSDPAPlugin::Id
//
//
// -----------------------------------------------------------------------------
//
TUid CGSPDataHSDPAPlugin::Id() const
    {
    return KGSPDataHSDPAPluginUID;
    }

// -----------------------------------------------------------------------------
// CGSPDataHSDPAPlugin::DoActivateL
//
//
// -----------------------------------------------------------------------------
//
void CGSPDataHSDPAPlugin::DoActivateL( const TVwsViewId& /*aPrevViewId*/,
                                  TUid /*aCustomMessageId*/,
                                  const TDesC8& /*aCustomMessage*/ )
    {
    }


// -----------------------------------------------------------------------------
// CGSPDataHSDPAPlugin::DoDeactivate
//
//
// -----------------------------------------------------------------------------
//
void CGSPDataHSDPAPlugin::DoDeactivate()
    {
    }


// ========================= From CGSPluginInterface ==================


// -----------------------------------------------------------------------------
// CGSPDataHSDPAPlugin::GetCaptionL
//
//
// -----------------------------------------------------------------------------
//
void CGSPDataHSDPAPlugin::GetCaptionL( TDes& aCaption ) const
    {
    HBufC* result = StringLoader::LoadL( R_GS_PDATAHSDPA_PLUGIN_CAPTION );
    aCaption.Copy( *result );
    delete result;
    }


// -----------------------------------------------------------------------------
// CGSPDataHSDPAPlugin::PluginProviderCategory
//
//
// -----------------------------------------------------------------------------
//
TInt CGSPDataHSDPAPlugin::PluginProviderCategory() const
    {
    return KGSPluginProviderInternal;
    }


// -----------------------------------------------------------------------------
// CGSPDataHSDPAPlugin::ItemType()
//
//
// -----------------------------------------------------------------------------
//
TGSListboxItemTypes CGSPDataHSDPAPlugin::ItemType()
    {
    return EGSItemTypeSettingDialog;
    }


// -----------------------------------------------------------------------------
// CGSPDataHSDPAPlugin::GetValue()
//
//
// -----------------------------------------------------------------------------
//
void CGSPDataHSDPAPlugin::GetValue( const TGSPluginValueKeys aKey,
                                              TDes& aValue )
    {
    TRAPD( err, GetValueL( aValue ) );
    if ( err != KErrNone )
        {
        CGSPluginInterface::GetValue( aKey, aValue );
        }
    }


// -----------------------------------------------------------------------------
// CGSPDataHSDPAPlugin::GetValueL()
//
// Leaving version of GetValue()
// -----------------------------------------------------------------------------
//
void CGSPDataHSDPAPlugin::GetValueL( TDes& aValue )
    {
    const TInt hsdpaState = iModel->CurrentHSDPAItemL();
    HBufC* result;

    switch( hsdpaState )
        {
        case KSettingHsdpaEnable:
            result = StringLoader::LoadL( R_GS_HSDPA_ENABLE_TEXT );
            aValue.Copy( *result );
            delete result;
            break;
        case KSettingHsdpaDisable:
            result = StringLoader::LoadL( R_GS_HSDPA_DISABLE_TEXT );
            aValue.Copy( *result );
            delete result;
         default:
            break;
        }

    }


// -----------------------------------------------------------------------------
// CGSPDataHSDPAPlugin::HandleSelection()
//
//
// -----------------------------------------------------------------------------
//
void CGSPDataHSDPAPlugin::HandleSelection(
    const TGSSelectionTypes aSelectionType )
    {
    switch( aSelectionType )
        {
        // Launch setting page to change value.
        case EGSSelectionBySelectionKey:
        case EGSSelectionByMenu:
            TRAP_IGNORE( SetHSDPAStateL() );  
            break;
        default:
            break;
        }

    }


// -----------------------------------------------------------------------------
// CGSPDataHSDPAPlugin::ShowHSDPASettingPageL()
//
// Indexes of HSDPA on/off items in a listbox are not the same as aPDataHSDPAState
// value. Therefore matching between these two is required.
// -----------------------------------------------------------------------------
//
TBool CGSPDataHSDPAPlugin::ShowHSDPASettingPageL( TInt& aHsdpaState )
    {
    TInt prevHsdpaState = aHsdpaState;
    TInt selectedIndex;
    TBool isValueUpdated = EFalse;

    // Match HSDPA state to lbx item index:
    switch( aHsdpaState )
        {
        case KSettingHsdpaEnable:
            selectedIndex = KSettingHsdpaDisable;
            break;
        case KSettingHsdpaDisable:
            selectedIndex = KSettingHsdpaEnable;
            break;
        }
    CDesCArrayFlat* items = iCoeEnv->ReadDesC16ArrayResourceL( 
                            R_GS_PDATAHSDPA_SETTING_PAGE_LBX );
    CleanupStack::PushL( items );

    CAknRadioButtonSettingPage* dlg =
        new (ELeave) CAknRadioButtonSettingPage
                                  ( R_GS_PDATAHSDPA_SETTING_PAGE,
                                    selectedIndex,
                                    items );
    dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged );
    CleanupStack::PopAndDestroy( items );

    // Match selected lbx item index to HSDPA state:
    switch( selectedIndex )
        {
        case KSettingHsdpaEnable:
            aHsdpaState = KSettingHsdpaDisable;
            break;
        case KSettingHsdpaDisable:
            aHsdpaState = KSettingHsdpaEnable;
            break;
        }
    // Check if aPDataHSDPAState has been changed:
    if( prevHsdpaState != aHsdpaState )
        {
        TInt connectionErr(KErrNone);
		TInt networkStatus;
	
		CRepository* repository = CRepository::NewL( KCRUidCoreApplicationUIs );
		connectionErr = repository->Get( KCoreAppUIsNetworkConnectionAllowed, 
		                                 networkStatus );
		delete repository;
		
		if ( connectionErr == KErrNone )
			{
			CAknQueryDialog* resetNote = CAknQueryDialog::NewL();
		    if( resetNote->ExecuteLD( R_CONFIRM_NW_RESET_HSDPA ) )
            	{
        		ResetActiveConnectionsL();
        		isValueUpdated = ETrue;
				}
			}
        }
    return isValueUpdated;
    }

// -----------------------------------------------------------------------------
// CGSPDataHSDPAPlugin::SetHSDPAStateL()
//
//
// -----------------------------------------------------------------------------
//
void CGSPDataHSDPAPlugin::SetHSDPAStateL()
    {
    // Get information of if there are open connections
    TInt hsdpaState = iModel->CurrentHSDPAItemL();
    const TBool isValueUpdated = ShowHSDPASettingPageL( hsdpaState );
    if ( isValueUpdated )
        {
        // Store updated Active Idle state:
        iModel->SetHSDPAItemValueL( hsdpaState );
        // Update HSDPA item's value to lbx:
        UpdateParentViewL();
        }
    }

// -----------------------------------------------------------------------------
// CGSPDataHSDPAPlugin::OpenLocalizedResourceFileL()
//
//
// -----------------------------------------------------------------------------
//
void CGSPDataHSDPAPlugin::OpenLocalizedResourceFileL(
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
// CGSPDataHSDPAPlugin::TGSMenuActivationItems()
//
//
// -----------------------------------------------------------------------------
//
TGSMenuActivationItems CGSPDataHSDPAPlugin::MenuActivationItem()
    {
    return EGSMenuActivationItemChange;
    }


// ----------------------------------------------------------------------------
// CGSPDataHSDPAPlugin
//
// Provides the visibility status of self to framework.
// ----------------------------------------------------------------------------
//
TBool CGSPDataHSDPAPlugin::Visible() const
    {
    // We check for if HSDPA item is supported to make it visble
    TBool hsdpaVisible = EFalse;
    if ( FeatureManager::FeatureSupported( KFeatureIdHsxpaSupport ) &&
         iNetworkMode == ENWNetworkModeWcdma &&
         iProfileEngine->ActiveProfileId() != EProfileOffLineId &&
         iHSDPASettingSupport )
        {
        hsdpaVisible = ETrue;
        }
    return hsdpaVisible;
    }

// ---------------------------------------------------------------------------
// CGSPDataHSDPAPlugin::ResetActiveConnectionsL
//
// All Active connections if any, are reset
// instead of rebooting the phone, this is all
// to make sure that HSDPA setting item is enabled or disabled
// from UI level which now doesnt conflict to exact functionality of CoreUi
// ---------------------------------------------------------------------------
//  
void CGSPDataHSDPAPlugin::ResetActiveConnectionsL()
{
#ifdef RD_STARTUP_CHANGE
	RStarterSession rSession;
	
	if( iProfileEngine->ActiveProfileId() != EProfileOffLineId )
		{	
		User::LeaveIfError( rSession.Connect() );		
		rSession.SetState( RStarterSession::EOffline );
		rSession.SetState( RStarterSession::ENormal );
		rSession.Close();
		}
    
#endif
}

// ---------------------------------------------------------------------------
// CGSPDataHSDPAPlugin::HandleNotifyPSL
//
// Handling PS keys change
// ---------------------------------------------------------------------------
//  
void CGSPDataHSDPAPlugin::HandleNotifyPSL( const TUid aUid, const TInt& aKey,
                                          const TRequestStatus& /* aStatus */ )
    {
    // We check the NW mode value from PS Key
    if ( aUid == KPSUidNetworkInfo && 
         aKey == KNWTelephonyNetworkMode )
        {
        iNwModeListener->Get( iNetworkMode );
        }
	UpdateParentViewL();     
    }


// ----------------------------------------------------------------------------
// CGSPDataHSDPAPlugin::HandleProfileActivatedL
// From MProEngProfileActivationObserver
//
// Callback for device profile change
// Used for observing offline mode activation
// ----------------------------------------------------------------------------
//
void CGSPDataHSDPAPlugin::HandleProfileActivatedL( TInt /*aProfileId*/ )
    {
    UpdateParentViewL();
    }

// ----------------------------------------------------------------------------
// CGSPDataHSDPAPlugin::UpdateParentViewL
// 
// Updating parent view
// ----------------------------------------------------------------------------
//
void CGSPDataHSDPAPlugin::UpdateParentViewL()
    {    
    CGSParentPlugin* parent = static_cast<CGSParentPlugin*>(
            AppUi()->View( KGSPDataPluginUid ) );// This is the parent plugin
            
    // Now let's check if Standby view is active
    TGSCustomOperationType paramValue = EGSCustomOperationViewActive;
    TAny* param = &paramValue;
    TAny* returnValue = parent->CustomOperationL( param, NULL );
    TBool* ViewActive = static_cast<TBool*> ( returnValue );
    
    if ( *ViewActive )
        {
        if ( parent )
            {
            parent->UpdateView();
            }
        }
    }
    
// End of file
