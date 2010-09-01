/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef __MT_CGSLAUNCHER_H__
#define __MT_CGSLAUNCHER_H__

//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <EUnitDecorators.h>
#include <GSLauncher.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS


//  CLASS DEFINITION
/**
 * Auto-generated test suite
 *
 */
NONSHARABLE_CLASS( MT_CGSLauncher )
	: public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static MT_CGSLauncher* NewL();
        static MT_CGSLauncher* NewLC();
        /**
         * Destructor
         */
        ~MT_CGSLauncher();

    private:    // Constructors and destructors

        MT_CGSLauncher();
        void ConstructL();

    private:    // New methods

		 void EmptySetupL();
		 
         void SetupL();
        
         void Teardown();
         
         TBool KillGSL();
        
         void T_CGSLauncher_NewLL();
        
         void T_CGSLauncher_LaunchGSViewLL();
         
         void LaunchGWView_gs_not_running();
         
         void LaunchGWView_gs_is_running();
        

    private:    // Data
		
        CGSLauncher* iCGSLauncher;
        EUNIT_DECLARE_TEST_TABLE; 

    };

#endif      //  __MT_CGSLAUNCHER_H__

// End of file
