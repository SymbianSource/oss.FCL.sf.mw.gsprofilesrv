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
* Description: 
*        Base container for GS some views.
*
*/


// INCLUDE FILES
#include    <gsbasecontainer.h>

#include    <akncontext.h>
#include    <aknlists.h>
#include    <aknnavi.h>
#include    <akntitle.h>
#include    <AknUtils.h>
#include    <avkon.hrh>
#include    <avkon.mbg>
#include    <avkon.rsg>
#include    <barsread.h>
#include    <calslbs.h>
#include    <e32def.h>
#include    <eikbtgpc.h>
#include    <eikclbd.h>
#include    <eikenv.h>
#include    <eikfrlbd.h>
#include    <eiklbx.h>
#include    <eikslb.h>
#include    <gulicon.h>

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CGSBaseContainer::BaseConstructL
//
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CGSBaseContainer::BaseConstructL( const TRect& aRect,
                                                TInt aResTitleId,
                                                TInt aResLbxId )
    {
    CEikStatusPane* sp = iAvkonAppUi->StatusPane();

    CAknTitlePane* title =
        static_cast<CAknTitlePane*> (
        sp->ControlL( TUid::Uid( EEikStatusPaneUidTitle ) ) );

    TResourceReader rReader;
    iCoeEnv->CreateResourceReaderLC( rReader, aResTitleId );
    title->SetFromResourceL( rReader );
    CleanupStack::PopAndDestroy(); // rReader


    CreateWindowL(); // Makes the control a window-owning control

    // Set iListBox to be contained in this container:
    iListBox->SetContainerWindowL( *this );

    ConstructListBoxL( aResLbxId );

    iListBox->CreateScrollBarFrameL( ETrue );
    iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(
        CEikScrollBarFrame::EOff,
        CEikScrollBarFrame::EAuto );

    SetRect( aRect );
    ActivateL();
    }


// ---------------------------------------------------------------------------
// CGSBaseContainer::CGSBaseContainer
// Constructor
//
// ---------------------------------------------------------------------------
//
EXPORT_C CGSBaseContainer::CGSBaseContainer()
    {
    iElaf = ( AknLayoutUtils::Variant() == EEuropeanVariant );
    }


// ---------------------------------------------------------------------------
// CGSBaseContainer::~CGSBaseContainer
// Destructor
//
// ---------------------------------------------------------------------------
//
EXPORT_C CGSBaseContainer::~CGSBaseContainer()
    {
    if ( iListBox )
        {
        delete iListBox;
        }
    }


// ---------------------------------------------------------------------------
// CGSBaseContainer::ConstructListBoxL
//
//  Construct's ListBox from Resource ID
// ---------------------------------------------------------------------------
//
void CGSBaseContainer::ConstructListBoxL( TInt aResLbxId )
    {
    TResourceReader rReader;
    iCoeEnv->CreateResourceReaderLC( rReader, aResLbxId );

    // Construct's iListBox from resource file
    iListBox->ConstructFromResourceL( rReader );
    CleanupStack::PopAndDestroy(); // rReader
    }


// ---------------------------------------------------------------------------
// CGSBaseContainer::SizeChanged
// called by framwork when the view size is changed
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CGSBaseContainer::SizeChanged()
    {
    if ( iListBox )
        {
        iListBox->SetRect( Rect() );
        }
    }


// ---------------------------------------------------------------------------
// CGSBaseContainer::CountComponentControls
//
//
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CGSBaseContainer::CountComponentControls() const
    {
    return 1;
    }


// ---------------------------------------------------------------------------
// CGSBaseContainer::ComponentControl
//
//
// ---------------------------------------------------------------------------
//
EXPORT_C CCoeControl* CGSBaseContainer::ComponentControl( TInt /*aIndex*/ )
    const
    {
    return iListBox;
    }


// ---------------------------------------------------------------------------
// CGSMainContainer::TKeyResponse OfferKeyEventL
// Called when a key is pressed.
//
// ---------------------------------------------------------------------------

EXPORT_C TKeyResponse CGSBaseContainer::OfferKeyEventL(
    const TKeyEvent& aKeyEvent,
    TEventCode aType )
    {
    switch ( aKeyEvent.iCode )
        {
        case EKeyLeftArrow:
        case EKeyRightArrow:
            // Listbox takes all event even if it doesn't use them:
            return EKeyWasNotConsumed;
        default:
            break;
        }

    // Now it's iListBox's job to process the key event
    return iListBox->OfferKeyEventL( aKeyEvent, aType );
    }


// ---------------------------------------------------------------------------
// CGSMainContainer::HandleResourceChange
//
// ---------------------------------------------------------------------------
EXPORT_C void CGSBaseContainer::HandleResourceChange( TInt aType )
    {
    CCoeControl::HandleResourceChange( aType );
    if ( aType == KAknsMessageSkinChange ||
         aType == KEikDynamicLayoutVariantSwitch )
        {
        TRect mainPaneRect;
        AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EMainPane,
                                           mainPaneRect);
        SetRect( mainPaneRect );
        }
    }


// -----------------------------------------------------------------------------
// CGSBaseContainer::SetSelectedItem()
//
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CGSBaseContainer::SetSelectedItem( TInt aIndex )
    {
    if( iListBox && aIndex >= 0 )
        {
        iListBox->SetCurrentItemIndexAndDraw( aIndex );
        }
    }


// -----------------------------------------------------------------------------
// CGSBaseContainer::SelectedItem()
//
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CGSBaseContainer::SelectedItem()
    {
    TInt selectedItem = 0;
    if( iListBox )
        {
        selectedItem = iListBox->CurrentItemIndex();
        }
    return selectedItem;
    }


// ---------------------------------------------------------------------------
// CGSContainer::FocusChanged
//
// Set focus on the selected listbox. For animated skins feature.
// ---------------------------------------------------------------------------
EXPORT_C void CGSBaseContainer::FocusChanged( TDrawNow /*aDrawNow*/ )
    {
    if( iListBox )
        {
        iListBox->SetFocus( IsFocused() );
        }
    }

//  End of File
