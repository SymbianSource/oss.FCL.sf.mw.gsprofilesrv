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
* Description: CLF Operation Observer.
*
*/



#ifndef __CFLDOPERATIONOBSERVER_H__
#define __CFLDOPERATIONOBSERVER_H__

// INTERNAL INCLUDES

// EXTERNAL INCLUDES
#include <e32base.h>
#include <MCLFOperationObserver.h>

// CLASS DECLARATION

/**
*  CLF Operation Observer.
*  CFLDOperationObserver receives operation events of Content 
*  Listing Framework. Operation event is generated when asynchronous refresh
*  operation of List Model is finished.
*/
NONSHARABLE_CLASS( CFLDOperationObserver )
    : public CBase,
      public MCLFOperationObserver
    {
    public:  // Constructors and destructor

		/**
		 * Creates and returns a new instance of this class.
		 * @return Pointer to the CFLDOperationObserver object
		 */
	    static CFLDOperationObserver* NewL();

        /**
        * Destructor.
        */
        virtual ~CFLDOperationObserver();

    private:    // Constructors
        /**
        * C++ default constructor.
        */
        CFLDOperationObserver();

    protected:  // Methods derived from MCLFOperationObserver

        void HandleOperationEventL( TCLFOperationEvent aOperationEvent,
                                    TInt aError );

	public:     // New functions
                                    
  		/**
        * Check if there is a refresh operation is ongoing.
        * @return ETrue if refresh is not finished,
        *         EFalse if refresh is finished
        */
        TBool IsRefreshOngoing();

        /**
        * Prepare for a refresh operation to start.
        * @param aWait The wait instance to stop when refreshing is completed
        */
        void PrepareForRefresh( CActiveSchedulerWait& aWait );

    private:    // Data

        /// Own: For waiting the refresh operation to complete
        CActiveSchedulerWait* iWait;
    
        // For indicating that refresh operation is started but not finished
        TBool iRefreshOngoing;                                    

    };

#endif // __CFLDOperationObserver_H__

// End of File
