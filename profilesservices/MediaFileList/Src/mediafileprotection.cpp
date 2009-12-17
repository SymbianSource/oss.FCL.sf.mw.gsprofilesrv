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
* Description:   Class used to check the protection of the 
*                media files.
*
*/




// INCLUDE FILES

#include "mediafileprotection.h"
#include "mediafilelist.h"
#include "mediafilelistdebug.h"
#include <mediafilelist.rsg>

#include <DRMCommon.h>
#include <drmutility.h>
#include <drmagents.h>
#include <bautils.h>
#include <StringLoader.h>



/******************************************************************************
 * class CMFProtectionHandler
 ******************************************************************************/

// -----------------------------------------------------------------------------
// CMFProtectionHandler::NewL
//
// -----------------------------------------------------------------------------
CMFProtectionHandler* CMFProtectionHandler::NewL()
    {
    CMFProtectionHandler* self = new (ELeave) CMFProtectionHandler();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }


// -----------------------------------------------------------------------------
// CMFProtectionHandler::CMFProtectionHandler
// 
// -----------------------------------------------------------------------------
//
CMFProtectionHandler::CMFProtectionHandler()
	{
	iMaxFileSize = KErrNotFound;
    }


// -----------------------------------------------------------------------------
// CMFProtectionHandler::CMFProtectionHandler
//
// -----------------------------------------------------------------------------
//
void CMFProtectionHandler::ConstructL()
    {
    iDriveUtil = CDriveUtil::NewL();
    iVariation = CMediaFileDialogVariation::NewL();

    iDRMHelper = CDRMHelper::NewL( *CCoeEnv::Static() );
    
    iDRMCommon = DRMCommon::NewL();
    User::LeaveIfError( iDRMCommon->Connect() );
    
    iExcludedMimeTypes = CMimeTypeList::NewL();
    
    User::LeaveIfError( iApaLsSession.Connect() );
    User::LeaveIfError( iFsSession.Connect() );
    }


// ----------------------------------------------------------------------------
// Destructor
//
// ----------------------------------------------------------------------------
//
CMFProtectionHandler::~CMFProtectionHandler()
    {
    delete iDriveUtil;
    delete iVariation;
    
    delete iDRMHelper;
    
    if ( iDRMCommon )
        {
        iDRMCommon->Disconnect(); // ignore possible error
        delete iDRMCommon;
        }
    
    delete iExcludedMimeTypes;
    iApaLsSession.Close();
    iFsSession.Close();
    }


// -----------------------------------------------------------------------------
// CMFProtectionHandler::SetAttr
// 
// -----------------------------------------------------------------------------
void CMFProtectionHandler::SetAttrL( TInt aAttr, TInt aValue )
    {
    switch ( aAttr )
        {
        case CMediaFileList::EAttrFileSize:
            {
            iMaxFileSize = aValue;
            break;
            }
        case CMediaFileList::EAttrAutomatedType:
            {
            iAutomatedType = (CDRMHelper::TDRMHelperAutomatedType) aValue;
            break;
            }

        default:
            {
            break;
            }
        }
    }


// -----------------------------------------------------------------------------
// CMFProtectionHandler::SetAttrL
// 
// -----------------------------------------------------------------------------
void CMFProtectionHandler::SetAttrL( TInt aAttr, const TDesC& aValue )
    {
    if ( aAttr == CMediaFileList::EAttrExcludeMimeType )
        {
        if ( aValue.Length() > KMaxFileName || aValue.Length() == 0 )
            {
            User::Leave( KErrArgument );
            }
            
        iExcludedMimeTypes->AddMimeTypeL( aValue );
        }
    }


// -----------------------------------------------------------------------------
// CMFProtectionHandler::IsVideoValid
// 
// -----------------------------------------------------------------------------
//
TBool CMFProtectionHandler::IsVideoValid( const TDesC& aFileName,
                                         TIntention aIntention )
    {
    TInt err = KErrNone;
    TBool ret = EFalse;
    
    TRAP( err, ret = IsVideoValidL (aFileName, aIntention ) )
    
    if ( err != KErrNone )
        {
        return EFalse;  // in case of error file is not valid
        }
    
    return ret;
    }


// -----------------------------------------------------------------------------
// CMFProtectionHandler::IsVideoValidL
// 
// -----------------------------------------------------------------------------
//
TBool CMFProtectionHandler::IsVideoValidL( const TDesC& aFileName,
                                          TIntention /*aIntention*/ )
    {
    TBuf<KMaxDataTypeLength> dataType( DataTypeL( aFileName ).Des() );
  
    if ( iExcludedMimeTypes->FindMimeTypeL( dataType ) )
        {
        return EFalse;
        }

    CContent* content = CContent::NewLC( aFileName,
                                         EContentShareReadWrite );
    TInt deliveryMethod = 0;
    content->GetAttribute( EDeliveryMethod, deliveryMethod );
    
    CleanupStack::PopAndDestroy( content );
    
    if( deliveryMethod == EOmaDrm2 )
        {
        // if video it is OMA DRM 2 protected, it cannot be previewed
        return EFalse;
        }
    return ETrue;
    }


// -----------------------------------------------------------------------------
// CMFProtectionHandler::IsFileValid
// 
// Checks protection state of media file. If this function returns EFalse,
// media file cannot be used for playing/selection.
//
// -----------------------------------------------------------------------------
//
TBool CMFProtectionHandler::IsFileValid( const TDesC& aFileName,
                                         TIntention aIntention )
    {
    TInt err = KErrNone;
    TBool ret = EFalse;
    
    TRAP( err, ret = IsFileValidL (aFileName, aIntention ) )
    
    if ( err != KErrNone )
        {
        return EFalse;  // in case of error file is not valid
        }
    
    return ret;
    }


// -----------------------------------------------------------------------------
// CMFProtectionHandler::IsFileValidL
// 
// Function is copy from CFLDDRMImplementation.cpp.
// -----------------------------------------------------------------------------
//
TBool CMFProtectionHandler::IsFileValidL( const TDesC& aFileName, 
                                          TIntention aIntention )
    {
    TInt err = KErrNone;
    TBuf<KMaxDataTypeLength> dataType( DataTypeL( aFileName ).Des() );
    
    if ( aFileName.Length() == 0 )
        {
        return ETrue;  // empty filename is valid??
        }
 
    if ( iExcludedMimeTypes->FindMimeTypeL( dataType ) )
        {
        ShowErrorNoteL( R_QTN_INFO_FILE_FORMAT_ERROR );
        return EFalse;
        }
    
    if ( aIntention == CMFProtectionHandler::ESelect && 
        !CheckFileSize( aFileName, dataType ) )
        {
        HBufC* hBuf = StringLoader::LoadLC( R_QTN_INFO_FILE_SIZE_ERROR, iMaxFileSize );
        TMFDialogUtil::ShowInformationNoteL( *hBuf );
        CleanupStack::PopAndDestroy( hBuf );
        return EFalse;
        }

    if ( iDriveUtil->IsRom( aFileName ) )
        {
        return ETrue;      // files in ROM are always valid
        }

    // check if file is WMDRM protected
    TBool prot =  EFalse;
    TRAP( err, prot = IsFileWMDRMProtectedL( aFileName ) );
    if ( err != KErrNone )
        {
        ShowErrorNoteL( R_QTN_INFO_FILE_FORMAT_ERROR );
        return EFalse;
        }
    if ( prot )
        {
        if ( aIntention == EPlay )
            {
            ShowErrorNoteL( R_QTN_INFO_DRM_PREV_RIGHTS_USE );
            }
        else
            {
            ShowErrorNoteL( R_QTN_INFO_DRM_PROTECTED );
            }
        return EFalse;
        }

    ContentAccess::TVirtualPathPtr path( aFileName,
                        ContentAccess::KDefaultContentObject );
    CData* data = CData::NewLC( path, EContentShareReadWrite );
    TInt isProtected;
    err = data->GetAttribute( EIsProtected, isProtected );
    CleanupStack::PopAndDestroy( data );

    if ( err != DRMCommon::EOk )
        {
        // DRM Helper class knows at least rights db corrupted error message.
        // Leaves on system-wide error code.
        iDRMHelper->HandleErrorL( err, aFileName );
        return EFalse;
        }

    // Obtain information whether the file can be set as automated content
    TBool canSetAutomated = EFalse;
    TInt canSetAutomatedErr = 
         iDRMHelper->CanSetAutomated( aFileName, canSetAutomated );

    if ( !isProtected && canSetAutomated )
        {
        // The file in question is not DRM protected.
        // Return ETrue if file is also Ok unprotected, otherwise EFalse.
        TBool ret = IsFileValidUnprotectedL( aFileName, aIntention );
        return ret;
        }
    
    // Operator requirement: Check restrictions if file is mp4 audio
    if ( iVariation->IsBlockedProtectedType( dataType ) )
        {
        ShowErrorNoteL( R_QTN_INFO_DRM_PROTECTED );
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
    if ( !canSetAutomated )
        {
        // This is DRM v2 file OR count based v1 tone
        if ( aIntention == EPlay )
            {
            ShowErrorNoteL( R_QTN_INFO_DRM_PREV_RIGHTS_USE );
            }
        else
            {
            ShowErrorNoteL( R_QTN_INFO_DRM_PROTECTED );
            }
        return EFalse;
        }
            
    TInt32 infoBits( 0x00000000 );

    // Find out rights information
    if ( !GetFileInfoL( aFileName, infoBits ) )
        {
        // Corrupted file or "No rights" situation
        return EFalse;
        }

    // Operator requirement: Check CFM protection
    if ( infoBits & ENoRingingTone )
        {
        // This is CFM protected file, ringingtone is set to "no"
        if ( aIntention == EPlay )
            {
            ShowErrorNoteL( R_QTN_INFO_DRM_PREV_RIGHTS_USE );
            }
        else
            {
            ShowErrorNoteL( R_QTN_INFO_DRM_PREV_RIGHTS_SET );
            }
        return EFalse;
        }
      
    if ( aIntention == ESelect )
        {
        // Rights are good to go, and intention is selection
        // call SetAutomatedPassive to show 'activation query' 
        iDRMHelper->SetAutomatedType( iAutomatedType );
        err = iDRMHelper->SetAutomatedPassive( aFileName );
        if ( err != KErrCancel )
            {
            // User accepted dialog
            User::LeaveIfError( err );
            // New way, does not require DRM capability
            data = CData::NewLC( path, EContentShareReadWrite );
            err = data->ExecuteIntent( ContentAccess::EPlay );
            // Wrongly requires DRM after all. According to Risto Vilkman
            // from DRM, KErrAccessDenied can be ignored, since if
            // CanSetAutomated says the tone is OK, it's OK.
            if ( err != KErrNone && err != KErrAccessDenied )
                {
                User::Leave( err );
                }
            CleanupStack::PopAndDestroy( data );
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
// CMFProtectionHandler::GetFileInfo
//
// Function is copy from CFLDDRMImplementation.cpp.
// -----------------------------------------------------------------------------
//
TBool CMFProtectionHandler::GetFileInfoL( const TDesC& aFileName,
                                          TInt32& aInfoBits )
    {
    DRMCommon::TContentProtection contentProtection; // ignored
    HBufC8* mimeType = NULL; // ignored
    TUint dataLength = 0; // ignored
    HBufC8* contentURI = NULL;
       
    // Obtain content URI
    TInt error = iDRMCommon->GetFileInfo(
        aFileName, contentProtection, mimeType, contentURI, dataLength );
    delete mimeType;

    if ( error != DRMCommon::EOk )
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

    if ( error == DRMCommon::ENoRights )
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
// CMFProtectionHandler::IsFileWMDRMProtectedL
//
// Function is copy from CFLDDRMImplementation.cpp.
// -----------------------------------------------------------------------------
//
TBool CMFProtectionHandler::IsFileWMDRMProtectedL( const TDesC& aFileName )
    {
    TBool res = EFalse;
    RFile hFile;

    TInt err = hFile.Open( iFsSession, aFileName, 
                           EFileRead | EFileStream | EFileShareReadersOnly );
    if ( err == KErrInUse )
        {
        err = hFile.Open( iFsSession, aFileName, 
                        EFileRead | EFileStream | EFileShareAny );
        }
    if ( err != KErrNone )
        {
        User::Leave( err );
        }
    CleanupClosePushL( hFile );

    TPtrC agent( KNullDesC );
    DRM::CDrmUtility* drmUtil = DRM::CDrmUtility::NewLC();
    drmUtil->GetAgentL( hFile, agent );
    if ( agent.Compare( DRM::KDrmWMAgentName ) == 0 )
        {
        res = ETrue;
        }
    CleanupStack::PopAndDestroy( drmUtil );

    CleanupStack::PopAndDestroy( &hFile );
    return res;
    }


// -----------------------------------------------------------------------------
// CMFProtectionHandler::DataTypeL
//
// Function is copy from CFLDDRMImplementationCommon.cpp.
// -----------------------------------------------------------------------------
//
TDataType CMFProtectionHandler::DataTypeL( const TDesC& aFileName )
    {
    TUid dummyUid = { 0 };
    TDataType dataType( dummyUid );
    User::LeaveIfError(
        iApaLsSession.AppForDocument( aFileName, dummyUid, dataType ) );

    return dataType;
    }


// -----------------------------------------------------------------------------
// CMFProtectionHandler::IsFileValidUnprotectedL
// 
// Function is copy from CFLDDRMImplementationCommon.cpp.
// -----------------------------------------------------------------------------
//
TBool CMFProtectionHandler::IsFileValidUnprotectedL( const TDesC& aFileName,
                                                     TIntention aIntention )
    {
    TDataType dataType = DataTypeL( aFileName );
    TBuf<KMaxDataTypeLength> mimeType;
    
    mimeType = dataType.Des();

    if ( iVariation->IsBlockedType( mimeType ) )
        {
        if ( aIntention == ESelect )
            {
            ShowErrorNoteL( R_QTN_INFO_TEXT_NOT_ALLOWED );
            }
        return EFalse;
        }

    // Operator requirement. Check if DRM is required with tones.
    if ( aIntention == EPlay )
        {
        if ( iVariation->IsBlockedDemoPlayType( mimeType ) )
            {
            return EFalse;
            }
        }
    else
        {
        if ( iVariation->IsBlockedUnprotectedType( mimeType ) )
            {
            ShowErrorNoteL( R_QTN_INFO_NO_DRM );
            return EFalse;
            }
        }

    return ETrue;
    }


// -----------------------------------------------------------------------------
// CMFProtectionHandler::CheckFileSize
//
// -----------------------------------------------------------------------------
//
TBool CMFProtectionHandler::CheckFileSize( const TDesC& aFile, const TDesC& aMimeType  )
    {
    _LIT( KVideo, "video" );
    const TInt KKiloByte = 1024;
    const TInt KSmallSize = 10;
   
    if ( iMaxFileSize < KSmallSize )
        {
        return ETrue; // too small size limit
        }
    if ( aMimeType.Find( KVideo ) != KErrNotFound )
        {
        return ETrue;  // only audio files are checked
        }
    
    TEntry entry;
    TInt err = iFsSession.Entry( aFile, entry );
    if ( err == KErrNone && iMaxFileSize != KErrNotFound )
        {
        TInt size = iMaxFileSize * KKiloByte; // KBytes -> Bytes
        if ( entry.iSize > size )
            {
            return EFalse;
            }
        }
  
    return ETrue;
    }


// -----------------------------------------------------------------------------
// CMFProtectionHandler::ShowErrorNoteL
// 
// -----------------------------------------------------------------------------
//
void CMFProtectionHandler::ShowErrorNoteL( TInt aResourceId )
    {
    TMFDialogUtil::ShowInformationNoteL( aResourceId );
    }


//  End of File  
