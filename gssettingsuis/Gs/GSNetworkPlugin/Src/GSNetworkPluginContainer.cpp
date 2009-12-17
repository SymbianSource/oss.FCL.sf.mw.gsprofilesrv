/*
* Copyright (c) 2003 - 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Container for Network folder in GSTelPlugin.
*
*/


// INCLUDE FILES
#include "GSNetworkPluginModel.h"       //for CGSModel
#include "gssettingid.h"            //for view IDs
#include "GSNetworkPluginContainer.h" //for CGSNetworkPluginContainer
#include "GsLogger.h"


#include <aknlists.h>               //for CAknSettingStyleListBox
#include <aknnotewrappers.h>        //for AknErrorNote
#include <aknappui.h>               //for iAvkonAppUi
#include <StringLoader.h>           //for StringLoader
#include <GsNetworkPluginRsc.rsg>       //for resource IDs
#include <PsetContainer.h>          //for CPsetContainer
#include <PsetNetwork.h>            //for CPsetNetwork
#include <gslistbox.h>              //for listbox classes
#include <csxhelp/cp.hlp.hrh>
#include <gsfwviewuids.h>           // for KUidGS
#include <MPsetNetworkModeObs.h>    //for network mode values
#include <gsmerror.h>               //for GSM-specific error messages
#include <featmgr.h>                //Feature Manager
#include <MPsetNetworkModeObs.h>
#include "GSNetworkDebugHelper.h"

#ifndef RD_STARTUP_CHANGE
    #include <sysstartup.h>             //Startup reasons
#else
    #include <starterclient.h>          // Startup reasons
#endif // RD_STARTUP_CHANGE


// LOCAL CONSTANTS
_LIT( KNameOfClass, "CGSNetworkPluginContainer" );

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// C++ Constructor. Needed for initializing iPlugin.
// ---------------------------------------------------------------------------
CGSNetworkPluginContainer::CGSNetworkPluginContainer( CGSNetworkPlugin* aPlugin )
    :iPlugin( aPlugin )
    {
    }

// ---------------------------------------------------------------------------
//
// Symbian OS two phased constructor with pointer to Network object
//
// ---------------------------------------------------------------------------
void CGSNetworkPluginContainer::ConstructL( const TRect& aRect )
    {
    iListBox = new ( ELeave ) CAknSettingStyleListBox;

    //model construction
    iModel = CGSNetworkPluginModel::NewL( this, iPlugin );

    BaseConstructL( aRect,
                    R_GS_NET_VIEW_TITLE,
                    R_NET_LBX );
    }

// ---------------------------------------------------------------------------
//
// Destructor
//
// ---------------------------------------------------------------------------
CGSNetworkPluginContainer::~CGSNetworkPluginContainer()
    {
    if ( iListboxItemArray )
        {
        delete iListboxItemArray;
        iListboxItemArray = NULL;
        }

    if( iModel )
        {
        delete iModel;
        iModel = NULL;
        }
    }

// ---------------------------------------------------------------------------
//
// Creates list box
//
// ---------------------------------------------------------------------------
void CGSNetworkPluginContainer::ConstructListBoxL( TInt aResLbxId )
    {
    __GSLOGSTRING("[GS]--> CGSNetworkPluginContainer::ConstructListBoxL");
    iListBox->ConstructL( this, EAknListBoxSelectionList );
    iListboxItemArray = CGSListBoxItemTextArray::NewL( aResLbxId,
        *iListBox, *iCoeEnv );
    iListBox->Model()->SetItemTextArray( iListboxItemArray );
    iListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );

    CreateListBoxItemsL();
    __GSLOGSTRING("[GS] <--CGSNetworkPluginContainer::ConstructListBoxL");
    }

// ---------------------------------------------------------------------------
//
// Creates list box items
//
// ---------------------------------------------------------------------------
void CGSNetworkPluginContainer::CreateListBoxItemsL()
    {
    __GSLOGSTRING("[GS]--> CGSNetworkPluginContainer::CreateListBoxItemsL");
// do not get check CSP if not using WINS
#ifndef __WINS__
    //Set CSP on, if it is not already
    if ( !iModel->GetCSPStatus() )
        {
        iModel->SetCSPActiveL( ETrue );
        }
#endif

    if ( FeatureManager::FeatureSupported( KFeatureIdProtocolWcdma ) )
        {
        if ( iModel->IsNetworkModeVisible() )
            {
            MakeNwModeUiItemL();
            }
        }

    MakeNwItemL();
    MakeMcnItemL();
    __GSLOGSTRING("[GS] <--CGSNetworkPluginContainer::CreateListBoxItemsL");
    }

// ---------------------------------------------------------------------------
//
// Updates changed list box item
//
// ---------------------------------------------------------------------------
void CGSNetworkPluginContainer::UpdateListBoxL( TInt aFeatureId, TInt aValue )
    {
    __GSLOGSTRING("[GS]--> CGSNetworkPluginContainer::UpdateListBoxL");
    switch ( aFeatureId )
        {
        case EGSNetworkModeItemId:
            if ( FeatureManager::FeatureSupported( KFeatureIdProtocolWcdma ) )
                {
                if ( iModel->IsNetworkModeVisible() )
                    {
                    MakeNwModeUiItemL();
                    }
                }
            break;
        case EGSNetworkModeSelectionItemId:
            MakeNwItemL( aValue );
            break;
        case EGSMCNItemId:
            MakeMcnItemL();
            break;
        default:
            Panic( KNameOfClass, EInvalidIndex );
            break;
        }
    iListBox->HandleItemAdditionL();
    __GSLOGSTRING("[GS] <--CGSNetworkPluginContainer::UpdateListBoxL");
    }

// ---------------------------------------------------------------------------
//
// Creates new Network list box item
//
// ---------------------------------------------------------------------------
void CGSNetworkPluginContainer::MakeNwItemL( TInt aValue )
    {
    __GSLOGSTRING("[GS]--> CGSNetworkPluginContainer::MakeNwItemL");
    HBufC* dynamicText = NULL;
    TInt currentValue = KErrNone;

    if ( aValue != KErrNotFound )
        {
        currentValue = aValue;
        }
    else
        {
    //use bogus values for WINS to avoid jamming the emulator
#ifdef __WINS__
    currentValue = EGSManualSelectMode;
#else
    currentValue = GetSelectionMode();
#endif // __WINS__
        }

    TUint32 resourceId = R_NETSL_NETSELECTMODE_LBX;
    TInt featureId = EGSAutomaticSelectMode;

    if ( currentValue == EGSManualSelectMode )
        {
        featureId = EGSManualSelectMode;
        }

    dynamicText = CGSItemTextArray::GetItemCaptionFromFeatureIdLC(
                                                 resourceId,
                                                 featureId,
                                                 *iCoeEnv );

#ifndef __WINS__
    //Create network mode setting item
    if ( iModel->IsSettingSupported( EGSCSPManualNetworkSelection ) )
        {
#endif
        // Finally, set the dynamic text
        iListboxItemArray->SetDynamicTextL( EGSNetworkModeSelectionItemId,
            *dynamicText );

        // And add to listbox
        iListboxItemArray->SetItemVisibilityL( EGSNetworkModeSelectionItemId,
            CGSListBoxItemTextArray::EVisible );

#ifndef __WINS__
        }
#endif

    CleanupStack::PopAndDestroy( dynamicText );
    __GSLOGSTRING("[GS] <--CGSNetworkPluginContainer::MakeNwItemL");
    }

// ---------------------------------------------------------------------------
//
// Gets the currently active selection mode.
//
// ---------------------------------------------------------------------------
TInt CGSNetworkPluginContainer::GetSelectionMode()
    {
    __GSLOGSTRING("[GS]--> CGSNetworkPluginContainer::GetSelectionMode");
    MPsetNetworkSelect::TSelectMode mode =
        iModel->GetNetworkSelectionMode();

    if ( mode == MPsetNetworkSelect::ENetSelectModeManual )
        {
        __GSLOGSTRING("[GS] <--CGSNetworkPluginContainer::GetSelectionMode");
        return EGSManualSelectMode;
        }
    else
        {
        __GSLOGSTRING("[GS] <--CGSNetworkPluginContainer::GetSelectionMode");
        return EGSAutomaticSelectMode;
        }
    }

// ---------------------------------------------------------------------------
//
// Creates new MCN list box item
//
// ---------------------------------------------------------------------------
void CGSNetworkPluginContainer::MakeMcnItemL()
    {
    __GSLOGSTRING("[GS]--> CGSNetworkPluginContainer::MakeMcnItemL");
    HBufC* dynamicText = NULL;
    TInt readValue = 0;

    GetMcnValue( readValue );
    TUint32 resourceId = R_MCN_CELL_INFO_DISP_MODE_LBX;
    TInt featureId = EGSMcnSetOn;

    switch ( readValue ) //resource manipulation
        {
        case 0: // Off should be displayed in UI
            featureId = EGSMcnSetOff;
            break;
        case 1: // On should be displayed in UI
            featureId = EGSMcnSetOn;
            break;
        default:
            break;
        }

    dynamicText = CGSItemTextArray::GetItemCaptionFromFeatureIdLC(
                                                 resourceId,
                                                 featureId,
                                                 *iCoeEnv );

    // Finally, set the dynamic text
    iListboxItemArray->SetDynamicTextL( EGSMCNItemId, *dynamicText );

    // And add to listbox
    iListboxItemArray->SetItemVisibilityL( EGSMCNItemId,
        CGSListBoxItemTextArray::EVisible );

    CleanupStack::PopAndDestroy( dynamicText );
    __GSLOGSTRING("[GS] <--CGSNetworkPluginContainer::MakeMcnItemL");
    }

// ---------------------------------------------------------------------------
//
// Gets MCN value from model.
//
// ---------------------------------------------------------------------------
//
void CGSNetworkPluginContainer::GetMcnValue( TInt &aMcnValue )
    {
    iModel->GetMCNSettingValue( aMcnValue );
    }

// ---------------------------------------------------------------------------
//
// Sets MCN value from model.
//
// ---------------------------------------------------------------------------
//
void CGSNetworkPluginContainer::SetMcnValue( TInt &aMcnValue )
    {
    iModel->SetMCNSettingValue( aMcnValue );
    }


// ---------------------------------------------------------------------------
//
// Creates network mode ui list box item
//
// ---------------------------------------------------------------------------
void CGSNetworkPluginContainer::MakeNwModeUiItemL()
    {
    __GSLOGSTRING("[GS]--> CGSNetworkPluginContainer::MakeNwModeUiItemL");
    HBufC* dynamicText = NULL;

    // In here KGSNetworkModeCapsNotUpdated means mode is not updated yet
    if( iModel->GetNetworkMode() != KGSNetworkModeCapsNotUpdated )
        {
        // Allocate value to dynamicText from the resource item
        TUint32 resourceId= R_NET_NETWORK_MODE_LBX;
        dynamicText = CGSItemTextArray::GetItemCaptionFromFeatureIdLC(
            resourceId, GetCurrentNetworkModeSelectionL(), *iCoeEnv );
        }
    else
        {
        _LIT( KEmpty, " ");
        dynamicText = KEmpty().AllocLC ( );
        }

    // Finally, set the dynamic text
    iListboxItemArray->SetDynamicTextL( EGSNetworkModeItemId, *dynamicText );

    // And add to listbox
    iListboxItemArray->SetItemVisibilityL( EGSNetworkModeItemId,
        CGSListBoxItemTextArray::EVisible );

    // this variable needs to be removed from cleanup stack
    CleanupStack::PopAndDestroy( dynamicText );
    __GSLOGSTRING("[GS] <--CGSNetworkPluginContainer::MakeNwModeUiItemL");
    }


// ---------------------------------------------------------------------------
// CGSNetworkPluginContainer::CheckAndAlterContentsL
//
// Trims non-supported variable options from the array.
// This array items are displayed as radio button setting page items
// ---------------------------------------------------------------------------
//
void CGSNetworkPluginContainer::CheckAndAlterContentsL(
                         CGSRadioButtonSettingPageItemTextArray& aItemArray )
    {
    __GSLOGSTRING("[GS]--> CGSNetworkPluginContainer::CheckAndAlterContentsL");
    TInt value = 0;
    TInt supportedNetworks = iModel->GetSupportedNetworksL();
    __GSLOGSTRING1("[GS]    CheckAndAlterContentsL: supportedNetworks: %d", supportedNetworks);

    // The 0th item corresponds to DualMode. This is always available in the
    // setting page, so no actions required, just make it visible.
    aItemArray.SetItemVisibilityL( EGSNetworkModeDualmode,
                                   CGSListBoxItemTextArray::EVisible );

    // the first bit corresponds to UMTS
    value = supportedNetworks & ENetFirstBit;
    __GSLOGSTRING1("[GS]    CheckAndAlterContentsL: (UMTS)value: %d", value);

    if ( value == EGSNetworkModeUMTS )
        {
        aItemArray.SetItemVisibilityL( EGSNetworkModeUMTS,
                                       CGSListBoxItemTextArray::EVisible );
        }
    else
        {
        aItemArray.SetItemVisibilityL( EGSNetworkModeUMTS,
                                       CGSListBoxItemTextArray::EInvisible );
        }

    // Get the zeroeth value (GSM) from the list
    value = supportedNetworks & ENetSecondBit;
    __GSLOGSTRING1("[GS]    CheckAndAlterContentsL: (GSM)value: %d", value);

    if ( value == EGSNetworkModeGSM )
        {
        aItemArray.SetItemVisibilityL( EGSNetworkModeGSM,
                                       CGSListBoxItemTextArray::EVisible );
        }
    else
        {
        aItemArray.SetItemVisibilityL( EGSNetworkModeGSM,
                                       CGSListBoxItemTextArray::EInvisible );
        }

    __GSLOGSTRING("[GS] <--CGSNetworkPluginContainer::CheckAndAlterContentsL");
    }

// ---------------------------------------------------------------------------
// CGSNetworkPluginContainer::GetHelpContext(TCoeHelpContext& aContext) const
// Gets Help
//
// ---------------------------------------------------------------------------
//
void CGSNetworkPluginContainer::GetHelpContext(TCoeHelpContext& aContext) const
    {
    aContext.iMajor = KUidGS;
    aContext.iContext = KCP_HLP_NETWORK;
    }

// ---------------------------------------------------------------------------
// CGSSettListCallContainer::CurrentFeatureId()
//
// ---------------------------------------------------------------------------
//
TInt CGSNetworkPluginContainer::CurrentFeatureId( ) const
    {
    return iListboxItemArray->CurrentFeature( );
    }


void CGSNetworkPluginContainer::HandleNetworkModeChangeL()
    {
    UpdateListBoxL( EGSNetworkModeItemId, GetCurrentNetworkModeSelectionL() );

     //updating value
     CGSRadioButtonSettingPageItemTextArray* nwModeArray =
            CGSRadioButtonSettingPageItemTextArray::NewL(
            R_NET_NETWORK_MODE_LBX,
            *iCoeEnv,
            NULL );
     // For showing updated network mode value???
    CleanupStack::PushL( nwModeArray );
    CheckAndAlterContentsL( *nwModeArray );
    TInt currentIndex = nwModeArray->IndexForFeatureIdL(
        GetCurrentNetworkModeSelectionL() );
    HBufC* buf = StringLoader::LoadL(
             R_CONFIRM_NOTE_NWMODE_STRING,
             nwModeArray->MdcaPoint( currentIndex ),
             iEikonEnv );
    CleanupStack::PushL( buf );
    CAknConfirmationNote* note = new( ELeave ) CAknConfirmationNote( ETrue );
    note->ExecuteLD( *buf );
    CleanupStack::PopAndDestroy( buf );
    CleanupStack::PopAndDestroy( nwModeArray );
    }


// ---------------------------------------------------------------------------
// CGSNetworkPluginContainer::TelPluginModel
//
// Return the model pointer for use in view class
// ---------------------------------------------------------------------------
//
CGSNetworkPluginModel* CGSNetworkPluginContainer::NetPluginModel()
    {
    return iModel;
    }

// ---------------------------------------------------------------------------
// Sets observer for MSK label updations.
// ---------------------------------------------------------------------------
//
void CGSNetworkPluginContainer::SetMiddleSoftkeyObserver(
    MGsFWMSKObserver* aObserver )
    {
    if ( !iMSKObserver && aObserver )
        {
        iMSKObserver = aObserver;
        }
    }

// ---------------------------------------------------------------------------
// CGSNetworkPluginContainer::OfferKeyEventL
// Called when a key is pressed.
// ---------------------------------------------------------------------------
TKeyResponse CGSNetworkPluginContainer::OfferKeyEventL(
    const TKeyEvent& aKeyEvent,
    TEventCode aType )
    {
    switch ( aKeyEvent.iCode )
        {
        case EKeyUpArrow:
        case EKeyDownArrow:
            {
            TKeyResponse listboxResp =
                iListBox->OfferKeyEventL( aKeyEvent, aType );
            if ( iMSKObserver )
                {
                iMSKObserver->CheckMiddleSoftkeyLabelL();
                }
            return listboxResp;
            }
        case EKeyLeftArrow:
        case EKeyRightArrow:
            // Listbox takes all events even if it doesn't use them
            return EKeyWasNotConsumed;
        default:
            break;
        }

    return iListBox->OfferKeyEventL( aKeyEvent, aType );
    }


// ---------------------------------------------------------------------------
// CGSNetworkPluginContainer::GetCurrentNetworkModeSelectionL
//
// Returns the current listbox selection of network mode.
// ---------------------------------------------------------------------------
//
TGSNetworkModeItems CGSNetworkPluginContainer::GetCurrentNetworkModeSelectionL()
    {
    TGSNetworkModeItems featureId = EGSNetworkModeDualmode;

    switch( iModel->GetNetworkMode() )
        {
        case RMmCustomAPI::KCapsNetworkModeGsm:
            featureId = EGSNetworkModeGSM;
            break;

        case RMmCustomAPI::KCapsNetworkModeUmts:
            featureId = EGSNetworkModeUMTS;
            break;

        case RMmCustomAPI::KCapsNetworkModeDual:
            featureId = EGSNetworkModeDualmode;
            break;
        default:
            // Mode might not be updated yet, in which case default is dual.
            // Not a perfect solution but have to have an item selected in 
            // options box
            featureId = EGSNetworkModeDualmode;
            break;
        }

#ifdef _DEBUG
    TBuf<KGSNetworkModeDebugStrLen> selectedModeBuf;
    GSNetworkDebugHelper::NwToDes( featureId, selectedModeBuf );
    __GSLOGSTRING1( "[CGSNetworkPluginContainer::GetCurrentNetworkModeSelectionL] %S ",
                    &selectedModeBuf );
#endif // _DEBUG

    iModel->CheckState( featureId );
    return featureId;
    }


// ---------------------------------------------------------------------------
// CGSNetworkPluginContainer::SetCurrentNetworkModeSelectionL
//
//
// ---------------------------------------------------------------------------
//
void CGSNetworkPluginContainer::SetCurrentNetworkModeSelectionL(
        TGSNetworkModeItems aNetworkModeLbxItem )
    {
    __GSLOGSTRING1( "[CGSNetworkPluginContainer::SetCurrentNetworkModeSelectionL] Mode:%d", aNetworkModeLbxItem );
    TUint32 networkMode;
    switch ( aNetworkModeLbxItem )
        {
        case EGSNetworkModeGSM: //just using the index
            networkMode = RMmCustomAPI::KCapsNetworkModeGsm;
            break;
        case EGSNetworkModeUMTS:
            networkMode = RMmCustomAPI::KCapsNetworkModeUmts;
            break;
        case EGSNetworkModeDualmode:
        default:
            networkMode = RMmCustomAPI::KCapsNetworkModeDual;
            break;
        }
    // Don't change the network mode if there is ongoing phone call
    // since this will disconnect it
    if ( !iModel->IsCallActive() )
        {
        iModel->SetNetworkModeL( networkMode );
        iSelectedNetworkModeLbxItem = aNetworkModeLbxItem;
        }
    __GSLOGSTRING( "[CGSNetworkPluginContainer::SetCurrentNetworkModeSelectionL] End]" );
    }



//End of File
