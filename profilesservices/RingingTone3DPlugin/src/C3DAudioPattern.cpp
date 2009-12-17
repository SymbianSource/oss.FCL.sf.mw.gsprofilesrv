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
* Description:  Handles accessing 3D motion pattern data.
*
*/



// INCLUDE FILES
#include "C3DAudioPattern.h"		// C3DAudioPattern
#include <s32std.h>				// RReadStream, RWriteStream

// CONSTANTS
// Granularity for array holding updates.
const TInt KPatternArrayGranularity = 84;

// Maximum length for name of the pattern.
const TUint KMaxPatternNameLength = 100;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// C3DAudioPattern::NewL
// Two-phased constructor. 
// -----------------------------------------------------------------------------
//
C3DAudioPattern* C3DAudioPattern::NewL()
    {
    C3DAudioPattern * self = C3DAudioPattern::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// C3DAudioPattern::NewLC
// Two-phased constructor. Leaves pointer on cleanup stack.
// -----------------------------------------------------------------------------
//
C3DAudioPattern* C3DAudioPattern::NewLC()
    {
    C3DAudioPattern * self = new ( ELeave ) C3DAudioPattern();
    CleanupStack::PushL( self );
    return self;
    }

// -----------------------------------------------------------------------------
// C3DAudioPattern::NewL
// Two-phased constructor. 
// -----------------------------------------------------------------------------
//

C3DAudioPattern* C3DAudioPattern::NewL( RReadStream& aStream )
	{
	C3DAudioPattern* self = C3DAudioPattern::NewLC( aStream );
	CleanupStack::Pop( self );
	return self;
	}


// -----------------------------------------------------------------------------
// C3DAudioPattern::NewLC
// Two-phased constructor. Leaves pointer on cleanup stack.
// -----------------------------------------------------------------------------
//
C3DAudioPattern* C3DAudioPattern::NewLC( RReadStream& aStream )
	{
	C3DAudioPattern* self = new ( ELeave ) C3DAudioPattern();
	CleanupStack::PushL( self );
	self->InternalizeL( aStream );
	return self;
	}


// Destructor
C3DAudioPattern::~C3DAudioPattern()
    {
    iScript.Close();
    iLoops.Close();
    iOpenLoops.Close();
    delete iName;
    }

// -----------------------------------------------------------------------------
// C3DAudioPattern::ExternalizeL
// Externalizes pattern to writestream.
// -----------------------------------------------------------------------------
//
void C3DAudioPattern::ExternalizeL(	RWriteStream& aStream ) const
	{
	// Name of the pattern
	aStream << *iName;
	
	// Initial doppler and reverb.
	aStream.WriteInt8L( iInitialDoppler );
	aStream.WriteUint8L( iInitialReverb );

	// Number of loops
	aStream.WriteUint8L( iLoops.Count() );
	// Each loop
	for ( TInt i = 0; i < iLoops.Count(); ++i )
		{
		aStream.WriteUint8L( iLoops[i].iLoopStart );
		aStream.WriteUint8L( iLoops[i].iLoopEnd );
		aStream.WriteUint8L( iLoops[i].iCount );
		}

	// Number of updates in the script		
	aStream.WriteUint8L( iScript.Count() );
	// Each update
	for ( TInt i = 0; i < iScript.Count(); ++i )
		{
		// Position of the update
		aStream.WriteInt16L( iScript[i].iPosition.iX );
		aStream.WriteInt16L( iScript[i].iPosition.iY );
		aStream.WriteInt16L( iScript[i].iPosition.iZ );
		
		// Velocity of the update
		aStream.WriteInt8L( iScript[i].iVelocity.iScalar );
		aStream.WriteUint16L( iScript[i].iVelocity.iScalarVelocity );
		aStream.WriteInt16L( iScript[i].iVelocity.iXVector );
		aStream.WriteInt16L( iScript[i].iVelocity.iYVector );
		aStream.WriteInt16L( iScript[i].iVelocity.iZVector );
		
		// Time reserved for the update.
		aStream.WriteUint16L( iScript[i].iDTime );
		}
	}

// -----------------------------------------------------------------------------
// C3DAudioPattern::InternalizeL
// Internalizes pattern from readstream
// -----------------------------------------------------------------------------
//
void C3DAudioPattern::InternalizeL(	RReadStream& aStream )
	{
	// Internalize the name from stream.
	delete iName;
	iName = NULL;
	iName = HBufC8::NewL( aStream, KMaxPatternNameLength );
	
	// Initial doppler and reverberation
	iInitialDoppler = aStream.ReadInt8L();
	iInitialReverb = aStream.ReadUint8L();

	// Reset the array of loops.
	iLoops.Reset();
	TInt loopCount = aStream.ReadUint8L();
	// Read each loop.
	for (TInt i = 0; i < loopCount; ++i)
		{
		T3DLoop tempLoop;
		tempLoop.iLoopStart = aStream.ReadUint8L();
		tempLoop.iLoopEnd = aStream.ReadUint8L();
		tempLoop.iCount = aStream.ReadUint8L();
		iLoops.Append( tempLoop );
		}
	
	// Reset the array of loops.
	iScript.Reset();
	TInt scriptCount = aStream.ReadUint8L();
	// Read each update.
	for (TInt i = 0; i < scriptCount; ++i)
		{
		T3DPosition tempPosition;
		tempPosition.iX = aStream.ReadInt16L();
		tempPosition.iY = aStream.ReadInt16L();
		tempPosition.iZ = aStream.ReadInt16L();
		
		T3DVelocity tempVelocity;
		tempVelocity.iScalar = aStream.ReadInt8L();
		tempVelocity.iScalarVelocity = aStream.ReadUint16L();
		tempVelocity.iXVector = aStream.ReadInt16L();
		tempVelocity.iYVector = aStream.ReadInt16L();
		tempVelocity.iZVector = aStream.ReadInt16L();
		
		T3DScriptUpdate tempScript;
		tempScript.iDTime = aStream.ReadUint16L();
		tempScript.iPosition = tempPosition;
		tempScript.iVelocity = tempVelocity;
		iScript.Append( tempScript );
		}
	}

// -----------------------------------------------------------------------------
// C3DAudioPattern::SetName
// Set the name of the pattern. Memory will be allocated.
// -----------------------------------------------------------------------------
//
void C3DAudioPattern::SetNameL( const TDesC8& aName )
	{
	delete iName;
	iName = NULL;
	iName = aName.AllocL();
	}

// -----------------------------------------------------------------------------
// C3DAudioPattern::SetReverb
// Set the value of reverberation.
// -----------------------------------------------------------------------------
//	
void C3DAudioPattern::SetReverb( const TUint aReverb )
	{
	iInitialReverb = aReverb;
	}

// -----------------------------------------------------------------------------
// C3DAudioPattern::SetDoppler
// Set the status of doppler.
// -----------------------------------------------------------------------------
//		
void C3DAudioPattern::SetDoppler( const TBool aDoppler )
	{
	iInitialDoppler = aDoppler;
	}

// -----------------------------------------------------------------------------
// C3DAudioPattern::AddUpdate
// Adds a update to script.
// -----------------------------------------------------------------------------
//
void C3DAudioPattern::AddUpdateL( T3DScriptUpdate& aUpdate )
	{
	User::LeaveIfError( iScript.Append( aUpdate ) );
	}

// -----------------------------------------------------------------------------
// C3DAudioPattern::StartLoop
// Start loop at current place in the script.
// -----------------------------------------------------------------------------
//
void C3DAudioPattern::StartLoopL( TUint& aCount )
	{
	// Create T3DLoop and append it to array.
	T3DLoop loop = { iScript.Count(), 0, aCount };
	User::LeaveIfError( iLoops.Append( loop ) );
	
	// 	Update open loops array, to be able to determine which loop to end.
	User::LeaveIfError( iOpenLoops.Append( iLoops.Count() - 1 ) );
	}

// -----------------------------------------------------------------------------
// C3DAudioPattern::EndLoop
// End the lastly started loop.
// -----------------------------------------------------------------------------
//	
void C3DAudioPattern::EndLoop()
	{
	// There is at least one open loop.
	if ( iOpenLoops.Count() != 0 )
		{
		// Get the index of the loop.
		TInt aIndex = iOpenLoops[ iOpenLoops.Count() - 1 ];
		// Set the index of loop end and remove loop from array of open loops.
		iLoops[ aIndex ].iLoopEnd = iScript.Count() - 1;
		iOpenLoops.Remove( iOpenLoops.Count() - 1 );
		}
	}

// -----------------------------------------------------------------------------
// C3DAudioPattern::Name
// Get the name of the pattern.
// -----------------------------------------------------------------------------
//	
const TDesC8& C3DAudioPattern::Name()
    {
    return *iName;
    }

// -----------------------------------------------------------------------------
// C3DAudioPattern::Script
// Get the script of pattern.
// -----------------------------------------------------------------------------
//	
RArray< T3DScriptUpdate > C3DAudioPattern::Script()
	{
	return iScript;
	}

// -----------------------------------------------------------------------------
// C3DAudioPattern::DopplerAvailable
// Is doppler available in the pattern.
// -----------------------------------------------------------------------------
//		
TBool C3DAudioPattern::DopplerAvailable() const
	{
	return iInitialDoppler;
	}

// -----------------------------------------------------------------------------
// C3DAudioPattern::Reverb
// Get initial reverb of pattern.
// -----------------------------------------------------------------------------
//			
TInt C3DAudioPattern::Reverb() const
	{
	return iInitialReverb;
	}

// -----------------------------------------------------------------------------
// C3DAudioPattern::C3DAudioPattern
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
C3DAudioPattern::C3DAudioPattern(): 
	iScript( KPatternArrayGranularity )
    {
    }

// End of File
