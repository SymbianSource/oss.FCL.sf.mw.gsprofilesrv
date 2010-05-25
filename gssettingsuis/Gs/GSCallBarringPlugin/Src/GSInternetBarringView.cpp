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
* Description:  View for the Internet Barring folder
*
*/

 
// INCLUDE FILES
#include "CallBarringPlugin.hrh"    //for enumerations (e.g.TGSInternetBarringSetting)
#include "GSInternetBarringView.h"//for CGSSettListInternetBarringView
#include "GSInternetBarringContainer.h" //for CGSBaseContainer
#include "LocalViewIds.h"         //for KSettListBarringViewId, KGSMainViewId
#include "GsLogger.h"


#include <e32base.h>
#include <featmgr.h>
#include <gscallbarringpluginrsc.rsg>      //for resource IDs
#include <gscommon.hrh>
#include <StringLoader.h>           //for StringLoader
#include <aknradiobuttonsettingpage.h>
#include <aknViewAppUi.h>
#include <aknnotewrappers.h>
#include <hlplch.h>                 // For HlpLauncher

// CONSTANS
const TInt KGSBarringGranularity = 2;

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// 
// C++ constructor.
// 
// ---------------------------------------------------------------------------
CGSSettListInternetBarringView::CGSSettListInternetBarringView()
    {
    }

// ---------------------------------------------------------------------------
// 
// Symbian OS two-phased constructor (second phase)
// 
// ---------------------------------------------------------------------------
void CGSSettListInternetBarringView::ConstructL()
    {
    BaseConstructL( R_GS_INTERNET_BARRING_VIEW );
    }

// ---------------------------------------------------------------------------
// CGSSettListInternetBarringView::NewL()
// Symbian OS two-phased constructor
// 
// ---------------------------------------------------------------------------
CGSSettListInternetBarringView* CGSSettListInternetBarringView::NewL()
    {
    CGSSettListInternetBarringView* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// 
// Symbian OS two-phased constructor (first phase)
// 
// ---------------------------------------------------------------------------
CGSSettListInternetBarringView* CGSSettListInternetBarringView::NewLC()
    {
    CGSSettListInternetBarringView* self = 
        new ( ELeave ) CGSSettListInternetBarringView();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }
 
// ---------------------------------------------------------------------------
// 
// C++ destructor
// 
// ---------------------------------------------------------------------------
CGSSettListInternetBarringView::~CGSSettListInternetBarringView()
    {
    }
 
// ---------------------------------------------------------------------------
// 
// Returns Id of the Barring view.
// 
// ---------------------------------------------------------------------------
TUid CGSSettListInternetBarringView::Id() const
    {
    return KCallInternetBarringViewId;
    }
 
// ---------------------------------------------------------------------------
// 
// Handles other than list box events.
// 
// ---------------------------------------------------------------------------
void CGSSettListInternetBarringView::HandleCommandL( TInt aCommand )
    {
    switch ( aCommand )
        {
       	case EGSMSKCmdAppChange:
			HandleListBoxSelectionL();
       		break;
       	case EGSCmdAppChange:
            ShowSettingPageL();
            break;
        case EAknSoftkeyBack:
            iAppUi->ActivateLocalViewL( KCallMainBarringViewId );
            break;
        case EAknCmdHelp:
            if( FeatureManager::FeatureSupported( KFeatureIdHelp ) )
                {
                HlpLauncher::LaunchHelpApplicationL(
                    iEikonEnv->WsSession(), iAppUi->AppHelpContextL() );
                }
            break;
        default:
            iAppUi->HandleCommandL( aCommand );
            break;
        }
    }
  
// ---------------------------------------------------------------------------
// 
// Activates view.
// 
// ---------------------------------------------------------------------------
void CGSSettListInternetBarringView::DoActivateL( const TVwsViewId& aPrevViewId,
                                          TUid aCustomMessageId,
                                          const TDesC8& aCustomMessage )
    {
    __GSLOGSTRING("[GS]--> CGSSettListInternetBarringView::DoActivateL");
    CGSLocalBaseView::DoActivateL( aPrevViewId, aCustomMessageId, 
                                   aCustomMessage );

    iContainer->iListBox->SetTopItemIndex( iTopItemIndex );

    if ( iCurrentItem >= 0 && 
         iCurrentItem < iContainer->iListBox->Model()->NumberOfItems() )
        {
        iContainer->iListBox->SetCurrentItemIndexAndDraw( iCurrentItem );
        }
    __GSLOGSTRING("[GS]<-- CGSSettListInternetBarringView::DoActivateL");
    }
 
// ---------------------------------------------------------------------------
// 
// Deactivates view.
// 
// ---------------------------------------------------------------------------
void CGSSettListInternetBarringView::DoDeactivate()
    {
    if ( iContainer )
        {
        if ( iContainer->iListBox && iContainer->iListBox->View() )
            {
            iCurrentItem = iContainer->iListBox->CurrentItemIndex();
            iTopItemIndex = iContainer->iListBox->TopItemIndex();
            }
        iAppUi->RemoveFromViewStack( *this, iContainer );
        delete iContainer;
        iContainer = NULL;
        }
    }
 
// ---------------------------------------------------------------------------
// 
// Before showing a options menu, delete options key specific settings if 
// rocker key has been pressed.
// ---------------------------------------------------------------------------
//
void CGSSettListInternetBarringView::DynInitMenuPaneL( TInt aResourceId, 
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

// ---------------------------------------------------------------------------
// 
// Creates new iContainer.
// 
// ---------------------------------------------------------------------------
void CGSSettListInternetBarringView::NewContainerL()
    {
    iContainer = new (ELeave) CGSSettListInternetBarringContainer;
    }

// ---------------------------------------------------------------------------
// 
// Handles changing of barring password.
// 
// ---------------------------------------------------------------------------
void CGSSettListInternetBarringView::HandleListBoxSelectionL()
    {
    CGSSettListInternetBarringContainer& container = 
        *static_cast<CGSSettListInternetBarringContainer*>( iContainer );
    
    TInt value( KErrNone );
    container.GetVoIPCallSettingValue( EGSVoIPBarring, value );
    
    if ( value == EGSCSCallBarringOff )
        {
        container.SetVoIPCallSettingValue( EGSVoIPBarring, EGSCSCallBarringOn );
        }
    else
        {
        container.SetVoIPCallSettingValue( EGSVoIPBarring, EGSCSCallBarringOff );
        }

    if ( iContainer->iListBox )
        {
        container.UpdateListBoxL();
        }
    
    // Show SCCP note if needed. 
    TInt status( KErrNone );
    container.GetSCCPStatus( status );
       
    if ( value == EGSCSCallBarringOff && status == 1 )
        {
        HBufC* prompt = 
            StringLoader::LoadLC( R_QTN_VOIP_SCCP_CALL_BARRING_NOTE );
        CAknInformationNote* note = new(ELeave) CAknInformationNote( ETrue );
        note->ExecuteLD( *prompt );
        CleanupStack::PopAndDestroy( prompt ); 
        }    
    }

// ---------------------------------------------------------------------------
// 
// Shows setting page - for ON/OFF setting items
// 
// ---------------------------------------------------------------------------
void CGSSettListInternetBarringView::ShowSettingPageL()
    {
    CGSSettListInternetBarringContainer& container = 
        *static_cast<CGSSettListInternetBarringContainer*>( iContainer );
    
    CDesCArrayFlat* items = 
          new ( ELeave ) CDesCArrayFlat( KGSBarringGranularity );
    CleanupStack::PushL( items );
    
    AppendItemL( *items, R_INTERNET_BARRING_ON );
    AppendItemL( *items, R_INTERNET_BARRING_OFF );
    
    TInt value( KErrNone );
    container.GetVoIPCallSettingValue( EGSVoIPBarring, value );
      
    CAknRadioButtonSettingPage* dlg = new( ELeave ) 
        CAknRadioButtonSettingPage( 
        R_GS_INTERNET_BARRING_SETTING_PAGE,
        value, items );
     
    if ( dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged ) )
        {
        if ( value == EGSCSCallBarringOff )
            {
            container.SetVoIPCallSettingValue( 
                EGSVoIPBarring, EGSCSCallBarringOff );
            }
        else
            {
            container.SetVoIPCallSettingValue( 
                EGSVoIPBarring, EGSCSCallBarringOn );
            }
        }
        
    CleanupStack::PopAndDestroy( items );
    
    container.UpdateListBoxL();
    }

// ---------------------------------------------------------------------------
// 
// Appends given item to given list
// 
// ---------------------------------------------------------------------------
void CGSSettListInternetBarringView::AppendItemL( 
    CDesCArrayFlat& aList, TInt aItem )
    {
    HBufC* string = StringLoader::LoadLC( aItem );
    aList.AppendL( *string );
    CleanupStack::PopAndDestroy( string );
    }    
        
 // End of file
