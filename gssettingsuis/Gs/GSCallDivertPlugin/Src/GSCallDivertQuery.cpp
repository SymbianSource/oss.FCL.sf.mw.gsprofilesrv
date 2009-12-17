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
* Description:  Phone number query for Call Divert view.
*
*/


// INCLUDE FILES
#include    "GSCallDivertQuery.h"

// CONSTANTS
const TInt KCFQueryClosing = 1;
const TInt KCFQueryRunning = 0;

// ================= MEMBER FUNCTIONS =======================
// ---------------------------------------------------------
// GSCallDivertQuery::NewL
// ---------------------------------------------------------
CGSCallDivertQuery* CGSCallDivertQuery::NewL( TDes& aDataText,
    const TTone& aTone, TInt aOKCba, TInt aDefaultCba )
    {
    CGSCallDivertQuery* self =
        new ( ELeave ) CGSCallDivertQuery(
            aDataText, aTone, aOKCba, aDefaultCba );
    return self;
    }

// ---------------------------------------------------------
// GSCallDivertQuery::GSCallDivertQuery
// ---------------------------------------------------------
CGSCallDivertQuery::CGSCallDivertQuery(
        TDes& aDataText,
        const TTone& aTone,
        TInt aOKCba,
        TInt aDefaultCba )
    : CAknTextQueryDialog( aDataText, aTone ),
      iOKCba( aOKCba ),
      iDefaultCba( aDefaultCba )
    {
    }

// ---------------------------------------------------------
// GSCallDivertQuery::OfferKeyEventL
// ---------------------------------------------------------
TKeyResponse CGSCallDivertQuery::OfferKeyEventL( const TKeyEvent& aKeyEvent,
        TEventCode aType )
    {
    if ( aKeyEvent.iCode == EKeyEnter )
        {
        CAknQueryControl* control = 
            static_cast<CAknQueryControl*> ( Control( EGeneralQuery ) );
        if ( control->GetTextLength() )
            {
            TryExitL( EAknSoftkeyOk );
            }
        else
            {
            TryExitL( EAknSoftkeySearch );
            }            
        
        return EKeyWasConsumed;
        }
    
    CAknTextQueryDialog::OfferKeyEventL( aKeyEvent, aType );

    if ( aKeyEvent.iCode != EKeyEscape )
        {
        UpdateSoftkeysL();
        }

    return EKeyWasConsumed;
    }


// ---------------------------------------------------------
// GSCallDivertQuery::OfferKeyEventL
// ---------------------------------------------------------
TBool CGSCallDivertQuery::NeedToDismissQueryL( const TKeyEvent& aKeyEvent )
    {
    if ( aKeyEvent.iCode == EKeyOK )
        {
        CAknQueryControl* control =
            static_cast<CAknQueryControl*> ( Control( EGeneralQuery ) );
        // check if the editor is empty or has some text
        if ( control && control->GetTextLength() )
            {
            return EFalse;
            }
        else
            {
            return ETrue;
            }
        }
    else
        {
        return EFalse;
        }
    }


// ---------------------------------------------------------
// GSCallDivertQuery::PostLayoutDynInitL
// ---------------------------------------------------------
void CGSCallDivertQuery::PostLayoutDynInitL()
    {
    iCurrentCba = 0;
    iLeaving = KCFQueryRunning;
    CAknTextQueryDialog::PostLayoutDynInitL();
    UpdateSoftkeysL();
    }

// ---------------------------------------------------------
// GSCallDivertQuery::UpdateSoftkeysL
// ---------------------------------------------------------
void CGSCallDivertQuery::UpdateSoftkeysL()
    {
    if ( iLeaving != KCFQueryRunning )
        {
        return;
        }
    CAknQueryControl* control =
        static_cast<CAknQueryControl*> ( Control( EGeneralQuery ) );
    if ( control && control->GetTextLength() )
        {
        // There is text in cba; we can update softkeys to OK - Cancel.
        SetCbaL( iOKCba );
        }
    else
        {
        // There is no text in cba; update softkeys to Search - Back.
        SetCbaL( iDefaultCba );
        }
    }

// ---------------------------------------------------------
// GSCallDivertQuery::OkToExitL
// ---------------------------------------------------------
TBool CGSCallDivertQuery::OkToExitL( TInt aButtonId )
    {

    if ( aButtonId == EAknSoftkeySearch )
        {
        return ETrue;
        }
    if ( aButtonId == EAknSoftkeyCancel )
        {
        iLeaving = KCFQueryClosing;
        return ETrue;
        }
    if ( aButtonId == EAknSoftkeyBack )
        {
        iLeaving = KCFQueryClosing;
        return ETrue;
        }
    iLeaving = KCFQueryClosing;
    return CAknTextQueryDialog::OkToExitL( aButtonId );
    }


// ---------------------------------------------------------
// GSCallDivertQuery::SetCba
// ---------------------------------------------------------
void CGSCallDivertQuery::SetCbaL( TInt aCba )
    {
    if ( iCurrentCba != aCba )
        {
        CEikButtonGroupContainer& buttonGroup = ButtonGroupContainer();
        buttonGroup.SetCommandSetL( aCba );
        buttonGroup.DrawNow();
        iCurrentCba = aCba;
        }
    MakeLeftSoftkeyVisible( ETrue );
    }


// ---------------------------------------------------------
// GSCallDivertQuery::PreLayoutDynInitL
// ---------------------------------------------------------
void CGSCallDivertQuery::PreLayoutDynInitL()
    {
    iCurrentCba = 0;
    CAknTextQueryDialog::PreLayoutDynInitL();

    if( this->Text().Length() )
        {
        MakeLeftSoftkeyVisible( ETrue );
        }
    }


// ---------------------------------------------------------
// GSCallDivertQuery::HandleQueryEditorStateEventL
// ---------------------------------------------------------
TBool CGSCallDivertQuery::HandleQueryEditorStateEventL(
    CAknQueryControl* aQueryControl,
    TQueryControlEvent aEventType,
    TQueryValidationStatus aStatus )
    {
    CAknTextQueryDialog::HandleQueryEditorStateEventL( aQueryControl,
                                                       aEventType,
                                                       aStatus );
    if( aEventType == EQueryControlEditorStateChanging &&
        aStatus == EEditorValueValid )
        {
        UpdateSoftkeysL();
        }
    return EFalse;
    }


//  End of File
