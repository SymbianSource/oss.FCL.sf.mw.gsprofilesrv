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


//  CLASS HEADER
#include "MT_CGSLauncher.h"

//  EXTERNAL INCLUDES
#include <EUnitMacros.h>
#include <EUnitDecorators.h>
#include <GSLauncher.h>
#include <GSFWViewUIDs.h>

//  INTERNAL INCLUDES


// CONSTRUCTION
MT_CGSLauncher* MT_CGSLauncher::NewL()
    {
    MT_CGSLauncher* self = MT_CGSLauncher::NewLC();
    CleanupStack::Pop();

    return self;
    }

MT_CGSLauncher* MT_CGSLauncher::NewLC()
    {
    MT_CGSLauncher* self = new( ELeave ) MT_CGSLauncher();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
MT_CGSLauncher::~MT_CGSLauncher()
    {
    }

// Default constructor
MT_CGSLauncher::MT_CGSLauncher()
    {
    }

// Second phase construct
void MT_CGSLauncher::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void MT_CGSLauncher::EmptySetupL(  )
    {
    }
    
void MT_CGSLauncher::SetupL(  )
    {
    iCGSLauncher = CGSLauncher::NewL();;
    }
    

void MT_CGSLauncher::Teardown(  )
    {
    if ( iCGSLauncher )
    	{
	    delete iCGSLauncher; 
    	iCGSLauncher = NULL; 
    	}
    }
    

void MT_CGSLauncher::T_CGSLauncher_NewLL(  )
    {
    CGSLauncher* Launcher = CGSLauncher::NewL();
    CleanupStack::PushL( Launcher );
    EUNIT_ASSERT_DESC( Launcher != NULL, "An instance of CGSLauncher is not created");
    CleanupStack::PopAndDestroy(); 
    }
    
void MT_CGSLauncher::T_CGSLauncher_LaunchGSViewLL(  )
    {
    // iCGSLauncher->LaunchGSViewL( <add parameters here > );
     EUNIT_ASSERT_DESC( EFalse, "Generated assert, replace with real");
    }
    
//----------------------------------------------------------------------------
TBool MT_CGSLauncher::KillGSL()
	{
	RDebug::Print(_L("T_CGSLauncher::KillGSL()"));
	TUid gsuid = TUid::Uid(0x100058EC);
	TBool taskFoundAndEnded(EFalse);
	RWsSession 		ws;
	ws.Connect();
	TApaTaskList 	l(ws);
	TBool GSExist = l.FindApp(gsuid).Exists();
	TInt i=5;
	if ( GSExist )
		{
		while ( 1 )
			{
			RDebug::Print(_L("T_CGSLauncher::KillGSL()  trying to End GS"));
			TApaTask 		gs = l.FindApp(gsuid);
			if( !gs.Exists() || i-- < 0) 
				{
				taskFoundAndEnded = i>0 ? ETrue : EFalse;
				break; 
				}
			gs.EndTask();
			RDebug::Print(_L("T_CGSLauncher::KillGSL   Ended GS"));
			User::After(TTimeIntervalMicroSeconds32 (100000));
			}
		}
	ws.Close();
	return taskFoundAndEnded;
	}

// ---------------------------------------------------------------------------
//   Launches GS view, waits 8 sec and kills GS application (success criteria)
void MT_CGSLauncher::LaunchGWView_gs_not_running(  )
    {
    	RDebug::Print(_L("T_CGSLauncher::T_CGSLauncher_LaunchGSViewL(  )"));
     iCGSLauncher->LaunchGSViewL( KGSTelPluginUid,TUid::Uid(0),_L8("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA") ); // telephony
     User::After(TTimeIntervalMicroSeconds32 (8000000));
     TBool endedSuccesfully=KillGSL();
     EUNIT_ASSERT_DESC( endedSuccesfully, "test result");
    }

// ---------------------------------------------------------------------------
// Launches GS view, waits 8 sec and launches other GS view. 
// Waits 2 sec and kills GS application (success criteria)
void MT_CGSLauncher::LaunchGWView_gs_is_running()
	{
 	RDebug::Print(_L("T_CGSLauncher::LaunchGWView_gs_is_running(  )"));

	iCGSLauncher->LaunchGSViewL( KGSGenPluginUid,TUid::Uid(0),_L8("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA") ); // security view
	User::After(TTimeIntervalMicroSeconds32 (8000000));

	iCGSLauncher->LaunchGSViewL( KGSConPluginUid,TUid::Uid(0),KNullDesC8  ); // connection view
	User::After(TTimeIntervalMicroSeconds32 (3000000));

	TBool endedSuccesfully=KillGSL();
	EUNIT_ASSERT_DESC( endedSuccesfully, "test result");
	}
	
//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    MT_CGSLauncher,
    "Add test suite description here.",
    "MODULE" )

EUNIT_TEST(
    "NewL and NewLC - test",
    "CGSLauncher",
    "NewL and NewLC",
    "FUNCTIONALITY",
    EmptySetupL, T_CGSLauncher_NewLL, Teardown)
    
EUNIT_TEST(
    "Launch GS once",
    "CGSLauncher",
    "LaunchGSViewL",
    "FUNCTIONALITY",
    SetupL, LaunchGWView_gs_not_running, Teardown)

EUNIT_TEST(
    "Launch GS twice",
    "CGSLauncher",
    "LaunchGSViewL",
    "FUNCTIONALITY",
    SetupL, LaunchGWView_gs_is_running, Teardown)
    

EUNIT_END_TEST_TABLE

//  END OF FILE
