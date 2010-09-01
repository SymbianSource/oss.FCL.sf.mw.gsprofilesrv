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
* Description:  Container for the Call folder
*
*/


// INCLUDE FILES

#include "GSCallPluginContainer.h"  //for CGSCallPluginContainer
#include "GsLogger.h"               //for logging traces
#include "gssettingid.h"            //for constants
#include "GSCallPluginModel.h"      //plug-in model
#include "GSCallPlugin.h"


#include <aknlists.h>               //for CAknSettingStyleListBox
#include <PsetCli.h>                //for CPsetCli
#include <PsetSAObserver.h>         //for CPsetSAObserver
#include <PsetCSP.h>                //for CSP features
#include <gscallpluginrsc.rsg>      //for resource IDs
#include <csxhelp/cp.hlp.hrh>
#include <featmgr.h>
#include <bldvariant.hrh>
#include <gslistbox.h>              //for listbox clases
#include <aknViewAppUi.h>
#include <eikappui.h>
#include <gsfwviewuids.h>

//LOCAL CONSTANTS
_LIT( KGSNameOfClass, "CGSCallPluginContainer" );
_LIT(KGSReplaceListControlChars, "\n\t\x2029");

// ========================= MEMBER FUNCTIONS ================================
// ---------------------------------------------------------------------------
//
// Symbian OS two phased constructor
//
// ---------------------------------------------------------------------------
void CGSCallPluginContainer::ConstructL( const TRect& aRect )
    {
    __GSLOGSTRING("[CGSCallPluginContainer]--> CGSCallPluginContainer::ConstructL");

    __GSLOGSTRING("[CGSCallPluginContainer] Creating model...");
    iModel = CGSCallPluginModel::NewL();
    __GSLOGSTRING("[CGSCallPluginContainer] Model created.");

    iListBox = new ( ELeave ) CAknSettingStyleListBox;

    __GSLOGSTRING("[CGSCallPluginContainer]--> ConstructL - Model created");
    
    iSsSettings = new (ELeave) RSSSettings;
    User::LeaveIfError( iSsSettings->Open() );
    __GSLOGSTRING("[CGSCallPluginContainer]--> ConstructL - SSSettings created & opened");

    BaseConstructL( aRect,
                    R_GS_CALL_VIEW_TITLE,
                    R_CALL_LBX );

    __GSLOGSTRING("[CGSCallPluginContainer] <--CGSCallPluginContainer::ConstructL");
    }

// ---------------------------------------------------------------------------
//
// Destructor
//
// ---------------------------------------------------------------------------
CGSCallPluginContainer::~CGSCallPluginContainer()
    {
    if ( iSsSettings )
        {
        iSsSettings->Close();
        }
    delete iSsSettings;

        delete iClirItems;
        delete iCWItems;
        delete iPCTItems;
        delete iDndItems;

    if ( iStillImgValues )
        {
        delete iStillImgValues;
        }
    if ( iChangeModeItems &&
         FeatureManager::FeatureSupported ( KFeatureIdJapanPrefixChange ) )
        {
        delete iChangeModeItems;
        }
    if ( iCallDurationItems )
        {
        delete iCallDurationItems;
        }
    if ( iLongPressCallKeyItems )
    	{
    	delete iLongPressCallKeyItems;
    	}
    if ( iListboxItemArray )
        {
        delete iListboxItemArray;
        }
    if ( iOwnImageVtCallItems )
        {
        delete iOwnImageVtCallItems;
        }
    if ( iModel )
        {
        delete iModel;
        }
    }

// ---------------------------------------------------------------------------
//
// Creates list box
//
// ---------------------------------------------------------------------------
void CGSCallPluginContainer::ConstructListBoxL( TInt aResLbxId )
    {
    __GSLOGSTRING("[CGSCallPluginContainer]--> CGSCallPluginContainer::ConstructListBoxL");

    iListBox->ConstructL( this, EAknListBoxSelectionList );

    iListboxItemArray = CGSListBoxItemTextArray::NewL( aResLbxId,
        *iListBox, *iCoeEnv );
    iListBox->Model()->SetItemTextArray( iListboxItemArray );
    iListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );

    iStillImgValues =  iCoeEnv->ReadDesC16ArrayResourceL(
        R_GS_STILL_IMAGE_SETTING_PAGE_LBX );

    if ( iModel->VoIPSupported() )
        {
        iClirItems = iCoeEnv->ReadDesC16ArrayResourceL(
                             R_IC_CLIR_SETTING_PAGE_LBX );
        iCWItems = iCoeEnv->ReadDesC16ArrayResourceL(
                             R_IC_WAITING_SETTING_PAGE_LBX );
        iPCTItems = iCoeEnv->ReadDesC16ArrayResourceL(
                             R_IC_PREFERRED_CALL_TYPE_SETTING_PAGE_LBX );
        iDndItems = iCoeEnv->ReadDesC16ArrayResourceL(
                             R_IC_DND_SETTING_PAGE_LBX );
        }

    if ( FeatureManager::FeatureSupported( KFeatureIdJapanPrefixChange ) )
        {
        iChangeModeItems = iCoeEnv->ReadDesC16ArrayResourceL(
            R_GS_REPLACE_PREFIX_SETTING_PAGE_LBX );
        }

    iCallDurationItems = iCoeEnv->ReadDesC16ArrayResourceL(
            R_GS_SHOW_CALL_DURATION_SETTING_PAGE_LBX );
    
    iLongPressCallKeyItems = iCoeEnv->ReadDesC16ArrayResourceL(
            R_GS_LONG_PRESS_CALL_SETTING_PAGE_LBX );
            
    iOwnImageVtCallItems = iCoeEnv->ReadDesC16ArrayResourceL(
            R_GS_OWN_IMAGE_VT_CALL_SETTING_PAGE_LBX );
            
    CreateListBoxItemsL();
    __GSLOGSTRING("[CGSCallPluginContainer] <--CGSCallPluginContainer::ConstructListBoxL");
    }

// ---------------------------------------------------------------------------
//
// Creates List box items
//
// ---------------------------------------------------------------------------
void CGSCallPluginContainer::CreateListBoxItemsL()
    {
    __GSLOGSTRING("[CGSCallPluginContainer]--> CGSCallPluginContainer::CreateListBoxItemsL");
// do not get check CSP if not using WINS
#ifndef __WINS__
    //Set CSP on, if it is not already
    if ( !iModel->GetCSPStatus() )
        {
        iModel->SetCSPActiveL( ETrue );
        }
#endif //__WINS__

    //create slide settings items
    if ( iModel->SlideSettingsShownL() )
        {
        MakeSlideSettingsItem( EGSOpeningSlideAnswerCallItemId );
        MakeSlideSettingsItem( EGSClosingSlideEndCallItemId );
        }
    
    //Create own number sending, call waiting soft-reject items and ccbs
    MakeONSItemL();
    MakeCWItemL();

    if ( iModel->VoIPSupported() )
        {
        MakeICClirItemL();
        MakeICWaitingItemL();
#ifndef RD_VOIP_REL_2_2 
        MakeICPreferredCallTypeItemL();
#endif // !RD_VOIP_REL_2_2
        MakeDndItemL();
        }


    MakeSoftRejectTxtItemL();

    if ( FeatureManager::FeatureSupported(
                                 KFeatureIdCsVideoTelephony ) )
        {
        MakeStillImageItemL(); //Still Image item
        MakeOwnImageVtCallItemL(); //VT call own image mute item
        }

    if ( FeatureManager::FeatureSupported( KFeatureIdJapanPrefixChange ) )
        {
        MakeReplacePrefixItemL();
        }

    // Create on/off settings
    MakeCallItemL( KGSSettIdSummaryAfterCall );
    
    //Create line in use
    TInt als = ESSSettingsAlsNotSupported;
    TInt alsError = iSsSettings->Get( ESSSettingsAls, als );

    if ( als != ESSSettingsAlsNotSupported && alsError == KErrNone )
        {
        MakePLUItemL( als );

        //Create Line blocking
        TInt alsBlock = ESSSettingsAlsBlockingNotSupported;
        alsError = iSsSettings->Get( ESSSettingsAlsBlocking, alsBlock );
        if ( alsBlock != ESSSettingsAlsBlockingNotSupported &&
             alsError == KErrNone )
            {
            MakePLBItemL( alsBlock );
            }
        }
    MakeCallDurationItemL();
    
  #ifdef RD_VT_LONG_SEND_KEY
    MakeLongPressCallKeyItemL();
  #endif
    __GSLOGSTRING("[CGSCallPluginContainer] <--CGSCallPluginContainer::CreateListBoxItemsL");
    }

// ---------------------------------------------------------------------------
//
// Updates list box with a changed item
//
// ---------------------------------------------------------------------------
void CGSCallPluginContainer::UpdateListBoxL( TInt aFeatureId, TInt aNewValue )
    {
    __GSLOGSTRING("[CGSCallPluginContainer]--> CGSCallPluginContainer::UpdateListBoxL");
    switch( aFeatureId )
        {
        case EGSOpeningSlideAnswerCallItemId:
        case EGSClosingSlideEndCallItemId:
            if ( iModel->SlideSettingsShownL() )
                {
                MakeSlideSettingsItem( aFeatureId );
                }
            break;
        case EGSCliItemId:
            MakeONSItemL();
            break;
//        case EGSCCBSItemId:
        case EGSCallWaitingItemId:
            return; //CW and CCBS does not have a value => no need to update
        case EGSICSendIntCallIdItemId:
            if ( iModel->VoIPSupported() )
                {
              MakeICClirItemL();
                }
            break;
        case EGSICWaitingItemId:
            if ( iModel->VoIPSupported() )
                {
              MakeICWaitingItemL();
                }
            break;
        case EGSICPreferredTelephonyId:
            if ( iModel->VoIPSupported() )
                {
              MakeICPreferredCallTypeItemL();
                }
            break;
        case EGSDndItemId:
            if ( iModel->VoIPSupported() )
                {
              MakeDndItemL();
                }
            break;
        case EGSSummaryAfterCallItemId:
            MakeCallItemL( KGSSettIdSummaryAfterCall );
            break;
        case EGSALSBlockItemId:
            MakePLBItemL( aNewValue );
            break;
        case EGSLineChangeItemId:
            MakePLUItemL( aNewValue );
            break;

        case EGSSoftRejectTextItemId:
            MakeSoftRejectTxtItemL();
            break;
        case EGSStillImageItemId:
            if ( FeatureManager::FeatureSupported(
                                 KFeatureIdCsVideoTelephony ) )
                {
                MakeStillImageItemL();
                }
            break;
        case EGSReplacePrefixItemId:
            if ( FeatureManager::FeatureSupported( KFeatureIdJapanPrefixChange ) )
                {
                MakeReplacePrefixItemL();
                }
                break;
        case EGSShowCallDurationItemId:
            MakeCallDurationItemL();
            break;
        case EGSLongPressCallKeyItemId:
		#ifdef RD_VT_LONG_SEND_KEY 
   			MakeLongPressCallKeyItemL();
		#endif
    		break;
        case EGSOwnImageVtCallItemId:
            if ( FeatureManager::FeatureSupported(
                                 KFeatureIdCsVideoTelephony ) )
                {
                MakeOwnImageVtCallItemL();
                }
            break;
        default:
            Panic( KGSNameOfClass, EInvalidIndex );
            break;
        }

    //iListBox->HandleItemAdditionL();
    iListBox->DrawDeferred();
    __GSLOGSTRING("[CGSCallPluginContainer] <--CGSCallPluginContainer::UpdateListBoxL");
    }


// ---------------------------------------------------------------------------
//
// Creates StillImage list item.
//
// ---------------------------------------------------------------------------
void CGSCallPluginContainer::MakeStillImageItemL()
    {
    __GSLOGSTRING("[CGSCallPluginContainer]--> CGSCallPluginContainer::MakeStillImageItemL");
    HBufC* dynamicText = HBufC::NewLC( KGSBufSize128 );
    TPtr ptrBuffer ( dynamicText->Des() );
    
    TInt dialogSelection = EGSVtStillImageNotInUse;
    if ( iModel->VTStillImageL() )
        dialogSelection = EGSVtStillImageInUse;
    ptrBuffer = (*iStillImgValues)[ dialogSelection ];

    // Finally, set the dynamic text
    iListboxItemArray->SetDynamicTextL( EGSStillImageItemId, ptrBuffer );
    CleanupStack::PopAndDestroy( dynamicText );

    // And add to listbox
    iListboxItemArray->SetItemVisibilityL( EGSStillImageItemId,
        CGSListBoxItemTextArray::EVisible );

    __GSLOGSTRING("[CGSCallPluginContainer] <--CGSCallPluginContainer::MakeStillImageItemL");
    }



// ---------------------------------------------------------------------------
//
// Creates OwnNumberSending list item.
//
// ---------------------------------------------------------------------------
void CGSCallPluginContainer::MakeONSItemL()
    {
    __GSLOGSTRING("[CGSCallPluginContainer]--> CGSCallPluginContainer::MakeONSItemL");
    HBufC16* dynamicText = NULL;

    TInt clir = ESSSettingsClirNetworkDefault;
    //to allow emulator usage without phone, do not retrieve values.
    TInt err = iSsSettings->Get( ESSSettingsClir, clir );
    __GSLOGSTRING2("[CGSCallPluginContainer]    MakeONSItemL: clir: %d, Error: %d", clir, err);
    if ( err != KErrNone )
        {
        return;
        }

    switch( clir )
        {
        case ESSSettingsClirExplicitSuppress:
            dynamicText = StringLoader::LoadL( R_CLI_SENDING_ON );
            break;
        case ESSSettingsClirExplicitInvoke:
            dynamicText = StringLoader::LoadL( R_CLI_SENDING_OFF );
            break;
        default:
            if ( iModel->RestrictedSendCallerIdSupportedL() )
                {
                //set initially to ON if restricted CLIR is supported
                dynamicText = StringLoader::LoadL( R_CLI_SENDING_ON );
                TInt err = iSsSettings->Set( 
                    ESSSettingsClir, ESSSettingsClirExplicitSuppress );
                if ( err != KErrNone )
                    {
                    delete dynamicText;
                    return;
                    }
                }
            else
                {
                dynamicText = StringLoader::LoadL( R_CLI_SENDING_DEFAULT );
                }
            break;
        }

    // Finally, set the dynamic text
    iListboxItemArray->SetDynamicTextL( EGSCliItemId, *dynamicText );

    // And add to listbox
    iListboxItemArray->SetItemVisibilityL( EGSCliItemId,
        CGSListBoxItemTextArray::EVisible );

    delete dynamicText;
    dynamicText = NULL;

    __GSLOGSTRING("[CGSCallPluginContainer] <--CGSCallPluginContainer::MakeONSItemL");
    }

// ---------------------------------------------------------------------------
//
// Creates CallWaiting list item.
//
// ---------------------------------------------------------------------------
void CGSCallPluginContainer::MakeCWItemL()
    {
    __GSLOGSTRING("[CGSCallPluginContainer]--> CGSCallPluginContainer::MakeCWItemL");
#ifndef __WINS__
    //Create call waiting
    if ( iModel->IsSettingSupported( EGSCSPCallWaiting ) )
        {
#endif //__WINS__
    iListboxItemArray->SetItemVisibilityL( EGSCallWaitingItemId,
        CGSListBoxItemTextArray::EVisible );
#ifndef __WINS__
        }
#endif //__WINS__
    __GSLOGSTRING("[CGSCallPluginContainer] <--CGSCallPluginContainer::MakeCWItemL");
    }

// ---------------------------------------------------------------------------
// CGSCallPluginContainer::MakeICClirItemL()
//
//
// ---------------------------------------------------------------------------
//
void CGSCallPluginContainer::MakeICClirItemL()
    {
    __GSLOGSTRING("[CGSCallPluginContainer]--> CGSCallPluginContainer::MakeICClirItemL");
    TInt value( KErrNone );
    iModel->GetVoIPCallSettingValue( EGSVoIPSendIntCallId, value );

    HBufC* dynamicText = HBufC::NewLC( KGSBufSize128 );
    TPtr ptrBuffer ( dynamicText->Des() );

    ptrBuffer = ( *iClirItems )[ value ];

    // Finally, set the dynamic text
    iListboxItemArray->SetDynamicTextL( EGSICSendIntCallIdItemId, ptrBuffer );

    iListboxItemArray->SetItemVisibilityL( EGSICSendIntCallIdItemId, 
      CGSListBoxItemTextArray::EVisible );

    CleanupStack::PopAndDestroy( dynamicText );
    
    __GSLOGSTRING("[CGSCallPluginContainer] <--CGSCallPluginContainer::MakeICClirItemL");
    }

// ---------------------------------------------------------------------------
// CGSCallPluginContainer::MakeICWaitingItemL()
//
//
// ---------------------------------------------------------------------------
//
void CGSCallPluginContainer::MakeICWaitingItemL()
    {
    __GSLOGSTRING("[CGSCallPluginContainer]--> CGSCallPluginContainer::MakeICWaitingItemL");
    TInt value( KErrNone );
    iModel->GetVoIPCallSettingValue( EGSVoIPCW, value );
    
    // Swap items because of different order.
    if ( value == EGSCSCallWaitingOff )
        {
        value = EGSCSCallWaitingOn;
        }
    else
        {
        value = EGSCSCallWaitingOff;
        }

    HBufC* dynamicText = HBufC::NewLC( KGSBufSize128 );
    TPtr ptrBuffer ( dynamicText->Des() );

    ptrBuffer = ( *iCWItems )[ value ];

    // Finally, set the dynamic text
    iListboxItemArray->SetDynamicTextL( EGSICWaitingItemId, ptrBuffer );
    iListboxItemArray->SetItemVisibilityL( EGSICWaitingItemId, 
        CGSListBoxItemTextArray::EVisible );
    
    CleanupStack::PopAndDestroy( dynamicText );
    __GSLOGSTRING("[CGSCallPluginContainer] <--CGSCallPluginContainer::MakeICWaitingItemL");
    }

// ---------------------------------------------------------------------------
// CGSCallPluginContainer::MakeICPreferredCallTypeItemL()
//
//
// ---------------------------------------------------------------------------
//
void CGSCallPluginContainer::MakeICPreferredCallTypeItemL()
    {
    __GSLOGSTRING("[CGSCallPluginContainer]--> CGSCallPluginContainer::MakeICPreferredCallTypeItemL");
    TInt value( KErrNone );
    iModel->GetVoIPCallSettingValue( EGSVoIPPreType, value );
    
    HBufC* dynamicText = HBufC::NewLC( KGSBufSize128 );
    TPtr ptrBuffer ( dynamicText->Des() );

    ptrBuffer = ( *iPCTItems )[ value ];

    // Finally, set the dynamic text
    iListboxItemArray->SetDynamicTextL( EGSICPreferredTelephonyId, ptrBuffer );
    CleanupStack::PopAndDestroy( dynamicText );

    iListboxItemArray->SetItemVisibilityL( EGSICPreferredTelephonyId,
      CGSListBoxItemTextArray::EVisible );

  __GSLOGSTRING("[CGSCallPluginContainer] <--CGSCallPluginContainer::MakeICPreferredCallTypeItemL");
    }

// ---------------------------------------------------------------------------
// CGSCallPluginContainer::MakeDndItemL()
//
//
// ---------------------------------------------------------------------------
//
void CGSCallPluginContainer::MakeDndItemL()
    {
    __GSLOGSTRING("[CGSCallPluginContainer]--> CGSCallPluginContainer::MakeDndItemL");
    TInt value( KErrNone );
    iModel->GetVoIPCallSettingValue( EGSVoIPDnd, value );
    
    // Swap items because of different order.
    if ( value == EGSCSCallDndOff )
        {
        value = EGSCSCallDndOn;
        }
    else
        {
        value = EGSCSCallDndOff;
        }

    HBufC* dynamicText = HBufC::NewLC( KGSBufSize128 );
    TPtr ptrBuffer ( dynamicText->Des() );

    ptrBuffer = ( *iDndItems )[ value ];

    // Finally, set the dynamic text
    iListboxItemArray->SetDynamicTextL( EGSDndItemId, ptrBuffer );
    CleanupStack::PopAndDestroy( dynamicText );

    iListboxItemArray->SetItemVisibilityL( EGSDndItemId,
      CGSListBoxItemTextArray::EVisible );

    __GSLOGSTRING("[CGSCallPluginContainer] <--CGSCallPluginContainer::MakeDndItemL");
    }

// ---------------------------------------------------------------------------
//
// Creates PhoneLineInUse list item.
//
// ---------------------------------------------------------------------------
void CGSCallPluginContainer::MakePLUItemL( TInt& aAlsValue )
    {
    __GSLOGSTRING("[CGSCallPluginContainer]--> CGSCallPluginContainer::MakePLUItemL");

    if ( aAlsValue == ESSSettingsAlsNotSupported )
        {
        iListboxItemArray->SetItemVisibilityL( EGSLineChangeItemId,
                           CGSListBoxItemTextArray::EInvisible );
        
        __GSLOGSTRING("[CGSCallPluginContainer] <--MakePLUItemL:AlsNotSupported");
        return;
        }

    HBufC* dynamicText = NULL;
    
    if ( aAlsValue == ESSSettingsAlsAlternate )
        {
        dynamicText = StringLoader::LoadL( R_ALS_LINE2 );
        }
    else
        {
        dynamicText = StringLoader::LoadL( R_ALS_LINE1 );
        }

    // for A&H number conversion
    TPtr bufPtr = dynamicText->Des();
    AknTextUtils::DisplayTextLanguageSpecificNumberConversion( bufPtr );

    iListboxItemArray->SetDynamicTextL( EGSLineChangeItemId, *dynamicText );

    // Add to listbox
    iListboxItemArray->SetItemVisibilityL( EGSLineChangeItemId,
        CGSListBoxItemTextArray::EVisible );

    delete dynamicText;
    dynamicText = NULL;
    __GSLOGSTRING("[CGSCallPluginContainer] <--CGSCallPluginContainer::MakePLUItemL");
    }

// ---------------------------------------------------------------------------
//
// Creates PhoneLineBlocking list item. No item is created, if service is not
// supported.
//
// ---------------------------------------------------------------------------
void CGSCallPluginContainer::MakePLBItemL( TInt& aAlsBlockValue )
    {
    __GSLOGSTRING("[CGSCallPluginContainer]--> CGSCallPluginContainer::MakePLBItemL");
    HBufC* dynamicText = NULL;

    if ( aAlsBlockValue == ESSSettingsAlsBlockingOn )
        {
        dynamicText = StringLoader::LoadL( R_ALS_LINE_BLOCKING_ON );
        }
    else
        {
        dynamicText = StringLoader::LoadL( R_ALS_LINE_BLOCKING_OFF );
        }

    iListboxItemArray->SetDynamicTextL( EGSALSBlockItemId, *dynamicText );

    // Add to listbox
    iListboxItemArray->SetItemVisibilityL( EGSALSBlockItemId,
        CGSListBoxItemTextArray::EVisible );

    delete dynamicText;
    dynamicText = NULL;
    __GSLOGSTRING("[CGSCallPluginContainer] <--CGSCallPluginContainer::MakePLBItemL");
    }

// ---------------------------------------------------------------------------
//
// Creates Speed Dialing, Anykey Answer, Summary After Call
// and Automatic redial items.
//
// ---------------------------------------------------------------------------
void CGSCallPluginContainer::MakeCallItemL( TInt aIndex )
    {
    __GSLOGSTRING("[CGSCallPluginContainer]--> CGSCallPluginContainer::MakeCallItemL");
    HBufC* settingOn  = NULL;
    HBufC* settingOff = NULL;
    TInt value = KGSSettingOff; //default is OFF for all
    TInt settingItem = 0;
    TBool ok = ETrue;

    TCallSettId keyName = EGSNotSet;

    switch ( aIndex )
        {
        case KGSSettIdSummaryAfterCall:
            keyName = EGSSummaryAfterCall;
            settingItem = EGSSummaryAfterCallItemId;
            settingOn = StringLoader::LoadLC( R_CALL_TERMINATION_NOTE_ON );
            settingOff = StringLoader::LoadLC( R_CALL_TERMINATION_NOTE_OFF );
            break;
        default:
            Panic( KGSNameOfClass, EInvalidIndex );
            break;
        }

    if( ok ) //if everything is ok so far
        {
        iModel->GetCallSettingValue( keyName, value );
        __GSLOGSTRING1("[CGSCallPluginContainer]    MakeCallItemL: value: %d", value);

        // Finally, set the dynamic text
        if ( value == KGSSettingOn )
            {
            iListboxItemArray->SetDynamicTextL( settingItem, settingOn->Des() );
            }
        else
            {
            iListboxItemArray->SetDynamicTextL( settingItem, settingOff->Des() );
            }

        CleanupStack::PopAndDestroy( 2 ); //settingOn, settingOff

        // And add to listbox
        iListboxItemArray->SetItemVisibilityL( settingItem,
            CGSListBoxItemTextArray::EVisible );
        }

    __GSLOGSTRING("[CGSCallPluginContainer] <--CGSCallPluginContainer::MakeCallItemL");
    }

// ---------------------------------------------------------------------------
// CGSCallPluginContainer::GetHelpContext(TCoeHelpContext& aContext) const
// Gets Help
//
// ---------------------------------------------------------------------------
//
void CGSCallPluginContainer::GetHelpContext(TCoeHelpContext& aContext) const
    {
    __GSLOGSTRING("[CGSCallPluginContainer]--> CGSCallPluginContainer::GetHelpContext");
    aContext.iMajor = KUidGS;
    aContext.iContext = KSCP_HLP_CALL;
    __GSLOGSTRING("[CGSCallPluginContainer] <--CGSCallPluginContainer::GetHelpContext");
    }

// ---------------------------------------------------------------------------
// Creates Call Completion When Busy Subscriber item.
//
// ---------------------------------------------------------------------------
//
/*
void CGSCallPluginContainer::MakeCCBSItemL()
    {
    if ( FeatureManager::FeatureSupported( KFeatureIdPhoneCcbs ) )
        {
        iListboxItemArray->SetItemVisibilityL( EGSCCBSItemId,
            CGSListBoxItemTextArray::EVisible );
        }
    }
*/

// ---------------------------------------------------------------------------
// CGSCallPluginContainer::CurrentFeatureId()
//
// ---------------------------------------------------------------------------
//
TInt CGSCallPluginContainer::CurrentFeatureId( ) const
    {
    return iListboxItemArray->CurrentFeature( );
    }


// ---------------------------------------------------------------------------
// Creates Soft reject text item.
//
// ---------------------------------------------------------------------------
//
void CGSCallPluginContainer::MakeSoftRejectTxtItemL()
    {
    __GSLOGSTRING("[CGSCallPluginContainer]--> CGSCallPluginContainer::MakeSoftRejectTxtItemL");
    HBufC* dynamicText = HBufC::NewLC( KGSSMSTxtEditorLength );
    TPtr string( dynamicText->Des() );
    string.Zero();

    GetSoftRejectTextL( string );
    if ( string.Length() == 0 )
        {
        string = KGSEmptySpace;
        }


    AknTextUtils::ReplaceCharacters( string, KGSReplaceListControlChars, 
                                                                  TChar(' ') );
                                                                  
    iListboxItemArray->SetDynamicTextL( EGSSoftRejectTextItemId, string );

    CleanupStack::PopAndDestroy( dynamicText );

    iListboxItemArray->SetItemVisibilityL( EGSSoftRejectTextItemId,
        CGSListBoxItemTextArray::EVisible );

    __GSLOGSTRING("[CGSCallPluginContainer] <--CGSCallPluginContainer::MakeSoftRejectTxtItemL");
    }

// ---------------------------------------------------------------------------
// Gets the value to the Soft Reject Txt.
//
// ---------------------------------------------------------------------------
//
void CGSCallPluginContainer::GetSoftRejectTextL( TDes& aSoftRejectTxt )
    {
    __GSLOGSTRING("[CGSCallPluginContainer]--> CGSCallPluginContainer::GetSoftRejectTextL");
    TInt value = KGSSettingOff;

    TCallSettId keyName = EGSSoftRejectDefault;
    iModel->GetCallSettingValue( keyName, value );

    if ( value == KGSSettingOff )
        {
        HBufC* defaultText = ReadFromResourceL( R_SOFT_REJECT_DEFAULT_TXT );
        CleanupStack::PushL( defaultText );
        aSoftRejectTxt = *defaultText;
        CleanupStack::PopAndDestroy(); //defaultText
        }
    else
        {
        TGSSMSTextEditor softRejectTxt;
        iModel->GetSoftRejectText( softRejectTxt );
        aSoftRejectTxt = softRejectTxt;
        }

    __GSLOGSTRING("[CGSCallPluginContainer] <--CGSCallPluginContainer::GetSoftRejectTextL");
    }

// ---------------------------------------------------------
// CGSCallPluginContainer::ReadFromResourceL
// ---------------------------------------------------------
//
HBufC* CGSCallPluginContainer::ReadFromResourceL(
        TInt aResourceId )
    {
    __GSLOGSTRING("[CGSCallPluginContainer]--> CGSCallPluginContainer::ReadFromResourceL");
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
        return NULL;
        }

    CleanupClosePushL( loader );
    HBufC* result = StringLoader::LoadL( aResourceId );
    CleanupStack::PopAndDestroy(); //loader & it is closed also.

    __GSLOGSTRING("[CGSCallPluginContainer] <--CGSCallPluginContainer::ReadFromResourceL");
    return result;
    }

// ---------------------------------------------------------------------------
// Creates Replace Prefix list box item.
//
// ---------------------------------------------------------------------------
//
void CGSCallPluginContainer::MakeReplacePrefixItemL()
    {
    __GSLOGSTRING("[CGSCallPluginContainer]--> MakeReplacePrefixItemL");
    HBufC* dynamicText = HBufC::NewLC( KGSBufSize128 );
    TPtr ptrBuffer ( dynamicText->Des() );

    const TInt changeMode = iModel->PrefixChangeModeL();
    ptrBuffer = ( *iChangeModeItems )[ changeMode ];

    // Finally, set the dynamic text
    iListboxItemArray->SetDynamicTextL( EGSReplacePrefixItemId, ptrBuffer );
    CleanupStack::PopAndDestroy( dynamicText );

    // And add to listbox
    iListboxItemArray->SetItemVisibilityL( EGSReplacePrefixItemId,
        CGSListBoxItemTextArray::EVisible );

    __GSLOGSTRING("[CGSCallPluginContainer] <--MakeReplacePrefixItemL");
    }

// ---------------------------------------------------------------------------
// Creates Show Call Duration list box item.
//
// ---------------------------------------------------------------------------
//
void CGSCallPluginContainer::MakeCallDurationItemL()
    {
    __GSLOGSTRING("[CGSCallPluginContainer]--> MakeCallDurationItemL");
    HBufC* dynamicText = HBufC::NewLC( KGSBufSize128 );
    TPtr ptrBuffer ( dynamicText->Des() );

    const TInt duration = iModel->CallDurationL();
    ptrBuffer = ( *iCallDurationItems )[ duration ];

    // Finally, set the dynamic text
    iListboxItemArray->SetDynamicTextL( EGSShowCallDurationItemId, ptrBuffer );
    CleanupStack::PopAndDestroy( dynamicText );

    // And add to listbox
    iListboxItemArray->SetItemVisibilityL( EGSShowCallDurationItemId,
        CGSListBoxItemTextArray::EVisible );

    __GSLOGSTRING("[CGSCallPluginContainer] <--MakeCallDurationItemL");
    }

// ---------------------------------------------------------------------------
// Creates Long Press Call Key list box items.
//
// ---------------------------------------------------------------------------
//
void CGSCallPluginContainer::MakeLongPressCallKeyItemL()
    {
    __GSLOGSTRING("[CGSCallPluginContainer]--> MakeLongPressCallKeyItemL");
    HBufC* dynamicText = HBufC::NewLC( KGSBufSize128 );
    TPtr ptrBuffer ( dynamicText->Des() );

    TInt statusLongPressCallKey = iModel->LongPressCallKeyL();
	// Here we again decrement by 1 so that values resemble appropriate title 
	// EGSLongPressCallKeyVoiceCall  ------> InActive in GS
	// EGSLongPressCallKeyVideoCall  ------> Video Call in GS
	// Refer to GSCallPlugin.hrh file for more info for the  description
	iModel->MapLongPressKeyCallKeyValue( statusLongPressCallKey );
   	ptrBuffer = ( *iLongPressCallKeyItems )[ statusLongPressCallKey ];

    // Finally, set the dynamic text
    iListboxItemArray->SetDynamicTextL( EGSLongPressCallKeyItemId, ptrBuffer );
    CleanupStack::PopAndDestroy( dynamicText );

    // And add to listbox
    iListboxItemArray->SetItemVisibilityL( EGSLongPressCallKeyItemId,
        CGSListBoxItemTextArray::EVisible );

    __GSLOGSTRING("[CGSCallPluginContainer] <--MakeLongPressCallKeyItemL");
    }


// ----------------------------------------------------------------------------
// CGSCallPluginContainer::MakeOwnImageVtCallItemL
// 
// Own image mute status on video call
// ----------------------------------------------------------------------------
//
void CGSCallPluginContainer::MakeOwnImageVtCallItemL()
    {
    __GSLOGSTRING("[CGSCallPluginContainer]--> MakeOwnImageVtCallItemL");
    HBufC* dynamicText = HBufC::NewLC( KGSBufSize128 );
    TPtr ptrBuffer ( dynamicText->Des() );

    TInt statusOwnImageVtCallKey = iModel->OwnImageVtCallStatusL();
   	ptrBuffer = ( *iOwnImageVtCallItems )[ statusOwnImageVtCallKey ];

    // Finally, set the dynamic text
    iListboxItemArray->SetDynamicTextL( EGSOwnImageVtCallItemId, ptrBuffer );
    CleanupStack::PopAndDestroy( dynamicText );

    // And add to listbox
    iListboxItemArray->SetItemVisibilityL( EGSOwnImageVtCallItemId,
        CGSListBoxItemTextArray::EVisible );

    __GSLOGSTRING("[CGSCallPluginContainer] <--MakeOwnImageVtCallItemL");
    }


// ---------------------------------------------------------------------------
// Sets observer for MSK label updations.
// ---------------------------------------------------------------------------
//
void CGSCallPluginContainer::SetMiddleSoftkeyObserver( 
    MGsFWMSKObserver* aObserver )
    {
    if ( !iMSKObserver && aObserver )
        {        
        iMSKObserver = aObserver;
        }
    }

// ---------------------------------------------------------------------------
// CGSCallPluginContainer::OfferKeyEventL
// Called when a key is pressed.
// ---------------------------------------------------------------------------
TKeyResponse CGSCallPluginContainer::OfferKeyEventL(
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
//
// CGSCallPluginContainer::MakeSlideSettingsItem
// Create a slide settings item ( "Opening slide to answer call" or "Closing slide to end call" ) 
//
// ---------------------------------------------------------------------------
void CGSCallPluginContainer::MakeSlideSettingsItem( TInt aItemId )
    {
    __GSLOGSTRING("[CGSCallPluginContainer]--> CGSCallPluginContainer::MakeSlideSettingsItem");
    
    HBufC* settingValue = NULL;
    TInt   value(0);
    
    switch ( aItemId )
        {
        case EGSOpeningSlideAnswerCallItemId:           
            iModel->GetCallSettingValue( EGSOpeningSlideAnswerCall,value );
            if ( EGSOpeningSlideAnswerCallOn == value )
                {
                settingValue = StringLoader::LoadLC
                    ( R_CP_SETTING_OPENING_SLIDE_ANSWER );
                }
            else
                {
                settingValue = StringLoader::LoadLC
                    ( R_CP_SETTING_OPENING_SLIDE_NOT_ANSWER );
                }                                    
            break;
        case EGSClosingSlideEndCallItemId:
            iModel->GetCallSettingValue( EGSClosingSlideEndCall,value );
            if ( EGSClosingSlideEndCallOn == value )
                {
                settingValue = StringLoader::LoadLC
                    ( R_CP_SETTING_CLOSING_SLIDE_END );
                }
            else 
                {
                settingValue = StringLoader::LoadLC
                    ( R_CP_SETTING_CLOSING_SLIDE_NOT_END );
                }
            break;
        default:
            Panic( KGSNameOfClass, EInvalidIndex );
        }
    
    iListboxItemArray->SetDynamicTextL( aItemId,settingValue->Des() );
    
    CleanupStack::PopAndDestroy( settingValue );
    
    // And add to listbox
    iListboxItemArray->SetItemVisibilityL
        ( aItemId,CGSListBoxItemTextArray::EVisible );
    
    __GSLOGSTRING("[CGSCallPluginContainer] <--CGSCallPluginContainer::MakeSlideSettingsItem");
    }
//End of File
