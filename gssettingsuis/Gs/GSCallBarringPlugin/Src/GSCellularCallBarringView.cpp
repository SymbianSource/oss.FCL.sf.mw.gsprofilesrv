/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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
#include "GSCellularCallBarringView.h"  //for CGSSettListBarringView
#include "GSCellularCallBarringContainer.h"//for CGSBaseContainer
#include "GSPhoneSettingConstants.h"  //for constant values
#include "GSCallBarringPlugin.h"            //for plugin UID
#include "GsLogger.h"
#include "LocalViewIds.h"           //for KTellBarringViewId

#include <gscommon.hrh>
#include <coeaui.h>
#include <hlplch.h>             // For HlpLauncher
#include <e32base.h>
#include <featmgr.h>
#include <GSCallBarringPluginRsc.rsg>      //for resource IDs
#include <exterror.h>               //for extended error codes
#include <StringLoader.h>           //for StringLoader
#include <PsetContainer.h>          //for CPsetContainer
#include <PsuiContainer.h>          //for CPsuiContainer
#include <eikmenup.h>               //for menu bar
#include <SecUiCodeQueryDialog.h>
#include <nwdefs.h>
#include <aknViewAppUi.h>
#include <gsfwviewuids.h>
#if defined(__VOIP) && defined(RD_VOIP_REL_2_2)
#include <spsettings.h>
#endif // __VOIP && RD_VOIP_REL_2_2
 
// ========================= MEMBER FUNCTIONS ================================
// ---------------------------------------------------------------------------
// 
// C++ constructor.
// 
// ---------------------------------------------------------------------------
CGSSettListBarringView::CGSSettListBarringView()
    {
    }

// ---------------------------------------------------------------------------
// 
// Symbian OS two-phased constructor (second phase)
// 
// ---------------------------------------------------------------------------
void CGSSettListBarringView::ConstructL()
    {
    __GSLOGSTRING("[GS]--> CGSSettListBarringView::ConstructL");
    BaseConstructL( R_GS_BARRING_VIEW );

    iSettings = CPsetContainer::NewL();
    iObsContainer = CPsuiContainer::NewL();
    iCbObserver = iObsContainer->CreateCBObsL();
    iBarring = iSettings->CreateCBObjectL( *iCbObserver );
    iRockerPress = EFalse;
    iContextMenuInitialized = EFalse;
    __GSLOGSTRING("[GS] <--CGSSettListBarringView::ConstructL");
    }


// ---------------------------------------------------------------------------
// CGSSettListBarringView::NewL()
// Symbian OS two-phased constructor
// 
// ---------------------------------------------------------------------------
CGSSettListBarringView* CGSSettListBarringView::NewL()
    {
    CGSSettListBarringView* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// 
// Symbian OS two-phased constructor (first phase)
// 
// ---------------------------------------------------------------------------
CGSSettListBarringView* CGSSettListBarringView::NewLC()
    {
    __GSLOGSTRING("[GS]--> CGSSettListBarringView::NewLC");
    CGSSettListBarringView* self = new ( ELeave ) CGSSettListBarringView;
    CleanupStack::PushL( self );
    self->ConstructL();

    __GSLOGSTRING("[GS] <--CGSSettListBarringView::NewLC");
    return self;
    }
 
// ---------------------------------------------------------------------------
// 
// C++ destructor
// 
// ---------------------------------------------------------------------------
CGSSettListBarringView::~CGSSettListBarringView()
    {
    delete iBarring;
    delete iSettings;
    delete iObsContainer;
    delete iCbObserver;
    iRockerPress = EFalse;

    }
 
// ---------------------------------------------------------------------------
// 
// Returns Id of the Barring view.
// 
// ---------------------------------------------------------------------------
TUid CGSSettListBarringView::Id() const
    {
    return KCallBarringViewId;
    }
 
// ---------------------------------------------------------------------------
// 
// Handles other than list box events.
// 
// ---------------------------------------------------------------------------
void CGSSettListBarringView::HandleCommandL( TInt aCommand )
    {
    __GSLOGSTRING("[GS]--> CGSSettListBarringView::HandleCommandL");
    iRockerPress = EFalse;

    TCallBarringSetting barringSetting;
    iCurrentItem = iContainer->iListBox->CurrentItemIndex();
    SetBarringType( 
        aCommand, 
        barringSetting, 
        static_cast <TGSBarringSetting> (iCurrentItem) );
#if defined(__VOIP) && defined(RD_VOIP_REL_2_2)
    CSPSettings* spSettings = CSPSettings::NewL();
    TBool showVoipFeatures( EFalse );
    if ( spSettings->IsFeatureSupported( ESupportVoIPFeature ) &&
        spSettings->IsFeatureSupported( ESupportVoIPSSFeature ) )
        {
        showVoipFeatures = ETrue;
        }
#endif // __VOIP && RD_VOIP_REL_2_2
    
    switch ( aCommand )
        {
        case EGSCmdAppChange:
            iRockerPress = EFalse;
            HandleListBoxEventL( iContainer->iListBox, EEventEnterKeyPressed );
            break;
        case EAknSoftkeyBack:
#if defined(__VOIP) && defined(RD_VOIP_REL_2_2)
            if ( showVoipFeatures )
                {
                iAppUi->ActivateLocalViewL( KCallMainBarringViewId );
                }
            else
                {
                iAppUi->ActivateLocalViewL( KGSTelPluginUid );
                }
#else // __VOIP && RD_VOIP_REL_2_2
            if ( FeatureManager::FeatureSupported( KFeatureIdCommonVoip ) )
                {
                iAppUi->ActivateLocalViewL( KCallMainBarringViewId );
                }
            else
                {
                iAppUi->ActivateLocalViewL( KGSTelPluginUid );
                }
#endif // __VOIP && RD_VOIP_REL_2_2
            break;
        case EGSCmdBarringActivate:
            barringSetting.iSetting = EActivateBarring;
            HandleBarringOperationL( barringSetting, EAllTeleAndBearer );
            break;
        case EGSCmdBarringCancel:
            barringSetting.iSetting = ECancelBarring;
            HandleBarringOperationL( barringSetting, EAllTeleAndBearer );
            break;
        case EGSCmdBarringInquiry:
            iBarring->GetBarringStatusL( EServiceGroupVoice, 
                barringSetting.iType );             
            break;
        case EGSCmdBarringChangePwd:
            ChangeBarringPwdL();            
            break;
        case EGSCmdBarringCancelAll:
            CancelAllBarringsL();            
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
    __GSLOGSTRING("[GS] <--CGSSettListBarringView::HandleCommandL");
    }
  
// ---------------------------------------------------------------------------
// 
// Activates view.
// 
// ---------------------------------------------------------------------------
void CGSSettListBarringView::DoActivateL( const TVwsViewId& aPrevViewId,
                                          TUid aCustomMessageId,
                                          const TDesC8& aCustomMessage )
    {
    __GSLOGSTRING("[GS]--> CGSSettListBarringView::DoActivateL");
    CGSLocalBaseView::DoActivateL( aPrevViewId, aCustomMessageId, 
                                   aCustomMessage );

    iContainer->iListBox->SetTopItemIndex(iTopItemIndex);

    if (iCurrentItem >= 0 && 
        iCurrentItem < iContainer->iListBox->Model()->NumberOfItems())
        {
        iContainer->iListBox->SetCurrentItemIndexAndDraw(iCurrentItem);
        }
    __GSLOGSTRING("[GS] <--CGSSettListBarringView::DoActivateL");
    }
 
// ---------------------------------------------------------------------------
// 
// Deactivates view.
// 
// ---------------------------------------------------------------------------
void CGSSettListBarringView::DoDeactivate()
    {
    __GSLOGSTRING("[GS]--> CGSSettListBarringView::DoDeactivate");
    if ( iContainer )
        {
        iCurrentItem = iContainer->iListBox->CurrentItemIndex();
        iTopItemIndex = iContainer->iListBox->TopItemIndex();
 
        iAppUi->RemoveFromViewStack( *this, iContainer );
        delete iContainer;
        iContainer = NULL;
        }
    __GSLOGSTRING("[GS] <--CGSSettListBarringView::DoDeactivate");
    }
 
 
 // -----------------------------------------------------------------------------
// CGSSettListBarringView::ProcessCommandL
// -----------------------------------------------------------------------------
void CGSSettListBarringView::ProcessCommandL( TInt aCommand )
    {
    // Context menu is needed to be initialized only if selection key
    // is pressed. Initialization need to be performed only once.
    if( aCommand == EAknSoftkeyContextOptions &&
            !iContextMenuInitialized )
            {
            MenuBar()->SetContextMenuTitleResourceId(
                    R_CALL_BARRING_MSK_CONTEXTMENUBAR );
            iContextMenuInitialized = ETrue;
            }
    
    // Call base class
    CAknView::ProcessCommandL( aCommand );
    }
// ---------------------------------------------------------------------------
// 
// Before showing a options menu, delete options key specific settings if 
// rocker key has been pressed.
// ---------------------------------------------------------------------------
//
void CGSSettListBarringView::DynInitMenuPaneL( TInt aResourceId, 
                                              CEikMenuPane* aMenuPane )
    {
    __GSLOGSTRING("[GS]--> CGSSettListBarringView::DynInitMenuPaneL");
    CEikMenuBar* menuBar = MenuBar();
    // show or hide the 'help' menu item when supported
    if( aResourceId == R_GS_MENU_ITEM_HELP )
        {
        if ( FeatureManager::FeatureSupported( KFeatureIdHelp ) )
            {
            aMenuPane->SetItemDimmed( EAknCmdHelp, EFalse );
            }
        else
            {
            aMenuPane->SetItemDimmed( EAknCmdHelp, ETrue );
            }
        }
        
    if ( iRockerPress )
        {
        // always hide 'help' & 'exit' menu items when rocker key is pressed
        if( aResourceId == R_GS_MENU_ITEM_EXIT )
            {
            aMenuPane->SetItemDimmed( EAknCmdExit, ETrue );
            }
        else if ( aResourceId == R_GS_MENU_ITEM_HELP )
            {
            aMenuPane->SetItemDimmed( EAknCmdHelp, ETrue );
            }
        else if ( aResourceId == R_GS_BARRING_VIEW_MENU )
            {
            // these options are not initialized in restricted call barring
            if ( !FeatureManager::FeatureSupported( KFeatureIdRestrictedCallBarring ) )
                {
                aMenuPane->SetItemDimmed( EGSCmdBarringCancelAll, ETrue );
                aMenuPane->SetItemDimmed( EGSCmdBarringChangePwd, ETrue );
                }
            }
        }
	else if (aResourceId == R_GS_BARRING_VIEW_MENU)
		{
		if (!menuBar->ItemSpecificCommandsEnabled() && menuBar->GetMenuType()
				== CEikMenuBar::EMenuOptions)
			{
			aMenuPane->SetItemDimmed(EGSCmdBarringActivate, ETrue);
			aMenuPane->SetItemDimmed(EGSCmdBarringCancel, ETrue);
			aMenuPane->SetItemDimmed(EGSCmdBarringInquiry, ETrue);
			}
		}
    __GSLOGSTRING("[GS] <--CGSSettListBarringView::DynInitMenuPaneL");
    }

// ---------------------------------------------------------------------------
// 
// Queries password from the user.
// 
// ---------------------------------------------------------------------------
//
TInt CGSSettListBarringView::BarringPasswordQueryL( 
    TBarringPassword& aBarringPassword,
    const TInt& aTitleID )
    {
    __GSLOGSTRING("[GS]--> CGSSettListBarringView::BarringPasswordQueryL");
    TBool continueLoop = ETrue;

    while ( continueLoop )
        {
        aBarringPassword = KNullDesC;
        CCodeQueryDialog* dlg = new (ELeave)
            CCodeQueryDialog( aBarringPassword, KGSBarringPasswordLength, 
            KGSBarringPasswordLength, ESecUiNone );
        dlg->PrepareLC( R_BARRING_PASSWORD_DLG );

        HBufC* string = NULL;
        string = StringLoader::LoadLC( aTitleID );        
        dlg->SetPromptL( *string );
        CleanupStack::PopAndDestroy( string );        

        TInt res = dlg->RunLD();
        dlg = NULL;

        if ( res )
            {
            if ( aBarringPassword.Length() == KGSBarringPasswordLength )
                {
                continueLoop = EFalse;
                }
            }
        else
            {
            __GSLOGSTRING("[GS]    BarringPasswordQueryL: EAknSoftkeyCancel returned");
            __GSLOGSTRING("[GS] <--CGSSettListBarringView::BarringPasswordQueryL");
            return EAknSoftkeyCancel;
            }        
    }
    __GSLOGSTRING("[GS]    BarringPasswordQueryL: EAknSoftkeyOk returned");
    __GSLOGSTRING("[GS] <--CGSSettListBarringView::BarringPasswordQueryL");
    return EAknSoftkeyOk;
 }
 
// ---------------------------------------------------------------------------
// 
// Adds given item to a given list
// 
// ---------------------------------------------------------------------------
void CGSSettListBarringView::AppendItemL( 
    CDesCArrayFlat& aList, const TInt aItem )
    {
    HBufC* string = NULL;
    string = StringLoader::LoadLC( aItem );
    aList.AppendL( *string );
    CleanupStack::PopAndDestroy( string );
    }

// ---------------------------------------------------------------------------
// 
// Handles barring operation requests.
// 
// ---------------------------------------------------------------------------
void CGSSettListBarringView::HandleBarringOperationL( 
    TCallBarringSetting aBarringSetting, TBasicServiceGroups aBsc )
    {
    __GSLOGSTRING("[GS]--> CGSSettListBarringView::HandleBarringOperationL");
    TInt ret = KErrNone;
    ret = BarringPasswordQueryL( 
        aBarringSetting.iPassword, R_BARRING_PASSWORD );
    if ( ret == EAknSoftkeyOk )
         {
         iBarring->SetBarringL( aBarringSetting, aBsc );
         }
    __GSLOGSTRING("[GS] <--CGSSettListBarringView::HandleBarringOperationL");
    }

// ---------------------------------------------------------------------------
// 
// Sets barring type. Switches from GS internal values, to values
// shared by phone/phonesettings.
// ---------------------------------------------------------------------------
void CGSSettListBarringView::SetBarringType( TInt& aCommand,
    TCallBarringSetting& aBarringSetting, 
    TGSBarringSetting aType )
    {
    __GSLOGSTRING("[GS]--> CGSSettListBarringView::SetBarringType");
    if ( aCommand != EGSCmdBarringActivate &&
         aCommand != EGSCmdBarringCancel &&
         aCommand != EGSCmdBarringInquiry &&
         aCommand != EGSCmdBarringChangePwd &&
         aCommand != EGSCmdBarringCancelAll )
        {
        __GSLOGSTRING("[GS]    SetBarringType: No barring variables set");
        return; //in case of exit do not set barring variables.
        }

    aBarringSetting.iServiceGroup = EServiceGroupVoice;

    switch ( aType )
        {
        case EGSCBOutgoing:
            aBarringSetting.iType = EBarringTypeAllOutgoing;
            break;    
        case EGSCBInternational:
            aBarringSetting.iType = EBarringTypeOutgoingInternational;
            break;
        case EGSCBInternationalExceptHome:
            aBarringSetting.iType = 
                EBarringTypeOutgoingInternationalExceptToHomeCountry;
            break;
        case EGSCBIncoming:
            aBarringSetting.iType = EBarringTypeAllIncoming;
            break;
        case EGSCBAbroad:
            aBarringSetting.iType = EBarringTypeIncomingWhenRoaming;
            break;
        default:
            aBarringSetting.iType = EBarringTypeAllBarrings;
            break;
        }
        
    if ( FeatureManager::FeatureSupported( KFeatureIdRestrictedCallBarring ) )
        {
        // Restricted call barring only support EBarringTypeIncomingWhenRoaming
        aBarringSetting.iType = EBarringTypeIncomingWhenRoaming;
        }    

    __GSLOGSTRING("[GS] <--CGSSettListBarringView::SetBarringType");
    }

// ---------------------------------------------------------------------------
// 
// Creates a request to cancel all barrings.
// 
// ---------------------------------------------------------------------------
void CGSSettListBarringView::CancelAllBarringsL()
    {
    __GSLOGSTRING("[GS]--> CGSSettListBarringView::CancelAllBarringsL");
    TCallBarringSetting cancelAllCB;       

    TInt retValue = KErrNone; 
    retValue = BarringPasswordQueryL( cancelAllCB.iPassword, 
        R_BARRING_PASSWORD );
    if ( retValue == EAknSoftkeyOk )
        { 
        cancelAllCB.iSetting = ECancelBarring;
        cancelAllCB.iType = EBarringTypeAllBarrings;
        cancelAllCB.iServiceGroup = EServiceGroupVoice;
        iBarring->SetBarringL( cancelAllCB, EAllTeleAndBearer );
        }
    __GSLOGSTRING("[GS] <--CGSSettListBarringView::CancelAllBarringsL");
    }

// ---------------------------------------------------------------------------
// 
// Handles changing of barring password.
// 
// ---------------------------------------------------------------------------
void CGSSettListBarringView::ChangeBarringPwdL()
    {
    __GSLOGSTRING("[GS]--> CGSSettListBarringView::ChangeBarringPwdL");
    TInt returnValue = KErrNone;
    RMobilePhone::TMobilePhonePasswordChangeV2 pwd;

    returnValue = BarringPasswordQueryL( 
        pwd.iOldPassword, R_CURRENT_BARRING_PASSWORD );

    if ( returnValue != EAknSoftkeyOk ) //cancelled or error
        {
        __GSLOGSTRING("[GS]    ChangeBarringPwdL: Current PW cancelled/error");
        return;
        }
    returnValue = BarringPasswordQueryL( pwd.iNewPassword, 
        R_NEW_BARRING_PASSWORD );
    if ( returnValue != EAknSoftkeyOk ) //cancelled or error
        {
        __GSLOGSTRING("[GS]    ChangeBarringPwdL: New PW cancelled/error");
        return;
        }
    returnValue = BarringPasswordQueryL( pwd.iVerifiedPassword , 
                                         R_VERIFY_NEW_BARRING_PASSWORD );
    if ( returnValue != EAknSoftkeyOk )
        {
        __GSLOGSTRING("[GS]    ChangeBarringPwdL: Verify PW1 cancelled/error");
        return;
        }            
    // check if newpw and verifypw do not 
    // match->password error->return to query            
    while ( pwd.iNewPassword.Compare( pwd.iVerifiedPassword ) != 0 )
        {
        iCbObserver->HandleBarringErrorL( KErrGsmSSNegativePasswordCheck );

        returnValue = BarringPasswordQueryL( pwd.iVerifiedPassword, 
                                             R_VERIFY_NEW_BARRING_PASSWORD );
        if ( returnValue != EAknSoftkeyOk ) //cancelled or error
            {
            __GSLOGSTRING("[GS]    ChangeBarringPwdL: Verify PW2 cancelled/error");
            return;
            }
        }
    iBarring->ChangePasswordL( pwd, ETrue );
    __GSLOGSTRING("[GS] <--CGSSettListBarringView::ChangeBarringPwdL");
    }

// ---------------------------------------------------------------------------
// 
// Creates new iContainer.
// 
// ---------------------------------------------------------------------------
void CGSSettListBarringView::NewContainerL()
    {
    iContainer = new (ELeave) CGSSettListBarringContainer;
    }

// ---------------------------------------------------------------------------
// 
// Handles listbox selection
// 
// ---------------------------------------------------------------------------
void CGSSettListBarringView::HandleListBoxSelectionL()
    {
    iRockerPress = ETrue;
    CEikMenuBar* menuBar = this->MenuBar();
    menuBar->SetMenuType( CEikMenuBar::EMenuContext );
    menuBar->SetContextMenuTitleResourceId(R_CALL_BARRING_MSK_CONTEXTMENUBAR);
    // Display menu bar.
    menuBar->TryDisplayContextMenuBarL();
    iRockerPress = EFalse;
    menuBar->SetMenuType( CEikMenuBar::EMenuOptions );
    iContextMenuInitialized = ETrue;
    }


 // End of file
