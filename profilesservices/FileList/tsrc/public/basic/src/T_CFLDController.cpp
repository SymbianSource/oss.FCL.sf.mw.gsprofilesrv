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


//  Include Files
#include "T_CFLDController.h"
#define private public
#define protected public
#include "CFLDController.h"
#include <EUnitMacros.h>
#include <TEUnitAssertionInfo.h>
#include <CEUnitAllocTestCaseDecorator.h>

namespace
	{
	// Default delay 1000000 = 1sec
	const TInt KDefaultDelay( 1000000 );
	}

// CONSTRUCTION
// Static constructor
T_CFLDController* T_CFLDController::NewLC()
    {
    T_CFLDController* self = new(ELeave) T_CFLDController;
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// Destructor (virtual by CBase)
T_CFLDController::~T_CFLDController()
    {
    delete iController;
    iController = NULL;
    }

// Second phase construct
void T_CFLDController::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }

T_CFLDController::T_CFLDController()
    {
    }

//---------------------------------------------------------------
//----------------- TEST CASE METHODS ---------------------------
//---------------------------------------------------------------

//  METHODS

// Setup nothing.
void T_CFLDController::EmptySetupL()
    {
    }

void T_CFLDController::SetupL()
    {
    EmptySetupL();
    TBool showErrorMsgs( ETrue );
 
    iController = CFLDController::NewL( showErrorMsgs, KDefaultDelay );
    }

// Teardown nothing.
void T_CFLDController::Teardown()
    {
    delete iController;
    iController = NULL;
    }

// CREATIONAL TESTS
void T_CFLDController::TestCreateObjectL()
    {
    TBool showErrorMsgs( ETrue );
    
    CFLDController* controller = NULL;
    controller = CFLDController::NewL( showErrorMsgs, KDefaultDelay );
    delete controller;
    controller = NULL;

    controller = CFLDController::NewLC( showErrorMsgs, KDefaultDelay );
    if( controller )
    	{
    	RWindow *dummy = NULL;
    	controller->CompleteConstructionL( *dummy );
    	}
    CleanupStack::Pop( controller );
    delete controller;
    }

void T_CFLDController::SetDelayTestL()
    {
    __UHEAP_MARK;
    iController->SetDelay( 1000000 );
    __UHEAP_MARKEND;
    }

void T_CFLDController::SetVolumeTestL()
    {
    __UHEAP_MARK;
    iController->SetVolume( 1 );
    __UHEAP_MARKEND;
    }
void T_CFLDController::SetRingingTypeTestL()
    {
    __UHEAP_MARK;
    iController->SetRingingType( 1 );
    __UHEAP_MARKEND;
    }
void T_CFLDController::SetVibraTestL()
    {
    __UHEAP_MARK;
    iController->SetVibra( EFalse );
    __UHEAP_MARKEND;
    }
void T_CFLDController::Set3dEffectsTestL()
    {
    __UHEAP_MARK;
    iController->Set3dEffects( EFalse );
    __UHEAP_MARKEND;
    }
void T_CFLDController::SetFileObserverTestL()
    {
    __UHEAP_MARK;
    iController->SetFileObserver( NULL );
    __UHEAP_MARKEND;
    }
void T_CFLDController::HandleFileListBoxEventTestL()
    {
    __UHEAP_MARK;
    iController->HandleFileListBoxEventL( MFLDFileListBoxObserver::EVideoPreviewSelected, KNullDesC );
    __UHEAP_MARKEND;
    }


// Test case table for this test suite class
EUNIT_BEGIN_TEST_TABLE(
    T_CFLDController,
    "T_CFLDController test suite",
    "MODULE" )

EUNIT_TEST(
    "Create and delete",
    "CFLDController",
    "NewL",
    "FUNCTIONALITY",
    EmptySetupL, TestCreateObjectL, Teardown )
EUNIT_TEST(
    "Set delay",
    "CFLDController",
    "SetDelay",
    "FUNCTIONALITY",
    SetupL, SetDelayTestL, Teardown )
EUNIT_TEST(
    "Set volume",
    "CFLDController",
    "SetVolume",
    "FUNCTIONALITY",
    SetupL, SetVolumeTestL, Teardown )
EUNIT_TEST(
    "Set ringing type",
    "CFLDController",
    "SetRingingType",
    "FUNCTIONALITY",
    SetupL, SetRingingTypeTestL, Teardown )
EUNIT_TEST(
    "Set vibra",
    "CFLDController",
    "SetVibra",
    "FUNCTIONALITY",
    SetupL, SetVibraTestL, Teardown )
EUNIT_TEST(
    "Set 3D effects",
    "CFLDController",
    "Set3dEffects",
    "FUNCTIONALITY",
    SetupL, Set3dEffectsTestL, Teardown )
EUNIT_TEST(
    "Set file observer",
    "CFLDController",
    "SetFileObserver",
    "FUNCTIONALITY",
    SetupL, SetFileObserverTestL, Teardown )
EUNIT_TEST(
    "Handle ListBox Event",
    "CFLDController",
    "HandleFileListBoxEventL",
    "FUNCTIONALITY",
    SetupL, HandleFileListBoxEventTestL, Teardown )
EUNIT_END_TEST_TABLE

//  End of File
