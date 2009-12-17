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
* Description:  Timer for reading 3D pattern data.
*
*/


// INCLUDE FILES
#include "C3DAudioTimeOutTimer.h"



// ========================= MEMBER FUNCTIONS ==================================

// -----------------------------------------------------------------------------
// C3DAudioTimeOutTimer::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
C3DAudioTimeOutTimer* C3DAudioTimeOutTimer::NewL( 
	const TInt aPriority,
    M3DAudioTimeOutNotifier* aTimeOutNotify )
    {
    C3DAudioTimeOutTimer* self = C3DAudioTimeOutTimer::NewLC( aPriority, 
    														  aTimeOutNotify );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// C3DAudioTimeOutTimer::NewLC
// Two-phased constructor. Leaves pointer on cleanup stack.
// -----------------------------------------------------------------------------
//
C3DAudioTimeOutTimer* C3DAudioTimeOutTimer::NewLC( 
	const TInt aPriority,
	M3DAudioTimeOutNotifier* aTimeOutNotify )
    {
    C3DAudioTimeOutTimer* self = new ( ELeave ) C3DAudioTimeOutTimer( aPriority,
                                                        			  aTimeOutNotify );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// C3DAudioTimeOutTimer::C3DAudioTimeOutTimer
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
C3DAudioTimeOutTimer::C3DAudioTimeOutTimer( 
	const TInt aPriority,
	M3DAudioTimeOutNotifier* aTimeOutNotify ):
 	CTimer( aPriority ), 
 	iNotify( aTimeOutNotify )
    {
    }

// -----------------------------------------------------------------------------
// C3DAudioTimeOutTimer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void C3DAudioTimeOutTimer::ConstructL()
    {
    CTimer::ConstructL();
    CActiveScheduler::Add( this );
    }

// Destructor.
C3DAudioTimeOutTimer::~C3DAudioTimeOutTimer()
    {
    }

// -----------------------------------------------------------------------------
// C3DAudioTimeOutTimer::RunError
// Called when RunL() leaves.
// -----------------------------------------------------------------------------
//
TInt C3DAudioTimeOutTimer::RunError( TInt /*aError*/ )
	{
	return KErrNone;				 
	}

// -----------------------------------------------------------------------------
// C3DAudioTimeOutTimer::RunL
// Called when operation completes.
// -----------------------------------------------------------------------------
//
void C3DAudioTimeOutTimer::RunL()
    {
    // Timer request has completed, so notify the timer's owner
    if ( iStatus == KErrNone )
        {
        iNotify->TimerExpiredL();
        }
    }

// End of File
