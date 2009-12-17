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
* Description:  Container for the Data sub-folder
*
*/


// INCLUDE FILES
#include "GSDataCallPluginContainer.h"
#include "GSDataCallPluginModel.h"
#include "GsDataCallPlugin.hrh"

#include <bldvariant.hrh>
#include <GsDataCallPluginRsc.rsg>
#include <aknlists.h>
#include <StringLoader.h>
#include <AknUtils.h>
#include <conset.hlp.hrh>
#include <gsfwviewuids.h>
#include <gslistbox.h>

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSDataCallPluginContainer::ConstructL()
// 
// Symbian OS two phased constructor
// ---------------------------------------------------------------------------
//
void CGSDataCallPluginContainer::ConstructL( const TRect& aRect )
    {
    iListBox = new( ELeave ) CAknSettingStyleListBox;
    
    BaseConstructL( aRect, R_GS_CSD_VIEW_TITLE, R_DCALL_LBX );
    }


// ---------------------------------------------------------------------------
// CGSDataCallPluginContainer::~CGSDataCallPluginContainer()
// 
// Destructor 
// ---------------------------------------------------------------------------
//
CGSDataCallPluginContainer::~CGSDataCallPluginContainer()
    {
    delete iAutodisconValue;
    delete iListboxItemArray;
    }


// ---------------------------------------------------------------------------
// CGSDataCallPluginContainer::ConstructListBoxL()
// 
// Construct the listbox from resource array.
// ---------------------------------------------------------------------------
//
void CGSDataCallPluginContainer::ConstructListBoxL( TInt aResLbxId )
    {
    iListBox->ConstructL( this, EAknListBoxSelectionList );

    iListboxItemArray = CGSListBoxItemTextArray::NewL( aResLbxId, 
                                                 *iListBox, *iCoeEnv );
    iListBox->Model()->SetItemTextArray( iListboxItemArray );
    iListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );


    CreateListBoxItemsL();
    }


// ---------------------------------------------------------------------------
// CGSDataCallPluginContainer::CreateListBoxItemsL()
// 
// Create listbox items.
// ---------------------------------------------------------------------------
//
void CGSDataCallPluginContainer::CreateListBoxItemsL()
    {
    MakeAutodisconItemL();
    }


// ---------------------------------------------------------------------------
// CGSDataCallPluginContainer::UpdateListBoxL()
// 
// Update listbox item.
// ---------------------------------------------------------------------------
//
void CGSDataCallPluginContainer::UpdateListBoxL( TInt aFeatureId )
    {
    switch( aFeatureId )
        {
        case KGSSettIdAutodiscon:
            MakeAutodisconItemL();
            break;
        default:
            break;
        }

    iListBox->HandleItemAdditionL();
    }


// ---------------------------------------------------------------------------
// CGSDataCallPluginContainer::MakeAutodisconItemL()
// 
// Create auto disconnect list item 
// ---------------------------------------------------------------------------
//
void CGSDataCallPluginContainer::MakeAutodisconItemL()
    {
    CGSDataCallPluginModel* model = CGSDataCallPluginModel::NewL();    
    
    TInt time = model->AutodisconnectTimeL();    
    delete model;
    
    HBufC* buf = NULL;
    
    switch( time )
        {
        case KGSTimeUnlimited:
            buf = iEikonEnv->AllocReadResourceL( R_AUTODISCON_VALUE_NO );            
            break;
        case KGSOneMinute:
            {
            buf = iEikonEnv->AllocReadResourceL( R_AUTODISCON_VALUE_1MIN );
            // for A&H number conversion
            TPtr bufPtr( buf->Des() );
            if( AknTextUtils::DigitModeQuery( 
                              AknTextUtils::EDigitModeShownToUser ) )
                {
                AknTextUtils::LanguageSpecificNumberConversion( bufPtr );
                }
            }
            break;
        default:
            buf = StringLoader::LoadL( R_AUTODISCON_VALUE_MIN, 
                                       time, iEikonEnv );
            break;
        }

    TPtr bufPtr( buf->Des() );
    iListboxItemArray->SetDynamicTextL( KGSSettIdAutodiscon, bufPtr );

    // And add to listbox
    iListboxItemArray->SetItemVisibilityL( KGSSettIdAutodiscon, 
        CGSListBoxItemTextArray::EVisible );


    delete buf;
    }


// ---------------------------------------------------------------------------
// CGSDataCallPluginContainer::GetHelpContext() const
//  
// Gets Help 
// ---------------------------------------------------------------------------
//
void CGSDataCallPluginContainer::GetHelpContext( 
                                 TCoeHelpContext& aContext ) const
    {
    aContext.iMajor = KUidGS;
    aContext.iContext = KSET_HLP_CONNEC_DATA;
    }


// ---------------------------------------------------------------------------
// CGSDataCallPluginContainer::CurrentFeatureId()
//
// Return the feature id of selected listitem  
// ---------------------------------------------------------------------------
//
TInt CGSDataCallPluginContainer::CurrentFeatureId( ) const
    {
    return iListboxItemArray->CurrentFeature( );
    }


// End of File
