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
*     A simple container class that is used to reference a single
*     entry in the file list.
*
*
*/


#ifndef __CFLDENTRYREFERENCE_H
#define __CFLDENTRYREFERENCE_H

// INTERNAL INCLUDES

// EXTERNAL INCLUDES
#include <e32base.h>
#include <CLFContentListing.hrh>

// CLASS DECLARATION

/**
* A simple container class that is used to reference a single
* entry in the file list.
* 
* @lib filelist.lib
* @since 2.1
*/
NONSHARABLE_CLASS( CFLDEntryReference ): public CBase
    {
    public:
    	
    	/**
    	* Constructor
    	* @param aDirectory Pointer to directory
    	*/
    	CFLDEntryReference();
    
        /**
        * Destructor
        */
        virtual ~CFLDEntryReference();

    public:	// New functions
    
    	/**
        * Sets media type
        * @param aMediaType Media type
        */
    	void SetMediaType( TInt32 aMediaType );
    
    	/**
        * Sets path and filename
        * @param aPathAndMediaFileName Path and filename, ownership transferred
        */
	    void SetPathAndMediaFileName( HBufC* aPathAndMediaFileName );
	    
	    /**
        * Sets formatted presentation
        * @param aFormattedPresentation Formatted presentation, ownership
        *        transferred
        */
	    void SetFormattedPresentation( HBufC* aFormattedPresentation );

		/**
        * Gets formatted presentation
        * @return Reference to formatted presentation
        */
    	HBufC& FormattedPresentation() const;

		/**
        * Gets path and filename
        * @return Reference to path and filename
        */
    	HBufC& PathAndMediaFileName() const;
    	
    	/**
        * Gets Media type
        * @return Media type
        */
    	TInt32 MediaType() const;
    
    private:	// Data
    
    	/// Own: Listbox formatted data
		/// However, not created by CFLDEntryReference
        HBufC* iPathAndMediaFileName;
        
        /// Own: Listbox formatted data
		/// However, not created by CFLDEntryReference
        HBufC* iFormattedPresentation;
        
        /// Media type
        TInt32 iMediaType;

    };

#endif      // __CFLDENTRYREFERENCE_H

// End of File
