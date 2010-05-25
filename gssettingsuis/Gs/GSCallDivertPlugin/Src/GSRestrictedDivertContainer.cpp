/*
* Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Container for Restricted Call Divert's view.
*
*/


// INCLUDE FILES
#include "GSRestrictedDivertContainer.h"
#include "CallDivertPlugin.hrh"

#include <aknlists.h>
#include <gscalldivertpluginrsc.rsg>
#include <featmgr.h>
#include <csxhelp/cp.hlp.hrh>          // for help context of Divert
#include <gslistbox.h>
#include <gsfwviewuids.h>


// ========================= MEMBER FUNCTIONS ================================
// ---------------------------------------------------------------------------
// CGSRestrictedDivertContainer::ConstructL
// Symbian OS two phased constructor
//
// ---------------------------------------------------------------------------
void CGSRestrictedDivertContainer::ConstructL( const TRect& aRect )
    {
    iListBox = new ( ELeave ) CAknSettingStyleListBox;
    BaseConstructL( aRect, R_CF_MAIN_VIEW_TITLE, R_CF_RESTRICTED_MAIN_LBX );
    }

// ---------------------------------------------------------------------------
// CGSRestrictedDivertContainer::~CGSRestrictedDivertContainer
// Destructor
//
// ---------------------------------------------------------------------------
CGSRestrictedDivertContainer::~CGSRestrictedDivertContainer()
    {
    if ( iListboxItemArray )
        {
        delete iListboxItemArray;
        }
    }

// ---------------------------------------------------------------------------
// CGSRestrictedDivertContainer::GetHelpContext
// This function is called when Help application is launched.
// Only available, when __SERIES60_HELP is defined.
// ---------------------------------------------------------------------------
//
void CGSRestrictedDivertContainer::GetHelpContext(
    TCoeHelpContext& aContext ) const
    {
    aContext.iMajor = KUidGS;
    aContext.iContext = KDIV_HLP_CALL_FORW_MAIN;
    }


// ---------------------------------------------------------------------------
// CGSRestrictedDivertContainer::ConstructListBoxL
// Creates list box from resources.
//
// ---------------------------------------------------------------------------
//
void CGSRestrictedDivertContainer::ConstructListBoxL( TInt aResLbxId )
    {
    iListBox->ConstructL( this, EAknListBoxSelectionList );

    iListboxItemArray = CGSListBoxItemTextArray::NewL( aResLbxId,
        *iListBox, *iCoeEnv );
    iListBox->Model()->SetItemTextArray( iListboxItemArray );
    iListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );

    CreateListBoxItemsL();
    }

// ---------------------------------------------------------------------------
// CGSRestrictedDivertContainer::CreateListBoxItemsL
// Creates List box items
//
// ---------------------------------------------------------------------------
void CGSRestrictedDivertContainer::CreateListBoxItemsL()
    {
    MakeRestrictedDivertItemL();
    }

// ---------------------------------------------------------------------------
// CGSRestrictedDivertContainer::UpdateListBoxL
//
//
// ---------------------------------------------------------------------------
void CGSRestrictedDivertContainer::UpdateListBoxL( TInt aFeatureId )
    {
    switch( aFeatureId )
        {
        case EGSSettIdVoiceDivert:
            MakeRestrictedDivertItemL();
            break;
        default:
            break;
        }

    // Update the listbox
    iListBox->HandleItemAdditionL();
    }


// ---------------------------------------------------------------------------
// CGSRestrictedDivertContainer::CurrentFeatureId
//
//
// ---------------------------------------------------------------------------
TInt CGSRestrictedDivertContainer::CurrentFeatureId() const
    {
    return iListboxItemArray->CurrentFeature( );
    }


// ---------------------------------------------------------------------------
// CGSRestrictedDivertContainer::MakeVoiceDivertsItemL
//
//
// ---------------------------------------------------------------------------
void CGSRestrictedDivertContainer::MakeRestrictedDivertItemL()
    {
    iListboxItemArray->SetItemVisibilityL( EGSSettIdVoiceDivert,
        CGSListBoxItemTextArray::EVisible );
    }


//End of file
