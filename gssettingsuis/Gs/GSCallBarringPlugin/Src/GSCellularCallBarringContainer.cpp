/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Container for the Barring folder
*
*/


// INCLUDE FILES
#include "GSCellularCallBarringContainer.h"

#include <aknlists.h>
#include <StringLoader.h>
#include <gscallbarringpluginrsc.rsg>
#include <csxhelp/cp.hlp.hrh>
#include <gsfwviewuids.h>
#include "LocalViewIds.h"
#include "GSCallBarringPlugin.h"

// ========================= MEMBER FUNCTIONS ================================
// ---------------------------------------------------------------------------
// 
// Symbian OS two phased constructor
// 
// ---------------------------------------------------------------------------
void CGSSettListBarringContainer::ConstructL( const TRect& aRect )
    {
    iListBox = new ( ELeave ) CAknSettingStyleListBox;

    TInt resTitleId = R_GS_BARRING_VIEW_TITLE;
    
    CGSCallBarringPlugin* plugin = static_cast<CGSCallBarringPlugin*>
      ( ( static_cast<CAknViewAppUi*>( iCoeEnv->AppUi() ) )
              ->View( KCallBarringPluginId ) ); 
    
    if ( plugin && plugin->VoipSupported() )
        {
        resTitleId = R_GS_BARRING_VIEW_TITLE_VOIP;
        }
    
    BaseConstructL( aRect, 
                    resTitleId, 
                    R_BARRING_LBX_RESOURCE );
    }

// ---------------------------------------------------------------------------
// 
// Destructor
//  
// ---------------------------------------------------------------------------
CGSSettListBarringContainer::~CGSSettListBarringContainer()
    {
    if ( iItems )
        {
        delete iItems;
        }
    }

// ---------------------------------------------------------------------------
// 
// Creates list box
//  
// ---------------------------------------------------------------------------
void CGSSettListBarringContainer::ConstructListBoxL( TInt /*aResLbxId*/ )
    {        
    iListBox->ConstructL( this, EAknListBoxSelectionList); 
    iItemArray = static_cast <CDesCArray*> 
        ( iListBox->Model()->ItemTextArray() );
    iItems = iCoeEnv->ReadDesC16ArrayResourceL( R_BARRING_LBX );

    CreateListBoxItemsL();
    }

// ---------------------------------------------------------------------------
// 
// Creates List box items
//  
// ---------------------------------------------------------------------------
void CGSSettListBarringContainer::CreateListBoxItemsL()
    {
    TInt placeInArray = 0;
    TSettingItem readItem;
    for ( TInt i = 0; i < iItems->Count(); i++ )
        {
        readItem = ( *iItems ) [ i ]; 
        iItemArray->InsertL( placeInArray++, readItem ); 
        }
    iListBox->HandleItemAdditionL();
    }

// ---------------------------------------------------------------------------
// CGSSettListBarringContainer::GetHelpContext
// Gets Help Context
//  
// ---------------------------------------------------------------------------
//
void CGSSettListBarringContainer::GetHelpContext(
    TCoeHelpContext& aContext ) const
    {
    aContext.iMajor = KUidGS;
    aContext.iContext = KSET_HLP_BARRING_SETTINGS;
    }

//End of File
