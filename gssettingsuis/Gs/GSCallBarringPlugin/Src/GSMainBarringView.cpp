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
* Description:  View for the Main Barring folder
*
*/


// INCLUDE FILES
#include <e32base.h>
#include <featmgr.h>
#include <StringLoader.h>
#include <GSCallBarringPluginRsc.rsg>
#include "CallBarringPlugin.hrh"            //for enumerations (e.g.TGSBarringSetting)
#include "GSMainBarringView.h"      //for CGSSettListMainBarringView
#include "GSMainBarringContainer.h" //for CGSBaseContainer
#include "GSCellularCallBarringView.h"          //for CGSSettListBarringView
#include "GSInternetBarringView.h"  //for CGSSettListInternetBarringView
#include "GSPhoneSettingConstants.h"//for constant values
#include "LocalViewIds.h"           //for KSettListBarringViewId, KGSMainViewId
#include <eikmenup.h>               //for menu bar
#include "GSCallBarringPlugin.h"
#include <aknViewAppUi.h>
#include <gsfwviewuids.h>
#include <gscommon.hrh>
#include <aknlists.h>
#include <hlplch.h>                 // For HlpLauncher

// ========================= MEMBER FUNCTIONS ================================
// ---------------------------------------------------------------------------
//
// C++ constructor.
//
// ---------------------------------------------------------------------------
CGSSettListMainBarringView::CGSSettListMainBarringView()
    {
    }

// ---------------------------------------------------------------------------
//
// Symbian OS two-phased constructor (second phase)
//
// ---------------------------------------------------------------------------
void CGSSettListMainBarringView::ConstructL()
    {
    BaseConstructL( R_GS_BARRING_MAIN_VIEW );

    // Create call barring and Internet call barring views.
    iBarringView = CGSSettListBarringView::NewL();
    iAppUi->AddViewL( iBarringView );

    iInternetBarringView = CGSSettListInternetBarringView::NewL();
    iAppUi->AddViewL( iInternetBarringView );
    }

// ---------------------------------------------------------------------------
//
// Symbian OS two-phased constructor (first phase)
//
// ---------------------------------------------------------------------------
CGSSettListMainBarringView* CGSSettListMainBarringView::NewLC()
    {
    CGSSettListMainBarringView* self =
        new ( ELeave ) CGSSettListMainBarringView;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
//
// C++ destructor
//
// ---------------------------------------------------------------------------
CGSSettListMainBarringView::~CGSSettListMainBarringView()
    {
    }

// ---------------------------------------------------------------------------
//
// Returns Id of the Barring view.
//
// ---------------------------------------------------------------------------
TUid CGSSettListMainBarringView::Id() const
    {
    return KCallMainBarringViewId;
    }

// ---------------------------------------------------------------------------
//
// Handles other than list box events.
//
// ---------------------------------------------------------------------------
void CGSSettListMainBarringView::HandleCommandL( TInt aCommand )
    {
    iCurrentItem = iContainer->iListBox->CurrentItemIndex();

    switch ( aCommand )
        {
        case EAknSoftkeyOpen:
		case EGSCmdAppOpen:
            HandleListBoxSelectionL();
            break;
        case EAknSoftkeyBack:
            iAppUi->ActivateLocalViewL( KGSTelPluginUid );
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
void CGSSettListMainBarringView::DoActivateL( const TVwsViewId& aPrevViewId,
                                          TUid aCustomMessageId,
                                          const TDesC8& aCustomMessage )
    {
    CGSLocalBaseView::DoActivateL( aPrevViewId, aCustomMessageId,
                                   aCustomMessage );

    iContainer->iListBox->SetTopItemIndex( iTopItemIndex );

    if ( iCurrentItem >= 0 &&
         iCurrentItem < iContainer->iListBox->Model()->NumberOfItems() )
        {
        iContainer->iListBox->SetCurrentItemIndexAndDraw( iCurrentItem );
        }
    }

// ---------------------------------------------------------------------------
//
// Deactivates view.
//
// ---------------------------------------------------------------------------
void CGSSettListMainBarringView::DoDeactivate()
    {
    if ( iContainer )
        {
        iCurrentItem = iContainer->iListBox->CurrentItemIndex();
        iTopItemIndex = iContainer->iListBox->TopItemIndex();

        iAppUi->RemoveFromViewStack( *this, iContainer );
        delete iContainer;
        iContainer = NULL;
        }
    }

// -----------------------------------------------------------------------------
// CGSSettListMainBarringView::ProcessCommandL
// -----------------------------------------------------------------------------
void CGSSettListMainBarringView::ProcessCommandL( TInt aCommand )
    {
    MenuBar()->SetContextMenuTitleResourceId(R_CALL_BARRING_MSK_CONTEXTMENUBAR);
    // Call base class
    CAknView::ProcessCommandL( aCommand );
    }
// ---------------------------------------------------------------------------
//
// Before showing a options menu, delete options key specific settings if
// rocker key has been pressed.
// ---------------------------------------------------------------------------
//
void CGSSettListMainBarringView::DynInitMenuPaneL( TInt aResourceId,
                                                   CEikMenuPane* aMenuPane )
    {
    // show or hide the 'help' menu item when supported
    if( aResourceId == R_GS_MENU_ITEM_HELP )
        {
        User::LeaveIfNull( aMenuPane );
            
        //Handle Help Fature
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
void CGSSettListMainBarringView::NewContainerL()
    {
    iContainer = new ( ELeave ) CGSSettListMainBarringContainer;
    }


// ---------------------------------------------------------------------------
//
// Handles changing of barring password.
//
// ---------------------------------------------------------------------------
void CGSSettListMainBarringView::HandleListBoxSelectionL()
    {
    const TInt currentItem = Container()->CurrentFeatureId();

    switch ( currentItem )
        {
        case EGSCBCellular:
            iAppUi->ActivateLocalViewL( KCallBarringViewId );
            break;
        case EGSCBInternet:
            iAppUi->ActivateLocalViewL( KCallInternetBarringViewId );
            break;
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// CGSSettListMainBarringView::Container
//
// Returns call container item
// ---------------------------------------------------------------------------
//
CGSSettListMainBarringContainer* CGSSettListMainBarringView::Container()
    {
    return static_cast <CGSSettListMainBarringContainer*> ( iContainer );
    }

// End of File

