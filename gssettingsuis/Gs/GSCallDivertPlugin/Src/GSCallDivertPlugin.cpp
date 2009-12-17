/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Main view handling for Call Divert.
*
*/


// INCLUDE FILES
#include "GSCallDivertPlugin.h"
#include "GSCallDivertPluginContainer.h"
#include "GSCallDivertView.h"
#include "LocalViewId.h"
#include "GSCallDivertModel.h"

#include <gscommon.hrh>
#include <coeaui.h>
#include <hlplch.h>             // For HlpLauncher
#include <aknViewAppUi.h>
#include <GSCallDivertPluginRsc.rsg>
#include <gscalldivertplugin.mbg>
#include <StringLoader.h>
#include <featmgr.h>
#include <gsfwviewuids.h>
#include <gscommon.hrh>
#include <gsprivatepluginproviderids.h>
#include <BTSapInternalPSKeys.h>

// LOCAL CONSTANTS
_LIT( KGSCallDivertPluginResourceFileName, "z:GSCallDivertPluginRsc.rsc" );
_LIT( KGSNameOfClass, "CGSCallDivertPlugin" );
enum
    {
    EGSCallDivertPluginPanicNullPtr
    };

// ========================= MEMBER FUNCTIONS ================================
// ---------------------------------------------------------------------------
// CGSCallDivertPlugin::CGSCallDivertPlugin
//
// C++ constructor.
// ---------------------------------------------------------------------------
CGSCallDivertPlugin::CGSCallDivertPlugin()
    {
    }

// ---------------------------------------------------------------------------
// CGSCallDivertPlugin::ConstructL
//
// Symbian OS two-phased constructor
// ---------------------------------------------------------------------------
void CGSCallDivertPlugin::ConstructL()
    {
    FeatureManager::InitializeLibL();
    OpenLocalizedResourceFileL( KGSCallDivertPluginResourceFileName,
                                iResourceLoader );

    //PS listener initialization
    iBtSapListener = CGSPubSubsListener::NewL(
                     KPSUidBluetoothSapConnectionState,
                     KBTSapConnectionState, this );

    SetCSPActiveL( ETrue );
    BaseConstructL( R_CF_MAIN_VIEW );

    iCallDivertModel = CGSCallDivertModel::NewL();
    CGSCallDivertView* callDivertView = CGSCallDivertView::NewLC( iCallDivertModel );
    iAppUi->AddViewL( callDivertView );
    CleanupStack::Pop( callDivertView );
    iCallDivertView = callDivertView;
    }

// ---------------------------------------------------------------------------
// CGSCallDivertPlugin::~CGSCallDivertPlugin
//
// Destructor.
// ---------------------------------------------------------------------------
CGSCallDivertPlugin::~CGSCallDivertPlugin()
    {
    FeatureManager::UnInitializeLib();
    TRAP_IGNORE( SetCSPActiveL( EFalse ) );
    if ( iBtSapListener )
        {
        delete iBtSapListener;
        }

    if( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        AppUi()->RemoveView( KCallDivertPluginId );
        delete iContainer;
        }
    delete iCallDivertModel;
    }


// ---------------------------------------------------------------------------
// CGSCallDivertPlugin::NewL()
//
// Symbian OS default constructor
// ---------------------------------------------------------------------------
CGSCallDivertPlugin* CGSCallDivertPlugin::NewL( TAny* /*aInitParams*/ )
    {
    CGSCallDivertPlugin* self = new( ELeave ) CGSCallDivertPlugin();

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }


// ---------------------------------------------------------------------------
// CGSCallDivertPlugin::NewLC
//
// 1st phase constructor.
// ---------------------------------------------------------------------------
CGSCallDivertPlugin* CGSCallDivertPlugin::NewLC()
    {
    CGSCallDivertPlugin* self =
            new ( ELeave ) CGSCallDivertPlugin;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CGSCallDivertPlugin::NewContainerL
//
// Creates new iContainer.
// ---------------------------------------------------------------------------
void CGSCallDivertPlugin::NewContainerL()
    {
    iContainer = new ( ELeave ) CGSCallDivertPluginContainer;
    }


// ---------------------------------------------------------------------------
// CGSCallDivertPlugin::Id
//
// implementation of the frameworks virtual function, closely related with view
// architecture and has little or no relevance here
// ---------------------------------------------------------------------------
TUid CGSCallDivertPlugin::Id() const
    {
    return KCallDivertPluginId;
    }

// ---------------------------------------------------------------------------
// CGSCallDivertPlugin::HandleCommandL
//
// Handles the received commands
// ---------------------------------------------------------------------------
void CGSCallDivertPlugin::HandleCommandL( TInt aCommand )
    {
    switch ( aCommand )
        {
        case EAknSoftkeyBack:
            iAppUi->ActivateLocalViewL( KGSTelPluginUid );
            break;
        case EGSCmdAppOpen:
        case EAknSoftkeyOpen:
            HandleListBoxSelectionL();
            break;
        case ECFCmdCancelAll:
            iCallDivertView->HandleCommandL( ECFCmdCancelAll );
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
    }

// ---------------------------------------------------------------------------
// CGSCallDivertPlugin::HandleListBoxSelectionL
//
// Handles List box selection
// ---------------------------------------------------------------------------
void CGSCallDivertPlugin::HandleListBoxSelectionL()
    {
    // Other components will see updated value each time this funciton is 
    // called.
    iCallDivertModel->SetDivertServiceGroup( Container()->CurrentFeatureId() );

    switch ( iCallDivertModel->DivertServiceGroup() )
        {
        case EGSSettIdVoiceDivert:
        case EGSSettIdVideoDivert:
            iAppUi->ActivateLocalViewL( KCallDivertViewId );
            break;
        default:
            break;
        }
    }


// ---------------------------------------------------------------------------
// CGSCallDivertPlugin::UpdateListBox
//
// Calls Containers UpdateListBoxL method
// ---------------------------------------------------------------------------
void CGSCallDivertPlugin::UpdateListBox( TInt aItemId )
    {
    if ( Container() )
        {
        TRAP_IGNORE( Container()->UpdateListBoxL( aItemId ) );
        }
    }


// ---------------------------------------------------------------------------
// CGSCallDivertPlugin::DoActivateL
//
// Activates view
// ---------------------------------------------------------------------------
void CGSCallDivertPlugin::DoActivateL( const TVwsViewId& aPrevViewId,
                                             TUid aCustomMessageId,
                                             const TDesC8& aCustomMessage )
    {
    CGSBaseView::DoActivateL( aPrevViewId, aCustomMessageId, aCustomMessage );

    iContainer->iListBox->SetTopItemIndex( iTopItemIndex );

    if ( iCurrentItem >= 0 &&
        iCurrentItem < iContainer->iListBox->Model()->NumberOfItems() ) // magic
        {
        iContainer->iListBox->SetCurrentItemIndexAndDraw( iCurrentItem );
        }
    }

// ---------------------------------------------------------------------------
// CGSCallDivertPlugin::DoDeactivate
//
// Deactivates view
// ---------------------------------------------------------------------------
void CGSCallDivertPlugin::DoDeactivate()
    {
    CGSBaseView::DoDeactivate();
    }

// ---------------------------------------------------------------------------
// CGSCallDivertPlugin::Container
//
// Returns call container item
// ---------------------------------------------------------------------------
//
CGSCallDivertPluginContainer* CGSCallDivertPlugin::Container()
    {
    return static_cast <CGSCallDivertPluginContainer*> ( iContainer );
    }


// ========================= From CGSPluginInterface ==================

// ----------------------------------------------------------------------------
// CGSCallDivertPlugin::GetCaption
//
// Return application/view caption.
// ----------------------------------------------------------------------------
//
void CGSCallDivertPlugin::GetCaptionL( TDes& aCaption ) const
    {
    // the resource file is already opened.
    HBufC* result = StringLoader::LoadL( R_GS_CALL_DIVERT_CAPTION );

    aCaption.Copy( *result );
    delete result;
    }


// ----------------------------------------------------------------------------
// CGSCallDivertPlugin::PluginProviderCategory
//
// A means to identify the location of this plug-in in the framework.
// ----------------------------------------------------------------------------
//
TInt CGSCallDivertPlugin::PluginProviderCategory() const
    {
    //To identify internal plug-ins.
    return KGSPluginProviderInternal;
    }


// ----------------------------------------------------------------------------
// CGSCallDivertPlugin::Visible
//
// Provides the visibility status of self to framework.
// ----------------------------------------------------------------------------
//
TBool CGSCallDivertPlugin::Visible() const
    {
    TBool result( ETrue );
    TInt value = 0;
    iBtSapListener->Get( value );
    if ( value == EBTSapNotConnected || value == EBTSapConnecting )
        {
        //Now checking also for CSP support
        __ASSERT_ALWAYS( iCSP, User::Panic( KGSNameOfClass,
                         EGSCallDivertPluginPanicNullPtr ) );
        TInt err = KErrNone;
        err = iCSP->IsCFSupported( result );
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
// CGSCallDivertPlugin::CreateIconL()
//
//
// -----------------------------------------------------------------------------
//
CGulIcon* CGSCallDivertPlugin::CreateIconL( const TUid aIconType )
    {
    //EMbm<Mbm_file_name><Bitmap_name>
    CGulIcon* icon;
    TParse* fp = new( ELeave ) TParse();
    CleanupStack::PushL( fp );
    fp->Set( KGSCallDivertPluginIconDirAndName, &KDC_BITMAP_DIR, NULL );

    if( aIconType == KGSIconTypeLbxItem )
        {
        icon = AknsUtils::CreateGulIconL(
        AknsUtils::SkinInstance(),
        KAknsIIDQgnPropSetDivSub,
        fp->FullName(),
        EMbmGscalldivertpluginQgn_prop_set_div_sub,
        EMbmGscalldivertpluginQgn_prop_set_div_sub_mask );
        }
    else
        {
        icon = CGSPluginInterface::CreateIconL( aIconType );
        }

    CleanupStack::PopAndDestroy( fp );

    return icon;
    }

// ---------------------------------------------------------------------------
// CGSCallDivertPlugin::HandleNotifyPSL
//
// Handling PS keys change
// ---------------------------------------------------------------------------
//
void CGSCallDivertPlugin::HandleNotifyPSL( const TUid aUid, const TInt& aKey,
                                          const TRequestStatus& /* aStatus */ )
    {
    if ( aUid == KPSUidBluetoothSapConnectionState &&
         aKey == KBTSapConnectionState )
        {
        Visible();
        }
    }

// ----------------------------------------------------------------------------
// CGSCallDivertPlugin::SetCSPActiveL
//
// Set Customer Service Profile active
// ----------------------------------------------------------------------------
//
void CGSCallDivertPlugin::SetCSPActiveL( TBool aValue )
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


// ----------------------------------------------------------------------------
// CGSCallDivertPlugin::DynInitMenuPaneL()
//
// Display the dynamic menu
// ----------------------------------------------------------------------------
void CGSCallDivertPlugin::DynInitMenuPaneL( TInt aResourceId,
                                            CEikMenuPane* aMenuPane )
    {
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
    }

// End of File
