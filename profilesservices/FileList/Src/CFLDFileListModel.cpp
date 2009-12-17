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
*      This class implements a model of a file list. Features:
*      - Contents of multiple folders in the same list
*      - Filtering with wildcards (*?)
*      - Filtering by maximum file size
*      - Sorting (name / date, ascending / descending)
*      - Entry presentation formatting using an abstract interface
*     The class is inherited from MDesCArray, so it can be used directly
*     as a model for a listbox.
*
*
*/


// CLASS HEADER
#include "CFLDFileListModel.h"

// INTERNAL INCLUDES
#include "CFLDOperationObserver.h"
#include "CFLDChangedItemObserver.h"
#include "CFLDEntryReference.h"
#include "CFLDWaitNote.h"
#include "CFLDPopupList.h"
#include "MFLDEntryFormatter.h"

// EXTERNAL INCLUDES
#include <MCLFContentListingEngine.h>
#include <MCLFItem.h>
#include <MCLFItemListModel.h>
#include <MCLFModifiableItem.h>
#include <ContentListingFactory.h>
#include <CLFContentListing.hrh>
#include <barsread.h>	// For TResourceReader
#include <ConeResLoader.h>	// For RConeResourceLoader
#include <coemain.h>	// CCoeEnv
#include <pathinfo.h>
#include <data_caging_path_literals.hrh>	// For KDC_RESOURCE_FILES_DIR
#include <FileList.rsg>	// For R_FLD_WAIT_NOTE
#include <bautils.h>	// For BaflUtils
#include <apgcli.h>	// For RApaLsSession
#include <aknnotewrappers.h>
#include <StringLoader.h>

// CONSTANTS
namespace
	{
	_LIT( KFLDROMDriveLetter, "Z:" );
	const TInt KGranularityFilters( 2 );
	}

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CFLDFileListModel::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
 CFLDFileListModel* CFLDFileListModel::NewL(
  const TInt aModelResourceId, const TInt aDirectoriesResourceId )
    {
    // The default stuff.
    CFLDFileListModel* self =
     CFLDFileListModel::NewLC( aModelResourceId, aDirectoriesResourceId );
    CleanupStack::Pop( self ); // self
    return self;
    }

// -----------------------------------------------------------------------------
// CFLDFileListModel::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CFLDFileListModel* CFLDFileListModel::NewLC(
 const TInt aModelResourceId, const TInt aDirectoriesResourceId )
    {
    CFLDFileListModel* self =
        new( ELeave ) CFLDFileListModel();
    CleanupStack::PushL( self );
    self->ConstructL( aModelResourceId, aDirectoriesResourceId );
    return self;
    }

// -----------------------------------------------------------------------------
// CFLDFileListModel::CFLDFileListModel
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CFLDFileListModel::CFLDFileListModel()
	: iExclusiveMimeTypes( KGranularityFilters ),
	iExclusiveMediaTypes( KGranularityFilters ),
	iDirectoryPaths( KGranularityFilters ),
	iRomDirectoryPaths( KGranularityFilters ),
	iMaxFileSize( 0 )
    {
    }

// -----------------------------------------------------------------------------
// CFLDFileListModel::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CFLDFileListModel::ConstructL(
 const TInt aModelResourceId, const TInt aDirectoriesResourceId )
    {
	// Create CLF Operation Observer to monitor, when
    // refresh operation has completed
    iObserver = CFLDOperationObserver::NewL();
     
    // Create Content Listing Engine and a list model
    iEngine = ContentListingFactory::NewContentListingEngineLC();
    CleanupStack::Pop(); // iEngine

	// Load CLF definitions from user given resource
    TResourceReader reader;
	CCoeEnv::Static()->CreateResourceReaderLC( reader, aModelResourceId );
    iModel = iEngine->CreateListModelLC( *iObserver, reader );
    CleanupStack::Pop(); // iModel
   	CleanupStack::PopAndDestroy();  // reader

    // Load wait note from default-resource
    iWaitNote = CFLDWaitNote::NewL( R_FLD_WAIT_NOTE );
    
	// Load directories
	CFLDFileListModel::LoadDirectoriesL( aDirectoriesResourceId );
	
	// Create changed item observer to monitor, when
    // tone selection files are modified or deleted
    iChangedItemObserver = CFLDChangedItemObserver::NewL(iWaitNote);
	iEngine->AddChangedItemObserverL( *iChangedItemObserver );

	// Set post filter	
	iModel->SetPostFilter( this );
    }

// Destructor
 CFLDFileListModel::~CFLDFileListModel()
    {
     if( iModel )
        {
        iModel->CancelRefresh();
        delete iModel;
        }

    delete iEngine;
    delete iObserver;
    delete iChangedItemObserver;
    delete iEntryFormatter;
	delete iWaitNote;

    iEndNullTextRingingToneArray.ResetAndDestroy();
    iEndNullTextArray.ResetAndDestroy();
    iNullTextRingingToneArray.ResetAndDestroy();
    iNullTextArray.ResetAndDestroy();
    iEntryReferences.ResetAndDestroy();
    iExclusiveMimeTypes.Reset();
    iExclusiveMediaTypes.Reset();
   	iDirectoryPaths.Reset();
   	iRomDirectoryPaths.Reset();
    iRomRingingToneArray.ResetAndDestroy();
    }


// -----------------------------------------------------------------------------
// CFLDFileListModel::LoadRomDirectoriesL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDFileListModel::LoadDirectoriesL( const TInt aDirectoriesResourceId )	
	 {
	 TResourceReader reader;
  	 CCoeEnv::Static()->CreateResourceReaderLC( reader, aDirectoriesResourceId );
	 
	 // Read ROM directories from resource
	 TInt count( reader.ReadInt16() );
	 while( --count >= 0 )
     	{
        TPtrC ptr( reader.ReadTPtrC() );
		// Ignore return value
        AddRomDirectoryL( ptr );
        }
        
     // Read directories from resource
	 count = reader.ReadInt16();
	 while( --count >= 0 )
     	{
        TPtrC ptr( reader.ReadTPtrC() );
		// Ignore return value
        AddDirectoryL( ptr );
        }
        
  	 CleanupStack::PopAndDestroy();  // reader
	 }
// -----------------------------------------------------------------------------
// CFLDFileListModel::AddRomDirectoryL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
 TInt CFLDFileListModel::AddRomDirectoryL( const TDesC& aDirectory )
    {
    TInt index( 0 );
	if( iRomDirectoryPaths.Find( aDirectory, index ) == 0 )
		{
        // A duplicate was found, do not add to the array, return.
		return KErrAlreadyExists;
		}

	RFs& fsSession( CCoeEnv::Static()->FsSession() );
    // Check that the directory really exists
    if ( !BaflUtils::FolderExists( fsSession, aDirectory ) )
        {
        return KErrPathNotFound;
        }
    iRomDirectoryPaths.AppendL( aDirectory );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CFLDFileListModel::AddDirectoryL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
 TInt CFLDFileListModel::AddDirectoryL( const TDesC& aDirectory )
    {
    TInt index( 0 );
	if( iDirectoryPaths.Find( aDirectory, index ) == 0 )
		{
        // A duplicate was found, do not add to the array, return.
		return KErrAlreadyExists;
		}

    iDirectoryPaths.AppendL( aDirectory );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CFLDFileListModel::SetWantedMimeTypesL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDFileListModel::SetWantedMimeTypesL( const MDesCArray& aMimeTypes )
	{
	CFLDFileListModel::ResetExclusiveMimeTypes();
	CFLDFileListModel::ResetExclusiveMediaTypes();

	iModel->SetWantedMimeTypesL( aMimeTypes );
	}

// -----------------------------------------------------------------------------
// CFLDFileListModel::SetWantedMediaTypesL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDFileListModel::SetWantedMediaTypesL(
 const TArray<TCLFMediaType>& aMediaTypes )
	{
	CFLDFileListModel::ResetExclusiveMimeTypes();
	CFLDFileListModel::ResetExclusiveMediaTypes();

	// Temporary solution
	RArray<TInt> array;
	TInt count( aMediaTypes.Count() );
    CleanupClosePushL( array );
	for( TInt i = 0; i < count; i++ )
		{
		array.AppendL( aMediaTypes[i]);
		}

    iModel->SetWantedMediaTypesL( array.Array() );
    CleanupStack::PopAndDestroy( &array );
	}

// -----------------------------------------------------------------------------
// CFLDFileListModel::PopulateReferenceListL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
 void CFLDFileListModel::PopulateReferenceListL()
    {
    // Do not allow to update model from changed item observer
    iChangedItemObserver->SetFileListModel( NULL );
    
    // Send reference of CActiveSchedulerWait to the CLF Operation Observer
    iObserver->PrepareForRefresh( iWait );

    // CLF hogs the processor when doing its refresh. Make sure we'll
    // have enough of a slice to be able to handle the End key presses.
    RWsSession& wsSession = CCoeEnv::Static()->WsSession();
    wsSession.ComputeMode( RWsSession::EPriorityControlDisabled );

    // Call refresh to get all music files from the file system to the model
    iModel->RefreshL();

	// Display a wait note if user tries to do something while refresh
	// operation of the list model has not finished yet.
    if( iObserver->IsRefreshOngoing() && !iWait.IsStarted() )
        {
        // View the wait note
        if( iWaitNote && !iWaitNote->IsRunning() )
        	{
        	 iWaitNote->OpenWaitNoteL();
        	}
       
   	    // Wait for the refresh operation to complete. Operation Observer will
       	// stop this wait when the refresh operation has compeleted.
       	iWait.Start(); // CSI: 10 # the state of iWait is checked above

        // Close the wait note
        iWaitNote->CloseWaitNoteL();
        }

    // Back to default behavior
    wsSession.ComputeMode( RWsSession::EPriorityControlComputeOff );

	PopulateListBoxL();
		
	if( iPopupList )
		{ // Model has updated; list should try to handle it
		iPopupList->CancelPreview();
		TRAP_IGNORE( iPopupList->ListBox()->HandleItemAdditionL() );
		// List has updated; set focus to the first item
		iPopupList->ListBox()->SetCurrentItemIndexAndDraw( 0 );
		}

	// Allow to update model from changed item observer
	// (unless we're displaying ROM tones only)
	if( !iRomTonesOnly )
		{
		iChangedItemObserver->SetFileListModel( this );
		}
    }

// -----------------------------------------------------------------------------
// CFLDFileListModel::ScanRomDirectoriesL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDFileListModel::ScanRomDirectoriesL()
	{
	CDir* fileList( NULL );
	RFs& fsSession( CCoeEnv::Static()->FsSession() );
	
	TInt directoryCount( iRomDirectoryPaths.Count() );
	for( TInt i= 0; i<directoryCount; i++ )
		{
		User::LeaveIfError(fsSession.GetDir(
			iRomDirectoryPaths.MdcaPoint(i),
			 KEntryAttNormal,ESortByName, fileList));
		CleanupStack::PushL( fileList );
	
		TInt fileCount( fileList->Count() );
		for ( TInt j=0; j<fileCount; j++ )
			{
			const TEntry item = (*fileList)[j];
			TBufC<KMaxFileName> tempBuf = item.iName;
			HBufC* concateName = HBufC::NewLC(
			 iRomDirectoryPaths.MdcaPoint(i).Length() + tempBuf.Length() );
			TPtr des = concateName->Des();
			des.Insert(0, iRomDirectoryPaths.MdcaPoint(i) );
			des.Append( tempBuf );
			
			// Only files located on ROM must be scanned
			// CLF has already scanned other ( supported ) locations
			if( ( des.Left( KFLDROMDriveLetter().Length() ).CompareF(
          		KFLDROMDriveLetter ) == 0 ) )
				{
				TParsePtrC parsedName( des );
				
				MCLFModifiableItem* clfModifiableItem
				 = ContentListingFactory::NewModifiableItemLC();
				clfModifiableItem->AddFieldL( ECLFFieldIdFileName, parsedName.NameAndExt() );
				clfModifiableItem->AddFieldL( ECLFFieldIdPath, parsedName.Path() );
				clfModifiableItem->AddFieldL( ECLFFieldIdDrive, parsedName.Drive() );
#ifdef RD_VIDEO_AS_RINGING_TONE
				if( PathInfo::PathType( parsedName.DriveAndPath() ) == PathInfo::EVideosPath )
					{
					clfModifiableItem->AddFieldL( ECLFFieldIdMediaType, ECLFMediaTypeVideo);
					}
				else
					{
					clfModifiableItem->AddFieldL( ECLFFieldIdMediaType, ECLFMediaTypeSound);
					}
#else
				clfModifiableItem->AddFieldL( ECLFFieldIdMediaType, ECLFMediaTypeSound);
#endif
				clfModifiableItem->AddFieldL( ECLFFieldIdFileNameAndPath, des );
		
        		// Find out MIME type
        		RApaLsSession apaLsSession;
    			User::LeaveIfError( apaLsSession.Connect() );
    			CleanupClosePushL( apaLsSession );
    			TUid dummyUid = { 0 }; // instantiate as zero
    			TDataType dataType( dummyUid );
    			TInt err = apaLsSession.AppForDocument( parsedName.FullName(), dummyUid, dataType );
    			CleanupStack::PopAndDestroy(); // apaLsSession.Close()
    			if( err == KErrNone )
    				{
        			clfModifiableItem->AddFieldL( ECLFFieldIdMimeType, dataType.Des() );
    				User::LeaveIfError(	iRomRingingToneArray.Append( clfModifiableItem ) );
    				CleanupStack::Pop(); // clfModifiableItem
    				}
    			else
    				{
    				CleanupStack::PopAndDestroy(); // clfModifiableItem
    				}
				}
			CleanupStack::PopAndDestroy( concateName );
			}
		CleanupStack::PopAndDestroy( fileList);
		}
	}


// -----------------------------------------------------------------------------
// CompareEntries
// For sorting entry references alphabetically by tone name.
// -----------------------------------------------------------------------------
//
TInt CompareEntries(const CFLDEntryReference& entry1, const CFLDEntryReference& entry2 )
	{
	TParsePtrC parsed1( entry1.PathAndMediaFileName() );
	TParsePtrC parsed2( entry2.PathAndMediaFileName() );

	return parsed1.Name().CompareF( parsed2.Name() );
	}

// -----------------------------------------------------------------------------
// CFLDFileListModel::ScanRomDirectoriesAndInsertL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDFileListModel::ScanRomDirectoriesAndInsertL()
	{
	CDir* fileList( NULL );
	RFs fsSession;
	User::LeaveIfError( fsSession.Connect() ); 
	CleanupClosePushL( fsSession );
	
	TInt directoryCount( iRomDirectoryPaths.Count() );
	for( TInt i= 0; i<directoryCount; i++ )
		{
		User::LeaveIfError(fsSession.GetDir(
			iRomDirectoryPaths.MdcaPoint(i),
			 KEntryAttNormal,ESortByName, fileList));
		CleanupStack::PushL( fileList );
	
		TInt fileCount( fileList->Count() );
		for ( TInt j=0; j<fileCount; j++ )
			{
			const TEntry item = (*fileList)[j];
			TBufC<KMaxFileName> tempBuf = item.iName;
			HBufC* concateName = HBufC::NewLC(
			 iRomDirectoryPaths.MdcaPoint(i).Length() + tempBuf.Length() );
			TPtr des = concateName->Des();
			des.Insert(0, iRomDirectoryPaths.MdcaPoint(i) );
			des.Append( tempBuf );
			
			// Only files located on ROM must be scanned
			if( ( des.Left( KFLDROMDriveLetter().Length() ).CompareF(
          		KFLDROMDriveLetter ) == 0 ) )
				{
				// Add file to entry-list 							            	
		   		CFLDEntryReference* entryRef = new( ELeave ) CFLDEntryReference();
		   		CleanupStack::PushL( entryRef );
	        	HBufC* pathAndMediaFileName	= des.AllocL();
				entryRef->SetPathAndMediaFileName( pathAndMediaFileName );

				TParsePtrC parsedName( des );
				if( PathInfo::PathType( parsedName.DriveAndPath() ) == PathInfo::EVideosPath )
					{
					entryRef->SetMediaType( ECLFMediaTypeVideo);
					}
				else
					{
					entryRef->SetMediaType( ECLFMediaTypeSound);
					}

    			iEntryFormatter->FormatL( *entryRef );
				User::LeaveIfError(	iEntryReferences.Append( entryRef ) );
				CleanupStack::Pop( entryRef );
				}
			CleanupStack::PopAndDestroy( concateName );
			}
		CleanupStack::PopAndDestroy( fileList);
		}

	TLinearOrder<CFLDEntryReference>* sorter = 
		new( ELeave ) TLinearOrder<CFLDEntryReference> ( CompareEntries );
	iEntryReferences.Sort( *sorter );
	delete sorter;

	CleanupStack::PopAndDestroy(); // fsSession
	}


// -----------------------------------------------------------------------------
// CFLDFileListModel::FilterItemsL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDFileListModel::FilterItemsL( const TArray<MCLFItem*>& aItemList,
                                RPointerArray<MCLFItem>& aFilteredItemList )
	{
	
	// Add all found entries into model
    TInt count( aItemList.Count() );
	for( TInt i = 0 ; i < count ; ++i )
    	{
		MCLFItem* item = aItemList[i];
		aFilteredItemList.AppendL( item );	    	
    	}
	
	// Fetch tones from ROM directories
	CFLDFileListModel::ScanRomDirectoriesL();
	
	// Add all found entries into model
    count = iRomRingingToneArray.Count();
	for( TInt i = 0 ; i < count ; ++i )
    	{
		MCLFItem* item = iRomRingingToneArray[i];
		aFilteredItemList.AppendL( item );
    	}
    
	}	

// -----------------------------------------------------------------------------
// CFLDFileListModel::GetPopupList
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CFLDPopupList* CFLDFileListModel::GetPopupList()
	{
	return iPopupList;
	}

// -----------------------------------------------------------------------------
// CFLDFileListModel::AddExclusiveMimeTypeL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDFileListModel::AddExclusiveMimeTypeL( const TDesC& aMimeType )
	{
	TInt index( 0 );
	if( iExclusiveMimeTypes.Find( aMimeType, index ) == 0 )

		{
        // A duplicate was found, do not add to the array, just return.
		return;
		}
	iExclusiveMimeTypes.AppendL( aMimeType );
	}

// -----------------------------------------------------------------------------
// CFLDFileListModel::AddExclusiveMediaTypeL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDFileListModel::AddExclusiveMediaTypeL( const TInt32 aMediaType )
	{
	if( iExclusiveMediaTypes.Find( aMediaType ) != KErrNotFound )
		{
		// A duplicate was found, do not add to the array, just return.
		return;
		}
	iExclusiveMediaTypes.AppendL( aMediaType );
	}

// -----------------------------------------------------------------------------
// CFLDFileListModel::ResetExclusiveMimeTypes
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDFileListModel::ResetExclusiveMimeTypes()
	{
	iExclusiveMimeTypes.Reset();
	}

// -----------------------------------------------------------------------------
// CFLDFileListModel::ResetExclusiveMediaTypes
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDFileListModel::ResetExclusiveMediaTypes()
	{
	iExclusiveMediaTypes.Reset();
	}

// -----------------------------------------------------------------------------
// CFLDFileListModel::CheckRules
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CFLDFileListModel::CheckRules( 
		 TPtrC& aMimeType, TInt32 aMediaType, TPtrC& aPathAndFileName )
	{
	
	// Check exclusive MIME types
    if( iExclusiveMimeTypes.Count() > 0 )
    	{
       	TInt index( 0 );
       	if( iExclusiveMimeTypes.Find( aMimeType, index ) == 0 )
      		{
           	return EFalse;
           	}
       	}

	// Check exclusive media types
	if( iExclusiveMediaTypes.Count() > 0 )
		{
       	if( iExclusiveMediaTypes.Find( aMediaType ) != KErrNotFound )
	   		{
           	return EFalse;
           	}
    	}

   	if( ( aPathAndFileName.Left( KFLDROMDriveLetter().Length() ).CompareF(
          KFLDROMDriveLetter ) == 0 ) )
		{
		// ROM tones are accepted always
		return ETrue;
		}
	
	// Check defined pathes
	if( iDirectoryPaths.Count() > 0 )
		{
		TParsePtrC parsedName( aPathAndFileName );
		TInt index( 0 );
       	if( iDirectoryPaths.Find( parsedName.DriveAndPath(), index ) != 0 )
	   		{
	   		// File is not part of defined path set
           	return EFalse;
           	}
    	}
    	
	// File is part of defined path set or then no directory paths are defined
    return ETrue;
	}

// -----------------------------------------------------------------------------
// CFLDFileListModel::PopulateListBoxL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDFileListModel::PopulateListBoxL()
    {
    // Add all items to the list box
    TInt countItems( iModel->ItemCount() );
    TInt error;
       
    for( TInt i = 0; i < countItems; i++ )
        {
        // Check name and path
        TPtrC fieldValueNameAndPath;
        error = iModel->Item( i ).GetField(
         ECLFFieldIdFileNameAndPath, fieldValueNameAndPath );

        if( error == KErrNone )
            {
	        // Check media type
			TInt32 mediaTypeValue;
    		error = iModel->Item( i ).GetField(
     		ECLFFieldIdMediaType, mediaTypeValue );
	        	
	   		if( error == KErrNone )
		   		{
	       		// Check MIME type
        		TPtrC fieldValueMime;
        		error = iModel->Item( i ).GetField(
        			ECLFFieldIdMimeType, fieldValueMime );
	
	   		 	if( error == KErrNone )
		       		{
            		// Check include/exclude rules
		        	if( CFLDFileListModel::CheckRules(
	       	     		fieldValueMime, mediaTypeValue,
                 		fieldValueNameAndPath ) )
 		       			{
						// Everything seems to be ok,
						// now check file-size limit
    					if ( iMaxFileSize )
        					{
   	            			if ( CheckToneFileSizeL(
   	            				fieldValueNameAndPath, iMaxFileSize)
   	            		 		!= KErrNone )
	        	   				{
	        	   				continue;
            	   				}
							}
						// Finally, add file to entry-list 							            	
				   		CFLDEntryReference* entryRef
				  			= new( ELeave ) CFLDEntryReference();
    			   		CleanupStack::PushL( entryRef );
			        	HBufC* pathAndMediaFileName
			      			= fieldValueNameAndPath.AllocL();
						entryRef->SetPathAndMediaFileName(
							pathAndMediaFileName );
						entryRef->SetMediaType( mediaTypeValue );
	        			iEntryFormatter->FormatL( *entryRef );
						User::LeaveIfError(
							iEntryReferences.Append( entryRef ) );
						CleanupStack::Pop( entryRef );
 		       			}
		       		}
	            }
	        }
        }

	if( countItems == 0 )
		{
		// No tones from CLF -- likely out of memory. Scan for ROM tones only.
		HBufC* errorText = StringLoader::LoadLC( 
									R_FLD_QTN_PROFILES_ERROR_NOT_ENOUGH_MEMORY );
		CAknErrorNote* note = new( ELeave ) CAknErrorNote( ETrue );
		note->ExecuteLD( *errorText );
		CleanupStack::PopAndDestroy( errorText );

		iRomTonesOnly = ETrue;
		ScanRomDirectoriesAndInsertL();
		}

    }


// -----------------------------------------------------------------------------
// CFLDFileListModel::RefreshEntryListL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
 void CFLDFileListModel::RefreshEntryListL()
    {
    iRomTonesOnly = EFalse;

    iRomRingingToneArray.ResetAndDestroy();
    iEntryReferences.ResetAndDestroy();
    PopulateReferenceListL();
    }

// -----------------------------------------------------------------------------
// CFLDFileListModel::SetPopupList
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDFileListModel::SetPopupList( CFLDPopupList* aPopupList )
	{
	iPopupList = aPopupList;
	}

// -----------------------------------------------------------------------------
// CFLDFileListModel::MdcaPoint
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
 TPtrC16 CFLDFileListModel::MdcaPoint( TInt aIndex ) const
    {
	TInt nullItemCount( iNullTextArray.Count() );
	if( aIndex < nullItemCount )
		{
		// Points to a null item in the beginning of the list.
		return iNullTextArray[aIndex]->Des();
		}
	TInt referenceListCount( iEntryReferences.Count() );
	if( aIndex >= referenceListCount + nullItemCount )
		{
		// Points to a null item in the end of the list.
		return iEndNullTextArray[aIndex - referenceListCount - nullItemCount]
                ->Des();
		}

	// The index points to one of the directory entries
	return iEntryReferences[aIndex - nullItemCount]
            ->FormattedPresentation().Des();
	
    }

// -----------------------------------------------------------------------------
// CFLDFileListModel::MdcaCount
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
 TInt CFLDFileListModel::MdcaCount() const
    {
	return iNullTextArray.Count()
	 + iEntryReferences.Count() + iEndNullTextArray.Count();
    }

// -----------------------------------------------------------------------------
// CFLDFileListModel::SetEntryFormatter
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
 void CFLDFileListModel::SetEntryFormatter( MFLDEntryFormatter* aFormatter )
    {
    delete iEntryFormatter;
    iEntryFormatter = aFormatter;
    }

// -----------------------------------------------------------------------------
// CFLDFileListModel::InsertNullItemL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
 void CFLDFileListModel::InsertNullItemL( const TDesC& aItemText )
    {
    InsertNullItemL( aItemText, KNullDesC );
    }

// -----------------------------------------------------------------------------
// CFLDFileListModel::InsertNullItemL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
 void CFLDFileListModel::InsertNullItemL(
  const TDesC& aItemText, const TDesC& aFileName )
    {
	HBufC* text = iEntryFormatter->FormatTextLC( aItemText );
	// Takes ownership
	User::LeaveIfError( iNullTextArray.Append( text ) ); 
	CleanupStack::Pop( text );

	text = aFileName.AllocLC();
	// Takes ownership
	User::LeaveIfError( iNullTextRingingToneArray.Append( text ) ); 
	CleanupStack::Pop( text );
    }

// -----------------------------------------------------------------------------
// CFLDFileListModel::InsertEndNullItemL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
 void CFLDFileListModel::InsertEndNullItemL( const TDesC& aItemText )
    {
    InsertEndNullItemL( aItemText, KNullDesC );
    }

// -----------------------------------------------------------------------------
// CFLDFileListModel::InsertEndNullItemL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
 void CFLDFileListModel::InsertEndNullItemL(
 const TDesC& aItemText, const TDesC& aFileName )
    {
	HBufC* text = iEntryFormatter->FormatTextLC( aItemText );
    // Takes ownership
	User::LeaveIfError( iEndNullTextArray.Append( text ) ); 
	CleanupStack::Pop( text );

	text = aFileName.AllocLC();
	// Takes ownership
	User::LeaveIfError( iEndNullTextRingingToneArray.Append( text ) ); 
	CleanupStack::Pop( text );
    }

// -----------------------------------------------------------------------------
// CFLDFileListModel::GetFileName
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
 void CFLDFileListModel::GetFileName( TDes& aFileName, const TInt aIndex )
    {
	TInt nullItemCount( iNullTextRingingToneArray.Count() );
	if( aIndex < nullItemCount )
		{
		// Points to a null item in the beginning of the list.
		aFileName.Copy( iNullTextRingingToneArray[aIndex]->Des() );
		return;
		}
	TInt referenceListCount( iEntryReferences.Count() );
	if( aIndex >= referenceListCount + nullItemCount )
		{
		// Points to a null item in the end of the list.
		aFileName.Copy( iEndNullTextRingingToneArray[
			aIndex - referenceListCount - nullItemCount]->Des() );
		return;
		}

	// The index points to one of the directory entries
	CFLDEntryReference& entry = *iEntryReferences[aIndex - nullItemCount];
	aFileName.Copy( entry.PathAndMediaFileName().Des() );

    }

// -----------------------------------------------------------------------------
// CFLDFileListModel::FindFileL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
 TInt CFLDFileListModel::FindFileL( const TDesC& aFileName )
    {
	TInt ret( KErrNotFound );
	TInt count( MdcaCount() );
	TFileName fileName( KNullDesC );
	for( TInt index( 0 ); index < count; index++ )
		{
		GetFileNameAtIndex( index, fileName );
		if( fileName.CompareF( aFileName ) == 0 )
			{
			ret = index;
			break;
			}
		}
	return ret;
    }

// -----------------------------------------------------------------------------
// CFLDFileListModel::GetFileNameAtIndex
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDFileListModel::GetFileNameAtIndex(
 TInt aIndex, TDes& aFileName ) const
	{
	TInt nullItemCount( iNullTextRingingToneArray.Count() );
	if( aIndex < nullItemCount )
		{
		// Points to a null item in the beginning of the list.
		aFileName.Copy( iNullTextRingingToneArray[aIndex]->Des() );
		return;
		}
	TInt referenceListCount( iEntryReferences.Count() );
	if( aIndex >= referenceListCount + nullItemCount )
		{
		// Points to a null item in the end of the list.
		aFileName.Copy( iEndNullTextRingingToneArray[
			aIndex - referenceListCount - nullItemCount]->Des() );
		return;
		}

	// The index points to one of the directory entries
	CFLDEntryReference& entry = *iEntryReferences[aIndex - nullItemCount];
	aFileName.Copy( entry.PathAndMediaFileName().Des() );
	}

// -----------------------------------------------------------------------------
// CFLDFileListModel::MediaFileType
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt32 CFLDFileListModel::MediaFileType( const TDesC& aFileName ) const
	{
	if( iRomTonesOnly )
		{
		// Model is empty, get media type from entry reference
		for(TInt i = 0; i < iEntryReferences.Count(); i++)
			{
			if( aFileName.CompareF( iEntryReferences[i]->PathAndMediaFileName() ) == 0 )
				{
				return iEntryReferences[i]->MediaType();
				}
			}
		}
	else
		{
	    TInt countItems( iModel->ItemCount() );
	    for( TInt i = 0; i < countItems; i++ )
	        {
	        // Get value of requested field and add item to the list box
	        TPtrC fieldValue;
	        TInt error( iModel->Item( i ).GetField(
	         ECLFFieldIdFileNameAndPath, fieldValue ) );

	        if( error == KErrNone )
	            {
	            if( aFileName.CompareF( fieldValue ) == 0 )
		   			{
		   			TInt32 mediaTypeValue;
	        		TInt error( iModel->Item( i ).GetField(
	        		 ECLFFieldIdMediaType, mediaTypeValue ) );

	            	if( error == KErrNone )
		            	{
		            	return mediaTypeValue;
		   				}
		   			}
	            }
	        }
		}
    return KErrNotFound;
	}

// -----------------------------------------------------------------------------
// CFLDFileListModel::SetMaxFileSize
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDFileListModel::SetMaxFileSize( const TInt aMaxFileSize )
    {
    iMaxFileSize = aMaxFileSize;
    }

// -----------------------------------------------------------------------------
// CFLDFileListModel::CheckToneFileSizeL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CFLDFileListModel::CheckToneFileSizeL(
 const TDesC& aFile, TInt aSizeLimit )
    {
	RFs& fsSession( CCoeEnv::Static()->FsSession() );
	
    // Get file size
    TInt size = 0;
    TInt error = KErrNone;

    TEntry entry;
    if ( fsSession.Entry( aFile, entry ) == KErrNone )
        {
        size = entry.iSize;        
        }

	// Check. NOTE: now if file size couldn't be determined, check fails.
	if ( aSizeLimit  &&  size > aSizeLimit )
		{
		error = KErrTooBig;
		}

    return error;
    }
    
    
//  End of File
