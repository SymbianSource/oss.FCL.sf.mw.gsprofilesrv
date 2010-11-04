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
* Description:  Container for the Internet Barring folder
*
*/


// INCLUDE FILES
#include <aknlists.h>
#include <StringLoader.h>
#include <gscallbarringpluginrsc.rsg>
#include <csxhelp/cp.hlp.hrh>
#include <gsfwviewuids.h>
#include <generalsettingsvariant.hrh>
#include <PSVariables.h>  //PubSub 
#include <settingsinternalcrkeys.h>
#include <LogsDomainCRKeys.h>
#include "GSInternetBarringContainer.h"
#include "gslistbox.h"              //for listbox clases

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// 
// Symbian OS two phased constructor
// 
// ---------------------------------------------------------------------------
void CGSSettListInternetBarringContainer::ConstructL( const TRect& aRect )
    {
    iListBox = new ( ELeave ) CAknSettingStyleListBox;
		iRichCallRepository = CRepository::NewL( KCRUidRichCallSettings );
		iTelephonyRepository = CRepository::NewL( KCRUidTelephonySettings );
    BaseConstructL( aRect, 
                    R_GS_INTERNET_BARRING_VIEW_TITLE, 
                    R_INTERNET_BARRING_LBX );
    }

// ---------------------------------------------------------------------------
// 
// Destructor
//  
// ---------------------------------------------------------------------------
CGSSettListInternetBarringContainer::~CGSSettListInternetBarringContainer()
    {
    if ( iItems )
        {
        delete iItems;
        }
    if ( iListboxItemArray )
        {
        delete iListboxItemArray;
        }
    if ( iTelephonyRepository )
        {
        delete iTelephonyRepository;
        iTelephonyRepository = NULL;
        }
    if ( iRichCallRepository )
        {
        delete iRichCallRepository;
        iRichCallRepository = NULL;
        }
    }

// ---------------------------------------------------------------------------
// 
// Creates list box
//  
// ---------------------------------------------------------------------------
void CGSSettListInternetBarringContainer::ConstructListBoxL( TInt aResLbxId )
    {        
    iListBox->ConstructL( this, EAknListBoxSelectionList); 
       
    iListboxItemArray = CGSListBoxItemTextArray::NewL( aResLbxId, 
        *iListBox, *iCoeEnv );        
    iListBox->Model()->SetItemTextArray( iListboxItemArray );
    iListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );
    
    // Create items  
    iItems = iCoeEnv->ReadDesC16ArrayResourceL( 
                        R_GS_INTERNET_BARRING_SETTING_PAGE_LBX );
    
    CreateListBoxItemsL();
    }

// ---------------------------------------------------------------------------
// 
// Creates List box items
//  
// ---------------------------------------------------------------------------
void CGSSettListInternetBarringContainer::CreateListBoxItemsL()
    {    
    MakeAnonymousItemL();
    }

// ---------------------------------------------------------------------------
// CGSSettListInternetBarringContainer::GetHelpContext
// Gets Help Context
//  
// ---------------------------------------------------------------------------
//
void CGSSettListInternetBarringContainer::GetHelpContext(
    TCoeHelpContext& aContext ) const
    {
    aContext.iMajor = KUidGS;
    aContext.iContext = KDIV_HLP_CALL_FORW_VOIP;
    }

// ---------------------------------------------------------------------------
// CGSSettListInternetBarringContainer::MakeAnonymousItemL
//  
// ---------------------------------------------------------------------------
//
void CGSSettListInternetBarringContainer::MakeAnonymousItemL()
    {
    TInt value( KErrNone );
    GetVoIPCallSettingValue( EGSVoIPBarring, value );
    
    HBufC* dynamicText = HBufC::NewLC( KGSSettingItemLength );
    TPtr ptrBuffer ( dynamicText->Des() );

    ptrBuffer = ( *iItems )[ value ];
    
    // Finally, set the dynamic text
    iListboxItemArray->SetDynamicTextL( EGSCBAnonymous, ptrBuffer );
    iListboxItemArray->SetItemVisibilityL( EGSCBAnonymous, 
        CGSListBoxItemTextArray::EVisible );    
    
    CleanupStack::PopAndDestroy( dynamicText );  
    }

// ---------------------------------------------------------------------------
// CGSSettListInternetBarringContainer::UpdateListBoxL
//  
// ---------------------------------------------------------------------------
//
void CGSSettListInternetBarringContainer::UpdateListBoxL()
    {
    MakeAnonymousItemL();     
    iListBox->HandleItemAdditionL();
    }


// ----------------------------------------------------------------------------
// CGSSettListInternetBarringContainer::GetVoIPCallSettingValue
// Gets one of the VoIP call related values (send my Int. call id, 
// Internet call waiting, preferred call type, do not disturb or 
// Internet call barring) from Central Repository
// ----------------------------------------------------------------------------
//
TInt CGSSettListInternetBarringContainer::GetVoIPCallSettingValue( 
    const TGSVoIPSetting aVoipSettingId, TInt& aVoipId ) 
    {
    TInt ret( KErrNone );
   
    switch ( aVoipSettingId )
        {
        case EGSVoIPSendIntCallId:
            ret = iRichCallRepository->Get( KRCSEClir, aVoipId );
            break;
        case EGSVoIPCW:
            ret = iRichCallRepository->Get( KRCSPSCallWaiting, aVoipId );
            break;
        case EGSVoIPPreType:
            ret = iRichCallRepository->Get( KRCSEPreferredTelephony, aVoipId );
            break;
        case EGSVoIPDnd:
            ret = iRichCallRepository->Get( KRCSEDoNotDisturb, aVoipId );
            break;
        case EGSVoIPBarring:
            ret = iRichCallRepository->Get( KRCSEAnonymousCallBlockRule, aVoipId );
            SwitchValue( aVoipId );
            break;
        default:
            break;
        }
    
    return ret;
    }

// ----------------------------------------------------------------------------
// CGSSettListInternetBarringContainer::SetVoIPCallSettingValue
// Sets one of the VoIP call related values (send my Int. call id, 
// Internet call waiting, preferred call type, do not disturb or 
// Internet call barring) from Central Repository
// ----------------------------------------------------------------------------
//
TInt CGSSettListInternetBarringContainer::SetVoIPCallSettingValue( 
    const TGSVoIPSetting aVoipSettingId, TInt aVoipId )
    {
    TInt ret( KErrNone );
    
    switch( aVoipSettingId )
        {
        case EGSVoIPSendIntCallId:
            ret = iRichCallRepository->Set( KRCSEClir, aVoipId );
            break;
        case EGSVoIPCW:
            ret = iRichCallRepository->Set( KRCSPSCallWaiting, aVoipId );
            break;
        case EGSVoIPPreType:
            ret = iRichCallRepository->Set( KRCSEPreferredTelephony, aVoipId );
            break;
        case EGSVoIPDnd:
            ret = iRichCallRepository->Set( KRCSEDoNotDisturb, aVoipId );
            break;
        case EGSVoIPBarring:
            SwitchValue( aVoipId );
            ret = iRichCallRepository->Set( KRCSEAnonymousCallBlockRule, aVoipId );
            break;
        default:
            break;
        }
        
    return ret;    
    }
    
// ----------------------------------------------------------------------------
// CGSSettListInternetBarringContainer::GetSCCPStatus
// Gets SCCP status from Central Repository
// ----------------------------------------------------------------------------
//
TInt CGSSettListInternetBarringContainer::GetSCCPStatus( TInt& aStatus )
    {
    TInt ret( KErrNone );
    ret = iTelephonyRepository->Get( KSCCPinstallUpgrade, aStatus );
    return ret;
    }

// ----------------------------------------------------------------------------
// CGSSettListInternetBarringContainer::SwitchValue
//
// Switching value from 1 to 0 and back
// ----------------------------------------------------------------------------
//
void CGSSettListInternetBarringContainer::SwitchValue( TInt& aValue )
    {
    switch( aValue )
        {
        case KGSSettingOff: //0
            aValue = KGSIndexOff; //1
            break;
        case KGSSettingOn: //1
            aValue = KGSIndexOn; //0
            break;
        default:
            break;
        }
    }
// End of File

