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
* This class implements a model of a file list.
* The class is inherited from MDesCArray, so it can be used directly
* as a model for a listbox.
*
*
*/



#ifndef __CFLDFILELISTMODEL_H__
#define __CFLDFILELISTMODEL_H__

// INTERNAL INCLUDES

// EXTERNAL INCLUDES
#include <e32base.h>
#include <bamdesca.h>
#include <badesca.h>
#include <CLFContentListing.hrh>
#include <MCLFChangedItemObserver.h>
#include <MCLFPostFilter.h>

// FORWARD DECLARATIONS
class CFLDOperationObserver;
class CFLDChangedItemObserver;
class MCLFContentListingEngine;
class MCLFItemListModel;
class CFLDEntryReference;
class MFLDEntryFormatter;
class CFLDWaitNote;
class RConeResourceLoader;
class TResourceReader;
class MCLFModifiableItem;
class CFLDPopupList;

//  CLASS DECLARATION
/**
* This class implements a model of a file list.
* The class is inherited from MDesCArray, so it can be used directly
* as a model for a listbox.
*
* @lib filelist.lib
* @since 2.1
*/
NONSHARABLE_CLASS( CFLDFileListModel )
    : public CBase, public MDesCArray, public MCLFPostFilter
    {
    public:     // Constructors and destructors

		/**
        * Two-phased static constructor.
        * @param aModelResourceId A CLF model resource, see CLFContentListing.rh
        * @param aDirectoriesResourceId A resource using FILELISTDIRECTORIES
        * structure, see FileListModel.rh for details
        * @return A pointer to a constructed CFLDFileListModel object
        */
        static CFLDFileListModel* NewL(
         const TInt aModelResourceId, const TInt aDirectoriesResourceId );

        /**
        * Two-phased static constructor.
        * @param aModelResourceId A CLF model resource, see CLFContentListing.rh
        * @param aDirectoriesResourceId A resource using FILELISTDIRECTORIES
        * structure, see FileListModel.rh for details
        * @return A pointer to a constructed CFLDFileListModel object
        */
        static CFLDFileListModel* NewLC(
         const TInt aModelResourceId, const TInt aDirectoriesResourceId );

        /**
        * Destructor.
        */
        virtual ~CFLDFileListModel();

    private:  // Constructors and destructors

        /**
        * Constructor
        */
        CFLDFileListModel();

        /**
        * Second phase constructor
        * @param aModelResourceId A CLF model resource, see CLFContentListing.rh
        * @param aDirectoriesResourceId A resource using FILELISTDIRECTORIES
        * structure, see FileListModel.rh for details
        */
        void ConstructL(
         const TInt aModelResourceId, const TInt aDirectoriesResourceId );

    public:     // New methods

    	/**
        * Returns file media type
        * @param aFileName A descriptor containing the media file name
        * @return Media file type (see TCLFMediaType enumeration
		* inside CFLContentListing.hrh)
        */
    	TInt32 MediaFileType( const TDesC& aFileName ) const;

    	/**
		* Adds a new MIME type into blocking list.
		* @param aMimeType The blocked MIME type
        */
    	void AddExclusiveMimeTypeL( const TDesC& aMimeType );

    	/**
		* Adds a new media type into blocking list.
		* @param aMediaType The blocked media type (see TCLFMediaType enumeration
		* inside CFLContentListing.hrh)
        */
    	void AddExclusiveMediaTypeL( const TInt32 aMediaType );

    	/**
		* Removes all the defined exlusive MIME types
        */
    	void ResetExclusiveMimeTypes();

    	/**
		* Removes all the defined exlusive media types
        */
    	void ResetExclusiveMediaTypes();

    	/**
		* Sets wanted mime types. Overrides all settings defined inside
		* resource, defined with AddExclusive -methods or with
		* SetWanted -methods.
		* @param aMimeTypes The array containing wanted MIME types
        */
    	void SetWantedMimeTypesL( const MDesCArray& aMimeTypes );

     	/**
		* Sets wanted media types. Overrides all settings defined inside
		* resource, defined with AddExclusive -methods or with
		* SetWanted -methods.
		* @param aMediaTypes The array containing wanted media types
        */
    	void SetWantedMediaTypesL( const TArray<TCLFMediaType>& aMediaTypes );

     	/**
        * Refreshes the file list.
        */
        void RefreshEntryListL();

    	/**
		* Sets popup list to Model. Needed to update the listbox after the
		* model has changed (e.g. after inserting a memory card).
		* @param aPopupList The popup list
        */
		void SetPopupList( CFLDPopupList* aPopupList );

    	/**
        * Retrieves a full path of an entry (e.g. "c:\folder\file.ext")
        * @param aFileName A modifiable descriptor where the path and filename
        *        is placed, or KNullDesc if aIndex points to the null text item.
        *        Note: aFileName.Length() >= KMaxFileName
        * @param aIndex Index of the entry to retrieve
        */
        void GetFileName( TDes& aFileName, const TInt aIndex );

	    /**
        * Sets an MFLDEntryFormatter inherited for the model. Ownership of the given
        * object is transferred to CFLDFileListModel.
        * @param aFormatter A pointer to a MFLDEntryFormatter inherited object.
        */
        void SetEntryFormatter( MFLDEntryFormatter* aFormatter );

     	/**
        * Adds a text (e.g. "None") as the first item of the list,
        * which will not be sorted among the file name items.
        * @param aItemText A descriptor containing the null item text
        */
        void InsertNullItemL( const TDesC& aItemText );

        /**
        * Adds a text (e.g. "None") as the first item of the list,
        * which will not be sorted among the file name items.
        * @param aItemText A descriptor containing the null item text
        * @param aFileName A descriptor containing the ringing tone file name form null item
        */
        void InsertNullItemL( const TDesC& aItemText, const TDesC& aFileName );

        /**
        * Adds a text (e.g. "None") as the last item of the list,
        * which will not be sorted among the file name items.
        * @param aItemText A descriptor containing the null item text
        */
        void InsertEndNullItemL( const TDesC& aItemText );

        /**
        * Adds a text (e.g. "None") as the last item of the list,
        * which will not be sorted among the file name items.
        * @param aItemText A descriptor containing the null item text
        * @param aFileName A descriptor containing the ringing tone file name form null item
        */
        void InsertEndNullItemL( const TDesC& aItemText, const TDesC& aFileName );

        /**
        * Find a file from the model's list
        * @param aFileName The file name to be searched
        * @return An index to the searched file, or KErrNotFound
        */
        TInt FindFileL( const TDesC& aFileName );
        
        /**
        * Sets the maximum file size for the list.
        * @param aMaxFileSize The maximum file size in bytes. If you want to allow all file
        *        sizes, set this to KFLDNoSizeFiltering (defined in FileListModel.hrh).
        */
        void SetMaxFileSize( const TInt aMaxFileSize );
        
        /**
         * Returns PopupList.
         */
        CFLDPopupList* GetPopupList();

    private:     // Methods derived from MDesCArray

        TPtrC16 MdcaPoint( TInt aIndex ) const;
        
        TInt MdcaCount() const;
    
    private:	// Methods derived from MCLFPostFilter
    
        void FilterItemsL( const TArray<MCLFItem*>& aItemList,
                                RPointerArray<MCLFItem>& aFilteredItemList );

	private:	// New methods

		/*
		* Checks if file needs to be filtered out from model
		* @param aMimeType Media MIME type
		* @param aMediaType Media type
		* @param aPathAndFileName Path and filename
		* @return EFalse if MO should be filtered out
		*/
		TBool CheckRules( 
		 TPtrC& aMimeType, TInt32 aMediaType, TPtrC& aPathAndFileName );

        /**
        * Fill the entry reference list according to the added directories list
        * and filtering parameters
        */
        void PopulateReferenceListL();

         /**
        * List all files from the list model to the list box.
        */
        void PopulateListBoxL();

        /**
		* Gets a filename at specific index.
		* @param aIndex Index of the file (whole list is indexed)
		* @param aFileName Descriptor where the filename is stored.
		*/
		void GetFileNameAtIndex( TInt aIndex, TDes& aFileName ) const;
		
		/**
		* Scans user defined Rom directories and adds found entries
		* into iRomRingingToneArray
		*/
		void ScanRomDirectoriesL();
		
		/**
		* Scans user defined Rom directories and adds found entries
		* into iRomRingingToneArray
		*/
		void ScanRomDirectoriesAndInsertL();

		/**
		* Adds directory into iDirectoryPaths
		* @param aDirectory Directory to be scanned
		*/
		TInt AddDirectoryL( const TDesC& aDirectory );

		/**
		* Adds directory into iRomDirectoryPaths
		* @param aDirectory Directory to be scanned
		*/
		TInt AddRomDirectoryL( const TDesC& aDirectory );
		
		/**
		* Loads directories from resource file
		* @param aDirectoriesResourceId Resource ID to directory structure
		*/
 		void LoadDirectoriesL( const TInt aDirectoriesResourceId );

 		/**
         * Checks ringing tone file size.
         * @param aFile         file to check.
         * @param aSizeLimit  maximum allowed file size in bytes.
         * @return KErrNone, if ok.
         *         KErrTooBig, if configurable file size limit exceeded.
         */
        TInt CheckToneFileSizeL( const TDesC& aFile, TInt aSizeLimit );

    private:  // Data

        /// Own: Content listing engine
        MCLFContentListingEngine* iEngine;

        /// Own: Content listing model
        MCLFItemListModel* iModel;

         /// Own: Content listeng operation observer
        CFLDOperationObserver* iObserver;
        
         /// Own: Content listeng operation observer
        CFLDChangedItemObserver* iChangedItemObserver;

        /// Own: MFLDEntryFormatter to use in formatting the entries
        MFLDEntryFormatter* iEntryFormatter;

		/// Own: Wait note dialog for indicating refreshing process of the List Model
        CFLDWaitNote* iWaitNote;
    
    	/// Contains references to all the entries. This is the list that
        /// gets sorted and filtered
        RPointerArray<CFLDEntryReference> iEntryReferences;

        /// Optional text items in the beginning of the list
        RPointerArray<HBufC> iNullTextArray;

        /// Optional ringing tones for null texts
        RPointerArray<HBufC> iNullTextRingingToneArray;

        /// Optional text items in the end of the list
        RPointerArray<HBufC> iEndNullTextArray;

        /// Optional ringing tones for end null items
        RPointerArray<HBufC> iEndNullTextRingingToneArray;

        /// For waiting the asynchronous refresh operation to complete
        CActiveSchedulerWait iWait;

        /// Array for exclusive mimetypes.
		CDesCArraySeg iExclusiveMimeTypes;

		/// Array for exclusive mediatypes.
		RArray<TInt32> iExclusiveMediaTypes;
				
	    /// Contains descriptors containing
	    /// the user-defined phone directory paths
        CDesCArraySeg iDirectoryPaths;
        
        /// Contains descriptors containing
        /// the user-defined ROM directory paths
        CDesCArraySeg iRomDirectoryPaths;
		
		/// Array for user-defined ringingtones
		RPointerArray<MCLFModifiableItem> iRomRingingToneArray;
		
        /// Maximum file size in bytes
        TInt iMaxFileSize;

        /// Popup list
		CFLDPopupList* iPopupList;
		
		/// Using ROM tones only
		TBool iRomTonesOnly;
    };

#endif      //  __CFLDFILELISTMODEL_H__

// End of File
