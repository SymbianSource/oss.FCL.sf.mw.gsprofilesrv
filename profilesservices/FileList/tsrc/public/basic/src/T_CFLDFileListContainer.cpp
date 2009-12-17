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

//  Include Files
#include "T_CFLDFileListContainer.h"
#include <CFLDFileListContainer.h>
#include <EUnitMacros.h>
#include <TEUnitAssertionInfo.h>
#include <CEUnitAllocTestCaseDecorator.h>
#include <coemain.h>
#include <FLDTest.rsg>
#include "FLDTestConst.h"


namespace
	{
	_LIT( KFLDTestMimeMP4, "audio/mp4" );
	_LIT( KFLDTestMime3GPP, "video/3gpp" );
	_LIT( KFLDTestMimeRNG, "application/vnd.nokia.ringing-tone" );
	}


// Classes under test include

//  LOCAL FUNCTIONS

// CONSTRUCTION
// Static constructor
T_CFLDFileListContainer* T_CFLDFileListContainer::NewLC()
    {
    T_CFLDFileListContainer* self = new(ELeave) T_CFLDFileListContainer;
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// Destructor (virtual by CBase)
T_CFLDFileListContainer::~T_CFLDFileListContainer()
    {
    delete iContainer;
    delete iContainer1;
    if( iResourceLoader )
        {
        iResourceLoader->Close();
        delete iResourceLoader;
        }
    iWsSession.Close();
    }

// Second phase construct
void T_CFLDFileListContainer::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }

T_CFLDFileListContainer::T_CFLDFileListContainer()
    {
    }

//---------------------------------------------------------------
//----------------- TEST CASE METHODS ---------------------------
//---------------------------------------------------------------

//  METHODS

// Setup nothing.
void T_CFLDFileListContainer::EmptySetupL()
    {
    iResourceLoader = new ( ELeave ) RConeResourceLoader( *CCoeEnv::Static() );
    TFileName fileName( KFLDResourceFile );
    User::LeaveIfError( iResourceLoader->Open( fileName ) );
    }

void T_CFLDFileListContainer::SetupL()
    {
    EmptySetupL();
	
    iContainer = CFLDFileListContainer::NewL();
    iContainer1 = CFLDFileListContainer::NewLC();
    CleanupStack::Pop();
    iContainer2 = CFLDFileListContainer::NewL( R_FLD_TEST_MODEL1 );
    iContainer3 = CFLDFileListContainer::NewLC( R_FLD_TEST_MODEL1 );
    CleanupStack::Pop();
    iContainer4 = CFLDFileListContainer::NewL( R_FLD_TEST_MODEL1, R_FLD_TEST_ROM_MODEL );
    iContainer5 = CFLDFileListContainer::NewLC( R_FLD_TEST_MODEL1, R_FLD_TEST_ROM_MODEL );
    CleanupStack::Pop();
    
    iWsSession.Connect();

    iMimeTypeArray = new (ELeave) CDesCArrayFlat( 8 );
    }

void T_CFLDFileListContainer::Teardown()
    {
    delete iContainer;
    iContainer = NULL;
    
    delete iContainer1;
    iContainer1 = NULL;
    
    delete iContainer2;
    iContainer2 = NULL;
    
    delete iContainer3;
    iContainer3 = NULL;
    
    delete iContainer4;
    iContainer4 = NULL;
    
    delete iContainer5;
    iContainer5 = NULL;
    
    if( iResourceLoader )
        {
        iResourceLoader->Close();
        delete iResourceLoader;
        iResourceLoader = NULL;
        }
    iWsSession.Close();

    delete iMimeTypeArray;
    iMimeTypeArray = NULL;

    iMediaTypeArray.Reset();
    iMediaTypeArray.Close();
    }

// CREATIONAL TESTS
void T_CFLDFileListContainer::TestCreateObjectL()
    {
    CFLDFileListContainer* container = CFLDFileListContainer::NewL();
    EUNIT_ASSERT( container );
    delete container;
    container = NULL;

    container = CFLDFileListContainer::NewLC();
    EUNIT_ASSERT( container );
    CleanupStack::PopAndDestroy();
    }

void T_CFLDFileListContainer::LaunchTestL()
    {
    _LIT( KFLDTestFileName, "" );
    _LIT( KFLDTestPopupTitle, "PopupTitle" );
    TFileName fn( KFLDTestFileName );

    iContainer->LaunchL( fn );
    iContainer->LaunchL( fn, KFLDTestPopupTitle );
    }

void T_CFLDFileListContainer::InsertNullItemTestL()
    {
    iContainer->InsertNullItemL( KFLDTestNullItem );
    iContainer->InsertNullItemL( KFLDTestNullItem, KFLDTestNullItemRingingTone );
    }

void T_CFLDFileListContainer::InsertEndNullItemTestL()
    {
    iContainer->InsertEndNullItemL( KFLDTestNullItem );
    iContainer->InsertEndNullItemL( KFLDTestNullItem, KFLDTestNullItemRingingTone );
    }

void T_CFLDFileListContainer::SetDelayTestL()
    {
    __UHEAP_MARK;
    iContainer->SetDelay( 0 );
    iContainer->SetDelay( 1000000 );
    __UHEAP_MARKEND;
    }

void T_CFLDFileListContainer::SetVolumeTestL()
    {
    __UHEAP_MARK;
    iContainer->SetVolume( 5 );
    __UHEAP_MARKEND;
    }

void T_CFLDFileListContainer::SetRingingTypeTestL()
    {
    __UHEAP_MARK;
    iContainer->SetRingingType( 0 );
    __UHEAP_MARKEND;
    }

void T_CFLDFileListContainer::SetVibraTestL()
    {
    __UHEAP_MARK;
    iContainer->SetVibra( EFalse );
    iContainer->SetVibra( ETrue );
    __UHEAP_MARKEND;
    }

void T_CFLDFileListContainer::Set3dEffectsTestL()
    {
    __UHEAP_MARK;
    iContainer->Set3dEffects( EFalse );
    __UHEAP_MARKEND;
    }

void T_CFLDFileListContainer::SetAutomatedTypeTestL()
    {
    __UHEAP_MARK;
    iContainer->SetAutomatedType( CDRMHelper::EAutomatedTypeRingingTone );
    __UHEAP_MARKEND;
    }

void T_CFLDFileListContainer::SetMaxFileSizeTestL()
    {
    __UHEAP_MARK;
    iContainer->SetMaxFileSize( 0 );
    __UHEAP_MARKEND;
    }

void T_CFLDFileListContainer::AddExclusiveMimeTypeTestL()
    {
    iContainer->AddExclusiveMimeTypeL( KFLDTestMimeRNG );
    iContainer->AddExclusiveMimeTypeL( KFLDTestMimeMP4 );
    iContainer->AddExclusiveMimeTypeL( KFLDTestMime3GPP );

    iContainer->ResetExclusiveMimeTypes();
    }

void T_CFLDFileListContainer::AddExclusiveMediaTypeTestL()
    {
    iContainer->AddExclusiveMediaTypeL( ECLFMediaTypeSound );
    iContainer->AddExclusiveMediaTypeL( ECLFMediaTypeMusic );
    iContainer->AddExclusiveMediaTypeL( ECLFMediaTypeVideo );

    iContainer->ResetExclusiveMediaTypes();
    }

void T_CFLDFileListContainer::ResetExclusiveMimeTypesTestL()
	{
    iContainer->AddExclusiveMimeTypeL( KFLDTestMimeRNG );
    iContainer->ResetExclusiveMimeTypes();
	}

void T_CFLDFileListContainer::ResetExclusiveMediaTypesTestL()
	{
    iContainer->AddExclusiveMediaTypeL( ECLFMediaTypeSound );
    iContainer->ResetExclusiveMediaTypes();
	}

void T_CFLDFileListContainer::SetWantedMimeTypesTestL()
	{
    iContainer->AddExclusiveMediaTypeL( ECLFMediaTypeSound );
    iContainer->AddExclusiveMediaTypeL( ECLFMediaTypeMusic );
    iContainer->AddExclusiveMediaTypeL( ECLFMediaTypeVideo );

	iMimeTypeArray->Reset();
    iMimeTypeArray->AppendL( _L("*") );
    iContainer->SetWantedMimeTypesL( *iMimeTypeArray );
	}

void T_CFLDFileListContainer::SetWantedMediaTypesTestL()
	{
    iContainer->AddExclusiveMediaTypeL( ECLFMediaTypeSound );
    iContainer->AddExclusiveMediaTypeL( ECLFMediaTypeMusic );
    iContainer->AddExclusiveMediaTypeL( ECLFMediaTypeVideo );
    
    iMediaTypeArray.AppendL( ECLFMediaTypeSound );
    iMediaTypeArray.AppendL( ECLFMediaTypeMusic );
    iContainer->SetWantedMediaTypesL( iMediaTypeArray.Array() );
	}



// Test case table for this test suite class
EUNIT_BEGIN_TEST_TABLE(
    T_CFLDFileListContainer,
    "T_CFLDFileListContainer test suite",
    "MODULE" )

EUNIT_TEST(
    "Create and delete",
    "CFLDFileListContainer",
    "NewL",
    "FUNCTIONALITY",
    EmptySetupL, TestCreateObjectL, Teardown )
EUNIT_TEST(
    "Launch, CAknPopupList causes resource imbalance",
    "CFLDFileListContainer",
    "LaunchL",
    "FUNCTIONALITY",
    SetupL, LaunchTestL, Teardown )
EUNIT_TEST(
    "Insert null item",
    "CFLDFileListContainer",
    "InsertNullItemL",
    "FUNCTIONALITY",
    SetupL, InsertNullItemTestL, Teardown )
EUNIT_TEST(
    "Insert end null item",
    "CFLDFileListContainer",
    "InsertEndNullItemL",
    "FUNCTIONALITY",
    SetupL, InsertEndNullItemTestL, Teardown )
EUNIT_TEST(
    "Set delay",
    "CFLDFileListContainer",
    "SetDelay",
    "FUNCTIONALITY",
    SetupL, SetDelayTestL, Teardown )
EUNIT_TEST(
    "Set volume",
    "CFLDFileListContainer",
    "SetVolume",
    "FUNCTIONALITY",
    SetupL, SetVolumeTestL, Teardown )
EUNIT_TEST(
    "Set Ringing type",
    "CFLDFileListContainer",
    "SetRingingType",
    "FUNCTIONALITY",
    SetupL, SetRingingTypeTestL, Teardown )
EUNIT_TEST(
    "Set Vibra",
    "CFLDFileListContainer",
    "SetVibra",
    "FUNCTIONALITY",
    SetupL, SetVibraTestL, Teardown )
EUNIT_TEST(
    "Set automated type",
    "CFLDFileListContainer",
    "SetAutomatedType",
    "FUNCTIONALITY",
    SetupL, SetAutomatedTypeTestL, Teardown )
EUNIT_TEST(
    "Set 3d effects",
    "CFLDFileListContainer",
    "Set3dEffects",
    "FUNCTIONALITY",
    SetupL, Set3dEffectsTestL, Teardown )
EUNIT_TEST(
    "Set max file size",
    "CFLDFileListContainer",
    "SetMaxFileSize",
    "FUNCTIONALITY",
    SetupL, SetMaxFileSizeTestL, Teardown )
EUNIT_TEST(
    "Add exclusive MIME type",
    "CFLDFileListContainer",
    "AddExclusiveMimeType",
    "FUNCTIONALITY",
    SetupL, AddExclusiveMimeTypeTestL, Teardown )
EUNIT_TEST(
    "Add exclusive media type",
    "CFLDFileListContainer",
    "AddExclusiveMediaType",
    "FUNCTIONALITY",
    SetupL, AddExclusiveMediaTypeTestL, Teardown )
EUNIT_TEST(
    "Set wanted MIME types",
    "CFLDFileListContainer",
    "SetWantedMimeTypes",
    "FUNCTIONALITY",
    SetupL, SetWantedMimeTypesTestL, Teardown )
EUNIT_TEST(
    "Set wanted media types",
    "CFLDFileListContainer",
    "SetWantedMediaTypes",
    "FUNCTIONALITY",
    SetupL, SetWantedMediaTypesTestL, Teardown )
EUNIT_TEST(
    "Reset exclusive MIME types",
    "CFLDFileListContainer",
    "ResetExclusiveMimeTypes",
    "FUNCTIONALITY",
    SetupL, ResetExclusiveMimeTypesTestL, Teardown )
EUNIT_TEST(
    "Reset exclusive media types",
    "CFLDFileListContainer",
    "ResetExclusiveMediaTypes",
    "FUNCTIONALITY",
    SetupL, ResetExclusiveMediaTypesTestL, Teardown )

EUNIT_END_TEST_TABLE

//  End of File
