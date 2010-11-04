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
*  FileList test suite.
*
*
*/


//  CLASS HEADER

//  EXTERNAL INCLUDES
#include <CEUnitTestSuite.h>

#include "T_CFLDController.h"
#include "T_CFLDFileListContainer.h"

/**
 * T_CFLDFileList.dll test suite factory function.
 */
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    CEUnitTestSuite* mainSuite = CEUnitTestSuite::NewLC(_L("T_FileList Module Tests"));

    // NewLC leaves the pointer to cleanupstack until AddL finishes
    mainSuite->AddL( T_CFLDController::NewLC() );
    CleanupStack::Pop();
    mainSuite->AddL( T_CFLDFileListContainer::NewLC() );
    CleanupStack::Pop();
    CleanupStack::Pop( mainSuite );
    return mainSuite;
    }

//  END OF FILE
