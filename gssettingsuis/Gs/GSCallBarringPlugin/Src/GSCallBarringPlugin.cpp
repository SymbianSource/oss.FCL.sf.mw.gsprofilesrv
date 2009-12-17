/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  View for the Barring folder
*
*/

 
// INCLUDE FILES
#include "GSCallBarringPlugin.h"
#include "GSCallBarringPluginContainer.h"
#include "GSPhoneSettingConstants.h"
#include "GSInternetBarringView.h"
#include "GSCellularCallBarringView.h"
#include "GSMainBarringView.h"
#include "CallBarringPlugin.hrh"
#include "LocalViewIds.h"
#include "GsLogger.h"

#include <coeaui.h>
#include <hlplch.h>                 // For HlpLauncher
#include <e32base.h>
#include <featmgr.h>
#include <exterror.h>               //for extended error codes
#include <StringLoader.h>           //for StringLoader
#include <PsetContainer.h>          //for CPsetContainer
#include <PsuiContainer.h>          //for CPsuiContainer
#include <eikmenup.h>               //for menu bar
#include <SecUiCodeQueryDialog.h>
#include <nwdefs.h>
#include <aknViewAppUi.h>

#include <gscommon.hrh>
#include <GSCallBarringPluginRsc.rsg>
#include <gscallbarringplugin.mbg>
#include <gsfwviewuids.h>
#include <gsprivatepluginproviderids.h>
#include <BTSapInternalPSKeys.h>
#include <centralrepository.h>
#include <settingsinternalcrkeys.h>

#if defined(__VOIP) && defined(RD_VOIP_REL_2_2)
#include <spsettings.h>
#endif // __VOIP && RD_VOIP_REL_2_2
// LOCAL CONSTANTS
_LIT( KGSCallBarringPluginResourceFileName, "z:GSCallBarringPluginRsc.rsc" );
_LIT( KGSNameOfClass, "CGSCallBarringPlugin" );

enum
    {
    EGSCallBarringPluginPanicNullPtr
    };
 

// ========================= MEMBER FUNCTIONS ================================
// ---------------------------------------------------------------------------
// 
// C++ constructor.
// 
// ---------------------------------------------------------------------------
CGSCallBarringPlugin::CGSCallBarringPlugin()
    {
    }

// ---------------------------------------------------------------------------
// 
// Symbian OS two-phased constructor (second phase)
// 
// ---------------------------------------------------------------------------
void CGSCallBarringPlugin::ConstructL()
    {
    __GSLOGSTRING("[GS]--> CGSCallBarringPlugin::ConstructL");
    FeatureManager::InitializeLibL();
    OpenLocalizedResourceFileL( KGSCallBarringPluginResourceFileName,
                                iResourceLoader );
    //PS listener initialization
    iBtSapListener = CGSPubSubsListener::NewL( 
                     KPSUidBluetoothSapConnectionState,
                     KBTSapConnectionState, this );
    SetCSPActiveL( ETrue );
    iBarringView = NULL;
#if defined(__VOIP) && defined(RD_VOIP_REL_2_2)
    iVoipSupported = EFalse;
    CSPSettings* spSettings = CSPSettings::NewL();
    if ( spSettings->IsFeatureSupported( ESupportVoIPFeature ) &&
        spSettings->IsFeatureSupported( ESupportVoIPSSFeature ) )
        {
        iVoipSupported = ETrue;
        }
    delete spSettings;
#else // __VOIP && RD_VOIP_REL_2_2
    if ( FeatureManager::FeatureSupported( KFeatureIdCommonVoip ) )
        {
        TInt supported( KGSSettingOff );
        CRepository* telephonyrep = 
            CRepository::NewL( KCRUidTelephonySettings );
        telephonyrep->Get( KDynamicVoIP, supported );
        iVoipSupported = KGSSettingOff != supported;
        delete telephonyrep;
        }
#endif // __VOIP && RD_VOIP_REL_2_2
    BaseConstructL( R_GS_BARRING_MAIN_VIEW );
    // VoIP feature supported
    __GSLOGSTRING("[GS] <--CGSCallBarringPlugin::ConstructL");
    }


// ---------------------------------------------------------------------------
// CGSCallBarringPlugin::NewL()
//
// Symbian OS default constructor
// ---------------------------------------------------------------------------
CGSCallBarringPlugin* CGSCallBarringPlugin::NewL( TAny* /*aInitParams*/ )
    {
    CGSCallBarringPlugin* self = new( ELeave ) CGSCallBarringPlugin();

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }


// ---------------------------------------------------------------------------
// 
// Symbian OS two-phased constructor (first phase)
// 
// ---------------------------------------------------------------------------
CGSCallBarringPlugin* CGSCallBarringPlugin::NewLC()
    {
    __GSLOGSTRING("[GS]--> CGSCallBarringPlugin::NewLC");
    CGSCallBarringPlugin* self = new ( ELeave ) CGSCallBarringPlugin;
    CleanupStack::PushL( self );
    self->ConstructL();

    __GSLOGSTRING("[GS] <--CGSCallBarringPlugin::NewLC");
    return self;
    }
 
// ---------------------------------------------------------------------------
// 
// C++ destructor
// 
// ---------------------------------------------------------------------------
CGSCallBarringPlugin::~CGSCallBarringPlugin()
    {
    FeatureManager::UnInitializeLib();
    TRAP_IGNORE( SetCSPActiveL( EFalse ) );
    if ( iBtSapListener )
        {
        delete iBtSapListener;
        }
    }
 
// ---------------------------------------------------------------------------
// 
// Returns Id of the Barring view.
// 
// ---------------------------------------------------------------------------
TUid CGSCallBarringPlugin::Id() const
    {
    return KCallBarringPluginId;
    }
 
// ---------------------------------------------------------------------------
// 
// Handles other than list box events.
// 
// ---------------------------------------------------------------------------
void CGSCallBarringPlugin::HandleCommandL( TInt aCommand )
    {
    __GSLOGSTRING("[GS]--> CGSCallBarringPlugin::HandleCommandL");
		switch ( aCommand )
        {
        case EAknSoftkeyBack:
            RemoveLocalViews();
            iAppUi->ActivateLocalViewL( KGSMainViewUid );
            break;
        case EAknCmdHelp:
            {
            if( FeatureManager::FeatureSupported( KFeatureIdHelp ) )
                {
                HlpLauncher::LaunchHelpApplicationL(
                    iEikonEnv->WsSession(), iAppUi->AppHelpContextL() );
                }
            break;
            }
        default:
            iAppUi->HandleCommandL( aCommand );
            break;
        }
    __GSLOGSTRING("[GS] <--CGSCallBarringPlugin::HandleCommandL");
    }
  
// ---------------------------------------------------------------------------
// 
// Activates view.
// 
// ---------------------------------------------------------------------------
void CGSCallBarringPlugin::DoActivateL( const TVwsViewId& aPrevViewId,
                                        TUid aCustomMessageId,
                                        const TDesC8& aCustomMessage )
    {
    __GSLOGSTRING("[GS]--> CGSCallBarringPlugin::DoActivateL");
    CGSBaseView::DoActivateL( aPrevViewId, aCustomMessageId, aCustomMessage );
    ActivateInternalViewL();
    __GSLOGSTRING("[GS] <--CGSCallBarringPlugin::DoActivateL");
    }
 
// ---------------------------------------------------------------------------
// 
// Deactivates view.
// 
// ---------------------------------------------------------------------------
void CGSCallBarringPlugin::DoDeactivate()
    {
    __GSLOGSTRING("[GS]--> CGSCallBarringPlugin::DoDeactivate");
    CGSBaseView::DoDeactivate();
    __GSLOGSTRING("[GS] <--CGSCallBarringPlugin::DoDeactivate");
    }
 
 
// ---------------------------------------------------------------------------
// 
// Creates new iContainer.
// 
// ---------------------------------------------------------------------------
void CGSCallBarringPlugin::NewContainerL()
    {
    iContainer = new (ELeave) CGSCallBarringPluginContainer();
    }

// ---------------------------------------------------------------------------
// CGSCallBarringPlugin::Container
//
// Returns call container item
// ---------------------------------------------------------------------------
//
CGSCallBarringPluginContainer* CGSCallBarringPlugin::Container()
    {
    return static_cast <CGSCallBarringPluginContainer*> ( iContainer );
    }

// ========================= from CGSPluginInterface ==================

// ----------------------------------------------------------------------------
// CGSCallBarringPlugin::GetCaption
//
// Return application/view caption.
// ----------------------------------------------------------------------------
//
void CGSCallBarringPlugin::GetCaptionL( TDes& aCaption ) const
    {
    // the resource file is already opened.
    HBufC* result = StringLoader::LoadL( R_GS_CALL_BARRING_CAPTION );

    aCaption.Copy( *result );
    delete result;
    }


// ----------------------------------------------------------------------------
// CGSCallBarringPlugin::PluginProviderCategory
//
// A means to identify the location of this plug-in in the framework.
// ----------------------------------------------------------------------------
//
TInt CGSCallBarringPlugin::PluginProviderCategory() const
    {
    //To identify internal plug-ins.
    return KGSPluginProviderInternal;
    }


// ----------------------------------------------------------------------------
// CGSCallBarringPlugin::Visible
//
// Provides the visibility status of self to framework.
// ----------------------------------------------------------------------------
//
TBool CGSCallBarringPlugin::Visible() const
    {
    TBool result( ETrue );
    TInt value = 0;
    RProperty::Get( KPSUidBluetoothSapConnectionState, 
                    KBTSapConnectionState, value );
    if ( value == EBTSapNotConnected || value == EBTSapConnecting )
        {
        //Now checking also for CSP support
        __ASSERT_ALWAYS( iCSP != NULL, User::Panic( KGSNameOfClass, 
                                   EGSCallBarringPluginPanicNullPtr ) );
        TInt err = KErrNone;
        err = iCSP->IsCBSupported( result );
        //CSP modifies result value even if something goes wrong
        //CSP will return KErrNotSupported (-5) in cases when
        //it is not possible to read CSP values from SIM card
        //Call Barring should be still displayed in this case
        if ( err != KErrNone )
            {
            result = ETrue;
            }
        }
    else
        {
        result = EFalse;
        }
    return result;    
    }    

// -----------------------------------------------------------------------------
// CGSCallBarringPlugin::CreateIconL()
//
//
// -----------------------------------------------------------------------------
//
CGulIcon* CGSCallBarringPlugin::CreateIconL( const TUid aIconType )
    {
    //EMbm<Mbm_file_name><Bitmap_name>
    CGulIcon* icon;
    TParse* fp = new( ELeave ) TParse();
    CleanupStack::PushL( fp );
    fp->Set( KGSCallBarringPluginIconDirAndName, &KDC_BITMAP_DIR, NULL );

    if( aIconType == KGSIconTypeLbxItem )
        {
        icon = AknsUtils::CreateGulIconL(
        AknsUtils::SkinInstance(),
        KAknsIIDQgnPropSetBarrSub,
        fp->FullName(),
        EMbmGscallbarringpluginQgn_prop_set_barr_sub,
        EMbmGscallbarringpluginQgn_prop_set_barr_sub_mask );
        }
    else
        {
        icon = CGSPluginInterface::CreateIconL( aIconType );
        }

    CleanupStack::PopAndDestroy( fp );

    return icon;
    }


// ---------------------------------------------------------------------------
// CGSCallBarringPlugin::HandleNotifyPSL
//
// Handling PS keys change
// ---------------------------------------------------------------------------
//  
void CGSCallBarringPlugin::HandleNotifyPSL( const TUid aUid, const TInt& aKey,
                                          const TRequestStatus& /* aStatus */ )
    {
    if ( aUid == KPSUidBluetoothSapConnectionState && 
         aKey == KBTSapConnectionState )
        {
        Visible();
        }
    }

// ----------------------------------------------------------------------------
// CGSCallBarringPlugin::SetCSPActiveL
// 
// Set Customer Service Profile active
// ----------------------------------------------------------------------------
//
void CGSCallBarringPlugin::SetCSPActiveL( TBool aValue )
    {
    if ( aValue )
        {
        iCSP = CPsetCustomerServiceProfile::NewL();
        User::LeaveIfError( iCSP->OpenCSProfileL() );
        }
    else
        {        
        delete iCSP;
        iCSP = NULL;
        }
    }

// ---------------------------------------------------------------------------
// CGSCallBarringPlugin::HandleClientRectChange
//
// Handle changes to rect().
// ---------------------------------------------------------------------------
//
void CGSCallBarringPlugin::HandleClientRectChange()
    {
    if ( iContainer )
        {
        iContainer->SetRect( ClientRect() );
        }
    }

// ---------------------------------------------------------------------------
// CGSCallBarringPlugin::ActivateInternalViewL
//
// Activate local views within this plug-in.
// ---------------------------------------------------------------------------
void CGSCallBarringPlugin::ActivateInternalViewL()
    {
    TInt btSapState = 0; 
    RProperty::Get( KPSUidBluetoothSapConnectionState,
                    KBTSapConnectionState,
                    btSapState );

    if ( iVoipSupported )
        {
        CreateLocalViewL( KCallMainBarringViewId );
        }
    else
        {
        if ( btSapState == EBTSapNotConnected )
            {
            CreateLocalViewL( KCallBarringViewId );
            }
        }

    }

// ---------------------------------------------------------------------------
// CGSCallBarringPlugin::CreateLocalViewL
//
// Initializes local views within this plug-in based on the view ID.
// ---------------------------------------------------------------------------
void CGSCallBarringPlugin::CreateLocalViewL( TUid aLocalViewId )
    {
    CGSLocalBaseView* view;

    // Check if the view exists. If view does not exist:
    // 1. Create the view
    // 2. Add view to cleanupstack (NewLC)
    // 3. Add view to iAppUi -> iAppUi takes the view ownership.
    // 4. Remove view from cleanupstack
    // 5. Assign pointer of created view to member variable
    //
    if ( aLocalViewId == KCallBarringViewId )
        {
        //Check if BarringView already exists
        if ( !iBarringView )
            {
            view = CGSSettListBarringView::NewLC();
            iAppUi->AddViewL( view );
            CleanupStack::Pop( view );
            iBarringView = view;
            }
        iAppUi->ActivateLocalViewL( aLocalViewId );
        }
    else if ( aLocalViewId == KCallMainBarringViewId )
        {
        TUid viewId = aLocalViewId;
        if ( !iBarringView )
            {            
            if ( FeatureManager::FeatureSupported( KFeatureIdRestrictedCallBarring ) )
                {
                // create the barring view directly if it's in restricted
                // call barring
                view = CGSSettListBarringView::NewLC();
                viewId = view->Id();
                }
            else
                {
                view = CGSSettListMainBarringView::NewLC();
                }
            iAppUi->AddViewL( view );
            CleanupStack::Pop( view );
            iBarringView = view;    
            }
        // Activate the correct view in case restricted call barring is
        // active
        iAppUi->ActivateLocalViewL( viewId );    
        }

    }


// ---------------------------------------------------------------------------
// CGSCallBarringPlugin::RemoveLocalViews
//
//
// ---------------------------------------------------------------------------
//
void CGSCallBarringPlugin::RemoveLocalViews()
    {
    // Remove view from iAppUi -> View is deleted by iAppUi automatically.
    if ( iBarringView )
        {
        if ( iVoipSupported )
            {
            iAppUi->RemoveView( KCallMainBarringViewId );// Also deletes view.
            }
        else
            {
            iAppUi->RemoveView( KCallBarringViewId );// Also deletes view.
            }    
        iBarringView = NULL;
        }
    }

// ---------------------------------------------------------------------------
// CGSCallBarringPlugin::HandleListBoxSelectionL
//
// Handle any user actions while in the list view.
// ---------------------------------------------------------------------------
void CGSCallBarringPlugin::HandleListBoxSelectionL()
    {
    }

// ---------------------------------------------------------------------------
// CGSCallBarringPlugin::VoIPSupported
//
// Is VoIP feature supported.
// ---------------------------------------------------------------------------

TBool CGSCallBarringPlugin::VoipSupported() const 
    {
    return iVoipSupported;
    }

 // End of file
