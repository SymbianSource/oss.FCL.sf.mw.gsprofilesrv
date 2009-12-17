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
* Description:   Variation functionality class
*
*/



#ifndef MEDIAFILEVARIATION_H
#define MEDIAFILEVARIATION_H


//  INCLUDES
#include <e32base.h>


// CONSTANTS

//FORWARD DECLARATIONS
class CRepository;    


/**
* CMediaFileDialogVariation
* 
* CMediaFileDialogVariation is used to varitate media file dialog.
* Main purpose is to prevent selecting certain mime types. These
* 'blocked' mime types are stored in central repository.  
* Notice: This code is a copy from CProfilesLocalFeatures.  
*/
NONSHARABLE_CLASS(CMediaFileDialogVariation) : public CBase
    {
    public: // Constructors and destructor
        static CMediaFileDialogVariation* NewL();
        ~CMediaFileDialogVariation();

    private:
        void ConstructL();
        CMediaFileDialogVariation( TInt aFeatureBitMask );

    public:
        TBool IsFeatureSupported( TInt aLocalFeatureId );
        TBool IsBlockedType( const TDesC& aMimeType );
        TBool IsBlockedUnprotectedType( const TDesC& aMimeType );
        TBool IsBlockedProtectedType( const TDesC& aMimeType );
        TBool IsBlockedDemoPlayType( const TDesC& aMimeType );
        TBool IsExcludedType( const TDesC& aMimeType );

    private:

        void ReadListKeyL( CRepository& aCenRep, TUint32 aKey,
                           RBuf& aValue );
        TBool ContainsMimeType( const TDesC& aList, const TDesC& aMimeType );


    private:

        // the key containing boolean type features
        TInt iFeatureBitMask;

        // contains space separated list of blocked MIME-types
        RBuf iBlockedTones;

        // contains space separated list of blocked unprotected MIME-types
        RBuf iBlockedUnprotectedTones;

        // contains space separated list of blocked protected MIME-types
        RBuf iBlockedProtectedTones;

        // contains space separated list of blocked demo-play MIME-types
        RBuf iBlockedDemoPlayTones;

        // contains space separated list of excluded MIME-types
        RBuf iExcludedTones;

        // helper buffer for mime-type matching
        RBuf iMimeBuf;
    };
    


#endif  // MEDIAFILEVARIATION_H

// End of File
