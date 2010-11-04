/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
*
*/
#ifndef TESTGSPLUGIN_H
#define TESTGSPLUGIN_H

// INCLUDES
#include    <GSParentPlugin.h>



// CLASS DECLARATION

class CTestGSPlugin : public CGSParentPlugin
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static CTestGSPlugin* NewL();
        static CTestGSPlugin* NewLC();
        /**
         * Destructor
         */
        ~CTestGSPlugin();

        
    public:
    
    	void GetCaptionL( TDes& aCaption ) const;
    	
    	TUid Id() const;
    	
    private:    // Constructors and destructors

        CTestGSPlugin();
        void ConstructL();

    };

#endif // TESTGSPLUGIN_H
// End of File