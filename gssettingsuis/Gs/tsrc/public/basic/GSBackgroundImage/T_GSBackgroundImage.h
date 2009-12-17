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


#ifndef __T_GSBACKGROUNDIMAGE_H__
#define __T_GSBACKGROUNDIMAGE_H__

//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <EUnitDecorators.h>
#include <BackgroundImage.h>
#include <f32file.h>
#include <GsAsyncImageHandling.h>
#include <MGsAsyncImageHandlingObserver.h>

//  INTERNAL INCLUDES
//#include "BackgroundImage.cpp"

//  FORWARD DECLARATIONS


//  CLASS DEFINITION
NONSHARABLE_CLASS( T_GSBackgroundImage )
	: public CEUnitTestSuiteClass,
	  public MGSAsyncImageHandlingObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static T_GSBackgroundImage* NewL();
        static T_GSBackgroundImage* NewLC();
        /**
         * Destructor
         */
        ~T_GSBackgroundImage();
        
    public: //From MGSAsyncImageHandlingObserver
        void ImageHandlingCompleteL( TInt aError );

    private:    // Constructors and destructors

        T_GSBackgroundImage();
        void ConstructL();

    private:    // New methods

         void EmptySetupL();
         
         void EmptyTeardown();
        
         void Teardown();
        
         void SetupBackgroundImageL();
         void SetupBackgroundImageAndAsyncHandlerL();
         
         void T_Global_BackgroundImage_NewL();
         void T_Global_BackgroundImage_GetPlnsStatus();
         void T_Global_BackgroundImage_BackgroundImageL();
         void T_Global_BackgroundImage_SetBackgroundImageL();
         void T_Global_BackgroundImage_SetBackgroundImagePathL();
         void T_Global_BackgroundImage_SetBackgroundImagePath2L();
         void T_Global_BackgroundImage_SetBackgroundImagePath3L();
         void T_Global_BackgroundImage_CreateImageHandlingAsyncObjectL();
         void T_Global_BackgroundImage_SetWelcomeNoteImageL();
         void T_Global_BackgroundImage_SetVtStillImagePathL();
         void T_Global_BackgroundImage_IsPersonalisationApplicationL();
         void T_Global_BackgroundImage_DeleteImageL();
        

    private:    // Data
		
        CGSBackgroundImage* iCGSBackgroundImage;
        CGSAsyncImageHandling* iImgHandler;
        RFs iFs;
        EUNIT_DECLARE_TEST_TABLE; 

    };

#endif      //  __T_GSBACKGROUNDIMAGE_H__

// End of file
