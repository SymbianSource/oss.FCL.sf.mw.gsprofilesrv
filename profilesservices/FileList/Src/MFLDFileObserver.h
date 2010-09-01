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
* Description:  Abstract interface for check media files.
*
*/



#ifndef MFLDFILEOBSERVER_H
#define MFLDFILEOBSERVER_H

// INTERNAL INCLUDES

// EXTERNAL INCLUDES
#include <e32base.h>

// CLASS DEFINITION

/**
*  Abstract interface for check media files.
*
*  @lib filelist.lib
*  @since 2.1
*/
class MFLDFileObserver
    {
    public: // Enumerations

        // An enumeration which tells what is the file used for.
        enum TIntention
            {
            EPlay = 1,
            ESelect = 2
            };

    public: // New functions

        /**
        * An abstract function for checking if a file is valid.
        * @since 2.1
        * @param aFileName Full path and filename.
        * @param aIntention Describes what is the file used for.
        * @return Returns ETrue if the file is valid for intended use.
        */
        virtual TBool IsFileValidL(
            const TDesC& aFileName,
            TIntention aIntention ) = 0;

		/**
        * An abstract function for checking media type
        * @since 3.1
        * @param aFileName Full path and filename.
        * @return Returns media type
        */
        virtual TInt32 MediaFileType(
         const TDesC& aFileName ) const = 0;

    protected:

        /**
        * Destructor.
        */
        virtual ~MFLDFileObserver() {}

    };

#endif      // MFLDFILEOBSERVER_H

// End of File
