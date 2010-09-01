/*
* Copyright (c) 2002 - 2008 Nokia Corporation and/or its subsidiary(-ies).
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
#include "MT_GSFramework.h"

//  EXTERNAL INCLUDES
#include <EUnitMacros.h>
#include <EUnitDecorators.h>

//#include <..\..\..\..\GSFramework\src\GSParentContainer.cpp>
#include <APGCLI.H>	// RApaLsSession

#include "TestGSPlugin.h"


//  INTERNAL INCLUDES

const TUid KTestActiveViewId = {  0x10 };
const TUid KTestWhateverPubSubId = {  0x00 };
const TInt KTestWhateverPubSubKey = 0;


// CONSTRUCTION
MT_GSFramework* MT_GSFramework::NewL()
    {
    MT_GSFramework* self = MT_GSFramework::NewLC();
    CleanupStack::Pop();

    return self;
    }

MT_GSFramework* MT_GSFramework::NewLC()
    {
    MT_GSFramework* self = new( ELeave ) MT_GSFramework();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
MT_GSFramework::~MT_GSFramework()
    {
    }

// Default constructor
MT_GSFramework::MT_GSFramework()
    {
    }

// Second phase construct
void MT_GSFramework::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

void MT_GSFramework::SetupNone()
    {
    }


void MT_GSFramework::TeardownNone()
    {
    }



////////////////// CGSTabHelper test cases //////////////////////////////////


void MT_GSFramework::SetupTabHelperL(  )
    {
    iCGSTabHelper = CGSTabHelper::NewL();
    iPlugins = 	new (ELeave) CArrayPtrFlat<CGSPluginInterface>(2);
    iTabbedViews = 	new (ELeave) CArrayPtrFlat<MGSTabbedView>(2);
    }


void MT_GSFramework::TeardownTabHelper()
    {
    delete iCGSTabHelper;
    iCGSTabHelper = NULL;
    delete iPlugins;
    iPlugins = NULL;
    delete iTabbedViews;
    iTabbedViews = NULL;
    }


void MT_GSFramework::T_CGSTabHelper_NewLL()
    {
    CGSTabHelper* ptr = CGSTabHelper::NewL();
    EUNIT_ASSERT( ptr );
    delete ptr;
    }

void MT_GSFramework::T_CGSTabHelper_NewLCL()
    {
    CGSTabHelper* ptr = CGSTabHelper::NewLC();
    EUNIT_ASSERT( ptr );
    CleanupStack::PopAndDestroy();
    }



void MT_GSFramework::T_CGSTabHelper__CGSTabHelperL(  )
    {
    RHeap heap = User::Heap();
    TInt block = 0;
    TInt after;
    CGSTabHelper* th;

    TInt before = heap.Available( block );

    th = CGSTabHelper::NewL();
    delete th;

    after = heap.Available( block );
    TInt delta = before - after;

    EUNIT_ASSERT_DESC( before <= after, "Possible heap memory leak");
    }

void MT_GSFramework::T_CGSTabHelper_CreateTabGroupLL(  )
    {
    iCGSTabHelper->CreateTabGroupL( KTestActiveViewId, this );
    EUNIT_ASSERT_DESC( ETrue, "Tab group pseudo creation done");
    }

void MT_GSFramework::T_CGSTabHelper_CreateTabGroupL2L(  )
    {
    iCGSTabHelper->CreateTabGroupL( KTestActiveViewId, iPlugins );
    EUNIT_ASSERT_DESC( ETrue, "Run this far wihout panic?");
    }

void MT_GSFramework::T_CGSTabHelper_CreateTabGroupL3L(  )
    {
    iCGSTabHelper->CreateTabGroupL( KTestActiveViewId, iTabbedViews );
    EUNIT_ASSERT_DESC( ETrue, "Run this far without panic?");
    }

void MT_GSFramework::T_CGSTabHelper_RemoveTabGroupL(  )
    {
    iCGSTabHelper->RemoveTabGroup();
    EUNIT_ASSERT_DESC( ETrue, "Run this far without panic?");
    }

void MT_GSFramework::T_CGSTabHelper_TabChangedLL(  )
    {
    TInt tabIndex = 0;
    iCGSTabHelper->TabChangedL( tabIndex );
    EUNIT_ASSERT_DESC( ETrue, "Run this far without panic?");
    }

// MGSTabbedViewOwner
CArrayPtrFlat<CGSPluginInterface>* MT_GSFramework::TabbedViews()
	{
	return iPlugins;
	}

// MGSTabbedViewOwner
void MT_GSFramework::TabChangedL( TUid /*selectedTabUid*/ )
	{

	}

// MGSPluginLoadObserver
void MT_GSFramework::HandlePluginLoaded( KGSPluginLoaderStatus aStatus )
    {

    }


/////////////////////// CGSPluginLoader test cases ///////////////////////////////////
void MT_GSFramework::SetupPluginLoaderL()
    {
    iPlugins =  new (ELeave) CArrayPtrFlat<CGSPluginInterface>(2);

    CAknViewAppUi* appUi = (CAknViewAppUi*)CEikonEnv::Static()->EikAppUi();
    iCGSPluginLoader = CGSPluginLoader::NewL( appUi );
    }

void MT_GSFramework::TearDownPluginLoader()
    {
    delete iCGSPluginLoader;
    iCGSPluginLoader = NULL;

    delete iPlugins;
    iPlugins = NULL;
    }


void MT_GSFramework::T_CGSPluginLoader_NewL()
    {
    CAknViewAppUi* appUi = (CAknViewAppUi*)CEikonEnv::Static()->EikAppUi();
    iCGSPluginLoader = CGSPluginLoader::NewL( appUi );
    delete iCGSPluginLoader;
    iCGSPluginLoader = NULL;

    EUNIT_ASSERT_DESC( ETrue, "Run this far?");
    }

void MT_GSFramework::T_CGSPluginLoader__CGSPluginLoader()
    {
    CAknViewAppUi* appUi = (CAknViewAppUi*)CEikonEnv::Static()->EikAppUi();
    iCGSPluginLoader = CGSPluginLoader::NewL( appUi );
    delete iCGSPluginLoader;
    iCGSPluginLoader = NULL;

    EUNIT_ASSERT_DESC( ETrue, "Run this far?");
    }

void MT_GSFramework::T_CGSPluginLoader_LoadAsyncL()
    {
    iCGSPluginLoader->LoadAsyncL( KGSPluginInterfaceUid,
                                  KTestActiveViewId,
                                  iPlugins );

    EUNIT_ASSERT_DESC( ETrue, "Run this far?");
    }

void MT_GSFramework::T_CGSPluginLoader_SetObserver()
    {
    iCGSPluginLoader->SetObserver( this );
    EUNIT_ASSERT_DESC( ETrue, "Run this far?");
    }

void MT_GSFramework::T_CGSPluginLoader_AbortAsyncLoad()
    {
    iCGSPluginLoader->AbortAsyncLoad();
    EUNIT_ASSERT_DESC( ETrue, "Run this far?");
    }

void MT_GSFramework::T_CGSPluginLoader_SortPluginsL()
    {
    iCGSPluginLoader->SortPluginsL( iPlugins );
    EUNIT_ASSERT_DESC( ETrue, "Run this far?");
    }



/////////////// CGSParentContainer test cases //////////////////////////////


// CGSParentContainer
void MT_GSFramework::SetupParentContainerL()
	{
//    iGSParentContainer = new (ELeave) CGSParentContainer();
//    iGSParentContainer->ConstructL();
	}


void MT_GSFramework::TearDownParentContainer()
	{
//	delete iGSParentContainer;
//    iGSParentContainer = NULL;
	}


// CGSParentContainer test cases:
void MT_GSFramework::T_CGSParentContainer_FocusChanged()
	{
//    iGSParentContainer->FocusChanged( ENoDrawNow );
#pragma message("TODO: implement real test case!")
    EUNIT_ASSERT_DESC( EFalse, "Not implemented.");
	}


///////////////////// CGSPubSubsListener test cases ///////////////////////

void MT_GSFramework::SetupPubSubsListenerL()
    {
    iCGSPubSubsListener = CGSPubSubsListener::NewL( KTestWhateverPubSubId,
                                                    KTestWhateverPubSubKey,
                                                    this );
    }

void MT_GSFramework::TearDownPubSubsListener()
    {
    delete iCGSPubSubsListener;
    iCGSPubSubsListener = NULL;
    }


void MT_GSFramework::T_CGSPubSubsListener_NewL()
    {
    CGSPubSubsListener* psl = CGSPubSubsListener::NewL( KTestWhateverPubSubId,
            KTestWhateverPubSubKey,
            this );
    delete psl;
    psl = NULL;

    EUNIT_ASSERT_DESC( ETrue, "Run this far?");
    }

void MT_GSFramework::T_CGSPubSubsListener__CGSPubSubsListener()
    {
    CGSPubSubsListener* psl = CGSPubSubsListener::NewL( KTestWhateverPubSubId,
            KTestWhateverPubSubKey,
            this );
    delete psl;
    psl = NULL;

    EUNIT_ASSERT_DESC( ETrue, "Run this far?");
    }

void MT_GSFramework::T_CGSPubSubsListener_Get1()
    {
    TInt val = 0;
    iCGSPubSubsListener->Get( val );
    EUNIT_ASSERT_DESC( ETrue, "Run this far?");
    }

void MT_GSFramework::T_CGSPubSubsListener_Get2()
    {
    TBuf8<25> buf;
    iCGSPubSubsListener->Get( buf );
    EUNIT_ASSERT_DESC( ETrue, "Run this far?");
    }

void MT_GSFramework::T_CGSPubSubsListener_Get3()
    {
    TBuf16<25> buf;
    iCGSPubSubsListener->Get( buf );
    EUNIT_ASSERT_DESC( ETrue, "Run this far?");
    }


// From MGSSettingPSObserver
void MT_GSFramework::HandleNotifyPSL( const TUid aUid,
                                      const TInt& aKey,
                                      const TRequestStatus& aStatus )
    {

    }

// ------------------------------- Launch Gs application test cases ---------------------------------------//

void MT_GSFramework::SetupLaunch()
    {
    iPeriodic = CPeriodic::NewL( CActive::EPriorityStandard );

    // Launch Gs application...
    RApaLsSession lsSession;
    User::LeaveIfError( lsSession.Connect() );
    CleanupClosePushL( lsSession );

	TApaAppInfo appInfo;
	TUid uid;
	uid.iUid = 0x100058EC;	// Gs application uid
	User::LeaveIfError( lsSession.GetAppInfo(appInfo, uid) );
    TFileName appName = appInfo.iFullName;

    CApaCommandLine* cmdLine = CApaCommandLine::NewLC();
    cmdLine->SetExecutableNameL( appName );
    cmdLine->SetCommandL( EApaCommandRun );
    User::LeaveIfError( lsSession.StartApp( *cmdLine ) );

    CleanupStack::PopAndDestroy( cmdLine );
    CleanupStack::PopAndDestroy();	// lsSession
    }


void MT_GSFramework::TeardownLaunch()
    {
    if ( !iPeriodic->IsActive() )
    	{
	    iPeriodic->Start( 2000000, 1000000, TCallBack( CloseTopMostWindow, static_cast<TAny*>( this ) ) );
    	}

    CActiveScheduler::Start();

	// Close the error note
	ikeySimulateCase = 0;
	iKeyNumber = 0;
    if ( !iPeriodic->IsActive() )
    	{
	    iPeriodic->Start( 2000000, 500000, TCallBack( SimulateKeyEvent, static_cast<TAny*>( this ) ) );
    	}

    CActiveScheduler::Start();

    if ( iPeriodic )
        {
        iPeriodic->Cancel();
        }
    delete iPeriodic;
    }


TBool MT_GSFramework::GetKeyCodeCase0( TKeyCode& aKeyCode )
	{
	TBool ret = ETrue;
    iKeyNumber++;

    switch ( iKeyNumber )
    	{
    	case 1:
    		aKeyCode = EKeyOK;
    		break;
    	default:
    		ret = EFalse;
    	}

    return ret;
	}


TInt MT_GSFramework::CloseTopMostWindow( TAny* aPtr )
    {
    MT_GSFramework* instL = static_cast<MT_GSFramework*>( aPtr );
    instL->DoCloseTopMostWindowL();

    CActiveScheduler::Stop();
	return KErrNone;
    }

void MT_GSFramework::DoCloseTopMostWindowL()
	{
	RWsSession* ws = &CCoeEnv::Static()->WsSession();

    // Get a list of IDs of the all the window groups
    CArrayFixFlat<TInt>* windowList = new CArrayFixFlat<TInt>(4);
    CleanupStack::PushL(windowList);
    // Get a list of window group IDs
    User::LeaveIfError(ws->WindowGroupList(0, windowList));

    // Get the top most window group
    TInt wgId = (*windowList)[0];

	CleanupStack::PopAndDestroy(); // windowList

	TApaTask task(*ws);
	task.SetWgId(wgId);
	task.SendSystemEvent(EApaSystemEventShutdown);

	iPeriodic->Cancel();
	}

TInt MT_GSFramework::SimulateKeyEvent( TAny* aPtr )
    {
    MT_GSFramework* instL = static_cast<MT_GSFramework*>( aPtr );

    instL->DoSimulateKeyEventL();

	return KErrNone;
    }

void MT_GSFramework::DoSimulateKeyEventL()
	{
	TKeyCode keyCode;
	TBool hasKey;

	switch ( ikeySimulateCase )
		{
		case 0:
			hasKey = GetKeyCodeCase0( keyCode );
			break;
		case 1:
			hasKey = GetKeyCodeCase1( keyCode );
			break;
		case 2:
			hasKey = GetKeyCodeCase2( keyCode );
			break;
		case 3:
			hasKey = GetKeyCodeCase3( keyCode );
			break;
		case 4:
			hasKey = GetKeyCodeCase4( keyCode );
			break;
		default:
			hasKey = EFalse;
		}

	if ( hasKey )
		{
	    TKeyEvent event;
	    event.iCode = keyCode;
	    event.iScanCode = 0;
	    event.iRepeats = 0;
	    event.iModifiers = 0;

		TWsEvent wsEvent;
		*( wsEvent.Key() ) = event;
		wsEvent.SetType( EEventKey );
		wsEvent.SetTimeNow();

		RWsSession*	ws = &CCoeEnv::Static()->WsSession();

		TInt wgId = ws->GetFocusWindowGroup();
		ws->SendEventToWindowGroup( wgId, wsEvent );
		}
	else
		{
		iPeriodic->Cancel();
		CActiveScheduler::Stop();
		}
	}
void MT_GSFramework::LaunchGsApp1L()
	{
	ikeySimulateCase = 1;
	iKeyNumber = 0;
    if ( !iPeriodic->IsActive() )
    {
    iPeriodic->Start( 2000000, 1000000, TCallBack( SimulateKeyEvent, static_cast<TAny*>( this ) ) );
    }

    CActiveScheduler::Start();
	}

TBool MT_GSFramework::GetKeyCodeCase1( TKeyCode& aKeyCode )
	{
	TBool ret = ETrue;
    iKeyNumber++;

    switch ( iKeyNumber )
    	{
    	case 1:
    		aKeyCode = EKeyDownArrow;
    		break;
    	case 2:
    		aKeyCode = EKeyDownArrow;
    		break;
    	case 3:
    		aKeyCode = EKeyUpArrow;
    		break;
    	case 4:
    		aKeyCode = EKeyEnter;	// Phone
    		break;
    	case 5:
    		aKeyCode = EKeyEnter;	// Date & time
    		break;
    	case 6:
    		aKeyCode = EKeyEnter;	// Time
    		break;
    	case 7:
    		aKeyCode = EKeyCBA2;
    		break;
    	case 8:
    		aKeyCode = EKeyDownArrow;
    		break;
    	case 9:
    		aKeyCode = EKeyEnter;	// Time Zone
    		break;
    	case 10:
    		aKeyCode = EKeyCBA2;
    		break;
    	case 11:
    		aKeyCode = EKeyDownArrow;
    		break;
     	case 12:
    		aKeyCode = EKeyEnter;	// Date
    		break;
    	case 13:
    		aKeyCode = EKeyCBA2;
    		break;
    	case 14:
    		aKeyCode = EKeyDownArrow;
    		break;
     	case 15:
    		aKeyCode = EKeyEnter;	// Date format
    		break;
    	case 16:
    		aKeyCode = EKeyDownArrow;
    		break;
    	case 17:
    		aKeyCode = EKeyCBA1;
    		break;
     	case 18:
    		aKeyCode = EKeyDownArrow;
    		break;
     	case 19:
    		aKeyCode = EKeyEnter;	// Date separator
    		break;
    	case 20:
    		aKeyCode = EKeyCBA2;
    		break;
    	case 21:
    		aKeyCode = EKeyDownArrow;
    		break;
     	case 22:
    		aKeyCode = EKeyEnter;	// Time format
    		break;
    	case 23:
    		aKeyCode = EKeyDownArrow;
    		break;
     	case 24:
    		aKeyCode = EKeyEnter;	// Time separator
    		break;
    	case 25:
    		aKeyCode = EKeyDownArrow;
    		break;
     	case 26:
    		aKeyCode = EKeyEnter;	// Clock type
    		break;
    	case 27:
    		aKeyCode = EKeyDownArrow;
    		break;
     	case 28:
    		aKeyCode = EKeyEnter;	// Clock Alarm Tone
    		break;
     	case 29:
     		User::After(2000000);
    		aKeyCode = EKeyCBA2;
    		break;
     	case 30:
    		aKeyCode = EKeyDownArrow;
    		break;
     	case 31:
    		aKeyCode = EKeyEnter;	// Alarm Snooze Time
    		break;
     	case 32:
    		aKeyCode = EKeyCBA2;
    		break;
    	case 33:
    		aKeyCode = EKeyDownArrow;
    		break;
     	case 34:
    		aKeyCode = EKeyEnter;	// Workdays
    		break;
     	case 35:
    		aKeyCode = EKeyCBA2;
    		break;
     	case 36:
    		aKeyCode = EKeyCBA2;
    		break;
     	case 37:
    		aKeyCode = EKeyCBA2;
    		break;
    	default:
    		ret = EFalse;
    	}

    return ret;
	}

void MT_GSFramework::LaunchGsApp2L()
	{
	ikeySimulateCase = 2;
	iKeyNumber = 0;
    if ( !iPeriodic->IsActive() )
    {
    iPeriodic->Start( 2000000, 1000000, TCallBack( SimulateKeyEvent, static_cast<TAny*>( this ) ) );
    }

    CActiveScheduler::Start();
	}

TBool MT_GSFramework::GetKeyCodeCase2( TKeyCode& aKeyCode )
	{
	TBool ret = ETrue;
    iKeyNumber++;

    switch ( iKeyNumber )
    	{
    	case 1:
    		aKeyCode = EKeyDownArrow;
    		break;
    	case 2:
    		aKeyCode = EKeyEnter;	// phone
    		break;
    	case 3:
    		aKeyCode = EKeyDownArrow;
    		break;
    	case 4:
    		aKeyCode = EKeyDownArrow;
    		break;
    	case 5:
    		aKeyCode = EKeyDownArrow;
    		break;
    	case 6:
    		aKeyCode = EKeyEnter;	// Display
    		break;
    	case 7:
    		aKeyCode = EKeyEnter;	// Light sensor
    		break;
     	case 8:
    		aKeyCode = EKeyCBA2;
    		break;
    	case 9:
    		aKeyCode = EKeyDownArrow;
    		break;
    	case 10:
    		aKeyCode = EKeyEnter;	// Display text size
    		break;
    	case 11:
    		aKeyCode = EKeyCBA2;
    		break;
    	case 12:
    		aKeyCode = EKeyDownArrow;
    		break;
     	case 13:
    		aKeyCode = EKeyEnter;	// Screen save time-out
    		break;
    	case 14:
    		aKeyCode = EKeyCBA2;
    		break;
    	case 15:
    		aKeyCode = EKeyDownArrow;
    		break;
     	case 16:
    		aKeyCode = EKeyEnter;	// Welcome note or image
    		break;
    	case 17:
    		aKeyCode = EKeyCBA1;
    		break;
     	case 18:
    		aKeyCode = EKeyDownArrow;
    		break;
     	case 19:
    		aKeyCode = EKeyEnter;	// Backlight time-out
    		break;
    	case 20:
    		aKeyCode = EKeyCBA2;
    		break;
     	case 21:
    		aKeyCode = EKeyCBA2;
    		break;
     	case 22:
    		aKeyCode = EKeyCBA2;
    		break;
    	default:
    		ret = EFalse;
    	}

    return ret;
	}

void MT_GSFramework::LaunchGsApp3L()
	{
	ikeySimulateCase = 3;
	iKeyNumber = 0;
    if ( !iPeriodic->IsActive() )
    {
    iPeriodic->Start( 2000000, 1000000, TCallBack( SimulateKeyEvent, static_cast<TAny*>( this ) ) );
    }

    CActiveScheduler::Start();
	}

TBool MT_GSFramework::GetKeyCodeCase3( TKeyCode& aKeyCode )
	{
	TBool ret = ETrue;
    iKeyNumber++;

    switch ( iKeyNumber )
    	{
    	case 1:
    		aKeyCode = EKeyEnter;	// open Persional
    		break;
    	case 2:
    		aKeyCode = EKeyDownArrow;
    		break;
    	case 3:
    		aKeyCode = EKeyDownArrow;
    		break;
    	case 4:
    		aKeyCode = EKeyDownArrow;
    		break;
    	case 5:
    		aKeyCode = EKeyEnter;	// open Standby
    		break;
    	case 6:
    		aKeyCode = EKeyEnter;	// open Shortcut
    		break;
    	case 7:
    		aKeyCode = EKeyEnter;	// open Lift idle softkey
    		break;
     	case 8:
    		aKeyCode = EKeyCBA2;
    		break;
    	case 9:
    		aKeyCode = EKeyCBA2;
    		break;
/*    	case 10:
    		aKeyCode = EKeyCBA2;
    		break;
    	case 11:
    		aKeyCode = EKeyCBA2;
    		break;*/
    	default:
    		ret = EFalse;
    	}

    return ret;
	}

void MT_GSFramework::LaunchGsApp4L()
	{
	ikeySimulateCase = 4;
	iKeyNumber = 0;
    if ( !iPeriodic->IsActive() )
    {
    iPeriodic->Start( 2000000, 1000000, TCallBack( SimulateKeyEvent, static_cast<TAny*>( this ) ) );
    }

    CActiveScheduler::Start();
	}

TBool MT_GSFramework::GetKeyCodeCase4( TKeyCode& aKeyCode )
	{
	TBool ret = ETrue;
    iKeyNumber++;

    switch ( iKeyNumber )
    	{
    	case 1:
    		aKeyCode = EKeyEnter;	// open Persional
    		break;
    	case 2:
    		aKeyCode = EKeyDownArrow;
    		break;
    	case 3:
    		aKeyCode = EKeyDownArrow;
    		break;
    	case 4:
    		aKeyCode = EKeyEnter;	// open Standby
    		break;
    	case 5:
    		aKeyCode = EKeyEnter;	// open Shortcut
    		break;
    	case 6:
    		aKeyCode = EKeyEnter;	// open Lift idle softkey
    		break;
     	case 7:
    		aKeyCode = EKeyCBA2;
    		break;
    	case 8:
    		aKeyCode = EKeyCBA2;
    		break;
/*    	case 10:
    		aKeyCode = EKeyCBA2;
    		break;
    	case 11:
    		aKeyCode = EKeyCBA2;
    		break;*/
    	default:
    		ret = EFalse;
    	}

    return ret;
	}

// ----------------------------- CGSBaseView test cases ------------------------------------ //
void MT_GSFramework::SetupCGSBaseViewL()
	{
	iTestBaseView = CTestGSBaseView::NewL();
	}

void MT_GSFramework::TeardownCGSBaseView()
	{
	delete iTestBaseView;
	iTestBaseView = NULL;
	}

void MT_GSFramework::MT_CGSBaseView_SetCurrentItemL()
	{
	iTestBaseView->SetCurrentItem( 0 );
	}

void MT_GSFramework::MT_CGSBaseView_ContainerL()
	{
	iTestBaseView->Container();
	}

void MT_GSFramework::MT_CGSBaseView_SetNaviPaneLL()
	{
	iTestBaseView->SetNaviPaneL();
	}

void MT_GSFramework::MT_CGSBaseView_CreateNaviPaneContextLL()
	{
	iTestBaseView->CreateNaviPaneContextL( 1 );
	}




// -------------------------------	CGSParentPlugin test cases -------------------------------- //
void MT_GSFramework::SetupCGSParentPluginL()
	{
	iParentPlugin = static_cast<CGSParentPlugin*> ( CTestGSPlugin::NewL() );
	}

void MT_GSFramework::TeardownCGSParentPlugin()
	{
	delete iParentPlugin;
	iParentPlugin = NULL;
	}


void MT_GSFramework::MT_CGSParentPlugin_UpperLevelViewUidL()
	{
	EUNIT_ASSERT_NO_LEAVE( iParentPlugin->UpperLevelViewUid() );
	}

void MT_GSFramework::MT_CGSParentPlugin_UpdateViewL()
	{
	EUNIT_ASSERT_NO_LEAVE( iParentPlugin->UpdateView() );
	}

void MT_GSFramework::MT_CGSParentPlugin_SetOptionFlagsL()
	{
	TBitFlags optionFlags;
	EUNIT_ASSERT_NO_LEAVE( iParentPlugin->SetOptionFlags( optionFlags ) );
	}

void MT_GSFramework::MT_CGSParentPlugin_OptionFlagsL()
	{
	EUNIT_ASSERT_NO_LEAVE( iParentPlugin->OptionFlags() );
	}

void MT_GSFramework::MT_CGSParentPlugin_GetHelpContextL()
	{
	TCoeHelpContext context;
	EUNIT_ASSERT_NO_LEAVE( iParentPlugin->GetHelpContext( context ) );
	}

void MT_GSFramework::MT_CGSParentPlugin_SelectedPluginL()
	{
	CGSPluginInterface* plug = iParentPlugin->SelectedPlugin();
	EUNIT_ASSERT( plug == NULL );
	}

void MT_GSFramework::MT_CGSParentPlugin_TransferDynamicPluginLL()
	{
	TRAPD( err, iParentPlugin->TransferDynamicPluginL( NULL ) )
	EUNIT_PRINT( _L("error = %d"), err );
	}

// ---------------------------- CGSPluginInterface test cases -------------------------- //
void MT_GSFramework::MT_CGSPluginInterface_ItemTypeL()
	{
	CGSPluginInterface* plugInterface = static_cast<CGSPluginInterface*> ( iParentPlugin );
	EUNIT_ASSERT_NO_LEAVE( plugInterface->ItemType() );
	}

void MT_GSFramework::MT_CGSPluginInterface_HandleSelectionL()
	{
	CGSPluginInterface* plugInterface = static_cast<CGSPluginInterface*> ( iParentPlugin );
	TGSSelectionTypes selectionType = EGSSelectionBySelectionKey;
	EUNIT_ASSERT_NO_LEAVE( plugInterface->HandleSelection( selectionType ) );
	}

void MT_GSFramework::MT_CGSPluginInterface_PluginProviderCategoryL()
	{
	CGSPluginInterface* plugInterface = static_cast<CGSPluginInterface*> ( iParentPlugin );
	EUNIT_ASSERT_NO_LEAVE( plugInterface->PluginProviderCategory() );
	}

void MT_GSFramework::MT_CGSPluginInterface_CustomOperationLL()
	{
	CGSPluginInterface* plugInterface = static_cast<CGSPluginInterface*> ( iParentPlugin );
	TAny* any = plugInterface->CustomOperationL( NULL, NULL );
	EUNIT_ASSERT( any == NULL );
	}

void MT_GSFramework::MT_CGSPluginInterface_SetOrderL()
	{
	CGSPluginInterface* plugInterface = static_cast<CGSPluginInterface*> ( iParentPlugin );
	EUNIT_ASSERT_NO_LEAVE( plugInterface->SetOrder( 0 ) );
	}

void MT_GSFramework::MT_CGSPluginInterface_CreateIconLL()
	{
	CGSPluginInterface* plugInterface = static_cast<CGSPluginInterface*> ( iParentPlugin );
	CGulIcon* icon = plugInterface->CreateIconL( KGSIconTypeLbxItem );
	CleanupStack::PushL( icon );
	EUNIT_ASSERT( icon != NULL );
	CleanupStack::PopAndDestroy( icon );
	}


void MT_GSFramework::MT_CGSPluginInterface_GetEcomDestructorKey()
	{
	CGSPluginInterface* plugInterface = static_cast<CGSPluginInterface*> ( iParentPlugin );
	TUid dTorID;
	EUNIT_ASSERT_NO_LEAVE( dTorID = plugInterface->GetEcomDestructorKey() );
	}



/**
 * @param text1 Textual description of the test case.
 * @param text2 The class to be tested.
 * @param text3 The method to be tested.
 * @param text4 Test case type: refer to EUnit User Manual for correct values.
 * @param setupFunc Test setup function.
 * @param runFunc Test run function.
 * @param teardownFunc Test teardown function.
 */

//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    MT_GSFramework,
    "Tests for GSFramework.dll.",
    "MODULE" )

EUNIT_TEST(
    "#1 launch Gs application",
    "Settings framework classes",
    "settings framework api",
    "FUNCTIONALITY",
    SetupLaunch, LaunchGsApp1L, TeardownLaunch)

EUNIT_TEST(
    "#2 launch Gs application",
    "Settings framework classes",
    "settings framework api",
    "FUNCTIONALITY",
    SetupLaunch, LaunchGsApp2L, TeardownLaunch)

EUNIT_TEST(
    "#3 launch Gs application",
    "Settings framework classes",
    "settings framework api",
    "FUNCTIONALITY",
    SetupLaunch, LaunchGsApp3L, TeardownLaunch)

EUNIT_TEST(
    "#4 launch Gs application",
    "Settings framework classes",
    "settings framework api",
    "FUNCTIONALITY",
    SetupLaunch, LaunchGsApp4L, TeardownLaunch)

EUNIT_TEST(
    "GSBsView:SetCurrentItem",
    "CGSBaseView",
    "SetCurrentItem",
    "FUNCTIONALITY",
    SetupCGSBaseViewL, MT_CGSBaseView_SetCurrentItemL, TeardownCGSBaseView)

EUNIT_TEST(
    "GSBsView:Container",
    "CGSBaseView",
    "Container",
    "FUNCTIONALITY",
    SetupCGSBaseViewL, MT_CGSBaseView_ContainerL, TeardownCGSBaseView)

EUNIT_TEST(
    "GSBsView:SetNaviPaneL",
    "CGSBaseView",
    "SetNaviPaneL",
    "FUNCTIONALITY",
    SetupCGSBaseViewL, MT_CGSBaseView_SetNaviPaneLL, TeardownCGSBaseView)

EUNIT_TEST(
    "GSBsView:CreateNaviPaneContextL",
    "CGSBaseView",
    "CreateNaviPaneContextL",
    "FUNCTIONALITY",
    SetupCGSBaseViewL, MT_CGSBaseView_CreateNaviPaneContextLL, TeardownCGSBaseView)

EUNIT_TEST(
    "GSPrntPlg:UpperLevelViewUid",
    "CGSParentPlugin",
    "UpperLevelViewUid",
    "FUNCTIONALITY",
    SetupCGSParentPluginL, MT_CGSParentPlugin_UpperLevelViewUidL, TeardownCGSParentPlugin)

EUNIT_TEST(
    "GSPrntPlg:UpdateView",
    "CGSParentPlugin",
    "UpdateView",
    "FUNCTIONALITY",
    SetupCGSParentPluginL, MT_CGSParentPlugin_UpdateViewL, TeardownCGSParentPlugin)

EUNIT_TEST(
    "GSPrntPlg:SetOptionFlags",
    "CGSParentPlugin",
    "SetOptionFlags",
    "FUNCTIONALITY",
    SetupCGSParentPluginL, MT_CGSParentPlugin_SetOptionFlagsL, TeardownCGSParentPlugin)

EUNIT_TEST(
    "GSPrntPlg:OptionFlags",
    "CGSParentPlugin",
    "OptionFlags",
    "FUNCTIONALITY",
    SetupCGSParentPluginL, MT_CGSParentPlugin_OptionFlagsL, TeardownCGSParentPlugin)

EUNIT_TEST(
    "GSPrntPlg:GetHelpContext",
    "CGSParentPlugin",
    "GetHelpContext",
    "FUNCTIONALITY",
    SetupCGSParentPluginL, MT_CGSParentPlugin_GetHelpContextL, TeardownCGSParentPlugin)

EUNIT_TEST(
    "GSPrntPlg:SelectedPlugin",
    "CGSParentPlugin",
    "SelectedPlugin",
    "FUNCTIONALITY",
    SetupCGSParentPluginL, MT_CGSParentPlugin_SelectedPluginL, TeardownCGSParentPlugin)

EUNIT_TEST(
    "GSPrntPlg:TransferDynamicPluginL",
    "CGSParentPlugin",
    "TransferDynamicPluginL",
    "FUNCTIONALITY",
    SetupCGSParentPluginL, MT_CGSParentPlugin_TransferDynamicPluginLL, TeardownCGSParentPlugin)

EUNIT_TEST(
    "GSPlgIF:ItemType",
    "CGSPluginInterface",
    "ItemType",
    "FUNCTIONALITY",
    SetupCGSParentPluginL, MT_CGSPluginInterface_ItemTypeL, TeardownCGSParentPlugin)

EUNIT_TEST(
    "GSPlgIF:HandleSelection",
    "CGSPluginInterface",
    "HandleSelection",
    "FUNCTIONALITY",
    SetupCGSParentPluginL, MT_CGSPluginInterface_HandleSelectionL, TeardownCGSParentPlugin)

EUNIT_TEST(
    "GSPlgIF:PluginProviderCategory",
    "CGSPluginInterface",
    "PluginProviderCategory",
    "FUNCTIONALITY",
    SetupCGSParentPluginL, MT_CGSPluginInterface_PluginProviderCategoryL, TeardownCGSParentPlugin)

EUNIT_TEST(
    "GSPlgIF:CustomOperationL",
    "CGSPluginInterface",
    "CustomOperationL",
    "FUNCTIONALITY",
    SetupCGSParentPluginL, MT_CGSPluginInterface_CustomOperationLL, TeardownCGSParentPlugin)

EUNIT_TEST(
    "GSPlgIF:SetOrder",
    "CGSPluginInterface",
    "SetOrder",
    "FUNCTIONALITY",
    SetupCGSParentPluginL, MT_CGSPluginInterface_SetOrderL, TeardownCGSParentPlugin)

EUNIT_TEST(
    "GSPlgIF:CreateIconL",
    "CGSPluginInterface",
    "CreateIconL",
    "FUNCTIONALITY",
    SetupCGSParentPluginL, MT_CGSPluginInterface_CreateIconLL, TeardownCGSParentPlugin)

EUNIT_TEST(
    "GSPlgIF:GetEcomDestructorKey",
    "CGSPluginInterface",
    "GetEcomDestructorKey",
    "FUNCTIONALITY",
    SetupCGSParentPluginL, MT_CGSPluginInterface_GetEcomDestructorKey, TeardownCGSParentPlugin)

EUNIT_TEST(
    "GSTabHlpr:~CGSTabHelper",
    "CGSTabHelper",
    "~CGSTabHelper",
    "FUNCTIONALITY",
    SetupTabHelperL, T_CGSTabHelper__CGSTabHelperL, TeardownTabHelper)

EUNIT_TEST(
    "GSTabHlpr:CreateTabGroupL",
    "CGSTabHelper",
    "CreateTabGroupL",
    "FUNCTIONALITY",
    SetupTabHelperL, T_CGSTabHelper_CreateTabGroupLL, TeardownTabHelper)

EUNIT_TEST(
    "GSTabHlpr:CreateTabGroupL2",
    "CGSTabHelper",
    "CreateTabGroupL2",
    "FUNCTIONALITY",
    SetupTabHelperL, T_CGSTabHelper_CreateTabGroupL2L, TeardownTabHelper)

EUNIT_TEST(
    "GSTabHlpr:CreateTabGroupL3",
    "CGSTabHelper",
    "CreateTabGroupL3",
    "FUNCTIONALITY",
    SetupTabHelperL, T_CGSTabHelper_CreateTabGroupL3L, TeardownTabHelper)

EUNIT_TEST(
    "GSTabHlpr:RemoveTabGroupL",
    "CGSTabHelper",
    "RemoveTabGroupL",
    "FUNCTIONALITY",
    SetupTabHelperL, T_CGSTabHelper_RemoveTabGroupL, TeardownTabHelper)

EUNIT_TEST(
    "GSTabHlpr:TabChangedL",
    "CGSTabHelper",
    "TabChangedL",
    "FUNCTIONALITY",
    SetupTabHelperL, T_CGSTabHelper_TabChangedLL, TeardownTabHelper)

EUNIT_TEST(
    "GSTabHlpr:NewL",
    "CGSTabHelper",
    "NewL",
    "FUNCTIONALITY",
    SetupNone, T_CGSTabHelper_NewLL, TeardownNone)

EUNIT_TEST(
    "GSTabHlpr:NewLC",
    "CGSTabHelper",
    "NewLC",
    "FUNCTIONALITY",
    SetupNone, T_CGSTabHelper_NewLCL, TeardownNone)
/*
EUNIT_TEST(
    "FocusChanged",
    "CGSParentContainer",
    "FocusChanged",
    "FUNCTIONALITY",
    SetupParentContainerL, T_CGSParentContainer_FocusChanged, TearDownParentContainer)
*/

EUNIT_TEST(
    "GSPlgLdr:NewL",
    "CGSPluginLoader",
    "NewL",
    "FUNCTIONALITY",
    SetupNone, T_CGSPluginLoader_NewL, TeardownNone)

EUNIT_TEST(
    "GSPlgLdr:Destructor",
    "CGSPluginLoader",
    "~CGSPluginLoader",
    "FUNCTIONALITY",
    SetupNone, T_CGSPluginLoader__CGSPluginLoader, TeardownNone)

EUNIT_TEST(
    "GSPlgLdr:LoadAsyncL",
    "CGSPluginLoader",
    "LoadAsyncL",
    "FUNCTIONALITY",
    SetupPluginLoaderL, T_CGSPluginLoader_LoadAsyncL, TearDownPluginLoader)

EUNIT_TEST(
    "GSPlgLdr:SetObserver",
    "CGSPluginLoader",
    "SetObserver",
    "FUNCTIONALITY",
    SetupPluginLoaderL, T_CGSPluginLoader_SetObserver, TearDownPluginLoader)

EUNIT_TEST(
    "GSPlgLdr:AbortAsyncLoad",
    "CGSPluginLoader",
    "AbortAsyncLoad",
    "FUNCTIONALITY",
    SetupPluginLoaderL, T_CGSPluginLoader_AbortAsyncLoad, TearDownPluginLoader)

EUNIT_TEST(
    "GSPlgLdr:SortPluginsL",
    "CGSPluginLoader",
    "SortPluginsL",
    "FUNCTIONALITY",
    SetupPluginLoaderL, T_CGSPluginLoader_SortPluginsL, TearDownPluginLoader)

EUNIT_TEST(
    "GSPSLsnr:NewL",
    "CGSPubSubsListener",
    "NewL",
    "FUNCTIONALITY",
    SetupNone, T_CGSPubSubsListener_NewL, TeardownNone)

EUNIT_TEST(
    "GSPSLsnr:CGSPubSubsListener",
    "CGSPubSubsListener",
    "~CGSPubSubsListener",
    "FUNCTIONALITY",
    SetupNone, T_CGSPubSubsListener__CGSPubSubsListener, TeardownNone)

EUNIT_TEST(
    "GSPSLsnr:Get1",
    "CGSPubSubsListener",
    "Get1",
    "FUNCTIONALITY",
    SetupPubSubsListenerL, T_CGSPubSubsListener_Get1, TearDownPubSubsListener)

EUNIT_TEST(
    "GSPSLsnr:Get2",
    "CGSPubSubsListener",
    "Get2",
    "FUNCTIONALITY",
    SetupPubSubsListenerL, T_CGSPubSubsListener_Get2, TearDownPubSubsListener)

EUNIT_TEST(
    "GSPSLsnr:Get3",
    "CGSPubSubsListener",
    "Get3",
    "FUNCTIONALITY",
    SetupPubSubsListenerL, T_CGSPubSubsListener_Get3, TearDownPubSubsListener)

EUNIT_END_TEST_TABLE

//  END OF FILE
