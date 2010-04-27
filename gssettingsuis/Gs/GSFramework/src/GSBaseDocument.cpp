/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Base Document for the General Settings Application
*
*/

#include "GSBaseDocument.h"

// User includes
#include "GSPluginWrapper.h"
#include "GSPluginAndViewIdCache.h"
#include "GSWatchDog.h"
#include "gsplugininterface.h"
#include "GsLogger.h"

// Constants
const TInt KGSPluginWrapperObjectGranularity = 10;


// ================= MEMBER FUNCTIONS =======================

// ----------------------------------------------------
// CGSBaseDocument::CGSBaseDocument()
// constructor
//
// ----------------------------------------------------
//
EXPORT_C CGSBaseDocument::CGSBaseDocument( CEikApplication& aApp )
:   CAknDocument( aApp ),
    iUnloadWrapperObjects( KGSPluginWrapperObjectGranularity ),
    iWatchDog ( NULL )
    {
    }


// ----------------------------------------------------
// CGSBaseDocument::~CGSBaseDocument()
// destructor
//
// ----------------------------------------------------
//
EXPORT_C CGSBaseDocument::~CGSBaseDocument()
    {
    // This physically unloads the plugin dlls if their ECOM
    // reference count is zero. This must be the last thing to
    // occur to any plugin object (i.e. all of its code must have
    // finished running by the time this next line takes place).
    const TInt count = iUnloadWrapperObjects.Count();
    for( TInt i=count-1; i>=0; i-- )
        {
        CGSPluginWrapper* object = iUnloadWrapperObjects[ i ];
        (void) object;
        delete object;
        }

    iUnloadWrapperObjects.Close();
    delete iPluginViewIdCache;
#ifdef GS_ENABLE_WATCH_DOG
    delete iWatchDog;
#endif

    iImplInfoArray.ResetAndDestroy();// This is needed
    iImplInfoArray.Close();
    }


// ----------------------------------------------------
// CGSBaseDocument::ConstructL()
// Symbian OS second phase constructor
//
// ----------------------------------------------------
//
EXPORT_C void CGSBaseDocument::ConstructL()
    {
#ifdef GS_ENABLE_WATCH_DOG
    iWatchDog = CGSWatchDog::NewL();
#endif

    REComSession::ListImplementationsL(
        KGSPluginInterfaceUid,
        iImplInfoArray );
#ifdef _DEBUG
    PrintImplInfoArray( iImplInfoArray );
#endif //_DEBUG
        
    __GSLOGSTRING1(
        "[CGSBaseDocument::ConstructL] iImplInfoArray count:%d",
        iImplInfoArray.Count() );
    }


// ----------------------------------------------------
// CGSBaseDocument::NewPluginUnloadWrapperLC()
//
//
// ----------------------------------------------------
//
EXPORT_C CGSPluginWrapper* CGSBaseDocument::NewPluginUnloadWrapperLC()
    {
    CGSPluginWrapper* wrapper = new(ELeave) CGSPluginWrapper( *this );
    TCleanupItem wrapperCleanupItem( CleanupPluginWrapperObject, wrapper );
    CleanupStack::PushL( wrapperCleanupItem );
    iUnloadWrapperObjects.AppendL( wrapper );
    //
    return wrapper;
    }


// ----------------------------------------------------
// CGSBaseDocument::PreparePluginCacheL()
//
//
// ----------------------------------------------------
//
EXPORT_C void CGSBaseDocument::PreparePluginCacheL( CEikAppUi& aAppUi )
    {
    iPluginViewIdCache = CGSPluginAndViewIdCache::NewL( aAppUi );
    }


// ----------------------------------------------------
// CGSBaseDocument::PrepareForUIDestruction()
//
//
// ----------------------------------------------------
//
EXPORT_C void CGSBaseDocument::PrepareForUIDestruction()
    {
    if  ( iPluginViewIdCache )
        {
        iPluginViewIdCache->PrepareForUIDestruction();
        }
    }


// ----------------------------------------------------
// CGSBaseDocument::CleanupPluginWrapperObject()
// Cleanup support
//
// ----------------------------------------------------
//
void CGSBaseDocument::CleanupPluginWrapperObject( TAny* aWrapper )
    {
    CGSPluginWrapper* wrapper =
        reinterpret_cast< CGSPluginWrapper* >( aWrapper );
    CGSBaseDocument& document = wrapper->Document();
    document.DoCleanupPluginWrapper( wrapper );
    }


// ----------------------------------------------------
// CGSBaseDocument::DoCleanupPluginWrapper()
// Cleanup support - called when loading a plugin fails
//
// ----------------------------------------------------
//
void CGSBaseDocument::DoCleanupPluginWrapper( CGSPluginWrapper* aWrapper )
    {
    const TInt index = iUnloadWrapperObjects.Find( aWrapper );
    if  ( index >= 0 && index < iUnloadWrapperObjects.Count() )
        {
        iUnloadWrapperObjects.Remove( index );
        delete aWrapper;
        }
    }


// ----------------------------------------------------
// CGSBaseDocument::PluginViewIdCache()
//
//
// ----------------------------------------------------
//
CGSPluginAndViewIdCache& CGSBaseDocument::PluginViewIdCache()
    {
    ASSERT( iPluginViewIdCache );
    return *iPluginViewIdCache;
    }


// ----------------------------------------------------
// CGSBaseDocument::PluginViewIdCache()
//
//
// ----------------------------------------------------
//
const CGSPluginAndViewIdCache& CGSBaseDocument::PluginViewIdCache() const
    {
    ASSERT( iPluginViewIdCache );
    return *iPluginViewIdCache;
    }



// ----------------------------------------------------
// CGSBaseDocument::WatchDog()
//
//
// ----------------------------------------------------
//
EXPORT_C MGSWatchDog* CGSBaseDocument::WatchDog()
    {
    return iWatchDog;
    }

// ----------------------------------------------------
// CGSBaseDocument::GetImplInfos()
//
//
// ----------------------------------------------------
//
EXPORT_C RImplInfoPtrArray CGSBaseDocument::GetImplInfo()
    {
    return iImplInfoArray;
    }

EXPORT_C void CGSBaseDocument::CreateShimmedViewL(const TUid aViewUid)
	{
	ASSERT(iPluginViewIdCache);
	if ( !iPluginViewIdCache->PluginInstance(aViewUid) )
		{
		iPluginViewIdCache->RegisterViewAndImplementationAssociationL(aViewUid, aViewUid, ETrue);
		}
	}

void CGSBaseDocument::PrintImplInfoArray( const RImplInfoPtrArray& aImplInfoArray )
    {
    for( TInt i = 0; i < aImplInfoArray.Count(); i++ )
        {
        PrintImplInfo( *aImplInfoArray[ i ] );
        }
    }


void CGSBaseDocument::PrintImplInfo( const CImplementationInformation& aInfo )
    {
    __GSLOGSTRING1( "[CGSBaseDocument] --Plugin 0x%X info--", &aInfo);
    __GSLOGSTRING1( "[CGSBaseDocument] DisplayName:%S", &aInfo.DisplayName() );
    __GSLOGSTRING1( "[CGSBaseDocument] ImplementationUid:0x%X",
                    aInfo.ImplementationUid() );

    const TInt KMaxEComDataLength = 256;

    HBufC* dataType = HBufC::New( KMaxEComDataLength );
    HBufC* opaqueData = HBufC::New( KMaxEComDataLength );

    dataType->Des().Copy( aInfo.DataType() );
    opaqueData->Des().Copy( aInfo.OpaqueData() );
    __GSLOGSTRING1( "[CGSBaseDocument] DataType:%S", dataType );
    __GSLOGSTRING1( "[CGSBaseDocument] OpaqueData:%S", opaqueData );

    delete opaqueData;
    delete dataType;
    }



// End of File
