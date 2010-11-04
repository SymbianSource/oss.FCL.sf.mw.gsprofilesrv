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
*  CFLDFileListContainer test class.
*
*
*/


#ifndef __T_CFLDFileListContainer_H__
#define __T_CFLDFileListContainer_H__

// INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <coneresloader.h>
#include <W32STD.H>

#include <f32file.h>
#include <badesca.h>
#include <CLFContentListing.hrh>


class CFLDFileListContainer;

//  CLASS DEFINITION

/*
-----------------------------------------------------------------------------

    DESCRIPTION

    Test suite for class CEUnitTestCase.

-----------------------------------------------------------------------------
*/
class T_CFLDFileListContainer
    : public CEUnitTestSuiteClass
    {
    public:		// Constructors and destructors
        static T_CFLDFileListContainer* NewLC();
        ~T_CFLDFileListContainer();

    private:	// New methods
        T_CFLDFileListContainer();
        void ConstructL();

    private:    // Test case functions
        void EmptySetupL();
        void SetupL();
        void Teardown();

        // tests:
        void TestCreateObjectL();

        void LaunchTestL();
        void InsertNullItemTestL();
        void InsertEndNullItemTestL();
        void SetDelayTestL();
        void SetVolumeTestL();
        void SetRingingTypeTestL();
        void SetVibraTestL();

		void SetAutomatedTypeTestL();
		void Set3dEffectsTestL();
		void SetMaxFileSizeTestL();
		
		void AddExclusiveMimeTypeTestL();
		void AddExclusiveMediaTypeTestL();
		void SetWantedMimeTypesTestL();
		void SetWantedMediaTypesTestL();
		void ResetExclusiveMimeTypesTestL();
		void ResetExclusiveMediaTypesTestL();

    private:  // Implementation
        EUNIT_DECLARE_TEST_TABLE;

    private:	// Data
    	RConeResourceLoader* iResourceLoader;
        
        CFLDFileListContainer* iContainer;
        CFLDFileListContainer* iContainer1;
        CFLDFileListContainer* iContainer2;
        CFLDFileListContainer* iContainer3;
        CFLDFileListContainer* iContainer4;
        CFLDFileListContainer* iContainer5;
        RWsSession iWsSession;

        CDesCArray* iMimeTypeArray;
        RArray<TCLFMediaType> iMediaTypeArray;

    };

#endif      //  __T_CFLDFileListContainer_H__

// end of file

