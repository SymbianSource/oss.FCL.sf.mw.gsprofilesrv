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
* Description:  Container for Call Divert's view.
*
*/


// INCLUDE FILES
#include "CallDivertPlugin.hrh"
#include "GSCallDivertPluginContainer.h"

#include <aknlists.h>
#include <GSCallDivertPluginRsc.rsg>
#include <featmgr.h>
#include <csxhelp/cp.hlp.hrh>          // for help context of Divert
#include <gsfwviewuids.h>       // for KUidGS
#include <gslistbox.h>

// ========================= MEMBER FUNCTIONS ================================
// ---------------------------------------------------------------------------
// CGSCallDivertPluginContainer::ConstructL
// Symbian OS two phased constructor
//
// ---------------------------------------------------------------------------
void CGSCallDivertPluginContainer::ConstructL( const TRect& aRect )
    {
    iListBox = new ( ELeave ) CAknSettingStyleListBox;
    BaseConstructL( aRect, R_CF_MAIN_VIEW_TITLE, R_CF_MAIN_LBX );
    }

// ---------------------------------------------------------------------------
// CGSCallDivertPluginContainer::~CGSCallDivertPluginContainer
// Destructor
//
// ---------------------------------------------------------------------------
CGSCallDivertPluginContainer::~CGSCallDivertPluginContainer()
    {
    if ( iListboxItemArray )
        {
        delete iListboxItemArray;
        }
    }

// ---------------------------------------------------------------------------
// CGSCallDivertPluginContainer::GetHelpContext
// This function is called when Help application is launched.
// Only available, when __SERIES60_HELP is defined.
// ---------------------------------------------------------------------------
//
void CGSCallDivertPluginContainer::GetHelpContext(
    TCoeHelpContext& aContext ) const
    {
    aContext.iMajor = KUidGS;
    aContext.iContext = KDIV_HLP_CALL_FORW_MAIN;
    }

// ---------------------------------------------------------------------------
// CGSCallDivertPluginContainer::ConstructListBoxL
// Creates list box from resources.
//
// ---------------------------------------------------------------------------
//
void CGSCallDivertPluginContainer::ConstructListBoxL( TInt aResLbxId )
    {
    iListBox->ConstructL( this, EAknListBoxSelectionList );

    iListboxItemArray = CGSListBoxItemTextArray::NewL( aResLbxId,
        *iListBox, *iCoeEnv );
    iListBox->Model()->SetItemTextArray( iListboxItemArray );
    iListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );

    MakeVoiceDivertsItemL();
    MakeVideoDivertsItemL();
    }

// ---------------------------------------------------------------------------
// CGSCallDivertPluginContainer::UpdateListBoxL
//
//
// ---------------------------------------------------------------------------
void CGSCallDivertPluginContainer::UpdateListBoxL( TInt aFeatureId )
    {
    switch( aFeatureId )
        {
        case EGSSettIdVoiceDivert:
            MakeVoiceDivertsItemL();
            break;
        case EGSSettIdVideoDivert:
            MakeVideoDivertsItemL();
            break;
        default:
            break;
        }

    // Update the listbox
    iListBox->HandleItemAdditionL();
    }

// ---------------------------------------------------------------------------
// CGSCallDivertPluginContainer::CurrentFeatureId
//
//
// ---------------------------------------------------------------------------
TGSCallDivertServiceGroups CGSCallDivertPluginContainer::CurrentFeatureId() const
    {
    // Actually listbox item IDs are from TGSCallDivertServiceGroups so safe to cast
    return (TGSCallDivertServiceGroups) iListboxItemArray->CurrentFeature( );
    }

// ---------------------------------------------------------------------------
// CGSCallDivertPluginContainer::MakeVoiceDivertsItemL
//
//
// ---------------------------------------------------------------------------
void CGSCallDivertPluginContainer::MakeVoiceDivertsItemL()
    {
    iListboxItemArray->SetItemVisibilityL( EGSSettIdVoiceDivert,
        CGSListBoxItemTextArray::EVisible );
    }

// ---------------------------------------------------------------------------
// CGSCallDivertPluginContainer::MakeDataAndVideoDivertsItemL
//
//
// ---------------------------------------------------------------------------
void CGSCallDivertPluginContainer::MakeVideoDivertsItemL()
    {
	
     if ( FeatureManager::FeatureSupported(
           KFeatureIdCsVideoTelephony ) )
         {
         iListboxItemArray->SetItemVisibilityL( EGSSettIdVideoDivert,
             CGSListBoxItemTextArray::EVisible );
         } 
     else 
    	 {
         iListboxItemArray->SetItemVisibilityL( EGSSettIdVideoDivert,
             CGSListBoxItemTextArray::EInvisible ); 
       	 }
    }
//End of file
