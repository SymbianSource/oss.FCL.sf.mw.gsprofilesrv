/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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
*           Plugin and view id caching support
*
*/


// INCLUDE FILES
#include "GSPluginAndViewIdCache.h"

// System includes
#include <bautils.h>
#include <s32file.h>
#include <eikappui.h>

// User includes
#include "GSShimmedView.h"
#include "GSPlaceholderView.h"

// Constants
_LIT( KGSCacheFileName, "PluginUidToViewIdCache.dat" );


// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CGSPluginAndViewIdCache::CGSPluginAndViewIdCache
// C++ constructor.
//
// ---------------------------------------------------------------------------
//
CGSPluginAndViewIdCache::CGSPluginAndViewIdCache( CEikAppUi& aAppUi )
:   iAppUi( aAppUi ), iLookupTable( &HashFunction, &HashIdentityRelation )
    {
    }

// ---------------------------------------------------------------------------
// CGSPluginAndViewIdCache::~CGSPluginAndViewIdCache
// C++ destructor.
//
// ---------------------------------------------------------------------------
//
CGSPluginAndViewIdCache::~CGSPluginAndViewIdCache()
    {
    TRAP_IGNORE( StoreL() );
    //
    delete iPlaceholderView;
    DestroyShims();
    iLookupTable.Close();
    //
    iFsSession.Close();
    }


// ---------------------------------------------------------------------------
// CGSPluginAndViewIdCache::ConstructL
// 
//
// ---------------------------------------------------------------------------
//
void CGSPluginAndViewIdCache::ConstructL()
    {
    User::LeaveIfError( iFsSession.Connect() );
    User::LeaveIfError( iFsSession.CreatePrivatePath( EDriveC ) );
    //
    iPlaceholderView = CGSPlaceholderView::NewL( iAppUi );
    //
    TRAP_IGNORE( RestoreL() );
    }


// ---------------------------------------------------------------------------
// CGSPluginAndViewIdCache::NewL
// 
//
// ---------------------------------------------------------------------------
//
CGSPluginAndViewIdCache* CGSPluginAndViewIdCache::NewL( CEikAppUi& aAppUi )
    {
    CGSPluginAndViewIdCache* self = new(ELeave) CGSPluginAndViewIdCache( aAppUi );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CGSPluginAndViewIdCache::ActivatePlaceholderViewL
// 
//
// ---------------------------------------------------------------------------
//
void CGSPluginAndViewIdCache::ActivatePlaceholderViewL()
    {
    iPlaceholderView->ActivateL();
    }


// ----------------------------------------------------------------------------
// CGSPluginAndViewIdCache::PriorToPlaceholderActiveViewId
//
// 
// ----------------------------------------------------------------------------
//
const TVwsViewId& CGSPluginAndViewIdCache::PriorToPlaceholderActiveViewId() const
    {
    return iPlaceholderView->PreviouslyActiveViewId();
    }


// ---------------------------------------------------------------------------
// CGSPluginAndViewIdCache::SetPriorToPlaceholderActiveViewId
// 
//
// ---------------------------------------------------------------------------
//
void CGSPluginAndViewIdCache::SetPriorToPlaceholderActiveViewId( const TVwsViewId& aViewId )
    {
    iPlaceholderView->SetPreviouslyActiveViewId( aViewId );
    }


// ---------------------------------------------------------------------------
// CGSPluginAndViewIdCache::RegisterViewAndImplementationAssociationL
// 
//
// ---------------------------------------------------------------------------
//
void CGSPluginAndViewIdCache::RegisterViewAndImplementationAssociationL( TUid aViewId, TUid aImplementationUid )
    {
    RegisterViewAndImplementationAssociationL( aViewId, aImplementationUid, EFalse );
    }


// ---------------------------------------------------------------------------
// CGSPluginAndViewIdCache::PrepareToRegisterPluginInstanceLC
// 
//
// ---------------------------------------------------------------------------
//
void CGSPluginAndViewIdCache::PrepareToRegisterPluginInstanceLC( TUid aViewId )
    {
    TCacheEntry* value = iLookupTable.Find( aViewId );
    if  ( value && value->iShim )
        {
        // Store cleanup view uid so that we know which shim to re-register
        // if the cleanup item is invoked.
        iTransientCleanupViewUid = aViewId;

        // Prepare item
        TCleanupItem item( ReRegisterShimWithUi, this );
        CleanupStack::PushL( item );

        // Unregister shim view
        value->iShim->DeregisterView();

        // If a leave now occurs, the cleanup stack will unwind and will
        // invoke our cleanup item. This item will then re-add
        // the specified shim to the view framework. See ReRegisterShimWithUiL
        // for further details.
        }
    else
        {
        // Record not found - just push NULL to ensure cleanup stack
        // remains balanced.
        CleanupStack::PushL( (CBase*) NULL );
        }
    }


// ---------------------------------------------------------------------------
// CGSPluginAndViewIdCache::RegisterPluginInstanceL
// 
//
// ---------------------------------------------------------------------------
//
void CGSPluginAndViewIdCache::RegisterPluginInstanceL( TUid aViewId, CGSPluginInterface& aPluginInstance )
    {
    TCacheEntry* value = iLookupTable.Find( aViewId );
    ASSERT( value != NULL );
    
    // Can now associate the plugin instance with the cache entry.
    value->iLoadedPluginImplementation = &aPluginInstance;
    }


// ---------------------------------------------------------------------------
// CGSPluginAndViewIdCache::PluginImplementationUidForView
// 
//
// ---------------------------------------------------------------------------
//
TBool CGSPluginAndViewIdCache::IsPluginLoaded( TUid aViewId ) const
    {
    CGSPluginInterface* instance = PluginInstance( aViewId );

    TBool loaded = EFalse;
    if ( instance )
        {
        loaded = ETrue;
        }
    
    return loaded;
    }


// ---------------------------------------------------------------------------
// CGSPluginAndViewIdCache::PluginInstance
// 
//
// ---------------------------------------------------------------------------
//
CGSPluginInterface* CGSPluginAndViewIdCache::PluginInstance( TUid aViewId ) const
    {
    CGSPluginInterface* ret = NULL;
    //
    const TCacheEntry* value = iLookupTable.Find( aViewId );
    if  ( value )
        {
        ret = value->iLoadedPluginImplementation;
        }
    //
    return ret;
    }


// ---------------------------------------------------------------------------
// CGSPluginAndViewIdCache::PluginImplementationUidForView
// 
//
// ---------------------------------------------------------------------------
//
TUid CGSPluginAndViewIdCache::PluginImplementationUidForView( TUid aViewId ) const
    {
    TUid pluginUid = KNullUid;
    //
    const TCacheEntry* value = iLookupTable.Find( aViewId );
    if  ( value )
        {
        pluginUid = value->iImplementationUid;
        }
    //
    return pluginUid;
    }


// ---------------------------------------------------------------------------
// CGSPluginAndViewIdCache::DestroyShims
// 
//
// ---------------------------------------------------------------------------
//
void CGSPluginAndViewIdCache::DestroyShims()
    {
	RHashMap< TUid, TCacheEntry >::TIter iterator( iLookupTable );
    FOREVER
		{
		const TUid* key = iterator.NextKey();
		if  ( key )
            {
            TCacheEntry* value = iterator.CurrentValue();

            // Destroy shim instance
            delete value->iShim;
            value->iShim = NULL;
            }
        else
            {
			break;
            }
		}
    }


// ---------------------------------------------------------------------------
// CGSPluginAndViewIdCache::RestoreL
// 
//
// ---------------------------------------------------------------------------
//
void CGSPluginAndViewIdCache::RestoreL()
    {
    TFileName* cacheFile = new(ELeave) TFileName();
    CleanupStack::PushL( cacheFile );
    GetCacheFileNameL( *cacheFile );
    //
    if  ( BaflUtils::FileExists( iFsSession, *cacheFile ) )
        {
        RFileReadStream cacheStream;
        TInt error = cacheStream.Open( iFsSession, *cacheFile, EFileRead );
        User::LeaveIfError( error );
        CleanupClosePushL( cacheStream );

        // Now read in cache hash table
        TUid key = KNullUid;
        TUid value = KNullUid;
        
        const TInt entryCount = cacheStream.ReadInt32L();
        for( TInt i=0; i<entryCount; i++ )
            {
            key.iUid = cacheStream.ReadUint32L();
            value.iUid = cacheStream.ReadUint32L();
            //
            RegisterViewAndImplementationAssociationL( key, value, ETrue );
            }

        CleanupStack::PopAndDestroy( &cacheStream );
        }
    //
    CleanupStack::PopAndDestroy( cacheFile );
    }


// ---------------------------------------------------------------------------
// CGSPluginAndViewIdCache::StoreL
// 
//
// ---------------------------------------------------------------------------
//
void CGSPluginAndViewIdCache::StoreL()
    {
    TFileName* intermediateFileName = new(ELeave) TFileName();
    CleanupStack::PushL( intermediateFileName );

    TFileName* cachePath = new(ELeave) TFileName();
    CleanupStack::PushL( cachePath );
    GetCachePathL( *cachePath );
    
    // Create a temporary file in the GS private data cage to contain
    // the cache data. We'll rename this to the real cache file name
    // once the operation has been completed successfully.
    RFile intermediateCacheFile;
    TInt error = intermediateCacheFile.Temp( iFsSession, *cachePath, *intermediateFileName, EFileWrite );
    User::LeaveIfError( error );
  
    // Create a stream from the file. Stream takes ownership of file
    RFileWriteStream stream( intermediateCacheFile );
    CleanupClosePushL( stream );

    // Write each entry to the stream
    stream.WriteUint32L( iLookupTable.Count() );

	RHashMap< TUid, TCacheEntry >::TIter iterator( iLookupTable );
    FOREVER
		{
		const TUid* key = iterator.NextKey();
		if  ( key )
            {
            const TCacheEntry* value = iterator.CurrentValue();
            stream.WriteUint32L( key->iUid );
            stream.WriteUint32L( value->iImplementationUid.iUid );
            }
        else
            {
			break;
            }
		}
	
    // Finalise the stream
    stream.CommitL();
    stream.Close(); // Also closes the file
    CleanupStack::PopAndDestroy( &stream ); 

    // If we got this far it means we created the cache file successfully.
    // Now its okay to replace any pre-existing cache file with the
    // file we just created. We're re-use the cachePath variable for the
    // ultimate filename since it saves allocating another 1/2kb on the heap.
    GetCacheFileNameL( *cachePath );
    if  ( BaflUtils::FileExists( iFsSession, *cachePath ) )
        {
        error = iFsSession.Delete( *cachePath );
        User::LeaveIfError( error );
        }

    // Now rename intermediate file name to ultimate cache file name.
    error = iFsSession.Rename( *intermediateFileName, *cachePath );
    User::LeaveIfError( error );

    CleanupStack::PopAndDestroy( 2, intermediateFileName );
    }


// ---------------------------------------------------------------------------
// CGSPluginAndViewIdCache::GetCacheFileNameL
// 
//
// ---------------------------------------------------------------------------
//
void CGSPluginAndViewIdCache::GetCacheFileNameL( TDes& aFileName )
    {
    GetCachePathL( aFileName );
    aFileName.Append( KGSCacheFileName );
    }


// ---------------------------------------------------------------------------
// CGSPluginAndViewIdCache::GetCachePathL
// 
//
// ---------------------------------------------------------------------------
//
void CGSPluginAndViewIdCache::GetCachePathL( TDes& aPath )
    {
    aPath.Zero();

    // Get private data cage path
    TInt err = iFsSession.PrivatePath( aPath );
    User::LeaveIfError( err );

    // Combine with C: drive
    const TDriveUnit cDrive( EDriveC );
    const TDriveName cDriveName( cDrive.Name() );
    aPath.Insert( 0, cDriveName );
    }


// ---------------------------------------------------------------------------
// CGSPluginAndViewIdCache::RegisterViewAndImplementationAssociationL
// 
//
// ---------------------------------------------------------------------------
//
void CGSPluginAndViewIdCache::RegisterViewAndImplementationAssociationL( TUid aViewId, TUid aImplementationUid, TBool aCreateShim )
    {
    // Check if aViewId is a key within the look up table, so as to avoid
    // adding duplicate entries...
    const TCacheEntry* value = iLookupTable.Find( aViewId );
    if  ( !value )
        {
        // No registration for this view yet, so create a new entry
        TCacheEntry cacheEntry;
        cacheEntry.iImplementationUid = aImplementationUid;
        cacheEntry.iLoadedPluginImplementation = NULL;

        if  ( aCreateShim )
            {
            // Create a new shim
            cacheEntry.iShim = CGSShimmedView::NewLC( aViewId, *this, iAppUi );
            }
        else
            {
            cacheEntry.iShim = NULL;
            }

        // Add entry to table - the shim ownership is also transferred 
        // at this point, since the destructor of this class will
        // destroy all shims.
        iLookupTable.InsertL( aViewId, cacheEntry );
        
        if  ( aCreateShim )
            {
            CleanupStack::Pop( cacheEntry.iShim );
            }
        }
    }


// ---------------------------------------------------------------------------
// CGSPluginAndViewIdCache::PrepareForUIDestruction
// 
//
// ---------------------------------------------------------------------------
//
void CGSPluginAndViewIdCache::PrepareForUIDestruction()
    {
    // Have to remove all UI furniture before the AppUi rug is pulled out
    // from underneath us...
    DestroyShims();
    //
    delete iPlaceholderView;
    iPlaceholderView = NULL;
    }


// ---------------------------------------------------------------------------
// CGSPluginAndViewIdCache::HandleShimDestruction
// 
//
// ---------------------------------------------------------------------------
//
void CGSPluginAndViewIdCache::HandleShimDestruction( TUid aViewId )
    {
    TCacheEntry* value = iLookupTable.Find( aViewId );
    if  ( value && value->iShim )
        {
        // The shim has been destroyed.
        value->iShim = NULL;
        }
    }


// ---------------------------------------------------------------------------
// CGSPluginAndViewIdCache::ReRegisterShimWithUi
// 
//
// ---------------------------------------------------------------------------
//
void CGSPluginAndViewIdCache::ReRegisterShimWithUi( TAny* aSelf )
    {
    CGSPluginAndViewIdCache* self = reinterpret_cast< CGSPluginAndViewIdCache* >( aSelf );
    //
    if  ( self->iTransientCleanupViewUid != KNullUid )
        {
        TCacheEntry* value = self->iLookupTable.Find( self->iTransientCleanupViewUid );
        if  ( value && value->iShim )
            {
            TRAP_IGNORE( value->iShim->RegisterViewL() );
            }
        }
    //
    self->iTransientCleanupViewUid = KNullUid;
    }


// ---------------------------------------------------------------------------
// CGSPluginAndViewIdCache::HashFunction
// 
//
// ---------------------------------------------------------------------------
//
TUint32 CGSPluginAndViewIdCache::HashFunction( const TUid& aViewId )
    {
    const TUint32 hash = DefaultHash::Integer( aViewId.iUid );
    return hash;
    }


// ---------------------------------------------------------------------------
// CGSPluginAndViewIdCache::HashIdentityRelation
// 
//
// ---------------------------------------------------------------------------
//
TBool CGSPluginAndViewIdCache::HashIdentityRelation( const TUid& aLeft, const TUid& aRight )
    {
    const TBool equal = ( aLeft.iUid == aRight.iUid );
    return equal;
    }

