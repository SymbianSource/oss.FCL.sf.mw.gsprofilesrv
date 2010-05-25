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
* Description:  Container for Call Divert's view.
*
*/


// INCLUDE FILES
#include "GSCallDivertContainer.h"
#include "CallDivertPlugin.hrh"

#include <aknlists.h>
#include <gscalldivertpluginrsc.rsg>
#include <gslistbox.h>
#include <csxhelp/cp.hlp.hrh>        // for help context of Divert
#include <gsfwviewuids.h>     // for KUidGS

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSCallDivertContainer::CGSCallDivertContainer
//
// C++ constructor
// ---------------------------------------------------------------------------
CGSCallDivertContainer::CGSCallDivertContainer( TGSCallDivertServiceGroups aItemId )
    : iItemId( aItemId )
    {
    }

// ---------------------------------------------------------------------------
// CGSCallDivertContainer::ConstructL
//
// Symbian OS two phased constructor
// ---------------------------------------------------------------------------
void CGSCallDivertContainer::ConstructL( const TRect& aRect )
    {
    iListBox = new ( ELeave ) CAknSettingStyleListBox;
    switch( iItemId )
        {
        case EGSSettIdVoiceDivert:
            BaseConstructL( aRect, R_CF_VOICE_VIEW_TITLE, R_CF_DIV_LBX );
            break;
        case EGSSettIdVideoDivert:
            BaseConstructL( aRect, R_CF_VIDEO_VIEW_TITLE, R_CF_DIV_LBX );
            break;
        default:
            break;
        }

    }


// ---------------------------------------------------------------------------
// CGSCallDivertContainer::~CGSCallDivertContainer
//
// Destructor
// ---------------------------------------------------------------------------
CGSCallDivertContainer::~CGSCallDivertContainer()
    {
    if ( iListboxItemArray )
        {
        delete iListboxItemArray;
        }
    }

// ---------------------------------------------------------------------------
// CGSCallDivertContainer::GetHelpContext
//
// This function is called when Help application is launched.
// Only available, when __SERIES60_HELP is defined.
// ---------------------------------------------------------------------------
void CGSCallDivertContainer::GetHelpContext(
    TCoeHelpContext& aContext ) const
    {
    aContext.iMajor = KUidGS;

    switch( iItemId )
        {
        case EGSSettIdVoiceDivert:
            aContext.iContext = KDIV_HLP_CALL_FORW_VOI;
            break;
        case EGSSettIdVideoDivert:
            aContext.iContext = KDIV_HLP_CALL_FORW_DAT;
            break;
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// CGSCallDivertContainer::ConstructListBoxL
// Creates list box from resources.
//
// ---------------------------------------------------------------------------
//
void CGSCallDivertContainer::ConstructListBoxL( TInt aResLbxId )
    {
    iListBox->ConstructL(this, EAknListBoxSelectionList);

    iListboxItemArray = CGSListBoxItemTextArray::NewL( aResLbxId,
        *iListBox, *iCoeEnv );
    iListBox->Model()->SetItemTextArray( iListboxItemArray );
    iListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );

    CreateListBoxItemsL();
    }

// ---------------------------------------------------------------------------
// CGSCallDivertContainer::CreateListBoxItemsL
//
// Creates List box items
// ---------------------------------------------------------------------------
void CGSCallDivertContainer::CreateListBoxItemsL()
    {
    if ( iItemId == EGSSettIdVoiceDivert )
        {
        MakeAllVoiceCallDivertsItemL();
        }
    else if ( iItemId == EGSSettIdVideoDivert )
        {
        MakeAllVideoCallDivertsItemL();
        }

    MakeIfBusyItemL();
    MakeIfNotAnsweredItemL();
    MakeIfOutOfReachItemL();
    MakeIfNotAvailableItemL();
    }


// ---------------------------------------------------------------------------
// CGSCallDivertContainer::MakeAllVoiceCallDivertsItemL
//
// Create voice call diverts item
// ---------------------------------------------------------------------------
void CGSCallDivertContainer::MakeAllVoiceCallDivertsItemL()
    {
    iListboxItemArray->SetItemVisibilityL( ECFDivertVoiceCalls,
        CGSListBoxItemTextArray::EVisible );
    }


// ---------------------------------------------------------------------------
// CGSCallDivertContainer::MakeAllVideoCallDivertsItemL
//
// Create data & video call diverts item
// ---------------------------------------------------------------------------
void CGSCallDivertContainer::MakeAllVideoCallDivertsItemL()
    {
    iListboxItemArray->SetItemVisibilityL( ECFDivertVideoCalls,
        CGSListBoxItemTextArray::EVisible );
    }


// ---------------------------------------------------------------------------
// CGSCallDivertContainer::MakeAllVoiceCallDivertsItemL
//
// Create if_busy item
// ---------------------------------------------------------------------------
void CGSCallDivertContainer::MakeIfBusyItemL()
    {
    iListboxItemArray->SetItemVisibilityL( ECFDivertWhenBusy,
        CGSListBoxItemTextArray::EVisible );
    }


// ---------------------------------------------------------------------------
// CGSCallDivertContainer::MakeAllVoiceCallDivertsItemL
//
// Create if_not_answered item
// ---------------------------------------------------------------------------
void CGSCallDivertContainer::MakeIfNotAnsweredItemL()
    {
    iListboxItemArray->SetItemVisibilityL( ECFDivertWhenNoAnswer,
        CGSListBoxItemTextArray::EVisible );
    }


// ---------------------------------------------------------------------------
// CGSCallDivertContainer::MakeAllVoiceCallDivertsItemL
//
// Create if_out_of_reach item
// ---------------------------------------------------------------------------
void CGSCallDivertContainer::MakeIfOutOfReachItemL()
    {
    iListboxItemArray->SetItemVisibilityL( ECFDivertWhenOutOfReach,
        CGSListBoxItemTextArray::EVisible );
    }


// ---------------------------------------------------------------------------
// CGSCallDivertContainer::MakeIfNotAvailableItemL
//
// Create if_not_available item
// ---------------------------------------------------------------------------
void CGSCallDivertContainer::MakeIfNotAvailableItemL()
    {
    iListboxItemArray->SetItemVisibilityL( ECFDivertWhenNotAvailable,
        CGSListBoxItemTextArray::EVisible );
    }

// ---------------------------------------------------------------------------
// CGSCallDivertContainer::CurrentFeatureId
//
//
// ---------------------------------------------------------------------------
TInt CGSCallDivertContainer::CurrentFeatureId() const
    {
    return iListboxItemArray->CurrentFeature( );
    }


//End of File
