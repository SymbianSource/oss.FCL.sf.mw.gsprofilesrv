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
* Description: 
*  CFLDController test class.
*
*
*/


#ifndef __T_CFLDController_H__
#define __T_CFLDController_H__

// INCLUDES
#include <CEUnitTestSuiteClass.h>

//  FORWARD DECLARATIONS
class CFLDController;

//  CLASS DEFINITION

/*
-----------------------------------------------------------------------------

    DESCRIPTION

    Test suite for class CEUnitTestCase.

-----------------------------------------------------------------------------
*/
class T_CFLDController
    : public CEUnitTestSuiteClass
    {
    public:		// Constructors and destructors
        static T_CFLDController* NewLC();
        ~T_CFLDController();

    private:	// New methods
        T_CFLDController();
        void ConstructL();

    private:    // Test case functions
        void SetupL();
        void EmptySetupL();
        void Teardown();

        // tests:
        void TestCreateObjectL();

        void HandleFileListBoxEventTestL();
        void SetDelayTestL();

        void SetVolumeTestL();
        void SetRingingTypeTestL();
        void SetVibraTestL();
        void Set3dEffectsTestL();
        void SetFileObserverTestL();

    private:  // Implementation
        EUNIT_DECLARE_TEST_TABLE;

    private:	// Data
        CFLDController* iController; // own
    };

#endif      //  __T_CFLDController_H__

// end of file

