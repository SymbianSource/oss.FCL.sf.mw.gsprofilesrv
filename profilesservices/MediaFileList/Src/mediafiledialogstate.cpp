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
* Description:   Class used to deal with the items of the Main view
*
*/




// INCLUDE FILES

#include "mediafiledialogstate.h"
#include "mediafiledialogutils.h"

#include <mediafilelist.rsg>  // for .rss resource enums
#include <StringLoader.h>



/******************************************************************************
 * class TMFListItem
 ******************************************************************************/


// -------------------------------------------------------------------------------
// TMFListItem::TMFListItem
//
// -------------------------------------------------------------------------------
//
TMFListItem::TMFListItem()
 : iListIndex(KErrNotFound), iFolderId(KErrNotFound), iParentFolderId(KErrNotFound),
   iChildFolderId(KErrNotFound), iPersistent(ETrue),
   iHidden(EFalse), iIconId(EEIconEmpty), iChildIconId(EEIconEmpty),
   iUnknownCount(KErrNotFound)
	 {
	 }



/******************************************************************************
 * class TMFCurrentItemInfo
 ******************************************************************************/


// -------------------------------------------------------------------------------
// TMFCurrentItemInfo::TMFCurrentItemInfo
//
// -------------------------------------------------------------------------------
//
void TMFCurrentItemInfo::Reset()
    {
    iIndexCheckNeeded = EFalse;
    iItemIndex = KErrNotFound;
    iMediaFileId = KErrNotFound;
    iFolderId = KErrNotFound;
    iFolderText = KNullDesC;
    }


/******************************************************************************
 * class CMediaFileDialogState
 ******************************************************************************/


// -----------------------------------------------------------------------------
// CMediaFileDialogState::NewL
//
// -----------------------------------------------------------------------------
CMediaFileDialogState* CMediaFileDialogState::NewL()
    {
    CMediaFileDialogState* self = new ( ELeave ) CMediaFileDialogState();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CMediaFileDialogState::CMediaFileDialogState
// 
// -----------------------------------------------------------------------------
//
CMediaFileDialogState::CMediaFileDialogState()
	{
	iCurrentFolderId = EEFolderRoot;
	iCurrentFolderIndex = 0;
	iListPosition = KErrNotFound;
	
    iQueryFolderId = EEFolderRoot;
    iQueryDirection = EEQueryDown;
    iQueryText = KNullDesC;
    iPreviousFolderId = KErrNotFound;
    iCurrentItemInfo.Reset();
    }

// -----------------------------------------------------------------------------
// CMediaFileDialogState::ConstructL
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialogState::ConstructL()
    {
    AddPermanentItemsL();
    } 

// ----------------------------------------------------------------------------
// Destructor
//
// ----------------------------------------------------------------------------
//
CMediaFileDialogState::~CMediaFileDialogState()
    {
    iList.Close();
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::Init
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialogState::Init()
    {
    // initialize non-persistent folders
    TInt count = iList.Count();
    for ( TInt i=0; i<count; i++ )
        {
        TMFListItem& item = iList[i];
        if ( !item.iPersistent )
            {
            item.iListIndex = KErrNotFound;
            item.iText = KNullDesC;
            item.iTitle = KNullDesC;
            }
        }

	iCurrentFolderId = EEFolderRoot;
	iCurrentFolderIndex = 0;
	iListPosition = KErrNotFound;
  	
    iQueryFolderId = EEFolderRoot;
    iQueryDirection = EEQueryDown;
    iQueryText = KNullDesC;
    iPreviousFolderId = KErrNotFound;
    iCurrentItemInfo.Reset();
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::HideRootFolder
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialogState::HideRootFolder( TInt aFolderId, TBool aHidden,
                                            const TDesC& aText, TInt aIconId )
    {
    TInt index = IndexFromId( aFolderId );
    TMFListItem& item = iList[index];
    if ( item.iParentFolderId != EEFolderRoot )
        {
        return; // only root folders can be variated
        }

    item.iHidden = aHidden;
    if ( aText != KNullDesC )
        {
        TMFDialogUtil::StrCopy( item.iText, aText );
        }
    if ( aIconId != KErrNotFound )
        {
        item.iIconId = aIconId;
        }
    
    // update root folder indexes. Note that root folders must be
    // created in the same order they appear in UI (see AddPermanentItemsL).
    TInt listIndex = 0;
    TInt count = iList.Count();
    for ( TInt i=0; i<count; i++ )
        {
        TMFListItem& temp = iList[i];
        if ( temp.iParentFolderId == EEFolderRoot )
            {
            if ( !temp.iHidden )
                {
                temp.iListIndex = listIndex;
                listIndex++;
                }
            }
        }
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::ResetUnknownCount
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialogState::ResetUnknownCount()
    {
    SetUnknownCount( EEFolderArtistRoot, KErrNotFound );
    SetUnknownCount( EEFolderAlbumRoot, KErrNotFound );
    SetUnknownCount( EEFolderGenreRoot, KErrNotFound );
    SetUnknownCount( EEFolderComposerRoot, KErrNotFound );
    SetUnknownCount( EEFolderArtist, KErrNotFound );
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::SetUnknownCount
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialogState::SetUnknownCount( TInt aFolderId, TInt aCount )
    {
    TInt index = IndexFromId( aFolderId );
    TMFListItem& item = iList[index];
    item.iUnknownCount = aCount;
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::UnknownCount
//
// -----------------------------------------------------------------------------
//
TInt CMediaFileDialogState::UnknownCount( TInt aFolderId )
    {
    TInt index = IndexFromId( aFolderId );
    TMFListItem& item = iList[index];
    return item.iUnknownCount;
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::HideUnknownFolder
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialogState::HideUnknownFolder( TInt aFolderId, TInt aUnknownCount )
    {
    TInt index = IndexFromId( aFolderId );
    TMFListItem& item = iList[index];
    
    item.iHidden = EFalse;
    if ( aUnknownCount < 1 )
        {
        item.iHidden = ETrue;
        }
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::HideUnknownFolders
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialogState::HideUnknownFolders()
    {
    TInt count = UnknownCount( EEFolderArtistRoot );
    HideUnknownFolder( EEFolderArtistUnknown, count );
    
    count = UnknownCount( EEFolderAlbumRoot );
    HideUnknownFolder( EEFolderAlbumUnknown, count );

    count = UnknownCount( EEFolderGenreRoot );
    HideUnknownFolder( EEFolderGenreUnknown, count );

    count = UnknownCount( EEFolderComposerRoot );
    HideUnknownFolder( EEFolderComposerUnknown, count );
    
    count = UnknownCount( EEFolderArtist );
    HideUnknownFolder( EEFolderArtistAlbumUnknown, count );
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::SetUnknownFolderPosition
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialogState::SetUnknownFolderPosition( TInt aQueryCount )
    {
    //
    // Unknown folder is the last list item (after query result items)
    //
    if ( iCurrentFolderId == EEFolderArtistRoot|| 
         iCurrentFolderId == EEFolderAlbumRoot ||
         iCurrentFolderId == EEFolderGenreRoot ||
         iCurrentFolderId == EEFolderComposerRoot ||
         iCurrentFolderId == EEFolderArtist )
        {
        SetListIndex( EEFolderArtistUnknown, aQueryCount );
        SetListIndex( EEFolderAlbumUnknown, aQueryCount );
        SetListIndex( EEFolderGenreUnknown, aQueryCount );
        SetListIndex( EEFolderComposerUnknown, aQueryCount );
    
        // folder EEFolderArtist has persistent "All" folder as a first item
        SetListIndex( EEFolderArtistAlbumUnknown, aQueryCount + 1 );
        }
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::IndexFromId
//
// -----------------------------------------------------------------------------
//
TInt CMediaFileDialogState::IndexFromId( TInt aFolderId )
    {
    TInt index = KErrNotFound;
    
    TInt count = iList.Count();
    for (TInt i=0; i<count; i++)
        {
        TMFListItem& item = iList[i];
        if ( item.iFolderId == aFolderId )
            {
            index = i;
            break;
            }
        }
        
    __ASSERT_ALWAYS( index != KErrNotFound, Panic( KErrGeneral ) );
    
    return index;
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::PersistentFolderCount
//
// Get count of persistent subfolders in folder aFolderId.
// -----------------------------------------------------------------------------
//
TInt CMediaFileDialogState::PersistentFolderCount( TInt aFolderId )
    {
    TInt folderCount = 0;
    
    TInt count = iList.Count();
    for ( TInt i=0; i<count; i++ )
        {
        TMFListItem& item = iList[i];
        if ( item.iParentFolderId == aFolderId )
            {
            if ( item.iHidden )
                {
                continue;
                }
            if ( item.iPersistent )
                {
                folderCount++;
                }
            }
        }

    return folderCount;
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::PersistentFolderCount
//
// -----------------------------------------------------------------------------
//
TInt CMediaFileDialogState::PersistentFolderCount()
    {
    return PersistentFolderCount( iCurrentFolderId );
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::LeadingPersistentFolderCount
//
// -----------------------------------------------------------------------------
//
TInt CMediaFileDialogState::LeadingPersistentFolderCount()
    {
    // how many persisten folders in the beginning of list
    if ( iCurrentFolderId == EEFolderArtist )
        {
        return 1;
        }
        
    return 0;    
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::CurrentItemIndex
//
// -----------------------------------------------------------------------------
//
TInt CMediaFileDialogState::CurrentItemIndex()
    {
    return iListPosition;
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::SetCurrentItemIndex
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialogState::SetCurrentItemIndex( TInt aIndex )
    {
    iListPosition = aIndex;
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::PreviousFolderId
//
// -----------------------------------------------------------------------------
//
TInt CMediaFileDialogState::PreviousFolderId()
    {
    return iPreviousFolderId;
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::GetCurrentItemInfo
//
// -----------------------------------------------------------------------------
//
TMFCurrentItemInfo& CMediaFileDialogState::CurrentItemInfo()
    {
    return iCurrentItemInfo;
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::QueryDirection
//
// -----------------------------------------------------------------------------
//
TInt CMediaFileDialogState::QueryDirection()
    {
    return iQueryDirection;
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::IsPersistent
//
// -----------------------------------------------------------------------------
//
TInt CMediaFileDialogState::IsPersistent( TInt aFolderId )
    {
    TInt index = IndexFromId( aFolderId );

    TMFListItem& item = iList[index];
    return item.iPersistent;
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::SetListIndex
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialogState::SetListIndex( TInt aFolderId, TInt aListIndex )
    {
    TInt count = iList.Count();
    for ( TInt i=0; i<count; i++ )
        {
        TMFListItem& item = iList[i];
        if ( item.iFolderId == aFolderId )
            {
            if ( item.iParentFolderId == EEFolderRoot || 
                 item.iParentFolderId == EEFolderMusic )
                {
                // root and music folder indexes cannot change after initialization
                break;
                }
            item.iListIndex = aListIndex;
            break;
            }
        }
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::ListIndex
//
// -----------------------------------------------------------------------------
//
TInt CMediaFileDialogState::ListIndex( TInt aFolderId )
    {
    TInt count = iList.Count();
    for ( TInt i=0; i<count; i++ )
        {
        TMFListItem& item = iList[i];
        if ( item.iFolderId == aFolderId )
            {
            return item.iListIndex;
            }
        }
    
    return KErrNotFound;
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::FindBoxNeeded
//
// -----------------------------------------------------------------------------
//
TBool CMediaFileDialogState::FindBoxNeeded()
    {
    //return ETrue;  // find box used in every folder
    
    
    if ( iCurrentFolderId == EEFolderSearch ||
         iCurrentFolderId == EEFolderRoot )
        {
        return ETrue; // these folders have find box
        }
    
    TMFListItem& currentFolder = iList[iCurrentFolderIndex];
    if ( currentFolder.iChildFolderId == EEFolderNone )
        {
        return ETrue; // folder contains audio or fideo files
        }
        
    return EFalse;
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::QueryNeeded
//
// -----------------------------------------------------------------------------
//
TBool CMediaFileDialogState::QueryNeeded()
    {
    return QueryNeeded( iCurrentFolderId );
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::QueryNeeded
//
// -----------------------------------------------------------------------------
//
TBool CMediaFileDialogState::QueryNeeded( TInt aFolderId )
    {
    if ( aFolderId == EEFolderRoot || aFolderId == EEFolderMusic )
        {
        return EFalse;
        }
    return ETrue;
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::SetText
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialogState::SetText( TInt aFolderId, const TDesC& aText )
    {
    TInt index = IndexFromId( aFolderId );

    TMFListItem& item = iList[index];
    
    // persistent folder data cannot change after initialization
    if ( !item.iPersistent )
        {
        TMFDialogUtil::StrCopy( item.iText, aText);
        }
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::GetText
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialogState::GetText( TInt aFolderId, TDes& aText )
    {
    aText = KNullDesC;
    
    TInt index = IndexFromId( aFolderId );

    TMFListItem& item = iList[index];
    TMFDialogUtil::StrCopy( aText, item.iText );
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::GetSearchTitleL
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialogState::GetSearchTitleL( TDes& aText, TInt aCount )
    {
    aText = KNullDesC;
    
    HBufC* hBuf = StringLoader::LoadLC( R_QTN_VIEW_SEARCH, aCount );
    TMFDialogUtil::StrCopy( aText, *hBuf );
    CleanupStack::PopAndDestroy( hBuf );
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::GetSearchTitle
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialogState::GetSearchTitle( TDes& aText, TInt aCount )
    {
    TRAP_IGNORE( GetSearchTitleL( aText, aCount ) );
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::GetTitle
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialogState::GetTitle( TInt aFolderId, TDes& aText )
    {
    aText = KNullDesC;
     
    TInt index = IndexFromId( aFolderId );

    TMFListItem& item = iList[index];
    TMFDialogUtil::StrCopy( aText, item.iTitle );
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::SetTitle
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialogState::SetTitle( TInt aFolderId, const TDesC& aText )
    {
    TInt index = IndexFromId( aFolderId );

    TMFListItem& item = iList[index];
    
    // persistent folder data cannot change after initialization
    if ( !item.iPersistent )
        {
        TMFDialogUtil::StrCopy( item.iTitle, aText);
        }
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::CurrentFolder
//
// -----------------------------------------------------------------------------
//
TInt CMediaFileDialogState::CurrentFolder()
    {
    return iCurrentFolderId;
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::SetCurrentFolder
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialogState::SetCurrentFolder( TInt aFolderId )
    {
    iCurrentFolderIndex = IndexFromId( aFolderId );
    iCurrentFolderId = aFolderId;
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::SetStateAfterQuery
//
// Set media file list state. This should be called after successful
// database query.
// -----------------------------------------------------------------------------
//
void CMediaFileDialogState::SetStateAfterQuery()
    {
    if ( iQueryDirection == EEQueryUpdate )
        {
        return;  // state does not change when updating
        }
    
    if ( iQueryDirection == EEQueryDown )
        {
        iListPosition = 0;
        }
    if ( iQueryDirection == EEQueryUp )
        {
        // set list position to folder that was closed
        TInt index = IndexFromId( iCurrentFolderId );
        TMFListItem& item = iList[index];
        iListPosition = item.iListIndex;
        }
    
    iPreviousFolderId = iCurrentFolderId;
    SetCurrentFolder( iQueryFolderId );
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::SetQueryInfo
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialogState::SetQueryInfo( TInt aQueryFolderId, const TDesC& aQueryText,
                                          TInt aQueryListPosition, TInt aQueryDirection )
    {
    if ( aQueryDirection == EEQueryUpdate )
        {
        iQueryDirection = aQueryDirection;
        return;
        }
    
    if ( aQueryDirection == EEQueryDown )
        {
        // folder/title updated even is query is not successful
        SetText( aQueryFolderId, aQueryText );
        SetTitle( aQueryFolderId, aQueryText );
        
        // set folder list position so that it can be restored
        // when folder is closed
        SetListIndex( aQueryFolderId, aQueryListPosition );
        }
       
    
    iQueryDirection = aQueryDirection;
    
    // new list position set after successful query
    iListPosition = aQueryListPosition;
    
    // iCurrentFolderId set after successful query
    iQueryFolderId = aQueryFolderId;
    }

/*
void CMediaFileDialogState::SetStateAfterQuery()
    {
    TInt previousCurrentFolderId = iCurrentFolderId;
    
    SetCurrentFolder( iQueryFolderId );
    
    if ( iQueryDirection == EEQueryDown )
        {
        SetListIndex( iQueryFolderId, iQueryListPosition );
        iListPosition = 0;
        }
    if ( iQueryDirection == EEQueryUp )
        {
        // set list position to folder that was closed
        TInt index = IndexFromId( previousCurrentFolderId );
        TMFListItem& item = iList[index];
        iListPosition = item.iListIndex;
        }
    }
*/



/*
void CMediaFileDialogState::SetQueryInfo( TInt aQueryFolderId, const TDesC& aQueryText,
                                          TInt aQueryListPosition, TInt aQueryDirection )
    {
    iQueryFolderId = aQueryFolderId;
    if ( aQueryDirection == EEQueryDown )
        {
        // folder/title updated even is query is not successful
        SetText( aQueryFolderId, aQueryText );
        SetTitle( aQueryFolderId, aQueryText );
        }
       
    iQueryListPosition = aQueryListPosition;
    iQueryDirection = aQueryDirection;
    
    iListPosition = aQueryListPosition; // updated after successful query
    }
*/

// -----------------------------------------------------------------------------
// CMediaFileDialogState::NextFolderDown
//
// -----------------------------------------------------------------------------
//
TInt CMediaFileDialogState::NextFolderDown( TInt aListIndex )
    {
    TInt folderId = FolderId( aListIndex );
    
    // folderId is KErrNotFound if list item is not a folder
    return folderId;
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::NextFolderUp
//
// -----------------------------------------------------------------------------
//
TInt CMediaFileDialogState::NextFolderUp()
    {
    TMFListItem& item = iList[iCurrentFolderIndex];
    
    if ( item.iParentFolderId != KErrNotFound )
        {
        return item.iParentFolderId;
        }
        
    return KErrNotFound;
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::QueryFolder
//
// -----------------------------------------------------------------------------
//
TInt CMediaFileDialogState::QueryFolder()
    {
    return iQueryFolderId;
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::FolderId
//
// -----------------------------------------------------------------------------
//
TInt CMediaFileDialogState::FolderId( TInt aListIndex )
    {
    TBool persistent = EFalse;
    TInt icon = EEIconEmpty;
    TInt id = KErrNotFound;
    TBuf<KBufSize64> buf;
    GetFolderInfo( aListIndex, id, buf, persistent, icon );
    
    return id;
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::FolderIndex
//
// -----------------------------------------------------------------------------
//
TInt CMediaFileDialogState::FolderIndex( TInt aListIndex )
    {
    if ( aListIndex < 0 )
        {
        return KErrNotFound;
        }

    TInt folderIndex = KErrNotFound;
    
    TInt count = iList.Count();
    for ( TInt i=0; i<count; i++ )
        {
        TMFListItem& item = iList[i];
        if ( item.iParentFolderId == iCurrentFolderId )
            {
            if ( item.iHidden )
                {
                continue;
                }
            if ( item.iListIndex == aListIndex && item.iPersistent )
                {
                folderIndex = i;
                break;
                }
            }
        }
    
    return folderIndex;
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::GetFolderInfo
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialogState::GetFolderInfo( TInt aListIndex, TInt& aId, TDes& aText,
                                           TBool& aPersistent, TInt& aIconId )
    {
    aId = KErrNotFound;
    aText = KNullDesC;
    aPersistent = EFalse;
    aIconId = EEIconEmpty;
    
    TMFListItem& currentFolder = iList[iCurrentFolderIndex];
    if ( currentFolder.iChildFolderId == EEFolderNone )
        {
        // this is optimization and is based on the the fact that a folder
        // cannot contain both folder items and audio/video items.
        
        return; // not folder (item is audio or video file)
        }
    
    TInt index = FolderIndex( aListIndex );
    if ( index != KErrNotFound )
        {
        // get persistent folder info
        TMFListItem& item = iList[index];
        TMFDialogUtil::StrCopy( aText, item.iText );
        aId = item.iFolderId;
        aPersistent = ETrue;
        aIconId = item.iIconId;
        }
    else
        {
        // get non-persistent folder info.
        // folder text (eg album name) is in database
        aId = currentFolder.iChildFolderId;
        aIconId = currentFolder.iChildIconId;
        }
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::EnableNullItemL
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialogState::EnableNullItemL( TInt aFolderId, const TDesC& aText,
                                              TInt aIconId  )
    {
    HideRootFolder( aFolderId, EFalse, aText, aIconId );
    }



// -----------------------------------------------------------------------------
// CMediaFileDialogState::AddPermanentItemsL
//
//
// Root
//   Downloads
//   Tones
//     <tone1>
//   Music
//     All
//     Artist
//       <artist1>
//       <artist2>
//         All
//         <album1>
//         <album2>
//         Unknown
//       Unknown
//     Album
//       <album1>
//         <song1>
//       <album2>
//       Unknown
//         <song1>
//         <song2>
//     Genre
//       Unknown
//     Composer
//       Unknown
//   Videos
//     <video1>
//     <video2>
//   Recordings
//      <recording1>
//      <recording2>
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialogState::AddPermanentItemsL()
    {
    TMFListItem item;
    
    // Root
    item.iListIndex = 0;
    item.iFolderId = EEFolderRoot;
    item.iParentFolderId = KErrNotFound;
    item.iChildFolderId = KErrNotFound;
    item.iPersistent = ETrue;
    item.iHidden = EFalse;
    item.iIconId = KErrNotFound;
    item.iChildIconId = KErrNotFound;
    item.iUnknownCount = KErrNotFound;
    StringLoader::Load( item.iText, R_QTN_MEDIA_FILE_DIALOG_TITLE );
    item.iTitle = KNullDesC;
    User::LeaveIfError( iList.Append( item ) );

    // Downloads
    item.iListIndex = 0;
    item.iFolderId = EEFolderDownload;
    item.iParentFolderId = EEFolderRoot;
    item.iChildFolderId = EEFolderNone;
    item.iPersistent = ETrue;
    item.iHidden = EFalse;
    item.iIconId = EEIconFolderDownload;
    item.iChildIconId = EEIconEmpty;
    item.iUnknownCount = KErrNotFound;
    StringLoader::Load( item.iText, R_QTN_FOLDER_DOWNLOAD );
    item.iTitle = KNullDesC;
    User::LeaveIfError( iList.Append( item ) );

    // Tones
    item.iListIndex = 1;
    item.iFolderId = EEFolderTones;
    item.iParentFolderId = EEFolderRoot;
    item.iChildFolderId = EEFolderNone;
    item.iPersistent = ETrue;
    item.iHidden = EFalse;
    item.iIconId = EEIconFolder;
    item.iChildIconId = EEIconTone;
    item.iUnknownCount = KErrNotFound;
    StringLoader::Load( item.iText, R_QTN_FOLDER_TONES );
    StringLoader::Load( item.iTitle, R_QTN_VIEW_TONES );
    User::LeaveIfError( iList.Append( item ) );
    
    // Music
    item.iListIndex = 2;
    item.iFolderId = EEFolderMusic;
    item.iParentFolderId = EEFolderRoot;
    item.iChildFolderId = KErrNotFound;
    item.iPersistent = ETrue;
    item.iHidden = EFalse;
    item.iIconId = EEIconFolder;
    item.iChildIconId = KErrNotFound;
    item.iUnknownCount = KErrNotFound;
    StringLoader::Load( item.iText, R_QTN_FOLDER_MUSIC );
    StringLoader::Load( item.iTitle, R_QTN_VIEW_MUSIC );
    User::LeaveIfError( iList.Append( item ) );

    // Videos
    item.iListIndex = 3;
    item.iFolderId = EEFolderVideos;
    item.iParentFolderId = EEFolderRoot;
    item.iChildFolderId = EEFolderNone;
    item.iPersistent = ETrue;
    item.iHidden = EFalse;
    item.iIconId = EEIconFolder;
    item.iChildIconId = EEIconVideo;
    item.iUnknownCount = KErrNotFound;
    StringLoader::Load( item.iText, R_QTN_FOLDER_VIDEOS );
    StringLoader::Load( item.iTitle, R_QTN_VIEW_VIDEOS );
    User::LeaveIfError( iList.Append( item ) );
    
    // Recordings
    item.iListIndex = 4;
    item.iFolderId = EEFolderRecordings;
    item.iParentFolderId = EEFolderRoot;
    item.iChildFolderId = EEFolderNone;    
    item.iPersistent = ETrue;
    item.iHidden = EFalse;
    item.iIconId = EEIconFolder;
    item.iChildIconId = EEIconFolder;
    item.iUnknownCount = KErrNotFound;
    StringLoader::Load( item.iText, R_QTN_FOLDER_RECORDINGS );
    StringLoader::Load( item.iTitle, R_QTN_VIEW_RECORDINGS );
    User::LeaveIfError( iList.Append( item ) );

    // Null item one
    item.iListIndex = 5;
    item.iFolderId = EEFolderNullItemOne;
    item.iParentFolderId = EEFolderRoot;
    item.iChildFolderId = EEFolderNone;
    item.iPersistent = ETrue;
    item.iHidden = ETrue;  // by default not visible
    item.iIconId = EEIconEmpty;
    item.iChildIconId = KErrNotFound;
    item.iUnknownCount = KErrNotFound;
    item.iText = KNullDesC;
    item.iTitle = KNullDesC;
    User::LeaveIfError( iList.Append( item ) );
    
    // Null item two
    item.iListIndex = 6;
    item.iFolderId = EEFolderNullItemTwo;
    item.iParentFolderId = EEFolderRoot;
    item.iChildFolderId = KErrNotFound;
    item.iPersistent = ETrue;
    item.iHidden = ETrue;  // by default not visible
    item.iIconId = EEIconEmpty;
    item.iChildIconId = KErrNotFound;
    item.iUnknownCount = KErrNotFound;
    item.iText = KNullDesC;
    item.iTitle = KNullDesC;
    User::LeaveIfError( iList.Append( item ) );

    // Search
    item.iListIndex = KErrNotFound;
    item.iFolderId = EEFolderSearch;
    item.iParentFolderId = EEFolderRoot;
    item.iChildFolderId = EEFolderNone;    
    item.iPersistent = ETrue;
    item.iHidden = ETrue;  // visible only when search is used in root level
    item.iIconId = EEIconEmpty;
    item.iChildIconId = KErrNotFound;
    item.iUnknownCount = KErrNotFound;
    item.iText = KNullDesC;
    item.iTitle = KNullDesC;

    //StringLoader::Load( item.iText, R_QTN_VIEW_SEARCH );
    //StringLoader::Load( item.iTitle, R_QTN_VIEW_SEARCH );
    User::LeaveIfError( iList.Append( item ) );


    // Music Search
    item.iListIndex = KErrNotFound;
    item.iFolderId = EEFolderSearchMusic;
    item.iParentFolderId = EEFolderMusic;
    item.iChildFolderId = EEFolderNone;    
    item.iPersistent = ETrue;
    item.iHidden = ETrue;  // visible only when search is used in root level
    item.iIconId = EEIconEmpty;
    item.iChildIconId = KErrNotFound;
    item.iUnknownCount = KErrNotFound;
    item.iText = KNullDesC;
    item.iTitle = KNullDesC;
    User::LeaveIfError( iList.Append( item ) );

    // All music
    item.iListIndex = 0;
    item.iFolderId = EEFolderAllMusic;
    item.iParentFolderId = EEFolderMusic;
    item.iChildFolderId = EEFolderNone;    
    item.iPersistent = ETrue;
    item.iHidden = EFalse;
    item.iIconId = EEIconFolder;
    item.iChildIconId = EEIconMusic;
    item.iUnknownCount = KErrNotFound;
    StringLoader::Load( item.iText, R_QTN_FOLDER_ALL_MUSIC );
    StringLoader::Load( item.iTitle, R_QTN_FOLDER_ALL_MUSIC );
    User::LeaveIfError( iList.Append( item ) );
    

    // Artist Root
    item.iListIndex = 1;
    item.iFolderId = EEFolderArtistRoot;
    item.iParentFolderId = EEFolderMusic;
    item.iChildFolderId = EEFolderArtist;
    item.iPersistent = ETrue;
    item.iHidden = EFalse;
    item.iIconId = EEIconFolder;
    item.iChildIconId = EEIconArtist;
    item.iUnknownCount = KErrNotFound;
    StringLoader::Load( item.iText, R_QTN_FOLDER_ARTIST );
    StringLoader::Load( item.iTitle, R_QTN_VIEW_ARTIST );
    User::LeaveIfError( iList.Append( item ) );

    // Album Root
    item.iListIndex = 2;
    item.iFolderId = EEFolderAlbumRoot;
    item.iParentFolderId = EEFolderMusic;
    item.iChildFolderId = EEFolderAlbum;
    item.iPersistent = ETrue;
    item.iHidden = EFalse;
    item.iIconId = EEIconFolder;
    item.iChildIconId = EEIconAlbum;
    item.iUnknownCount = KErrNotFound;
    StringLoader::Load( item.iText, R_QTN_FOLDER_ALBUM );
    StringLoader::Load( item.iTitle, R_QTN_VIEW_ALBUM );
    User::LeaveIfError( iList.Append( item ) );
    
    // Genre Root
    item.iListIndex = 3;
    item.iFolderId = EEFolderGenreRoot;
    item.iParentFolderId = EEFolderMusic;
    item.iChildFolderId = EEFolderGenre;
    item.iPersistent = ETrue;
    item.iHidden = EFalse;
    item.iIconId = EEIconFolder;
    item.iChildIconId = EEIconGenre;
    item.iUnknownCount = KErrNotFound;
    StringLoader::Load( item.iText, R_QTN_FOLDER_GENRE );
    StringLoader::Load( item.iTitle, R_QTN_VIEW_GENRE );
    User::LeaveIfError( iList.Append( item ) );
    
    // Composer Root
    item.iListIndex = 4;
    item.iFolderId = EEFolderComposerRoot;
    item.iParentFolderId = EEFolderMusic;
    item.iChildFolderId = EEFolderComposer;
    item.iPersistent = ETrue;
    item.iHidden = EFalse;
    item.iIconId = EEIconFolder;
    item.iChildIconId = EEIconComposer;
    item.iUnknownCount = KErrNotFound;
    StringLoader::Load( item.iText, R_QTN_FOLDER_COMPOSER );
    StringLoader::Load( item.iTitle, R_QTN_VIEW_COMPOSER );
    User::LeaveIfError( iList.Append( item ) );

    // Album ( represents some album under EEFolderAlbumRoot )
    item.iListIndex = KErrNotFound;
    item.iFolderId = EEFolderAlbum;
    item.iParentFolderId = EEFolderAlbumRoot;
    item.iChildFolderId = EEFolderNone;
    item.iPersistent = EFalse;
    item.iHidden = EFalse;
    item.iIconId = EEIconAlbum;
    item.iChildIconId = EEIconMusic;
    item.iUnknownCount = KErrNotFound;
    item.iText = KNullDesC;
    item.iTitle = KNullDesC;
    User::LeaveIfError( iList.Append( item ) );

    // Album Unknown ( all music files without album attribute )
    item.iListIndex = KErrNotFound;
    item.iFolderId = EEFolderAlbumUnknown;
    item.iParentFolderId = EEFolderAlbumRoot;
    item.iChildFolderId = EEFolderNone;
    item.iPersistent = ETrue;
    item.iHidden = EFalse;
    item.iIconId = EEIconUnknown;
    item.iChildIconId = EEIconMusic;
    item.iUnknownCount = KErrNotFound;
    StringLoader::Load( item.iText, R_QTN_UNKNOWN_ALBUM );
    StringLoader::Load( item.iTitle, R_QTN_UNKNOWN_ALBUM );
    User::LeaveIfError( iList.Append( item ) );
    
    // Genre ( represents some genre under EEFolderGenreRoot )
    item.iListIndex = KErrNotFound;
    item.iFolderId = EEFolderGenre;
    item.iParentFolderId = EEFolderGenreRoot;
    item.iChildFolderId = EEFolderNone;
    item.iPersistent = EFalse;
    item.iHidden = EFalse;
    item.iIconId = EEIconGenre;
    item.iChildIconId = EEIconMusic;
    item.iUnknownCount = KErrNotFound;
    item.iText = KNullDesC;
    item.iTitle = KNullDesC;
    User::LeaveIfError( iList.Append( item ) );

    // Genre Unknown ( all music files without genre attribute )
    item.iListIndex = KErrNotFound;
    item.iFolderId = EEFolderGenreUnknown;
    item.iParentFolderId = EEFolderGenreRoot;
    item.iChildFolderId = EEFolderNone;
    item.iPersistent = ETrue;
    item.iHidden = EFalse;
    item.iIconId = EEIconUnknown;
    item.iChildIconId = EEIconMusic;
    item.iUnknownCount = KErrNotFound;
    StringLoader::Load( item.iText, R_QTN_UNKNOWN_GENRE );
    StringLoader::Load( item.iTitle, R_QTN_UNKNOWN_GENRE );
    User::LeaveIfError( iList.Append( item ) );
    
    // Composer ( represents some composer under EEFolderComposerRoot )
    item.iListIndex = KErrNotFound;
    item.iFolderId = EEFolderComposer;
    item.iParentFolderId = EEFolderComposerRoot;
    item.iChildFolderId = EEFolderNone;
    item.iPersistent = EFalse;
    item.iHidden = EFalse;
    item.iIconId = EEIconComposer;
    item.iChildIconId = EEIconMusic;
    item.iUnknownCount = KErrNotFound;
    item.iText = KNullDesC;
    item.iTitle = KNullDesC;
    User::LeaveIfError( iList.Append( item ) );

    // Composer Unknown ( all music files without composer attribute )
    item.iListIndex = KErrNotFound;
    item.iFolderId = EEFolderComposerUnknown;
    item.iParentFolderId = EEFolderComposerRoot;
    item.iChildFolderId = EEFolderNone;
    item.iPersistent = ETrue;
    item.iHidden = EFalse;
    item.iIconId = EEIconUnknown;
    item.iChildIconId = EEIconMusic;
    item.iUnknownCount = KErrNotFound;
    StringLoader::Load( item.iText, R_QTN_UNKNOWN_COMPOSER );
    StringLoader::Load( item.iTitle, R_QTN_UNKNOWN_COMPOSER );
    User::LeaveIfError( iList.Append( item ) );



    //
    // folders under Artist root
    //
    
    // Artist (artist folder, folder name is artist name)
    item.iListIndex = KErrNotFound;
    item.iFolderId = EEFolderArtist;
    item.iParentFolderId = EEFolderArtistRoot;
    item.iChildFolderId = EEFolderArtistAlbum;
    item.iPersistent = EFalse;
    item.iHidden = EFalse;
    item.iIconId = EEIconArtist;
    item.iChildIconId = EEIconAlbum;
    item.iUnknownCount = KErrNotFound;
    item.iText = KNullDesC;
    item.iTitle = KNullDesC;
    User::LeaveIfError( iList.Append( item ) );

    // Artist all (all music files for one artist)
    item.iListIndex = 0;
    item.iFolderId = EEFolderArtistAll;
    item.iParentFolderId = EEFolderArtist;
    item.iChildFolderId = EEFolderNone;
    item.iPersistent = ETrue;
    item.iHidden = EFalse;
    item.iIconId = EEIconFolder;
    item.iChildIconId = EEIconMusic;
    item.iUnknownCount = KErrNotFound;
    StringLoader::Load( item.iText, R_QTN_FOLDER_ARTIST_ALL );
    StringLoader::Load( item.iTitle, R_QTN_FOLDER_ARTIST_ALL );
    User::LeaveIfError( iList.Append( item ) );

    // Artist album (one album, folder name is album name)
    item.iListIndex = KErrNotFound;
    item.iFolderId = EEFolderArtistAlbum;
    item.iParentFolderId = EEFolderArtist;
    item.iChildFolderId = EEFolderNone;
    item.iPersistent = EFalse;
    item.iHidden = EFalse;
    item.iIconId = EEIconAlbum;
    item.iChildIconId = EEIconMusic;
    item.iUnknownCount = KErrNotFound;
    item.iText = KNullDesC;
    item.iTitle = KNullDesC;
    User::LeaveIfError( iList.Append( item ) );

    // Artist album unknown (music files for one artist without album attribute)
    item.iListIndex = KErrNotFound;
    item.iFolderId = EEFolderArtistAlbumUnknown;
    item.iParentFolderId = EEFolderArtist;
    item.iChildFolderId = EEFolderNone;
    item.iPersistent = ETrue;
    item.iHidden = EFalse;
    item.iIconId = EEIconUnknown;
    item.iChildIconId = EEIconMusic;
    item.iUnknownCount = KErrNotFound;
    StringLoader::Load( item.iText, R_QTN_UNKNOWN_ALBUM );
    StringLoader::Load( item.iTitle, R_QTN_UNKNOWN_ALBUM );
    User::LeaveIfError( iList.Append( item ) );

    // Artist unknown (all music files without artist attribute)
    item.iListIndex = KErrNotFound;
    item.iFolderId = EEFolderArtistUnknown;
    item.iParentFolderId = EEFolderArtistRoot;
    item.iChildFolderId = EEFolderNone;
    item.iPersistent = ETrue;
    item.iHidden = EFalse;
    item.iIconId = EEIconUnknown;
    item.iChildIconId = EEIconMusic;
    item.iUnknownCount = KErrNotFound;
    StringLoader::Load( item.iText, R_QTN_UNKNOWN_ARTIST );
    StringLoader::Load( item.iTitle, R_QTN_UNKNOWN_ARTIST );
    User::LeaveIfError( iList.Append( item ) );
    }



// -----------------------------------------------------------------------------
// CMediaFileDialogState::Panic
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialogState::Panic( TInt aReason ) 
    {
	_LIT( KPanicCategory, "CMediaFileDialogState" );
	
	User::Panic( KPanicCategory, aReason ); 
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogState::PrintCurrentFolder (test code)
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialogState::PrintCurrentFolder()
    {
	TBuf<KBufSize128> buf;
	
	GetFolderTypeText( buf, iCurrentFolderId );
	User::InfoPrint( buf );
    }
		
		
// -----------------------------------------------------------------------------
// CMediaFileDialogState::GetFolderTypeText (test code)
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialogState::GetFolderTypeText(TDes& aText, TInt aType)
    {
	switch (aType)
		{
		case EEFolderRoot:
			aText = _L("EFolderRoot");
			break;
		case EEFolderTones:
			aText = _L("EFolderTones");
			break;
		case EEFolderMusic:
			aText = _L("EFolderMusic");
			break;
		case EEFolderVideos:
			aText = _L("EFolderVideos");
			break;
		case EEFolderRecordings:
			aText = _L("EFolderRecordings");
			break;
        case EEFolderSearch:
            aText = _L("EEFolderSearch");
            break;

		case EEFolderRecent:
			aText = _L("EFolderRecent");
			break;
		

		case EEFolderArtistRoot:
			aText = _L("EFolderArtistRoot");
			break;
		case EEFolderArtist:
			aText = _L("EFolderArtist");
			break;
		case EEFolderArtistUnknown:
			aText = _L("EFolderArtistUnknown");
			break;
		case EEFolderArtistAll:
			aText = _L("EEFolderArtistAll");
	        break;
    	case EEFolderArtistAlbum:
			aText = _L("EEFolderArtistAlbum");
			break;
		case EEFolderArtistAlbumUnknown:
			aText = _L("EEFolderArtistAlbumUnknown");
			break;


		case EEFolderAllMusic:
			aText = _L("EFolderAllMusic");
			break;

		case EEFolderAlbumRoot:
			aText = _L("EFolderAlbumRoot");
			break;
		case EEFolderAlbum:
			aText = _L("EFolderAlbum");
			break;
		case EEFolderAlbumUnknown:
			aText = _L("EFolderAlbumUnknown");
			break;
			
		case EEFolderGenreRoot:
			aText = _L("EFolderGenreRoot");
			break;
		case EEFolderGenre:
			aText = _L("EFolderGenre");
			break;
		case EEFolderGenreUnknown:
			aText = _L("EFolderGenreUnknown");
			break;
			
		case EEFolderComposerRoot:
			aText = _L("EFolderComposerRoot");
			break;
		case EEFolderComposer:
			aText = _L("EFolderComposer");
			break;
		case EEFolderComposerUnknown:
			aText = _L("EFolderComposerUnknown");
			break;
			
		case EEFolderUnknown:
			aText = _L("EFolderUnknown");
			break;

		case EEFolderNone:
			aText = _L("EFolderNone");
			break;

		default:
			aText = _L("### unknown ###");
			break;
		}
    }







//  End of File  
