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
*     This class is responsible of formatting a directory entry to
*     a format that is displayable in a CAknSingleGraphicBtPopupMenuStyleListBox.
*
*
*/



#ifndef CFLDSINGLEGRAPHICENTRYFORMATTER_H
#define CFLDSINGLEGRAPHICENTRYFORMATTER_H

// INTERNAL INCLUDES
#include "MFLDEntryFormatter.h"

// EXTERNAL INCLUDES
#include <e32base.h>
#include <f32file.h>

// FORWARD DECLARATIONS
class CFLDEntryRefererence;

// CLASS DECLARATION


/**
* CDriveUtil
*
* CDriveUtil is used for getting drive information.
*/
NONSHARABLE_CLASS (CDriveUtil) : public CBase
    {
    public:
        static CDriveUtil* NewL();
        virtual ~CDriveUtil();

    private:
	    CDriveUtil();
	    void ConstructL();

	public:
		TBool IsMemoryCard(const TDesC& aFullPath);
		TBool IsMassStorage(const TDesC& aFullPath);
        
    private:
        RFs iFsSession;
    };


/**
* This class is responsible of formatting a directory entry to
* a format that is displayable in a CAknSingleGraphicBtPopupMenuStyleListBox.
*
* @lib FileList.lib
* @since Series 60 2.6
*/
NONSHARABLE_CLASS( CFLDSingleGraphicEntryFormatter ) :
 public CBase, public MFLDEntryFormatter
    {
    public:     // // Functions from base classes

		CFLDSingleGraphicEntryFormatter();
		
		/**
        * Destructor.
        */
        virtual ~CFLDSingleGraphicEntryFormatter();

		
        /**
        * From MFLDEntryFormatter
        */
        void FormatL( CFLDEntryReference& aEntry );

        /**
        * From MFLDEntryFormatter
        */
        HBufC* FormatTextLC( const TDesC& aText );
        
    private:
        CDriveUtil* iDriveUtil;
    };

#endif      //  CFLDSINGLEGRAPHICENTRYFORMATTER_H

// End of File
