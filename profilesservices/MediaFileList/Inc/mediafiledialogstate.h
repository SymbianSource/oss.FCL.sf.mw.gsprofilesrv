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
* Description:   Header file of mediafiledialogstate.
*                Class used to deal with the items of the Main view
*
*/



#ifndef MEDIAFILEDIALOGSTATE_H
#define MEDIAFILEDIALOGSTATE_H

// INCLUDES
#include "mediafiledialogutils.h"

#include <e32base.h>


// CONSTANTS


enum TMediaFileListFolder
	{
	EEFolderRoot = 0,
	EEFolderDownload,
	EEFolderTones,
	EEFolderMusic,
	EEFolderVideos,
	EEFolderRecordings,
	EEFolderSearch,
	EEFolderAllMusic,
	EEFolderArtistRoot,
	EEFolderArtist,
	EEFolderArtistUnknown,  // 10
	EEFolderArtistAll,
	EEFolderArtistAlbum,
	EEFolderArtistAlbumUnknown,
	EEFolderAlbumRoot,
	EEFolderAlbum,
	EEFolderAlbumUnknown,
	EEFolderGenreRoot,
	EEFolderGenre,
	EEFolderGenreUnknown,
	EEFolderComposerRoot,  // 20
	EEFolderComposer,
	EEFolderComposerUnknown,
	EEFolderNullItemOne,
	EEFolderNullItemTwo,
	EEFolderRecent,
	EEFolderNone,
	EEFolderPersistent,
	EEFolderUnknown,
	EEFolderSearchMusic
	};


enum TMediaFileListIcons
	{
	EEIconMusic = 0,
	EEIconTone = 1,
	EEIconVideo = 2,
	EEIconRecording = 3,
	EEIconArtist = 4,
    EEIconAlbum = 5,
    EEIconGenre = 6,
    EEIconComposer = 7,
    EEIconFolder = 8,
    EEIconUnknown = 9,
    EEIconFolderDownload = 10,
    EEIconNullItemOff = 11,
	EEIconMemoryCard = 12,
	EEIconMassStorage = 13,
    EEIconEmpty = 14	
    };

enum TMediaFileListQueryDirection
    {
    EEQueryUp,
    EEQueryDown,
    EEQueryUpdate
    };



// CLASS DECLARATION

NONSHARABLE_CLASS (TMFListItem)
	{
    public:
        // position in listbox
        TInt iListIndex;
        
        // one of TMediaFileListFolder
        TInt iFolderId;
        
        // one of TMediaFileListFolder
        TInt iParentFolderId;
        
        // non-persistent child items from metadata (folders or file names)
        TInt iChildFolderId;
        
        // is list item a persistent folder 
        TBool iPersistent;
        
        // is list item hidden
        TBool iHidden;

        // icon that is shown in listbox
        TInt iIconId;
        
        // icon that is shown in listbox
        TInt iChildIconId;
        
        // count of unknown items in this folder
        TInt iUnknownCount;

        // text that is shown in listox
        TBuf<KBufSize128> iText;
        
        // text that is shown in navi pane
        TBuf<KBufSize128> iTitle;
        
        
    public:
        TMFListItem();
	};

/**
* TMFCurrentItemInfo
* 
* Used for maintaining the open item info in a list box
* folder and current list item position.
*/

NONSHARABLE_CLASS (TMFCurrentItemInfo)
    {
    public:
        // has media file database changed 
        TBool iIndexCheckNeeded;
        
        // current item listbox index
        TInt iItemIndex;
        
        // currently selected media file id
        TInt iMediaFileId;
        
        // currently selected persistent folder id
        TInt iFolderId;
        
        // currently selected non-persistent folder id
        TBuf<KBufSize128> iFolderText;
        
    public:
        void Reset();
    };




/**
* CMediaFileDialogState
* 
* Used for maintaining MediaFileDialog state eg currently open
* folder and current list item position.
*/
NONSHARABLE_CLASS (CMediaFileDialogState) : public CBase
	{
    public:
        static CMediaFileDialogState* NewL();
        virtual ~CMediaFileDialogState();
    
    private:
        CMediaFileDialogState();
        void ConstructL();
	
    public:
        void Init();
        void AddPermanentItemsL();
    
        TInt NextFolderUp();
        TInt NextFolderDown( TInt aListIndex );
        void SetQueryInfo( TInt aQueryId, const TDesC& aQueryText,
                           TInt aQueryListPosition, TInt aQueryDirection );
        void SetStateAfterQuery();
        
    public:        
        void SetCurrentFolder( TInt aFolderId );
        TInt CurrentFolder();
     
        TBool FindBoxNeeded();
        TBool QueryNeeded();
        TBool QueryNeeded( TInt aFolderId );
        TInt QueryFolder();

        void SetText( TInt aFolderId, const TDesC& aText );
        void GetText( TInt aFolderId, TDes& aText );
        void SetTitle( TInt aFolderId, const TDesC& aText );
        void GetTitle( TInt aFolderId, TDes& aText );
        void GetSearchTitleL( TDes& aText, TInt aCount );
        void GetSearchTitle( TDes& aText, TInt aCount );

        void SetUnknownFolderPosition( TInt aQueryCount );
        void SetListIndex( TInt aFolderId, TInt aListIndex );
        TInt ListIndex( TInt aFolderId );
        void EnableNullItemL( TInt aFolderId, const TDesC& aText, TInt aIconId );


    private:
        TInt IndexFromId( TInt aFolderId );
        TInt FolderIndex( TInt aListIndex );
        
    public:
        TInt PersistentFolderCount( TInt aFolderId );
        TInt PersistentFolderCount();
        TInt LeadingPersistentFolderCount();
        TInt CurrentItemIndex();
        void SetCurrentItemIndex( TInt aIndex );
        TInt FolderId( TInt aListIndex );
        void GetFolderInfo( TInt aListIndex, TInt& aId, TDes& aText,
                             TBool& aPersistent, TInt& aIconId );

    public:
        TMFCurrentItemInfo& CurrentItemInfo();
        TInt QueryDirection();
        TInt PreviousFolderId();
        TBool IsPersistent( TInt aFolderId );

    public:
        void ResetUnknownCount();
        void SetUnknownCount( TInt aFolderId, TInt aCount );
        TInt UnknownCount( TInt aFolderId );
        void HideUnknownFolders();
        void HideUnknownFolder( TInt aFolderId, TInt aUnknownCount );
        void HideRootFolder( TInt aFolderId, TBool aHidden,
                             const TDesC& aText, TInt aIconId );
     
    public:
        void PrintCurrentFolder();
        
    public:
        static void GetFolderTypeText(TDes& aText, TInt aType);
        
    private:
        static void Panic( TInt aReason );
        
    private:
        RArray<TMFListItem> iList;
        TInt iCurrentFolderId;
        TInt iCurrentFolderIndex;
        TInt iListPosition;
        
        // temporary query info - iList is updated after successful query 
        TInt iQueryFolderId;
        TInt iQueryDirection;
        TBuf<KBufSize128> iQueryText;
        
        // previous folder id
        TInt iPreviousFolderId;
        
        // current item info, needed for setting focus to current item
        // when media file database changes
        TMFCurrentItemInfo iCurrentItemInfo;
	};



#endif  // MEDIAFILEDIALOGSTATE_H

// End of File
