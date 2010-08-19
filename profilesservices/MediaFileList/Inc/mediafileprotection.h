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
* Description:   Class used to check the protection of the 
*                media files.
*
*/



#ifndef MEDIAFILEPROTECTION_H
#define MEDIAFILEPROTECTION_H

#include "mediafiledialogutils.h"
#include "mediafilevariation.h"

//  INCLUDES

#include <e32base.h>
#include <DRMHelper.h>
#include <apgcli.h>   // RApaLsSession


// CONSTANTS


//	FORWARD DECLARATIONS



/**
* CMFProtectionHandler
* 
* For gettting media file protection information (eg drm protection).
* If media is protected and does not have necessary rights it cannot
* be used eg as ringing tone. 
*
*/
NONSHARABLE_CLASS (CMFProtectionHandler) : public CBase
    {
    public:
    // enumeration which tells what is the file used for.
    enum TIntention
        {
        EPlay,
        ESelect
        };
    
    public:// Constructors and destructor

        /**
        * Two-phased constructor.
        */
		static CMFProtectionHandler* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CMFProtectionHandler();
    
    private:

        /**
        * C++ default constructor.
        */
        CMFProtectionHandler();
	
        /**
        * By default Symbian OS constructor is private.
        */
        void ConstructL();

    public: // Functions from base classes

        /**
        * Checks if an unprotected file is valid.
        * @param aFileName Full path and filename.
        * @param aIntention Describes what the file is used for.
        * @return ETrue if the file is valid for intended use.
        */
        TBool IsFileValidL( const TDesC& aFileName, TIntention aIntention );
        TBool IsFileValid( const TDesC& aFileName, TIntention aIntention );
        TBool IsVideoValidL( const TDesC& aFileName, TIntention aIntention );
        TBool IsVideoValid( const TDesC& aFileName, TIntention aIntention );
        
        /**
        * Check if the DRM protect file is expired or have no rights to use.
        * @param aFileName Full path and filename.
        * @return ETrue if the file is DRM expired or have rights to use.
        */
        TBool IsFlieDRMExpired( const TDesC& aFileName );

        void SetAttrL( TInt aAttr, TInt aValue );
        void SetAttrL( TInt aAttr, const TDesC& aValue );
        

    private:
        /**
        * Return media file type
        */
        //TInt32 MediaFileType( const TDesC& aFileName );

        /**
        * Set the automated content type
        * @param  aAutomatedType: type of automated content
        */
        void SetAutomatedType(
             CDRMHelper::TDRMHelperAutomatedType aAutomatedType );

    private:

        /**
        * Displays an error note.
        * @param @aResourceId Resource ID
        */
        void ShowErrorNoteL( TInt aResourceId );

        /**
        * Checks if an unprotected file is valid.
        * @param aFileName Full path and filename.
        * @param aIntention Describes what the file is used for.
        * @return ETrue if the file is valid for intended use.
        */
        TBool IsFileValidUnprotectedL(
              const TDesC& aFileName, TIntention aIntention );

        /**
        * Returns info whether demoplay is allowed for given filetype
        * @param aDataType Tone MIME type
        * @return ETrue if tone allowed to be played
        */
        //TBool AllowDemoPlay( const TDesC& aDataType );

        /**
        * Returns info whether given filetype is allowed as ringingtone
        * @param aDataType Tone MIME type
        * @return ETrue if tone is allowed to be selected
        */
        //TBool AllowSelect( const TDesC& aDataType ) const;

        /**
        * Finds out the MIME type of a given file
        * @param aFileName File to be checked
        * @return Returns datatype of a file
        */
        TDataType DataTypeL( const TDesC& aFileName );
        
        /**
        * Finds out DRM restrictions for a given file
        * @param aFileName File to be checked
        * @param aInfoBits(out) Determines CFM protection
        * @return Returns EFalse if file is corrupted or has no rights
        */
        TBool GetFileInfoL( const TDesC& aFileName, TInt32& aInfoBits );

        /**
        * Checks if file is WMDRM protected.
        * @param aFileName Name of the file to be checked.
        * @return ETrue if file is WMDRM protected, EFalse otherwise.
        */
        TBool   IsFileWMDRMProtectedL( const TDesC& aFileName );
        
        TBool CheckFileSize( const TDesC& aFile, const TDesC& aMimeType );


    private:
        // DRM common functions
        DRMCommon* iDRMCommon;

        // DRM helper
        CDRMHelper* iDRMHelper;
        
        // for finding out drive type (eg memory card)
        CDriveUtil* iDriveUtil;
        
        // for media file list variation
        CMediaFileDialogVariation* iVariation;
          
        // Automated content type
        CDRMHelper::TDRMHelperAutomatedType iAutomatedType;
        
        // max file size
        TInt iMaxFileSize;
        
        // excluded mime types 
        CMimeTypeList* iExcludedMimeTypes;
        
        RApaLsSession iApaLsSession;
        RFs iFsSession;

    };
    

    


#endif  // MEDIAFILEPROTECTION_H

// End of File
