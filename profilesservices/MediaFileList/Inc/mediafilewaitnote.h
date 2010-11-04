/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Waiting note for query
*
*/



#ifndef MEDIAFILEWAITNOTE_H
#define MEDIAFILEWAITNOTE_H


//  INCLUDES
#include "mediafiledialogutils.h"

#include <e32base.h>
#include <AknWaitDialog.h>
#include <AknProgressDialog.h>
#include <AknsItemID.h>


// CLASS DECLARATIONS


/**
* MMediaFileWaitNoteObserver
* 
* Observer interface function for observing wait note.
*/
NONSHARABLE_CLASS (MMediaFileWaitNoteObserver)
    {
    public:
        /**
        * Callback method
        * Get's called when a dialog is dismissed.
        */
        virtual void HandleWaitNoteL( TInt aButtonId ) = 0;
    };




/**
* MMediaFileWaitNote
*  
* MMediaFileWaitNote shows progress dialog.
*/
NONSHARABLE_CLASS (CMediaFileWaitNote) : public CBase,
                                         public MProgressDialogCallback
    {

    public:// Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CMediaFileWaitNote* NewL( MMediaFileWaitNoteObserver* aObserver );
        
        /**
        * Destructor.
        */
        virtual ~CMediaFileWaitNote();
    
    private:

        /**
        * C++ default constructor.
        */
        CMediaFileWaitNote( MMediaFileWaitNoteObserver* aObserver );
	
        /**
        * By default Symbian OS constructor is private.
        */
        void ConstructL();
        
		/**
		* From MProgressDialogCallback.
        * Called by the framework when dialog is dismissed.
        * @param aButtonId
        * @return None
        */
        void DialogDismissedL( TInt aButtonId );

    public:        
        
        /**
        * Launches progress dialog.
        * @return None.
        */
        void LaunchL( const TDesC& aLabel );

        
        /**
        * Closes progress dialog.
        * @param None
        * @return None
        */
        void CancelProgressDialogL();
        
        /**
        * Set dialog label text.
        * @param aText.
        * @return None.
        */
        void SetTextL( const TDesC& aText );
        
        /**
        * Set image data.
        * @param aId.
        * @param aFileName.
        * @param aFileBitmapId.
        * @param aFileMaskId.
        * @return None.
        */
        void SetImageL( TAknsItemID aId, const TDesC& aImageFile,
                        TInt aBitmapId, TInt aBitmapMaskId );

        /**
        * Set progress bar state.
        * @param aValue.
        * @return None
        */
        void SetProgress( TInt aValue );

        /**
        * Set progress bar state.
        * @param aFinalValue.
        * @return None
        */
        void SetFinalProgress( TInt aFinalValue );

        /**
        * Gets CAknNoteControl from CAknProgressDialog.
        * @param None
        * @return CAknNoteControl.
        */
		CAknNoteControl* NoteControl();

       /**
        * Cancels animation.
        * @param None.
        * @return None.
        */
		void CancelAnimation();
        
		/**
        * Starts animation.
        * @param None
        * @return None.
        */
		void StartAnimationL();
		
		/**
        * Hides cancel button.
        * @param None
        * @return None.
        */
		void HideButtonL();


    private:
    
        // progress dialog
        CAknProgressDialog* iProgressDialog;

        // progress dialog info
        CEikProgressInfo* iProgressInfo;

		// last label text
		TBuf<KBufSize128> iLastText;

		// animation state
		TBool iAnimation;

		// dialog observer
		MMediaFileWaitNoteObserver*	iObserver;
			    
		TFileName iImageFile;
		TAknsItemID iImageId;
	    TInt iBitmapId;
	    TInt iBitmapMaskId;
	    
	    // is observer call needed
	    TBool iObserverCallNeeded;
        
	    // has dialog's DialogDismissedL called (ie has it been closed)
	    TBool iDialogDismissed;
    };




#endif      // MEDIAFILEWAITNOTE_H
            
// End of File
