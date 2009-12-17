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
* Description:  Time query for setting Autokeyguard period.
*           
*           
*           
*
*/



// INCLUDE FILES
#include "GSAutoKeyguardPluginAutoKeyguardTimeQuery.h"

// ========================= MEMBER FUNCTIONS ================================

// -----------------------------------------------------------------------------
// CAutoKeyguardTimeQuery::CAutoKeyguardTimeQuery
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAutoKeyguardTimeQuery::CAutoKeyguardTimeQuery( TTime& aTime, const TTone aTone ) :
    CAknTimeQueryDialog( aTime, aTone )
    {
    }

// Destructor
CAutoKeyguardTimeQuery::~CAutoKeyguardTimeQuery()
    {
    }

// -----------------------------------------------------------------------------
// CAutoKeyguardTimeQuery::OfferKeyEventL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TKeyResponse CAutoKeyguardTimeQuery::OfferKeyEventL(const TKeyEvent& aKeyEvent, 
                                                            TEventCode aType)
    {
    if( aType != EEventKey )
        return EKeyWasNotConsumed;

    if( NeedToDismissQueryL( aKeyEvent ) )
            return EKeyWasConsumed;
    return CAknDialog::OfferKeyEventL(aKeyEvent,aType);
    }

// -----------------------------------------------------------------------------
// CAutoKeyguardTimeQuery::NeedToDismissQueryL
// Allows user to acknowledge time and date queries with the send key.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAutoKeyguardTimeQuery::NeedToDismissQueryL(const TKeyEvent& aKeyEvent)
    {
    if (aKeyEvent.iCode == EKeyPhoneSend)
        {
        DismissQueryL();
        return ETrue;
        }
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CAutoKeyguardTimeQuery::DismissQueryL
// Query is accepted if the left softkey is displayed 
// (left softkey is displayed only if there is valid data in the query).
// Query is discarded if the left softkey is not displayed.
// Clients can override this and implement something different.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAutoKeyguardTimeQuery::DismissQueryL()
    {
    if (IsLeftSoftkeyVisible())
        {
        TryExitL(EEikBidOk);
        }
    }

// -----------------------------------------------------------------------------
// CAutoKeyguardTimeQuery::IsLeftSoftkeyVisible
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAutoKeyguardTimeQuery::IsLeftSoftkeyVisible()
    {
    return ButtonGroupContainer().ButtonGroup()->IsCommandVisible(
                      ButtonGroupContainer().ButtonGroup()->CommandId(0));
    }

// End of file
