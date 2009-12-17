/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
#include "T_CGSPluginInterface.h"

//  EXTERNAL INCLUDES
#include <EUnitMacros.h>
#include <EUnitDecorators.h>
#include <GSFWViewUIDs.h>
#include <ecom.h>

#include "GSWatchDog.h"

//  INTERNAL INCLUDES



CGSPluginIndexer* CGSPluginIndexer::NewL( TUid aInterface )
    {
    CGSPluginIndexer* self = new( ELeave ) CGSPluginIndexer( aInterface );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

CGSPluginIndexer::CGSPluginIndexer( TUid aInterface )
    : iInterfaceUid( aInterface)
    {
    }


CGSPluginIndexer::~CGSPluginIndexer()
    {
    iImplInfoArray.ResetAndDestroy();// This is needed
    iImplInfoArray.Close();
    }


void CGSPluginIndexer::ConstructL()
    {
    REComSession::ListImplementationsL( iInterfaceUid,
                                        iImplInfoArray );
    iIndex = 0;
    }


TBool CGSPluginIndexer::HasNext()
    {
    if( iImplInfoArray.Count() >= iIndex + 1 )
        {
        return ETrue;
        }
    else
        {
        EUNIT_PRINT( _L("CGSPluginIndexer(%d/%d)"), iIndex, iImplInfoArray.Count() );
        return EFalse;
        }
    }


TUid CGSPluginIndexer::NextUid()
    {
    EUNIT_PRINT( _L("NextUid(%d/%d)"), iIndex, iImplInfoArray.Count() );
    return iImplInfoArray[ iIndex++ ]->ImplementationUid();
    }


void CGSPluginIndexer::ResetIndex()
    {
    iIndex = 0;
    }


TInt CGSPluginIndexer::Index()
    {
    return iIndex;
    }

TInt CGSPluginIndexer::Count()
    {
    return iImplInfoArray.Count();
    }

///////////////////////////////////////////////////////////////


// CONSTRUCTION
T_CGSPluginInterface* T_CGSPluginInterface::NewL()
    {
    T_CGSPluginInterface* self = T_CGSPluginInterface::NewLC();
    CleanupStack::Pop();
    return self;
    }

T_CGSPluginInterface* T_CGSPluginInterface::NewLC()
    {
    T_CGSPluginInterface* self = new( ELeave ) T_CGSPluginInterface();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// Destructor (virtual by CBase)
T_CGSPluginInterface::~T_CGSPluginInterface()
    {
    delete iPluginIndexer;
    }

// Default constructor
T_CGSPluginInterface::T_CGSPluginInterface()
    {
    }

// Second phase construct
void T_CGSPluginInterface::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();

    // Create iterator for plugins:
    iPluginIndexer = CGSPluginIndexer::NewL( KGSPluginInterfaceUid );
    }

//  METHODS


void T_CGSPluginInterface::SetupL(  )
    {
    if( !iPluginIndexer->HasNext() )
        {
        iPluginIndexer->ResetIndex();
        }
    }

void T_CGSPluginInterface::Teardown(  )
    {
//    REComSession::DestroyedImplementation( iCGSPluginInterface->DestructorID() );
//    delete iCGSPluginInterface;
//    iCGSPluginInterface = NULL;
//    REComSession::FinalClose();
    }


void T_CGSPluginInterface::T_CGSPluginInterface_SetToBlacklistL()
    {
    TUid toBeBlacklisted = {  0x10282E06 };
    EUNIT_PRINT( _L("Setting 0x%X to blacklist..."), toBeBlacklisted.iUid );

    CGSWatchDog* dog =  CGSWatchDog::NewL();
    CleanupStack::PushL( dog );
    dog->QuarantineL( toBeBlacklisted );
    // Destroying watchdog without removing from quarantine will blacklist the 
    // plugin.
    CleanupStack::PopAndDestroy( dog );
    
    dog =  CGSWatchDog::NewL();
    CleanupStack::PushL( dog );
    dog->QuarantineL( toBeBlacklisted );
    // Destroying watchdog without removing from quarantine will blacklist the 
    // plugin. Do this twice as dog does not blacklist for the first run.
    CleanupStack::PopAndDestroy( dog );
    
    dog =  CGSWatchDog::NewL();
    CleanupStack::PushL( dog );
    
    EUNIT_ASSERT_DESC( dog->IsInBlackList( toBeBlacklisted ), 
            "The plugin should be in the blacklist");
    
    CleanupStack::PopAndDestroy( dog );
    EUNIT_PRINT( _L("setting to blacklist done.") );
    }


void T_CGSPluginInterface::T_CGSPluginInterface_ResetBlacklistL()
    {
    EUNIT_PRINT( _L("Resetting blacklist...") );
    TInt runs = KGSDefaultMaxPluginBlackListedRuns + 1;
    for( TInt i = 0; i < runs; i++ )
        {
        CGSWatchDog* dog =  CGSWatchDog::NewL();
        CleanupStack::PushL( dog );
        dog->ReportCleanExitL();
        CleanupStack::PopAndDestroy( dog );
        }
    EUNIT_PRINT( _L("Resetting blacklist done.") );
    }


void T_CGSPluginInterface::T_CGSPluginInterface_CheckHeapL()
    {
    EUNIT_PRINT( _L("CheckHeapL...") );
    RHeap heap = User::Heap();
    TInt maxBlock;
    TInt free;
    TInt delta;
    TBuf<256> buf;
    TUid plgId;
    _LIT( KCaptionErr, "Caption unavailable");

    CGSWatchDog* dog =  CGSWatchDog::NewL();
    CleanupStack::PushL( dog );

    while( iPluginIndexer->HasNext() )
        {
        EUNIT_PRINT( _L("Heap test (%d/%d)..."), iPluginIndexer->Index()+1, iPluginIndexer->Count() );
        plgId = iPluginIndexer->NextUid();
        if( dog->IsInBlackList( plgId ) )
            {
            EUNIT_PRINT( _L("Blacklisted: 0x%X"), plgId.iUid );
            continue;
            }
        dog->QuarantineL( plgId );

        free = heap.Available( maxBlock );

        CGSPluginInterface* plugin = CGSPluginInterface::NewL( plgId, NULL );
        TRAPD(
            err,
            plugin->GetCaptionL( buf );
            );
        if( err )
            {
            buf = KCaptionErr;
            }
        EUNIT_PRINT( _L("Releasing (0x%X %S)..."), plgId.iUid, &buf );
        REComSession::DestroyedImplementation( plugin->DestructorID() );
        EUNIT_PRINT( _L("Deleting (0x%X %S)..."), plgId.iUid, &buf );
        delete plugin;
        plugin = NULL;
        EUNIT_PRINT( _L("REComSession::FinalClose()...") );
        REComSession::FinalClose();

        delta = heap.Available( maxBlock ) - free;
        
        EUNIT_PRINT( _L("Plugin heap alloc (%S, 0x%X) delta:%d"), &buf, plgId.iUid, delta );
        if( delta > 0 )
            {
            EUNIT_PRINT( _L("MEMORY LEAK: %S(0x%X) delta:%d"), &buf, plgId.iUid, delta );
            }
        dog->RemoveFromQuarantineL( plgId );
        }
    dog-> ReportCleanExitL();
    CleanupStack::PopAndDestroy( dog );
    EUNIT_ASSERT_DESC( ETrue, "Wow, the test ran here without a crash!");
    EUNIT_PRINT( _L("CheckHeapL finished.") );
    }


void T_CGSPluginInterface::T_CGSPluginInterface__CGSPluginInterfaceL(  )
    {
//    iCGSPluginInterface->~CGSPluginInterface( );
     EUNIT_ASSERT_DESC( EFalse, "Generated assert, replace with real");
    }

void T_CGSPluginInterface::T_CGSPluginInterface_GetCaptionLL(  )
    {
    // iCGSPluginInterface->GetCaptionL( <add parameters here > );
    EUNIT_PRINT( _L("Caption test...") );
    EUNIT_ASSERT_DESC( /*EFalse*/ETrue, "Generated assert, replace with real");
    }

void T_CGSPluginInterface::T_CGSPluginInterface_GetValueL(  )
    {
    // iCGSPluginInterface->GetValue( <add parameters here > );
     EUNIT_ASSERT_DESC( EFalse, "Generated assert, replace with real");
    }

void T_CGSPluginInterface::T_CGSPluginInterface_HandleSelectionL(  )
    {
    // iCGSPluginInterface->HandleSelection( <add parameters here > );
     EUNIT_ASSERT_DESC( EFalse, "Generated assert, replace with real");
    }

void T_CGSPluginInterface::T_CGSPluginInterface_ItemTypeL(  )
    {
    iCGSPluginInterface->ItemType( );
     EUNIT_ASSERT_DESC( EFalse, "Generated assert, replace with real");
    }

void T_CGSPluginInterface::T_CGSPluginInterface_MenuActivationItemL(  )
    {
    iCGSPluginInterface->MenuActivationItem( );
     EUNIT_ASSERT_DESC( EFalse, "Generated assert, replace with real");
    }

void T_CGSPluginInterface::T_CGSPluginInterface_CreateIconLL(  )
    {
    // iCGSPluginInterface->CreateIconL( <add parameters here > );
     EUNIT_ASSERT_DESC( EFalse, "Generated assert, replace with real");
    }

void T_CGSPluginInterface::T_CGSPluginInterface_PluginProviderCategoryL(  )
    {
    iCGSPluginInterface->PluginProviderCategory( );
     EUNIT_ASSERT_DESC( EFalse, "Generated assert, replace with real");
    }

void T_CGSPluginInterface::T_CGSPluginInterface_CustomOperationLL(  )
    {
    // iCGSPluginInterface->CustomOperationL( <add parameters here > );
     EUNIT_ASSERT_DESC( EFalse, "Generated assert, replace with real");
    }

void T_CGSPluginInterface::T_CGSPluginInterface_VisibleL(  )
    {
    iCGSPluginInterface->Visible( );
     EUNIT_ASSERT_DESC( EFalse, "Generated assert, replace with real");
    }

void T_CGSPluginInterface::T_CGSPluginInterface_ResetSelectedItemIndexL(  )
    {
    iCGSPluginInterface->ResetSelectedItemIndex( );
     EUNIT_ASSERT_DESC( EFalse, "Generated assert, replace with real");
    }

void T_CGSPluginInterface::T_CGSPluginInterface_SetOrderL(  )
    {
    // iCGSPluginInterface->SetOrder( <add parameters here > );
     EUNIT_ASSERT_DESC( EFalse, "Generated assert, replace with real");
    }


//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    T_CGSPluginInterface,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "~CGSPluginInterface - test0",
    "CGSPluginInterface",
    "~CGSPluginInterface - test0",
    "FUNCTIONALITY",
    SetupL, T_CGSPluginInterface__CGSPluginInterfaceL, Teardown)

EUNIT_TEST(
    "GetCaptionL - test1",
    "CGSPluginInterface",
    "GetCaptionL - test1",
    "FUNCTIONALITY",
    SetupL, T_CGSPluginInterface_GetCaptionLL, Teardown)

EUNIT_TEST(
    "GetValue - test2",
    "CGSPluginInterface",
    "GetValue - test2",
    "FUNCTIONALITY",
    SetupL, T_CGSPluginInterface_GetValueL, Teardown)

EUNIT_TEST(
    "HandleSelection - test3",
    "CGSPluginInterface",
    "HandleSelection - test3",
    "FUNCTIONALITY",
    SetupL, T_CGSPluginInterface_HandleSelectionL, Teardown)

EUNIT_TEST(
    "ItemType - test4",
    "CGSPluginInterface",
    "ItemType - test4",
    "FUNCTIONALITY",
    SetupL, T_CGSPluginInterface_ItemTypeL, Teardown)

EUNIT_TEST(
    "MenuActivationItem - test5",
    "CGSPluginInterface",
    "MenuActivationItem - test5",
    "FUNCTIONALITY",
    SetupL, T_CGSPluginInterface_MenuActivationItemL, Teardown)

EUNIT_TEST(
    "CreateIconL - test6",
    "CGSPluginInterface",
    "CreateIconL - test6",
    "FUNCTIONALITY",
    SetupL, T_CGSPluginInterface_CreateIconLL, Teardown)

EUNIT_TEST(
    "PluginProviderCategory - test7",
    "CGSPluginInterface",
    "PluginProviderCategory - test7",
    "FUNCTIONALITY",
    SetupL, T_CGSPluginInterface_PluginProviderCategoryL, Teardown)

EUNIT_TEST(
    "CustomOperationL - test8",
    "CGSPluginInterface",
    "CustomOperationL - test8",
    "FUNCTIONALITY",
    SetupL, T_CGSPluginInterface_CustomOperationLL, Teardown)

EUNIT_TEST(
    "Visible - test9",
    "CGSPluginInterface",
    "Visible - test9",
    "FUNCTIONALITY",
    SetupL, T_CGSPluginInterface_VisibleL, Teardown)

EUNIT_TEST(
    "ResetSelectedItemIndex - test10",
    "CGSPluginInterface",
    "ResetSelectedItemIndex - test10",
    "FUNCTIONALITY",
    SetupL, T_CGSPluginInterface_ResetSelectedItemIndexL, Teardown)

EUNIT_TEST(
    "SetOrder - test11",
    "CGSPluginInterface",
    "SetOrder - test11",
    "FUNCTIONALITY",
    SetupL, T_CGSPluginInterface_SetOrderL, Teardown)

EUNIT_TEST(
        "Check heap - test12",
        "CGSPluginInterface",
        "Check heap - test12",
        "FUNCTIONALITY",
        SetupL, T_CGSPluginInterface_CheckHeapL, Teardown)

EUNIT_TEST(
        "Reset blacklist - test13",
        "CGSPluginInterface",
        "Reset blacklist - test13",
        "FUNCTIONALITY",
        SetupL, T_CGSPluginInterface_ResetBlacklistL, Teardown)

        EUNIT_TEST(
        "Set to blacklist - test14",
        "CGSPluginInterface",
        "Set to blacklist - test14",
        "FUNCTIONALITY",
        SetupL, T_CGSPluginInterface_SetToBlacklistL, Teardown)

        

EUNIT_END_TEST_TABLE








//  END OF FILE
