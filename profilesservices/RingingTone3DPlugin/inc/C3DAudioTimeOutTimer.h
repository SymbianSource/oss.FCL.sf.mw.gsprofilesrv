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


#ifndef C3DAUDIOTIMEOUTTIMER_H
#define C3DAUDIOTIMEOUTTIMER_H

// INCLUDES
#include <e32base.h>	// CTimer


// CLASS DECLARATIONS


/**
* This class specifies the function to be called when a timeout occurs.
* Used in conjunction with C3DAudioTimeOutTimer class.
*
* @lib RingingTone3DPlugin.dll
* @since 5.0
*/
NONSHARABLE_CLASS (M3DAudioTimeOutNotifier)
    {
    public:

       /**
        * The function to be called when a timeout occurs.
        */
        virtual void TimerExpiredL() = 0;
    };




/**
* This class will notify an object after a specified timeout.
*
* @lib RingingTone3DPlugin.dll
* @since 5.0
*/
NONSHARABLE_CLASS (C3DAudioTimeOutTimer) : public CTimer
    {
    public:

       /**
        * Two-phased constructor. 
        * @param aPriority Priority to use for this timer.
        * @param aTimeOutNotify Object to notify of timeout event.
        * @return A pointer to the created instance of C3DAudioTimeOutTimer.
        */
        static C3DAudioTimeOutTimer* NewL( const TInt aPriority,
                                    	   M3DAudioTimeOutNotifier* aTimeOutNotify );

       /**
        * Two-phased constructor. Leaves pointer to object to cleanup stack.
        * @param aPriority Priority to use for this timer.
        * @param aTimeOutNotify Object to notify of timeout event.
        * @return A pointer to the created instance of C3DAudioTimeOutTimer.
        */
        static C3DAudioTimeOutTimer* NewLC( const TInt aPriority,
                                     	   M3DAudioTimeOutNotifier* aTimeOutNotify );

       /**
        * Destructor.
        */
        virtual ~C3DAudioTimeOutTimer();

    private: 

       /**
        * C++ default constructor.
        */
        C3DAudioTimeOutTimer( const TInt aPriority,
                       		  M3DAudioTimeOutNotifier* aTimeOutNotify );

       /**
        * 2nd phase constructor.
        */
        void ConstructL();

    protected: // from CActive

       /**
        * From CActive Called when RunL() leaves.
        */
        TInt RunError( TInt aError );
        
       /**
        * From CActive Called when operation completes.
        */
        void RunL();


    private: // Data

        // The observer for this objects events.
        M3DAudioTimeOutNotifier* iNotify;
    };

#endif // C3DAUDIOTIMEOUTTIMER_H

// End of File
