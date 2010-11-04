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
* Description:   Variation functionality class
*
*/



// INCLUDE FILES

#include "mediafilevariation.h"

#include <centralrepository.h>
#include <apmstd.h> // for KMaxDataTypeLength
#include <ProfileEngineDomainCRKeys.h>



/******************************************************************************
 * class CMediaFileDialogVariation
 ******************************************************************************/


// -----------------------------------------------------------------------------
// CMediaFileDialogVariation::NewL
//
// -----------------------------------------------------------------------------
//
CMediaFileDialogVariation* CMediaFileDialogVariation::NewL()
    {
    CMediaFileDialogVariation* self = new ( ELeave ) CMediaFileDialogVariation( 0 );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self);
    return self;
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogVariation::~CMediaFileDialogVariation
//
// -----------------------------------------------------------------------------
//
CMediaFileDialogVariation::~CMediaFileDialogVariation()
    {
    iBlockedTones.Close();
    iBlockedUnprotectedTones.Close();
    iBlockedProtectedTones.Close();
    iBlockedDemoPlayTones.Close();
    iExcludedTones.Close();
    iMimeBuf.Close();
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogVariation::CMediaFileDialogVariation
//
// -----------------------------------------------------------------------------
//
CMediaFileDialogVariation::CMediaFileDialogVariation( TInt aFeatureBitMask )
    : iFeatureBitMask( aFeatureBitMask )
    {
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogVariation::ConstructL
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialogVariation::ConstructL()
    {
    _LIT( KSpace, " " );
    
    CRepository* cenrep = CRepository::NewL( KCRUidProfilesLV );
    CleanupStack::PushL( cenrep );
    
    User::LeaveIfError( cenrep->Get( KProfilesLVFlags, iFeatureBitMask ) );
    ReadListKeyL( *cenrep, KProEngBlockedTones, iBlockedTones );
    ReadListKeyL( *cenrep, KProEngBlockedUnprotectedTones,
                  iBlockedUnprotectedTones );
    ReadListKeyL( *cenrep, KProEngBlockedProtectedTones,
                  iBlockedProtectedTones );
    ReadListKeyL( *cenrep, KProEngBlockedDemoPlayTones,
                  iBlockedDemoPlayTones );
    ReadListKeyL( *cenrep, KProEngExcludedTones,
                  iExcludedTones );
    
    CleanupStack::PopAndDestroy( cenrep );
    
    iMimeBuf.CreateL( KMaxDataTypeLength + KSpace().Length() );
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogVariation::IsFeatureSupported
//
// -----------------------------------------------------------------------------
//
TBool CMediaFileDialogVariation::IsFeatureSupported( TInt aLocalFeatureId )
    {
    return iFeatureBitMask & aLocalFeatureId;
    }


// -----------------------------------------------------------------------------
// CProfilesLocalFeatures::IsBlockedType
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CMediaFileDialogVariation::IsBlockedType( const TDesC& aMimeType )
    {
    return ContainsMimeType( iBlockedTones, aMimeType );
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogVariation::IsBlockedUnprotectedType
//
// -----------------------------------------------------------------------------
//
TBool CMediaFileDialogVariation::IsBlockedUnprotectedType(
        const TDesC& aMimeType )
    {
    return ContainsMimeType( iBlockedUnprotectedTones, aMimeType );
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogVariation::IsBlockedProtectedType
//
// -----------------------------------------------------------------------------
//
TBool CMediaFileDialogVariation::IsBlockedProtectedType( const TDesC& aMimeType )
    {
    return ContainsMimeType( iBlockedProtectedTones, aMimeType );
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogVariation::IsBlockedDemoPlayType
//
// -----------------------------------------------------------------------------
//
TBool CMediaFileDialogVariation::IsBlockedDemoPlayType( const TDesC& aMimeType )
    {
    return ContainsMimeType( iBlockedDemoPlayTones, aMimeType );
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogVariation::IsExcludedType
//
// -----------------------------------------------------------------------------
//
TBool CMediaFileDialogVariation::IsExcludedType( const TDesC& aMimeType )
    {
    return ContainsMimeType( iExcludedTones, aMimeType );
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogVariation::ReadListKeyL
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialogVariation::ReadListKeyL( CRepository& aCenRep,
                                              TUint32 aKey, RBuf& aValue )
    {
    _LIT( KSpace, " " );
    
    TBuf<1> tmp; // one char to get length of actual value
    TInt realLen = 0;
    TInt err = aCenRep.Get( aKey, tmp, realLen );
    if ( err == KErrOverflow )
        {
        // prepare list of blocked MIME types
        aValue.CreateL( realLen + KSpace().Length() );
        User::LeaveIfError( aCenRep.Get( aKey, aValue ) );
        aValue.Append( KSpace );
        }
    else
        {
        User::LeaveIfError( err );
        }
    }


// -----------------------------------------------------------------------------
// CMediaFileDialogVariation::ContainsMimeTypeL
//
// -----------------------------------------------------------------------------
//
TBool CMediaFileDialogVariation::ContainsMimeType( const TDesC& aList,
                                                   const TDesC& aMimeType )
    {
    _LIT( KSpace, " " );
    
    if( aList.Length() == 0 )
        {
        return EFalse;
        }

    iMimeBuf.Copy( aMimeType );
    // FindF() would find "audio/3gpp" in "audio/3gpp2" without
    // the added space.
    iMimeBuf.Append( KSpace );
    
    if ( aList.FindF( iMimeBuf ) == KErrNotFound )
        {
        // aMimeType was not in blocked list
        return EFalse;
        }
    
    return ETrue;
    }



//  End of File  
