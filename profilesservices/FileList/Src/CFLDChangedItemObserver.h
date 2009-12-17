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
* Description: CLF changed item Observer.
*
*/



#ifndef __CFLDCHANGEDITEMOBSERVER_H__
#define __CFLDCHANGEDITEMOBSERVER_H__

// INTERNAL INCLUDES

// EXTERNAL INCLUDES
#include <e32base.h>
#include <MCLFChangedItemObserver.h>

// FORWARD DECLARATIONS
class CFLDFileListModel;
class CFLDWaitNote;
class CFLDController;
// CLASS DECLARATION

/**
*  CLF changed item Observer.
*  CFLDChangedItemObserver receives operation events of Content
*  Listing Framework. Operation event is generated when model
*  has been changed ( for example file has been renamed )
*/
NONSHARABLE_CLASS( CFLDChangedItemObserver )
    : public CBase,
      public MCLFChangedItemObserver
    {
    public:  // Constructors and destructor

		/**
		 * Creates and returns a new instance of this class.
		 * @return Pointer to the CFLDChangedItemObserver object
		 */
	    static CFLDChangedItemObserver* NewL();
	    
	    /**
	     * Creates and returns a new instance of this class.
	     * @return Pointer to the CFLDChangedItemObserver object
	     */
	    static CFLDChangedItemObserver* NewL(CFLDWaitNote* aWaitNote);

        /**
        * Destructor.
        */
        virtual ~CFLDChangedItemObserver();

    private:    // Constructors
        /**
        * C++ default constructor.
        */
        CFLDChangedItemObserver();
        
        /**
         * C++ constructor.
         */
        CFLDChangedItemObserver(CFLDWaitNote* aWaitNote);
        
        /**
         * Second phase constructor
         */
        void ConstructL(CFLDWaitNote* aWaitNote );

    protected:  // Methods derived from MCLFChangedItemObserver

		void HandleItemChangeL( const TArray<TCLFItemId>& aItemIDArray );

        void HandleError( TInt aError );

	public:     // New functions
		/*
		* Sets callback instance of filelist model
		* @param aModel Instance of filelist model
		*/
		void SetFileListModel( CFLDFileListModel* aModel );
		
	private:  // New methods

		/**
		 * This callback method is called when the timer expires. Calls
		 * CFLDFileListModel::RefreshEntryListL().
		 * @param aPtr pointer to CFLDChangedItemObserver
		 * @return Returns always zero
		 */
		static TInt HandleModelRefreshL( TAny* aPtr );

    private:    // Data

    	/// Ref: FileList Model
    	CFLDFileListModel* iModel;
    	
    	/// Ref: Wait note dialog for indicating refreshing process of the List Model
    	CFLDWaitNote* iWaitNote;
    	
    	/// Own: Pointer to timer
    	CPeriodic* iTimer;
    	
    	/// The delay in microseconds until timer expires
    	TTimeIntervalMicroSeconds32 iDelay;

    };

#endif // __CFLDCHANGEDITEMOBSERVER_H__

// End of File
