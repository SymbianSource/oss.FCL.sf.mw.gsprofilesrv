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
* Description: Implementation of the CFLDDRMImplementation.
*
*/



// CLASS HEADER
#include    "CFLDDRMImplementation.h"

// INTERNAL INCLUDES
#include    "CFLDRingingTonePlayer.h" // KFLDResourceFileName

// EXTERNAL INCLUDES
#include    <DRMCommon.h>
#include    <DRMHelper.h>
#include    <FileList.rsg>
#include    <MProfilesLocalFeatures.h>
#include    <MProfileUtilitySingleton.h>

#ifdef RD_DRM_COMMON_INTERFACE_FOR_OMA_AND_WMDRM
#include	<drmutility.h>
#include	<drmagents.h>
#endif

// CONSTANTS
namespace
	{
	_LIT( KFLDROMDriveLetter, "Z:" );
	}

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CFLDDRMImplementation::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CFLDDRMImplementation::ConstructL()
    {
    iProfilesFeatures = &( ProfileUtilityInstanceL().ProfilesLocalFeatures() );
	iDRMCommon = DRMCommon::NewL();
	User::LeaveIfError( iDRMCommon->Connect() );
    iDRMHelper = CDRMHelper::NewL( *CCoeEnv::Static() );
    }

// Destructor
CFLDDRMImplementation::~CFLDDRMImplementation()
    {
   	delete iDRMHelper;
	
    if( iDRMCommon )
    	{
		iDRMCommon->Disconnect(); // ignore possible error
		delete iDRMCommon;
		}
    ReleaseProfileUtility();
    }

// -----------------------------------------------------------------------------
// CFLDDRMImplementation::SetAutomatedType
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDDRMImplementation::SetAutomatedType(
 CDRMHelper::TDRMHelperAutomatedType aAutomatedType )
	{
	iAutomatedType = aAutomatedType;
	}

// -----------------------------------------------------------------------------
// CFLDDRMImplementation::IsFileValidL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CFLDDRMImplementation::IsFileValidL(
    const TDesC& aFileName, TIntention aIntention )
	{
    // If the destructor is empty, return that it's valid
    if( aFileName.Compare( KNullDesC ) == 0 )
        {
        return ETrue;
        }

 	// Tone files on ROM are always valid
   	if( ( aFileName.Left( KFLDROMDriveLetter().Length() ).CompareF(
          KFLDROMDriveLetter ) == 0 ) )
        {
        return ETrue;
        }

	// Check if file is WMDRM protected
   	TBool prot( EFalse );
   	TRAPD( err, prot = IsFileWMDRMProtectedL( aFileName ) );
   	if( err != KErrNone )
   		{
   		ShowErrorNoteL( R_FLD_QTN_FILE_FORMAT_ERROR );
        return EFalse;
   		}
   	if( prot )
   		{
        ShowErrorNoteL( R_FLD_QTN_PROFILES_INFO_TONE_DRM_PROTECTED );
		return EFalse;
   		}

   	ContentAccess::TVirtualPathPtr path( aFileName,
   						ContentAccess::KDefaultContentObject );
   	CData* data = CData::NewLC( path, EContentShareReadWrite );
   	TInt isProtected;
   	TInt error = data->GetAttribute( EIsProtected, isProtected );
   	CleanupStack::PopAndDestroy(); // data

    if( error != DRMCommon::EOk )
        {
        // DRM Helper class knows at least rights db corrupted error message.
        // Leaves on system-wide error code.
        iDRMHelper->HandleErrorL( error, aFileName );
        return EFalse;
        }

	// Obtain information whether the file can be set as automated content
	TBool canSetAutomated( EFalse );
    TInt canSetAutomatedErr( iDRMHelper->CanSetAutomated( aFileName, canSetAutomated ) );

	if( !isProtected && canSetAutomated )
		{
		// The file in question is not DRM protected.
        // Return ETrue if file is also Ok unprotected, otherwise EFalse.
        return IsFileValidUnprotectedL( aFileName, aIntention );
		}
	
	// Operator requirement: Check restrictions if file is mp4 audio
	TBuf<KMaxDataTypeLength> dataType( DataTypeL( aFileName ).Des() );
    if( iProfilesFeatures->IsBlockedProtectedType( dataType ) )
        {
        ShowErrorNoteL( R_FLD_QTN_PROFILES_INFO_TONE_DRM_PROTECTED );
        return EFalse;
        }
	
	if ( canSetAutomatedErr == DRMCommon::ERightsExpired ||
		canSetAutomatedErr == DRMCommon::ENoRights )
		{
		// Rights are expired, future rights or missing
		iDRMHelper->HandleErrorL( canSetAutomatedErr, aFileName );
		return EFalse;
		}

    // Operator requirement: Check DRM v2 tones
	if( !canSetAutomated )
        {
		// This is DRM v2 file OR count based v1 tone
	     ShowErrorNoteL( aIntention == EPlay ?
				R_FLD_QTN_DRM_PREV_RIGHTS_USE : R_FLD_QTN_DRM_PREV_RIGHTS_SET);
		return EFalse;
        }
            
	TInt32 infoBits( 0x00000000 );

	// Find out rights information	            
	if( !CFLDDRMImplementation::GetFileInfoL(
	 aFileName, infoBits ) )
		{
		// Corrupted file or "No rights" situation
		return EFalse;
		}

	// Operator requirement: Check CFM protection
	if ( infoBits & ENoRingingTone )
		{
		// This is CFM protected file, ringingtone is set to "no"
		ShowErrorNoteL( aIntention == EPlay ?
			R_FLD_QTN_DRM_PREV_RIGHTS_USE : R_FLD_QTN_DRM_PREV_RIGHTS_SET);
		return EFalse;
		}
      
    if( aIntention == ESelect )
    	{
		// Rights are good to go, and intention is selection
    	// call SetAutomatedPassive to show 'activation query' 
		iDRMHelper->SetAutomatedType( iAutomatedType );
    	error = iDRMHelper->SetAutomatedPassive( aFileName );
    	if( error != KErrCancel )
    		{
	       	// User accepted dialog
        	User::LeaveIfError( error );
        	// New way, does not require DRM capability
        	data = CData::NewLC( path, EContentShareReadWrite );
        	error = data->ExecuteIntent( ContentAccess::EPlay );
        	// Wrongly requires DRM after all. According to Risto Vilkman
        	// from DRM, KErrAccessDenied can be ignored, since if
        	// CanSetAutomated says the tone is OK, it's OK.
        	if ( error != KErrNone && error != KErrAccessDenied )
        		{
        		User::Leave( error );
        		}
        	CleanupStack::PopAndDestroy(); // data
        	}
        else
        	{
        	// User canceled dialog
        	return EFalse;
        	}
		}
        
    return ETrue;
	}

// -----------------------------------------------------------------------------
// CFLDDRMImplementation::GetFileInfo
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CFLDDRMImplementation::GetFileInfoL(
 const TDesC& aFileName, TInt32& aInfoBits ) const
	{
	DRMCommon::TContentProtection contentProtection; // ignored
	HBufC8* mimeType = NULL; // ignored
	TUint dataLength( 0 ); // ignored
	HBufC8* contentURI( NULL );
	   
	// Obtain content URI
    TInt error = iDRMCommon->GetFileInfo(
		aFileName, contentProtection, mimeType, contentURI, dataLength );
    delete mimeType;

    if( error != DRMCommon::EOk )
    	{
    	delete contentURI;
        // Handle possible corrupt file situation
        iDRMHelper->HandleErrorL( error, aFileName );
        return EFalse;
    	}

	// Obtain rights object
	CDRMRights* rights = NULL;
    error = iDRMCommon->GetActiveRights( *contentURI, DRMCommon::EPlay, rights );
    delete contentURI;

	if( error == DRMCommon::ENoRights )
		{
		delete rights;
		// There is no rights for given file
		// Should never arrive here, ENoRights is handled
		// already in IsFileValidL()
   	   	iDRMHelper->HandleErrorL( error, aFileName );
        return EFalse;
		}
	 
	// Obtain infobits ( needed only for CFM case )		
   aInfoBits = rights->GetPermission().iInfoBits;
   delete rights;
   return ETrue;
   }


// -----------------------------------------------------------------------------
//
// Functions related to WMDRM protection check
//
// -----------------------------------------------------------------------------

#ifndef RD_DRM_COMMON_INTERFACE_FOR_OMA_AND_WMDRM

// Some magic constants
static const TInt KMinContentLength( 16 );
//_LIT8( KContentProtectionType, "DRM" );
_LIT8( KASFHeaderObject, "75B22630668E11CFA6D900AA0062CE6C" );
_LIT8( KWrmHeader, "W\0R\0M\0H\0E\0A\0D\0E\0R\0" );

// -----------------------------------------------------------------------------
// FormatGUID
// -----------------------------------------------------------------------------
//
LOCAL_C void FormatGUID( TDes8& aGUID )
    {
    TBuf8<16> copyGUID( aGUID );
    TInt i;
    for( i = 0; i < 4; i++ )
        {
        copyGUID[i] = aGUID[3-i];
        }
    for( i = 4; i < 6; i++ )
        {
        copyGUID[i] = aGUID[9 - i];
        }
    for( i = 6; i < 8; i++ )
        {
        copyGUID[i] = aGUID[13 - i];
        }
    for( i = 8; i < 16 ; i++ )
        {
        copyGUID[i] = aGUID[i];
        }
    aGUID.Delete( 0, 32 );
    for( i = 0; i <16; i++ )
        {
        aGUID.AppendNumFixedWidthUC( copyGUID[i], EHex, 2 );
        }
    }

// -----------------------------------------------------------------------------
// ConvertToInt64
// -----------------------------------------------------------------------------
//
LOCAL_C TInt64 ConvertToInt64( TDesC8& aDes )
    {
    TInt64 num = 0;
    TInt i;
    for( i = 7 ; i >= 0; i-- )
        {
        num <<= 8;
        num |= aDes[i];
        }
    return num;
    }


// -----------------------------------------------------------------------------
// IsProtectedWmDrmL
// returns ETrue, if file is protected WMDRM file
//         EFalse if file is not protected WMDRM file
// Leaves with KErrUnderflow if file has too little data to decide
//         whether WmDrm or not
//         may also leave with other system wide error code
// -----------------------------------------------------------------------------
//
LOCAL_C TBool IsProtectedWmDrmL( RFile& aFileHandle )
    {
    TInt r( KErrNone );
    HBufC8* buffer( NULL );
    TInt pos( 0 );
    RFile file;
    TBuf8< 32 > header;

    TInt64 headerSize( 0 );
    TBool isProtectedWmDrm( EFalse );
    TPtr8 headerPtr( NULL, 0 );

    // Leave if given handle is invalid
    if( !aFileHandle.SubSessionHandle() )
        {
        User::Leave( KErrBadHandle );
        }

    User::LeaveIfError( file.Duplicate( aFileHandle ) );
    CleanupClosePushL( file );

    User::LeaveIfError( file.Seek( ESeekStart, pos ) );

    // Check if the file is an ASF file
    // : Check on runtime wether WM DRM is supporeted or not

    User::LeaveIfError( file.Read( 0, header, KMinContentLength ) );
    if( header.Length() < KMinContentLength )
        {
        User::Leave( KErrUnderflow );
        }

    FormatGUID( header );

    if( header == KASFHeaderObject )
        {
        // It's ASF, check still whether it's WM DRM protected or not
        file.Read( header,8 );
        headerSize = ConvertToInt64( header );
        if( headerSize <= 30 )
            {
            User::Leave( KErrUnderflow );
            }
        if ( headerSize > ( ( KMaxTInt32 / 2 ) - 1 ) )
            {
            User::Leave( KErrOverflow );
            }
        buffer = HBufC8::NewLC( headerSize );

        headerPtr.Set( buffer->Des() );
        User::LeaveIfError( file.Read( headerPtr, headerSize - 24 ) );

        r = headerPtr.Find( KWrmHeader );
        if ( KErrNotFound != r )
            {
            isProtectedWmDrm = ETrue;
            }
        CleanupStack::PopAndDestroy( buffer ); // buffer
        }
        CleanupStack::PopAndDestroy(); // file

    return isProtectedWmDrm;
    }

#endif // RD_DRM_COMMON_INTERFACE_FOR_OMA_AND_WMDRM

// -----------------------------------------------------------------------------
// CFLDDRMImplementation::IsFileWMDRMProtectedL
// -----------------------------------------------------------------------------
//
TBool CFLDDRMImplementation::IsFileWMDRMProtectedL( const TDesC& aFileName ) const
	{
	TBool res = EFalse;
	RFs& fsSession( CCoeEnv::Static()->FsSession() );
	RFile hFile;

	TInt err = hFile.Open( fsSession, aFileName, 
						EFileRead | EFileStream | EFileShareReadersOnly );
	if( err == KErrInUse )
		{
		err = hFile.Open( fsSession, aFileName, 
						EFileRead | EFileStream | EFileShareAny );
		}
	if( err != KErrNone )
		{
		User::Leave( err );
		}
	CleanupClosePushL( hFile );

#ifdef RD_DRM_COMMON_INTERFACE_FOR_OMA_AND_WMDRM
	TPtrC agent( KNullDesC );
	DRM::CDrmUtility* drmUtil( DRM::CDrmUtility::NewLC() );
	drmUtil->GetAgentL( hFile, agent );
	if( agent.Compare( DRM::KDrmWMAgentName ) == 0 )
		{
		res = ETrue;
		}
	CleanupStack::PopAndDestroy( drmUtil );
#else
	res = IsProtectedWmDrmL( hFile );
#endif

	CleanupStack::PopAndDestroy( &hFile );
	return res;
	}

// -----------------------------------------------------------------------------
//
// End of Functions related to WMDRM protection check
//
// -----------------------------------------------------------------------------

//  End of File
