/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies).
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
#include "GSCallDivertContainer.h"
#include "GSCallDivertModel.h"
#include "GSCallDivertQuery.h"
#include "GSCallDivertView.h"
#include "GsLogger.h"
#include "GSPhoneSettingConstants.h"
#include "GSRestrictedDivertContainer.h"
#include "LocalViewId.h"


#include <AiwContactSelectionDataTypes.h>
#include <AiwGenericParam.h>
#include <aknnotewrappers.h>        //for AknErrorNote
#include <AknUtils.h>
#include <aknViewAppUi.h>
#include <CPbk2SortOrderManager.h>
#include <CPbk2StoreConfiguration.h>
#include <CPbkContactItem.h>
#include <CPbkSingleItemFetchDlg.h>
#include <CVPbkContactLinkArray.h>
#include <CVPbkContactManager.h>
#include <featmgr.h>
#include <gscommon.hrh>
#include <gsfwviewuids.h>          // for restricted call forward
#include <gscalldivertpluginrsc.rsg>
#include <MPbk2ContactNameFormatter.h>
#include <MVPbkContactFieldData.h>
#include <MVPbkContactFieldTextData.h>
#include <MVPbkContactLink.h>
#include <MVPbkContactOperationBase.h>
#include <MVPbkContactStoreList.h>
#include <MVPbkStoreContact.h>
#include <NumberGrouping.h>         // for Number Grouping
#include <Pbk2ContactNameFormatterFactory.h>
#include <PsetCallDiverting.h>
#include <PsuiConstants.h>
#include <RPbkViewResourceFile.h>
#include <AiwServiceHandler.h>
#include <coeaui.h>
#include <CVPbkContactStoreUriArray.h>
#include <CVPbkFieldTypeSelector.h>
#include <hlplch.h>                 // for Help Launcher
#include <hlplch.h>             // For HlpLauncher
#include <RVPbkContactFieldDefaultPriorities.h>
#include <StringLoader.h>
#include <VPbkContactViewFilterBuilder.h>
#include <voicemailboxdomaincrkeys.h>
#include <centralrepository.h>

using namespace AiwContactAssign;

// LOCAL CONSTANTS
_LIT( KGSDivertClassName, "CGSCallDivertView" );

_LIT( KCallDivertInvalidchars, " ()" );

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSCallDivertView::CGSCallDivertView
//
// C++ constructor.
// ---------------------------------------------------------------------------
CGSCallDivertView::CGSCallDivertView( CGSCallDivertModel* aModel )
    : iModel( aModel )
    {
    }

// ---------------------------------------------------------------------------
// CGSCallDivertView::ConstructL
//
// Symbian OS two-phased constructor.
// ---------------------------------------------------------------------------
void CGSCallDivertView::ConstructL()
    {
    if( FeatureManager::FeatureSupported( KFeatureIdRestrictedCallDivert ) )
            {
            iModel->SetDivertServiceGroup( EGSSettIdVoiceDivert );
            }
    iModel->SetDivertView( this );

    iStoresOpened = EFalse;
    iNumberFetchedAsynchronously = EFalse;

    BaseConstructL( R_CF_DIV_VIEW );

    iTitles = iCoeEnv->ReadDesCArrayResourceL( R_TITLE_ARRAY );
    iRockerPress = EFalse;

    //Check video mailbox suppport from cenrep
    //Since it is static feature, needs to be checked only once
    CRepository* videoMailboxRepository = 
                        CRepository::NewLC( KCRUidVideoMailbox );
    videoMailboxRepository->Get( KVideoMbxSupport, iVideoMailboxSupported );
    CleanupStack::PopAndDestroy( videoMailboxRepository );

    // AIW for pbk2 contact selection functionality. Construct contact manager
    // with the same store configuration as in Phonebook currently.
    iServiceHandler = CAiwServiceHandler::NewL();
    iServiceHandler->AttachL( R_CALL_DIVERT_CONTACT_SELECTION_INTEREST );
    CPbk2StoreConfiguration* configuration = CPbk2StoreConfiguration::NewL();
    CleanupStack::PushL( configuration );
    CVPbkContactStoreUriArray* uriArray = configuration->CurrentConfigurationL();
    CleanupStack::PushL( uriArray );
    iContactManager = CVPbkContactManager::NewL( *uriArray );
    iContactManager->ContactStoresL().OpenAllL( *this );
    CleanupStack::PopAndDestroy( uriArray );
    CleanupStack::PopAndDestroy( configuration );
    }

// ---------------------------------------------------------------------------
// CGSCallDivertView::~CGSCallDivertView
//
// Destructor.
// ---------------------------------------------------------------------------
CGSCallDivertView::~CGSCallDivertView()
    {
    if ( iTitles )
        {
        delete iTitles;
        }
    if( iRetrieveOperation )
        {
        delete iRetrieveOperation;
        iRetrieveOperation = NULL;
        }
    if( iContactLinks )
        {
        delete iContactLinks;
        iContactLinks = NULL;
        }
    
    TRAP_IGNORE
        (
        if( iContactManager )
            {
            iContactManager->ContactStoresL().CloseAll( *this );
            iStoresOpened = EFalse;
            delete iContactManager;
            }
        );
    delete iServiceHandler;


    if( iContextMenuItems )
    	{
    	delete iContextMenuItems;
    	iContextMenuItems = NULL;
    	}
    }

// ---------------------------------------------------------------------------
// CGSCallDivertView::NewLC
//
// 1st phase constructor.
// ---------------------------------------------------------------------------
CGSCallDivertView* CGSCallDivertView::NewLC( CGSCallDivertModel* aModel )
    {
    CGSCallDivertView* self;
    self = new ( ELeave ) CGSCallDivertView( aModel );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CGSCallDivertView::NewContainerL
//
// Creates new iContainer.
// ---------------------------------------------------------------------------
void CGSCallDivertView::NewContainerL()
    {
    if( FeatureManager::FeatureSupported( KFeatureIdRestrictedCallDivert ) )
        {
        iContainer = new (ELeave) CGSRestrictedDivertContainer();
        }
    else
        {
        iContainer = new (ELeave) CGSCallDivertContainer(
                     iModel->DivertServiceGroup() );
        }
    }


// ---------------------------------------------------------------------------
// CGSCallDivertView::Id
//
// implementation of the frameworks virtual function, closely related with view
// architecture and has little or no relevance here
// ---------------------------------------------------------------------------
TUid CGSCallDivertView::Id() const
    {
    if( FeatureManager::FeatureSupported( KFeatureIdRestrictedCallDivert ) )
        {
        return KCallDivertPluginId;
        }
    else
        {
        return KCallDivertViewId;
        }
    }

// ---------------------------------------------------------------------------
// CGSCallDivertView::HandleCommandL
//
// Handles the received commands
// ---------------------------------------------------------------------------
void CGSCallDivertView::HandleCommandL( TInt aCommand )
    {
    iVoiceMailboxDivert = EFalse;
    TBool sendRequest = EFalse;
    RMobilePhone::TMobilePhoneServiceAction info =
        RMobilePhone::EServiceActionRegister;
    TCallDivertSetting divertRequest;

    if ( aCommand != ECFCmdCancelAll && iContainer )
        {
        if( FeatureManager::FeatureSupported( KFeatureIdRestrictedCallDivert ) )
            {
            CGSRestrictedDivertContainer& container =
                *static_cast<CGSRestrictedDivertContainer*>( iContainer );
            iCurrentItem = container.CurrentFeatureId();
            }
        else
            {
            CGSCallDivertContainer& container =
                *static_cast<CGSCallDivertContainer*>( iContainer );
            iCurrentItem = container.CurrentFeatureId();
            }
        __GSLOGSTRING1("[CGSCallDivertView::HandleCommandL] iCurrentItem: %d", iCurrentItem);

        __ASSERT_ALWAYS( iCurrentItem != KErrNotFound, Panic( KGSDivertClassName,
                                                              EInvalidCurrentItem ) );
        }

    switch ( aCommand )
        {
        case EGSCmdAppChange:
            iRockerPress = EFalse;
            if ( iContainer )
            	{
            	HandleListBoxEventL( iContainer->iListBox, EEventEnterKeyPressed );
            	}            
            break;
        case EAknSoftkeyBack:
            if( FeatureManager::FeatureSupported( KFeatureIdRestrictedCallDivert ) )
                {
                iAppUi->ActivateLocalViewL( KGSTelPluginUid );
                }
            else
                {
                iAppUi->ActivateLocalViewL( KCallDivertPluginId );
                }
            break;
        case EAknSoftkeyExit:
            AppUi()->HandleCommandL( EEikCmdExit );
            break;
        case ECFCmdActivate:
            divertRequest.iSetting = EActivateDiverting;
            info = RMobilePhone::EServiceActionRegister;

            // Activate depends on the selected call type (voice/data/fax)
            switch( iModel->DivertServiceGroup() )
                {
                case EGSSettIdVoiceDivert:
                    // Handle voice divert. Fetch used numbers and show it in query.
                    ExecuteDivertQueryL(
                        divertRequest.iNumber,
                        iTitles->MdcaPoint( iCurrentItem ),
                        iVoiceMailboxDivert );
                    sendRequest = ETrue;
                    break;
                case EGSSettIdVideoDivert:
                    if( iVideoMailboxSupported )
                        {
                        // Fetch used data divert number and show it in query.
                        ExecuteDivertQueryL(
                                divertRequest.iNumber,
                                iTitles->MdcaPoint( iCurrentItem ),
                                iVoiceMailboxDivert );
                        sendRequest = ETrue;
                        }
                    else
                        {
                        DisplayDataCallDivertNumberQueryL( divertRequest.iNumber );
                        sendRequest = ETrue;
                        }
                    break;
                default:
                    User::Panic( KGSDivertClassName, KErrArgument );
                    break;
                }
            break;
        case ECFCmdCancel:
            if( FeatureManager::FeatureSupported( KFeatureIdRestrictedCallDivert ) )
                {
                sendRequest = EFalse;
                iModel->CancelRestrictedCallDivertL();
                }
            else
                {
                sendRequest = ETrue;
                divertRequest.iSetting = ECancelDiverting;
                info = RMobilePhone::EServiceActionErase;
                }
            break;
        case ECFCmdCheckStatus:
            if( FeatureManager::FeatureSupported( KFeatureIdRestrictedCallDivert ) )
                {
                sendRequest = EFalse;
                iModel->CheckStatusRestrictedCallDivertL();
                }
            else
                {
                sendRequest = ETrue;
                divertRequest.iSetting = ECheckStatus;
                info = RMobilePhone::EServiceActionUnspecified;
                }
            break;
        case ECFCmdCancelAll:
            sendRequest = ETrue;
            divertRequest.iSetting = EEraseDiverting;
            info = RMobilePhone::EServiceActionErase;
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

    // If a request needs to be sent.
    if ( sendRequest )
        {
        // will not divert in case empty number
        iModel->SendCallDivertRequestL( divertRequest,
                                        info,
                                        iVoiceMailboxDivert,
                                        aCommand,
                                        (TGSCallDivertServices) iCurrentItem );
        }
    }


// ---------------------------------------------------------------------------
// CGSCallDivertView::HandleListBoxSelectionL
//
// Handles List box selection
// ---------------------------------------------------------------------------
void CGSCallDivertView::HandleListBoxSelectionL()
    {
    iRockerPress = ETrue;
    CEikMenuBar* menuBar = MenuBar();
    if ( iRockerPress )
        {
        menuBar->SetMenuTitleResourceId( R_CF_MENUBAR_OPTIONS_VIEW );
        menuBar->SetMenuType( CEikMenuBar::EMenuContext );
        }
    else
        {
        menuBar->SetMenuTitleResourceId( R_CF_MENUBAR_DIV_VIEW );
        menuBar->SetMenuType( CEikMenuBar::EMenuOptions );
        }

    // Display menu bar.
    menuBar->TryDisplayMenuBarL();

    // Reset menu type to original default (EMenuOptions).
    menuBar->SetMenuType( CEikMenuBar::EMenuOptions );

    iRockerPress = EFalse;
    }


// ---------------------------------------------------------------------------
// CGSCallDivertView::ExecuteDivertQueryL
//
// --> voice calls -> all voice calls -> activate -> this popup.
//                          OR
// --> video calls -> all video calls -> activate -> this popup.
//
// ---Example---
// All [voice/]video calls:
// To [voice/]video mailbox
// [050512345678]
// [543454545435]
// [342343242343]
// To other number
// ---------------------------------------------------------------------------
//
void CGSCallDivertView::ExecuteDivertQueryL( TTelNumber& aTelNumber,
                                             const TPtrC& aTitle,
                                             TBool& aVmbxValidNumber )
    {
    __GSLOGSTRING("[CGSCallDivertView::ExecuteDivertQueryL]");

    CAknSinglePopupMenuStyleListBox* list =
        new ( ELeave ) CAknSinglePopupMenuStyleListBox;
    CleanupStack::PushL( list );

    CDesC16ArrayFlat* defNumbers =
        new ( ELeave ) CDesC16ArrayFlat( KCFDefaultNumbers );
    CleanupStack::PushL( defNumbers );

    CAknPopupList* divertPopup =
        CAknPopupList::NewL( list, R_AVKON_SOFTKEYS_OK_CANCEL__OK );
    CleanupStack::PushL( divertPopup );

    list->ConstructL( divertPopup,CEikListBox::ELeftDownInViewRect );
    list->CreateScrollBarFrameL( ETrue );
    list->ScrollBarFrame()->SetScrollBarVisibilityL(
        CEikScrollBarFrame::EOff,
        CEikScrollBarFrame::EAuto );
    divertPopup->SetTitleL( aTitle );
    if( iContextMenuItems )
    	{
    	delete iContextMenuItems;
    	iContextMenuItems = NULL;
    	}
    iContextMenuItems = new ( ELeave ) CDesCArrayFlat( KCFMaxNrOfTargets );
    if( iModel->DivertServiceGroup() == EGSSettIdVoiceDivert )
        {
        CreateVoiceDivertItemsL( *iContextMenuItems, *defNumbers );
        }
    else
        {
        CreateVideoDivertItemsL( *iContextMenuItems );
        }
    __GSLOGSTRING2( "[CGSCallDivertView::ExecuteDivertQueryL] ItemCount: %d, defNumCount: %d",
                    iContextMenuItems->Count(),
                    defNumbers->Count() );
    CTextListBoxModel* model = list->Model();
    model->SetItemTextArray( iContextMenuItems );
    model->SetOwnershipType( ELbmDoesNotOwnItemArray );

    // Executeld handles the events in the list
    TBool modified = divertPopup->ExecuteLD();
    __GSLOGSTRING1("[CGSCallDivertView::ExecuteDivertQueryL] modified: %d", modified);
    CleanupStack::Pop(); //voiceDivertPopup

    if ( modified )
        {
        if( iModel->DivertServiceGroup() == EGSSettIdVoiceDivert )
            {
            aTelNumber = HandleVoiceDivertSelectionL( *list,
                                                      *defNumbers,
                                                      aVmbxValidNumber );
            }
        else
            {
            aTelNumber = HandleVideoDivertSelectionL( *list );
            }
        }
    __GSLOGSTRING1( "[CGSCallDivertView::ExecuteVoiceDivertQueryL] aTelNumber: %S",
                    &aTelNumber);
    CleanupStack::PopAndDestroy( 2 ); //defNumbers, list
    __GSLOGSTRING( "[CGSCallDivertView::ExecuteVoiceDivertQueryL] End." );
    }


// ---------------------------------------------------------------------------
// CGSCallDivertView::HandleVoiceDivertSelectionL
//
// Handles user's choice in the Target list.
// ---------------------------------------------------------------------------
TTelNumber CGSCallDivertView::HandleVoiceDivertSelectionL(
    CAknSinglePopupMenuStyleListBox& aList,
    CDesC16ArrayFlat& aDefNumbers,
    TBool& aVmbxDivert )
    {
    __GSLOGSTRING("[CGSCallDivertView::HandleVoiceDivertSelectionL] Start...");
    TTelNumber telNumber;
    telNumber.Zero();
    TInt index = aList.CurrentItemIndex();
    __GSLOGSTRING1("[CGSCallDivertView::HandleVoiceDivertSelectionL] index: %d", index);

    if ( index == KCFDivertToMailBox )
        {
        if ( iModel->VoiceMailActivationL( telNumber ) == KErrNone )
            {
            if ( telNumber == KNullDesC )
                {
                ShowErrorNoteL();
                }
            aVmbxDivert = ETrue;
            }
        }
    //To other number - make a dataquery
    else if ( index == ( aList.Model()->NumberOfItems() - 1 ) )
        {
        ExecuteDivertNumberQueryL( telNumber, R_TEXT_PRMPT_NUMBER );
        __GSLOGSTRING1("[CGSCallDivertView::HandleVoiceDivertSelectionL] telNumber: %S", &telNumber);

        UpdateDefaultVoiceDivertNumbersL( telNumber );
        }
    //To pre-defined default number
    else
        {
        telNumber = aDefNumbers.MdcaPoint( index - 1 );
        iModel->DivertHandler()->SwapDefaultNumberL( index );
        }
    __GSLOGSTRING("[CGSCallDivertView::HandleVoiceDivertSelectionL] End.");
    return telNumber;
    }


// ---------------------------------------------------------------------------
// CGSCallDivertView::HandleVideoDivertSelectionL
//
// Handles user's choice in the video Target list.
// ---------------------------------------------------------------------------
TTelNumber CGSCallDivertView::HandleVideoDivertSelectionL(
    CAknSinglePopupMenuStyleListBox& aList )
    {
    __GSLOGSTRING("[GSCallDivertView::HandleVideoDivertSelectionL] Start");
    TTelNumber telNumber( 0 );
    TInt index = aList.CurrentItemIndex();
    if ( index == KCFDivertToMailBox )
        {
        if ( iModel->VideoMailActivationL( telNumber ) == KErrNone )
            {
            if ( telNumber == KNullDesC )
                {
                ShowErrorNoteL();
                }
            }
        }
    else if( index == ( aList.Model()->NumberOfItems() - 1 ) )
        { // To other number - make a dataquery (Last item in list)
        DisplayDataCallDivertNumberQueryL( telNumber );
        __GSLOGSTRING1("[GSCallDivertView::HandleVideoDivertSelectionL] telNumber: %S", &telNumber );
        }
    __GSLOGSTRING("[GSCallDivertView::HandleVideoDivertSelectionL] Finished");
    return telNumber;
    }


// ---------------------------------------------------------------------------
// CGSCallDivertView::DisplayDataCallDivertNumberQueryL
//
// Displays data call number query and sets call divert
// ---------------------------------------------------------------------------
void CGSCallDivertView::DisplayDataCallDivertNumberQueryL( TTelNumber& aTelNumber )
    {
    aTelNumber = *iModel->DivertHandler()->GetUsedDataNumberLC();
    ExecuteDivertNumberQueryL( aTelNumber, R_TEXT_PRMPT_DATA_NUMBER );
    if( aTelNumber != KNullDesC )
        {
        iModel->DivertHandler()->SetUsedDataNumberL( aTelNumber );
        }
    CleanupStack::PopAndDestroy(); // aTelNumber from GetUsedDataNumberLC()
    }


// ---------------------------------------------------------------------------
// CGSCallDivertView::ExecuteDivertNumberQueryL
//
// Executes general number query dialog used by all diverts (voice/data/fax)
// ---------------------------------------------------------------------------
//
void CGSCallDivertView::ExecuteDivertNumberQueryL( TDes& aValue, TInt aType )
    {
    TBool accepted = EFalse;
    while ( !accepted )
        {
        RemoveInvalidChars( aValue ); 
        if ( aValue.Length() > KGSMaxDataLength )
            {
            //Chop the first extra digits and keep the last KGSMaxDataLength
            //digits intact. This is according to the spec.
            aValue.Delete( 0, aValue.Length() - KGSMaxDataLength );
            }

        __GSLOGSTRING1("[CGSCallDivertView]    ExecuteDivertNumberQueryL: aValue: %S", &aValue);

        if( AknTextUtils::DigitModeQuery( AknTextUtils::EDigitModeEditorDefault ) )
            {
            AknTextUtils::DisplayTextLanguageSpecificNumberConversion( aValue );
            }
        CGSCallDivertQuery* query = CGSCallDivertQuery::NewL( aValue,
            CAknQueryDialog::ENoTone,
            R_AVKON_SOFTKEYS_OK_CANCEL__OK, R_CF_SOFTKEYS_SEARCH_CANCEL );
        CleanupStack::PushL( query );
        SetPromptToQueryL( *query, aType );
        TInt queryAction = query->ExecuteLD( R_DATA_QUERY );
        CleanupStack::Pop( query );
        AknTextUtils::ConvertDigitsTo( aValue, EDigitTypeWestern );
        switch ( queryAction )
            {
            case EAknSoftkeySearch:
                if( iStoresOpened )
                    {
                    // Reset the number selection because 'search' is selected.
                    aValue.Zero();
                    StartAsyncPhoneNumberFetchL();
                    accepted = ETrue;// Change to asynchronous mode
                    }
                break;
            case EAknSoftkeyOk:
                accepted = ETrue;
                if ( ValidatePhoneNumberL( aValue ) > KCFValidPhoneNumber ||
                     aValue.Length() == 0 )
                    {
                    accepted = EFalse;
                    }
                if ( iNumberFetchedAsynchronously )
                	{
                	switch( iModel->DivertServiceGroup() )
                	    {
                	    case EGSSettIdVoiceDivert:
                        	UpdateDefaultVoiceDivertNumbersL( aValue );
                        	break;
                	    case EGSSettIdVideoDivert:
                            {
                	        TTelNumber dataNumber( aValue );// Convert to TTelNumber
                	        iModel->DivertHandler()->SetUsedDataNumberL( dataNumber );
                	        break;
                            }
                	    default:
                	        break;
                	    }
                	iNumberFetchedAsynchronously = EFalse;
                	}
                break;
            case EAknSoftkeyBack:
            case EAknSoftkeyCancel:
            default:
                accepted = ETrue;
                aValue.Zero();
                break;
            }
        }
    }


// ---------------------------------------------------------------------------
// CGSCallDivertView::StartAsyncPhoneNumberFetchL
//
// Fetches values from phonebook ASYNCHRONOUSLY (pbk2 requirement).
// ---------------------------------------------------------------------------
//
void CGSCallDivertView::StartAsyncPhoneNumberFetchL()
    {
    TUint fetchFlags = 0;

    // Construct empty filter
    CVPbkFieldTypeSelector* contactViewFilter =
                            CVPbkFieldTypeSelector::NewL(iContactManager->FieldTypes());
    CleanupStack::PushL(contactViewFilter);

    // Append the filter object with suitable criteria (only contacts with
    // phone number shown)
    VPbkContactViewFilterBuilder::BuildContactViewFilterL(*contactViewFilter,
                              EVPbkContactViewFilterPhoneNumber, *iContactManager);
    RVPbkContactFieldDefaultPriorities array;
    CleanupClosePushL(array);

    TAiwSingleItemSelectionDataV3 data = TAiwSingleItemSelectionDataV3();
    // Details selection shows only phone numbers
    data.SetAddressSelectType(EAiwPhoneNumberSelect);
    data.SetDefaultPriorities(array);
    // Contact list shows only contacts that have phone number(s)
    data.SetFetchFilter(contactViewFilter);
    data.SetFlags(fetchFlags);

    CAiwGenericParamList& inParamList = iServiceHandler->InParamListL();
    TAiwGenericParam param(EGenericParamContactSelectionData,
                     TAiwVariant(TAiwSingleItemSelectionDataV3Pckg(data)));
    inParamList.AppendL(param);

    iServiceHandler->ExecuteServiceCmdL(
            KAiwCmdSelect,
            inParamList,
            iServiceHandler->OutParamListL(),
            0,
            this);

    CleanupStack::PopAndDestroy(); // array
    CleanupStack::PopAndDestroy( contactViewFilter );
    }


// ---------------------------------------------------------------------------
// CGSCallDivertView::ValidatePhoneNumberL
//
// Puts a single resource string into a list.
// ---------------------------------------------------------------------------
TInt CGSCallDivertView::ValidatePhoneNumberL( const TDesC& aDivertTo )
    {
    TInt enteredNumber = KErrNone;
    TInt noOfInvalidChars = KCFInvalidChars().Length();

    // Plus is a special case, as it is accepted as first character.
    // FindPlus returns KCFErrIllegalNumber(1) if the number is not
    // acceptable.
    enteredNumber += FindPlus( aDivertTo );
    for ( TInt index = 0; index < noOfInvalidChars; index++ )
        {
        enteredNumber += aDivertTo.Find(
            KCFInvalidChars().Mid( index, 1 ) );
        }
    // Find returns KErrNotFound(-1), if character(s) is not found.
    // Therefore, after four searches for special characters in the
    // number, all results that have greater than -5 as result are not
    // acceptable.
    if ( enteredNumber > KCFValidPhoneNumber )
        {
        ShowErrorNoteL();
        __GSLOGSTRING( "[CGSCallDivertView::ValidatePhoneNumberL] Invalid phone number. " );
        }
    return enteredNumber;
    }


// ---------------------------------------------------------------------------
// CGSCallDivertView::AppendDelayItemL
//
// Appends items to delay time list.
// ---------------------------------------------------------------------------
void CGSCallDivertView::AppendDelayItemL( CDesCArrayFlat& aArrayToAppend,
                                          TInt aIndex )
    {
    //Delay times are defined as 5,10,15,20,25,30.
    //Append each of these to a list.
    TInt delaySeconds = KCFDelayStep * ( aIndex + 1);
    HBufC* string = StringLoader::LoadLC( R_TEXT_SLIST_SECONDS, delaySeconds );

    TPtr bufPtr = string->Des();
    if( AknTextUtils::DigitModeQuery( AknTextUtils::EDigitModeShownToUser ) )
        {
        AknTextUtils::LanguageSpecificNumberConversion( bufPtr );
        }
    aArrayToAppend.AppendL( *string );
    CleanupStack::PopAndDestroy( string );
    }


// ---------------------------------------------------------------------------
// CGSCallDivertView::CreateVoiceDivertItemsL
//
// Creates items for the targert list.
// ---------------------------------------------------------------------------
void CGSCallDivertView::CreateVoiceDivertItemsL(
    CDesCArray& aItems,
    CDesC16ArrayFlat& aDefaultNumbers )
    {
    HBufC* item = StringLoader::LoadLC( R_TEXT_DIVERT_TO_VOICE_MAIL );
    aItems.AppendL( *item );
    CleanupStack::PopAndDestroy( item );
    ReadDefaultVoiceDivertNumbersL( aItems, aDefaultNumbers );
    HBufC* string = StringLoader::LoadLC( R_TEXT_DIVERT_TO_NUMBER );
    aItems.AppendL( *string );
    CleanupStack::PopAndDestroy( string );
    }


// ---------------------------------------------------------------------------
// CGSCallDivertView::CreateVideoDivertItemsL
//
// Creates items for the data target list.
// ---------------------------------------------------------------------------
void CGSCallDivertView::CreateVideoDivertItemsL( CDesCArray& aItems )
    {
    HBufC* item = StringLoader::LoadLC( R_QTN_SS_DIVERT_TO_VIDEO_MAILBOX );
    aItems.AppendL( *item );
    CleanupStack::PopAndDestroy( item );
    HBufC* string = StringLoader::LoadLC( R_QTN_SS_DIVERT_VCALL_TO_OTHER_NUMBER );
    aItems.AppendL( *string );
    CleanupStack::PopAndDestroy( string );
    }


// ---------------------------------------------------------------------------
//
// Seeks '+' from given number. If '+' is first character, it is ignored.
//
// ---------------------------------------------------------------------------
TInt CGSCallDivertView::FindPlus( const TDesC& aDivertTo )
    {
    TInt seekPlus = aDivertTo.Find( KCFPlusSign );
    if ( seekPlus == KErrNotFound )
        {
        //no '+' in the number
        return KErrNone;
        }
    //first char '+', but there can additional '+'s
    if ( seekPlus == 0 )
        {
        //ignore first '+'
        TPtrC concat = aDivertTo.Right( aDivertTo.Length() - 1 );
        seekPlus = concat.Find( KCFPlusSign );
        if ( seekPlus != KErrNotFound )
            {
            return KCFErrIllegalNumber; //two '+'s in number
            }
        return KErrNone;
        }
    //'+' in number, but not first char
    else
        {
        return KCFErrIllegalNumber;
        }
    }


// ---------------------------------------------------------------------------
// CGSCallDivertView::SetPromptToQueryL
// Sets prompt to a query.
//
// ---------------------------------------------------------------------------
void CGSCallDivertView::SetPromptToQueryL( CGSCallDivertQuery& aQuery,
                                           const TInt& aType )
    {
    HBufC* title = StringLoader::LoadLC( aType );
    aQuery.SetPromptL( *title );
    CleanupStack::PopAndDestroy( title );
    }


// ---------------------------------------------------------------------------
// CGSCallDivertView::CreateDelayTimeItemsL
// Creates items for delay time list.
//
// ---------------------------------------------------------------------------
//
void CGSCallDivertView::CreateDelayTimeItemsL( CDesCArrayFlat& aItems)
    {
    for ( TInt index = 0; index < KGSNrOfDelays; index++ )
        {
        AppendDelayItemL( aItems, index );
        }
    }


// ---------------------------------------------------------------------------
// CGSCallDivertView::CreateRestrictedCallDivertDelayTimeItemsL
// Creates items for Delay time list for restricted call divert.
//
// ---------------------------------------------------------------------------
//
void CGSCallDivertView::CreateRestrictedCallDivertDelayTimeItemsL(
    CDesCArrayFlat& aItems )
    {
    // first item is 0 seconds
    AppendDelayItemL( aItems, -1);
    for ( TInt index = 0; index < KGSNrOfDelays; index++ )
        {
        AppendDelayItemL( aItems, index );
        }
    AppendDelayItemL( aItems, ( 60 / KCFDelayStep ) - 1);
    AppendDelayItemL( aItems, ( 90 / KCFDelayStep ) - 1);
    AppendDelayItemL( aItems, ( 120 / KCFDelayStep ) - 1);
    }

// -----------------------------------------------------------------------------
// CGSCallDivertView::ProcessCommandL
// -----------------------------------------------------------------------------
void CGSCallDivertView::ProcessCommandL( TInt aCommand )
    {
    MenuBar()->SetContextMenuTitleResourceId( R_CALL_DIVERT_MSK_CONTEXTMENUBAR );
    // Call base class
    CAknView::ProcessCommandL( aCommand );
    }


// ---------------------------------------------------------------------------
//
// Before showing a options menu, delete options key specific settings if
// rocker key has been pressed.
// ---------------------------------------------------------------------------
//
void CGSCallDivertView::DynInitMenuPaneL( TInt aResourceId,
                                          CEikMenuPane* aMenuPane )
    {
    TInt currentItem;
    CEikMenuBar* menuBar = MenuBar();
    if( FeatureManager::FeatureSupported( KFeatureIdRestrictedCallDivert ) )
        {
        CGSRestrictedDivertContainer &container =
            *static_cast<CGSRestrictedDivertContainer*>( iContainer );
        currentItem = container.CurrentFeatureId();
        }
    else
        {
        CGSCallDivertContainer& container =
            *static_cast<CGSCallDivertContainer*>( iContainer );
        currentItem = container.CurrentFeatureId();
        }

    if ( aResourceId == R_CF_DIV_VIEW_MENU )
        {
        if (!menuBar->ItemSpecificCommandsEnabled() && menuBar->GetMenuType() == CEikMenuBar::EMenuOptions)
        	{
        	aMenuPane->SetItemDimmed( ECFCmdActivate, ETrue );
        	aMenuPane->SetItemDimmed( ECFCmdCancel, ETrue );
        	aMenuPane->SetItemDimmed( ECFCmdCheckStatus, ETrue );
        	}
        //remove help, if not supported from the main divert view
        //in this step, help is added to context menu too, which
        //is deleted in the next conditional stmt
        if ( FeatureManager::FeatureSupported ( KFeatureIdHelp ) )
            {
            aMenuPane->SetItemDimmed( EAknCmdHelp, EFalse );
            }
        else
            {
            aMenuPane->SetItemDimmed( EAknCmdHelp, ETrue );
            }

        if ( iRockerPress )
            {
            //remove Exit & Help from the menu
            aMenuPane->SetItemDimmed( EAknCmdExit, ETrue );
            aMenuPane->SetItemDimmed( EAknCmdHelp, ETrue );
            }
        else
            {
            iRockerPress = EFalse;
            }
        }

    if ( aResourceId == R_CF_MSK_DIV_VIEW_MENU ||
         aResourceId == R_CF_DIV_VIEW_MENU )
        {
        // when not available does not have check status
        if ( currentItem == ECFDivertWhenNotAvailable )
            {
            aMenuPane->SetItemDimmed( ECFCmdCheckStatus, ETrue );
            }
        }
    }

// ---------------------------------------------------------
// CGSCallDivertView::ShowErrorNote
//
// ---------------------------------------------------------
void CGSCallDivertView::ShowErrorNoteL()
    {
    HBufC* string = StringLoader::LoadLC( R_INVALID_NUMBER );
    CAknErrorNote* note = new ( ELeave ) CAknErrorNote ( ETrue );
    note->ExecuteLD( *string );
    CleanupStack::PopAndDestroy( string );
    }

// ---------------------------------------------------------------------------
// CGSCallDivertView::ReadDefaultVoiceDivertNumbersL
//
// Creates defcault items for the target list.
// ---------------------------------------------------------------------------
void CGSCallDivertView::ReadDefaultVoiceDivertNumbersL(
    CDesCArray& aItems,
    CDesC16ArrayFlat& aDefaultNumbers )
    {
    TInt amount = KErrNone;

    // this is for fetching name to a number from phonebook
    HBufC* defaultName = NULL;

    //Default numbers are fetched from shared data
    amount = iModel->DivertHandler()->GetDefaultNumbersL( aDefaultNumbers );
    __ASSERT_ALWAYS( amount == aDefaultNumbers.MdcaCount(),
        Panic( KGSDivertClassName, ECFDefaultNumberCountError ) );

    for ( TInt i = 0; i < amount; i++ )
        {
        //Default numbers should be checked, if there is a contact name associated
        // with it.
        defaultName = NULL;
        defaultName = iModel->MatchNumberToContactLC( aDefaultNumbers.MdcaPoint( i ) );
        __GSLOGSTRING1("[CGSCallDivertView]    ReadDefaultVoiceDivertNumbersL: defaultName: %S", &defaultName);
        if ( defaultName )
            {
            aItems.AppendL( *defaultName );
            }
        else
            {
            TTelNumber string;
            if ( iModel->NumberGroupingSupportedL() )
                {
                CPNGNumberGrouping* numberGrouping = CPNGNumberGrouping::NewL(
                    KPSUIMaxPSUIItemLength );
                CleanupStack::PushL( numberGrouping );

                if ( aDefaultNumbers.MdcaPoint( i ).Length() <=
                    numberGrouping->MaxDisplayLength() )
                    {
                    numberGrouping->Set( aDefaultNumbers.MdcaPoint( i ) );
                    string = numberGrouping->FormattedNumber();
                    }
                CleanupStack::PopAndDestroy( numberGrouping );
                }
            if ( !string.Length() )
                {
                string = aDefaultNumbers.MdcaPoint(i);
                }
            AknTextUtils::DisplayTextLanguageSpecificNumberConversion( string );
            aItems.AppendL( string );
            __GSLOGSTRING1("[CGSCallDivertView]    ReadDefaultVoiceDivertNumbersL: string: %S", &string);
            }
        CleanupStack::PopAndDestroy(); //defaultName
        }
    }

// ---------------------------------------------------------------------------
// CGSCallDivertView::UpdateDefaultVoiceDivertNumbersL
//
// Updates most recently used numbers to cenrep
// ---------------------------------------------------------------------------
void CGSCallDivertView::UpdateDefaultVoiceDivertNumbersL(  TDes& aNumber )

    {
    if ( aNumber !=  KNullDesC && iContextMenuItems )
        {
        //If activation is done to already existing default number,
        //but using the "Other number" selection, this must be verified.
        HBufC* contactName = iModel->MatchNumberToContactLC( aNumber );

        TInt found = KErrNone;
        TInt loc = KErrNotFound;

        if ( contactName )
            {
            found = iContextMenuItems->Find( *contactName, loc );
            }
        else
            {
            found = iContextMenuItems->Find( aNumber, loc );
            }
        CleanupStack::PopAndDestroy(); //contactName

        if ( found > 0 ) //number is not part of current def.numbers
            {
            iModel->DivertHandler()->SetNewDefaultNumberL( aNumber );
            }
        else
            {
            iModel->DivertHandler()->SwapDefaultNumberL( loc );
            }
        }
    else
        {
        __GSLOGSTRING( "[CGSCallDivertView::UpdateDefaultVoiceDivertNumbersL] iContextMenuItems == NULL!!" );
        }

    }

// ---------------------------------------------------------------------------
// CGSCallDivertView::HandleNotifyL
//
// AIW callback function.
// ---------------------------------------------------------------------------
TInt CGSCallDivertView::HandleNotifyL(
    TInt aCmdId,
    TInt aEventId,
    CAiwGenericParamList& aEventParamList,
    const CAiwGenericParamList& /*aInParamList*/ )
    {
    TInt err = KErrNone;
    if( ( aCmdId == KAiwCmdSelect ) || ( aCmdId == KAiwCmdAssign ) )
        {
        if( aEventId == KAiwEventCompleted )
            {
            TInt index = 0;
            iNumberFetchedAsynchronously = ETrue;
            const TAiwGenericParam* param = aEventParamList.FindFirst(
                index, EGenericParamContactLinkArray );

            if( param && iStoresOpened )
                {
                if( iContactLinks )
                    {
                    delete iContactLinks;
                    iContactLinks = NULL;
                    }
                TPtrC8 contactLinks = param->Value().AsData();

                CVPbkContactLinkArray* localContactLinks =
                    CVPbkContactLinkArray::NewLC(
                    contactLinks,
                    iContactManager->ContactStoresL() );

                CleanupStack::Pop( localContactLinks );
                iContactLinks = localContactLinks;

                if( iContactLinks->Count() > 0 )
                    {
                    iRetrieveOperation = iContactManager->RetrieveContactL( iContactLinks->At( 0 ), *this );
                    }
                }
            }
        else if (aEventId == KAiwEventCanceled)
            {
            // Do nothing.
            }
        else if (aEventId == KAiwEventOutParamCheck)
            {
            // This is needed as PBK2 mysteriously crashes if check is not
            // acccepted. There's no actual need to check anything but to go
            // ahead here so setting to ETrue.
            err = ETrue;
            }
        else if (aEventId == KAiwEventQueryExit)
            {
            err = ETrue;
            }
        }
    return err;
    }


// --------------------------------------------------------------------------
// CGSCallDivertView::VPbkSingleContactOperationComplete
// --------------------------------------------------------------------------
//
void CGSCallDivertView::VPbkSingleContactOperationComplete(
    MVPbkContactOperationBase& /*aOperation*/,
    MVPbkStoreContact* aContact )
    {
    if( iRetrieveOperation )
        {
        delete iRetrieveOperation;
        iRetrieveOperation = NULL;
        }
    TRAP_IGNORE
        (
        MVPbkStoreContactField* field = aContact->Fields().RetrieveField( iContactLinks->At( 0 ) );
        if ( field && field->FieldData().DataType() == EVPbkFieldStorageTypeText )
            {
            // aContact has selected phone number from Pbk2 dialog. Set this as
            // a text for ExecuteDivertNumberQueryL.
            TCallDivertSetting setting;
            setting.iSetting = EActivateDiverting;
            setting.iNumber = MVPbkContactFieldTextData::Cast( field->FieldData() ).Text();

            //Ask for user input.
            ExecuteDivertNumberQueryL( setting.iNumber, R_TEXT_PRMPT_NUMBER );

            // ExecuteDivertNumberQueryL() will set setting.iNumber to KNullDesC if user
            // selected 'search'. In this case program execution is moved to
            // pbk2 asynchronous dialog and this function can exit without
            // sending call divert request.
            if( setting.iNumber != KNullDesC )
                {
                // Send divert request.
                iModel->SendCallDivertRequestL( setting,
                                                RMobilePhone::EServiceActionRegister,
                                                iVoiceMailboxDivert,
                                                ECFCmdActivate,
                                                (TGSCallDivertServices) iCurrentItem );
                }
            }
        );
    delete aContact;// Ownership is taken by this
    }


// --------------------------------------------------------------------------
// CGSCallDivertView::VPbkSingleContactOperationFailed
// --------------------------------------------------------------------------
//
void CGSCallDivertView::VPbkSingleContactOperationFailed(
    MVPbkContactOperationBase& /*aOperation*/,
    TInt /*aError*/)
    {
    if( iRetrieveOperation )
        {
        delete iRetrieveOperation;
        iRetrieveOperation = NULL;
        }

    }

// --------------------------------------------------------------------------
// CGSCallDivertView::OpenComplete
// --------------------------------------------------------------------------
//
void CGSCallDivertView::OpenComplete()
    {
    iStoresOpened = ETrue;
    }

// --------------------------------------------------------------------------
// CGSCallDivertView::StoreReady
// --------------------------------------------------------------------------
//
void CGSCallDivertView::StoreReady( MVPbkContactStore& /*aContactStore*/ )
    {
    // Do nothing
    }

// --------------------------------------------------------------------------
// CGSCallDivertView::StoreUnavailable
// --------------------------------------------------------------------------
//
void CGSCallDivertView::StoreUnavailable
        ( MVPbkContactStore& /*aContactStore*/, TInt /*aReason*/ )
    {
    // Do nothing
    }

// --------------------------------------------------------------------------
// CGSCallDivertView::HandleStoreEventL
// --------------------------------------------------------------------------
//
void CGSCallDivertView::HandleStoreEventL( MVPbkContactStore& /*aContactStore*/,
        TVPbkContactStoreEvent /*aStoreEvent*/ )
    {
    // Do nothing
    }



// ---------------------------------------------------------------------------
// CGSCallDivertView::CreateTimeListL
//
// Activates this view's popup list in order to change time-setting.
// ---------------------------------------------------------------------------
TInt CGSCallDivertView::CreateTimeListL()
    {
    CAknSinglePopupMenuStyleListBox* list =
        new ( ELeave ) CAknSinglePopupMenuStyleListBox;
    CleanupStack::PushL( list );

    CAknPopupList* popupList =
        CAknPopupList::NewL( list, R_AVKON_SOFTKEYS_OK_CANCEL__OK );
    CleanupStack::PushL( popupList );

    HBufC* title = StringLoader::LoadLC( R_TEXT_DELAY_HEADER );
    popupList->SetTitleL( *title );
    CleanupStack::PopAndDestroy( title );

    list->ConstructL( popupList,CEikListBox::ELeftDownInViewRect );
    list->CreateScrollBarFrameL( ETrue );
    list->ScrollBarFrame()->SetScrollBarVisibilityL(
        CEikScrollBarFrame::EOff,
        CEikScrollBarFrame::EAuto );

    CDesCArrayFlat* items;
    if( FeatureManager::FeatureSupported( KFeatureIdRestrictedCallDivert ) )
        {
        items = new ( ELeave ) CDesCArrayFlat( KGSNrOfDelays + 4 );
        CleanupStack::PushL( items );
        CreateRestrictedCallDivertDelayTimeItemsL( *items );
        }
    else
        {
        items = new ( ELeave ) CDesCArrayFlat( KGSNrOfDelays );
        CleanupStack::PushL( items );
        CreateDelayTimeItemsL( *items );
        }
    CTextListBoxModel* model = list->Model();
    model->SetItemTextArray( items );
    model->SetOwnershipType( ELbmOwnsItemArray );
    CleanupStack::Pop(); //items
    list->HandleItemAdditionL();

    //set focus according to file read by phonesettings
    //set delay timer field value
    TInt currIndex = KErrNone;
    if( FeatureManager::FeatureSupported( KFeatureIdRestrictedCallDivert ) )
        {
        TInt itemCount = items->Count();
        if (( iModel->DivertHandler()->GetTimerValueL() / KCFDelayStep ) > ( itemCount - 4))
            {
            // Within 60, 90, 120 range
            switch ( iModel->DivertHandler()->GetTimerValueL() )
                {
                case 60:
                    currIndex = itemCount - 3;
                    break;
                case 90:
                    currIndex = itemCount - 2;
                    break;
                case 120:
                    currIndex = itemCount - 1;
                    break;
                }
            }
        else
            {
            currIndex = iModel->DivertHandler()->GetTimerValueL() / KCFDelayStep;
            }
        }
    else
        {
        currIndex = ( iModel->DivertHandler()->GetTimerValueL() / KCFDelayStep ) - 1;
        }
    __GSLOGSTRING1( "[CGSCallDivertView::CreateTimeListL] currIndex: %d", currIndex );

    list->SetCurrentItemIndex( currIndex );
    //if possible show topmost item
    list->SetTopItemIndex(0);

    TInt keyPress = popupList->ExecuteLD();
    CleanupStack::Pop( popupList );
    if ( keyPress )
        {
        TInt index = list->CurrentItemIndex();
        __GSLOGSTRING1( "[CGSCallDivertView::CreateTimeListL] index: %d", index );

        if( FeatureManager::FeatureSupported( KFeatureIdRestrictedCallDivert ) )
            {
            TInt itemCount = items->Count();
            if ( index > ( itemCount - 4))
                {
                // Within 60, 90, 120 range
                if ( index == itemCount - 3)
                    {
                    iModel->DivertHandler()->SetTimerValueL( 60 );
                    }
                else if ( index == itemCount - 2 )
                    {
                    iModel->DivertHandler()->SetTimerValueL( 90 );
                    }
                else if ( index == itemCount - 1 )
                    {
                    iModel->DivertHandler()->SetTimerValueL( 120 );
                    }
                }
            else
                {
                // first entry is 0 seconds
                currIndex = iModel->DivertHandler()->SetTimerValueL( index * KCFDelayStep );
                }
            }
        else
            {
            //save to file the correct value calculated from index
            iModel->DivertHandler()->SetTimerValueL( ( index + 1 ) * KCFDelayStep );
            }
        }
    CleanupStack::PopAndDestroy( list );
    return keyPress;
    }

// --------------------------------------------------------------------------
// CGSCallDivertView::RemoveInvalidChars
// Remove invalid " ", "()" which were sync with PC suite. 
// --------------------------------------------------------------------------
//
void CGSCallDivertView::RemoveInvalidChars( TDes& aNumber )
  {
  TInt noOfInvalidchars = KCallDivertInvalidchars().Length();
  TInt location = KErrNone; 
  // delete the invalid chars of KCallDivertInvalidchars. 
  //aNumber changed to valid phone number if it contains invalid chars
  for ( TInt index = 0; index < noOfInvalidchars; index++ )
       {     
       while ( ETrue )
           {
           location = aNumber.Find( KCallDivertInvalidchars().Mid( index, 1 ) );   
           if ( location == KErrNotFound )
               {
               break;
               }
           aNumber.Delete( location, 1 ); //delete the invalid char, aNumber changed
           }
       }
  }
// End of File
