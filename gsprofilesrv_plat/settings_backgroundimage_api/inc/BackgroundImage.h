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
* Description:   Class definition for the background image API
*
*/




#ifndef BACKGROUNDIMAGE_H
#define BACKGROUNDIMAGE_H

//  INCLUDES
#include    <centralrepository.h>
#include    <fbs.h>

#include    <RPhCltServer.h>
#include    "gsasyncimagehandling.h"


// CONSTANTS
const    TInt    KGSBackgroundImageValue = 0;

// DATA TYPES
enum TGSWallpaperType
    {
    EGSWallpaperIdle,
    EGSWallpaperPinboard,
    EGSWallpaperWelcome,
    EGSVTStillImage
    };

// FORWARD DECLARATIONS
class RSharedDataClient;

// CLASS DECLARATION

/**
*  CGSBackgroundImage is the background image API class of GS app.
*  It provides functions to get and set background image values.
*/
class CGSBackgroundImage : public CBase
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        IMPORT_C static CGSBackgroundImage* NewL();
        
        /**
        * Destructor.
        */
        IMPORT_C ~CGSBackgroundImage();

    public: // New functions
        
        /**
        * Returns background image mode.
        * @return 0: no image
        *         1: image
        */
        IMPORT_C TInt BackgroundImageL();

        /**
        * Stores background image mode to shared data.
        * @param aImage Background image mode.
        */
        IMPORT_C void SetBackgroundImageL(const TInt aImage);

        /**
        * Converts an idle background image to bitmap and scales it to fit the
        * screen if necessary.
        *
        * After successful conversion, the background image path is stored
        * into shared data.
        *
        * @param aImagePath Descriptor which contains the background image 
        *   path. Max length is KGSMaxImagePath.
        */
        IMPORT_C void SetBackgroundImagePathL(const TDesC& aImagePath);

				/**
        * Converts an idle background image to bitmap, scales it to fit the
        * screen if necessary, and stores it to the given file.
        *
        * Background image path is not stored into shared data, but must be
        * set separately.
        *
        * @param aImagePath Descriptor which contains the background image 
        *   path. Max length is KGSMaxImagePath.
		    * @param aBmpFileName Descriptor which contains the bitmap file name.
        *   Converted image is again stored back in this variable.
        */
        IMPORT_C void SetBackgroundImagePathL(const TDesC& aImagePath, 
			                                  TDes& aBmpFileName);
			                                  
		/**
        * Converts the welcome note image to bitmap, scales it to fit the screen if 
        * necessary. 
        * Stores the bitmap to mbm file. Stores welcome note image path to shared data. 
        *
        * @param aImagePath Descriptor which contains the welcome note image path.
        *                   Max length is KGSMaxImagePath.
        */
        IMPORT_C void SetWelcomeNoteImageL(const TDesC& aImagePath,
                          CGSAsyncImageHandling& aImgHandler );


		    /**
        * Converts the background image to bitmap, scales it to fit the
        * screen if necessary, and stores it to the given file.
        *
        * Background image path is not stored into shared data, but must be
        * set separately.
        *
        * @since 2.6
        *
        * @param aImagePath Descriptor which contains the background image 
        *   path. Max length is KGSMaxImagePath.
		    * @param aBmpFileName Descriptor which contains the bitmap file name.
        *   Converted image is again stored back in this variable.
        * @param aWallpaperType Type of the wallpaper.
        */
        IMPORT_C void SetBackgroundImagePathL(
            const TDesC& aImagePath, 
			      TDes& aBmpFileName,
                  TGSWallpaperType aWallpaperType );
        
        /**
        * Set the current image path for Still Image
        * 
        */
        IMPORT_C void SetVTStillImagePathL( const TDesC& aImagePath,
                                          CGSAsyncImageHandling& aImgHandler );
                  
        /*
        * Returns if Personalisation application exists in device
        * @return 1: Personalisation application exists in device
        *  otherwise: Personalisation application does not exist in device
        */
        IMPORT_C TInt GetPlnsStatus();
        
        /*
        * Checking presence of the personalisation application on the
        * device
        */
        IMPORT_C void IsPersonalisationApplicationL();

        /*
        * Removing image
        *
        */
        IMPORT_C void DeleteImageL( const TDesC& aImagePath );
        
       
    private:

        /**
        * C++ default constructor.
        */
        CGSBackgroundImage();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
    private:
      
        void LoadImageL( const TDesC& aImagePath, 
                         TInt aWidth, TInt aHeight,
                         TBool aBgImage, 
                         TGSWallpaperType aWallpaperType );
                      
        void SaveBitmapsL( TDes& aBmpFileName, 
                           const TDesC& aOriginalFullname );
    public:     // Data
        TInt iPlnsInDevice;         //for personalisation app check
    
    private:    // Data
        RSharedDataClient* iClient;  //handle to shared data client
        CFbsBitmap* iBitmap;        //for scaling/dithering of bitmap
        
        CRepository* iPersonalizationRepository;
        CRepository* iStartupConfRepository;
        CRepository* iTelephonyRepository;
        TDisplayMode iDisplayMode;  //4k/64k display support
        
        RPhCltServer iPhoneClient;
        RLibrary iLibrary;
        
        CGSAsyncImageHandling* iImgHandler;
        
        // Reserved pointer for future extension
        //TAny* iReserved;

    };

#endif      // CGSBACKGROUNDIMAGE_H   
            
// End of File
