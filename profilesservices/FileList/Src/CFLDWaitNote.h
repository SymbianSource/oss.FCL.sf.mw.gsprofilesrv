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
* Description: Implements wait note when CLF is refreshed.
*
*/



#ifndef __CFLDWAITNOTE_H__
#define __CFLDWAITNOTE_H__

//  EXTERNAL INCLUDES
#include <e32base.h>
#include <AknWaitDialog.h>
#include <AknWaitNoteWrapper.h>

// CLASS DECLARATION

/**
*	Implements wait note when CLF is refreshed
*/
NONSHARABLE_CLASS( CFLDWaitNote )
    : public CBase,
      public MProgressDialogCallback
    {
    public:  // Constructors and destructor

		/**
		 * Creates and returns a new instance of this class.
	     * @param aResourceId Wait note resource id
		 * @return Pointer to the CFLDWaitNote object
		 */
	    static CFLDWaitNote* NewL( const TInt aWaitNoteResourceId );

        /**
        * Destructor.
        */
        virtual ~CFLDWaitNote();

    private:    // Constructors
        /**
        * C++ default constructor.
        */
        CFLDWaitNote( const TInt aWaitNoteResourceId );

    protected:  // Methods derived from MProgressDialogCallback
        
        void DialogDismissedL( const TInt aButtonId );

    public:     // New functions

        /**
        * Close the wait note dialog.
        */
        void CloseWaitNoteL();

        /**
        * Open the wait note dialog.
        */
        void OpenWaitNoteL();

        /**
        * Is wait note running.
        */
        TBool IsRunning();

    private:    // Data

        /// Own: Wait note dialog for indicating refresh operation
        /// of the List Model (owned)
		CAknWaitDialog* iWaitDialog;
		
		/// Wait note resource
		const TInt iWaitNoteResourceId;

    };

#endif // __CFLDWAITNOTE_H__

// End of File
