/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: 
*        GS plugin wrapper.
*
*/

#include "GSPlaceholderView.h"

// System includes
#include <coeaui.h>

// User includes
#include <gsfwviewuids.h>

// Constants
const TUid KGSInternalPlaceholderViewUid = { 0x0DEADBED };


// ================= MEMBER FUNCTIONS =======================

// ----------------------------------------------------------------------------
// CGSPlaceholderView::CGSPlaceholderView
//
// 
// ----------------------------------------------------------------------------
//
CGSPlaceholderView::CGSPlaceholderView( CCoeAppUi& aAppUi )
:   iAppUi( aAppUi )
    {
    }


// ----------------------------------------------------------------------------
// CGSPlaceholderView::~CGSPlaceholderView
//
// 
// ----------------------------------------------------------------------------
//
CGSPlaceholderView::~CGSPlaceholderView()
    {
    iAppUi.DeregisterView( *this );
    }


// ----------------------------------------------------------------------------
// CGSPlaceholderView::ConstructL
//
// 
// ----------------------------------------------------------------------------
//
void CGSPlaceholderView::ConstructL()
    {
    iAppUi.RegisterViewL( *this );
    }


// ----------------------------------------------------------------------------
// CGSPlaceholderView::NewL
//
// 
// ----------------------------------------------------------------------------
//
CGSPlaceholderView* CGSPlaceholderView::NewL( CCoeAppUi& aAppUi )
    {
    CGSPlaceholderView* self = new(ELeave) CGSPlaceholderView( aAppUi );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ----------------------------------------------------------------------------
// CGSPlaceholderView::ActivateL
//
// 
// ----------------------------------------------------------------------------
//
void CGSPlaceholderView::ActivateL()
    {
    iAppUi.ActivateViewL( ViewId() );
    }


// ----------------------------------------------------------------------------
// CGSPlaceholderView::PreviouslyActiveViewId
//
// 
// ----------------------------------------------------------------------------
//
const TVwsViewId& CGSPlaceholderView::PreviouslyActiveViewId() const
    {
    return iPreviouslyActiveViewId;
    }


// ---------------------------------------------------------------------------
// CGSPlaceholderView::SetPreviouslyActiveViewId
// 
//
// ---------------------------------------------------------------------------
//
void CGSPlaceholderView::SetPreviouslyActiveViewId( const TVwsViewId& aViewId )
    {
    iPreviouslyActiveViewId = aViewId;
    }


// ----------------------------------------------------------------------------
// CGSPlaceholderView::ViewId
//
// 
// ----------------------------------------------------------------------------
//
TVwsViewId CGSPlaceholderView::ViewId() const
    {
    const TVwsViewId ret( KUidGS, KGSInternalPlaceholderViewUid );
    return ret;
    }


// ----------------------------------------------------------------------------
// CGSPlaceholderView::ViewActivatedL
//
// 
// ----------------------------------------------------------------------------
//
void CGSPlaceholderView::ViewActivatedL( const TVwsViewId& /*aPrevViewId*/, TUid /*aCustomMessageId*/, const TDesC8& /*aCustomMessage*/ )
    {
    // NB: aPrevViewId will be the id of the plugin we are trying to load...
    }


// ----------------------------------------------------------------------------
// CGSPlaceholderView::ViewDeactivated
//
// 
// ----------------------------------------------------------------------------
//
void CGSPlaceholderView::ViewDeactivated()
    {
    }

//  End of File
