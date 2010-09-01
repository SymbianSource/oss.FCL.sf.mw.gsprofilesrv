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
* Description: 
*     FileListContainer owns the list box, the model, and the controller,
*     and it is used to launch the popup list.
*
*
*/



#ifndef __CFLDFILELISTCONTAINER_H__
#define __CFLDFILELISTCONTAINER_H__

// INTERNAL INCLUDES

// EXTERNAL INCLUDES
#include <e32base.h>
#include <DRMHelper.h>
#include <bamdesca.h>
#include <CLFContentListing.hrh>

// FORWARD DECLARATIONS
class CFLDDRMImplementation;
class CFLDFileListModel;
class CFLDController;

//  CLASS DECLARATION
/**
* FileListContainer owns the list box, the model, and the controller,
* and it is used to launch the popup list.
*
* @lib filelist.lib
* @since 2.1
*/
class   CFLDFileListContainer
    : public CBase
    {
    public:     // Constructors and destructors

	    /**
        * Two-phase static constructor
        * @return A pointer to a fully constructed CFLDFileListContainer instance
        */
        IMPORT_C static CFLDFileListContainer* NewL();

		/**
		* Two-phase static constructor
		* @return A pointer to a fully constructed CFLDFileListContainer instance.
		*/
        IMPORT_C static CFLDFileListContainer* NewLC();
    
        /**
        * Two-phase static constructor
        * @param aResourceId A CLF model resource, see CLFContentListing.rh
        * @return A pointer to a fully constructed CFLDFileListContainer instance
        */
        IMPORT_C static CFLDFileListContainer* NewL(
			const TInt aResourceId );

        /**
        * Two-phase static constructor
        * @param aResourceId A CLF model resource, see CLFContentListing.rh
        * @return A pointer to a fully constructed CFLDFileListContainer instance.
        */
        IMPORT_C static CFLDFileListContainer* NewLC(
			const TInt aResourceId );
			
		/**
        * Two-phase static constructor
        * @param aResourceId A CLF model resource, see CLFContentListing.rh
        * @param aDirectoriesResourceId A resource using FILELISTDIRECTORIES
        * structure, see FileListModel.rh for details
        * @return A pointer to a fully constructed CFLDFileListContainer instance
        */
        IMPORT_C static CFLDFileListContainer* NewL(
			const TInt aResourceId, const TInt aDirectoriesResourceId );

       /**
        * Two-phase static constructor
        * @param aResourceId A CLF model resource, see CLFContentListing.rh
        * @param aDirectoriesResourceId A resource using FILELISTDIRECTORIES
        * structure, see FileListModel.rh for details
        * @return A pointer to a fully constructed CFLDFileListContainer instance
        */
        IMPORT_C static CFLDFileListContainer* NewLC(
			const TInt aResourceId, const TInt aDirectoriesResourceId );			

        /**
        * Destructor
        */
        IMPORT_C virtual ~CFLDFileListContainer();

    private:  // Constructors and destructors

        /**
        * Constructor
        */
        CFLDFileListContainer();

        /**
        * Second phase constructor
        * @param aResourceId A CLF model resource, see CLFContentListing.rh
        * @param aDirectoriesResourceId A resource using FILELISTDIRECTORIES
        * structure, see FileListModel.rh for details
        */
        void ConstructL(
         const TInt aResourceId, const TInt aDirectoriesResourceId );

    public:     // New methods

		/**
        * Set the automated content type
        * @param  aAutomatedType: type of automated content
        */
        IMPORT_C void SetAutomatedType(
         CDRMHelper::TDRMHelperAutomatedType aAutomatedType );
         
        /**
        * Launches a popup list for browsing files
        * @param aFileName The selected file name will be stored here. If the selected
        *        item was not a file but a null text item, aFileName will contain KNullDesC.
        *        If aFileName contains something already, CFLDFileListContainer tries to find
        *        the name on the list and focus on it.
        *        NOTE: Size of aFileName must be KMaxFileName, otherwise KErrNotFound is
        *		 returned (aFileName is compared against TFileName entities).
        * @param aPopupTitle If this parameter is given, sets the title text of the popup
        * @return True: User selected a file, and its name is stored in aFileName.
        *         False: No file selected because user pressed cancel.
        */
        IMPORT_C TBool LaunchL( TDes& aFileName, const TDesC& aPopupTitle = KNullDesC );

        /**
        * Adds a text (e.g. "None") as the first item of the list,
        * which will not be sorted among the file name items.
        * @param aItemText A descriptor containing the null item text
        */
        IMPORT_C void InsertNullItemL( const TDesC& aItemText );

        /**
        * Adds a text (e.g. "None") as the first item of the list,
        * which will not be sorted among the file name items.
        * @param aItemText A descriptor containing the null item text
        * @param aFileName A descriptor containing the ringing tone file name for null item
        */
        IMPORT_C void InsertNullItemL( const TDesC& aItemText, const TDesC& aFileName );

        /**
        * Adds a text (e.g. "None") as the last item of the list,
        * which will not be sorted among the file name items.
        * @param aItemText A descriptor containing the null item text
        */
        IMPORT_C void InsertEndNullItemL( const TDesC& aItemText );

        /**
        * Adds a text (e.g. "None") as the last item of the list,
        * which will not be sorted among the file name items.
        * @param aItemText A descriptor containing the null item text
        * @param aFileName A descriptor containing the ringing tone file name for null item
        */
        IMPORT_C void InsertEndNullItemL( const TDesC& aItemText, const TDesC& aFileName );

        /**
        * Set the delay until the timer expires.
        * @param aDelay The delay in microseconds
        */
        IMPORT_C void SetDelay( TTimeIntervalMicroSeconds32 aDelay );

        /**
         * Set the volume level on which the sound is played.
         * If this method is not called, toneplayer uses the setting in active profile.
         * @param aVolume The volume level
         */
        IMPORT_C void SetVolume( TInt aVolume );

        /**
         * Sets the ringing type
         * If this method is not called, toneplayer uses the setting in active profile.
         * @param aRingingType The ringing type (see CFLDRingingTonePlayer::TRingingTypes)
         */
        IMPORT_C void SetRingingType( TInt aRingingType );

        /**
        * Sets vibrating alert on or off.
        * If this method is not called, toneplayer uses the setting in active profile.
        * @param aVibra True: Vibra is on. False: Vibra is off.
        */
        IMPORT_C void SetVibra( TBool aVibra );

        /**
        * Adds a new MIME type into blocking list.
        * @param aMimeType The blocked MIME type
        */
    	IMPORT_C void AddExclusiveMimeTypeL( const TDesC& aMimeType );

    	/**
        * Adds a new media type into blocking list.
        * @param aMediaType The blocked media type (see TCLFMediaType enumeration
        * inside CFLContentListing.hrh)
        */
    	IMPORT_C void AddExclusiveMediaTypeL( const TInt32 aMediaType );

    	/**
        * Sets wanted mime types. Overrides all settings defined inside
        * resource, defined with AddExclusive -methods or with
        * SetWanted -methods.
        * @param aMimeTypes The array containing wanted MIME types
        */
    	IMPORT_C void SetWantedMimeTypesL( const MDesCArray& aMimeTypes );

     	/**
        * Sets wanted media types. Overrides all settings defined inside
        * resource, defined with AddExclusive -methods or with
        * SetWanted -methods.
        * @param aMediaTypes The array containing wanted media types
        */
    	IMPORT_C void SetWantedMediaTypesL( const TArray<TCLFMediaType>& aMediaTypes );

    	/**
        * Removes all the defined exlusive MIME types
        */
    	IMPORT_C void ResetExclusiveMimeTypes();

    	/**
        * Removes all the defined exlusive media types
        */
    	IMPORT_C void ResetExclusiveMediaTypes();
    	
    	/**
        * Sets the maximum file size for the list.
        * @param aMaxFileSize The maximum file size in bytes. If you want to
        *        allow all file sizes, set this to KFLDNoSizeFiltering
        *        (defined in FileListModel.hrh).
        * @since 3.1
        */
        IMPORT_C void SetMaxFileSize( const TInt aMaxFileSize );
        
         /**
        * Sets 3d-effects on or off.
        * If this method is called using parameter EFalse 3d-effects are 
        * always disabled even though those are set in active profile.
        * Otherwise toneplayer uses the setting in active profile.
        * @param a3dEffects True: 3dEffects are on. False: 3dEffects are off.
        */
        IMPORT_C void Set3dEffects( TBool a3dEffects );

    private:    // Data

        /// Own: The file list model
        CFLDFileListModel* iModel;

        /// Own: The controller
        CFLDController* iController;

		/// Own: DRM implementation
		CFLDDRMImplementation* iDRMImplementation;
		
		/// Resourceloader which loads used resources
       	RConeResourceLoader iResourceLoader;

    };

#endif      //  __CFLDFILELISTCONTAINER_H__

// End of File
