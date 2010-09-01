/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef GS_ASYNC_IMAGE_HANDLE_H
#define GS_ASYNC_IMAGE_HANDLE_H

#include    <e32base.h>
#include    <imageconversion.h>
#include    <bitmaptransforms.h>
#include    <RPhCltServer.h>
#include    <centralrepository.h>
#include    "mgsasyncimagehandlingobserver.h"

// CONSTANTS
const TInt KGSMimeStringLength = 50;

// DATA TYPES
enum TGSBgImageType
    {
    EGSWelcomeNoteImage,
    EGSVtStillImage
    };

enum TGSImgState
    {
    EGSWelcomeConversion,
    EGSWelcomeScaling,
    EGSVTConversion,
    EGSVTScaling
    };
    
// FORWARD DECLARTION
class CImageDecoder;
class MGSAsyncImageHandlingObserver;


// CLASS DECLARTION
class CGSAsyncImageHandling : public CActive
  {
  public:
        // Static constructor
        IMPORT_C static CGSAsyncImageHandling* NewL(
                                RFs& aFs,
                                MGSAsyncImageHandlingObserver* aObserver,
                                const TDesC& aDestinationPath );
        
        // Destructor
        ~CGSAsyncImageHandling();
  
        /*
         * Asynchronous loading/scaling/dithering of an image. 
         * The image is converted to a bitmap.
         */
        void ProcessImageL( const TDesC& aFilename, 
                               TInt aScreenWidth,
                               TInt aScreenHeight,
                               TGSBgImageType aBgImageType );
                               
        TDisplayMode GetDisplayMode( const TFrameInfo& aFrameInfo );
        
  private:
        // C++ constructor
        CGSAsyncImageHandling( RFs& aFs,
                               MGSAsyncImageHandlingObserver* aObserver,
                               const TDesC& aDestinationPath );
        
        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
        TSize CalculateLoadSize(TFrameInfo& aFrameInfo,
                                    const TSize aScreenSize );
                                    
        void SaveVTStillImageL();
  
  private:
        // From CActive
        void RunL();
        TInt RunError( TInt aError );
        void DoCancel();
        void Release();
        
  private:
        MGSAsyncImageHandlingObserver* iObserver;
        
        // File session
        RFs& iFs;
        
        // Image decoder
        CImageDecoder* iDecoder;
        
        // Currently open file Mime-string
        TBuf8<KGSMimeStringLength> iMimeString;
        
        CFbsBitmap* iBitmap;
        
        const TDesC& iDestinationPath;
        TInt iBgImageType;
        
        RPhCltServer iPhoneClient;
        RLibrary iLibrary;
        CRepository* iStartupRepository;
        CBitmapScaler* iBitmapScaler;
        TSize iScaleSize;
        TInt iState;
        
  };
  
#endif // GS_ASYNC_IMAGE_HANDLE_H

// End of File
