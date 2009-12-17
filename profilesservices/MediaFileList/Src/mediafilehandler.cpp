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




#include "mediafilehandler.h"
#include "mediafilelistdebug.h"
#include "mediafiledialogstate.h"
#include "mediafilelist.h"

#include <pathinfo.h>
#include <bautils.h>




/******************************************************************************
 * class CMediaFileData
 ******************************************************************************/



// -----------------------------------------------------------------------------
// CMediaFileData::NewL
// 
// -----------------------------------------------------------------------------
//
CMediaFileData* CMediaFileData::NewL()
    {
    CMediaFileData* self = new (ELeave) CMediaFileData();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }


// -----------------------------------------------------------------------------
// Destructor
// 
// -----------------------------------------------------------------------------
//
CMediaFileData::~CMediaFileData()
    {
    delete iFullName;
    delete iName;
    delete iArtist;
    delete iAlbum;
    delete iGenre;
    delete iComposer;
    }


// -----------------------------------------------------------------------------
// CMediaFileData::CMediaFileData
// 
// -----------------------------------------------------------------------------
//
CMediaFileData::CMediaFileData()
    {
    iMediaFileType = KErrNotFound;
    }


// -----------------------------------------------------------------------------
// CMediaFileData::ConstructL
// 
// -----------------------------------------------------------------------------
//
void CMediaFileData::ConstructL()
    {
    iFullName  = HBufC::NewL( 0 );
    iName      = HBufC::NewL( 0 );
    iArtist    = HBufC::NewL( 0 );
    iAlbum     = HBufC::NewL( 0 );
    iGenre     = HBufC::NewL( 0 );
    iComposer  = HBufC::NewL( 0 );
    }


// -----------------------------------------------------------------------------
// CMediaFileData::Attr
// 
// -----------------------------------------------------------------------------
//
const TDesC& CMediaFileData::Attr( TInt aAttr )
    {
    if ( aAttr == CMediaFileHandler::EAttrFullName )
        {
        return *iFullName;
        }
    if ( aAttr == CMediaFileHandler::EAttrSongName )
        {
        return *iName;
        }
    if ( aAttr == CMediaFileHandler::EAttrArtist )
        {
        return *iArtist;
        }
    if ( aAttr == CMediaFileHandler::EAttrAlbum )
        {
        return *iAlbum;
        }
    if ( aAttr == CMediaFileHandler::EAttrGenre )
        {
        return *iGenre;
        }
    if ( aAttr == CMediaFileHandler::EAttrComposer )
        {
        return *iComposer;
        }

    return KNullDesC;
    }


// -----------------------------------------------------------------------------
// CMediaFileData::SetAttrL
// 
// -----------------------------------------------------------------------------
//
void CMediaFileData::SetAttrL( TInt aAttr, const TDesC& aValue )
    {
    if ( aAttr == CMediaFileHandler::EAttrFullName )
        {
        delete iFullName;
        iFullName = NULL;
        iFullName = aValue.AllocL();
        }
    if ( aAttr == CMediaFileHandler::EAttrSongName )
        {
        delete iName;
        iName = NULL;
        iName = aValue.AllocL();
 
        }
    if ( aAttr == CMediaFileHandler::EAttrArtist )
        {
        delete iArtist;
        iArtist = NULL;
        iArtist = aValue.AllocL();
        }
    if ( aAttr == CMediaFileHandler::EAttrAlbum )
        {
        delete iAlbum;
        iAlbum = NULL;
        iAlbum = aValue.AllocL();
        }
    if ( aAttr == CMediaFileHandler::EAttrGenre )
        {
        delete iGenre;
        iGenre = NULL;
        iGenre = aValue.AllocL();
        }
    if ( aAttr == CMediaFileHandler::EAttrComposer )
        {
        delete iComposer;
        iComposer = NULL;
        iComposer = aValue.AllocL();
        }
    }


// -----------------------------------------------------------------------------
// CMediaFileData::GetAttr
// 
// -----------------------------------------------------------------------------
//
void CMediaFileData::GetAttr( TInt aAttr, TDes& aValue )
    {
    if ( aAttr == CMediaFileHandler::EAttrFullName )
        {
        TMFDialogUtil::StrCopy( aValue, iFullName->Des() );
        }
    else if ( aAttr == CMediaFileHandler::EAttrSongName )
        {
        TMFDialogUtil::StrCopy( aValue, iName->Des() );
        }
    else if ( aAttr == CMediaFileHandler::EAttrArtist )
        {
        TMFDialogUtil::StrCopy( aValue, iArtist->Des() );
        }
    else if ( aAttr == CMediaFileHandler::EAttrAlbum )
        {
        TMFDialogUtil::StrCopy( aValue, iAlbum->Des() );
        }
    else if ( aAttr == CMediaFileHandler::EAttrGenre )
        {
        TMFDialogUtil::StrCopy( aValue, iGenre->Des() );
        }
    else if ( aAttr == CMediaFileHandler::EAttrComposer )
        {
        TMFDialogUtil::StrCopy( aValue, iComposer->Des() );
        }
    else
        {
        aValue = KNullDesC;
        }
    }
    

/******************************************************************************
 * class CMediaFileHandler
 ******************************************************************************/



// -----------------------------------------------------------------------------
// CMediaFileHandler::NewL
// 
// -----------------------------------------------------------------------------
//
CMediaFileHandler* CMediaFileHandler::NewL()
    {
    CMediaFileHandler* self = new (ELeave) CMediaFileHandler();
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );

	return self;
    }


// -----------------------------------------------------------------------------
// Destructor
// 
// -----------------------------------------------------------------------------
//
CMediaFileHandler::~CMediaFileHandler()
    {
    delete iSearchList;
    
    delete iQueryCaller;
    delete iObjectNotificationCaller;
    
    delete iDriveUtil;
    delete iExcludedMimeTypes;
    delete iIdle;
    
    delete iQuery;
    delete iArtistQuery;
    delete iSession;
    
    delete iRomFileList;
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::CMediaFileHandler
// 
// -----------------------------------------------------------------------------
//
CMediaFileHandler::CMediaFileHandler()
    {
    iSessionOpen = EFalse;
    iObserverCallEnabled = ETrue;
    
    iMaxFileSize = KErrNotFound;
    iQueryId = KErrNotFound;
    iQueryType = KErrNotFound;
    iQueryError = KErrNone;
    iQueryEvent = KErrNotFound;
    iVideoSearchDisabled = EFalse;
    
    iMediaFileCounter = 0;
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::ConstructL
// 
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::ConstructL()
    {
    iSession = CMdESession::NewL( *this );
    iSearchList = CSearchList::NewL();
    
    iQueryCaller = CMFActiveCaller::NewL( this );
    iObjectNotificationCaller = CMFActiveCaller::NewL( this );
    
    iDriveUtil = CDriveUtil::NewL();
    iArtistQuery = CArtistQuery::NewL();
    iExcludedMimeTypes = CMimeTypeList::NewL();
    
    iRomFileList = CRomFileList::NewL();
    iQueryinProgress = EFalse;
    iRomScanState = MMediaFileHandlerObserver::ENoneRomScan;
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::SetAttr
// 
// -----------------------------------------------------------------------------
void CMediaFileHandler::SetAttrL( TInt aAttr, TInt aValue )
    {
    switch ( aAttr )
        {
        case CMediaFileList::EAttrFileSize:
            {
            iMaxFileSize = aValue;
            break;
            }
        case CMediaFileList::EAttrExcludeFolder:
            {
            if ( aValue == CMediaFileList::EFolderVideos )
                {
                iVideoSearchDisabled = ETrue;
                }
            break;
            }
        default:
            {
            break;
            }
        }
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::SetAttrL
// 
// -----------------------------------------------------------------------------
void CMediaFileHandler::SetAttrL( TInt aAttr, const TDesC& aValue )
    {
    if ( aAttr == CMediaFileList::EAttrExcludeMimeType )
        {
        iExcludedMimeTypes->AddMimeTypeL( aValue );
        }
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::SetAttr
// 
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::SetAttrL( TInt /*aAttr*/, TAny* /*aValue*/ )
    {
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::HandleSessionOpened (from MMdESessionObserver)
// 
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::HandleSessionOpened( CMdESession& /*aSession*/, TInt aError )
    {
    if ( aError != KErrNone )
        {
        delete iSession;
        iSession = NULL;
        iSessionOpen = EFalse;
        }
    else
        {
        iSessionOpen = ETrue;

        TRAP_IGNORE( AddObjectObserverL() );
        }

    CallObserver( MMediaFileHandlerObserver::EInitComplete, aError );
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::AddObjectObserverL
// 
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::AddObjectObserverL()
    {
    if ( iSessionOpen )
        {
        TUint32 notificationType = ENotifyAdd | ENotifyModify | ENotifyRemove;
        CMdENamespaceDef& defNS = iSession->GetDefaultNamespaceDefL();
        iSession->AddObjectObserverL( *this, NULL, notificationType, &defNS );
        
        iSession->AddObjectPresentObserverL( *this );
        }
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::HandleSessionError (from MMdESessionObserver)
// 
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::HandleSessionError( CMdESession& /*aSession*/, TInt aError )
    {
    if ( aError == KErrNone )
        {
        return;
        }
        
    delete iSession;
    iSession = NULL;
    iSessionOpen = EFalse;

    CallObserver( MMediaFileHandlerObserver::EError, aError );
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::HandleQueryNewResults (from MMdEQueryObserver)
// 
// This observer function is called during query. Calling frequency is defined
// in second parameter of CMdEQuery::FindL.
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::HandleQueryNewResults( CMdEQuery& /*aQuery*/, 
                                               TInt /*aFirstNewItemIndex*/,
                                               TInt /*aNewItemCount*/ )
    {
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::HandleObjectNotification (MMdEObjectObserver)
// 
// Called when metadata database changes.
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::HandleObjectNotification( CMdESession& /*aSession*/, 
			                 			TObserverNotificationType aType,
						                const RArray<TItemId>& aObjectIdArray )
    {
    const TInt KObserverCallStep = 100;
    
    if ( aType == ENotifyAdd || aType == ENotifyModify || aType == ENotifyRemove )
        {
        if ( iObserver )
            {
            TInt count = aObjectIdArray.Count();
            FLOG( _L("CMediaFileHandler::HandleObjectNotification: count is %d"), count );
            
            iMediaFileCounter = iMediaFileCounter + count;
            if ( iMediaFileCounter >= KObserverCallStep )
                {
                iMediaFileCounter = 0;
                TRAP_IGNORE( iObserver->HandleMFEventL(
                             MMediaFileHandlerObserver::EMediaFileChanged, KErrNone ) );
                }
            else 
                {
                // delayed call to avoid many consecutive observer calls
                CallObserverWithDelay();
                }
            }
        }
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::HandleObjectPresentNotification (MMdEObjectPresentObserver)
// 
// Called when previously used memory card is inserted and hidden
// metadata files are made present again.
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::HandleObjectPresentNotification( CMdESession& /*aSession*/, 
			             TBool /*aPresent*/, const RArray<TItemId>& aObjectIdArray )
    {
    if ( iObserver )
        {
        TInt count = aObjectIdArray.Count();
        FLOG( _L("CMediaFileHandler::HandleObjectPresentNotification: count is %d"), count );

        // delayed call to avoid many consecutive observer calls
        CallObserverWithDelay();
        }
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::HandleQueryCompleted (MMdEQueryObserver)
// 
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::HandleQueryCompleted( CMdEQuery& aQuery, TInt aError )
    {
   
    TInt err = KErrNone;
    TRAP( err, HandleQueryCompletedL( aQuery, aError ) );
    
    if ( err == KErrCancel )
        {
        CallObserver( MMediaFileHandlerObserver::EQueryCanceled, KErrNone );
        }
    else if ( err != KErrNone )
        {
        iSearchList->Reset();
        TRAP_IGNORE( iArtistQuery->ResetL() );

        CallObserver( MMediaFileHandlerObserver::EError, err );
        }
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::HandleQueryCompletedL
// 
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::HandleQueryCompletedL( CMdEQuery& /*aQuery*/, TInt aError )
    {
    TInt err = aError;
    
    if ( !iQuery && ( iRomScanState != MMediaFileHandlerObserver::ERomScanComplete 
    		&& (iQueryType == EQueryTone || iQueryType == ESearchTones )) )
        {
        err = KErrGeneral;
        }
    if ( err == KErrCancel )
        {
        if ( iQueryType == ESearchAudio || iQueryType == ESearchMusic || 
             iQueryType == ESearchTones || iQueryType == ESearchVideo )
            {
            iSearchList->Reset();
            }
        User::Leave( err );
        }
    if ( err != KErrNone )
        {
        User::Leave( err );
        }
    
    
    iSearchList->SetSession( iSession );
    
    TQueryResultMode resultMode = EQueryResultModeItem;
    if ( iQuery )
    	{
        resultMode = iQuery->ResultMode();
    	}

    
    if ( resultMode == EQueryResultModeDistinctValues )
        {
        iSearchList->Reset();
        iSearchList->TakeDistinctResultL( iQuery, KErrNotFound );
        iSearchList->SetSearchId( iQueryId );
        iSearchList->Sort();

        CallObserver( MMediaFileHandlerObserver::EQueryComplete, KErrNone );
        }
    else if ( resultMode == EQueryResultModeCount )
        {
        // EQueryResultModeCount currently not used
        CallObserver( MMediaFileHandlerObserver::EUnknownQueryComplete, KErrNone );
        }
    else if ( resultMode == EQueryResultModeItem )
        {
        if ( iQueryType == ESearchAudio )
            {
            // ESearchAudio currently not used
            iSearchList->Reset();
            iSearchList->TakeResultL( iQuery, EMediaTypeMusic );
            iSearchList->Sort();
            iSearchList->SetSearchId( iQueryId );
            
            CallObserver( MMediaFileHandlerObserver::EQueryComplete, KErrNone );

            /*
            // ESearchAudio currently not used
            iSearchList->Reset();
            iSearchList->TakeResultL( iQuery, EMediaTypeMusic );
            
            delete iIdle;
            iIdle = NULL;
            iIdle = CIdle::NewL( CActive::EPriorityIdle );                
            if ( iIdle )
                {
                iIdle->Start( TCallBack( SearchVideo, this ) );
                }
            */
            }
        else if ( iQueryType == ESearchMusic )
            {
            // search is done in 3 steps (music, tones and videos)
            iSearchList->Reset();
            iSearchList->TakeResultL( iQuery, EMediaTypeMusic );
            
            // call SearchTonesL after short delay (iQuery cannot
            // be deleted in this function)
            delete iIdle;
            iIdle = NULL;
            iIdle = CIdle::NewL( CActive::EPriorityIdle );                
            if ( iIdle )
                {
                iIdle->Start( TCallBack( SearchTones, this ) );
                }
            else
                {
            	  User::Leave( KErrNoMemory );
                }
            }
        else if ( iQueryType == ESearchTones )
            {
            iSearchList->SearchInToneL( iRomFileList, iSearchText );
            iQueryinProgress = EFalse;
            // call SearchVideosL after short delay (iQuery cannot
            // be deleted in this function)
            delete iIdle;
            iIdle = NULL;
            iIdle = CIdle::NewL( CActive::EPriorityIdle );                
            if ( iIdle )
                {
                iIdle->Start( TCallBack( SearchVideo, this ) );
                }
            else
                {
            	  User::Leave( KErrNoMemory );
                }
            }
        else if ( iQueryType == ESearchVideo )
            {
            iSearchList->TakeResultL( iQuery, EMediaTypeVideo );
            iSearchList->VideoSearchInRomL( iRomFileList, iSearchText ); 
            iSearchList->Sort();
            iSearchList->SetSearchId( iQueryId );
            
            CallObserver( MMediaFileHandlerObserver::EQueryComplete, KErrNone );
            }
        
        else if ( iQueryType == EQueryArtist )
            {
            // store CMdEObjectQuery to avoid multiple queries for one artist
            iArtistQuery->SetQuery( iQuery );
            iQuery = NULL;
           
            iSearchList->Reset();
            iSearchList->TakeArtistResultL( iArtistQuery );
            iSearchList->Sort();
            iSearchList->SetSearchId( iQueryId );
            
            CallObserver( MMediaFileHandlerObserver::EQueryComplete, KErrNone );
            }
        else if ( iQueryType == EQueryArtistEmpty )
            {
            // use previously stored CMdEObjectQuery for artist query
            iSearchList->Reset();
            iSearchList->TakeArtistResultL( iArtistQuery );
            iSearchList->Sort();
            iSearchList->SetSearchId( iQueryId );
            
            CallObserver( MMediaFileHandlerObserver::EQueryComplete, KErrNone );
            }
        else if ( iQueryType == EQueryEmpty )
            {
            iSearchList->Reset();
            iSearchList->SetSearchId( iQueryId );
                
            CallObserver( MMediaFileHandlerObserver::EQueryComplete, KErrNone );
            }
        else if ( iQueryType == EQueryUnknown )
            {
            iSearchList->Reset();
            iSearchList->TakeResultL( iQuery, EMediaTypeMusic );
            iSearchList->SetSearchId( iQueryId );     
            
            CallObserver( MMediaFileHandlerObserver::EUnknownQueryComplete, KErrNone );
            }
        else
            {
            TInt mediaType = MediaType( iQueryType );
            iSearchList->Reset();
            if ( mediaType == EMediaTypeTone )
            	{
            	iSearchList->AddRomItemL( iRomFileList, mediaType );
            	}
            else
            	{
            	iSearchList->TakeResultL( iQuery, mediaType );
            	if ( mediaType == EMediaTypeVideo )
            	    {
            	    iSearchList->AddRomItemL( iRomFileList, mediaType );
            	    }
            	}
            iQueryinProgress = EFalse;
            iSearchList->SetSearchId( iQueryId );
            iSearchList->Sort();
        
            CallObserver( MMediaFileHandlerObserver::EQueryComplete, KErrNone );
            }
        }
    else
        {
        CallObserver( MMediaFileHandlerObserver::EError, KErrNotSupported );
        }
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::QueryArtistL
// 
// Finds all music files for one artist. Query results are placed in
// CArtistQuery.
// CArtistQuery can then be used for artist subqueries (all albums, one album, 
// all songs, unknown songs). 
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::QueryArtistL( TInt aAttr, const TDesC& aArtist, 
                                      const TDesC& aAlbum, TBool aRefresh )
    {
    LeaveIfSessionClosedL();

    if ( aAttr != EQueryArtist && aAttr != EQueryArtistAlbum && 
         aAttr != EQueryArtistAll && aAttr != EQueryArtistUnknown )
        {
        User::Leave( KErrNotSupported );
        }

    TBool updateFromDatabase = aRefresh;
    if ( iArtistQuery->Artist().Compare( aArtist ) != 0 )
        {
        updateFromDatabase = ETrue;
        }
    
    if ( updateFromDatabase )
        {
        CMdENamespaceDef& defNS = iSession->GetDefaultNamespaceDefL();
        CMdEObjectDef& objectDef = defNS.GetObjectDefL( MdeConstants::Audio::KAudioObject );
    
        delete iQuery;
        iQuery = NULL;
        iQuery = iSession->NewObjectQueryL( defNS, objectDef, this );
    
        // set attributes that are included in query result  
        CMdEPropertyDef& namePropertyDef = PropertyDefL( EAttrSongName );
        iQuery->AddPropertyFilterL( &namePropertyDef );
        CMdEPropertyDef& albumPropertyDef = PropertyDefL( EAttrAlbum );
        iQuery->AddPropertyFilterL( &albumPropertyDef );

        CMdELogicCondition& conditions = iQuery->Conditions();
        CMdEPropertyDef& propertyDef = PropertyDefL( EAttrArtist );
        conditions.AddPropertyConditionL( propertyDef, 
                            ETextPropertyConditionCompareEquals, aArtist );

        iQuery->SetResultMode( EQueryResultModeItem );
    
        iArtistQuery->SetArtistL( aArtist );
        iArtistQuery->SetAlbumL( aAlbum );
        iQuery->FindL();
        iQueryType = EQueryArtist;
        }
    else
        {
        iArtistQuery->SetAlbumL( aAlbum );
        QueryArtistEmptyL();
        iQueryType = EQueryArtistEmpty;
        }
   
    iArtistQuery->SetQueryType( aAttr );
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::QueryArtistEmptyL
// 
// Makes empty query. Query results were previously placed into CArtistQuery.
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::QueryArtistEmptyL()
    {
    LeaveIfSessionClosedL();

    CMdENamespaceDef& defNS = iSession->GetDefaultNamespaceDefL();
    CMdEObjectDef& objectDef = 
    defNS.GetObjectDefL( MdeConstants::Object::KBaseObject );
    
    delete iQuery;
    iQuery = NULL;
    iQuery = iSession->NewObjectQueryL( defNS, objectDef, this );
    
    iQuery->SetResultMode( EQueryResultModeItem );
 
    iQuery->FindL( 1 );
    
    iQueryType = EQueryArtistEmpty;
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::QueryMusicL
// 
// Finds music files.
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::QueryMusicL( TInt /*aAttr*/ )
    {
    LeaveIfSessionClosedL();
       
    CMdENamespaceDef& defNS = iSession->GetDefaultNamespaceDefL();
    CMdEObjectDef& musicObjectDef =
    defNS.GetObjectDefL( MdeConstants::Audio::KAudioObject );
  
    delete iQuery;
    iQuery = NULL;
    iQuery = iSession->NewObjectQueryL( defNS, musicObjectDef, this );
    
    // set attributes that are included in query result  
    CMdEPropertyDef& namePropertyDef = PropertyDefL( EAttrSongName );
    iQuery->AddPropertyFilterL( &namePropertyDef );
    CMdEPropertyDef& fileTypePropertyDef = PropertyDefL( EAttrMediaType );
    iQuery->AddPropertyFilterL( &fileTypePropertyDef );
    
    
    CMdELogicCondition& conditions = iQuery->Conditions();
    
    // include properties that identify music
    IncludeMusicPropertiesL( conditions );
        
    //
    // example of defining sort order
    //
    // iQuery->AppendOrderRuleL( TMdEOrderRule( namePropertyDef, ESortAscending ) );
   
    iQuery->SetResultMode( EQueryResultModeItem );
   
    iQuery->FindL();
    
    iQueryType = EQueryMusic;
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::QueryMusicL
// 
// Finds music files with attibute that matches aFindText. Supported attributes
// are EAttrArtist, EAttrAlbum, EAttrGenre and EAttrComposer.
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::QueryMusicL( TInt aAttr, const TDesC& aFindText )
    {
    LeaveIfSessionClosedL();
    CheckAttrL( aAttr );
    
    CMdENamespaceDef& defNS = iSession->GetDefaultNamespaceDefL();
    CMdEObjectDef& objectDef =
    defNS.GetObjectDefL( MdeConstants::Audio::KAudioObject );
    
    delete iQuery;
    iQuery = NULL;
    iQuery = iSession->NewObjectQueryL( defNS, objectDef, this );
    
    // set attributes that are included in query result  
    CMdEPropertyDef& namePropertyDef = PropertyDefL( EAttrSongName );
    iQuery->AddPropertyFilterL( &namePropertyDef );
    
    CMdELogicCondition& conditions = iQuery->Conditions();
    CMdEPropertyDef& propertyDef = PropertyDefL( aAttr );
    conditions.AddPropertyConditionL( propertyDef, 
                            ETextPropertyConditionCompareEquals, aFindText );
    IncludeMusicPropertiesL( conditions );
    iQuery->SetResultMode( EQueryResultModeItem );
    
    iQuery->FindL();
    
    iQueryType = EQueryMusic;
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::QueryVideosL
// 
// Finds all video files.
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::QueryVideosL( TInt /*aAttr*/ )
    {
    LeaveIfSessionClosedL();

    CMdENamespaceDef& defNS = iSession->GetDefaultNamespaceDefL();
    CMdEObjectDef& videoObjectDef =
    defNS.GetObjectDefL( MdeConstants::Video::KVideoObject );
   
    delete iQuery;
    iQuery = NULL;
    iQuery = iSession->NewObjectQueryL( defNS, videoObjectDef, this );
    
    // set attributes that are included in query result  
    CMdEPropertyDef& namePropertyDef = PropertyDefL( EAttrSongName );
    iQuery->AddPropertyFilterL( &namePropertyDef );
    
    iQuery->SetResultMode( EQueryResultModeItem );
    
    iQuery->FindL();
    
    iQueryType = EQueryVideo;
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::QueryAttributeValuesL
// 
// Finds all different values for aAttr (all artists, albums, genres,
// or composers).
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::QueryAttributeValuesL( TInt aAttr )
    {
    LeaveIfSessionClosedL();
    CheckAttrL( aAttr );
    
    CMdENamespaceDef& defNS = iSession->GetDefaultNamespaceDefL();
    CMdEObjectDef& objectDef =
    defNS.GetObjectDefL( MdeConstants::Audio::KAudioObject );
 
    delete iQuery;
    iQuery = NULL;
    iQuery = iSession->NewObjectQueryL( defNS, objectDef, this );
    
    iQuery->SetResultMode( EQueryResultModeDistinctValues );
    
    CMdEPropertyDef& propertyDef = PropertyDefL( aAttr );
    
    // distinct query requires exactly one property filter
    // (eg. attribute that is included in query result)
    CMdELogicCondition& conditions = iQuery->Conditions();
    conditions.AddPropertyConditionL( propertyDef );
    iQuery->AddPropertyFilterL( &propertyDef );
    IncludeMusicPropertiesL( conditions );
    iQuery->FindL();
    
    iQueryType = EQueryMusic;
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::QueryMusicUnknownL
// 
// Find music files that do not have attribute aAttr (eg find all files without 
// album attribute).
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::QueryMusicUnknownL( TInt aAttr )
    {
    LeaveIfSessionClosedL();
    CheckAttrL( aAttr );
 
    CMdENamespaceDef& defNS = iSession->GetDefaultNamespaceDefL();
    CMdEObjectDef& objectDef =
    defNS.GetObjectDefL( MdeConstants::Audio::KAudioObject );

    delete iQuery;
    iQuery = NULL;
    iQuery = iSession->NewObjectQueryL( defNS, objectDef, this );
    
    iQuery->SetResultMode( EQueryResultModeItem );
    
    CMdEPropertyDef& propertyDef = PropertyDefL( aAttr );
    CMdEPropertyDef& namePropertyDef = PropertyDefL( EAttrSongName );
    
    
    CMdELogicCondition& conditions = iQuery->Conditions();
    
    // include properties that identify music
    IncludeMusicPropertiesL( conditions );
    
    // add condition: get music that do not have aAttr attribute
    CMdELogicCondition& condition = 
        conditions.AddLogicConditionL( ELogicConditionOperatorAnd );
    condition.AddPropertyConditionL( propertyDef );
    condition.SetNegate( ETrue );
    
    // set attributes that are included in query result  
    iQuery->AddPropertyFilterL( &namePropertyDef );
    
    iQuery->FindL();
    
    iQueryType = EQueryMusic;
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::QueryMusicUnknownCountL
// 
// Find whether there are music files that do not have attribute aAttr
// (eg files without album attribute)
//
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::QueryMusicUnknownCountL( TInt aAttr )
    {
    LeaveIfSessionClosedL();
    CheckAttrL( aAttr );
 
    CMdENamespaceDef& defNS = iSession->GetDefaultNamespaceDefL();
    CMdEObjectDef& objectDef =
    defNS.GetObjectDefL( MdeConstants::Audio::KAudioObject );

    delete iQuery;
    iQuery = NULL;
    iQuery = iSession->NewObjectQueryL( defNS, objectDef, this );
    
    iQuery->SetResultMode( EQueryResultModeItem );
    
    CMdEPropertyDef& propertyDef = PropertyDefL( aAttr );
    
    CMdELogicCondition& conditions = iQuery->Conditions();

    // include properties that identify music
    IncludeMusicPropertiesL( conditions );

    // add condition: get music that do not have aAttr attribute
    CMdELogicCondition& condition = 
        conditions.AddLogicConditionL( ELogicConditionOperatorAnd );
    condition.AddPropertyConditionL( propertyDef );
    condition.SetNegate( ETrue );
      
    
    // count 0 or 1 is enough for UI
    iQuery->FindL( 1 );     
    iQueryType = EQueryUnknown;
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::QueryEmptyL
// 
// Makes empty query.
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::QueryEmptyL()
    {
    LeaveIfSessionClosedL();
    iQueryType = EQueryEmpty;
    if ( iQuery )
    	{
    	iQuery->SetResultMode( EQueryResultModeItem );
    	HandleQueryCompleted( *iQuery , KErrNone);
    	}
    else
    	{
        CMdENamespaceDef& defNS = iSession->GetDefaultNamespaceDefL();
        CMdEObjectDef& objectDef =
        defNS.GetObjectDefL( MdeConstants::Object::KBaseObject );
        
        delete iQuery;
        iQuery = NULL;
        iQuery = iSession->NewObjectQueryL( defNS, objectDef, this );

        iQuery->SetResultMode( EQueryResultModeItem );
        iQuery->FindL( 0 );
        
    	}
    }

// -----------------------------------------------------------------------------
// CMediaFileHandler::QueryRomEmptyL( TInt aQueryType )
// 
// Makes empty query.
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::QueryRomEmptyL( TInt aQueryType )
    {
    LeaveIfSessionClosedL();
    if ( iQuery )
    	{
    	iQueryType = aQueryType;
    	HandleQueryCompleted( *iQuery , KErrNone);
    	}
    else
    	{
        CMdENamespaceDef& defNS = iSession->GetDefaultNamespaceDefL();
        CMdEObjectDef& objectDef =
        defNS.GetObjectDefL( MdeConstants::Object::KBaseObject );
        
        delete iQuery;
        iQuery = NULL;
        iQuery = iSession->NewObjectQueryL( defNS, objectDef, this );

        iQuery->SetResultMode( EQueryResultModeItem );
        iQuery->FindL( 0 );
        
        iQueryType = aQueryType;
    	}
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::QueryRecordingsL
// 
// Finds all recordings.
//
// Note: File is considered a recording if file extension is "amr" or "wav" and
//       file is not in rom.
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::QueryRecordingsL( TInt /*aAttr*/ )
    {
    LeaveIfSessionClosedL();
        
    CMdENamespaceDef& defNS = iSession->GetDefaultNamespaceDefL();
    CMdEObjectDef& musicObjectDef =
    defNS.GetObjectDefL( MdeConstants::Audio::KAudioObject );
  
    delete iQuery;
    iQuery = NULL;
    iQuery = iSession->NewObjectQueryL( defNS, musicObjectDef, this );    
    
    
    // set attributes that are included in query result  
    CMdEPropertyDef& namePropertyDef = PropertyDefL( EAttrSongName );
    iQuery->AddPropertyFilterL( &namePropertyDef );
    
    iQuery->SetResultMode( EQueryResultModeItem );
    
    CMdEPropertyDef& fileTypeDef = PropertyDefL( EAttrMediaType );
    
    
    CMdELogicCondition& conditions = iQuery->Conditions();
    CMdEObjectCondition& cond = conditions.AddObjectConditionL(
                                EObjectConditionCompareUriBeginsWith, KDriveZ );
    cond.SetNegate( ETrue );
    
    
    CMdELogicCondition& condition = 
        conditions.AddLogicConditionL( ELogicConditionOperatorOr );
    condition.AddPropertyConditionL( fileTypeDef, 
            ETextPropertyConditionCompareContains, KMimeTypeAMR );
    condition.AddPropertyConditionL( fileTypeDef, 
            ETextPropertyConditionCompareContains, KMimeTypeWAV );
    
    iQuery->FindL();
    
    iQueryType = EQueryRecording;
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::QueryTonesL
// 
// Finds all tones.
// Note: Audio file is considered a tone if it is not music or video.
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::QueryTonesL( TInt /*aAttr*/ )
    {
    LeaveIfSessionClosedL();
        
    CMdENamespaceDef& defNS = iSession->GetDefaultNamespaceDefL();
    CMdEObjectDef& musicObjectDef =
    defNS.GetObjectDefL( MdeConstants::Audio::KAudioObject );
  
    delete iQuery;
    iQuery = NULL;
    iQuery = iSession->NewObjectQueryL( defNS, musicObjectDef, this );    
    
    
    // set attributes that are included in query result  
    CMdEPropertyDef& namePropertyDef = PropertyDefL( EAttrSongName );
    iQuery->AddPropertyFilterL( &namePropertyDef );
    
    iQuery->SetResultMode( EQueryResultModeItem );
    
    CMdELogicCondition& conditions = iQuery->Conditions();
    ExcludeMusicPropertiesL( conditions );

    iQuery->FindL();
    
    iQueryType = EQueryTone;
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::QueryDefaultTonesL
// 
// Finds all sound files from rom (nokia default tones).
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::QueryDefaultTonesL( TInt /*aAttr*/ )
    {
    
    if ( iRomScanState == MMediaFileHandlerObserver::ERomScanComplete )
    	{
    	QueryRomEmptyL( EQueryTone );
    	}
    else if ( iRomScanState == MMediaFileHandlerObserver::ERomScanInProgress )
    	{
    	iQueryinProgress = ETrue;
    	}
    else
    	{
    	//Rom scan not start or failed for some reason
    	iQueryinProgress = ETrue;
    	StartRomScaning( EQueryTone );
    	}
    }

// -----------------------------------------------------------------------------
// CMediaFileHandler::QueryDefaultTTonessL
// 
// Finds all sound files from rom (nokia default tones).
// -----------------------------------------------------------------------------
//
//void CMediaFileHandler::QueryDefaultTonesL( TInt /*aAttr*/ )
//    {
//    LeaveIfSessionClosedL();
//        
//    CMdENamespaceDef& defNS = iSession->GetDefaultNamespaceDefL();
//    CMdEObjectDef& musicObjectDef =
//    defNS.GetObjectDefL( MdeConstants::Audio::KAudioObject );
//  
//    delete iQuery;
//    iQuery = NULL;
//    iQuery = iSession->NewObjectQueryL( defNS, musicObjectDef, this );    
//    
//    
//    // set attributes that are included in query result  
//    CMdEPropertyDef& namePropertyDef = PropertyDefL( EAttrSongName );
//    iQuery->AddPropertyFilterL( &namePropertyDef );
//    
//    iQuery->SetResultMode( EQueryResultModeItem );
//    
//    CMdELogicCondition& conditions = iQuery->Conditions();
//    CMdEObjectCondition& cond = conditions.AddObjectConditionL(
//                                EObjectConditionCompareUriBeginsWith, KDriveZ );
//
//    iQuery->FindL();
//    
//    iQueryType = EQueryTone;
//    }

// -----------------------------------------------------------------------------
// CMediaFileHandler::SearchL
// 
// Finds all music, tone and video files with any attibute that
// matches aFindText.
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::SearchL( const TDesC& aSearchText )
    {
    //
    // search is done in 3 steps, SearchTonesL and SearchVideoL are
    // called after SearchMusicL is done. Search results are collected
    // in iSearchList.
    //
    
    if ( aSearchText.Length() == 0 )
        {
        iSearchText = KNullDesC;
        QueryEmptyL();
        }
    else
        {
        StrCopy( iSearchText, aSearchText );
        SearchMusicL( iSearchText );
        }
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::SearchAudioL
// 
// Finds all audio files with attibute that matches aFindText.
// Searched attributes are name, artist, album, genre, composer.
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::SearchAudioL( const TDesC& aSearchText )
    {
    SearchMusicL( aSearchText );
    iQueryType = ESearchAudio;
    
    /*
    LeaveIfSessionClosedL();

    CMdENamespaceDef& defNS = iSession->GetDefaultNamespaceDefL();
    CMdEObjectDef& objectDef =
    defNS.GetObjectDefL( MdeConstants::Audio::KAudioObject );

    delete iQuery;
    iQuery = NULL;
    iQuery = iSession->NewObjectQueryL( defNS, objectDef, this );
    
    // set attributes that are included in query result  
    CMdEPropertyDef& song = PropertyDefL( EAttrSongName );
    iQuery->AddPropertyFilterL( &song );
    
    CMdEPropertyDef& artist = PropertyDefL( EAttrArtist );
    iQuery->AddPropertyFilterL( &artist );
    CMdEPropertyDef& album = PropertyDefL( EAttrAlbum );
    iQuery->AddPropertyFilterL( &album );
    CMdEPropertyDef& genre = PropertyDefL( EAttrGenre );
    iQuery->AddPropertyFilterL( &genre );
    CMdEPropertyDef& composer = PropertyDefL( EAttrComposer );
    iQuery->AddPropertyFilterL( &composer );
  
    CMdEPropertyDef& fileTypePropertyDef = PropertyDefL( EAttrMediaType );
    iQuery->AddPropertyFilterL( &fileTypePropertyDef );

    
    CMdELogicCondition& conditions = iQuery->Conditions();
    conditions.SetOperator( ELogicConditionOperatorAnd );
    
    IncludeMusicSearchConditionL( conditions, aSearchText );
    
    

    // iQuery->AppendOrderRuleL( TMdEOrderRule( song, ESortAscending ) );                            
    iQuery->SetResultMode( EQueryResultModeItem );
    iQuery->FindL();
    
    iQueryType = ESearchAudio;
    */
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::SearchMusicL
// 
// Finds all music with attibute that matches aFindText.
// Searched attributes are name, artist, album, genre, composer.
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::SearchMusicL( const TDesC& aSearchText )
    {
    LeaveIfSessionClosedL();

    CMdENamespaceDef& defNS = iSession->GetDefaultNamespaceDefL();
    CMdEObjectDef& objectDef =
    defNS.GetObjectDefL( MdeConstants::Audio::KAudioObject );

    delete iQuery;
    iQuery = NULL;
    iQuery = iSession->NewObjectQueryL( defNS, objectDef, this );
    
    // set attributes that are included in query result  
    CMdEPropertyDef& songName = PropertyDefL( EAttrSongName );
    iQuery->AddPropertyFilterL( &songName );
    CMdEPropertyDef& fileTypePropertyDef = PropertyDefL( EAttrMediaType );
    iQuery->AddPropertyFilterL( &fileTypePropertyDef );

    
    CMdELogicCondition& conditions = iQuery->Conditions();
    conditions.SetOperator( ELogicConditionOperatorAnd );
    
    //IncludeMusicPropertiesL( conditions );
    IncludeMusicSearchConditionL( conditions, aSearchText );

    iQuery->SetResultMode( EQueryResultModeItem );
    iQuery->FindL();
    
    iQueryType = ESearchMusic;
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::SearchTonesL
// 
// Finds all tones with attibute that matches aFindText.
// Searched attribute is file name.
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::SearchTonesL( const TDesC& /*aSearchText */)
    {
    if ( iRomScanState == MMediaFileHandlerObserver::ERomScanComplete )
    	{
    	QueryRomEmptyL( ESearchTones );
    	}
    else if ( iRomScanState == MMediaFileHandlerObserver::ERomScanInProgress )
    	{
    	iQueryinProgress = ETrue;
    	}
    else
    	{
    	//Rom scan not start or failed for some reason
    	iQueryinProgress = ETrue;
    	StartRomScaning( ESearchTones );
    	}
    }

//
//// -----------------------------------------------------------------------------
//// CMediaFileHandler::SearchTonesL
//// 
//// Finds all tones with attibute that matches aFindText.
//// Searched attribute is file name.
//// -----------------------------------------------------------------------------
////
//void CMediaFileHandler::SearchTonesL( const TDesC& aSearchText )
//    {
//    LeaveIfSessionClosedL();
//
//    CMdENamespaceDef& defNS = iSession->GetDefaultNamespaceDefL();
//    CMdEObjectDef& objectDef =
//    defNS.GetObjectDefL( MdeConstants::Audio::KAudioObject );
//
//    delete iQuery;
//    iQuery = NULL;
//    iQuery = iSession->NewObjectQueryL( defNS, objectDef, this );
//    
//    // set attributes that are included in query result  
//    CMdEPropertyDef& songName = PropertyDefL( EAttrSongName );
//    iQuery->AddPropertyFilterL( &songName );
//    CMdEPropertyDef& fileTypePropertyDef = PropertyDefL( EAttrMediaType );
//    iQuery->AddPropertyFilterL( &fileTypePropertyDef );
//
//    
//    CMdELogicCondition& conditions = iQuery->Conditions();
//    conditions.SetOperator( ELogicConditionOperatorAnd );
//    
//    //ExcludeMusicPropertiesL( conditions );
//    CMdEObjectCondition& cond = conditions.AddObjectConditionL(
//                                EObjectConditionCompareUriBeginsWith, KDriveZ );
//    
//    IncludeToneSearchConditionL( conditions, aSearchText );
//
//    iQuery->SetResultMode( EQueryResultModeItem );
//    iQuery->FindL();
//    
//    iQueryType = ESearchTones;
//    }



// -----------------------------------------------------------------------------
// CMediaFileHandler::SearchVideoL
//
// Finds all videos with attibute that matches aFindText.
// Searched attributes are name, artist, genre.
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::SearchVideoL( const TDesC& aSearchText )
    {
    LeaveIfSessionClosedL();
    if ( iRomScanState != MMediaFileHandlerObserver::ERomScanComplete )
        {
        
        StartRomScaning( ESearchVideo );
        }
    
    CMdENamespaceDef& defNS = iSession->GetDefaultNamespaceDefL();
    CMdEObjectDef& objectDef =
    defNS.GetObjectDefL( MdeConstants::Video::KVideoObject );

    delete iQuery;
    iQuery = NULL;
    iQuery = iSession->NewObjectQueryL( defNS, objectDef, this );
    
    // set attributes that are included in query result  
    CMdEPropertyDef& songName = PropertyDefL( EAttrSongName );
    iQuery->AddPropertyFilterL( &songName );
    
    CMdELogicCondition& conditions = iQuery->Conditions();
    IncludeVideoSearchConditionL( conditions, aSearchText );
   
    iQuery->SetResultMode( EQueryResultModeItem );
    
    if ( iVideoSearchDisabled )
        {
        iQuery->FindL( 0 );
        }
    else
        {
        iQuery->FindL();    
        }
    
    iQueryType = ESearchVideo;
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::SearchVideo
//
// -----------------------------------------------------------------------------
//
TInt CMediaFileHandler::SearchVideo( TAny *aObj )
    {    
    CMediaFileHandler* handler = (CMediaFileHandler*) aObj;
    
    TInt err = KErrNone;
    TRAP( err, handler->SearchVideoL( handler->iSearchText ) );
    
    if ( err != KErrNone )
        {
        handler->CallObserver( MMediaFileHandlerObserver::EError, err );
        }
    
    return 0;
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::SearchTones
//
// -----------------------------------------------------------------------------
//
TInt CMediaFileHandler::SearchTones( TAny *aObj )
    {    
    CMediaFileHandler* handler = (CMediaFileHandler*) aObj;
    
    TInt err = KErrNone;
    TRAP( err, handler->SearchTonesL( handler->iSearchText ) );
    
    if ( err != KErrNone )
        {
        handler->CallObserver( MMediaFileHandlerObserver::EError, err );
        }
    
    return 0;
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::ResultCount
// 
// -----------------------------------------------------------------------------
//
TInt CMediaFileHandler::ResultCount()
    {
    if ( !iSessionOpen )
        {
        return 0;
        }
    
    return iSearchList->Count();
    }

    
// -----------------------------------------------------------------------------
// CMediaFileHandler::UnknownArtistCount
// 
// Returns 1 if artist has at least one music file without album attribute,
// 0 otherwise.
// -----------------------------------------------------------------------------
//
TInt CMediaFileHandler::UnknownArtistCount()
    {
    TInt ret = 0;
    
    TInt err = KErrNone;
    TRAP( err, ret = UnknownArtistCountL() );
    if ( err != KErrNone )
        {
        ret = 0;
        }
    return ret;
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::UnknownArtistCountL
// 
// Returns 1 if artist has at least one music file without album attribute,
// 0 otherwise.
// -----------------------------------------------------------------------------
//
TInt CMediaFileHandler::UnknownArtistCountL()
    {
    CMdEObjectQuery* artistQuery = iArtistQuery->Query();
    if ( !artistQuery )
        {
        return 0;
        }

    TInt count = artistQuery->Count();
    for ( TInt i=0; i<count; i++ )
        {
        CMdEObject& object = artistQuery->Result( i );
        
        CMdEPropertyDef& propDef = 
        CMediaFileHandler::PropertyDefL( iSession, CMediaFileHandler::EAttrAlbum );
        
        CMdEProperty* property = NULL;
        TInt ret = object.Property( propDef, property, 0 );
        if ( ret == KErrNotFound )
            {
            return 1;
            }
        }    
    
    return 0;
    }
// -----------------------------------------------------------------------------
// void CMediaFileHandler::StartRomScaning()
// 
// Start rom scanning
// -----------------------------------------------------------------------------
//

void CMediaFileHandler::StartRomScaning( TInt aQueryType )
    {
    
    if ( iRomScanState != MMediaFileHandlerObserver::ERomScanComplete )
        {
        iRomScanState = MMediaFileHandlerObserver::ERomScanInProgress;
        iRomFileList->Reset();
        TRAPD( err , iRomFileList->ReadRomFilesL() );
        if ( err != KErrNone )
            {
            iRomScanState = MMediaFileHandlerObserver::ERomScanError;
            }
        else
            {
            iRomScanState = MMediaFileHandlerObserver::ERomScanComplete;
            }
        }
    
    iSearchList->SetRomFileList( iRomFileList );
    
    if ( iQueryinProgress )
        {
        QueryRomEmptyL( aQueryType );
    	}
    
    CallObserver( MMediaFileHandlerObserver::EScanRomComplete, KErrNone );
    }

// -----------------------------------------------------------------------------
// CMediaFileHandler::CancelQuery
// 
// Cancels ongoing query.
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::CancelQuery()
    {
    if ( !iSessionOpen || !iQuery )
        {
        return;
        }
    
    if ( !iQuery->IsComplete() )
        {
        iQuery->Cancel(); // this calls HandleQueryCompleted synchronously
        }
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::LeaveIfSessionClosedL
// 
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::LeaveIfSessionClosedL()
    {
    if ( !iSession || !iSessionOpen )
        {
        User::Leave( KErrDisconnected );
        }
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::SetObserver
// 
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::SetObserver( MMediaFileHandlerObserver* aObserver )
    {
    iObserver = aObserver;
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::CheckAttrL
// 
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::CheckAttrL( TInt aAttr )
    {
    if ( aAttr != EAttrArtist && aAttr != EAttrAlbum &&
         aAttr != EAttrGenre && aAttr != EAttrComposer )
        {
        User::Leave( KErrNotSupported );
        }
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::GetAttributeL
// 
// NOTE: GetAttributeL only gets full name or file name
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::GetAttributeL( TInt aIndex, TInt aAttr, 
                                       TDes& aValue, TInt aQueryId )
    {
    aValue = KNullDesC;
    
    if ( !iSessionOpen )
         {
         return;
         }
    
    TInt id = iSearchList->SearchId();
    if ( id != aQueryId )
        {
        return; // search list and UI do not match
        }
    
    iSearchList->SetSession( iSession );
    iSearchList->GetAttributeL( aIndex, aAttr, aValue );
    }


    
// -----------------------------------------------------------------------------
// CMediaFileHandler::GetAttribute
// 
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::GetAttribute( TInt aIndex, TInt aAttr,
                                      TDes& aValue, TInt aQueryId  )
    {
    TRAP_IGNORE( GetAttributeL( aIndex, aAttr, aValue, aQueryId ) );
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::Attribute
// 
// -----------------------------------------------------------------------------
//
TInt CMediaFileHandler::Attribute( TInt aIndex, TInt aAttr, TInt aQueryId )
    {
    if ( !iSessionOpen )
         {
         return KErrNotFound;
         }
     
    TInt id = iSearchList->SearchId();
    if ( id != aQueryId )
        {
        return KErrNotFound;
        }
    
    return iSearchList->Attribute( aIndex, aAttr );
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::ItemIndex
// 
// -----------------------------------------------------------------------------
//
TInt CMediaFileHandler::ItemIndex( TInt aItemId, TInt aIndex )
    {
    return iSearchList->ItemIndex( aItemId, aIndex );
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::ItemIndex
// 
// -----------------------------------------------------------------------------
//
TInt CMediaFileHandler::ItemIndex( const TDesC& aItemText, TInt aIndex )
    {
    return iSearchList->ItemIndex( aItemText, aIndex );
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::ReadMediaFileDataL
// 
// -----------------------------------------------------------------------------
//
CMediaFileData* CMediaFileHandler::ReadMediaFileDataL( TInt aId )
    {
    if ( !iSessionOpen )
        {
        User::Leave( KErrNotFound );
        }
    
    CMdEObject* object = iSession->GetFullObjectL( aId );
    CleanupStack::PushL( object );
    
    CMediaFileData* data = CMediaFileData::NewL();
    CleanupStack::PushL( data );
    
    data->SetAttrL( EAttrFullName, object->Uri() );
    
    CMdEProperty* property = NULL;
    TInt ret = object->Property( PropertyDefL( EAttrSongName ), property, 0 );
    if ( ret != KErrNotFound )
        {
        data->SetAttrL( EAttrSongName, property->TextValueL() );
        }
    
    ret = object->Property( PropertyDefL( EAttrArtist ), property, 0 );
    if ( ret != KErrNotFound )
        {
        data->SetAttrL( EAttrArtist, property->TextValueL() );
        }
    
    ret = object->Property( PropertyDefL( EAttrAlbum ), property, 0 );
    if ( ret != KErrNotFound )
        {
        data->SetAttrL( EAttrAlbum, property->TextValueL() );
        }
    
    ret = object->Property( PropertyDefL( EAttrGenre ), property, 0 );
    if ( ret != KErrNotFound )
        {
        data->SetAttrL( EAttrGenre, property->TextValueL() );
        }
    
    ret = object->Property( PropertyDefL( EAttrComposer ), property, 0 );
    if ( ret != KErrNotFound )
        {
        data->SetAttrL( EAttrComposer, property->TextValueL() );
        }

    CleanupStack::Pop( data );
    CleanupStack::PopAndDestroy( object );
    
    return data;
    }

// -----------------------------------------------------------------------------
// CMediaFileHandler::ReadMediaFileDataL
// 
// -----------------------------------------------------------------------------
//
CMediaFileData* CMediaFileHandler::RomMediaFileDataL( TInt aId ,TInt aMediaType )
    {
    if (!iRomFileList)
        {
        User::Leave(KErrNotFound);
        }

    TBuf<KBufSize128> buf;
    CMediaFileData* data = CMediaFileData::NewL();
    CleanupStack::PushL(data);

    if (aMediaType == EMediaTypeTone)
        {
        data->SetAttrL(EAttrFullName, iRomFileList->Item(aId, 0));
        iRomFileList->GetRomFileName(aId, 0, buf);
        data->SetAttrL(EAttrSongName, buf);
        }

    if (aMediaType == EMediaTypeVideo)
        {
        data->SetAttrL(EAttrFullName, iRomFileList->Item(aId, 1));
        iRomFileList->GetRomFileName(aId, 1, buf);
        data->SetAttrL(EAttrSongName, buf);
        }

    CleanupStack::Pop(data);

    return data;
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::PropertyDefL
// 
// -----------------------------------------------------------------------------
//
CMdEPropertyDef& CMediaFileHandler::PropertyDefL( TInt aAttr )
    {
    return PropertyDefL( iSession, aAttr );
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::PropertyDefL
// 
// -----------------------------------------------------------------------------
//
CMdEPropertyDef& CMediaFileHandler::PropertyDefL( CMdESession* aSession, TInt aAttr )
    {
    CMdENamespaceDef& defNS = aSession->GetDefaultNamespaceDefL();
    
    CMdEObjectDef& objectDef =
    defNS.GetObjectDefL( MdeConstants::Audio::KAudioObject );
   
    if ( aAttr == EAttrFileSize )
        {
        return objectDef.GetPropertyDefL( MdeConstants::Object::KSizeProperty );
        }
    else if ( aAttr == EAttrMediaType )
        {
        return objectDef.GetPropertyDefL( MdeConstants::Object::KItemTypeProperty );
        }
    else if ( aAttr == EAttrSongName || aAttr == EAttrFileName )
        {
        return objectDef.GetPropertyDefL( MdeConstants::Object::KTitleProperty );
        }
    else if ( aAttr == EAttrArtist )
        {
        return objectDef.GetPropertyDefL( MdeConstants::MediaObject::KArtistProperty );
        }
    else if ( aAttr == EAttrAlbum )
        {
        return objectDef.GetPropertyDefL( MdeConstants::Audio::KAlbumProperty );
        }
    else if ( aAttr == EAttrGenre )
        {
        return objectDef.GetPropertyDefL( MdeConstants::MediaObject::KGenreProperty );
        }
    else if ( aAttr == EAttrComposer )
        {
        return objectDef.GetPropertyDefL( MdeConstants::Audio::KComposerProperty );
        }
    else
        {
        User::Leave( KErrNotSupported );
        }
    
    return objectDef.GetPropertyDefL( MdeConstants::Object::KTitleProperty );
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::QueryReady
//
// -----------------------------------------------------------------------------
//
TBool CMediaFileHandler::QueryReady() 
    {
    if ( iIdle )
        {
        TBool isSearching = iIdle->IsActive();
        if ( isSearching )
            {
            return EFalse; // we are between audio, tone and and video search
            }
        }
    
    TBool isWaitingObserverCall = iQueryCaller->IsActive();
    if ( isWaitingObserverCall )
        {
        return EFalse; // query is ready but observer not called yet
        }
    
    if ( iQuery )
        {
        return iQuery->IsComplete();    
        }
     
    return ETrue;
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::ExcludeMimeTypesL
// 
// Exclude mime types from query. 
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::ExcludeMimeTypesL( CMdELogicCondition& aCondition, 
                                           TInt /*aAttr*/ )
    {
    TInt count = iExcludedMimeTypes->Count();
    
    if ( count == 0 )
        {
        return;
        }

    CMdEPropertyDef& fileTypeDef = PropertyDefL( EAttrMediaType );
    CMdELogicCondition& condition = 
                        aCondition.AddLogicConditionL( ELogicConditionOperatorAnd );

    for ( TInt i=0; i<count; i++ )
        {
        TPtrC ptr = iExcludedMimeTypes->MimeType( i );
        condition.AddPropertyConditionL( fileTypeDef, 
                             ETextPropertyConditionCompareEquals, ptr );
        }
    condition.SetNegate( ETrue );
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::IncludeMusicPropertiesL
// 
// Adds properties that are used to find out whether media file is music.
// File is considered music if:
// 1. it has metadata artist or album or genre or composer
// 2. its mime type is 'music' mime type (eg mp3)
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::IncludeMusicPropertiesL( CMdELogicCondition& aCondition )
    {
    ExcludeRomFilesL( aCondition );
//    CMdEObjectCondition& cond = aCondition.AddObjectConditionL(
//                                EObjectConditionCompareUriBeginsWith, KDriveZ );
//    cond.SetNegate( ETrue );
    
//    CMdELogicCondition& condition = 
//                        aCondition.AddLogicConditionL( ELogicConditionOperatorAnd );
    ExcludeRecordingsL( aCondition );
//    CMdEPropertyDef& fileTypeDef = PropertyDefL( EAttrMediaType );
//    condition.AddPropertyConditionL( fileTypeDef, 
//            ETextPropertyConditionCompareContains, KMimeTypeAMR );
//    condition.AddPropertyConditionL( fileTypeDef, 
//            ETextPropertyConditionCompareContains, KMimeTypeWAV );
//    condition.SetNegate( ETrue );
    }



// -----------------------------------------------------------------------------
// CMediaFileHandler::IncludeMusicPropertiesL
// 
// Adds properties that are used to find out whether media file is music.
// File is considered music if:
// 1. it has metadata artist or album or genre or composer
// 2. its mime type is 'music' mime type (eg mp3)
// -----------------------------------------------------------------------------
//
/*
void CMediaFileHandler::IncludeMusicPropertiesL( CMdELogicCondition& aCondition )
    {
    //aLogicCondition.SetOperator( ELogicConditionOperatorAnd );
    
    CMdEPropertyDef& mimeTypeDef = PropertyDefL( EAttrMediaType );
    CMdEPropertyDef& artistTypeDef = PropertyDefL( EAttrArtist );
    CMdEPropertyDef& albumTypeDef = PropertyDefL( EAttrAlbum );
    CMdEPropertyDef& genreTypeDef = PropertyDefL( EAttrGenre );
    CMdEPropertyDef& composerTypeDef = PropertyDefL( EAttrComposer );
    
    CMdELogicCondition& condition = 
                        aCondition.AddLogicConditionL( ELogicConditionOperatorOr );

    condition.AddPropertyConditionL( mimeTypeDef, 
            ETextPropertyConditionCompareContains, KMimeMp3 );
    condition.AddPropertyConditionL( artistTypeDef );
    condition.AddPropertyConditionL( albumTypeDef );
    condition.AddPropertyConditionL( genreTypeDef );
    condition.AddPropertyConditionL( composerTypeDef );
    }
*/



// -----------------------------------------------------------------------------
// CMediaFileHandler::ExcludeMusicPropertiesL
// 
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::ExcludeMusicPropertiesL( CMdELogicCondition& aCondition )
    {
    CMdEPropertyDef& mimeTypeDef = PropertyDefL( EAttrMediaType );
    CMdEPropertyDef& artistTypeDef = PropertyDefL( EAttrArtist );
    CMdEPropertyDef& albumTypeDef = PropertyDefL( EAttrAlbum );
    CMdEPropertyDef& genreTypeDef = PropertyDefL( EAttrGenre );
    CMdEPropertyDef& composerTypeDef = PropertyDefL( EAttrComposer );
    
    CMdELogicCondition& condition = 
                        aCondition.AddLogicConditionL( ELogicConditionOperatorAnd );

    condition.AddPropertyConditionL( mimeTypeDef, 
            ETextPropertyConditionCompareContains, KMimeMp3 );
    condition.AddPropertyConditionL( artistTypeDef );
    condition.AddPropertyConditionL( albumTypeDef );
    condition.AddPropertyConditionL( genreTypeDef );
    condition.AddPropertyConditionL( composerTypeDef );
    
    condition.SetNegate( ETrue );
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::IncludeMusicSearchConditionL
// 
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::IncludeMusicSearchConditionL( CMdELogicCondition& aCondition,
                                                      const TDesC& aSearchText )
                                    
    {
    _LIT( KFormat, "%S%S" );
    
    CMdELogicCondition& condition = 
    aCondition.AddLogicConditionL( ELogicConditionOperatorOr );
        
    CMdEPropertyDef& songName = PropertyDefL( EAttrSongName );
    CMdEPropertyDef& artist = PropertyDefL( EAttrArtist );
    CMdEPropertyDef& album = PropertyDefL( EAttrAlbum );    
    CMdEPropertyDef& genre = PropertyDefL( EAttrGenre );
    CMdEPropertyDef& composer = PropertyDefL( EAttrComposer );
    
    // first find from the start of the property
    condition.AddPropertyConditionL( songName,
            ETextPropertyConditionCompareBeginsWith, aSearchText );

    condition.AddPropertyConditionL( artist, 
            ETextPropertyConditionCompareBeginsWith, aSearchText );
 

    condition.AddPropertyConditionL( album, 
            ETextPropertyConditionCompareBeginsWith, aSearchText );

    
    condition.AddPropertyConditionL( genre, 
            ETextPropertyConditionCompareBeginsWith, aSearchText );

    
    condition.AddPropertyConditionL( composer, 
            ETextPropertyConditionCompareBeginsWith, aSearchText );
    
    
    // then find from the start of property words
    TBuf<KBufSize128> buf;
    if ( aSearchText.Length() < KBufSize128-1 )
        {
        buf.Format( KFormat, &KSpace, &aSearchText );
    
        condition.AddPropertyConditionL( songName, 
             ETextPropertyConditionCompareContains, buf );
        condition.AddPropertyConditionL( artist, 
             ETextPropertyConditionCompareContains, buf );
        condition.AddPropertyConditionL( album, 
             ETextPropertyConditionCompareContains, buf );
        condition.AddPropertyConditionL( genre, 
             ETextPropertyConditionCompareContains, buf );
        condition.AddPropertyConditionL( composer, 
             ETextPropertyConditionCompareContains, buf );
        }
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::IncludeVideoConditionL
// 
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::IncludeVideoSearchConditionL(
                  CMdELogicCondition& aCondition, const TDesC& aSearchText )
                                    
    {
    _LIT( KFormat, "%S%S" );
    
    CMdELogicCondition& condition = 
    aCondition.AddLogicConditionL( ELogicConditionOperatorOr );
        
    CMdEPropertyDef& songName = PropertyDefL( EAttrSongName );
    CMdEPropertyDef& artist = PropertyDefL( EAttrArtist );
    CMdEPropertyDef& genre = PropertyDefL( EAttrGenre );
    
    // first find from the start of the property
    condition.AddPropertyConditionL( songName,
            ETextPropertyConditionCompareBeginsWith, aSearchText );
    condition.AddPropertyConditionL( artist, 
            ETextPropertyConditionCompareBeginsWith, aSearchText );
    condition.AddPropertyConditionL( genre, 
            ETextPropertyConditionCompareBeginsWith, aSearchText );
    
    // then find from the start of property words
    TBuf<KBufSize128> buf;
    if ( aSearchText.Length() < KBufSize128-1 )
        {
        buf.Format( KFormat, &KSpace, &aSearchText );
    
        condition.AddPropertyConditionL( songName, 
             ETextPropertyConditionCompareContains, buf );
        condition.AddPropertyConditionL( artist, 
             ETextPropertyConditionCompareContains, buf );
        condition.AddPropertyConditionL( genre, 
             ETextPropertyConditionCompareContains, buf );
        }
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::IncludeToneSearchConditionL
// 
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::IncludeToneSearchConditionL( CMdELogicCondition& aCondition,
                                                     const TDesC& aSearchText )
                                    
    {
    _LIT( KFormat, "%S%S" );
    
    CMdELogicCondition& condition = 
    aCondition.AddLogicConditionL( ELogicConditionOperatorOr );
        
    CMdEPropertyDef& songName = PropertyDefL( EAttrSongName );
    
    // first find from the start of the property
    condition.AddPropertyConditionL( songName,
              ETextPropertyConditionCompareBeginsWith, aSearchText );
    
    // then find from the start of property words
    TBuf<KBufSize128> buf;
    if ( aSearchText.Length() < KBufSize128-1 )
        {
        buf.Format( KFormat, &KSpace, &aSearchText );
    
        condition.AddPropertyConditionL( songName, 
             ETextPropertyConditionCompareContains, buf );
        }
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::ExcludeRecordingsL
// 
// Exclude recording mime types from query (.amr and .wav).
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::ExcludeRecordingsL( CMdELogicCondition& aCondition )
    {
    CMdEPropertyDef& mimeTypeDef = PropertyDefL( EAttrMediaType );
    CMdELogicCondition& condition = 
                        aCondition.AddLogicConditionL( ELogicConditionOperatorAnd );
    condition.AddPropertyConditionL( mimeTypeDef, 
                             ETextPropertyConditionCompareEquals, KMimeTypeAMR );
    condition.AddPropertyConditionL( mimeTypeDef, 
                              ETextPropertyConditionCompareEquals, KMimeTypeWAV );
    condition.SetNegate( ETrue );
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::ExcludeRomFilesL
// 
// Exclude music files that are in rom (default tones).
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::ExcludeRomFilesL( CMdELogicCondition& aCondition )
    {
    _LIT( KDriveZ, "z:\\" );
   
    CMdEObjectCondition& cond = aCondition.AddObjectConditionL(
                                EObjectConditionCompareUriBeginsWith, KDriveZ );
    cond.SetNegate( ETrue );
    }


// -------------------------------------------------------------------------------
// CMediaFileHandler::StrCopy
//
// String copy with lenght check.
// -------------------------------------------------------------------------------
//
void CMediaFileHandler::StrCopy( TDes& aTarget, const TDesC& aSource )
    {
	TInt len = aTarget.MaxLength();
    if( len < aSource.Length() ) 
	    {
		aTarget.Copy( aSource.Left( len ) );
		return;
	    }
	aTarget.Copy( aSource );
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::EnableObserverCall
// 
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::EnableObserverCall( TBool aEnable )
    {
    iObserverCallEnabled = aEnable;
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::CallObserver
// 
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::CallObserver( TInt aEvent, TInt aError )
    {
    const TInt KDelay = 100;  // milliseconds
    
    if ( !iObserverCallEnabled )
        {
        return;    
        }

    iQueryError = aError;
    iQueryEvent = aEvent;
    
    iQueryCaller->Start( 0, KDelay );
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::CallObserverWithDelay
// 
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::CallObserverWithDelay()
    {
    const TInt KOneSecond = 1000;
    
    if ( !iObserverCallEnabled )
        {
        return;    
        }

    iObjectNotificationCaller->Start( 1, 2*KOneSecond );
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::HandleActiveCallL (from MMFActiveCallerObserver)
//
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::HandleActiveCallL( TInt aCallId )
    {
    if ( !iObserver )
        {
        return;
        }
    
    // inform observer that query is ready
    if ( aCallId ==  0 )
        {
        TRAP_IGNORE( iObserver->HandleMFEventL( iQueryEvent, iQueryError ) );
        }

    // inform observer that database has changed
    if ( aCallId == 1 )
        {
        if ( QueryReady() )
            {
            TRAP_IGNORE( iObserver->HandleMFEventL( MMediaFileHandlerObserver::EMediaFileChanged, KErrNone ) );
            }
        }
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::SetQueryId
//
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::SetQueryId( TInt aId ) 
    {
    // this id is passed to CSearchList after successful database search
    iQueryId = aId;
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::Panic
//
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::Panic( TInt aReason ) 
    {
    _LIT( KPanicCategory, "CMediaFileHandler" );
    
    User::Panic( KPanicCategory, aReason ); 
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::MediaType
// 
// -----------------------------------------------------------------------------
//
TInt CMediaFileHandler::MediaType( TInt aQueryType )
    {
    TInt mediaType = EMediaTypeMusic;
    if ( aQueryType == EQueryVideo )
        {
        mediaType = EMediaTypeVideo;
        }
    if ( aQueryType == EQueryRecording )
        {
        mediaType = EMediaTypeRecording;
        }
    if ( aQueryType == EQueryTone )
        {
        mediaType = EMediaTypeTone;
        }

    return mediaType;
    }



/******************************************************************************
 * class CSearchListItem
 ******************************************************************************/


// -----------------------------------------------------------------------------
// CSearchListItem::NewL
// 
// -----------------------------------------------------------------------------
//
CSearchListItem* CSearchListItem::NewLC()
    {
    CSearchListItem* self = new (ELeave) CSearchListItem();
    CleanupStack::PushL( self );
    self->ConstructL();

    return self;
    }


// -----------------------------------------------------------------------------
// CSearchListItem::NewL
// 
// -----------------------------------------------------------------------------
//
CSearchListItem* CSearchListItem::NewL()
    {
    CSearchListItem* self = new (ELeave) CSearchListItem();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// Destructor
// 
// -----------------------------------------------------------------------------
//
CSearchListItem::~CSearchListItem()
    {
    delete iTitle;
    }


// -----------------------------------------------------------------------------
// CSearchListItem::CSearchListItem
// 
// -----------------------------------------------------------------------------
//
CSearchListItem::CSearchListItem()
    {
    iId = KErrNotFound;
    iMediaType = KErrNotFound;
    iStorageType = KErrNotFound;
    iTitle = NULL;
    }


// -----------------------------------------------------------------------------
// CSearchListItem::ConstructL
// 
// -----------------------------------------------------------------------------
//
void CSearchListItem::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CSearchListItem::SetTitleL
// 
// -----------------------------------------------------------------------------
//

void CSearchListItem::SetTitleL( const TDesC& aText, TBool aFullName )
    {
    delete iTitle;
    iTitle = NULL;
    
    if ( aFullName )
        {
        TParsePtrC parse( aText );
        iTitle = parse.Name().AllocL();
        }
    else
        {
        iTitle = aText.AllocL();    
        }
    }


// -----------------------------------------------------------------------------
// CSearchListItem::Title
// 
// -----------------------------------------------------------------------------
//

const TDesC& CSearchListItem::Title() const
    {
    return *iTitle;
    }




/******************************************************************************
 * class CRomFileList
 ******************************************************************************/



// -----------------------------------------------------------------------------
// Static constructor function CRomFileList::NewL
// 
// -----------------------------------------------------------------------------
//
CRomFileList* CRomFileList::NewL()
    {
    CRomFileList* self = new (ELeave) CRomFileList();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }


// -----------------------------------------------------------------------------
// Destructor
// 
// -----------------------------------------------------------------------------
//
CRomFileList::~CRomFileList()
    {
    iRomToneList.ResetAndDestroy();
    iRomVideoList.ResetAndDestroy();
    iList.Reset();
    iList.Close();
    iVedioList.Reset();
    iVedioList.Close();
    }


// -----------------------------------------------------------------------------
// CRomFileList::CRomFileList
// 
// -----------------------------------------------------------------------------
//
CRomFileList::CRomFileList()
    {
    }


// -----------------------------------------------------------------------------
// CRomFileList::ConstructL
// 
// -----------------------------------------------------------------------------
//
void CRomFileList::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CRomFileList::RomFileItem( TInt aIndex )
// 
// -----------------------------------------------------------------------------
//
TInt CRomFileList::RomFileItem( CSearchListItem* aItem, TInt aIndex)
    {
    if( aIndex >= 0 && aIndex < iRomToneList.Count() )
    	{
    	aItem->iId = iRomToneList[aIndex]->iId;
    	aItem->iMediaType = iRomToneList[aIndex]->iMediaType;
    	aItem->iStorageType = iRomToneList[aIndex]->iStorageType;
    	TPtrC title = iRomToneList[aIndex]->Title();
    	aItem->SetTitleL( title, EFalse );
    	return KErrNone;
    	}
    return KErrGeneral;
     }

// -----------------------------------------------------------------------------
// CRomFileList::RomVedioItem( TInt aIndex )
// 
// -----------------------------------------------------------------------------
//
TInt CRomFileList::RomVedioItem( CSearchListItem* aItem, TInt aIndex)
    {
    if( aIndex >= 0 && aIndex < iRomVideoList.Count() )
        {
        aItem->iId = iRomVideoList[aIndex]->iId;
        aItem->iMediaType = iRomVideoList[aIndex]->iMediaType;
        aItem->iStorageType = iRomVideoList[aIndex]->iStorageType;
        TPtrC title = iRomVideoList[aIndex]->Title();
        aItem->SetTitleL( title, EFalse );
        return KErrNone;
        }
    return KErrGeneral;
     }
// -----------------------------------------------------------------------------
// CRomFileList::Count()
// 
// -----------------------------------------------------------------------------
//

TInt CRomFileList::Count(TInt aType )
    {
    if ( aType == 0 )
        {
        //Tone item count
        return iRomToneList.Count();
        }
    else if ( aType == 1 )
        {
        // Video item count()
        return iRomVideoList.Count();
        }
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// CRomFileList::Reset()
// 
// -----------------------------------------------------------------------------
//

void CRomFileList::Reset()
    {
    iRomToneList.Reset();
    iRomVideoList.Reset();
    iList.Reset();
    iVedioList.Reset();
    }
// -----------------------------------------------------------------------------
// CRomFileList::ReadRomFilesL()
// 
// -----------------------------------------------------------------------------
//

void CRomFileList::ReadRomFilesL()
    {
    _LIT( KFormat, "%S%S" );
    
    iRomToneList.ResetAndDestroy();
    iList.Reset();
    
    iFileName.Format( KFormat, &PathInfo::RomRootPath(), &PathInfo::SoundsPath() );
    TRAP_IGNORE( ReadDirectoryL( iFileName, iRomToneList ) );    
    TRAP_IGNORE( ReadDirectoryL( iFileName, iList ) );
    
    iFileName.Format( KFormat, &PathInfo::RomRootPath(), &PathInfo::DigitalSoundsPath() );
    TRAP_IGNORE( ReadDirectoryL( iFileName, iRomToneList ) );    
    TRAP_IGNORE( ReadDirectoryL( iFileName, iList ) );

    iFileName.Format( KFormat, &PathInfo::RomRootPath(), &PathInfo::SimpleSoundsPath() );
    TRAP_IGNORE( ReadDirectoryL( iFileName, iRomToneList ) );    
    TRAP_IGNORE( ReadDirectoryL( iFileName, iList ) );

    TRAP_IGNORE( ReadRomVediosL() );
    }


// -----------------------------------------------------------------------------
// CRomFileList::ReadRomFilesL()
// 
// -----------------------------------------------------------------------------
//

void CRomFileList::ReadRomVediosL()
    {
    _LIT( KFormat, "%S%S" );
    
    iRomVideoList.Reset();
    iVedioList.Reset();
    
    iFileName.Format( KFormat, &PathInfo::RomRootPath(), &PathInfo::VideosPath() );
    TRAP_IGNORE( ReadDirectoryL( iFileName, iRomVideoList ) );    
    TRAP_IGNORE( ReadDirectoryL( iFileName, iVedioList ) );
    
    if ( iRomVideoList.Count() > 0 )
        {
        for( TInt i(0); i < iRomVideoList.Count(); i++)
            {
            iRomVideoList[i]->iMediaType = CMediaFileHandler::EMediaTypeVideo;
            }
        }
    
    }


// -----------------------------------------------------------------------------
// TDesC& CRomFileList::Item( TInt aIndex )
// 
// -----------------------------------------------------------------------------
//
const TDesC& CRomFileList::Item(TInt aIndex, TInt aType)
    {
    TInt count = 0;
    if ( aType == 0 )
        {
        count = iList.Count();
        if (aIndex >= 0 && aIndex < count)
            {
            return iList[aIndex];
            }
        }
    else if ( aType == 1 )
        {
        count = iVedioList.Count();
        if (aIndex >= 0 && aIndex < count)
            {
            return iVedioList[aIndex];
            }
        }
    return KNullDesC;
    }


// -----------------------------------------------------------------------------
// CRomFileList::ReadDirectoryL( const TDesC& aDirectory, 
//                                      RPointerArray<CSearchListItem>& aList )
// 
// -----------------------------------------------------------------------------
//

void CRomFileList::ReadDirectoryL( const TDesC& aDirectory, 
                                   RPointerArray<CSearchListItem>& aList )
    {
    _LIT( KFormat, "%S%S" );
    
    CDir* dir = NULL;
    RFs fsSession;
    User::LeaveIfError( fsSession.Connect() ); 
    CleanupClosePushL( fsSession );
    
    if ( !BaflUtils::FolderExists( fsSession, aDirectory ) )
        {
        User::Leave( KErrNotFound );
        }
    
    TInt err = fsSession.GetDir( aDirectory, KEntryAttNormal, ESortByName, dir );
    User::LeaveIfError( err );
    CleanupStack::PushL( dir );
    TInt index;
    TInt count = dir->Count();
    TFileName filename;
    for ( TInt i=0; i<count; i++ )
        {
        const TEntry item = (*dir)[i];
        filename.Format( KFormat, &aDirectory, &item.iName );
        index = aList.Count();
        CSearchListItem* item2 = SearchListItemFromRomL( filename, index );
        aList.Append( item2 );
        }

    CleanupStack::PopAndDestroy( dir );

    CleanupStack::PopAndDestroy(); // fsSession
    }

// -----------------------------------------------------------------------------
// CRomFileList::ReadDirectoryL( const TDesC& aDirectory, 
//                                      RPointerArray<CSearchListItem>& aList )
// 
// -----------------------------------------------------------------------------
//

void CRomFileList::ReadDirectoryL( const TDesC& aDirectory, 
											RArray<TFileName>& aList )
    {
    _LIT( KFormat, "%S%S" );
    
    CDir* dir = NULL;
    RFs fsSession;
    User::LeaveIfError( fsSession.Connect() ); 
    CleanupClosePushL( fsSession );
    
    if ( !BaflUtils::FolderExists( fsSession, aDirectory ) )
        {
        User::Leave( KErrNotFound );
        }
    
    TInt err = fsSession.GetDir( aDirectory, KEntryAttNormal, ESortByName, dir );
    User::LeaveIfError( err );
    CleanupStack::PushL( dir );

    TInt count = dir->Count();
    TFileName filename;
    for ( TInt i=0; i<count; i++ )
        {
        const TEntry item = (*dir)[i];
        filename.Format( KFormat, &aDirectory, &item.iName );
        aList.Append( filename );
        }

    CleanupStack::PopAndDestroy( dir );

    CleanupStack::PopAndDestroy(); // fsSession
    }
// -----------------------------------------------------------------------------
// CRomFileList::SearchListItemFromRomL( const TDesC& aFileName )
// 
// -----------------------------------------------------------------------------
//

CSearchListItem* CRomFileList::SearchListItemFromRomL( const TDesC& aFileName ,TInt aIndex )
    {
    TParsePtrC parsedName( aFileName );
    
    CSearchListItem* item  = CSearchListItem::NewLC();
    
    item->iId = aIndex;
    item->SetTitleL( aFileName , ETrue);
    item->iStorageType = CMediaFileHandler::ERomStorage;
    item->iMediaType = CMediaFileHandler::EMediaTypeTone;

    CleanupStack::Pop();  // item

    return item;
    }

// -----------------------------------------------------------------------------
// CRomFileList::GetRomFileName
// 
// -----------------------------------------------------------------------------
//
TInt CRomFileList::GetRomFileName(TInt aIndex, TInt aType, TDes& aValue)
	{
	//return title of rom item;
    if (aType == 0)
        {
        if (aIndex >= 0 && aIndex < Count(0))
            {
            aValue.Copy( iRomToneList[aIndex]->Title() );
            return KErrNone;
            }
        return KErrGeneral;
        }
    else if (aType == 1)
        {
        if (aIndex >= 0 && aIndex < Count(1))
            {
            aValue.Copy(iRomVideoList[aIndex]->Title());
            return KErrNone;
            }
        return KErrGeneral;
        }
    return KErrNotFound;
    }


/******************************************************************************
 * class CSearchList
 ******************************************************************************/


// -----------------------------------------------------------------------------
// CSearchList::NewL
// 
// -----------------------------------------------------------------------------
//
CSearchList* CSearchList::NewL()
    {
    CSearchList* self = new (ELeave) CSearchList();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }


// -----------------------------------------------------------------------------
// Destructor
// 
// -----------------------------------------------------------------------------
//
CSearchList::~CSearchList()
    {
    iList.ResetAndDestroy();
    delete iDriveUtil;
    }


// -----------------------------------------------------------------------------
// CSearchList::CSearchList
// 
// -----------------------------------------------------------------------------
//
CSearchList::CSearchList()
    {
    iSearchId = KErrNotFound;
    }


// -----------------------------------------------------------------------------
// CSearchList::ConstructL
// 
// -----------------------------------------------------------------------------
//
void CSearchList::ConstructL()
    {
    iDriveUtil = CDriveUtil::NewL();
    }


// -----------------------------------------------------------------------------
// CSearchList::SetSession
// 
// -----------------------------------------------------------------------------
//
void CSearchList::SetSession( CMdESession* aSession )
    {
    iSession = aSession;
    }

// -----------------------------------------------------------------------------
// CSearchList::SetRomFileList
// 
// -----------------------------------------------------------------------------
//
void CSearchList::SetRomFileList( CRomFileList* aRomFileList )
	{
	iRomFileList = aRomFileList;
	}

// -----------------------------------------------------------------------------
// CSearchList::SetSearchId
// 
// -----------------------------------------------------------------------------
//
void CSearchList::SetSearchId( TInt aSearchId )
    {
    iSearchId = aSearchId;
    }


// -----------------------------------------------------------------------------
// CSearchList::SearchId
// 
// -----------------------------------------------------------------------------
//
TInt CSearchList::SearchId()
    {
    return iSearchId;
    }


// -----------------------------------------------------------------------------
// CSearchList::TakeResultL
// 
// -----------------------------------------------------------------------------
//
void CSearchList::TakeResultL( CMdEObjectQuery* aQuery, TInt aMediaType )
    {
    if ( !aQuery )
        {
        return;
        }
    CMdEObjectQuery* query = static_cast<CMdEObjectQuery*> (aQuery);
    TInt count = query->Count();

    for (TInt i = 0; i < count; i++)
        {
        CMdEObject* object = (CMdEObject*) query->TakeOwnershipOfResult(i);
        CleanupStack::PushL(object);
        AddItemL(*object, aMediaType);
        CleanupStack::PopAndDestroy(object);
        }
    }

// -----------------------------------------------------------------------------
// CSearchList::SearchInToneL
// 
// -----------------------------------------------------------------------------
//
void CSearchList::SearchInToneL( CRomFileList* aQuery,  const TDesC& aSearchText )
    {
    if ( !aQuery )
        {
        return;
        }
    TInt error = KErrNone;
    TInt count = aQuery->Count( 0 );    
    for( int i = 0; i< count; i++ )
    	{
        CSearchListItem* item = CSearchListItem::NewL();
        error = aQuery->RomFileItem( item, i );
        if ( error == KErrNone )
        	{
            TPtrC title = item->Title();
            TInt rt = title.FindF( aSearchText );
            if ( rt != KErrNotFound )
            	{
            	iList.Append( item );
            	}
        	}
    	}
    }

// -----------------------------------------------------------------------------
// CSearchList::VideoSearchInRomL
// 
// -----------------------------------------------------------------------------
//
void CSearchList::VideoSearchInRomL( CRomFileList* aQuery,  const TDesC& aSearchText )
    {
    if ( !aQuery )
        {
        return;
        }
    TInt error = KErrNone;
    TInt count = aQuery->Count( 1 );    
    for( int i = 0; i< count; i++ )
        {
        CSearchListItem* item = CSearchListItem::NewL();
        error = aQuery->RomVedioItem( item, i );
        if ( error == KErrNone )
            {
            TPtrC title = item->Title();
            TInt rt = title.FindF( aSearchText );
            if ( rt != KErrNotFound )
                {
                iList.Append( item );
                }
            }
        }
    }
// -----------------------------------------------------------------------------
// CSearchList::TakeArtistResultL
// 
// -----------------------------------------------------------------------------
//
void CSearchList::TakeArtistResultL( CArtistQuery* aArtistQuery )
    {
    if ( !aArtistQuery )
        {
        return;
        }
    
    CMdEObjectQuery* query = aArtistQuery->Query();
    if ( !query )
        {
        return;
        }
    TInt queryType = aArtistQuery->QueryType();

    
    TInt count = query->Count();
    for ( TInt i=0; i<count; i++ )
        {
        CMdEObject& object = query->Result( i );
        AddArtistItemL( object, queryType, aArtistQuery->Album() );
        }    
    }


// -----------------------------------------------------------------------------
// CSearchList::TakeDistinctResultL
// 
// -----------------------------------------------------------------------------
//
void CSearchList::TakeDistinctResultL( CMdEObjectQuery* aQuery, 
                                       TInt /*aMediaType*/ )
    {
    if ( !aQuery )
        {
        return;
        }

    TInt count = aQuery->Count();
    
    for ( TInt i=0; i<count; i++ )
        {
        TPtrC ptr = aQuery->ResultDistinctValue( i );
        
        CSearchListItem* item = CSearchListItem::NewLC();
         
        item->SetTitleL( ptr, EFalse );
        item->iMediaType = KErrNotFound;
        item->iStorageType = KErrNotFound;

        iList.AppendL( item );
        
        CleanupStack::Pop( item );
        }
    }


// -----------------------------------------------------------------------------
// CSearchList::AddItemL
// 
// -----------------------------------------------------------------------------
//
void CSearchList::AddItemL( CMdEObject& aObject, TInt aMediaType )
    {
    CSearchListItem* item = CSearchListItem::NewLC();
    
    TPtrC songName = TextL( aObject, CMediaFileHandler::EAttrSongName );
    
    if ( songName.Length() > 0 )
        {
        item->SetTitleL( songName, EFalse );
        }
    else
        {
        item->SetTitleL( aObject.Uri(), ETrue );    
        }
    

    item->iId = aObject.Id();
    item->iMediaType = aMediaType;
   
    TInt storageType = CMediaFileHandler::EPhoneMemory;
    if ( iDriveUtil->IsMemoryCard( aObject.Uri() ) )
        {
        storageType = CMediaFileHandler::EMemoryCard;
        }
    else if ( iDriveUtil->IsMassStorage( aObject.Uri() ) )
        {
        storageType = CMediaFileHandler::EMassStorage;
        }
    item->iStorageType = storageType;
    
    iList.AppendL( item );
    
    CleanupStack::Pop( item );
    }

// -----------------------------------------------------------------------------
// CSearchList::AddRomItemL
// 
// -----------------------------------------------------------------------------
//
void CSearchList::AddRomItemL( CRomFileList* aRomList, TInt aMediaType )
    {
    if ( !aRomList )
        {
        return;
        }
    
    TInt count =0;
    TInt error = KErrNone;
    if ( aMediaType == CMediaFileHandler::EMediaTypeTone )
        {
        count = aRomList->Count( 0 );
        for ( TInt i=0; i< count; i++ )
            {
            CSearchListItem* item = CSearchListItem::NewL();
            error = aRomList->RomFileItem( item, i );
            if ( error == KErrNone )
                {
                iList.AppendL( item );
                }
            }
        }
    else if ( aMediaType == CMediaFileHandler::EMediaTypeVideo )
        {
        count = aRomList->Count( 1 );
        for (TInt i = 0; i < count; i++)
            {
            CSearchListItem* item = CSearchListItem::NewL();
            error = aRomList->RomVedioItem(item, i);
            if (error == KErrNone)
                {
                iList.AppendL(item);
                }
            }
        }

    }

// -----------------------------------------------------------------------------
// CSearchList::AddAlbumItemL
// 
// -----------------------------------------------------------------------------
//
void CSearchList::AddAlbumItemL( const TDesC& aAlbum )
    {
    TLinearOrder<CSearchListItem> sortFunction( CSearchList::Compare );
       
    CSearchListItem* item = CSearchListItem::NewLC();
   
    item->SetTitleL( aAlbum, EFalse );
    
    CleanupStack::Pop( item );
    
    item->iMediaType = KErrNotFound;
    item->iStorageType = KErrNotFound;
    item->iId = KErrNotFound;

    TInt err = iList.InsertInOrder( item, sortFunction );
    if ( err != KErrNone )
        {
        delete item;
        }
    if ( err != KErrNone && err != KErrAlreadyExists )
        {
        User::Leave( err );
        }
    }


// -----------------------------------------------------------------------------
// CSearchList::AddArtistItemL
// 
// -----------------------------------------------------------------------------
//
void CSearchList::AddArtistItemL( CMdEObject& aObject, 
                                  TInt aQueryType, const TDesC& aAlbum )
    {
    // all songs for one artist
    if ( aQueryType == CMediaFileHandler::EQueryArtistAll )
        {
        AddItemL( aObject, CMediaFileHandler::EMediaTypeMusic );
        return;
        }

    TPtrC album = TextL( aObject, CMediaFileHandler::EAttrAlbum );

    // unknown songs (no album metadata) for one artist 
    if ( aQueryType == CMediaFileHandler::EQueryArtistUnknown )
        {
        if ( album.Length() == 0 )
            {
            AddItemL( aObject, CMediaFileHandler::EMediaTypeMusic );
            }
        }

    // all albums for one artist
    if ( aQueryType == CMediaFileHandler::EQueryArtist )
        {
        if ( album.Length() > 0 )
            {
            AddAlbumItemL( album );
            }
        }

    // songs in one album
    if ( aQueryType == CMediaFileHandler::EQueryArtistAlbum )
        {
        if ( album.Length() > 0 )
            {
            if ( album.Compare( aAlbum ) == 0 )
                {
                AddItemL( aObject, CMediaFileHandler::EMediaTypeMusic );
                }
            }
        }
    }


// -----------------------------------------------------------------------------
// CSearchList::Sort
// 
// -----------------------------------------------------------------------------
//
void CSearchList::Sort()
    {
    iList.Sort( TLinearOrder<CSearchListItem>(CSearchList::Compare) );
    }


// -----------------------------------------------------------------------------
// CSearchList::Reset
// 
// -----------------------------------------------------------------------------
//
void CSearchList::Reset()
    {
    iSearchId = KErrNotFound;
    iList.ResetAndDestroy();
    }


// -----------------------------------------------------------------------------
// CSearchList::Count
// 
// -----------------------------------------------------------------------------
//
TInt CSearchList::Count()
    {
    return iList.Count();
    }


// -----------------------------------------------------------------------------
// CSearchList::GetAttributeL
// 
// -----------------------------------------------------------------------------
//
void CSearchList::GetAttributeL( TInt aIndex, TInt aAttr, TDes& aValue )
    {
    aValue = KNullDesC;
    
    if ( !iSession )
        {
        return;
        }

    if ( aIndex == KErrNotFound )
        {
        return;
        }

    TInt count = iList.Count();
    if ( aIndex < 0 || aIndex >= count )
        {
        return;
        }
   
    if ( aAttr == CMediaFileHandler::EAttrFullName )
        {
        CSearchListItem* item = iList[aIndex];
        if ( item->iStorageType == CMediaFileHandler::ERomStorage )
            {
            if ( item->iMediaType == CMediaFileHandler::EMediaTypeTone )
                {
                TPtrC name = iRomFileList->Item( item->iId , 0 );
                TMFDialogUtil::StrCopy( aValue, name );
                }
            
            if ( item->iMediaType == CMediaFileHandler::EMediaTypeVideo )
                {
                TPtrC name = iRomFileList->Item( item->iId , 1 );
                TMFDialogUtil::StrCopy( aValue, name );
                }            
            }
        else
        	{
            CMdEObject* object = iSession->GetFullObjectL( item->iId );
            CleanupStack::PushL( object );
            TMFDialogUtil::StrCopy( aValue, object->Uri() );
            CleanupStack::PopAndDestroy( object );
        	}
        }
    
    else if ( aAttr == CMediaFileHandler::EAttrSongName ||
              aAttr == CMediaFileHandler::EAttrFileName )
        {
        CSearchListItem* item = iList[aIndex];
        TMFDialogUtil::StrCopy( aValue, item->Title() );
        if ( aValue.Length() == 0 )
            {
            if ( item->iStorageType == CMediaFileHandler::ERomStorage )
                {
                if ( item->iMediaType == CMediaFileHandler::EMediaTypeTone )
                    {
                    TPtrC name = iRomFileList->Item( aIndex,0 );
                    TMFDialogUtil::StrCopy( aValue, name );
                    }
                
                if ( item->iMediaType == CMediaFileHandler::EMediaTypeVideo )
                    {
                    TPtrC name = iRomFileList->Item( aIndex,1 );
                    TMFDialogUtil::StrCopy( aValue, name );
                    }
                }
            else
            	{
                CMdEObject* object = iSession->GetFullObjectL( item->iId );
                CleanupStack::PushL( object );
                TParsePtrC parse( object->Uri() );
                TMFDialogUtil::StrCopy( aValue, parse.Name() );
                CleanupStack::PopAndDestroy( object );
            	}
            }
        }
    }


// -----------------------------------------------------------------------------
// CSearchList::Attribute
// 
// -----------------------------------------------------------------------------
//
TInt CSearchList::Attribute( TInt aIndex, TInt aAttr )
    {
    if ( aIndex == KErrNotFound )
        {
        return KErrNotFound;
        }

    TInt count = iList.Count();   
    if ( aIndex < 0 || aIndex >= count )
        {
        return KErrNotFound;
        }
    
    
    TInt ret = KErrNotFound;
    if ( aAttr == CMediaFileHandler::EAttrMediaType )
        {
        CSearchListItem* item = iList[aIndex];
        ret = item->iMediaType;
        }
    
    else if ( aAttr == CMediaFileHandler::EAttrStorageType )
        {
        CSearchListItem* item = iList[aIndex];
        ret = item->iStorageType;
        }
    else if ( aAttr == CMediaFileHandler::EAttrMediaFileId )
        {
        CSearchListItem* item = iList[aIndex];
        ret = item->iId;
        }

    return ret;
    }


// -----------------------------------------------------------------------------
// CSearchList::Compare
// 
// -----------------------------------------------------------------------------
//
TInt CSearchList::Compare( const CSearchListItem& aFirst, 
                           const CSearchListItem& aSecond )
    {
    TPtrC first = aFirst.Title();
    TPtrC second = aSecond.Title();
    
    return first.Compare( second );    
    }


// -----------------------------------------------------------------------------
// CSearchList::Panic
//
// -----------------------------------------------------------------------------
//
void CSearchList::Panic( TInt aReason ) 
    {
    TMFDialogUtil::Panic( aReason );
    }


// -----------------------------------------------------------------------------
// CSearchList::TextL
// 
// -----------------------------------------------------------------------------
//
const TDesC& CSearchList::TextL( CMdEObject& aObject, TInt aId )
    {
    CMdEPropertyDef& propDef = 
    CMediaFileHandler::PropertyDefL( iSession, aId );
    
    CMdEProperty* property = NULL;
    TInt err = aObject.Property( propDef, property, 0 );
    
    if ( err != KErrNotFound && property )
        {
        return property->TextValueL();
        }
    
    return KNullDesC;
    }



// -----------------------------------------------------------------------------
// CSearchList::ItemIndex
// 
// -----------------------------------------------------------------------------
//
TInt CSearchList::ItemIndex( TInt aId )
    {
    TInt count = iList.Count();
    
    for ( TInt i=0; i<count; i++ )
        {
        CSearchListItem* item = iList[i];
        if ( item->iId == aId )
            {
            return i;
            }
        }
        
    return KErrNotFound;
    }


// -----------------------------------------------------------------------------
// CSearchList::ItemIndex
// 
// -----------------------------------------------------------------------------
//
TInt CSearchList::ItemIndex( const TDesC& aText )
    {
    TInt count = iList.Count();
    
    for ( TInt i=0; i<count; i++ )
        {
        CSearchListItem* item = iList[i];
        if ( aText.CompareF( item->Title() ) == 0 )
            {
            return i;
            }
        }
    
    

    return KErrNotFound;
    }


// -----------------------------------------------------------------------------
// CSearchList::ItemIndex
// 
// -----------------------------------------------------------------------------
//
TInt CSearchList::ItemIndex( TInt aId, TInt aIndex )
    {
    TInt count = iList.Count();
    if ( aIndex >= 0 && aIndex < count )
        {
        CSearchListItem* item = iList[aIndex];
        if ( item->iId == aId )
            {
            return aIndex;
            }
        }
    
    return ItemIndex( aId );
    }


// -----------------------------------------------------------------------------
// CSearchList::ItemIndex
// 
// -----------------------------------------------------------------------------
//
TInt CSearchList::ItemIndex( const TDesC& aText, TInt aIndex )
    {
    TInt count = iList.Count();
    if ( aIndex >= 0 && aIndex < count )
        {
        CSearchListItem* item = iList[aIndex];
        if ( aText.CompareF( item->Title() ) == 0 )
            {
            return aIndex;
            }
        }

    return ItemIndex( aText );
    }


/******************************************************************************
 * class CArtistQuery
 ******************************************************************************/



// -----------------------------------------------------------------------------
// CArtistQuery::NewL
// 
// -----------------------------------------------------------------------------
//
CArtistQuery* CArtistQuery::NewL()
    {
    CArtistQuery* self = new (ELeave) CArtistQuery();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }


// -----------------------------------------------------------------------------
// Destructor
// 
// -----------------------------------------------------------------------------
//
CArtistQuery::~CArtistQuery()
    {
    delete iArtist;
    delete iAlbum;
    delete iQuery;
    }


// -----------------------------------------------------------------------------
// CArtistQuery::CArtistQuery
// 
// -----------------------------------------------------------------------------
//
CArtistQuery::CArtistQuery()
    {
    iQueryType = KErrNotFound;
    }


// -----------------------------------------------------------------------------
// CArtistQuery::ConstructL
// 
// -----------------------------------------------------------------------------
//
void CArtistQuery::ConstructL()
    {
    iArtist = HBufC::NewL( 0 );
    iAlbum = HBufC::NewL( 0 );
    }

// -----------------------------------------------------------------------------
// CArtistQuery::SetQueryType
// 
// -----------------------------------------------------------------------------
//
void CArtistQuery::SetQueryType( TInt aType )
    {
    iQueryType = aType;
    }


// -----------------------------------------------------------------------------
// CArtistQuery::QueryType
// 
// -----------------------------------------------------------------------------
//
TInt CArtistQuery::QueryType()
    {
    return iQueryType;
    }


// -----------------------------------------------------------------------------
// CArtistQuery::SetQuery
// 
// -----------------------------------------------------------------------------
//
void CArtistQuery::SetQuery( CMdEObjectQuery* aQuery )
    {
    delete iQuery;
    iQuery = NULL;
    iQuery = aQuery;
    }


// -----------------------------------------------------------------------------
// CArtistQuery::Query
// 
// -----------------------------------------------------------------------------
//
CMdEObjectQuery* CArtistQuery::Query()
    {
    return iQuery;
    }


// -----------------------------------------------------------------------------
// CArtistQuery::Artist
// 
// -----------------------------------------------------------------------------
//
const TDesC& CArtistQuery::Artist()
    {
    return *iArtist;
    }


// -----------------------------------------------------------------------------
// CArtistQuery::Album
// 
// -----------------------------------------------------------------------------
//
const TDesC& CArtistQuery::Album()
    {
    return *iAlbum;
    }


// -----------------------------------------------------------------------------
// CArtistQuery::SetAlbumL
// 
// -----------------------------------------------------------------------------
//
void CArtistQuery::SetAlbumL( const TDesC& aText )
    {
    delete iAlbum;
    iAlbum = NULL;
    iAlbum = aText.AllocL();
    }

// -----------------------------------------------------------------------------
// CArtistQuery::SetArtistL
// 
// -----------------------------------------------------------------------------
//
void CArtistQuery::SetArtistL( const TDesC& aText )
    {
    delete iArtist;
    iArtist = NULL;
    iArtist = aText.AllocL();
    }


// -----------------------------------------------------------------------------
// CArtistQuery::ResetL
// 
// -----------------------------------------------------------------------------
//
void CArtistQuery::ResetL()
    {
    iQueryType = KErrNotFound;
    
    delete iArtist;
    iArtist = NULL;

    delete iAlbum;
    iAlbum = NULL;
    
    delete iQuery;
    iQuery = NULL;
    
    iArtist = HBufC::NewL( 0 );
    iAlbum = HBufC::NewL( 0 );
    }


// End of File
