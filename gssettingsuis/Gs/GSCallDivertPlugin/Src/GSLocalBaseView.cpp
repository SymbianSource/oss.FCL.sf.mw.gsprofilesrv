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
* Description:  Base view for all the views in General Settings
*
*/


// INCLUDE FILES
#include    "GSLocalBaseView.h"

#include <aknViewAppUi.h>
#include <akntitle.h>
#include <barsread.h>
#include <aknnavi.h>
#include <akntabgrp.h>
#include <aknnavide.h>
#include <AknUtils.h>
#include <bldvariant.hrh>
#include <gsbasecontainer.h>
#include <gstelpluginrsc.rsg>

//Disabling warning caused by KGSDoActivateError constant
#pragma diag_suppress 177
// CONSTANTS
_LIT( KGSDoActivateError, "DoActivateL" );

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSLocalBaseView::CGSLocalBaseView
//
// C++ constructor
// ---------------------------------------------------------------------------
//
CGSLocalBaseView::CGSLocalBaseView()
    {
    iAppUi = iAvkonViewAppUi;
    }


// ---------------------------------------------------------------------------
// CGSLocalBaseView::~CGSLocalBaseView
//
// Destructor
// ---------------------------------------------------------------------------
//
CGSLocalBaseView::~CGSLocalBaseView()
    {
    if ( iContainer && iAppUi )
        {
        iAppUi->RemoveFromViewStack( *this, iContainer );
        delete iContainer;
        }
    }


// ---------------------------------------------------------------------------
// CGSLocalBaseView::SetCurrentItem
//
//
// ---------------------------------------------------------------------------
//
void CGSLocalBaseView::SetCurrentItem( TInt aIndex )
    {
    iCurrentItem = aIndex;
    if( iContainer )
        {
        iContainer->SetSelectedItem( aIndex );
        }
    }


// ---------------------------------------------------------------------------
// CGSLocalBaseView::HandleListBoxEventL
//
//
// ---------------------------------------------------------------------------
//
void CGSLocalBaseView::HandleListBoxEventL( CEikListBox* /*aListBox*/,
                                            TListBoxEvent aEventType )
    {
    switch ( aEventType )
        {
        case EEventEnterKeyPressed:
        case EEventItemSingleClicked:
            HandleListBoxSelectionL();
            break;
        default:
           break;
        }
    }


// ---------------------------------------------------------------------------
// CGSLocalBaseView::SetNaviPaneL
//
//
// ---------------------------------------------------------------------------
//
void CGSLocalBaseView::SetNaviPaneL()
    {
    }


// ---------------------------------------------------------------------------
// CGSLocalBaseView::CreateNaviPaneContextL
//
//
// ---------------------------------------------------------------------------
//
void CGSLocalBaseView::CreateNaviPaneContextL( TInt /*aResourceId*/ )
    {
    }


// ---------------------------------------------------------------------------
// CGSLocalBaseView::HandleClientRectChange
//
//
// ---------------------------------------------------------------------------
//
void CGSLocalBaseView::HandleClientRectChange()
    {
    if ( iContainer && iContainer->iListBox )
        {
        iContainer->SetRect( ClientRect() );
        }
    }


// ---------------------------------------------------------------------------
// CGSLocalBaseView::DoActivateL
// Activates the view.
//
// ---------------------------------------------------------------------------
//
void CGSLocalBaseView::DoActivateL(const TVwsViewId& /*aPrevViewId*/,
                              TUid /*aCustomMessageId*/,
                              const TDesC8& /*aCustomMessage*/)
    {
    if( iContainer )
        {
        iAppUi->RemoveFromViewStack( *this, iContainer );
        delete iContainer;
        iContainer = NULL;
        }

    CreateContainerL();

    iAppUi->AddToViewStackL( *this, iContainer );
    iContainer->iListBox->SetListBoxObserver( this );

    SetNaviPaneL();
    }


// ---------------------------------------------------------------------------
// CGSLocalBaseView::DoDeactivate()
//
//
// ---------------------------------------------------------------------------
//
void CGSLocalBaseView::DoDeactivate()
    {
    if ( iContainer )
        {
        iAppUi->RemoveFromViewStack( *this, iContainer );
        delete iContainer;
        iContainer = NULL;
        }
    }


// ---------------------------------------------------------------------------
// CGSLocalBaseView::CreateContainerL()
//
//
// ---------------------------------------------------------------------------
//
void CGSLocalBaseView::CreateContainerL()
    {
    NewContainerL();
    __ASSERT_DEBUG( iContainer,
        User::Panic( KGSDoActivateError, EGSViewPanicNullPtr ) );
    iContainer->SetMopParent( this );

    TRAPD( error, iContainer->ConstructL( ClientRect() ) );

    if ( error )
        {
        delete iContainer;
        iContainer = NULL;
        User::Leave( error );
        }
    }


//End of File
