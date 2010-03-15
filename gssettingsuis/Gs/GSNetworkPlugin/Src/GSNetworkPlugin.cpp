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
* Description:  Call submenu in General Settings.
*
*/


// INCLUDE FILES
#include "GSNetworkPlugin.h"         //for CGSCallPlugin
#include "GSNetworkPluginContainer.h"    //for CGSCallPluginContainer
#include "GsLogger.h"
#include "GSNetworkPlugin.h"             //for pluginUID

#include <coeaui.h>
#include <hlplch.h>                     // For HlpLauncher

//AVKON headers
#include <aknnotewrappers.h>            //for Note Wrappers
#include <aknPopup.h>                   //for Popup menus
#include <aknradiobuttonsettingpage.h>  //for Radio Button Setting Pages
#include <aknViewAppUi.h>               //for viewappui
#include <AknWaitDialog.h>              //for CAknWaitDialog
#include <AknIconArray.h>               //for AknIconArray
#include <StringLoader.h>               //for StringLoader
#include <apparc.h>

#include <gscommon.hrh>

//Phonesettings and TSY headers
#include <PsetContainer.h>              //for CPsetContainer
#include <PsetNetwork.h>                //for CPsetNetwork
#include <PsetSAObserver.h>             //for CPsetSAObserver
#include <gsmerror.h>                   //for GSM-specific error messages
#include <exterror.h>
#include <featmgr.h>
#include <MSSSettingsRefreshObserver.h> //for SAT refresh
#include <gslistbox.h>                  //for radiobutton class

#include <e32property.h>
#include <PSVariables.h>

#include <GsNetworkPluginRsc.rsg>           //for resource IDs
#include <gsfwviewuids.h>
#include <gslistbox.h>                  //for CGSListBoxItemTextArray

#include <e32property.h>
#include <PSVariables.h>
#include <gsprivatepluginproviderids.h>
#include <gsnetworkplugin.mbg>
#include <BTSapInternalPSKeys.h>
#include <CoreApplicationUIsSDKCRKeys.h> // KCRUidCoreApplicationUIs, TCoreAppUIsNetworkConnectionAllowed
#include <activeidle2domainpskeys.h>

#include <CNWSession.h>
#include <NetworkHandlingProxy.h>
#include <gsparentplugin.h>
#include "GSNetworkDebugHelper.h"

#ifdef FF_POWER_SAVE
    #include "GSNetworkPluginAO.h"
#endif // FF_POWER_SAVE

// LOCAL CONSTANTS
_LIT( KGSNetWCDMAIcon, "0\t" );
_LIT( KGSNetGSMIcon, "1\t" );
_LIT( KEmptyStr, "" );

// Warning disabled as this constant is used inside __ASSERT_DEBUG
#pragma diag_suppress 177
_LIT( KGSNetworkPluginAssertName, "CGSNetworkPlugin" );


// Middle Softkey control ID.
const TInt KGSMSKControlID = 3;

const TInt KGSNetIconAdditionalChars = 5;


// ========================= MEMBER FUNCTIONS ================================
// ---------------------------------------------------------------------------
//
// Constructor.
//
// ---------------------------------------------------------------------------
CGSNetworkPlugin::CGSNetworkPlugin()
    :iMskCommandFlag( ETrue ) , iPsmActive( NULL )
    {
    }

// ---------------------------------------------------------------------------
//
// Symbian OS two-phased constructor (second phase)
//
// ---------------------------------------------------------------------------
void CGSNetworkPlugin::ConstructL()
    {
    __GSLOGSTRING("[CGSNetworkPlugin]--> CGSNetworkPlugin::ConstructL");
    FeatureManager::InitializeLibL();
    OpenLocalizedResourceFileL( KGSNetworkPluginResourceFileName,
                                iResourceLoader );

    //PS listener initialization
    iBtSapListener = CGSPubSubsListener::NewL(
                     KPSUidBluetoothSapConnectionState,
                     KBTSapConnectionState, this );

    BaseConstructL( R_GS_NET_VIEW );

    iSettingsContainer = CPsetContainer::NewL();
    iPSRefreshHandler = iSettingsContainer->CreateRefreshHandlerL();
    iPSRefreshHandler->NotifyFileChangeL(
            *this,
            KCspEf,
            EFileChangeNotification );
    iPhoneSettingsEngine = iSettingsContainer->CreateNetworkObjectL( *this );
    iApprovedNetwork = EFalse;
    CheckAndCreateDlgL( EFalse );
    iSearchForNetworksActive = EFalse;

    iNetworkText = HBufC::NewL( KNWShortNameLength );
    UpdateNetworkTextL();
    
#ifdef FF_POWER_SAVE    
    iPsmActive = CGSNetworkPluginAO::NewL();
    iPsmActive->SetView( this );
#endif // FF_POWER_SAVE   

    __GSLOGSTRING("[CGSNetworkPlugin] <--CGSNetworkPlugin::ConstructL");
    }

// ---------------------------------------------------------------------------
//
// Symbian OS two-phased constructor (first phase)
//
// ---------------------------------------------------------------------------
CGSNetworkPlugin* CGSNetworkPlugin::NewLC()
    {
    CGSNetworkPlugin* self = new ( ELeave ) CGSNetworkPlugin;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CGSNetworkPlugin::NewL()
// Static constructor
//
// ---------------------------------------------------------------------------
//
CGSNetworkPlugin* CGSNetworkPlugin::NewL( TAny* /*aInitParams*/ )
    {
    CGSNetworkPlugin* self = new ( ELeave ) CGSNetworkPlugin();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
//
// Destructor
//
// ---------------------------------------------------------------------------
CGSNetworkPlugin::~CGSNetworkPlugin()
    {
    __GSLOGSTRING("[CGSNetworkPlugin] ~CGSNetworkPlugin()|->");
    FeatureManager::UnInitializeLib();

    if( iNWSession )
        {
        delete iNWSession;
        iNWSession = NULL;
        }
    if ( iBtSapListener )
        {
        delete iBtSapListener;
        iBtSapListener = NULL;
        }

    if ( iPhoneSettingsEngine )
        {
        if( iSearchForNetworksActive )
            {
            //Reset back to previously used network
            iPhoneSettingsEngine->ResetNetworkSearch();
            }
        delete iPhoneSettingsEngine;
        iPhoneSettingsEngine = NULL;
        }

    if ( iNetworkArray )
        {
        iNetworkArray->Reset();
        delete iNetworkArray;
        iNetworkArray = NULL;
        }

    if ( iDlg )
        {
        delete iDlg;
        iDlg = NULL;
        }
    if ( iNetworkPopupList )
        {
        iNetworkPopupList->CancelPopup();
        }

    CloseDialog();
    
    //Cancel any outstanding SAT notifications
    if ( iPSRefreshHandler )
        {
        iPSRefreshHandler->CancelNotify();
        delete iPSRefreshHandler;
        iPSRefreshHandler = NULL;
        }

    if( iSettingsContainer )
        {
        delete iSettingsContainer;
        iSettingsContainer = NULL;
        }

    delete iNetworkText;
    iNetworkText = NULL;
    
#ifdef FF_POWER_SAVE
    delete iPsmActive;
    iPsmActive = NULL;
#endif // FF_POWER_SAVE

    __GSLOGSTRING("[CGSNetworkPlugin] ~CGSNetworkPlugin()-|");
    }

// ---------------------------------------------------------------------------
//
// Returns Id of the Network submenu
//
// ---------------------------------------------------------------------------
TUid CGSNetworkPlugin::Id() const
    {
    return KGSNetworkPluginUid;
    }

// ----------------------------------------------------------------------------
// CGSNetworkPlugin::Visible
//
// Provides the visibility status of self to framework.
// ----------------------------------------------------------------------------
//
TBool CGSNetworkPlugin::Visible() const
    {
    TBool result( EFalse );
    TInt value = 0;
    iBtSapListener->Get( value );
    if ( value == EBTSapNotConnected || value == EBTSapConnecting )
        {
        result = ETrue;
        }
    else
        {
        result = EFalse;
        }
    return result;
    }

// ---------------------------------------------------------------------------
//
// Handles network info
//
// ---------------------------------------------------------------------------
void CGSNetworkPlugin::HandleNetworkInfoReceivedL(
        const CNetworkInfoArray* aInfoArray, const TInt /* aResult */ )
    {
    __GSLOGSTRING("[GS]--> CGSNetworkPlugin::HandleNetworkInfoReceivedL");
    const TInt itemsCount = aInfoArray->Count();

    //first delete old ones
    iNetworkArray->Delete( 0, iNetworkArray->Count() );

    //then insert found networks
    for ( TInt i = 0; i < itemsCount; i++ )
        {
        MPsetNetworkSelect::TNetworkInfo info = aInfoArray->At( i );
        iNetworkArray->InsertL( i, info );
        }
    NetworkListL(); //after search complete, show results
    __GSLOGSTRING("[GS] <--CGSNetworkPlugin::HandleNetworkInfoReceivedL");
    }

// ---------------------------------------------------------------------------
//
// Creates list of Network providers
//
// ---------------------------------------------------------------------------
void CGSNetworkPlugin::NetworkListL()
    {
    __GSLOGSTRING("[GS]--> CGSNetworkPlugin::NetworkListL");

    //check if the graphical list is needed... set in local variation
    TBool showGraphicalList =
        Container()->NetPluginModel()->GraphicalNetworkListSupportedL();
    __GSLOGSTRING1("[GS]    NetworkListL: showGraphicalList: %d", showGraphicalList);

    //if no net items were found, do not show list
    if ( iNetworkArray->Count() > 0 )
        {
        while ( !iApprovedNetwork )
            {
            AknPopupListEmpty<CEikFormattedCellListBox>* list;
            if ( showGraphicalList &&
                 FeatureManager::FeatureSupported( KFeatureIdProtocolWcdma ) )
                {
                list = new ( ELeave ) CAknSingleGraphicPopupMenuStyleListBox;
                }
            else
                {
                list = new ( ELeave ) CAknSinglePopupMenuStyleListBox;
                }

            CleanupStack::PushL( list );
            if ( iNetworkPopupList )
                {
                iNetworkPopupList->CancelPopup();
                iNetworkPopupList = NULL;
                }
            iNetworkPopupList =
                CAknPopupList::NewL( list, R_AVKON_SOFTKEYS_OK_CANCEL );

            list->ConstructL( iNetworkPopupList,
                              CEikListBox::ELeftDownInViewRect );
            list->CreateScrollBarFrameL( ETrue );
            list->ScrollBarFrame()->SetScrollBarVisibilityL(
                CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto );

            //Fill in the list texts
            CDesCArrayFlat* items =
                new ( ELeave ) CDesCArrayFlat( KGSNetPopupMenuItems );
            CleanupStack::PushL( items );
            const TInt itemsCount = iNetworkArray->Count();
            __GSLOGSTRING1("[GS]    NetworkListL: itemsCount: %d", itemsCount);

            for ( TInt loop = 0; loop < itemsCount; loop++ )
                {
                HBufC* insertString =
                    HBufC::NewLC(
                        KGSNwLongNameLength + KGSNetIconAdditionalChars );
                MPsetNetworkSelect::TNetworkInfo info =
                    iNetworkArray->At( loop );

                if ( info.iLongName.Length() <= 0 ||
                     info.iLongName.Length() > KGSNwLongNameLength )
                    {
                    //no names received, use IDs
                    if ( info.iShortName.Length() <= 0 ||
                        info.iShortName.Length() > KGSNwShortNameLength )
                        {
                        TNetworkID ids;
                        TNetworkID networkCode;
                        ids = info.iId.iCountryCode;
                        ids.Append( KGSEmptySpace );
                        networkCode = info.iId.iNetworkCode;
                        ids.Append( networkCode );
                        insertString->Des().Append( ids );
                        __GSLOGSTRING1("[GS]    NetworkListL: Network ID: %S", insertString);
                        }
                    //short name received
                    else
                        {
                        insertString->Des().Append( info.iShortName );
                        __GSLOGSTRING1("[GS]    NetworkListL: Network Shortname: %S", insertString);
                        }
                    }
                //long name received
                else
                    {
                    insertString->Des().Append( info.iLongName );
                    __GSLOGSTRING1("[GS]    NetworkListL: Network Longname: %S", insertString);
                    }

                //set icon for network
                if ( showGraphicalList )
                    {
                    if ( FeatureManager::FeatureSupported(
                                                  KFeatureIdProtocolWcdma ) )
                        {
                        if ( info.iAccess !=
                             MPsetNetworkSelect::ENetNetworkGSM )
                            {
                            insertString->Des().Insert( 0, KGSNetWCDMAIcon );
                            }
                        else
                            {
                            insertString->Des().Insert( 0, KGSNetGSMIcon );
                            }
                        }
                    }
                items->AppendL( *insertString );
                CleanupStack::PopAndDestroy();
                }

            // For Testing purpuses ONLY
            for ( TInt k=0; k<items->Count(); k++ )
                {
                TPtrC itemText( (*items)[k] );
                __GSLOGSTRING2("[GS]    NetworkListL: POS: %d, VALUE: %S", k, &itemText);
                }

            CTextListBoxModel* model = list->Model();
            model->SetItemTextArray( items );
            model->SetOwnershipType( ELbmOwnsItemArray );
            CleanupStack::Pop( items ); //listbox model now owns this

            //Set title for list
            SetTitleToPopupL( *iNetworkPopupList, R_NETSL_FOUNDOPERATORS );

            // Setup graphic items list for dual mode nw selection listbox
            // based on the local variation
            if ( showGraphicalList )
                {
                CAknIconArray* iconList = new (ELeave) CAknIconArray( 10 );
                CleanupStack::PushL( iconList );
                iconList->ConstructFromResourceL( R_NET_DUALMODE_ICONS );
                list->ItemDrawer()->ColumnData()->SetIconArray( iconList );
                CleanupStack::Pop( iconList ); //listbox model now owns this
                }

            TInt res = 0;
            if( items->Count() )
                {
                res = iNetworkPopupList->ExecuteLD();
                }
            iNetworkPopupList = NULL;

            iApprovedNetwork = ETrue; //not perhaps, but user might want to quit
            TInt selection = list->CurrentItemIndex();
            __GSLOGSTRING1("[GS]    NetworkListL: selection: %d", selection);

            CleanupStack::PopAndDestroy( list );

            if ( res )
                {
                MPsetNetworkSelect::TNetworkInfo info =
                    iNetworkArray->At( selection );
                info.iMode = MPsetNetworkSelect::ENetSelectModeManual;
                iPhoneSettingsEngine->SelectNetworkL( info );
                }
            else // user selected "Cancel"
                {
                //Reset back to previously used network
                iPhoneSettingsEngine->ResetNetworkSearch();
                //After list has been used, clear it.
                PurgeNetworkList();
                if ( iPhoneSettingsEngine->IsCallActive() !=
                     CPsetSAObserver::EPSetCallActive )
                    {
                    ShowSettingPageL( EGSNetworkModeSelectionItemId );
                    }
                }
            }
        }
    iApprovedNetwork = EFalse; //initialize before new search
    __GSLOGSTRING("[GS] <--CGSNetworkPlugin::NetworkListL");
    }

// ---------------------------------------------------------------------------
//
// Handles requests to change network
//
// ---------------------------------------------------------------------------
void CGSNetworkPlugin::HandleNetworkChangedL(
    const MPsetNetworkSelect::TNetworkInfo& /*aCurrentInfo*/,
    const MPsetNetworkSelect::TCurrentNetworkStatus /*aStatus*/,
    const TInt /*aResult*/ )
    {
    //DEPRECATED
    }

// ---------------------------------------------------------------------------
//
// Handles requests to change network
//
// ---------------------------------------------------------------------------
void CGSNetworkPlugin::HandleNetworkChangedL(
    const MPsetNetworkSelect::TNetworkInfo& aCurrentInfo,
    const RMobilePhone::TMobilePhoneRegistrationStatus& aStatus,
    const TInt /* aResult */ )
    {
    __GSLOGSTRING("[GS]--> CGSNetworkPlugin::HandleNetworkChangedL");
    switch ( aStatus )
        {
        case RMobilePhone::ERegisteredOnHomeNetwork:
            ShowNoteL( R_HOME_NETWORK_SELECTED, KGSConfirmationNote );
            PurgeNetworkList();
            break;
        case RMobilePhone::ERegisteredRoaming:
            {
            HBufC* stringholder;
            if ( aCurrentInfo.iLongName.Length() > 0 &&
                aCurrentInfo.iLongName.Length() <=
                MPsetNetworkSelect::ENetLongNameSize )
                {
                stringholder = StringLoader::LoadLC( R_NETSL_NETWORKSELECTED,
                                                     aCurrentInfo.iLongName );
                }
            else if (aCurrentInfo.iShortName.Length() > 0 &&
                aCurrentInfo.iShortName.Length() <=
                MPsetNetworkSelect::ENetShortNameSize )
                {
                stringholder = StringLoader::LoadLC( R_NETSL_NETWORKSELECTED,
                                                     aCurrentInfo.iShortName );
                }
            else //network returned only ids
                {
                // construct whole "id-string"
                // e.g. for Finland Nokia's test network "244 7"
                TNetworkID ids;
                TNetworkID networkCode;
                ids = aCurrentInfo.iId.iCountryCode;
                __GSLOGSTRING1("[GS]    HandleNetworkChangedL: iCountryCode: %S", &aCurrentInfo.iId.iCountryCode);

                ids.Append( KGSEmptySpace );
                networkCode = aCurrentInfo.iId.iNetworkCode;
                __GSLOGSTRING1("[GS]    HandleNetworkChangedL: iNetworkCode: %S", &aCurrentInfo.iId.iNetworkCode);

                ids.Append( networkCode );
                stringholder =
                    StringLoader::LoadLC( R_NETSL_NETWORKSELECTED, ids );
                }
            CAknConfirmationNote* note = new ( ELeave ) CAknConfirmationNote( ETrue );
            note->ExecuteLD( *stringholder );
            CleanupStack::PopAndDestroy( stringholder );
            //After list has been used, clear it.
            PurgeNetworkList();
            break;
            }
        case RMobilePhone::ERegistrationUnknown:
        case RMobilePhone::ENotRegisteredNoService:
        case RMobilePhone::ENotRegisteredEmergencyOnly:
        case RMobilePhone::ENotRegisteredSearching:
        case RMobilePhone::ERegisteredBusy:
        case RMobilePhone::ERegistrationDenied:
        default:
            break;
        }
    PhoneIdle();
    __GSLOGSTRING("[GS] <--CGSNetworkPlugin::HandleNetworkChangedL");
    }


// ---------------------------------------------------------------------------
//
// Handles user inputs in Options menu
//
// ---------------------------------------------------------------------------
void CGSNetworkPlugin::HandleCommandL( TInt aCommand )
    {
    __GSLOGSTRING1("[CGSNetworkPlugin] HandleCommandL(%d)|->", aCommand );
    switch ( aCommand )
        {
        case EGSMSKCmdAppChange:
             HandleListBoxSelectionL();
             break;
        case EGSCmdAppChange:
            {
            const TInt currentFeatureId = Container()->CurrentFeatureId();

            if ( currentFeatureId == EGSMCNItemId )
                {
                ShowSettingPageL( EGSMCNItemId );
                }
            else if ( currentFeatureId == EGSNetworkModeItemId )
                {
                CheckCallActiveL( EGSNetworkModeItemId );
                }
            else
                {
                HandleListBoxEventL( NULL, EEventEnterKeyPressed );
                }
            }
            break;
        case EAknSoftkeyBack:
            {
            if (!iPhoneSettingsEngine->IsActive())
                {
                iAppUi->ActivateLocalViewL( KGSConPluginUid /*KGSTelPluginUid*/ );                
                }
            break;
            }
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
    __GSLOGSTRING("[CGSNetworkPlugin] HandleCommandL()-|");
    }


// ---------------------------------------------------------------------------
//
// Handles user inputs in Options menu
//
// ---------------------------------------------------------------------------
void CGSNetworkPlugin::ProcessCommandL( TInt aCommand )
    {
    // Network base class
    CAknView::ProcessCommandL( aCommand );
    }

// ---------------------------------------------------------------------------
//
// While request to find networks is processed
//
// ---------------------------------------------------------------------------
void CGSNetworkPlugin::HandleSearchingNetworksL(
                         MPsetNetworkInfoObserver::TServiceRequest aRequest )
    {
    __GSLOGSTRING("[GS]--> CGSNetworkPlugin::HandleSearchingNetworksL");
    if ( !iNetworkArray )
        {
        iNetworkArray = new ( ELeave )
            CNetworkInfoArray( KGSNetworkInfoArrayGranularity );
        }

    if ( aRequest != MPsetNetworkInfoObserver::EServiceRequestNone )
        {
        CheckAndCreateDlgL( EFalse );
        if ( aRequest ==
             MPsetNetworkInfoObserver::EServiceRequestSetNetworkAutomatic )
            {
            UpdateListBoxL( EGSNetworkModeSelectionItemId,
                            EGSAutomaticSelectMode );
            }
        else
            {
            UpdateListBoxL( EGSNetworkModeSelectionItemId,
                            EGSManualSelectMode );
            }

        iSearchForNetworksActive = ETrue;
        TInt res = iDlg->ExecuteLD( R_SEARCHING_NOTE );

        if ( res == EGSSoftkeyQuit )
            {
            iPhoneSettingsEngine->CancelProcess();
            
            ShowSettingPageL( EGSNetworkModeSelectionItemId );
            }
        else
            {
            if ( aRequest ==
                 MPsetNetworkInfoObserver::EServiceRequestSetNetworkAutomatic )
                {
                PhoneIdle();
                iAppUi->HandleCommandL( EEikCmdExit );
                }
            }
        }
    else
        {
        delete iDlg;
        iDlg = NULL;
        }

    __GSLOGSTRING("[GS] <--CGSNetworkPlugin::HandleSearchingNetworksL");
    }

// ---------------------------------------------------------------------------
//
// While request to change network is processed, a note is shown
//
// ---------------------------------------------------------------------------
void CGSNetworkPlugin::HandleRequestingSelectedNetworkL( TBool aOngoing )
    {
    __GSLOGSTRING("[GS]--> CGSNetworkPlugin::HandleRequestingSelectedNetworkL");
    if ( aOngoing )
        {
        CheckAndCreateDlgL( ETrue );

        if ( iDlg->ExecuteLD( R_REQUESTING_NOTE ) == EGSSoftkeyQuit )
            {
            iPhoneSettingsEngine->CancelProcess();

            CAknNoteDialog* dlg = new ( ELeave ) CAknNoteDialog(
                CAknNoteDialog::EConfirmationTone,
                CAknNoteDialog::EShortTimeout );
            dlg->ExecuteLD( R_REGISTRATION_INTERRUPTED );
            iApprovedNetwork = EFalse;
            }
        }
    else
        {
        delete iDlg;
        iDlg = NULL;
        }

    __GSLOGSTRING("[GS] <--CGSNetworkPlugin::HandleRequestingSelectedNetworkL");
    }


// ---------------------------------------------------------------------------
//
// Activates the view
//
// ---------------------------------------------------------------------------
void CGSNetworkPlugin::DoActivateL( const TVwsViewId&  aPrevViewId ,
                                       TUid  aCustomMessageId,
                                       const TDesC8&  aCustomMessage  )
    {
    __GSLOGSTRING( "[CGSNetworkPlugin] DoActivateL()|->" );
    CGSBaseView::DoActivateL( aPrevViewId, aCustomMessageId, aCustomMessage );
    Container()->SetMiddleSoftkeyObserver( this );
    SetMiddleSoftKeyLabelL( R_CALL_MSK_CHANGE, EGSMSKCmdAppChange );
    // This way we set which command is set for MSK
    iMskCommandFlag = ETrue;
    iContainer->iListBox->SetTopItemIndex( iTopItemIndex );

    if ( iCurrentItem >= 0 &&
         iCurrentItem < iContainer->iListBox->Model()->NumberOfItems() )
        {
        iContainer->iListBox->SetCurrentItemIndexAndDraw( iCurrentItem );
        }
    if ( aCustomMessageId == TUid::Uid( KGSCustomActivateNetView ) )
       {
       UpdateListBoxL( EGSNetworkModeSelectionItemId, EGSManualSelectMode );
       iSearchForNetworksActive = ETrue;
       iPhoneSettingsEngine->GetAvailableNetworksL();
       }
    CheckMiddleSoftkeyLabelL();
    __GSLOGSTRING( "[CGSNetworkPlugin] DoActivateL()-|" );
    }

// ---------------------------------------------------------------------------
//
// Deactivates the view
//
// ---------------------------------------------------------------------------
void CGSNetworkPlugin::DoDeactivate()
    {
    __GSLOGSTRING("[CGSNetworkPlugin]--> DoDeactivate");
    if ( Container() )
        {
        if ( Container()->iListBox && iContainer->iListBox->View() )
            {
            iTopItemIndex = iContainer->iListBox->TopItemIndex();
            iCurrentItem = iContainer->iListBox->CurrentItemIndex();
            }
        iAppUi->RemoveFromViewStack( *this, Container() );
        delete iContainer;
        iContainer = NULL;
        }
    __GSLOGSTRING("[CGSNetworkPlugin] <--DoDeactivate");
    }

// ---------------------------------------------------------------------------
//
// Shows setting page - for ON/OFF setting items
//
// ---------------------------------------------------------------------------
void CGSNetworkPlugin::ShowSettingPageL( TGSNetworkItemIds aPage )
    {
    __GSLOGSTRING( "[CGSNetworkPlugin::ShowSettingPageL]" );
    TInt resourceId = 0;
    TInt currentItem = KGSSettingOff;
    HBufC* settingPageTitle = NULL;

    switch( aPage )
        {
        case EGSNetworkModeSelectionItemId:
            resourceId = R_NETSL_NETSELECTMODE_LBX;
            settingPageTitle = StringLoader::LoadLC( R_NETSL_NETSELECTMODE );
            currentItem = Container()->GetSelectionMode();
            break;
        case EGSMCNItemId:
            resourceId = R_MCN_CELL_INFO_DISP_MODE_LBX;
            settingPageTitle = StringLoader::LoadLC( R_MCN_CELL_INFO_DISP_MODE );
            Container()->GetMcnValue( iMCN );
            //for MCN, values need to be switched since Off = 0 in shared data, 1 on screen
            SwitchOnOffValue( iMCN );
            currentItem = iMCN;
            break;
        case EGSNetworkModeItemId:
            resourceId = R_NET_NETWORK_MODE_LBX;
            settingPageTitle = StringLoader::LoadLC( R_NET_NETWORK_MODE );
            currentItem = Container()->GetCurrentNetworkModeSelectionL();
            break;
        default:
            break;
        }

    

    //Checking if the phone is in offline mode
    const TBool iOfflineMode = IsPhoneOfflineL();

    // If it is in offline mode, then we dont launch the NW mode setting page at all
    if (  iOfflineMode /*&& aPage == EGSNetworkModeItemId*/ )
        {
        HBufC* string = StringLoader::LoadLC( R_OFFLINE_MODE );
        CAknErrorNote* note = new ( ELeave ) CAknErrorNote ( ETrue );
        note->ExecuteLD( *string );
        CleanupStack::PopAndDestroy( string );
        }
    else // if not offline mode we show the setting page
        {
        
        CGSRadioButtonSettingPageItemTextArray* itemArray =
                CGSRadioButtonSettingPageItemTextArray::NewL( resourceId, *iCoeEnv, NULL );

        // network mode requires special handling as items in the settings page
        // are variated. The method below checks which items to display & hide.
        if ( aPage == EGSNetworkModeItemId )
            {
            Container()->CheckAndAlterContentsL( *itemArray );
            }

        CleanupStack::PushL( itemArray );

        TInt currentIndex = itemArray->IndexForFeatureIdL( currentItem );

        // no editor resource given
        iSettingDlg = new ( ELeave ) CAknRadioButtonSettingPage(
                settingPageTitle, EAknSettingPageNoOrdinalDisplayed, 0, 0,
                R_SETTING_PAGE, currentIndex, itemArray );
        itemArray->SetRadioButtonSettingPage( *iSettingDlg );

        const TInt prevSelection = currentIndex;
        if ( settingPageTitle )
        	{
        	iSettingDlg->SetSettingTextL( *settingPageTitle );	
        	}

        //Start listening if call occurs
        iPhoneSettingsEngine->SetNetSAObserver( *this );
        
        __GSLOGSTRING( "[CGSNetworkPlugin::ShowSettingPageL] Executing dialog" );
        const TInt res =
            iSettingDlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged );
        __GSLOGSTRING( "[CGSNetworkPlugin::ShowSettingPageL] Dialog closed" );
        iSettingDlg = NULL;
        // Get the feature id corresponding to array index
        const TGSNetworkModeItems currentFeature = IntToEnum( currentIndex );

        if ( res )
            {
            //not required for network mode UI item.
            //other items require calling this method.
            if (aPage != EGSNetworkModeItemId)
				{
				CreateNetworkSsCallL(currentFeature, aPage);
				}
			else if (prevSelection != currentIndex)
				{
				if ( iPsmActive->Mode() == EPsmsrvModePowerSave )
					{
					// If PSM is on, block setting:
					DisplayBlockNoteL();
					}
				else
					{
					// Show the confirmation query. Uses TGSNetworkModeItems.
					Container()->SetCurrentNetworkModeSelectionL(currentFeature);
					}
				}
			}
		CleanupStack::PopAndDestroy(itemArray);
		}
	//We'll update listbox for Network Mode when we're sure that phone is not
    //in Offline mode
    if ( aPage != EGSNetworkModeItemId )
        {
        UpdateListBoxL( aPage );
        }

    CleanupStack::PopAndDestroy( settingPageTitle );

    __GSLOGSTRING( "[CGSNetworkPlugin::ShowSettingPageL] End" );
    }

// ---------------------------------------------------------------------------
//
// utility for converting TInt to TGSNetworkModes enum.
//
// ---------------------------------------------------------------------------
TGSNetworkModeItems CGSNetworkPlugin::IntToEnum( TInt aFeatureId )
    {
    TGSNetworkModeItems mode = EGSNetworkModeDualmode;
    TInt supportedNetworks = Container()->NetPluginModel()->GetSupportedNetworksL();
    TInt value = 0;
    
    switch( aFeatureId )
        {
        case EGSNetworkModeDualmode:
            mode = EGSNetworkModeDualmode;
            break;
        case EGSNetworkModeUMTS:
            value = supportedNetworks & CGSNetworkPluginContainer::ENetFirstBit;           
            if ( value ) 
                 {
                 mode = EGSNetworkModeUMTS;
                 }
             else
            //assume that only dual mode and gsm are possible.
                 {
                 mode = EGSNetworkModeGSM;
                 }
            break;
        case EGSNetworkModeGSM:
            mode = EGSNetworkModeGSM;
            break;
        default:
            // This should not happen as items should always have matching id.
            __ASSERT_DEBUG( EFalse, User::Panic( KGSNetworkPluginAssertName, KErrArgument ) );
        }
    return mode;
    }


// ---------------------------------------------------------------------------
//
// If calls are started when in Network Setting Page, it is closed.
//
// ---------------------------------------------------------------------------
void CGSNetworkPlugin::HandleCallActivatedL()
    {
    __GSLOGSTRING("[GS]--> CGSNetworkPlugin::HandleCallActivatedL");

    CGSNetworkPluginContainer* container = Container();
    if ( container )
        {
        const TInt currentFeatureId = container->CurrentFeatureId();

		    if ( currentFeatureId != EGSNetworkModeSelectionItemId )
		        {
		        iPhoneSettingsEngine->CancelProcess();
		        return;
		        }
		    iPhoneSettingsEngine->CancelProcess();
		    //Close requesting notes
		    if ( iDlg )
		        {
		        HandleSearchingNetworksL( MPsetNetworkInfoObserver::EServiceRequestNone );
		        HandleRequestingSelectedNetworkL( EFalse );
		        }

		    //Close available network's list
		    if ( iNetworkPopupList )
		        {
		        iNetworkPopupList->CancelPopup();
		        iNetworkPopupList = NULL;
		        }
		    
		    CloseDialog();
		    
        }
    __GSLOGSTRING("[GS] <--CGSNetworkPlugin::HandleCallActivatedL");
    }

// ---------------------------------------------------------------------------
//
// Handles errors.
// From MPsetNetworkInfoObserver.
// ---------------------------------------------------------------------------
void CGSNetworkPlugin::HandleNetworkErrorL(
    const MPsetNetworkInfoObserver::TServiceRequest aRequest,
    const TInt aError )
    {

#ifdef _DEBUG
    TBuf<KGSNetworkModeDebugStrLen> requestBuf;
    GSNetworkDebugHelper::NetworkInfoRequestToDes( aRequest, requestBuf );
    __GSLOGSTRING2( "[CGSNetworkPlugin::HandleNetworkErrorL] %S error:%d",
                    &requestBuf, aError );
#endif // _DEBUG

    TInt resourceTxt = KErrNone;
    switch ( aRequest )
        {
        case MPsetNetworkInfoObserver::EServiceRequestGetNetworkInfo:
            iPhoneSettingsEngine->CancelProcess();
            iApprovedNetwork = EFalse;
            break;
        case MPsetNetworkInfoObserver::EServiceRequestSetNetworkAutomatic:
        case MPsetNetworkInfoObserver::EServiceRequestSetNetwork:
        default:
            break;
        }

    TBool ignore = EFalse;

    switch ( aError )
        {
        case KErrGsmNetCauseCallActive:
            ignore = ETrue; //do not show an error
            break;
        case KErrGsm0707NoNetworkService:
            resourceTxt = R_NO_NETWORK_FOUND;
            break;
        case KErrGsmOfflineOpNotAllowed:
            resourceTxt = R_OFFLINE_MODE;
            break;
        default:
            resourceTxt = R_NO_NETWORK_ACCESS;
            break;
        }
    if ( !ignore )
        {
        ShowNoteL( resourceTxt, KGSErrorNote );
        iApprovedNetwork = EFalse;
        }
    __GSLOGSTRING("[GS] <--CGSNetworkPlugin::HandleNetworkErrorL");
    }

// ---------------------------------------------------------------------------
//
// Check if calls are active and show note if Nw-menu is tried to be opened
//
// ---------------------------------------------------------------------------
void CGSNetworkPlugin::CheckCallActiveL( TGSNetworkItemIds aItemId )
    {
    __GSLOGSTRING("[GS]--> CGSNetworkPlugin::CheckCallActiveL");
    if ( iPhoneSettingsEngine->IsCallActive() == CPsetSAObserver::EPSetCallActive )
        {
        ShowNoteL( R_ONGOING_CALL, KGSErrorNote );
        return;
        }

    // Show network mode selection only if the mode value is already available!
    if ( aItemId == EGSNetworkModeItemId && !iSettingDlg
         && Container()->NetPluginModel()->GetNetworkMode()
         != KGSNetworkModeCapsNotUpdated
         )
        {
#ifdef FF_POWER_SAVE            
            if ( iPsmActive->Mode() == EPsmsrvModePowerSave )
                {
                // If PSM is on, block setting:
                DisplayBlockNoteL();
                }
            else
                {
#endif // FF_POWER_SAVE                    
                ShowSettingPageL( EGSNetworkModeItemId );
#ifdef FF_POWER_SAVE
                }
#endif // FF_POWER_SAVE 
        }
    else if( !iSettingDlg         
    	    //&& Container()->NetPluginModel()->GetNetworkMode()
            /*!= KGSNetworkModeCapsNotUpdated*/ )
        {
        ShowSettingPageL( EGSNetworkModeSelectionItemId );
        }
    __GSLOGSTRING("[GS] <--CGSNetworkPlugin::CheckCallActiveL");
    }

// ---------------------------------------------------------------------------
//
// Sets a title to a given popup list.
//
// ---------------------------------------------------------------------------
void CGSNetworkPlugin::SetTitleToPopupL( CAknPopupList& aList, TInt aTitleID )
    {
    HBufC* text = StringLoader::LoadLC( aTitleID );
    aList.SetTitleL( *text );
    CleanupStack::PopAndDestroy( text );
    text = NULL;
    }

// ---------------------------------------------------------------------------
//
// Sets a title to a settings page.
//
// ---------------------------------------------------------------------------
void CGSNetworkPlugin::SetTitleToSettingsL( CAknRadioButtonSettingPage& aDlg,
                                              TInt aTitleID )
    {
    HBufC* title = StringLoader::LoadLC( aTitleID );
    aDlg.SetSettingTextL( *title );
    CleanupStack::PopAndDestroy( title );
    title = NULL;
    }

// ---------------------------------------------------------------------------
//
// Appends given item to given list
//
// ---------------------------------------------------------------------------
void CGSNetworkPlugin::AppendItemL( CDesCArrayFlat& aList, TInt aItem )
    {
    HBufC* string = StringLoader::LoadLC( aItem );
    aList.AppendL( *string );
    CleanupStack::PopAndDestroy( string );
    }

// ---------------------------------------------------------------------------
//
// Shows a note if GPRS is active, when MCN is turned on.
//
// ---------------------------------------------------------------------------
void CGSNetworkPlugin::CheckGPRSConnectionL( TInt aCurrentItem )
    {
    if ( ( iPhoneSettingsEngine->IsGPRSConnected() ==
                     CPsetSAObserver::EPSetGPRSConnectionActive )
        && ( aCurrentItem == KGSSettingOff ) ) //if trying to set On...
        {
        ShowNoteL( R_ACTIVE_GPRS_CONN_NOTE, KGSInformationNote );
        }
    }

// ---------------------------------------------------------------------------
//
// Set MCN value: off => on, on => off.
//
// ---------------------------------------------------------------------------
void CGSNetworkPlugin::SwitchOnOffValue( TInt& aValue )
    {
    if ( aValue == EGSMcnSetOn )
        {
        aValue = EGSMcnSetOff;
        }
    else
        {
        aValue = EGSMcnSetOn;
        }
    }

// ---------------------------------------------------------------------------
//
// Create new Net container.
//
// ---------------------------------------------------------------------------
//
void CGSNetworkPlugin::NewContainerL()
    {
    iContainer = new ( ELeave ) CGSNetworkPluginContainer( this );
    }


// ---------------------------------------------------------------------------
// CGSNetworkPlugin::HandleListBoxSelectionL
//
// Handle user selection in the listbox to perform an associated action
// ---------------------------------------------------------------------------
void CGSNetworkPlugin::HandleListBoxSelectionL()
    {
    CEikMenuBar* menuBar = this->MenuBar();

    const TInt currentFeatureId = Container()->CurrentFeatureId();

    switch ( currentFeatureId )
        {
         case EGSNetworkModeSelectionItemId:
            CheckCallActiveL( EGSNetworkModeSelectionItemId );
            return;
        case EGSMCNItemId:
            if ( IsPhoneOfflineL() )
                {
                HBufC* string = StringLoader::LoadLC( R_OFFLINE_MODE );
                CAknErrorNote* note = new ( ELeave ) CAknErrorNote ( ETrue );
                note->ExecuteLD( *string );
                CleanupStack::PopAndDestroy( string );
                }
            else
                {
                Container()->GetMcnValue( iMCN );
                CheckGPRSConnectionL( iMCN );
                SwitchOnOffValue( iMCN );
                Container()->SetMcnValue( iMCN );
                }
            break;
        case EGSNetworkModeItemId:
            CheckCallActiveL( EGSNetworkModeItemId );
            return;
        default:
            break;
        }

    UpdateListBoxL( currentFeatureId );
    }


// ---------------------------------------------------------------------------
//
// Makes a Network/MCN Ss operation.
//
// ---------------------------------------------------------------------------
void CGSNetworkPlugin::CreateNetworkSsCallL( TInt aIndex,
                                             TGSNetworkItemIds aPage  )
    {
    __GSLOGSTRING("[GS]--> CGSNetworkPlugin::CreateNetworkSsCallL");
    if ( aPage == EGSNetworkModeSelectionItemId )
        {
        if ( aIndex == CGSNetworkPlugin::EAutomaticMode )
            {
            MPsetNetworkSelect::TNetworkInfo info;
            MPsetNetworkSelect::TSelectMode mode =
                MPsetNetworkSelect::ENetSelectModeAutomatic;
            iSearchForNetworksActive = ETrue;
            iPhoneSettingsEngine->GetNetworkSelectMode( mode );
            if ( mode == MPsetNetworkSelect::ENetSelectModeAutomatic )
                {
                iPreviousState = ETrue;
                }

            //get the local variation status for auto-auto net search support
            //the flag is negative... so 1 means "no search allowed".
            TBool autoNetSearchOff =
             Container()->NetPluginModel()->AutomaticNetworkSearchSupportedL();

            //autoNetSearch is defaulted to 0
            if ( iPreviousState && autoNetSearchOff )
                {
                PhoneIdle();
                }
            else
                {
                info.iMode = MPsetNetworkSelect::ENetSelectModeAutomatic;
                iSearchForNetworksActive = ETrue;
                iPhoneSettingsEngine->SelectNetworkL( info );
                }
            }
        else //manual mode
            {
            iSearchForNetworksActive = ETrue;
            iPhoneSettingsEngine->GetAvailableNetworksL();
            }
        }
    else // MCN Setting page
        {
        iMCN = aIndex;
        CheckGPRSConnectionL( iMCN );
        SwitchOnOffValue( iMCN );
        Container()->SetMcnValue( iMCN );
        }
    __GSLOGSTRING("[GS] <--CGSNetworkPlugin::CreateNetworkSsCallL");
    }

// ---------------------------------------------------------------------------
//
// Shows note.
//
// ---------------------------------------------------------------------------
//
void CGSNetworkPlugin::ShowNoteL( TInt aResourceId, TInt aType )
    {
    HBufC* string = StringLoader::LoadLC( aResourceId );
    switch ( aType )
        {
        case KGSErrorNote:
            {
            CAknErrorNote* note = new ( ELeave ) CAknErrorNote ( ETrue );
            note->ExecuteLD( *string );
            break;
            }
        case KGSConfirmationNote:
            {
            CAknConfirmationNote* note =
                new ( ELeave ) CAknConfirmationNote( ETrue );
            note->ExecuteLD( *string );
            break;
            }
        case KGSInformationNote:
            {
            CAknInformationNote* note =
                new ( ELeave ) CAknInformationNote( ETrue );
            note->ExecuteLD( *string );
            break;
            }
        default:
            break;
        }
    CleanupStack::PopAndDestroy( string );
    }

// ---------------------------------------------------------------------------
//
// Creates dialog, if it is does not exist yet.
//
// ---------------------------------------------------------------------------
//
void CGSNetworkPlugin::CheckAndCreateDlgL( TBool aDelayOff )
    {
    if ( !iDlg )
        {
        iDlg = new ( ELeave ) CAknWaitDialog(
            reinterpret_cast<CEikDialog**> ( &iDlg ), aDelayOff );
        }
    }

// ---------------------------------------------------------------------------
//
// Empties fetched network provider's list
//
// ---------------------------------------------------------------------------
//
void CGSNetworkPlugin::PurgeNetworkList()
    {
    __GSLOGSTRING("[GS]--> CGSNetworkPlugin::PurgeNetworkList");
    if ( iNetworkArray )
        {
        iNetworkArray->Reset();
        delete iNetworkArray;
        iNetworkArray = NULL;
        }
    __GSLOGSTRING("[GS] <--CGSNetworkPlugin::PurgeNetworkList");
    }


// ---------------------------------------------------------------------------
// CGSNetworkPlugin::Container
//
// Returns Network container item
// ---------------------------------------------------------------------------
//
CGSNetworkPluginContainer* CGSNetworkPlugin::Container()
    {
    return static_cast <CGSNetworkPluginContainer*> ( iContainer );
    }

// ---------------------------------------------------------------------------
// @@see MSSSettingsRefreshObserver::AllowRefresh
//
// Notification to allow refresh from SAT
// ---------------------------------------------------------------------------
//
TBool CGSNetworkPlugin::AllowRefresh(
                const TSatRefreshType aType,
                const TSatElementaryFiles aFiles )
    {
    __GSLOGSTRING("[GS]--> CGSNetworkPlugin::AllowRefresh");
    TBool allowRefresh = ETrue;
    if ( iPhoneSettingsEngine->IsCallActive() == CPsetSAObserver::EPSetCallActive )
        {
        allowRefresh = EFalse;
        }

    //check that aFiles has a value before issuing a refresh command
    //no need to check for a specific file...
    if ( ( aType != EFileChangeNotification ) ||
         ( aType == EFileChangeNotification ) &&
         ( aFiles & KCsp1Ef ||  aFiles & KCsp2Ef ) )
        {
        if ( iNetworkPopupList )
            {
            allowRefresh = EFalse;
            }
        }

    __GSLOGSTRING1("[GS]    AllowRefresh: allowRefresh: %d", allowRefresh);
    __GSLOGSTRING("[GS] <--CGSNetworkPlugin::AllowRefresh");
    return allowRefresh;
    }



// ---------------------------------------------------------------------------
// @@see MSSSettingsRefreshObserver::Refresh
//
// Do the actual refresh of the UI for CSP file updation
// ---------------------------------------------------------------------------
//
void CGSNetworkPlugin::Refresh(
                const TSatRefreshType aType,
                const TSatElementaryFiles aFiles )
    {
    __GSLOGSTRING("[GS]--> CGSNetworkPlugin::Refresh");
    // for a file change notification to be handled, aFiles must always
    // contain a value
    if ( ( aType != EFileChangeNotification ) ||
         ( aType == EFileChangeNotification ) &&
         ( aFiles & KCsp1Ef ||  aFiles & KCsp2Ef  ) )
        {
        //check if this check is required, as it is visible in the UI
        if ( iNetworkPopupList )
            {
            iNetworkPopupList->CancelPopup();
            iNetworkPopupList = NULL;
            }

        //Change the network mode to Automatic.
        MPsetNetworkSelect::TNetworkInfo info;
        info.iMode = MPsetNetworkSelect::ENetSelectModeAutomatic;
        iSearchForNetworksActive = ETrue;
        TRAP_IGNORE( iPhoneSettingsEngine->SelectNetworkL( info ) );
        __GSLOGSTRING("[GS]    Refresh: before Updating the listbox");
        TRAP_IGNORE( UpdateListBoxL( EGSNetworkModeSelectionItemId ) );
        __GSLOGSTRING("[GS]    Refresh: after Updating the listbox");
        }

    __GSLOGSTRING("[GS] <--CGSNetworkPlugin::Refresh");
    }


// ---------------------------------------------------------------------------
// CGSNetworkPlugin::PhoneIdle
//
// Switch to Phone Idle view. GS is running at the background.
// ---------------------------------------------------------------------------
//
void CGSNetworkPlugin::PhoneIdle()
    {
    // Fetching the current Idle id
    TInt idleApp;
    RProperty::Get( KPSUidAiInformation, KActiveIdleUid, idleApp );
    TUid idleAppUid = { idleApp } ;
    TApaTaskList taskList( iEikonEnv->WsSession() );
    TApaTask task = taskList.FindApp( idleAppUid );

    // Expecting that idle application is open always
    // if not we dont do anything
    if( task.Exists() )  // App open
    	{
        task.BringToForeground();
        }

    //No need to reset network search on exit
    iSearchForNetworksActive = EFalse;
    }


// -----------------------------------------------------------------------------
// When this method is called, view checks based on highlight focus, if the MSK
// label is correct.
// -----------------------------------------------------------------------------
//
void CGSNetworkPlugin::CheckMiddleSoftkeyLabelL()
    {
    }


// -----------------------------------------------------------------------------
// Remove unnecessary commands from Middle softkey.
// @flag = ETrue means presently MSK value is "Change" so we remove that
// @flag = EFalse means presently MSK value is "Context Options" so we remove that
// -----------------------------------------------------------------------------
//
void CGSNetworkPlugin::RemoveCommandFromMSK(const TBool flag )
    {
    CEikButtonGroupContainer* cbaGroup = Cba();
    if ( cbaGroup )
        {
        if ( flag )
            {
            cbaGroup->RemoveCommandFromStack(
                                KGSMSKControlID, EGSMSKCmdAppChange );
            }
        else
            {
            cbaGroup->RemoveCommandFromStack(
                                KGSMSKControlID, EAknSoftkeyContextOptions );
            }
        }
    }

// ---------------------------------------------------------------------------
// Sets middle softkey label.
// ---------------------------------------------------------------------------
//
void CGSNetworkPlugin::SetMiddleSoftKeyLabelL(
    const TInt aResourceId, const TInt aCommandId )
    {
    CEikButtonGroupContainer* cbaGroup = Cba();
    if ( cbaGroup )
        {
        HBufC* middleSKText = StringLoader::LoadLC( aResourceId );
        TPtr mskPtr = middleSKText->Des();
        cbaGroup->AddCommandToStackL(
            KGSMSKControlID,
            aCommandId,
            mskPtr );
        CleanupStack::PopAndDestroy( middleSKText );
        }
    }

// -----------------------------------------------------------------------------
// CGSNetworkPlugin::HandleClientRectChange
//
//
// -----------------------------------------------------------------------------
//
void CGSNetworkPlugin::HandleClientRectChange()
    {
    if ( iContainer )
        {
        iContainer->SetRect( ClientRect() );
        }
    }

// ========================= From CGSPluginInterface ==================

// -----------------------------------------------------------------------------
// CGSNetworkPlugin::GetCaptionL
//
//
// -----------------------------------------------------------------------------
//
void CGSNetworkPlugin::GetCaptionL( TDes& aCaption ) const
    {
    HBufC* result = StringLoader::LoadL( R_GS_NET_VIEW_CAPTION );
    aCaption.Copy( *result );
    delete result;
    }


// -----------------------------------------------------------------------------
// CGSNetworkPlugin::PluginProviderCategory
//
//
// -----------------------------------------------------------------------------
//
TInt CGSNetworkPlugin::PluginProviderCategory() const
    {
    //This plugin is created by 3rd party.
    return KGSPluginProviderInternal;
    }

// ---------------------------------------------------------------------------
//
// Updates specific value to container.
//
// ---------------------------------------------------------------------------
//
void CGSNetworkPlugin::UpdateListBoxL( const TInt& aListItem,
                                       const TInt aNewValue )
    {
    if ( iContainer && iContainer->iListBox )
        {
        Container()->UpdateListBoxL( aListItem, aNewValue );
        }
    CheckMiddleSoftkeyLabelL();
    }

// -----------------------------------------------------------------------------
// CGSNetworkPlugin::CreateIconL()
//
//
// -----------------------------------------------------------------------------
//
CGulIcon* CGSNetworkPlugin::CreateIconL( const TUid aIconType )
    {
    //EMbm<Mbm_file_name><Bitmap_name>
    CGulIcon* icon;
    TParse* fp = new( ELeave ) TParse();
    CleanupStack::PushL( fp );
    fp->Set( KGSNetworkPluginIconDirAndName, &KDC_BITMAP_DIR, NULL );

    if( aIconType == KGSIconTypeLbxItem && PhoneOnline() )
        {
        icon = AknsUtils::CreateGulIconL(
            AknsUtils::SkinInstance(),
            KAknsIIDQgnPropSetNetworkSub,
            fp->FullName(),
            EMbmGsnetworkpluginQgn_prop_set_network_sub,
            EMbmGsnetworkpluginQgn_prop_set_network_sub_mask );
        }
    else if( aIconType == KGSIconTypeLbxItem /*->phone offline*/ )
        {
        icon = AknsUtils::CreateGulIconL(
            AknsUtils::SkinInstance(),
            KAknsIIDQgnPropSetNetworkSub,
            fp->FullName(),
            EMbmGsnetworkpluginQgn_prop_set_network_sub_offline,
            EMbmGsnetworkpluginQgn_prop_set_network_sub_offline_mask );
        }
    else
        {
        icon = CGSPluginInterface::CreateIconL( aIconType );
        }
    CleanupStack::PopAndDestroy( fp );
    return icon;
    }

// ---------------------------------------------------------------------------
// CGSNetworkPlugin::HandleNotifyPSL
//
// Handling PS keys change
// ---------------------------------------------------------------------------
//
void CGSNetworkPlugin::HandleNotifyPSL( const TUid aUid, const TInt& aKey,
                                        const TRequestStatus& /* aStatus */ )
    {
    if ( aUid == KPSUidBluetoothSapConnectionState &&
         aKey == KBTSapConnectionState )
        {
        Visible();
        }
    }

// ---------------------------------------------------------
// CGSNetworkPlugin::IsPhoneOfflineL
//
// Checks if phone is in offline mode or not.
// Return ETrue if phone is in offline mode.
// Return EFalse if phone is not in offline mode.
// ---------------------------------------------------------
//
TBool CGSNetworkPlugin::IsPhoneOfflineL() const
    {
    if ( FeatureManager::FeatureSupported( KFeatureIdOfflineMode ) )
        {
        CRepository* repository = CRepository::NewLC( KCRUidCoreApplicationUIs );
        TInt connAllowed = 1;
        repository->Get( KCoreAppUIsNetworkConnectionAllowed, connAllowed );
        CleanupStack::PopAndDestroy();  // repository
        if ( !connAllowed )
            {
            return ETrue;
            }
        }
    return EFalse;
    }


// ----------------------------------------------------------------------------
// CGSNetworkPlugin::DynInitMenuPaneL()
//
// Display the dynamic menu
// ----------------------------------------------------------------------------
void CGSNetworkPlugin::DynInitMenuPaneL( TInt aResourceId,
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


//Disabling warning caused by err variable used inside macros
#pragma diag_suppress 550
// -----------------------------------------------------------------------------
// CGSNetworkPlugin::GetValue()
//
// -----------------------------------------------------------------------------
//
void CGSNetworkPlugin::GetValue( const TGSPluginValueKeys aKey,
                                 TDes& aValue )
    {
    switch( aKey )
        {
        case EGSPluginKeySettingsItemValueString:
            // Operator name is async function but GetValue is synchronous.
            // As a result the first GetValue call might not contain updated
            // operator name. A callback will however ask to update parent plugin
            // which will then call GetValue a second time and the operator name
            // will then be updated.
            if( !iNWSession )
                {
                __GSLOGSTRING(" [CGSNetworkPlugin::GetValue] Opening iNWSession..." );

                // Create network handling engine session.
                // Question: is this callback one-shot or constant observer?
                TRAPD( err, iNWSession = CreateL( *this, iNWInfo ) );
                __GSLOGSTRING1(" [CGSNetworkPlugin::GetValue] Done. Err:%d", err );
                }
            aValue.Copy( iNetworkText->Des() );
            break;
        default:
            aValue.Append( KEmptyStr );
            break;
        }
    }

//Enabling warnings
#pragma diag_default 550
// -----------------------------------------------------------------------------
// CGSNetworkPlugin::GetNetworkValues()
// Get network status
// qtn.cp.detail.cellular.operational / qtn.cp.detail.cellular.offline
// qtn_cp_detail_cellular_operational / qtn_cp_detail_cellular_offline
// and operator name.
// -----------------------------------------------------------------------------
//
void CGSNetworkPlugin::UpdateNetworkTextL()
    {
    // On default text is 'off-line', if online status is received use,
    // 'operational'.
    TInt networkStatusRscId = R_CELLULAR_OFFLINE;

    if( iNetworkText )
        {
        delete iNetworkText;
        iNetworkText = NULL;
        }

    if( PhoneOnline() )
        {
        __GSLOGSTRING(" [CGSNetworkPlugin::UpdateNetworkTextL] Loading operational-text" );
        iNetworkText = iNWInfo.iLongName.AllocL();      
        }
    else
        {
        __GSLOGSTRING(" [CGSNetworkPlugin::UpdateNetworkTextL] Loading offline-text" );
        networkStatusRscId = R_CELLULAR_OFFLINE;
        iNetworkText = StringLoader::LoadL( networkStatusRscId );
        }
    __GSLOGSTRING1( "[CGSNetworkPlugin::UpdateNetworkTextL] iNetworkText:%S ", iNetworkText );
    }


//Disabling warning caused by err variable used inside macros
#pragma diag_suppress 550
// -----------------------------------------------------------------------------
// CGSNetworkPlugin::HandleNetworkMessage
// From MNWMessageObserver.
//
// -----------------------------------------------------------------------------
//
void CGSNetworkPlugin::HandleNetworkMessage( const TNWMessages aMessage )
    {
#ifdef _DEBUG
    TBuf<KGSNetworkModeDebugStrLen> messageBuf;
    GSNetworkDebugHelper::NetworkMessageToDes( aMessage, messageBuf );
    __GSLOGSTRING1( "[CGSNetworkPlugin::HandleNetworkMessage] %S",
                    &messageBuf );
#endif // _DEBUG

    switch( aMessage )
        {
        case ENWMessageNetworkInfoChange:
            TRAPD
                (
                err,
                UpdateNetworkTextL();
                )
            __GSLOGSTRING1(
                    "[CGSNetworkPlugin::HandleNetworkMessage] Update network text error:%d.",
                    err );
            TRAPD
                (
                err2,
                UpdateNetworkSettingPageL();
                )
            __GSLOGSTRING1(
                    "[CGSNetworkPlugin::HandleNetworkMessage] Update network setting page error:%d.",
                    err2 );
            Parent()->UpdateView();
            break;
        default:
            break;
        }
    }


// -----------------------------------------------------------------------------
// CGSNetworkPlugin::UpdateNetworkSettingPageL
//
//
// -----------------------------------------------------------------------------
//
void CGSNetworkPlugin::UpdateNetworkSettingPageL()
    {
    // If network mode setting page is open, close it and open
    // again so it will update the selected value correctly.
    // Whoopee! Who said setting dialogs are dumb?
    if( iSettingDlg && Container()->CurrentFeatureId() ==  EGSNetworkModeItemId )
        {
        __GSLOGSTRING( "[CGSNetworkPlugin::UpdateNetworkSettingPageL] Closing and reopening dialog..." );
        CloseDialog();
        ShowSettingPageL( EGSNetworkModeItemId );
        }
    }

//Enabling warnings
#pragma diag_default 550

// -----------------------------------------------------------------------------
// CGSNetworkPlugin::HandleNetworkError
// From MNWMessageObserver.
//
// -----------------------------------------------------------------------------
//
void CGSNetworkPlugin::HandleNetworkError( const TNWOperation aOperation,
                                           TInt aErrorCode )
    {
    // Probably no need to do anything as default assumption is off-line unless
    // iNWInfo.iRegistrationStatus defines otherwise. Question is, does
    // iNWSession update iNWInfo values to anything sensible in case of error.
#ifdef _DEBUG
    TBuf<KGSNetworkModeDebugStrLen> operationBuf;
    GSNetworkDebugHelper::NetworkOperationToDes( aOperation, operationBuf );
    __GSLOGSTRING2( "[CGSNetworkPlugin::HandleNetworkError] %S error:%d",
                    &operationBuf, aErrorCode );
#endif // _DEBUG

    // Later network operator name seems to be updated after HandleNetworkError
    // so we probably want to update the name also in UI as soon as possible.
    TRAP_IGNORE( UpdateNetworkTextL(); );

    // CGSParentPlugin::DoDeactivate() will delete it's container so calling
    // CGSParentPlugin::UpdateView() should not cause unnecessary container
    // updates.
    Parent()->UpdateView();
    }


// -----------------------------------------------------------------------------
// CGSNetworkPlugin::Parent
//
//
// -----------------------------------------------------------------------------
//
CGSParentPlugin* CGSNetworkPlugin::Parent()
    {
    CGSParentPlugin* parent = static_cast<CGSParentPlugin*>(
            AppUi()->View( KGSConPluginUid ) );
    return parent;
    }


// -----------------------------------------------------------------------------
// CGSNetworkPlugin::PhoneOnline
//
//
// -----------------------------------------------------------------------------
//
TBool CGSNetworkPlugin::PhoneOnline()
    {
#ifdef _DEBUG
    TBuf<KGSNetworkModeDebugStrLen> statusBuf;
    GSNetworkDebugHelper::NetworkStatusToDes( iNWInfo.iRegistrationStatus, statusBuf );
    __GSLOGSTRING1( "[CGSNetworkPlugin::PhoneOnline] %S", &statusBuf );
#endif // _DEBUG

    TBool online = EFalse;
    switch( iNWInfo.iRegistrationStatus )
        {
        case ENWRegisteredOnHomeNetwork: // Same functionality as below
        case ENWRegisteredRoaming:
            {
            online = ETrue;
            break;
            }
        default:
            {
            online = EFalse;
            break;
            }
        }
    return online;
    }


// -----------------------------------------------------------------------------
// CGSNetworkPlugin::CloseDialog
//
//
// -----------------------------------------------------------------------------
//
void CGSNetworkPlugin::CloseDialog()
    {
    //Send ESC key sequence to setting dialog, so that it closes itself.
    if ( iSettingDlg )
        {
        TKeyEvent event;
        event.iCode = EKeyEscape;
        event.iScanCode = EStdKeyEscape;
        event.iRepeats = 0;
        TRAP_IGNORE( iCoeEnv->SimulateKeyEventL( event, EEventKeyDown );
                     iCoeEnv->SimulateKeyEventL( event, EEventKey );
                     iCoeEnv->SimulateKeyEventL( event, EEventKeyUp ); );
        }
    }

#ifdef FF_POWER_SAVE
// ----------------------------------------------------------------------------------
// CGSDisplayPlugin::DisplayBlockNoteL()
// ----------------------------------------------------------------------------------
void CGSNetworkPlugin::DisplayBlockNoteL()
    {
    HBufC* prompt = iCoeEnv->AllocReadResourceLC(
            R_GS_POWER_SAVING_PROTECTED_SETTINGS_INFONOTE );
    CAknInformationNote* note = new ( ELeave ) CAknInformationNote( ETrue );
    note->ExecuteLD( *prompt );
    CleanupStack::PopAndDestroy( prompt );
    }

// ----------------------------------------------------------------------------------
// CGSDisplayPlugin::UpdateOnPsmChanged()
// ----------------------------------------------------------------------------------
void CGSNetworkPlugin::UpdateOnPsmChanged() 
    {
    __GSLOGSTRING( "[CGSNetworkPlugin::UpdateOnPsmChanged]" );
    
    if ( Container() && Container()->NetPluginModel() )
        {
        Container()->NetPluginModel()->StartAsynGetCurrentNetworkModeSelectionL();
        }
    }

#endif // FF_POWER_SAVE

//End of File
