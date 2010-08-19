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
* Description:   Used to interact with the MDE. 
*
*/



#ifndef MEDIAFILEHANDLER_H
#define MEDIAFILEHANDLER_H


// INCLUDES

#include "mediafiledialogutils.h"


#include <mdesession.h>
#include <mdequery.h>
#include <mdelogiccondition.h>
#include <mdeconstants.h>
#include <mdeobjectquery.h>
#include <mdccommon.h>
#include <mdeitem.h>
#include <mdeobject.h>


// FORWARD DECLARATIONS
class CSearchList;
class CRomFileList;
class CArtistQuery;

// CONSTANTS

_LIT( KMimeTypeAMR, "audio/amr" );
_LIT( KMimeTypeWAV, "audio/wav" );
_LIT( KMimeMp3, "mp3" );
_LIT( KDriveZ, "z:\\" );
_LIT( KSpace, " " );


/**
* CMediaFileData
* 
* Used for storing metadata information for one media file.
*/
NONSHARABLE_CLASS (CMediaFileData) : public CBase
    {
    public:
        static CMediaFileData* NewL();
        virtual ~CMediaFileData();

    private:
        CMediaFileData();
        void ConstructL();
        
    public:
        const TDesC& Attr( TInt aAttr );
        void GetAttr( TInt aAttr, TDes& aValue );
        void SetAttrL( TInt aAttr, const TDesC& aValue );

    public:
        TInt iMediaFileType;
        
    private:
        HBufC* iFullName;
        HBufC* iName;
        HBufC* iArtist;
        HBufC* iAlbum;
        HBufC* iGenre;
        HBufC* iComposer;
    };


/**
* MMediaFileHandlerObserver
*
* For observing CMediaFileHandler events.  
*/
NONSHARABLE_CLASS (MMediaFileHandlerObserver)
	{
    public:
    enum TMediaFileHandlerEvent
        {
        EInitComplete,
        EQueryComplete,
        EQueryCanceled,
        EUnknownQueryComplete,
        EMediaFileChanged,
        EScanRomComplete,
        EError
        };
    
    enum TScanRomState
        {
        ENoneRomScan,
        ERomScanInProgress,
        ERomScanComplete,
        ERomScanError
        };
	
	public:
		virtual void HandleMFEventL( TInt aEvent, TInt aError ) = 0;
	};



/**
* CMediaFileHandler
*
* Used for making media file queries. For example get all songs for
* one artist. Uses metadata engine.
*/
NONSHARABLE_CLASS (CMediaFileHandler) : public CBase, 
                                        public MMdESessionObserver,
                                        public MMdEQueryObserver,
                                        public MMdEObjectObserver,
                                        public MMdEObjectPresentObserver,
                                        public MMFActiveCallerObserver
    {
    public:
    enum TMediaType
        {
        EMediaTypeTone = 0,
        EMediaTypeVideo,
        EMediaTypeRecording,
        EMediaTypeMusic
        };

    enum TStorageType
         {
         EPhoneMemory = 0,
         ERomStorage,
         EMassStorage,
         EMemoryCard
         };

    enum TQueryAttribute
        {
        EAttrMediaType = 20,  // integer
        EAttrFileSize,        // integer
        EAttrStorageType,     // integer
        EAttrMediaFileId,     // integer
        EAttrFileName,     // string
        EAttrFullName,     // string
        EAttrSongName,     // string
        EAttrArtist,       // string
        EAttrAlbum,        // string
        EAttrGenre,        // string
        EAttrComposer      // string
        };

    enum TQueryType
        {
        EQueryNone = 0,
        EQueryAudio,
        EQueryMusic,
        EQueryVideo,
        EQueryRecording,
        EQueryTone,
        ESearchAudio,
        ESearchVideo,
        ESearchMusic,
        ESearchTones,
        EQueryEmpty,
        EQueryUnknown,
        
        EQueryArtist,
        EQueryArtistAlbum,
        EQueryArtistAll,
        EQueryArtistUnknown,
        EQueryArtistEmpty,
        };

    public:
        static CMediaFileHandler* NewL();
        virtual ~CMediaFileHandler();

    private:
	    CMediaFileHandler();
        void ConstructL();

    public:
        void SetAttrL( TInt aAttr, TInt aValue );
        void SetAttrL( TInt aAttr, const TDesC& aValue );
        void SetAttrL( TInt aAttr, TAny* aValue );

    public:
        void QueryMusicL( TInt aAttr );
        void QueryMusicL( TInt aAttr, const TDesC& aFindText );
        void QueryMusicUnknownL( TInt aAttr );
        void QueryMusicUnknownCountL( TInt aAttr );
        void QueryAttributeValuesL( TInt aAttr );
        void QueryArtistL( TInt aAttr, const TDesC& aArtist, 
                           const TDesC& aAlbum, TBool aRefresh = EFalse );
        void QueryArtistEmptyL();
        void QueryEmptyL();
        void QueryRomEmptyL( TInt aQueryType );
        
        void QueryVideosL( TInt aAttr );
        void QueryRecordingsL( TInt aAttr );
        void QueryTonesL( TInt aAttr );
        void QueryDefaultTonesL( TInt /*aAttr*/ );
       
        void SearchL( const TDesC& aSearchText );
        void SearchAudioL( const TDesC& aSearchText );
        
        void StartRomScaning( TInt aQueryType );
        
    public:
        TInt UnknownArtistCount();
        TInt UnknownArtistCountL();
        TInt ResultCount();
        TBool QueryReady();
        TInt Attribute( TInt aIndex, TInt aAttr, TInt aQueryId );
        void GetAttributeL( TInt aIndex, TInt aAttr, TDes& aValue, TInt aQueryId );
        void GetAttribute( TInt aIndex, TInt aAttr, TDes& aValue, TInt aQueryId );
        CMediaFileData* ReadMediaFileDataL( TInt aId );
        void CancelQuery();
        
        /**
        * Remove an item from list of search reuslt
        * @param aIndex Search list's index.
        * @return None.
        */
        void Remove( TInt aIndex );

    public:
        TInt ItemIndex( const TDesC& aItemText, TInt aIndex );
        TInt ItemIndex( TInt aItemId, TInt aIndex );
        
    private:
        
        void SearchVideoL( const TDesC& aSearchText );
        void SearchMusicL( const TDesC& aSearchText );
        void SearchTonesL( const TDesC& aSearchText );
        static TInt SearchTones( TAny *aObj );
        static TInt SearchVideo( TAny *aObj );

      
    public:
        void SetObserver( MMediaFileHandlerObserver* aObserver );
        void SetQueryId( TInt aId );
        void EnableObserverCall( TBool aEnable );
        
        
    public:
	CMediaFileData* RomMediaFileDataL( TInt aId ,TInt aMediaType );
           
    private:
        CMdEPropertyDef& PropertyDefL( TInt aAttr );
        void StrCopy( TDes& aTarget, const TDesC& aSource );
        void CheckAttrL( TInt aAttr );
        void CallObserverWithDelay();
        void CallObserver( TInt aEvent, TInt aError );
        void AddObjectObserverL();
        
    public:
        static CMdEPropertyDef& PropertyDefL( CMdESession* aSession, TInt aAttr );
        
    private:
        void ExcludeRecordingsL( CMdELogicCondition& aCondition );
        void ExcludeRomFilesL( CMdELogicCondition& aCondition );
        void ExcludeMimeTypesL( CMdELogicCondition& aCondition, TInt aAttr );
        void IncludeMusicPropertiesL( CMdELogicCondition& aCondition );
        void ExcludeMusicPropertiesL( CMdELogicCondition& aCondition );
        void IncludeMusicSearchConditionL( CMdELogicCondition& aCondition,
                                           const TDesC& aSearchText );
        void IncludeToneSearchConditionL( CMdELogicCondition& aCondition,
                                           const TDesC& aSearchText );
        void IncludeVideoSearchConditionL( CMdELogicCondition& aCondition,
                                           const TDesC& aSearchText );


        
    private:
        void LeaveIfSessionClosedL();
        void Panic( TInt aReason );
        TInt MediaType( TInt aQueryType );
        

    private:  // from MMdESessionObserver
        void HandleSessionOpened(CMdESession& aSession, TInt aError);
        void HandleSessionError(CMdESession& aSession, TInt aError);
        
    
    private:  // from MMdEQueryObserver (mdequery.h)
        void HandleQueryNewResults( CMdEQuery& aQuery, TInt aFirstNewItemIndex,
                                    TInt aNewItemCount );
        void HandleQueryCompleted( CMdEQuery& aQuery, TInt aError );
        void HandleQueryCompletedL( CMdEQuery& aQuery, TInt aError );
        
    private:  // from MMdEObjectObserver
        void HandleObjectNotification( CMdESession& aSession, 
                                       TObserverNotificationType aType,
                                       const RArray<TItemId>& aObjectIdArray);
                                       
    private:  // from MMdEObjectPresentObserver
        void HandleObjectPresentNotification( CMdESession& aSession, 
                         TBool aPresent, const RArray<TItemId>& aObjectIdArray);
        
    private:   // from MMFActiveCallerObserver
         void HandleActiveCallL( TInt aCallId );

    private:
    
        // media file event observer
        MMediaFileHandlerObserver* iObserver;
       
        // session to metadata engine
        CMdESession* iSession;
      
        // metadata query
        CMdEObjectQuery* iQuery;
        
        // used for optimizing artist queries
        CArtistQuery* iArtistQuery;

        // search result list - ui shows items from this list
        CSearchList* iSearchList;

        // metadata search text
        TBuf<KBufSize128> iSearchText;
         
        // for generating active object calls
        CMFActiveCaller* iQueryCaller;
        // for generating active object calls
        CMFActiveCaller* iObjectNotificationCaller;
        
        // for finding out file drive type
        CDriveUtil* iDriveUtil;
        
        // mime types that are exluded from queries
        CMimeTypeList* iExcludedMimeTypes;

        // for starting video query after short delay
        CIdle* iIdle;

        // used to prevent observer call when query is canceled fro ui
        TBool iObserverCallEnabled;
        
        // query id from UI, not used in this class
        TInt iQueryId;
        
        // max audio file file size
        TInt iMaxFileSize;
        
        // is metadata session open
        TBool iSessionOpen;

        // query type 
        TInt iQueryType;

        // query error
        TInt iQueryError;
        
        // query event
        TInt iQueryEvent;
        
	    TBool iVideoSearchDisabled; 
        
        TInt iMediaFileCounter;
        //for scan data from rom
        CRomFileList* iRomFileList;
	    //Record the rom scan state.
	    TInt iRomScanState;
	    //Tone query in progress
	    TBool iQueryinProgress;

    };




/**
* CSearchListItem
*
* Data for one media file item CSearchList.
*/
NONSHARABLE_CLASS (CSearchListItem) : public CBase 
    {
    public:
        static CSearchListItem* NewLC();
        static CSearchListItem* NewL();
        virtual ~CSearchListItem();

    private:
        CSearchListItem();
        void ConstructL();
        
    public:
        void SetTitleL( const TDesC& aText, TBool aFullName );
        const TDesC& Title() const;
 
    public:
        TInt iId;
        TInt iMediaType;
        TInt iStorageType;
    private:
        HBufC* iTitle;
    };


/**
* CRomFileList
* 
* Used for reading media files from rom.
*/
NONSHARABLE_CLASS (CRomFileList) : public CBase
    {
    public:
        static CRomFileList* NewL();
        virtual ~CRomFileList();

    private:
        CRomFileList();
        void ConstructL();
    public:
        void ReadDirectoryL( const TDesC& aDirectory, RArray<TFileName>& aList );
        void ReadDirectoryL( const TDesC& aDirectory, 
                                         RPointerArray<CSearchListItem>& aList );

        void ReadRomFilesL();
        void ReadRomVediosL();
        const TDesC& Item( TInt aIndex , TInt aType );
        TInt RomFileItem( CSearchListItem* aItem, TInt aIndex);
        TInt RomVedioItem( CSearchListItem* aItem, TInt aIndex);
        TInt Count( TInt aType );
        void Reset();
        
    public:
//    //Used to return the rom file attribute in rom;
//    TInt RomFileAttribute(TInt aIndex, TInt aAttr);
    //Used to return the title of rom item
    TInt GetRomFileName(TInt aIndex, TInt aType, TDes& aValue);
private:
    CSearchListItem* SearchListItemFromRomL(const TDesC& aFileName,
            TInt aIndex);
	//Used to get the title of rom item
	void RomItemTitle(TInt aIndex, TDes& aTitle );
        
private:
        TFileName iFileName;
        RArray<TFileName> iList;
        RArray<TFileName> iVedioList;
        RPointerArray<CSearchListItem> iRomToneList;
        RPointerArray<CSearchListItem> iRomVideoList;

    };

/**
* CSearchList
*
* List for storing search result for one (most recent) metadata query.
*/
NONSHARABLE_CLASS (CSearchList) : public CBase 
    {
    public:
        static CSearchList* NewL();
        virtual ~CSearchList();

    private:
        CSearchList();
        void ConstructL();
        
    public:
        void SetSession( CMdESession* aSession );
        void SetRomFileList( CRomFileList* aRomFileList );
        void TakeResultL( CMdEObjectQuery* aQuery, TInt aMediaType );
        void TakeArtistResultL( CArtistQuery* aArtistQuery );
        void TakeDistinctResultL( CMdEObjectQuery* aQuery, TInt aMediaType );
        void SearchInToneL( CRomFileList* aQuery,  const TDesC& aSearchText );
        void VideoSearchInRomL( CRomFileList* aQuery,  const TDesC& aSearchText );
  
        void AddAlbumItemL( const TDesC& aAlbum );
        void AddItemL( CMdEObject& aObject, TInt aMediaType );
        void AddRomItemL( CRomFileList* aRomList, TInt aMediaType );
        void AddArtistItemL( CMdEObject& aObject, TInt aQueryType, const TDesC& aAlbum );

        void Reset();
        void Sort();
        TInt Count();
        void GetAttributeL( TInt aIndex, TInt aAttr, TDes& aValue );
        TInt Attribute( TInt aIndex, TInt aAttr );
        void SetSearchId( TInt aId );
        TInt SearchId();
        
        /**
        * Remove an item from the list of search results.
        * @param aIndex List's index.
        * @return None.
        */
        void Remove( TInt aIndex );
        
    public:
        TInt ItemIndex( TInt aId );
        TInt ItemIndex( const TDesC& aText );
        TInt ItemIndex( const TDesC& aText, TInt aIndex );
        TInt ItemIndex( TInt aId, TInt aIndex );
        
    private:
        static TInt Compare( const CSearchListItem& aFirst, 
                             const CSearchListItem& aSecond );
        void Panic( TInt aReason );
        const TDesC& TextL( CMdEObject& aObject, TInt aId );
       
    private:
        CMdESession* iSession;  // does not own
        
        CRomFileList* iRomFileList; //does not own
        
        // list of search results
        RPointerArray<CSearchListItem> iList;
        
        // for finding out drive type (eg memory card)
        CDriveUtil* iDriveUtil;
        
        // search id - not used in this class
        TInt iSearchId;
    };





/**
* CArtistQuery
* 
* CArtistQuery is for storing all metadata for one artist.
* Once CArtistQuery is initialized it can be used to get artist 
* subqueries without making more metadata queries.
*/
NONSHARABLE_CLASS (CArtistQuery) : public CBase 
    {
    public:
        static CArtistQuery* NewL();
        virtual ~CArtistQuery();

    private:
        CArtistQuery();
        void ConstructL();
        
    public:
        void ResetL();
        
        void SetArtistL( const TDesC& aText );
        void SetAlbumL( const TDesC& aText );
        void SetQueryType( TInt aType );
        void SetQuery( CMdEObjectQuery* aQuery );

        const TDesC& Artist();
        const TDesC& Album();
        TInt QueryType();
        CMdEObjectQuery* Query();
        TInt UnknownCount();
        
    private:
        CMdEObjectQuery* iQuery;
        TInt iQueryType;
        
        HBufC* iArtist;
        HBufC* iAlbum;
    };



#endif  // MEDIAFILEHANDLER_H

// End of File
