/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Command absorber to eat all key presses.
*
*/

// CLASS HEADER
#include "CFLDCommandAbsorbingControl.h"

// INTERNAL INCLUDES

// EXTERNAL INCLUDES
#include <e32std.h>
#include <eikappui.h>
#include <eikenv.h>
#include <eikbtgpc.h>

// CONSTANTS
namespace
	{
	//position 0 --> left
	const TInt KLeftPosition( 0 );
    //position 2 --> right
   	const TInt KRightPosition( 2 );
	}

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CFLDCommandAbsorbingControl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CFLDCommandAbsorbingControl* CFLDCommandAbsorbingControl::NewL()
    {
    CFLDCommandAbsorbingControl* self = CFLDCommandAbsorbingControl::NewLC();
    CleanupStack::Pop();
    return self;
    }
    
// -----------------------------------------------------------------------------
// CFLDCommandAbsorbingControl::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CFLDCommandAbsorbingControl* CFLDCommandAbsorbingControl::NewLC()
    {
    CFLDCommandAbsorbingControl* self = new ( ELeave ) CFLDCommandAbsorbingControl();

    CleanupStack::PushL( self );
    self->ConstructL();

    return self;
    }

// Destructor
CFLDCommandAbsorbingControl::~CFLDCommandAbsorbingControl()
    {
  	Release();
    }


// -----------------------------------------------------------------------------
// CFLDCommandAbsorbingControl::CFLDPopupList
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CFLDCommandAbsorbingControl::CFLDCommandAbsorbingControl()
    {
    }



// -----------------------------------------------------------------------------
// CFLDCommandAbsorbingControl::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CFLDCommandAbsorbingControl::ConstructL()
    {
    //if we don't have aplication ui,
    //then there is no need to consume commands
    iAppUi = CEikonEnv::Static()->EikAppUi();
    if( iAppUi )
        {
        //add self to eat key events
        iAppUi->AddToStackL( this, ECoeStackPriorityDialog );
        }
    }


// -----------------------------------------------------------------------------
// CFLDCommandAbsorbingControl::OfferKeyEventL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TKeyResponse CFLDCommandAbsorbingControl::OfferKeyEventL( const TKeyEvent& /*aKeyEvent*/,
                                                           TEventCode /*aType*/ )
    {
   	return EKeyWasConsumed;
    }

// -----------------------------------------------------------------------------
// CFLDCommandAbsorbingControl::Release()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDCommandAbsorbingControl::Release()
	{
	  if( iAppUi )
        {
        //remove self
        iAppUi->RemoveFromStack( this );
        }
	}

// -----------------------------------------------------------------------------
// CFLDCommandAbsorbingControl::MakeCbaVisible()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDCommandAbsorbingControl::MakeCbaVisible( TBool aVisible ) const
    {
    //position 0 --> left
    TInt leftId( iCba->ButtonGroup()->CommandId( KLeftPosition ) );  
    //position 2 --> right
    TInt rightId( iCba->ButtonGroup()->CommandId( KRightPosition ) ); 

    iCba->MakeCommandVisible( leftId, aVisible );
    iCba->MakeCommandVisible( rightId, aVisible );

    iCba->DrawNow();
    }


//  End of File






