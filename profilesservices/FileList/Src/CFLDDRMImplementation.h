/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for DRM functionality.
*
*/



#ifndef CFLDDRMIMPLEMENTATION_H
#define CFLDDRMIMPLEMENTATION_H

// INTERNAL INCLUDES
#include "MFLDFileObserver.h"

// EXTERNAL
#include <e32base.h>
#include <apmstd.h> // KMaxDataTypeLength
#include <data_caging_path_literals.hrh>
#include <centralrepository.h>
#include <DRMHelper.h>	// For TDRMHelperAutomatedType
#include <DRMRights.h>	// For TRestriction

// FORWARD DECLARATIONS
class DRMCommon;
class CDRMHelper;
class CFLDFileListModel;
class MProfilesLocalFeatures;

// CLASS DECLARATION

/**
*  Implementation for DRM functionality.
*
*  @lib filelist.lib
*  @since 2.1
*/
NONSHARABLE_CLASS( CFLDDRMImplementation ) : public CBase, public MFLDFileObserver
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CFLDDRMImplementation* NewL( CFLDFileListModel* aModel );

        /**
        * Destructor.
        */
        virtual ~CFLDDRMImplementation();

    public: // Functions from base classes

        /**
        * From MFLDFileObserver.
        */
		TBool IsFileValidL( const TDesC& aFileName, TIntention aIntention );

		/**
		* From MFLDFileObserver.
		*/
		TInt32 MediaFileType( const TDesC& aFileName ) const;

	public:	// New functions

		/**
        * Set the automated content type
        * @param  aAutomatedType: type of automated content
        */
        void SetAutomatedType(
         CDRMHelper::TDRMHelperAutomatedType aAutomatedType );


    private: // New functions

        /**
        * Displays an error note.
        * @since 2.1
        * @param @aResourceId Resource ID for a TBUF resource containing the error text.
        */
        void ShowErrorNoteL( TInt aResourceId ) const;

        /**
        * Checks if an unprotected file is valid.
        * @param aFileName Full path and filename.
        * @param aIntention Describes what is the file used for.
        * @return Returns ETrue if the file is valid for intended use.
        */
        TBool IsFileValidUnprotectedL(
            const TDesC& aFileName, TIntention aIntention ) const;

		/**
		* Returns info whether demoplay is allowed for given filetype
		* @param aDataType Tone MIME type
		* @return ETrue if tone allowed to be played
		*/
		TBool AllowDemoPlay( const TDesC& aDataType ) const;

		/**
		* Returns info whether given filetype is allowed as ringingtone
		* @param aDataType Tone MIME type
		* @return ETrue if tone is allowed to be selected
		*/
		TBool AllowSelect( const TDesC& aDataType ) const;

		/**
		* Finds out the MIME type of a given file
		* @param aFileName File to be checked
		* @return Returns datatype of a file
		*/
		TDataType DataTypeL( const TDesC& aFileName ) const;
		
		/**
		* Finds out DRM restrictions for a given file
		* @param aFileName File to be checked
		* @param aInfoBits(out) Determines CFM protection
		* @return Returns EFalse if file is corrupted or has no rights
		*/
		TBool GetFileInfoL( const TDesC& aFileName,
		 TInt32& aInfoBits ) const;

		/**
         * Checks if file is WMDRM protected.
         * @param aFileName Name of the file to be checked.
         * @return ETrue if file is WMDRM protected, EFalse otherwise.
         */
		TBool	IsFileWMDRMProtectedL( const TDesC& aFileName ) const;

    private:

        /**
        * C++ default constructor.
        */
        CFLDDRMImplementation( CFLDFileListModel* aModel );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // Data

        /// Own: DRM common functions
        DRMCommon* iDRMCommon;

        /// Own: DRM helper
        CDRMHelper* iDRMHelper;

        /// Not owned: FileList model
        CFLDFileListModel* iModel;

        /// Automated content type
        CDRMHelper::TDRMHelperAutomatedType iAutomatedType;

        /// Not owned: Profiles local features
        MProfilesLocalFeatures* iProfilesFeatures;
    };

#endif      // CFLDDRMIMPLEMENTATION_H

// End of File
