/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: 
* 	CFLDSoftKeyChanger changes softkeys into video preview layout and
* 	video preview select layout. Allows also to restore old layout.	
*
*
*/


#ifndef __CFLDSOFTKEYCHANGER_H__
#define __CFLDSOFTKEYCHANGER_H__

// INTERNAL INCLUDES
#include <e32base.h>

// EXTERNAL INCLUDES
#include "MFLDFileListBoxObserver.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
* CFLDSoftKeyChanger changes softkeys into video preview layout and
* video preview select layout. Allows also to restore old layout.
*
* @lib filelist.lib
* @since 3.1
*/
NONSHARABLE_CLASS( CFLDSoftKeyChanger ) : public CBase
    {
    
    public:	// Constructors and destructors

		/**
		* Two-phased static constructor.
		* @return A pointer to a fully constructed CFLDSoftKeyChanger instance
		*/
    	static CFLDSoftKeyChanger* NewL();
 
 		/**
 	 	* Two-phased static constructor, leaves a pointer to cleanup stack
 		* @return A pointer to a fully constructed CFLDSoftKeyChanger instance
 		*/
		static CFLDSoftKeyChanger* NewLC();

    	/**
    	* Destructor
    	*/
    	virtual ~CFLDSoftKeyChanger();

    protected:  // Constructors and destructors

    	/**
    	* Constructor, protected
    	*/
    	CFLDSoftKeyChanger();

    	/**
    	* Second phase constructor
    	*/
    	void ConstructL();
    
    public: // New methods
    
    	/**
    	* Changes softkey layout into 'Preview' 'Cancel'
        */
    	void ChangeNewSoftKeysPreviewL();
    	
    	/**
    	* Changes softkey layout into 'Select' 'Cancel'
    	* NOTE: this layout is used only when tone selection list
    	* is at preview state
        */
    	void ChangeNewSoftKeysPreviewSelectL();
    	
    	/**
    	* Restores layout into original 'Select' 'Back'
        */
    	void RestoreOldToneSelectionSoftKeysL();
    	
    	/**
    	* Returns current softkeystate
        */
        MFLDFileListBoxObserver::TFileListSoftKeyState
         SoftKeyState() const;
                
    private: // New methods
    
    	/**
    	* Opens filelist internal resource file
        */
    	void OpenResourceL();
    	
    	/**
    	* Changes softkey label from given string
    	* @parameter aSoftKeyLabel new label for softkey
        */
   		void ChangeLeftSoftKeyL( const TDesC& aSoftKeyLabel ) const;
   		/**
    	* Changes softkey label from given string
    	* @parameter aSoftKeyLabel new label for softkey
        */
   		void ChangeRightSoftKeyL( const TDesC& aSoftKeyLabel ) const;

    
    private: // Data
    
		// Softkey state       	
       	MFLDFileListBoxObserver::TFileListSoftKeyState iSoftKeyState;
       	
       	/// Own: SoftkeyLabel
       	HBufC* iSoftKeySelect;
       	
       	/// Own: SoftkeyLabel
       	HBufC* iSoftKeyPreview;
       	
       	/// Own: SoftkeyLabel
       	HBufC* iSoftKeyCancel;

       	/// Own: SoftkeyLabel
       	HBufC* iSoftKeyBack;

    };

#endif      // __CFLDSOFTKEYCHANGER_H__

// End of File
