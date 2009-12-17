/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies).
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
#include "GSCallPlugin.h"         //for CGSCallPlugin
#include "GSCallPluginContainer.h"    //for CGSCallPluginContainer
#include "GsLogger.h"
#include "GSCallPlugin.h"             //for pluginUID
#include <akntitle.h>                // for title pane
#include <coeaui.h>
#include <hlplch.h>                     // For HlpLauncher
#include <aknnotewrappers.h>            //for Note Wrappers
#include <aknradiobuttonsettingpage.h>  //for Radiobutton pages
#include <akntextsettingpage.h>         //for TextSettings pages
#include <aknmessagequerydialog.h>      //for Message Query Dialog
#include <aknViewAppUi.h>
#include <aknnotedialog.h>
#include <AknWaitDialog.h>
#include <gscommon.hrh>

// headers for still image
#include <imageconversion.h>
#include <MGFetch.h>                    // Image selection
#include <DRMCommon.h>

#include <eikmenup.h>                   //for menu bar
#include <PsetContainer.h>              //for container
#include <PsuiContainer.h>              //for CPsuiContainer
#include <featmgr.h>
#include <SecUiSecurityHandler.h>       //for PIN2 query
#include <SecUi.h>                      //for TSecUi
#include <SecUiSecuritySettings.h>      //for CSecuritySettings
#include <GSCallPluginRsc.rsg>           //for resource IDs
#include <gscallplugin.mbg>
#include <gsfwviewuids.h>
#include <gslistbox.h>                  //for CGSListBoxItemTextArray

#include <settingsinternalcrkeys.h>
#include    <ctsydomainpskeys.h>
#include <gsprivatepluginproviderids.h>

// LOCAL CONSTANTS
_LIT( KGSNameOfClass, "GSCallPlugin" );
_LIT( KGSVTStillImgPath, "" );
//Use default values for external calls.
const TInt KGSUseDefault = 0;
//Call Prefix setting page items
const TInt KGSSettingItemOff = 1;
const TInt KGSSettingItemOn = 0;
//Lines in setting page.
const TInt KGSFirstLine = 0;
const TInt KGSSecondLine = 1;
// Middle Softkey control ID.
const TInt KGSMSKControlID = 3;

// VT still image values. Basically everything above 0 should be considered as
// 'On' value so do not compare with 1
const TInt KGSVTStillImageOff = 0;


// ========================= MEMBER FUNCTIONS ================================
// ---------------------------------------------------------------------------
//
// Constructor.
//
// ---------------------------------------------------------------------------
CGSCallPlugin::CGSCallPlugin()
    :iMskCommandFlag( ETrue )
    {
    }

// ---------------------------------------------------------------------------
//
// Symbian OS two-phased constructor (second phase)
//
// ---------------------------------------------------------------------------
void CGSCallPlugin::ConstructL()
    {
    __GSLOGSTRING("[GSCallPlugin]--> CGSCallPlugin::ConstructL");
    OpenLocalizedResourceFileL( KGSCallPluginResourceFileName,
                                iResourceLoader );
                                
    BaseConstructL( R_GS_CALL_VIEW );

    iModel = CGSCallPluginModel::NewL();
    iRestrictedClir = iModel->RestrictedSendCallerIdSupportedL();
    iSecureClir = iModel->SecureSendCallerIdSupportedL();
    __GSLOGSTRING("[GSCallPlugin]--> ConstructL - Model created");
    iSettings = CPsetContainer::NewL();
    __GSLOGSTRING("[GSCallPlugin]--> ConstructL - PSetContainer created");
    iObsContainer = CPsuiContainer::NewL();
    __GSLOGSTRING("[GSCallPlugin]--> ConstructL - PsuiContainer created");
    iCwObserver = iObsContainer->CreateCWObsL();
    __GSLOGSTRING("[GSCallPlugin]--> ConstructL - CreateCWObsL done");
    iWaiting = iSettings->CreateCWObjectL( *iCwObserver );
    __GSLOGSTRING("[GSCallPlugin]--> ConstructL - CreateCWObjectL done");
    iSsSettings = new (ELeave) RSSSettings;
    __GSLOGSTRING("[GSCallPlugin]--> ConstructL - SSSettings created");

    User::LeaveIfError( iSsSettings->Open() );
    User::LeaveIfError( iSsSettings->RegisterAll( *this ) );
    __GSLOGSTRING("[GSCallPlugin]--> ConstructL - SSSettings opening successful");

#ifndef __WINS__
    iSsSettings->Get( ESSSettingsAls, iAls );
    iSsSettings->Get( ESSSettingsAlsBlocking, iAlsBlock );
#endif //__WINS__
    
    iVTImageSelectionOngoing = EFalse;
    
    __GSLOGSTRING("[GSCallPlugin] <--CGSCallPlugin::ConstructL");
    }

// ---------------------------------------------------------------------------
//
// Symbian OS two-phased constructor (first phase)
//
// ---------------------------------------------------------------------------
CGSCallPlugin* CGSCallPlugin::NewLC()
    {
    CGSCallPlugin* self = new ( ELeave ) CGSCallPlugin;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CGSCallPlugin::NewL()
// Static constructor
//
// ---------------------------------------------------------------------------
//
CGSCallPlugin* CGSCallPlugin::NewL( TAny* /*aInitParams*/ )
    {
    CGSCallPlugin* self = new(ELeave) CGSCallPlugin();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
//
// Destructor
//
// ---------------------------------------------------------------------------
CGSCallPlugin::~CGSCallPlugin()
    {
    __GSLOGSTRING("[GSCallPlugin] ~CGSCallPlugin()|->");
    
    delete iObsContainer;
    delete iCwObserver;
    if ( iWaiting )
        {
        delete iWaiting;	
        }
    delete iSettings;

    if ( iSsSettings )
        {
        iSsSettings->CancelAll( *this );
        iSsSettings->Close();
        delete iSsSettings;
        }

    if( iModel )
        {
        delete iModel;
        }

    if( iImageHandler )
        {
        delete iImageHandler;
        }

    __GSLOGSTRING("[GSCallPlugin] ~CGSCallPlugin()-|");
    }

// ---------------------------------------------------------------------------
//
// Returns Id of the Call submenu
//
// ---------------------------------------------------------------------------
TUid CGSCallPlugin::Id() const
    {
    return KCallPluginUID;
    }

// ---------------------------------------------------------------------------
//
// Handles user inputs in Options menu
//
// ---------------------------------------------------------------------------
void CGSCallPlugin::HandleCommandL( TInt aCommand )
    {
    __GSLOGSTRING1("[GSCallPlugin] HandleCommandL(%d)|->", aCommand );
    switch ( aCommand )
        {
        case EGSMSKCmdAppChange:
             HandleListBoxSelectionL();
             break;
            // Command Obtained from Context Options
            // set in ProcessCommandL()
        case ECFCmdActivate:
        case EGSCmdCallWaitActivate:
            iWaiting->SetCallWaitingL( MPsetCallWaiting::EActivateCallWaiting,
                                       EAllTeleAndBearer );
            break;
            // Command Obtained from Context Options
            // set in ProcessCommandL()
        case ECFCmdCheckStatus:
        case EGSCmdCallWaitInquiry:
            iWaiting->GetCallWaitingStatusL();
            break;
            // Command Obtained from Context Options
            // set in ProcessCommandL()
        case ECFCmdCancel: 
        case EGSCmdCallWaitDeactivate:
            iWaiting->SetCallWaitingL( MPsetCallWaiting::EDeactivateCallWaiting,
                                       EAllTeleAndBearer );
            break;
        case EAppCmdCreatePopupList:
            {
            const TInt currentFeatureId = Container()->CurrentFeatureId();
            HandleCreatePopupL( currentFeatureId );
            }
            break;
        case EAknSoftkeyBack:
            iAppUi->ActivateLocalViewL( KGSTelPluginUid );
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
    __GSLOGSTRING("[CGSCallPlugin] HandleCommandL()-|");
    }


// ---------------------------------------------------------------------------
//
// Handles user inputs in Options menu
//
// ---------------------------------------------------------------------------
void CGSCallPlugin::ProcessCommandL( TInt aCommand )
    {
    MenuBar()->SetContextMenuTitleResourceId( R_CALL_DIVERT_MSK_CONTEXTMENUBAR );
    // Call base class
    CAknView::ProcessCommandL( aCommand );
    }
// ---------------------------------------------------------------------------
//
// Activates the view
//
// ---------------------------------------------------------------------------
void CGSCallPlugin::DoActivateL( const TVwsViewId&  aPrevViewId ,
                                       TUid  aCustomMessageId,
                                       const TDesC8&  aCustomMessage  )
    {
    __GSLOGSTRING( "[GSCallPlugin] DoActivateL()|->" );
    CGSBaseView::DoActivateL( aPrevViewId, aCustomMessageId, aCustomMessage );
    Container()->SetMiddleSoftkeyObserver( this );
    SetMiddleSoftKeyLabelL( R_CALL_MSK_CHANGE, EGSMSKCmdAppChange );
    // This way we set which command is set for MSK
    iMskCommandFlag = ETrue;
//    iContainer->iListBox->SetTopItemIndex( iTopItemIndex );

//    if ( iCurrentItem >= 0 &&
//         iCurrentItem < iContainer->iListBox->Model()->NumberOfItems() )
//        {
//        iContainer->iListBox->SetCurrentItemIndexAndDraw( iCurrentItem );
//        }
    CheckMiddleSoftkeyLabelL();
    
	// If AppUid is different or placeholderview is used, this view has been launched from outside GS
	if ( iPrevViewId.iAppUid != KUidGS || iPrevViewId.iViewUid == TUid::Uid(0x0DEADBED ) )
		{
		CEikButtonGroupContainer* cbaGroup = Cba();
		if( cbaGroup )
			{
			HBufC* rightSKText = StringLoader::LoadLC ( R_GS_CBA_EXIT );
			TPtr rskPtr = rightSKText->Des();
			cbaGroup->SetCommandL( 2, EAknSoftkeyExit, *rightSKText );
			CleanupStack::PopAndDestroy( rightSKText );
			}
	}
    
    __GSLOGSTRING( "[GSCallPlugin] DoActivateL()-|" );
    }

// ---------------------------------------------------------------------------
//
// Deactivates the view
//
// ---------------------------------------------------------------------------
void CGSCallPlugin::DoDeactivate()
    {
    __GSLOGSTRING("[GSCallPlugin]--> DoDeactivate");
    if ( iContainer )
        {
        if ( iContainer->iListBox && iContainer->iListBox->View() )
            {
            iTopItemIndex = iContainer->iListBox->TopItemIndex();
            iCurrentItem = iContainer->iListBox->CurrentItemIndex();
            }
        CGSBaseView::DoDeactivate();
        }
    __GSLOGSTRING("[GSCallPlugin] <--DoDeactivate");
    }



// ---------------------------------------------------------------------------
//
// Handle status pane size changed
//
// ---------------------------------------------------------------------------
void CGSCallPlugin::HandleStatusPaneSizeChange()
	{
		if ( Container() )
			Container()->SetRect( ClientRect());
	}


// ---------------------------------------------------------------------------
//
// Shows setting page - for ON/OFF setting items
//
// ---------------------------------------------------------------------------
void CGSCallPlugin::ShowSettingPageL( TGSCallItemIds aPage )
    {
    __GSLOGSTRING("[GSCallPlugin]--> CGSCallPlugin::ShowSettingPageL");

    CDesCArrayFlat* items =
          new ( ELeave ) CDesCArrayFlat( KGSCallPopupMenuItems );
    CleanupStack::PushL( items );

    TInt titleID = 0;
    TInt currentSettingItem = KErrNone;
    TCallSettId keyName = EGSNotSet;

    switch ( aPage )
        {
        case EGSOpeningSlideAnswerCallItemId:
            keyName = EGSOpeningSlideAnswerCall;
            titleID = R_CP_SETTING_TITLE_OPENING_SLIDE;
            AppendItemL( *items, R_CP_SETTING_OPENING_SLIDE_ANSWER );
            AppendItemL( *items, R_CP_SETTING_OPENING_SLIDE_NOT_ANSWER );
            break;
        case EGSClosingSlideEndCallItemId:
            keyName = EGSClosingSlideEndCall;
            titleID = R_CP_SETTING_TITLE_CLOSING_SLIDE;
            AppendItemL( *items, R_CP_SETTING_CLOSING_SLIDE_END );
            AppendItemL( *items, R_CP_SETTING_CLOSING_SLIDE_NOT_END );
            break;        
        case EGSICSendIntCallIdItemId:
            titleID = R_SET_IC_CLIR;
            AppendItemL( *items, R_SET_IC_CLIR_ON );
            AppendItemL( *items, R_SET_IC_CLIR_OFF );
            break;
        case EGSICWaitingItemId:
            titleID = R_SET_IC_WAITING;
            AppendItemL( *items, R_SET_IC_WAITING_ACTIVE );
            AppendItemL( *items, R_SET_IC_WAITING_NOT_ACTIVE );
            break;
        case EGSICPreferredTelephonyId:
            titleID = R_SET_IC_PREFERRED_TELEPHONE;
            AppendItemL( *items, R_SET_IC_PREFERRED_TELEPHONE_CS );
            AppendItemL( *items, R_SET_IC_PREFERRED_TELEPHONE_PS );
            break;
        case EGSDndItemId:
            titleID = R_SET_IC_DND;
            AppendItemL( *items, R_SET_IC_DND_ACTIVE );
            AppendItemL( *items, R_SET_IC_DND_NOT_ACTIVE );
            break;
        case EGSCliItemId:
            titleID = R_OWN_NUMBER_SENDING_HEADING;

            // Restrict the default option if necessary
            if ( !iRestrictedClir )
                {
                AppendItemL( *items, R_CLI_SENDING_DEFAULT );
                }

            AppendItemL( *items, R_CLI_SENDING_ON );
            AppendItemL( *items, R_CLI_SENDING_OFF );
            break;           
        case EGSSummaryAfterCallItemId:
            keyName = EGSSummaryAfterCall;
            titleID = R_CTERM_SETTING;
            AppendItemL( *items, R_CALL_TERMINATION_NOTE_ON );
            AppendItemL( *items, R_CALL_TERMINATION_NOTE_OFF );
            break;   
        default:
            Panic( KGSNameOfClass, EInvalidIndex );
            break;
    }

    if ( aPage == EGSICSendIntCallIdItemId  ||
         aPage == EGSICWaitingItemId        ||
         aPage == EGSICPreferredTelephonyId ||
         aPage == EGSDndItemId )
        {
        currentSettingItem = GetLineForSelectedVoipSetting( aPage );
        }
    // CLI item does not have a keyName and/or literal, hence
    // this additional checking for CLI item.
    else if ( aPage != EGSCliItemId && keyName != EGSNotSet )    
        {
        iModel->GetCallSettingValue( keyName, currentSettingItem );
        __GSLOGSTRING1("[GSCallPlugin]    ShowSettingPageL: currentSettingItem: %d", currentSettingItem);
        // switch value to settings page value
        SwitchValue( currentSettingItem );
        }
    else
        {
        TInt err = iSsSettings->Get( ESSSettingsClir, currentSettingItem );
        __GSLOGSTRING1("[GSCallPlugin]    ShowSettingPageL: currentSettingItem1: %d", currentSettingItem);
        if ( err == KErrNone )
            {
            CliConvertValue( currentSettingItem );

            if ( iRestrictedClir )
                {
                // For indexing listbox which does not include KGSCliDefault
                currentSettingItem--;
                }
            }
        }

    HBufC* buffer = NULL;

    buffer = HBufC::NewLC( KGSMaxStringLength );
    TPtr str = buffer->Des();
    StringLoader::Load( str, titleID );

    TPtr string = buffer->Des();
    const TInt oldSettingItem = currentSettingItem;
    CAknRadioButtonSettingPage* dlg = new ( ELeave )
        CAknRadioButtonSettingPage(
            &string,
            EAknSettingPageNoOrdinalDisplayed,
            KGSUseDefault,
            KGSUseDefault,
            R_SETTING_PAGE,
            currentSettingItem,
            items );

    if ( dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged ) )
        {
        const TBool settingItemChanged = oldSettingItem != currentSettingItem;

        if ( aPage == EGSICSendIntCallIdItemId  ||
             aPage == EGSICWaitingItemId        ||
             aPage == EGSICPreferredTelephonyId ||
             aPage == EGSDndItemId )
            {
            // Change new VoIP setting based selected line.
            SetVoipSettingBasedSelectedLine( aPage, currentSettingItem );
            }
        else
            {
            if ( aPage != EGSCliItemId && keyName != EGSNotSet )
                {
                // switch settings page value to shared data value
                SwitchValue( currentSettingItem );
                iModel->SetCallSettingValue( keyName, currentSettingItem );
                __GSLOGSTRING1("[GSCallPlugin]    ShowSettingPageL: currentSettingItem2: %d", currentSettingItem);
                }
            else
                {
                if ( iRestrictedClir )
                    {
                    // For indexing listbox which does not include KGSCliDefault
                    currentSettingItem++;
                    }

                CliConvertValue( currentSettingItem );

                // Secure setting of Send Caller Id if required
                if ( !iSecureClir )
                    {
                    iSsSettings->Set( ESSSettingsClir, currentSettingItem );
                    }
                else
                    {
                    if ( settingItemChanged && MakeSecCodeQueryL() )
                        {
                        iSsSettings->Set( ESSSettingsClir, currentSettingItem );
                        }
                    }
                __GSLOGSTRING1("[GSCallPlugin]    ShowSettingPageL: currentSettingItem3: %d", currentSettingItem);
                }
            }

        UpdateListBoxL( aPage, currentSettingItem );
        }

    CleanupStack::PopAndDestroy( buffer );
    CleanupStack::PopAndDestroy( items );
    __GSLOGSTRING("[GSCallPlugin] <--CGSCallPlugin::ShowSettingPageL");
    }

// ---------------------------------------------------------------------------
//
// Shows ALS setting page - either line blocking or line selected
//
// ---------------------------------------------------------------------------
void CGSCallPlugin::ShowAlsSettingPageL( TGSCallItemIds aPage  )
    {
    __GSLOGSTRING("[GSCallPlugin]--> CGSCallPlugin::ShowAlsSettingPageL");
    CDesCArrayFlat* items = new ( ELeave ) CDesCArrayFlat( KGSCallPopupMenuItems );
    CleanupStack::PushL( items );

    TInt currentItem = 0;

    switch ( aPage )
        {
        case EGSLineChangeItemId:
            //causes PIN2 query to launch, if necessary
            if ( iAlsBlock == ESSSettingsAlsBlockingOn )
                {
                TInt pin = MakePin2QueryL();
                if (  pin != KGSPinQueryOk )
                    {
                    CleanupStack::PopAndDestroy( items );
                    return; //User failed in Pin2 Query
                    }
                }
            AppendItemL( *items, R_ALS_LINE1 );
            AppendItemL( *items, R_ALS_LINE2 );
            currentItem = iAls - 1;
            break;
        case EGSALSBlockItemId:
            {
                TInt pin = MakePin2QueryL();
                if ( pin != KGSPinQueryOk )
                    {
                    CleanupStack::PopAndDestroy( items );
                    return; //User failed in Pin2 Query
                    }
                AppendItemL( *items, R_ALS_LINE_BLOCKING_ON );
                AppendItemL( *items, R_ALS_LINE_BLOCKING_OFF );
                if ( iAlsBlock == ESSSettingsAlsBlockingOn )
                    {
                    currentItem = KGSAlsBlockOn;
                    }
                else
                    {
                    currentItem = KGSAlsBlockOff;
                    }
                break;
            }
        default:
            Panic( KGSNameOfClass, EInvalidIndex );
            break;
        }
    HBufC* buffer =
        HBufC::NewLC( KGSMaxStringLength );
    TPtr string( buffer->Des() );
    StringLoader::Load(
        string,
        ( aPage == EGSLineChangeItemId ) ?
            R_ALS_LINE_SELECTION_MODE :
            R_ALS_PHONELINE_BLOCK_MODE );

    CAknRadioButtonSettingPage* dlg = new ( ELeave )
        CAknRadioButtonSettingPage(
            &string,
            EAknSettingPageNoOrdinalDisplayed,
            KGSUseDefault,
            KGSUseDefault,
            R_SETTING_PAGE,
            currentItem,
            items );

    // handle als and blocking separately...
    if ( dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged ) )
        {
        if ( aPage == EGSALSBlockItemId )
            {
            AlsBlockConvertValue( currentItem );
            iSsSettings->Set( ESSSettingsAlsBlocking, iAlsBlock );
            }
        else if ( aPage == EGSLineChangeItemId )
            {
            iAls = currentItem + 1;
            iSsSettings->Set( ESSSettingsAls, iAls );
            }
        }
    CleanupStack::PopAndDestroy( 2 ); // items, string
    __GSLOGSTRING("[GSCallPlugin] <--CGSCallPlugin::ShowAlsSettingPageL");
    }


// ---------------------------------------------------------------------------
//
// Changes value from 0 to 1 and vice versa
// Used when Central Repository is enabled
//
// ---------------------------------------------------------------------------
void CGSCallPlugin::SwitchOnOffSetting( TCallSettId aSetting )
    {
    TInt value = KGSSettingOff;
    // Reads a value and switches value (0 -> 1, 1 -> 0).
    iModel->GetCallSettingValue( aSetting, value );
    SwitchValue( value );
    iModel->SetCallSettingValue( aSetting, value );
    }

// ---------------------------------------------------------------------------
//
// Appends given item to given list
//
// ---------------------------------------------------------------------------
void CGSCallPlugin::AppendItemL( CDesCArrayFlat& aList, TInt aItem )
    {
    HBufC* string = StringLoader::LoadLC( aItem );
    aList.AppendL( *string );
    CleanupStack::PopAndDestroy( string );
    }

// ---------------------------------------------------------------------------
//
// Selects what kind of menu is created
//
// ---------------------------------------------------------------------------
void CGSCallPlugin::HandleCreatePopupL( TInt aIndex )
    {
    __GSLOGSTRING("[GSCallPlugin]--> CGSCallPlugin::HandleCreatePopupL");
    switch ( aIndex )
        {
        case EGSCallWaitingItemId:
            break;
        case EGSLineChangeItemId:
            ShowAlsSettingPageL( EGSLineChangeItemId );
            break;
        case EGSALSBlockItemId:
            ShowAlsSettingPageL( EGSALSBlockItemId );
            break;
        case EGSSoftRejectTextItemId:
            ShowSoftRejectSettingPageL();
            UpdateListBoxL( EGSSoftRejectTextItemId, KGSNotUsed );
            break;
        case EGSStillImageItemId:
            if ( FeatureManager::FeatureSupported(
                                 KFeatureIdCsVideoTelephony ) )
                {
                ShowVTStillImageSettingPageL();
                }
            break;
        case EGSOwnImageVtCallItemId:
            if ( FeatureManager::FeatureSupported(
                                 KFeatureIdCsVideoTelephony ) )
                {
                ShowOwnImageVtCallSettingPageL();
                }
            break;
        case EGSReplacePrefixItemId:
            if ( FeatureManager::FeatureSupported( KFeatureIdJapanPrefixChange ) )
                {
                ChangePrefixSettingL( ETrue );
                UpdateListBoxL( EGSReplacePrefixItemId, KGSNotUsed );
                }
            break;
        case EGSShowCallDurationItemId:
            ChangeCallDurationSettingL( ETrue );
            UpdateListBoxL( EGSShowCallDurationItemId, KGSNotUsed );
            break;
        case EGSLongPressCallKeyItemId:
        #ifdef RD_VT_LONG_SEND_KEY 
        	ChangeLongPressCallKeySettingL( ETrue );
        #endif
        	break;
        default:
            ShowSettingPageL( static_cast<TGSCallItemIds> ( aIndex ) );
            break;
        }
    __GSLOGSTRING("[GSCallPlugin] <--CGSCallPlugin::HandleCreatePopupL");
    }

// ---------------------------------------------------------------------------
//
// Sets title text to a setting page
//
// ---------------------------------------------------------------------------
void CGSCallPlugin::SetTitleToSettingL( CAknRadioButtonSettingPage& aDlg,
                                            TInt aTitleID )
    {
    HBufC* title = StringLoader::LoadLC ( aTitleID );
    aDlg.SetSettingTextL( *title );
    CleanupStack::PopAndDestroy( title );
    }


/*// ---------------------------------------------------------------------------
//
// Sets title text to a status pane
//
// ---------------------------------------------------------------------------
void CGSCallPlugin::SetTitleL( TInt aTitleID )
    {
    CEikStatusPane* statusPane = iEikonEnv->AppUiFactory()->StatusPane();
    CleanupStack::PushL(statusPane);
	CAknTitlePane* titlePane = (CAknTitlePane*) statusPane->ControlL(TUid::Uid(
			EEikStatusPaneUidTitle));
	CleanupStack::PushL( titlePane );
	
    TResourceReader reader;
    iCoeEnv->CreateResourceReaderLC( reader, aTitleID );
    titlePane->SetFromResourceL( reader ); 
    
    CleanupStack::PopAndDestroy();//reader
	
	CleanupStack::Pop(titlePane);
	CleanupStack::Pop(statusPane);
    }
*/

// ---------------------------------------------------------------------------
//
// Switches value between setting page and shared data.
//  Values in shared data do not map nicely to values in setting menu,
//  therefore sharedData:SettingOn(1) => settingPage:IndexOn(0).
// ---------------------------------------------------------------------------
TInt CGSCallPlugin::SwitchValue( TInt& aValue )
    {
    if ( aValue == KGSSettingOn )
        {
        aValue = KGSIndexOn;
        }
    else if ( aValue == KGSSettingOff )
        {
        aValue = KGSIndexOff;
        }
    return aValue;
    }

// ---------------------------------------------------------------------------
//
// Observes changes to ALS, ALS Block and CLIR values.
//
// ---------------------------------------------------------------------------
void CGSCallPlugin::PhoneSettingChanged( TSSSettingsSetting aSetting,
    TInt aNewValue )
    {
    __GSLOGSTRING("[GSCallPlugin]--> CGSCallPlugin::PhoneSettingChanged");
    TInt updateMe = KErrNone;
    switch( aSetting )
        {
        case ESSSettingsClir:
            iClir = aNewValue;
            updateMe = EGSCliItemId;
            break;
        case ESSSettingsAls:
            iAls = aNewValue;
            updateMe = EGSLineChangeItemId;
            break;
        case ESSSettingsAlsBlocking:
            iAlsBlock = aNewValue;
            updateMe = EGSALSBlockItemId;
            TRAP_IGNORE( ShowAlsBlockNoteL() );
            break;
        default:
            break;
        }
    __GSLOGSTRING1("[GSCallPlugin]    PhoneSettingChanged: aNewValue: %d", aNewValue);
    if ( ( iContainer ) && ( updateMe != KErrNone ) )
        {
        TRAP_IGNORE( UpdateListBoxL( updateMe, aNewValue ) );
        }
    __GSLOGSTRING("[GSCallPlugin] <--CGSCallPlugin::PhoneSettingChanged");
    }

// ---------------------------------------------------------------------------
//
// Shows notes when Als Block value has been changed.
//
// ---------------------------------------------------------------------------
void CGSCallPlugin::ShowAlsBlockNoteL()
    {
    TInt noteText;
    if ( iAlsBlock == ESSSettingsAlsBlockingOn )
        {
        noteText = R_ALS_LINE_BLOCKING_ON_SEL;
        }
    else
        {
        noteText = R_ALS_LINE_BLOCKING_OFF_SEL;
        }

    HBufC* string = StringLoader::LoadLC( noteText );
    CAknInformationNote* note = new ( ELeave ) CAknInformationNote();
    note->ExecuteLD( *string );
    CleanupStack::PopAndDestroy( string );
    }

// ---------------------------------------------------------------------------
//
// Creates new iContainer.
//
// ---------------------------------------------------------------------------
void CGSCallPlugin::NewContainerL()
    {
    __GSLOGSTRING("[GSCallPlugin]--> NewContainerL");
    iContainer = new (ELeave) CGSCallPluginContainer;
    __GSLOGSTRING("[GSCallPlugin] <--NewContainerL");
    }

// ---------------------------------------------------------------------------
// CGSCallPlugin::HandleListBoxSelectionL
//
// Handle user selection in the listbox to perform an associated action
// ---------------------------------------------------------------------------
void CGSCallPlugin::HandleListBoxSelectionL()
    {
    TBool updateValue = EFalse;
    TInt value( KErrNone );
    CEikMenuBar* menuBar = this->MenuBar();

    const TInt currentFeatureId = Container()->CurrentFeatureId();

    TBool featureVoipSupported = iModel->VoIPSupported();

    switch ( currentFeatureId )
        {
        case EGSOpeningSlideAnswerCallItemId:
            SwitchOnOffSetting( EGSOpeningSlideAnswerCall );
            updateValue = ETrue;
            break;
        case EGSClosingSlideEndCallItemId:
            SwitchOnOffSetting( EGSClosingSlideEndCall );
            updateValue = ETrue;
            break;
        case EGSCliItemId:
            ShowSettingPageL( EGSCliItemId );
            break;
        case EGSCallWaitingItemId:
            iRockerPress = ETrue;
            menuBar->SetMenuType(CEikMenuBar::EMenuContext);
            menuBar->TryDisplayMenuBarL();
            menuBar->SetMenuType(CEikMenuBar::EMenuOptions);
            iRockerPress = EFalse;
            break;
        case EGSICSendIntCallIdItemId:  // Send my Internet call id.
            if ( featureVoipSupported )
                {
                iModel->GetVoIPCallSettingValue( EGSVoIPSendIntCallId, value );

                if ( value == EGSSendIntCallIdOff )
                    {
                    iModel->SetVoIPCallSettingValue(
                        EGSVoIPSendIntCallId, EGSSendIntCallIdOn );
                    }
                else
                    {
                    iModel->SetVoIPCallSettingValue(
                        EGSVoIPSendIntCallId, EGSSendIntCallIdOff );

                    TInt status( KErrNone );
                    iModel->GetSCCPStatus( status );

                    if ( status == 1 )
                        {
                        HBufC* prompt = StringLoader::
                            LoadLC( R_QTN_VOIP_SCCP_NO_CLIR );
                        CAknInformationNote* note =
                            new(ELeave) CAknInformationNote( ETrue );
                        note->ExecuteLD( *prompt );
                        CleanupStack::PopAndDestroy( prompt );
                        }
                    }
                updateValue = ETrue;
                }
            break;
        case EGSICWaitingItemId:  // Internet call waiting item.
            if ( featureVoipSupported )
                {
                iModel->GetVoIPCallSettingValue( EGSVoIPCW, value );

                if ( value == EGSCSCallWaitingOff )
                    {
                    iModel->SetVoIPCallSettingValue(
                        EGSVoIPCW, EGSCSCallWaitingOn );
                    }
                else
                    {
                    iModel->SetVoIPCallSettingValue(
                        EGSVoIPCW, EGSCSCallWaitingOff );
                    }
                updateValue = ETrue;
                }
            break;
        case EGSICPreferredTelephonyId: // Preferred call type.
            if ( featureVoipSupported )
                {
                iModel->GetVoIPCallSettingValue( EGSVoIPPreType, value );

                if ( value == EGSPreferredCS )
                    {
                    iModel->SetVoIPCallSettingValue(
                        EGSVoIPPreType, EGSPreferredPS );
                    
                    // Show message query dialog if preferred call
                    // setting was changed to the Int. Telephone.
                    ShowPreferredTelephonyNoteL();     
                    }
                else
                    {
                    iModel->SetVoIPCallSettingValue(
                        EGSVoIPPreType, EGSPreferredCS );
                    }
                updateValue = ETrue;
                }
            break;
        case EGSDndItemId: // Do not disturb.
            if ( featureVoipSupported )
                {
                iModel->GetVoIPCallSettingValue( EGSVoIPDnd, value );

                if ( value == EGSCSCallDndOff )
                    {
                    iModel->SetVoIPCallSettingValue(
                        EGSVoIPDnd, EGSCSCallDndOn );
                    }
                else
                    {
                    iModel->SetVoIPCallSettingValue(
                        EGSVoIPDnd, EGSCSCallDndOff );
                    }
                updateValue = ETrue;
                }
            break;
        case EGSSoftRejectTextItemId:
            ShowSoftRejectSettingPageL();
            updateValue = ETrue;
            break;
        case EGSSummaryAfterCallItemId:
            SwitchOnOffSetting( EGSSummaryAfterCall );
            updateValue = ETrue;
            break;
        case EGSLineChangeItemId:
            SwitchAlsSettingL();
            break;
        case EGSALSBlockItemId:
            ShowAlsSettingPageL( EGSALSBlockItemId );
            break;
        //Still image listbox creating
        case EGSStillImageItemId:
            if ( FeatureManager::FeatureSupported(
                                 KFeatureIdCsVideoTelephony ) && 
                                 !iVTImageSelectionOngoing )
                {
                //Locking listbox selection so user cannot click
                //VT still image item twice
                iVTImageSelectionOngoing = ETrue;
                
                //This takes some time
                ShowVTStillImageSettingPageL();
                
                //Releasing lock
                iVTImageSelectionOngoing = EFalse;
                }
            break;
        case EGSOwnImageVtCallItemId:
            if ( FeatureManager::FeatureSupported(
                                 KFeatureIdCsVideoTelephony ) )
                {
                ShowOwnImageVtCallSettingPageL();
                }
            break;
        case EGSReplacePrefixItemId:
            if ( FeatureManager::FeatureSupported( KFeatureIdJapanPrefixChange ) )
                {
                ChangePrefixSettingL( EFalse );
                updateValue = ETrue;
                }
            break;
        case EGSShowCallDurationItemId:
            ChangeCallDurationSettingL( EFalse );
            updateValue = ETrue;
            break;
        case EGSLongPressCallKeyItemId:
        #ifdef RD_VT_LONG_SEND_KEY
        	ChangeLongPressCallKeySettingL( EFalse );
        #endif
        	break;
        default:
            break;
        }
    if ( updateValue )
        {
        UpdateListBoxL( currentFeatureId, KGSNotUsed );
        }
    }

// ---------------------------------------------------------------------------
//
// Converts value from setting page to SsSettings:
// settin page: line change disabled(0) => ESSSettingsAlsBlockingOn(2)
// settin page: line change enabled(1) => ESSSettingsAlsBlockingOff(1)
// ---------------------------------------------------------------------------
//
void CGSCallPlugin::AlsBlockConvertValue( TInt& aValue )
    {
    __GSLOGSTRING("[GSCallPlugin]--> CGSCallPlugin::AlsBlockConvertValue");
    __GSLOGSTRING1("[GSCallPlugin]    AlsBlockConvertValue: aValue: %d", aValue);
    if ( aValue == KGSAlsBlockOn )
        {
        iAlsBlock = ESSSettingsAlsBlockingOn;
        }
    else
        {
        iAlsBlock = ESSSettingsAlsBlockingOff;
        }
    __GSLOGSTRING1("[GSCallPlugin]    AlsBlockConvertValue: aValue: %d", aValue);
    __GSLOGSTRING("[GSCallPlugin] <--CGSCallPlugin::AlsBlockConvertValue");
    }

// ---------------------------------------------------------------------------
//
// Converts value from setting page to SsSettings:
// settin page: own number sending default(0) =>
//                               SsSettings: ESSSettingsClirNetworkDefault(0)
// settin page: own number sending on (1) =>
//                               SsSettings: ESSSettingsClirExplicitSuppress(2)
// settin page: own number sending off (2) =>
//                               SsSettings: ESSSettingsClirExplicitInvoke(1)
// ---------------------------------------------------------------------------
//
void CGSCallPlugin::CliConvertValue( TInt& aValue )
    {
    switch ( aValue )
        {
        case KGSCliDefault:
            aValue = ESSSettingsClirNetworkDefault;
            break;
        case KGSCliOn:
            aValue = ESSSettingsClirExplicitSuppress;
            break;
        case KGSCliOff:
            aValue = ESSSettingsClirExplicitInvoke;
            break;
        default:
            //error, return to-be converted value w/o changes
            break;
        }
    }

// ---------------------------------------------------------------------------
//
// Updates specific value to container.
//
// ---------------------------------------------------------------------------
//
void CGSCallPlugin::UpdateListBoxL( const TInt& aListItem,
                                          TInt aNewValue )
    {
    /*if ( aListItem == EGSStillImageItemId )
    	{
        TRAP_IGNORE( iAvkonAppUi->StatusPane()->SwitchLayoutL( R_AVKON_STATUS_PANE_LAYOUT_USUAL ) );
        SetTitleL( R_GS_CALL_VIEW_TITLE );
        TRAP_IGNORE( iAvkonAppUi->StatusPane()->DrawNow() );
    	}*/
    if ( Container() )
        {
        Container()->UpdateListBoxL( aListItem, aNewValue );
        }
    CheckMiddleSoftkeyLabelL();
    }

// ---------------------------------------------------------------------------
//
// Before showing a options menu
//
// ---------------------------------------------------------------------------
//
void CGSCallPlugin::DynInitMenuPaneL( TInt aResourceId,
                                      CEikMenuPane* aMenuPane )
    {
    __GSLOGSTRING("[GSCallPlugin]--> DynInitMenuPaneL");
    const TInt currentFeatureId = Container()->CurrentFeatureId();
    CEikMenuBar* menuBar = this->MenuBar();
    if ( aResourceId == R_GS_CALL_VIEW_MENU )
        {
        if ( currentFeatureId == EGSCallWaitingItemId  )
            {
            //add CW menu
            if (MenuBar()->ItemSpecificCommandsEnabled() || MenuBar()->GetMenuType() == CEikMenuBar::EMenuContext)
            	{
            	aMenuPane->AddMenuItemsL( R_GS_CALL_WAIT_VIEW_MENU,EAppCmdCreatePopupList );
            	}
            
            //hide Change if call waiting is selected
            aMenuPane->SetItemDimmed( EAppCmdCreatePopupList, ETrue );
            
            if ( iRockerPress )
                {
                aMenuPane->SetItemDimmed( EAknCmdExit, ETrue );
                }
            
            if ( FeatureManager::FeatureSupported( KFeatureIdHelp )
                 && !iRockerPress )
                {
                aMenuPane->SetItemDimmed( EAknCmdHelp, EFalse );
                }
            else
                {
                aMenuPane->SetItemDimmed( EAknCmdHelp, ETrue );
                }
            }
        else
            {
            //for other items than call waiting
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
    
    __GSLOGSTRING("[GSCallPlugin] <--DynInitMenuPaneL");
    }

// ---------------------------------------------------------------------------
//
// Requests user to enter PIN2
//
// ---------------------------------------------------------------------------
//
TInt CGSCallPlugin::MakePin2QueryL()
    {
    TInt retValue = KErrNone;
    TSecUi::InitializeLibL();

    CSecuritySettings* settings = CSecuritySettings::NewL();
    CleanupStack::PushL( settings );

    if ( settings->AskPin2L() )
        {
        retValue = KGSPinQueryOk;
        }

    CleanupStack::PopAndDestroy( settings );
    TSecUi::UnInitializeLib();

    return retValue;
    }

// ---------------------------------------------------------------------------
//
// Requests user to enter security code
//
// ---------------------------------------------------------------------------
//
TInt CGSCallPlugin::MakeSecCodeQueryL()
    {
    TBool retValue = EFalse;
    TSecUi::InitializeLibL();

    CSecuritySettings* settings = CSecuritySettings::NewL();
    CleanupStack::PushL( settings );

    retValue = settings->AskSecCodeL();

    CleanupStack::PopAndDestroy( settings );
    TSecUi::UnInitializeLib();

    return retValue;
    }

// ---------------------------------------------------------------------------
//
// Changes value in shared data for Als
//
// ---------------------------------------------------------------------------
//
void CGSCallPlugin::SwitchAlsSettingL()
    {
    __GSLOGSTRING("[GSCallPlugin]--> CGSCallPlugin::SwitchAlsSettingL");
    //causes PIN2 query to launch, if necessary
    if ( iAlsBlock == ESSSettingsAlsBlockingOn )
        {
        TInt pin = MakePin2QueryL();
        if (  pin != KGSPinQueryOk )
            {
            __GSLOGSTRING("[GSCallPlugin]    SwitchAlsSettingL: User failed in Pin2 Query");
            return; //User failed in Pin2 Query
            }
        }
    iSsSettings->Get( ESSSettingsAls, iAls );
    if ( iAls == ESSSettingsAlsPrimary )
        {
        iAls = ESSSettingsAlsAlternate;
        }
    else
        {
        iAls = ESSSettingsAlsPrimary;
        }
    iSsSettings->Set( ESSSettingsAls, iAls );
    __GSLOGSTRING("[GSCallPlugin] <--CGSCallPlugin::SwitchAlsSettingL");
    }


// ---------------------------------------------------------------------------
//
// Show Soft Reject Text setting page
//
// ---------------------------------------------------------------------------
//
void CGSCallPlugin::ShowSoftRejectSettingPageL()
    {
    __GSLOGSTRING("[GSCallPlugin]--> ShowSoftRejectSettingPageL");
    HBufC* dynamicText = HBufC::NewLC( KGSSMSTxtEditorLength );
    TPtr string( dynamicText->Des() );

    Container()->GetSoftRejectTextL( string );

    HBufC* title = Container()->ReadFromResourceL( R_SOFT_REJECT_TXT_TITLE );
    CleanupStack::PushL( title );

    // Open resource file, to get the resource texts for setting page.
    // Find the resource file
    RConeResourceLoader loader( *iCoeEnv );
    TParse parse;
    parse.Set( KGSSoftRejectResource, &KDC_RESOURCE_FILES_DIR, NULL );
    TFileName fileName( parse.FullName() );

    // Get language of resource file
    BaflUtils::NearestLanguageFile( iCoeEnv->FsSession(), fileName );

    // Open resource file
    TInt err = loader.Open( fileName );
    if ( err == KErrNotFound )
        {
        User::Leave( KErrGeneral );
        }
    CleanupClosePushL( loader );

    CAknSettingPage* dlg = new( ELeave )CAknTextSettingPage(
        title,
        EAknSettingPageNoOrdinalDisplayed,
        KGSUseDefault,
        KGSUseDefault,
        R_TEXT_SETTING_PAGE,
        string,
        CAknTextSettingPage::EZeroLengthAllowed |
        CAknTextSettingPage::EPredictiveTextEntryPermitted );

    if ( dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged ) )
        {
        TInt value = KGSSettingOn;
        iModel->SetCallSettingValue( EGSSoftRejectDefault, value );
        AknTextUtils::StripCharacters( string, KAknStripTabs );
        iModel->SetSoftRejectText( string );
        }

    CleanupStack::PopAndDestroy( 3 ); //title, dynamicText, loader
    __GSLOGSTRING("[GSCallPlugin] <--ShowSoftRejectSettingPageL");
    }


// ---------------------------------------------------------------------------
//
// Show Replace Prefix setting page
//
// ---------------------------------------------------------------------------
//
TBool CGSCallPlugin::ShowPrefixSettingPageL( TInt& aCurrentValue )
    {
    __GSLOGSTRING("[GSCallPlugin]--> ShowPrefixSettingPageL");
    CDesCArrayFlat* items =
        iCoeEnv->ReadDesC16ArrayResourceL( R_GS_REPLACE_PREFIX_SETTING_PAGE_LBX );
    CleanupStack::PushL( items );

    CAknRadioButtonSettingPage* dlg =
        new (ELeave) CAknRadioButtonSettingPage( R_GS_REPLACE_PREFIX_SETTING_PAGE,
                                                aCurrentValue, items );

    TBool ret = dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged );
    CleanupStack::PopAndDestroy( items );

    __GSLOGSTRING1("[GSCallPlugin]    ShowPrefixSettingPageL: aCurrentValue: %d", aCurrentValue);
    __GSLOGSTRING("[GSCallPlugin] <--ShowPrefixSettingPageL");
    return ret;
    }

// ---------------------------------------------------------------------------
// CGSCallPlugin::ChangePrefixSettingL
//
// Change replace prefix setting.
// ---------------------------------------------------------------------------
//
void CGSCallPlugin::ChangePrefixSettingL( TBool aSettingPage )
    {
    __GSLOGSTRING("[GSCallPlugin]--> ChangePrefixSettingL");

    TInt changeMode = iModel->PrefixChangeModeL();
    TInt prevChangeMode = changeMode;
    TBool updateValue = ETrue;
    TBool rockerPress = EFalse;

    __GSLOGSTRING1("[GSCallPlugin]    ChangePrefixSettingL: prevChangeMode: %d", prevChangeMode);
    if ( aSettingPage )
        {
        updateValue = ShowPrefixSettingPageL( changeMode );
        }
    else
        {
        rockerPress = ETrue;
        if( changeMode == KGSSettingItemOff )
            {
            changeMode = KGSSettingItemOn;
            }
        else
            {
            changeMode = KGSSettingItemOff;
            }
        }

    __GSLOGSTRING1("[GSCallPlugin]    ChangePrefixSettingL: changeMode: %d", changeMode);
    if ( updateValue )
        {
        //if the value is changed and is set to ON, display the query dialog
        if ( changeMode == KGSSettingItemOn )
            {
            HBufC* replacePrefix = HBufC::NewLC( KGSReplacePrefixTextLength );
            TPtr string( replacePrefix->Des() );
            string.Zero();

            iModel->PrefixChangeDataL( string );

            CAknTextQueryDialog* dlg = new( ELeave ) CAknTextQueryDialog( string );

            TBool ret = dlg->ExecuteLD( R_REPLACE_PREFIX_DATA_QUERY );
            if( ret )
                {
                rockerPress = ETrue;
                iModel->SetPrefixChangeDataL( string );
                }
            else
                {
                rockerPress = EFalse;
                }
            CleanupStack::PopAndDestroy( replacePrefix );

            }
        else
            {
            rockerPress = ETrue;
            }
        }

    if ( prevChangeMode != changeMode && rockerPress )
        {
        iModel->SetPrefixChangeModeL( changeMode );
        }

    __GSLOGSTRING("[GSCallPlugin] <--ChangePrefixSettingL");
    }


// ---------------------------------------------------------------------------
// CGSCallPlugin::SetVoipSettingBasedSelectedLine
//
// Set the new changes to VOIP variables.
// ---------------------------------------------------------------------------
//
void CGSCallPlugin::SetVoipSettingBasedSelectedLine(
    const TGSCallItemIds aPage, TInt aCurrentSetting )
    {
    switch ( aPage )
        {
        case EGSICSendIntCallIdItemId:
            if ( aCurrentSetting == KGSFirstLine )
                {
                iModel->SetVoIPCallSettingValue(
                    EGSVoIPSendIntCallId, EGSSendIntCallIdOn );
                }
            else
                {
                iModel->SetVoIPCallSettingValue(
                    EGSVoIPSendIntCallId, EGSSendIntCallIdOff );
                }
            break;
        case EGSICWaitingItemId:
            if ( aCurrentSetting == KGSFirstLine )
                {
                iModel->SetVoIPCallSettingValue(
                    EGSVoIPCW, EGSCSCallWaitingOn );
                }
            else
                {
                iModel->SetVoIPCallSettingValue(
                    EGSVoIPCW, EGSCSCallWaitingOff );
                }
            break;
        case EGSICPreferredTelephonyId:
            if ( aCurrentSetting == KGSFirstLine )
                {
                iModel->SetVoIPCallSettingValue(
                    EGSVoIPPreType, EGSPreferredCS );
                }
            else
                {
                iModel->SetVoIPCallSettingValue(
                    EGSVoIPPreType, EGSPreferredPS );
                           
                // Show message query dialog if preferred call
                // setting was changed to the Int. Telephone.
                TRAP_IGNORE( ShowPreferredTelephonyNoteL() );
                }
            break;
        case EGSDndItemId:
            if ( aCurrentSetting == KGSFirstLine )
                {
                iModel->SetVoIPCallSettingValue(
                    EGSVoIPDnd, EGSCSCallDndOn );
                }
            else
                {
                iModel->SetVoIPCallSettingValue(
                    EGSVoIPDnd, EGSCSCallDndOff );
                }
            break;
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// CGSCallPlugin::Container
//
// Returns call container item
// ---------------------------------------------------------------------------
//
CGSCallPluginContainer* CGSCallPlugin::Container()
    {
    return static_cast <CGSCallPluginContainer*> ( iContainer );
    }

// ---------------------------------------------------------------------------
// CGSCallPlugin::GetLineForSelectedVoipSetting
//
// Returns selected line based on setting value.
// ---------------------------------------------------------------------------
//
TInt CGSCallPlugin::GetLineForSelectedVoipSetting(
    const TGSCallItemIds aPage )
    {
    if ( !iModel->VoIPSupported() )
        {
        return KErrNone;
        }

    TInt value( KErrNone );

    // Get value based from setting page.
    switch ( aPage )
        {
        case EGSICSendIntCallIdItemId:  // No swap.
            iModel->GetVoIPCallSettingValue( EGSVoIPSendIntCallId, value );
            break;
        case EGSICWaitingItemId:
            iModel->GetVoIPCallSettingValue( EGSVoIPCW, value );
            break;
        case EGSICPreferredTelephonyId: // No swap.
            iModel->GetVoIPCallSettingValue( EGSVoIPPreType, value );
            break;
        case EGSDndItemId:
            iModel->GetVoIPCallSettingValue( EGSVoIPDnd, value );
            break;
        default:
            break;
        }

    // Swap items.
    if ( aPage != EGSICPreferredTelephonyId &&
        aPage != EGSICSendIntCallIdItemId )
        {
        if ( value == KGSSecondLine )
            {
            value = KGSFirstLine;
            }
        else
            {
            value = KGSSecondLine;
            }
        }

    return value;
    }


// ---------------------------------------------------------------------------
// CGSCallPlugin::ShowVTStillImageSettingPageL
//
// Display Video Telephony Still Image setting page.
// ---------------------------------------------------------------------------
//
void CGSCallPlugin::ShowVTStillImageSettingPageL()
    {
    iVtStillImage = iModel->VTStillImageL();
    // Store previous in case something goes wrong.
    iPreviousVtStillImage = iVtStillImage;
    
    CDesCArrayFlat* items = iCoeEnv->ReadDesC16ArrayResourceL(
                                     R_GS_STILL_IMAGE_SETTING_PAGE_LBX );
    CleanupStack::PushL( items );

    // Map CenRep value to lbx items:
    TInt dialogSelection = EGSVtStillImageNotInUse;
    if ( iVtStillImage != KGSVTStillImageOff ) dialogSelection = EGSVtStillImageInUse;

    CAknRadioButtonSettingPage* dlg =
        new( ELeave ) CAknRadioButtonSettingPage( R_GS_STILL_IMAGE_SETTING_PAGE,
                                                  dialogSelection, items );

    TBool ret = dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged );

    if ( ret )
        {
        if ( dialogSelection == EGSVtStillImageNotInUse )
            {
            // Still image not used
            iModel->SetVTStillImageL( KGSVTStillImageOff );
            }
        else
            {
            // User decided to update still image
            ShowStillImageListL();
            }
            
        // Update VT item in CGSCallPlugin listbox. iVtStillImage value is 
        // irrelevant as UpdateListBoxL actually reads the new value from 
        // CenRep and converts it to lbx item value correctly (Off/On).
        UpdateListBoxL( EGSStillImageItemId, iVtStillImage );
        }
    CleanupStack::PopAndDestroy( items );
    }


// ---------------------------------------------------------------------------
// CGSCallPlugin::ShowStillImageListL()
//
// Display background image list from media gallery.
// ---------------------------------------------------------------------------
//
void CGSCallPlugin::ShowStillImageListL()
    {
    CDesCArray* selectedFiles = new ( ELeave ) CDesCArrayFlat( 1 );
    CleanupStack::PushL( selectedFiles );

    TBool ret = MGFetch::RunL( *selectedFiles, EImageFile, EFalse, this );

    if ( ret && selectedFiles->MdcaCount() == 1 )
        {
        iImageHandler = CGSAsyncImageHandling::NewL( iCoeEnv->FsSession(),
                                               this, KGSVTStillImgPath );

        TPtrC imagePath = selectedFiles->MdcaPoint( 0 );

        TRAPD( error, iModel->iBackgroundApi->SetVTStillImagePathL( imagePath,
                                             *iImageHandler ) );
        if ( error == KErrNone )
            {
            // Show wait note
            ShowImageLoadWaitNoteL();
            }
        else
            {
            HandleImageErrorsL( error );
            }
        }
    CleanupStack::PopAndDestroy( selectedFiles );
    }


// ---------------------------------------------------------------------------
// CGSCallPlugin::ImageHandlingComplete()
//
// Method derived from MGSSyncImageHandlingObserver
// Run when the asynchronous image converting is complete
// ---------------------------------------------------------------------------
//
void CGSCallPlugin::ImageHandlingCompleteL( TInt aError )
    {
    if ( aError != KErrNone )
        {
        HandleImageErrorsL( aError );
        }
    else
        {
        // Needs to increment iVtStillImage value so CenRep will send 
        // notificatios to VT (only changing value will fire notification).
        iModel->SetVTStillImageL( ++iVtStillImage );
        UpdateListBoxL( EGSStillImageItemId, EGSVtStillImageInUse );
        }

    HideImageLoadWaitNoteL();

    delete iImageHandler;
    iImageHandler = NULL;
    }


// ---------------------------------------------------------------------------
// CGSCallPlugin::ShowImageLoadWaitNoteL()
//
// Display image load wait note dialog.
// ---------------------------------------------------------------------------
//
void CGSCallPlugin::ShowImageLoadWaitNoteL()
    {
    if ( !iWaitDialog )
        {
        iWaitDialog = new( ELeave ) CAknWaitDialog(
                      ( REINTERPRET_CAST(CEikDialog**,
                      &iWaitDialog ) ), ETrue );

        iWaitDialog->ExecuteDlgLD( CAknNoteDialog::ENoTone,
                                   R_IMAGE_LOAD_WAIT_NOTE );
        }
    }

// ---------------------------------------------------------------------------
// CGSCallPlugin::HideImageLoadWaitNoteL()
//
// Hide image load wait note dialog.
// ---------------------------------------------------------------------------
//
void CGSCallPlugin::HideImageLoadWaitNoteL()
    {
    if ( iWaitDialog )
        {
        iWaitDialog->ProcessFinishedL(); // deletes the dialog
        iWaitDialog = NULL;
        }
    }


// ---------------------------------------------------------------------------
// CGSCallPlugin::HandleImageErrorsL
//
// Prompt image related errors to the user
// ---------------------------------------------------------------------------
//
void CGSCallPlugin::HandleImageErrorsL( TInt aError )
    {
    TInt resourceId;
    switch( aError )
        {
        case KErrNotSupported:
        case KErrUnderflow:
            // Image is corrupted or in wrong format
            resourceId = R_GS_IMAGE_CORRUPTED;
            break;
        case KErrDiskFull:
        case KErrNoMemory:
            // Image is too large
            resourceId = R_GS_IMAGE_TOO_LARGE;
            break;
        default:
            // Better to give some error message than result in CONE5 panic:
            resourceId = R_GS_IMAGE_CORRUPTED;
            break;
        }
    // Show information note
    HBufC* prompt = iCoeEnv->AllocReadResourceLC( resourceId );
    CAknInformationNote* note = new( ELeave ) CAknInformationNote( ETrue );
    note->ExecuteLD( *prompt );

    // Setting previous VT still image value (rollback)
    iModel->SetVTStillImageL( iPreviousVtStillImage );
    UpdateListBoxL( EGSStillImageItemId, iPreviousVtStillImage );
    CleanupStack::PopAndDestroy( prompt );
    }


// ---------------------------------------------------------------------------
// CGSCallPlugin::VerifySelectionL()
//
// An overloaded method from MMGFetchVerifier interface class
// ---------------------------------------------------------------------------
//
TBool CGSCallPlugin::VerifySelectionL( const MDesCArray* aSelectedFiles )
    {
    const TBool KGSDrmProtectedContent = ETrue;

    TBool ret = ETrue;
    //
    if ( aSelectedFiles->MdcaCount() == 1 )
        {
        const TPtrC fileName( aSelectedFiles->MdcaPoint( 0 ) );

        // First, check if the selected file is DRM protected
        if  ( ret && CheckDRMProtectionL( fileName ) ==
                    KGSDrmProtectedContent )
            {
            // display the note to user
            // Show information note
            HBufC* prompt = iCoeEnv->AllocReadResourceLC(
                                          R_GS_DRM_NOT_ALLOWED );

            CAknInformationNote* note =
                new( ELeave ) CAknInformationNote( ETrue );
            note->ExecuteLD( *prompt );

            CleanupStack::PopAndDestroy( prompt );
            ret = EFalse;
            }

        // Next, check whether the image header is valid
        if  ( ret )
            {
            CImageDecoder* imageDecoder = NULL;
            TRAPD( err, imageDecoder = CImageDecoder::FileNewL(
                  iCoeEnv->FsSession(), fileName, ContentAccess::EPeek ) );

            if  ( err != KErrNone )
                {
                // Show information note
                HBufC* prompt = iCoeEnv->AllocReadResourceLC(
                                                  R_GS_IMAGE_CORRUPTED );
                CAknInformationNote* note =
                    new( ELeave ) CAknInformationNote( ETrue );
                note->ExecuteLD(*prompt);
                CleanupStack::PopAndDestroy( prompt );

                ret = EFalse;
                }
            delete imageDecoder;
            }
        }

    return ret;
    }


// ---------------------------------------------------------------------------
// CGSCallPlugin::CheckDRMProtectionL
//
// Check if the selected image file is DRM protected.
// ---------------------------------------------------------------------------
//
TBool CGSCallPlugin::CheckDRMProtectionL( const TDesC& aOriginalFileName )
    {
    TBool isProtected( EFalse );
    DRMCommon* drmClient = DRMCommon::NewL();
    CleanupStack::PushL( drmClient );
    TInt error = drmClient->Connect();
    if ( error != DRMCommon::EOk )
        {
        User::Leave( KErrCorrupt );
        }

    if ( aOriginalFileName != KNullDesC )
        {
        error = drmClient->IsProtectedFile( aOriginalFileName, isProtected );
        if ( error != DRMCommon::EOk )
            {
            User::Leave( KErrCorrupt );
            }
        }

    CleanupStack::PopAndDestroy( drmClient );
    return isProtected;
    }

// ---------------------------------------------------------------------------
//
// Show Call Duration setting page
//
// ---------------------------------------------------------------------------
//
TBool CGSCallPlugin::ShowCallDurationSettingPageL( TInt& aCurrentValue )
    {
    __GSLOGSTRING("[GSCallPlugin]--> ShowCallDurationSettingPageL");
    CDesCArrayFlat* items = iCoeEnv->ReadDesC16ArrayResourceL(
                            R_GS_SHOW_CALL_DURATION_SETTING_PAGE_LBX );
    CleanupStack::PushL( items );

    CAknRadioButtonSettingPage* dlg = new (ELeave) CAknRadioButtonSettingPage(
                                R_GS_SHOW_CALL_DURATION_SETTING_PAGE,
                                aCurrentValue, items );

    TBool ret = dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged );
    CleanupStack::PopAndDestroy( items );

    __GSLOGSTRING("[GSCallPlugin] <--ShowCallDurationSettingPageL");
    return ret;
    }

// ---------------------------------------------------------------------------
// CGSCallPlugin::ChangeCallDurationSettingL
//
// Change Call Duration setting.
// ---------------------------------------------------------------------------
//
void CGSCallPlugin::ChangeCallDurationSettingL( TBool aSettingPage )
    {
    __GSLOGSTRING("[GSCallPlugin]--> ChangeCallDurationSettingL");

    TInt callState = CheckCallStateL();

    // Get information of whether there are open connections or not
    if ( callState != EPSCTsyCallStateUninitialized &&
         callState != EPSCTsyCallStateNone )
        {
        HBufC* prompt = iCoeEnv->AllocReadResourceLC( R_TEXT_ACTIVE_CALL );

        CAknInformationNote* note =
            new( ELeave ) CAknInformationNote( ETrue );

        note->ExecuteLD( *prompt );

        CleanupStack::PopAndDestroy( prompt );

        return;
        }
        
    TInt duration = iModel->CallDurationL();
    TBool updateValue = ETrue;
    
    if ( aSettingPage )
        {
        updateValue = ShowCallDurationSettingPageL( duration );
        }
    else
        {
        if( duration == KGSSettingItemOff )
            {
            duration = KGSSettingItemOn;
            }
        else
            {
            duration = KGSSettingItemOff;
            }
        }

    if ( updateValue )
        {
        iModel->SetCallDurationL( duration );
        UpdateListBoxL( EGSShowCallDurationItemId, KGSNotUsed );
        }

    __GSLOGSTRING("[GSCallPlugin] <--ChangePrefixSettingL");
    }


// ---------------------------------------------------------------------------
// CGSCallPlugin::CheckCallStateL
// 
// Check call state using PubSub or SA
// ---------------------------------------------------------------------------
//
TInt CGSCallPlugin::CheckCallStateL()
    {
    TInt callState = KErrNone;
    //here are open connections or not
    RProperty::Get( KPSUidCtsyCallInformation,
                    KCTsyCallState,
                    callState );
    return callState;
    }


// ---------------------------------------------------------------------------
// CGSCallPlugin::ShowPreferredTelephonyNoteL
// 
// Shows note if preferred call setting was changed to the Int. Telephone.
// ---------------------------------------------------------------------------
//
void CGSCallPlugin::ShowPreferredTelephonyNoteL()
    {
    // Create message query dialog.
    HBufC* text = StringLoader::LoadLC( R_IC_PREFERRED_TELEPHONE_QUERY_TEXT );
    CAknMessageQueryDialog* query = CAknMessageQueryDialog::NewL( *text );
    query->SetMessageTextL( text->Des() );
    
    // Show message query dialog.
    query->ExecuteLD( R_IC_PREFERRED_TELEPHONE_QUERY );
    
    CleanupStack::PopAndDestroy( text );
    }


// -----------------------------------------------------------------------------
// When this method is called, view checks based on highlight focus, if the MSK
// label is correct.
// -----------------------------------------------------------------------------
//
void CGSCallPlugin::CheckMiddleSoftkeyLabelL()
    {
    const TInt currentFeatureId = Container()->CurrentFeatureId();
    RemoveCommandFromMSK( iMskCommandFlag );
    if ( currentFeatureId == EGSCallWaitingItemId )
        {
            // First remove any prevous commands.
        SetMiddleSoftKeyLabelL( R_CALL_SOFTKEY_OPTION,
                                EAknSoftkeyContextOptions );
        // This way we set which command is set for MSK
        iMskCommandFlag = EFalse;
        }
     else
        {
         //Set middle softkey as Change. 
        SetMiddleSoftKeyLabelL( R_CALL_MSK_CHANGE,
                                EGSMSKCmdAppChange );
        // This way we set which command is set for MSK
        iMskCommandFlag = ETrue;
        }
    }

// -----------------------------------------------------------------------------
// Remove unnecessary commands from Middle softkey.
// @flag = ETrue means presently MSK value is "Change" so we remove that
// @flag = EFalse means presently MSK value is "Context Options" so we remove that
// -----------------------------------------------------------------------------
//
void CGSCallPlugin::RemoveCommandFromMSK(const TBool flag )
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
void CGSCallPlugin::SetMiddleSoftKeyLabelL( 
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
// CGSCallPlugin::HandleClientRectChange
//
//
// -----------------------------------------------------------------------------
//
void CGSCallPlugin::HandleClientRectChange()
    {
    if ( iContainer )
        {
        iContainer->SetRect( ClientRect() );
        }
    }

// ========================= From CGSPluginInterface ==================

// -----------------------------------------------------------------------------
// CGSCallPlugin::GetCaptionL
//
//
// -----------------------------------------------------------------------------
//
void CGSCallPlugin::GetCaptionL( TDes& aCaption ) const
    {
    HBufC* result = StringLoader::LoadL( R_GS_CALL_PLUGIN_CAPTION );
    aCaption.Copy( *result );
    delete result;
    }


// -----------------------------------------------------------------------------
// CGSCallPlugin::PluginProviderCategory
//
//
// -----------------------------------------------------------------------------
//
TInt CGSCallPlugin::PluginProviderCategory() const
    {
    //This plugin is created by 3rd party.
    return KGSPluginProviderInternal;
    }


// -----------------------------------------------------------------------------
// CGSCallPlugin::CreateIconL()
//
//
// -----------------------------------------------------------------------------
//
CGulIcon* CGSCallPlugin::CreateIconL( const TUid aIconType )
    {
    //EMbm<Mbm_file_name><Bitmap_name>
    CGulIcon* icon;
    TParse* fp = new( ELeave ) TParse();
    CleanupStack::PushL( fp );
    fp->Set( KGSCallPluginIconDirAndName, &KDC_BITMAP_DIR, NULL );

    if( aIconType == KGSIconTypeLbxItem )
        {
        icon = AknsUtils::CreateGulIconL(
        AknsUtils::SkinInstance(),
        KAknsIIDQgnPropSetCallSub,
        fp->FullName(),
        EMbmGscallpluginQgn_prop_set_call_sub,
        EMbmGscallpluginQgn_prop_set_call_sub_mask );
        }
    else
        {
        icon = CGSPluginInterface::CreateIconL( aIconType );
        }

    CleanupStack::PopAndDestroy( fp );

    return icon;
    }


// ---------------------------------------------------------------------------
// CGSCallPlugin::ChangeLongPressCallKeySettingL
//
// Change Long Press Call Key setting.
// values:  0,1  --> Inactive (Default Value is 1 and when RFS default value is 0) 
// 			2 --> Video call
// ---------------------------------------------------------------------------
//
void CGSCallPlugin::ChangeLongPressCallKeySettingL( const TBool aShowSettingPage )
    {
    __GSLOGSTRING("[GSCallPlugin]--> ChangeLongPressCallKeySettingL");
    TInt statusLongPressCallKey = iModel->LongPressCallKeyL();
    TBool updateValue = ETrue;
    
    if ( aShowSettingPage )
        {
       	// If the status of Long Key is not active then we decrement to
       	// the value of statusLongPressCallKey so that the values are
       	// then tuned to launch the setting page normally with two setting
       	// items InActive and Video Call
       	// Refer to GSCallPlugin.hrh file for more info for the  description
       	iModel->MapLongPressKeyCallKeyValue( statusLongPressCallKey );
       	updateValue = ShowLongPressCallKeySettingPageL( 
        						       statusLongPressCallKey );
        
        // Here if the selected index is 1 it means Video Call from the setting page
        // we then again map this value back to 2 which is EGSLongPressCallKeyVideoCall
        // Refer to GSCallPlugin.hrh for more details					       
  		if ( statusLongPressCallKey == EGSLongPressCallKeyVoiceCall )
  			{
  			statusLongPressCallKey = EGSLongPressCallKeyVideoCall;
  			}
  		else // If selected index in setting page is InActive ( 0) we map
  		     // this to EGSLongPressCallKeyVoiceCall which is 1
  			{
  			statusLongPressCallKey = EGSLongPressCallKeyVoiceCall;
  			}
        }
	else // switch the value
		{
   		if ( statusLongPressCallKey == EGSLongPressCallKeyNotSet ||
   			 statusLongPressCallKey == EGSLongPressCallKeyVoiceCall )
			{
        	statusLongPressCallKey = EGSLongPressCallKeyVideoCall;
        	}
   		else
    		{
        	statusLongPressCallKey = EGSLongPressCallKeyVoiceCall;
        	}
        }
    if ( updateValue )
        {
        iModel->SetLongPressCallKeyL( statusLongPressCallKey );
        UpdateListBoxL( EGSLongPressCallKeyItemId, KGSNotUsed );
        }

    __GSLOGSTRING("[GSCallPlugin] <--ChangeLongPressCallKeySettingL");
    }

// ---------------------------------------------------------------------------
//
// 		Launching Long Press Key setting page
//
// ---------------------------------------------------------------------------
//
TBool CGSCallPlugin::ShowLongPressCallKeySettingPageL( TInt& aCurrentValue )
    {
    __GSLOGSTRING("[GSCallPlugin]--> ShowLongPressCallKeySettingPageL");
    CDesCArrayFlat* items = iCoeEnv->ReadDesC16ArrayResourceL(
                            R_GS_LONG_PRESS_CALL_SETTING_PAGE_LBX );
    CleanupStack::PushL( items );

    CAknRadioButtonSettingPage* dlg = new (ELeave) CAknRadioButtonSettingPage(
                                R_GS_LONG_PRESS_CALL_SETTING_PAGE,
                                aCurrentValue, items );

    TBool ret = dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged );
    CleanupStack::PopAndDestroy( items );

    __GSLOGSTRING("[GSCallPlugin] <--ShowLongPressCallKeySettingPageL");
    return ret;
    }


// ----------------------------------------------------------------------------
// CGSCallPlugin::ShowOwnImageVtCallSettingPageL
// 
// Displaying settings page for Own Image during video call setting
// ----------------------------------------------------------------------------
//
void CGSCallPlugin::ShowOwnImageVtCallSettingPageL()
    {
    __GSLOGSTRING("[GSCallPlugin]--> ShowOwnImageVtCallSettingPageL");
    TInt currentValue = iModel->OwnImageVtCallStatusL();
    CDesCArrayFlat* items = iCoeEnv->ReadDesC16ArrayResourceL(
                            R_GS_OWN_IMAGE_VT_CALL_SETTING_PAGE_LBX );
    CleanupStack::PushL( items );

    CAknRadioButtonSettingPage* dlg = new (ELeave) CAknRadioButtonSettingPage(
                                R_GS_OWN_IMAGE_VT_CALL_SETTING_PAGE,
                                currentValue, items );

    TBool ret = dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged );
    
    if( ret )
        {
        iModel->SetOwnImageVtCallStatusL( currentValue );
        UpdateListBoxL( EGSOwnImageVtCallItemId, currentValue );
        }
    
    CleanupStack::PopAndDestroy( items );

    __GSLOGSTRING("[GSCallPlugin] <--ShowOwnImageVtCallSettingPageL");
    }

//End of File
