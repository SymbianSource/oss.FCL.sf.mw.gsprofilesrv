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
*     MFLDEntryFormatter defines an abstract interface to create
*     formatted presentations of directory entries
*
*
*/



#ifndef __MFLDENTRYFORMATTER_H__
#define __MFLDENTRYFORMATTER_H__

// INTERNAL INCLUDES

// EXTERNAL INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CFLDEntryReference;

// CLASS DEFINITION

/**
* MFLDEntryFormatter defines an abstract interface to create
* formatted presentations of directory entries
*/
class MFLDEntryFormatter
    {
    public:  // Constructors and destructor

        /**
        * Destructor.
        */
        virtual ~MFLDEntryFormatter() {}

    public: // New functions

        /**
        * Formats a single entry
        * @param aEntry The entry to be formatted
        */
        virtual void FormatL( CFLDEntryReference& aEntry ) = 0;

        /**
        * Formats a null item text (see CFLDFileListModel::InsertNullItemL).
        * Default implementation does no modifications to the given text
        * (to maintain BC).
        * @param aText The text to be formatted.
        * @return Returns an allocated formatted HBufC.
        *         Ownership is returned to the caller.
        * @since Series 60 2.6
        */
        virtual HBufC* FormatTextLC( const TDesC& aText )
            { return aText.AllocLC(); }

    };

#endif      //  __MFLDENTRYFORMATTER_H__

// End of File
