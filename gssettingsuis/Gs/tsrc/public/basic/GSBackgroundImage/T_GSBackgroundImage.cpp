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
#include "T_GSBackgroundImage.h"

//  EXTERNAL INCLUDES
#include <EUnitMacros.h>
#include <EUnitDecorators.h>
#include <e32base.h>

//  INTERNAL INCLUDES
//#include "BackgroundImage.cpp"

// CONSTANTS
_LIT( KGSTestEmptyString, "" );
_LIT( KGSTestDestinationPath, "c:\\data\\bin\\test_path");

// CONSTRUCTION
T_GSBackgroundImage* T_GSBackgroundImage::NewL()
    {
    T_GSBackgroundImage* self = T_GSBackgroundImage::NewLC();
    CleanupStack::Pop();

    return self;
    }

T_GSBackgroundImage* T_GSBackgroundImage::NewLC()
    {
    T_GSBackgroundImage* self = new( ELeave ) T_GSBackgroundImage();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
T_GSBackgroundImage::~T_GSBackgroundImage()
    {
    }

// Default constructor
T_GSBackgroundImage::T_GSBackgroundImage()
    {
    }

// Second phase construct
void T_GSBackgroundImage::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void T_GSBackgroundImage::EmptySetupL(  )
    {
    }

void T_GSBackgroundImage::EmptyTeardown(  )
	{
	}
	
void T_GSBackgroundImage::SetupBackgroundImageL()
    {
    iFs.Connect();
    iCGSBackgroundImage = CGSBackgroundImage::NewL();
    }
    
void T_GSBackgroundImage::SetupBackgroundImageAndAsyncHandlerL()
    {
    iFs.Connect();
    iCGSBackgroundImage = CGSBackgroundImage::NewL();
    iImgHandler = CGSAsyncImageHandling::NewL( iFs,
                this, KGSTestDestinationPath );
    }

void T_GSBackgroundImage::Teardown(  )
    {
    if( iCGSBackgroundImage )
        {
        delete iCGSBackgroundImage;
        iCGSBackgroundImage = NULL;
        }
    iFs.Close();
    
    if( iImgHandler )
        {
        iImgHandler->Cancel();
        delete iImgHandler;
        iImgHandler = NULL;
        }
    }
    

void T_GSBackgroundImage::T_Global_BackgroundImage_NewL(  )
    {
    CGSBackgroundImage* image = CGSBackgroundImage::NewL();
    CleanupStack::PushL( image );
    EUNIT_ASSERT( image != NULL );
    CleanupStack::PopAndDestroy( image );
    }
    
void T_GSBackgroundImage::T_Global_BackgroundImage_GetPlnsStatus()
    {
    TInt returnValue;
    EUNIT_ASSERT_NO_LEAVE( returnValue = iCGSBackgroundImage->GetPlnsStatus() );
    EUNIT_PRINT( _L("returnValue = %d"), returnValue );
    }

void T_GSBackgroundImage::T_Global_BackgroundImage_BackgroundImageL()
    {
    TInt returnValue;
    EUNIT_ASSERT_NO_LEAVE( returnValue = iCGSBackgroundImage->BackgroundImageL() );
    EUNIT_PRINT( _L("returnValue = %d"), returnValue );
    }

void T_GSBackgroundImage::T_Global_BackgroundImage_SetBackgroundImageL()
    {
    TInt setValue = 1;
    EUNIT_ASSERT_NO_LEAVE( iCGSBackgroundImage->SetBackgroundImageL( 
                                                           setValue ) );
    }

void T_GSBackgroundImage::T_Global_BackgroundImage_SetBackgroundImagePathL()
    {
    EUNIT_ASSERT_NO_LEAVE( iCGSBackgroundImage->SetBackgroundImagePathL( 
            KGSTestEmptyString ) );
    }

void T_GSBackgroundImage::T_Global_BackgroundImage_SetBackgroundImagePath2L()
    {
    TInt desLenght = 25;
    RBuf rbuf;
    rbuf.CreateL( KGSTestDestinationPath );
    rbuf.CleanupClosePushL();
    //TBuf<desLenght> destinationPath( KGSTestDestinationPath );
    EUNIT_ASSERT_NO_LEAVE(iCGSBackgroundImage->SetBackgroundImagePathL( 
            KGSTestEmptyString, rbuf ) );
    CleanupStack::PopAndDestroy();
    }

void T_GSBackgroundImage::T_Global_BackgroundImage_SetBackgroundImagePath3L()
    {
    TGSWallpaperType wallType = EGSWallpaperIdle;
    RBuf rbuf;
    rbuf.CreateL( KGSTestDestinationPath );
    rbuf.CleanupClosePushL();
    EUNIT_ASSERT_NO_LEAVE(iCGSBackgroundImage->SetBackgroundImagePathL( 
            KGSTestEmptyString, rbuf, wallType ) );
    CleanupStack::PopAndDestroy();
    }

void T_GSBackgroundImage::T_Global_BackgroundImage_CreateImageHandlingAsyncObjectL()
    {
    EUNIT_ASSERT_NO_LEAVE( iImgHandler = CGSAsyncImageHandling::NewL( iFs,
            this, KGSTestDestinationPath ) );
    }


void T_GSBackgroundImage::T_Global_BackgroundImage_SetWelcomeNoteImageL()
    {
    EUNIT_ASSERT_NO_LEAVE( iCGSBackgroundImage->
            SetWelcomeNoteImageL( KGSTestDestinationPath, *iImgHandler ) );
    }

void T_GSBackgroundImage::T_Global_BackgroundImage_SetVtStillImagePathL()
    {
    EUNIT_ASSERT_NO_LEAVE( iCGSBackgroundImage->SetVTStillImagePathL( 
            KGSTestDestinationPath, *iImgHandler ) );
    }

void T_GSBackgroundImage::T_Global_BackgroundImage_IsPersonalisationApplicationL()
    {
    EUNIT_ASSERT_NO_LEAVE( iCGSBackgroundImage->
            IsPersonalisationApplicationL() );
    }

void T_GSBackgroundImage::T_Global_BackgroundImage_DeleteImageL()
    {
    EUNIT_ASSERT_LEAVE( iCGSBackgroundImage->
            DeleteImageL( KGSTestDestinationPath ) );
    }

void T_GSBackgroundImage::ImageHandlingCompleteL( TInt /*aError*/ )
    {
    
    }

//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    T_GSBackgroundImage,
    "Testing BackgroundImage.dll functionality",
    "UNIT" )

EUNIT_TEST(
    "BackgroundImage - NewL construction", //case description
    "CGSBackgroundImage",                   //class being tested
    "NewL",                                 //method being tested
    "FUNCTIONALITY",                        //FUNCTIONALITY, BOUNDARY, ERRORHANDLING
    EmptySetupL, T_Global_BackgroundImage_NewL, EmptyTeardown )

EUNIT_TEST(
    "BackgroundImage - GetPlnsStatus()",
    "CGSBackgroundImage",                
    "GetPlnsStatus",                     
    "FUNCTIONALITY",                     
    SetupBackgroundImageL, T_Global_BackgroundImage_GetPlnsStatus, Teardown )    

EUNIT_TEST(
    "BackgroundImage - BackgroundImage()",
    "CGSBackgroundImage",                
    "BackgroundImage",                     
    "FUNCTIONALITY",                     
    SetupBackgroundImageL, T_Global_BackgroundImage_BackgroundImageL, Teardown )
    
EUNIT_TEST(
    "BackgroundImage - SetBackgroundImageL()",
    "CGSBackgroundImage",                
    "SetBackgroundImageL",                     
    "FUNCTIONALITY",                     
    SetupBackgroundImageL, T_Global_BackgroundImage_SetBackgroundImageL, Teardown )
    
EUNIT_TEST(
    "BackgroundImage - SetBackgroundImagePathL()",
    "CGSBackgroundImage",                
    "SetBackgroundImagePathL",                     
    "FUNCTIONALITY",                     
    SetupBackgroundImageL, T_Global_BackgroundImage_SetBackgroundImagePathL, Teardown )
    
EUNIT_TEST(
    "BackgroundImage - SetBackgroundImagePath2L()",
    "CGSBackgroundImage",                
    "Overloaded version of SetBackgroundImagePathL",                     
    "FUNCTIONALITY",                     
    SetupBackgroundImageL, T_Global_BackgroundImage_SetBackgroundImagePath2L, Teardown ) 
    
EUNIT_TEST(
    "BackgroundImage - SetBackgroundImagePath3L()",
    "CGSBackgroundImage",                
    "Second overloaded version of SetBackgroundImagePathL",                     
    "FUNCTIONALITY",                     
    SetupBackgroundImageL, T_Global_BackgroundImage_SetBackgroundImagePath3L, Teardown )
    
EUNIT_TEST(
    "BackgroundImage - IsPersonalisationApplicationL()",
    "CGSBackgroundImage",                
    "IsPersonalisationApplicationL",                     
    "FUNCTIONALITY",                     
    SetupBackgroundImageL, 
        T_Global_BackgroundImage_IsPersonalisationApplicationL, Teardown )
        
EUNIT_TEST(
    "BackgroundImage - DeleteImageL()",
    "CGSBackgroundImage",                
    "DeleteImageL",                     
    "FUNCTIONALITY",                     
    SetupBackgroundImageL, T_Global_BackgroundImage_DeleteImageL, Teardown )        
    
EUNIT_TEST(
    "BackgroundImage - Creation of AsyncImageHandler()",
    "CGSAsyncImageHandling",                
    "NewL",                     
    "FUNCTIONALITY",                     
    SetupBackgroundImageL, 
        T_Global_BackgroundImage_CreateImageHandlingAsyncObjectL, Teardown ) 

EUNIT_TEST(
    "BackgroundImage - SetWelcomeNoteImageL()",
    "CGSBackgroundImage",                
    "SetWelcomeNoteImageL",                     
    "FUNCTIONALITY",                     
    SetupBackgroundImageAndAsyncHandlerL, 
        T_Global_BackgroundImage_SetWelcomeNoteImageL, Teardown )
        
EUNIT_TEST(
    "BackgroundImage - SetVtStillImagePathL()",
    "CGSBackgroundImage",                
    "SetVtStillImagePathL",                     
    "FUNCTIONALITY",                     
    SetupBackgroundImageAndAsyncHandlerL, 
        T_Global_BackgroundImage_SetVtStillImagePathL, Teardown )      
        
EUNIT_END_TEST_TABLE

//  END OF FILE
