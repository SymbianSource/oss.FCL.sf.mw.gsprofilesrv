/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Container for the Power saving query view
*
*/


// INCLUDE FILES
#include "GSPowerSavingQueryPluginContainer.h"
#include "GSPowerSavingQueryPluginModel.h"
#include "GSPowerSavingQueryPlugin.hrh"
#include "GsLogger.h"
#include <GSPowerSavingQueryPluginRsc.rsg>
#include <aknlists.h>
#include <StringLoader.h>
#include <AknUtils.h>
#include <gsfwviewuids.h>
#include <gslistbox.h>

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSPowerSavingQueryPluginContainer::ConstructL()
// 
// Symbian OS two phased constructor
// ---------------------------------------------------------------------------
//
void CGSPowerSavingQueryPluginContainer::ConstructL( const TRect& aRect )
    {
    __GSLOGSTRING("[CGSPowerSavingQueryPluginContainer]-->CGSPowerSavingQueryPluginContainer::ConstructL");
    
    iListBox = new( ELeave ) CAknSettingStyleListBox;
    iModel = CGSPowerSavingQueryPluginModel::NewL();

    BaseConstructL( aRect, R_GS_POWER_SAVING_QUERY_VIEW_TITLE, 
                    R_POWER_SAVING_QUERY_LBX );
    
    __GSLOGSTRING("[CGSPowerSavingQueryPluginContainer]<--CGSPowerSavingQueryPluginContainer::ConstructL");
    }

// ---------------------------------------------------------------------------
// CGSPowerSavingQueryPluginContainer::~CGSPowerSavingQueryPluginContainer()
// 
// Destructor 
// ---------------------------------------------------------------------------
//
CGSPowerSavingQueryPluginContainer::~CGSPowerSavingQueryPluginContainer()
    {
    delete iListboxItemArray;
    delete iModel;
    }


// ---------------------------------------------------------------------------
// CGSPowerSavingQueryPluginContainer::ConstructListBoxL()
// 
// Construct the listbox from resource array.
// ---------------------------------------------------------------------------
//
void CGSPowerSavingQueryPluginContainer::ConstructListBoxL( TInt aResLbxId )
    {
    __GSLOGSTRING("[CGSPowerSavingQueryPluginContainer]-->CGSPowerSavingQueryPluginContainer::ConstructListBoxL");
    
    iListBox->ConstructL( this, EAknListBoxSelectionList );

    iListboxItemArray = CGSListBoxItemTextArray::NewL( aResLbxId, 
                                                 *iListBox, *iCoeEnv );
    iListBox->Model()->SetItemTextArray( iListboxItemArray );
    iListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );

    CreateListBoxItemsL();
    
    __GSLOGSTRING("[CGSPowerSavingQueryPluginContainer]<--CGSPowerSavingQueryPluginContainer::ConstructListBoxL");
    }


// ---------------------------------------------------------------------------
// CGSPowerSavingQueryPluginContainer::CreateListBoxItemsL()
// 
// Create listbox items.
// ---------------------------------------------------------------------------
//
void CGSPowerSavingQueryPluginContainer::CreateListBoxItemsL()
    {
    __GSLOGSTRING("[CGSPowerSavingQueryPluginContainer]-->CGSPowerSavingQueryPluginContainer::CreateListBoxItemsL");
    
    MakePowerSavingQueryItemL();
    
    __GSLOGSTRING("[CGSPowerSavingQueryPluginContainer]<--CGSPowerSavingQueryPluginContainer::CreateListBoxItemsL");
    }


// ---------------------------------------------------------------------------
// CGSPowerSavingQueryPluginContainer::UpdateListBoxL()
// 
// Update listbox item.
// ---------------------------------------------------------------------------
//
void CGSPowerSavingQueryPluginContainer::UpdateListBoxL( TInt aFeatureId )
    {
    switch( aFeatureId )
        {
        case KGSSettIdPowerSavingQuery:
            MakePowerSavingQueryItemL();
            break;
        default:
            break;
        }

    iListBox->HandleItemAdditionL();
    }


// ---------------------------------------------------------------------------
// CGSPowerSavingQueryPluginContainer::MakePowerSavingQueryItemL()
// 
// Create Power saving query list item 
// ---------------------------------------------------------------------------
//
void CGSPowerSavingQueryPluginContainer::MakePowerSavingQueryItemL()
    {
    __GSLOGSTRING("[CGSPowerSavingQueryPluginContainer]-->CGSPowerSavingQueryPluginContainer::MakePowerSavingQueryItemL");
    
    TInt queryValue = iModel->PowerSavingQueryEnabled();
    
    TInt resId( R_POWER_SAVING_QUERY_OFF );
    if ( EGSPowerSavingQueryOn == queryValue )
        {
        resId = R_POWER_SAVING_QUERY_ON;
        }
    
    HBufC* buf = iEikonEnv->AllocReadResourceL( resId );

    TPtr bufPtr( buf->Des() );
    iListboxItemArray->SetDynamicTextL( KGSSettIdPowerSavingQuery, bufPtr );

    // And add to listbox
    iListboxItemArray->SetItemVisibilityL( KGSSettIdPowerSavingQuery, 
            CGSListBoxItemTextArray::EVisible );

    delete buf;
    
    __GSLOGSTRING("[CGSPowerSavingQueryPluginContainer]<--CGSPowerSavingQueryPluginContainer::MakePowerSavingQueryItemL");
    
    }

// ---------------------------------------------------------------------------
// CGSPowerSavingQueryPluginContainer::GetHelpContext() const
//  
// Gets Help 
// ---------------------------------------------------------------------------
//
void CGSPowerSavingQueryPluginContainer::GetHelpContext( TCoeHelpContext& aContext ) const
    {
      aContext.iMajor = KUidGS;
  //  aContext.iContext = 0;
    }

// ---------------------------------------------------------------------------
// CGSPowerSavingQueryPluginContainer::CurrentFeatureId()
//
// Return the feature id of selected listitem  
// ---------------------------------------------------------------------------
//
TInt CGSPowerSavingQueryPluginContainer::CurrentFeatureId() const
    {
    return iListboxItemArray->CurrentFeature();
    }

// ---------------------------------------------------------------------------
// CGSPowerSavingQueryPluginContainer::Model()
//
// Return the power saving query plugin data model 
// ---------------------------------------------------------------------------
//
CGSPowerSavingQueryPluginModel* CGSPowerSavingQueryPluginContainer::Model()
    {
    return iModel;
    }

// End of File
