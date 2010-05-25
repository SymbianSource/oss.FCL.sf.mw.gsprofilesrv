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
* Description:  Container for the Main Barring folder
*
*/


// INCLUDE FILES
#include <aknlists.h>
#include <StringLoader.h>
#include <gscallbarringpluginrsc.rsg>
#include <csxhelp/cp.hlp.hrh>
#include <gsfwviewuids.h>
#include "GSMainBarringContainer.h"

#include "gslistbox.h"
#include "CallBarringPlugin.hrh"
#include <BTSapInternalPSKeys.h>
#include <e32property.h>  //P&S


// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// 
// Symbian OS two phased constructor
// 
// ---------------------------------------------------------------------------
void CGSSettListMainBarringContainer::ConstructL( const TRect& aRect )
    {
    iListBox = new ( ELeave ) CAknSettingStyleListBox;

    BaseConstructL( aRect, 
                    R_GS_BARRING_MAIN_VIEW_TITLE, 
                    R_BARRING_MAIN_LBX );
                    
    RProperty::Get( KPSUidBluetoothSapConnectionState,
                    KBTSapConnectionState,
                    iBtSapConnectionState );
    }

// ---------------------------------------------------------------------------
// 
// Destructor
//  
// ---------------------------------------------------------------------------
CGSSettListMainBarringContainer::~CGSSettListMainBarringContainer()
    {
    if ( iItems )
        {
        delete iItems;
        }
    if ( iListboxItemArray )
        {
        delete iListboxItemArray;
        }
    }

// ---------------------------------------------------------------------------
// 
// Creates list box
//  
// ---------------------------------------------------------------------------
void CGSSettListMainBarringContainer::ConstructListBoxL( TInt aResLbxId )
    {        
    iListBox->ConstructL( this, EAknListBoxSelectionList); 
    iListboxItemArray = CGSListBoxItemTextArray::NewL( aResLbxId, 
    *iListBox, *iCoeEnv );
    iListBox->Model()->SetItemTextArray( iListboxItemArray );
    iListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );
    
    CreateListBoxItemsL();
    }

// ---------------------------------------------------------------------------
// 
// Creates List box items
//  
// ---------------------------------------------------------------------------
void CGSSettListMainBarringContainer::CreateListBoxItemsL()
    {
    if ( iBtSapConnectionState != EBTSapConnected )
        {
        iListboxItemArray->SetItemVisibilityL( EGSCBCellular, 
            CGSListBoxItemTextArray::EVisible );
        }

    iListboxItemArray->SetItemVisibilityL( EGSCBInternet, 
        CGSListBoxItemTextArray::EVisible );
    }

// ---------------------------------------------------------------------------
// CGSSettListMainBarringContainer::GetHelpContext
// Gets Help Context
//  
// ---------------------------------------------------------------------------
//
void CGSSettListMainBarringContainer::GetHelpContext(
    TCoeHelpContext& aContext ) const
    {
    aContext.iMajor = KUidGS;
    aContext.iContext = KDIV_HLP_BARRING_MAIN;
    }

// ---------------------------------------------------------------------------
// CGSSettListMainBarringContainer::CurrentFeatureId()
//  
// ---------------------------------------------------------------------------
//
TInt CGSSettListMainBarringContainer::CurrentFeatureId() const
    {
    return iListboxItemArray->CurrentFeature();
    }
    
//End of File

