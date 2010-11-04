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


#ifndef C3DAUDIOPATTERN_H
#define C3DAUDIOPATTERN_H

//  INCLUDES
#include <e32base.h>	// CBase

// FORWARD DECLARATIONS
class RReadStream;
class RWriteStream;

// DATA TYPES

// Struct representing position in three dimensional space.
struct T3DPosition 
    {
    // X-coordinate
    TInt iX;
    // Y-coordinate
    TInt iY;
    // Z-coordinate
    TInt iZ;
    };

// Struct representing velocity.
struct T3DVelocity
	{
	// Boolean indicating is scalar velocity in use
	TBool iScalar;
	// Scalar velocity
	TInt iScalarVelocity;
	// Velocity along x-axis.
	TInt iXVector;
	// Velocity along y-axis.
	TInt iYVector;
	// Velocity along z-axis.
	TInt iZVector;
	};

// Struct representing update in motion pattern.
struct T3DScriptUpdate
	{
	// Position of the trajectory.
	T3DPosition iPosition;
	// Velocity 
	T3DVelocity iVelocity;
	// Time the pattern stays in this position.
	TInt iDTime;
	};

// Struct representing loop in pattern.
struct T3DLoop
	{
	// Position where to start the loop.
	TInt iLoopStart;
	// Position where to end the loop.
	TInt iLoopEnd;
	// How many times the loop is to be gone through.
	TInt iCount;
	};


// CLASS DECLARATION

/**
* Handles accessing 3D motion pattern data.
*
* @lib RingingTone3DPlugin.dll 
* @since 5.0
*/
NONSHARABLE_CLASS (C3DAudioPattern): public CBase
    {
    
    public:  // Constructors and destructor
    
       /**
        * Two-phased constructor. 
        */
        static C3DAudioPattern* NewL();
        
       /**
        * Two-phased constructor.
        */
        static C3DAudioPattern* NewLC();
        
       /**
        * Two-phased constructor.
        * @param aStream Stream used to internalise the pattern.
        */
        static C3DAudioPattern* NewL( RReadStream& aStream );
        
       /**
        * Two-phased constructor.
        * @param aStream Stream used to internalise the pattern.
        */
        static C3DAudioPattern* NewLC( RReadStream& aStream );
        
       /**
		* Destructor
		*/
        virtual ~C3DAudioPattern();
        
    private:
    	
    	/**
        * C++ default constructor.
        */
        C3DAudioPattern();

    private:
    
	   /**
        * Internalizes the pattern from readstream.
        * @param aStream Readstream from which to internalize the pattern.
        */
        void InternalizeL( RReadStream& aStream );
        
    public:

       /**
        * Externalizes the pattern.
        * @param aStream Writestream to externalize the pattern to.
        */
	   	void ExternalizeL( RWriteStream& aStream ) const;
	   	
	   /**
        * Sets the name of the pattern.
        * @param aName Descriptor containing the name of the pattern.
        */	   	    
	   	void SetNameL( const TDesC8& aName );

	   /**
        * Getter for name of the pattern.
        * @return Descriptor containing name of the pattern.
        */			
	    const TDesC8& Name();
	   	
	   /**
        * Sets the doppler of the pattern.
        * @param aDoppler Boolean whether doppler is available or not.
        */	   	    
	   	void SetDoppler( const TBool aDoppler );

	   /**
        * Queries if doppler-effect is available in the pattern.
        * @return Boolean indicating whether doppler is available.
        */
	   	TBool DopplerAvailable() const;

	   /**
        * Sets the reververation of pattern.
        * @param aReverb Index of reverberation used with CEnvironmentalReverbUtility.
        */	   	    
	   	void SetReverb( const TUint aReverb );

	   /**
        * Queries reverberation preset of the pattern.
        * @return Reverberation preset used in pattern.
        */
	   	TInt Reverb() const;
	   	
	   /**
        * Adds a new update to script.
        * @param aUpdate Update to add to the script.
        */
		void AddUpdateL( T3DScriptUpdate& aUpdate );
		
	   /**
        * Used to indicate a start of a loop in pattern.
        * @param aCount Amount of times the loop takes place.
        */
		void StartLoopL( TUint& aCount );
		
	   /**
        * Used to indicate end of a loop in pattern.
        */
		void EndLoop();
		
	   /**
        * Getter for script containg motion pattern.
        * @return Array containing timeframes of pattern.
        */
		RArray< T3DScriptUpdate > Script();
	   

    private:	// Data
    
    	// Name of the pattern.
        HBufC8* iName;
        // Initial status of doppler-effect.
        TBool iInitialDoppler;
        // Initial reverb preset.
        TUint iInitialReverb;
        // Array containing indexes of open loops.
        RArray< TInt > iOpenLoops;
        // Array containing loops in the pattern.
        RArray< T3DLoop > iLoops;
        // Array containing the whole pattern as timeframes.
        RArray< T3DScriptUpdate > iScript;

    };

#endif // C3DAUDIOPATTERN_H

// End of File
