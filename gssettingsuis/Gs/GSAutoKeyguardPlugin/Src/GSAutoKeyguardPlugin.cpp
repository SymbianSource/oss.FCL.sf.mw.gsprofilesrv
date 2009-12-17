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
* Description:  View for Autokeyguard sub-folder
*
*/


// INCLUDE FILES
#include <coeaui.h>
#include <hlplch.h>             // For HlpLauncher
#include <bautils.h>
#include <gulicon.h>
#include <eikfrlbd.h>
#include <eiktxlbx.h>
#include <aknradiobuttonsettingpage.h>
#include <aknPopup.h>
#include <aknlists.h>
#include <RSSSettings.h>
#include <AknQueryDialog.h>
#include <aknnotedialog.h>
#include <aknViewAppUi.h>
#include <bldvariant.hrh>
#include <featmgr.h>
#include <StringLoader.h>
#include <BTSapDomainPSKeys.h>
#include <e32property.h>
#include <gsautokeyguardplugin.mbg>
#include <GSAutoKeyguardPluginRsc.rsg>
#include <gsprivatepluginproviderids.h>
#include <gsmainview.h>
#include <gsbasecontainer.h>

#include "GSAutoKeyguardPlugin.h"
#include "GSAutoKeyguardPluginContainer.h"
#include "GSAutoKeyguardPlugin.hrh"
#include <centralrepository.h> 
#include "GSAutoKeyguardPluginAutoKeyguardSettingPage.h"
#include "GSAutoKeyguardPluginAutoKeyguardTimeQuery.h"


// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

_LIT( KGSAutoKeyguardPluginResourceFileName, "z:GSAutoKeyguardPluginRsc.rsc" );
_LIT( KGSAutoKeyguardPluginIconDirAndName, "z:GSAutoKeyguardPlugin.mbm");
// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

/**
* CCugQuery
* It defines CCugQuery used in closed user group settings
*/
class CCugQuery
    : public CAknNumberQueryDialog
    {
    public: // Constructors and destructors
        /**
        * C++ constructor.
        */
        CCugQuery( TInt& aNumber,const TTone aTone = ENoTone );
    protected: // From base classes
        /**
        * From CCAknNumberQueryDialog Left softkey is allways OK.
        */
        void  UpdateLeftSoftKeyL();
    };

// ============================= LOCAL FUNCTIONS ==============================

// ========================= MEMBER FUNCTIONS =================================

// ---------------------------------------------------------------------------
// GSAutoKeyguardPlugin::NewL()
// 
// ---------------------------------------------------------------------------
CGSAutoKeyguardPlugin* CGSAutoKeyguardPlugin::NewL( TAny* /*aInitParams*/ )
    {
    CGSAutoKeyguardPlugin* self = new( ELeave ) CGSAutoKeyguardPlugin();
    
    CleanupStack::PushL( self );
    self->ConstructL();    
    CleanupStack::Pop( self );

    return self;
    }


// ---------------------------------------------------------------------------
// GSAutoKeyguardPlugin::CGSAutoKeyguardPlugin()
// 
// ---------------------------------------------------------------------------
CGSAutoKeyguardPlugin::CGSAutoKeyguardPlugin()
    : iResourceLoader( *iCoeEnv )
    {    
    }


// ---------------------------------------------------------------------------
// CGSAutoKeyguardPlugin::ConstructL()
// 
// Symbian OS two-phased constructor
// ---------------------------------------------------------------------------
void CGSAutoKeyguardPlugin::ConstructL()
    {
    FeatureManager::InitializeLibL();

    iModel = CGSAutoKeyguardPluginModel::NewL();
    //PS listener initialization
    iBtSapListener = CGSPubSubsListener::NewL( 
                     KPSUidBluetoothSapConnectionState,
                     KBTSapConnectionState, this );

    // Find the resource file
    TParse parse;
    parse.Set( KGSAutoKeyguardPluginResourceFileName, 
               &KDC_RESOURCE_FILES_DIR, NULL );
    TFileName fileName( parse.FullName() );
    
    // Get language of resource file
    BaflUtils::NearestLanguageFile( iCoeEnv->FsSession(), fileName );

    // Open resource file
    iResourceLoader.OpenL( fileName );
    
    BaseConstructL( R_GS_AUTO_KEYGUARD_VIEW );

    }


// ---------------------------------------------------------------------------
// CGSAutoKeyguardPlugin::~CGSAutoKeyguardPlugin()
// 
// 
// ---------------------------------------------------------------------------
CGSAutoKeyguardPlugin::~CGSAutoKeyguardPlugin()
    {
    FeatureManager::UnInitializeLib();
    // close resource loader
    iResourceLoader.Close();
    
    if ( iModel )
        {
            delete iModel;
        }
        
    if(iBtSapListener)
        {
            delete iBtSapListener;
        }
 
    }


// ---------------------------------------------------------------------------
// TUid CGSAutoKeyguardPlugin::Id()
// 
// 
// ---------------------------------------------------------------------------
TUid CGSAutoKeyguardPlugin::Id() const
    {
    return KGSAutoKeyguardPluginUid;
    }


// ---------------------------------------------------------------------------
// CGSAutoKeyguardPlugin::HandleCommandL()
// 
// 
// ---------------------------------------------------------------------------
void CGSAutoKeyguardPlugin::HandleCommandL( TInt aCommand )
    {
    switch ( aCommand )
        {
        case EGSCmdAppChange:
        case EAknSoftkeyChange:
            {           
             HandleListBoxSelectionL();
            }
            break;
        case EAknSoftkeyBack:
            iAppUi->ActivateLocalViewL( KGSDeviceManagementPluginUid );  
            break;
        case EAknCmdHelp:
            {
            if( FeatureManager::FeatureSupported( KFeatureIdHelp ) )
                {
                HlpLauncher::LaunchHelpApplicationL(
                    iEikonEnv->WsSession(), iAppUi->AppHelpContextL() );
                }
            }
            break;
        default:
            iAppUi->HandleCommandL( aCommand );
            break;
        }
    }


// ---------------------------------------------------------------------------
// CGSAutoKeyguardPlugin::UpdateListBoxL
// 
// Update the current item in the listbox.
// ---------------------------------------------------------------------------
void CGSAutoKeyguardPlugin::UpdateListBoxL( TInt aItemId )
    {
    Container()->UpdateListBoxL( aItemId );
    }


// ---------------------------------------------------------------------------
// CGSAutoKeyguardPlugin::DoActivateL(...)
// 
// 
// ---------------------------------------------------------------------------
void CGSAutoKeyguardPlugin::DoActivateL( const TVwsViewId& aPrevViewId, 
                                   TUid aCustomMessageId, 
                                   const TDesC8& aCustomMessage )
    {
    CGSBaseView::DoActivateL( aPrevViewId, aCustomMessageId, aCustomMessage );    
    }


// ----------------------------------------------------------------------------
// CGSAutoKeyguardPlugin::Container
// 
// Return handle to container class.
// ----------------------------------------------------------------------------
//
CGSAutoKeyguardPluginContainer* CGSAutoKeyguardPlugin::Container()
    {
    return static_cast<CGSAutoKeyguardPluginContainer*>( iContainer );
    }


// ---------------------------------------------------------------------------
// CGSAutoKeyguardPlugin::NewContainerL()
// 
// Creates new iContainer.
// ---------------------------------------------------------------------------
//
void CGSAutoKeyguardPlugin::NewContainerL()
    {
    iContainer = new( ELeave ) CGSAutoKeyguardPluginContainer( iModel );
    }


// ---------------------------------------------------------------------------
// CGSAutoKeyguardPlugin::HandleListBoxSelectionL()
// 
// 
// ---------------------------------------------------------------------------
void CGSAutoKeyguardPlugin::HandleListBoxSelectionL() 
    {
    const TInt currentFeatureId = Container()->CurrentFeatureId();
    TInt maxPeriod(0);
    switch ( currentFeatureId )
        {
        case EGSSettIdAutoKeyguard:
            maxPeriod = iModel->AutoKeyguardMaxPeriod();
            SetAutoKeyguardTimeL(iModel->AutoKeyguardPeriod(), maxPeriod);
        	break;
        default:
            break;
        }
    
    }

// ---------------------------------------------------------------------------
// CGSAutoKeyguardPlugin::SetAutoKeyguardTimeL()
// 
// 
// ---------------------------------------------------------------------------
void CGSAutoKeyguardPlugin::SetAutoKeyguardTimeL(TInt aPeriod, TInt aMaxPeriod)
    {
    TInt currentItem = 0; 
    CDesCArrayFlat* items =  iCoeEnv->ReadDesC16ArrayResourceL( R_AUTOKEYGUARD_LBX );
    CleanupStack::PushL(items);
    if (aPeriod == 0)
        {
        currentItem = 0;  // autokeyguard off
        }
    else
        {
        currentItem = 1;  // user defined
        }
    TInt64 periodInMicroseconds(aPeriod);
    periodInMicroseconds *= 1000000;
	TTime period(periodInMicroseconds);
	
	TInt64 maxPeriodInMicroseconds(aMaxPeriod);
    maxPeriodInMicroseconds *= 1000000;	
	TTime maximum(maxPeriodInMicroseconds);
	
    CAutoKeyguardSettingPage* dlg = new (ELeave)CAutoKeyguardSettingPage(R_AUTOKEYGUARD_SETTING_PAGE, 
                                                                            currentItem, items, &period, maxPeriodInMicroseconds);
    if (!dlg->ExecuteLD(CAknSettingPage::EUpdateWhenChanged))
        {
        CleanupStack::PopAndDestroy();    // items       
        return;
        }
    CleanupStack::PopAndDestroy();    // items
    TDateTime keyGuardTime = period.DateTime();
    //convert the period to seconds
    TInt newPeriod = (keyGuardTime.Minute() * 60) + keyGuardTime.Second();
    iModel->SetAutoKeyguardPeriod(newPeriod);
    UpdateListBoxL( EGSSettIdAutoKeyguard );
    }
    
// ---------------------------------------------------------------------------
// CGSAutoKeyguardPlugin::HandleResourceChangeL( TInt aType )
// Updates view layout
//  
// ---------------------------------------------------------------------------
//
void CGSAutoKeyguardPlugin::HandleResourceChangeL( TInt aType )
    {
    if( aType == KAknsMessageSkinChange ||
        aType == KEikDynamicLayoutVariantSwitch )
        {
        //iContainer->HandleResourceChangeL( aType );
        }
    }


// ---------------------------------------------------------------------------
// CGSAutoKeyguardPlugin::HandleNotifyPSL
//
// Handling PS keys change
// ---------------------------------------------------------------------------
//  
void CGSAutoKeyguardPlugin::HandleNotifyPSL( const TUid aUid, const TInt& aKey,
                                       const TRequestStatus& /* aStatus */ )
    {
    if ( aUid == KPSUidBluetoothSapConnectionState && 
         aKey == KBTSapConnectionState )
        {
        Visible();
        }
    }


// ========================= From CGSPluginInterface ==================

// ----------------------------------------------------------------------------
// CGSAutoKeyguardPlugin::GetCaption
// 
// Return application/view caption.
// ----------------------------------------------------------------------------
//
void CGSAutoKeyguardPlugin::GetCaptionL( TDes& aCaption ) const
    {
    // the resource file is already opened.
    HBufC* result = StringLoader::LoadL( R_GS_AUTO_KEYGUARD_VIEW_CAPTION );
    
    aCaption.Copy( *result );
    delete result;
    }
    

// ----------------------------------------------------------------------------
// CGSAutoKeyguardPlugin::PluginProviderCategory
// 
// A means to identify the location of this plug-in in the framework.
// ----------------------------------------------------------------------------
//
TInt CGSAutoKeyguardPlugin::PluginProviderCategory() const
    {
    //To identify internal plug-ins.
    return KGSPluginProviderInternal;
    }
    

// ----------------------------------------------------------------------------
// CGSAutoKeyguardPlugin::Visible
// 
// Provides the visibility status of self to framework.
// ----------------------------------------------------------------------------
//    
TBool CGSAutoKeyguardPlugin::Visible() const
    {
    TInt btSapConnectionState;
    TBool visible = ETrue;

    iBtSapListener->Get( btSapConnectionState );

    if ( btSapConnectionState == EBTSapConnected )
        {
        visible = EFalse;
        }
    
    return visible;
    }    
    
// ----------------------------------------------------------------------------
// CGSAutoKeyguardPlugin::DynInitMenuPaneL()
// 
// Display the dynamic menu
// ----------------------------------------------------------------------------
void CGSAutoKeyguardPlugin::DynInitMenuPaneL( TInt aResourceId,
                                          CEikMenuPane* aMenuPane )
    {

	// show or hide the 'help' menu item when supported
    if( aResourceId == R_GS_AUTO_KEYGUARD_MENU_ITEM_HELP )
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

    }
// -----------------------------------------------------------------------------
// CGSCallDivertPlugin::CreateIconL()
//
//
// -----------------------------------------------------------------------------
//
CGulIcon* CGSAutoKeyguardPlugin::CreateIconL( const TUid aIconType )
    {
    //EMbm<Mbm_file_name><Bitmap_name>
    CGulIcon* icon;
    TParse* fp = new( ELeave ) TParse();
    CleanupStack::PushL( fp );
    fp->Set( KGSAutoKeyguardPluginIconDirAndName, &KDC_BITMAP_DIR, NULL );

    if( aIconType == KGSIconTypeLbxItem )
        {
        icon = AknsUtils::CreateGulIconL(
        AknsUtils::SkinInstance(),
        KAknsIIDQgnPropCpDevKeyguard,
        fp->FullName(),
        EMbmGsautokeyguardpluginQgn_prop_cp_dev_keyguard, 
        EMbmGsautokeyguardpluginQgn_prop_cp_dev_keyguard_mask );
        }
    else
        {
        icon = CGSPluginInterface::CreateIconL( aIconType );
        }

    CleanupStack::PopAndDestroy( fp );

    return icon;
    } 
// End of File  
