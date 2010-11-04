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
* Description: Implementation of the CFLDSingleGraphicEntryFormatter.
*
*
*/


// CLASS HEADER
#include "CFLDSingleGraphicEntryFormatter.h"

// INTERNAL INCLUDES
#include "CFLDEntryReference.h"

// EXTERNAL INCLUDES
#include <f32file.h>	// For TParsePtrC
#include <AknUtils.h>	// For AknTextUtils
#include <driveinfo.h>

// CONSTANTS
namespace
	{
	// Some helper literals
	_LIT( KFLDTab, "\t" );
	_LIT( KFLDIgnoredExt, ".rng" );

	// these icon array indexes must match with the implementation
	// of the function CFLDFileListContainer.cpp/PopulateIconArrayL
	_LIT( KFLDZeroAndTab, "0\t" );
	_LIT( KFLDSoundAndTab, "3\t" );
	_LIT( KFLDVideoAndTab, "4\t" );
	_LIT( KFLDMemoryCard, "2" );
	_LIT( KFLDMassStorage, "5" );
	
	}

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CFLDSingleGraphicEntryFormatter::CFLDSingleGraphicEntryFormatter
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CFLDSingleGraphicEntryFormatter::CFLDSingleGraphicEntryFormatter()
	{
	
	}

// -----------------------------------------------------------------------------
// CFLDSingleGraphicEntryFormatter::~CFLDSingleGraphicEntryFormatter()
// -----------------------------------------------------------------------------
//
CFLDSingleGraphicEntryFormatter::~CFLDSingleGraphicEntryFormatter()
    {
    delete iDriveUtil;
    }
    
// -----------------------------------------------------------------------------
// CFLDSingleGraphicEntryFormatter::FormatL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDSingleGraphicEntryFormatter::FormatL( CFLDEntryReference& aEntry )
    {
    if ( !iDriveUtil )
        {
        iDriveUtil = CDriveUtil::NewL();
        }
        
	HBufC* formattedName = NULL;
	HBufC& pathAndMediaFileName = aEntry.PathAndMediaFileName();
    TParsePtrC parsedName( pathAndMediaFileName.Des() );

#ifdef RD_VIDEO_AS_RINGING_TONE	    
	TBool isFileVideo( EFalse );
	if ( aEntry.MediaType() == ECLFMediaTypeVideo )
		{
		isFileVideo = ETrue;
		}
  
    if( isFileVideo )
    	{
    	formattedName = HBufC::NewL( parsedName.NameAndExt().Length() + KFLDVideoAndTab().Length() * 2 );
    	}
    else
    	{
    	formattedName = HBufC::NewL( parsedName.NameAndExt().Length() + KFLDSoundAndTab().Length() * 2 );
    	}
#else
   	formattedName = HBufC::NewL( parsedName.NameAndExt().Length() + KFLDZeroAndTab().Length() * 2 );
#endif

	aEntry.SetFormattedPresentation( formattedName );
		
	// Combine all the data to the formatted data descriptor:
	TPtr des = formattedName->Des();

 	// RNG files are shown without the file extension
    TPtrC ext( parsedName.Ext() );
    if ( ext.CompareF( KFLDIgnoredExt ) == 0 )
        {
        des.Copy( parsedName.Name() );
        }
    else
        {
        des.Copy( parsedName.NameAndExt() );
        }
        
	// Make sure that the file name does not contain tab characters
	AknTextUtils::ReplaceCharacters( des, KAknReplaceListControlChars, TChar(' ') );        

#ifdef RD_VIDEO_AS_RINGING_TONE	    
	if( isFileVideo )
		{
		des.Insert( 0, KFLDVideoAndTab );
		}
	else
		{
		des.Insert( 0, KFLDSoundAndTab );
		}
    
#else
    des.Insert( 0, KFLDZeroAndTab );
#endif    
	des.Append( KFLDTab );
         
	/*
	TChar mmcDriveLetter( PathInfo::MemoryCardRootPath()[0] );
	mmcDriveLetter.UpperCase();

    // Check if the entry is on MMC
    TChar entryDriveLetter( parsedName.Drive()[ 0 ] );
    entryDriveLetter.UpperCase();

    if ( entryDriveLetter == mmcDriveLetter )
        {
        des.Append( '2' ); // MMC icon index
        }
    */
    
    if ( iDriveUtil->IsMemoryCard( aEntry.PathAndMediaFileName() ) )
        {
        des.Append( KFLDMemoryCard );
        }
    else if ( iDriveUtil->IsMassStorage( aEntry.PathAndMediaFileName() ) )
        {
        des.Append( KFLDMassStorage );
        }
    }

// -----------------------------------------------------------------------------
// CFLDSingleGraphicEntryFormatter::FormatTextLC()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
HBufC* CFLDSingleGraphicEntryFormatter::FormatTextLC( const TDesC& aText )
    {
    HBufC* text = NULL;
    if( aText.Find( KFLDTab ) == KErrNotFound )
        {
        text = HBufC::NewLC( aText.Length() + KFLDZeroAndTab().Length() );
        TPtr des( text->Des() );
        des.Copy( KFLDZeroAndTab );
        des.Append( aText );
        }
    else
        {
        text = aText.AllocLC();
        }
    return text;
    }




/******************************************************************************
 * class CDriveUtil
 ******************************************************************************/


// -----------------------------------------------------------------------------
// CDriveUtil::NewL
// 
// -----------------------------------------------------------------------------
//
CDriveUtil* CDriveUtil::NewL()
    {
    CDriveUtil* self = new (ELeave) CDriveUtil();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);

    return self;
    }


// -----------------------------------------------------------------------------
// Destructor
// 
// -----------------------------------------------------------------------------
//
CDriveUtil::~CDriveUtil()
    {
	iFsSession.Close();
    }


// -----------------------------------------------------------------------------
// CDriveUtil::CDriveUtil
// 
// -----------------------------------------------------------------------------
//
CDriveUtil::CDriveUtil()
    {
    }


// -----------------------------------------------------------------------------
// CDriveUtil::ConstructL
// 
// -----------------------------------------------------------------------------
//
void CDriveUtil::ConstructL()
    {
    User::LeaveIfError( iFsSession.Connect() );
    }


// -----------------------------------------------------------------------------
// CDriveUtil::IsMassStorage
// 
// -----------------------------------------------------------------------------
//
TBool CDriveUtil::IsMassStorage(const TDesC& aFullPath)
    {
    const TUint KMassStorageBits = DriveInfo::EDriveInternal |
                                   DriveInfo::EDriveExternallyMountable;

    TDriveUnit drive(aFullPath);
    TUint driveStatus(0);
    
    TInt err = DriveInfo::GetDriveStatus( iFsSession, drive, driveStatus );
    if ( err != KErrNone )
        {
        return EFalse;
        }
        
    if ( (driveStatus & KMassStorageBits) == KMassStorageBits )
        {
        return ETrue;
        }
        
    return EFalse;
    }


// -----------------------------------------------------------------------------
// CDriveUtil::IsMemoryCard
// 
// -----------------------------------------------------------------------------
//
TBool CDriveUtil::IsMemoryCard(const TDesC& aFullPath)
    {
    const TUint KMemoryCardBits = DriveInfo::EDriveRemovable |
                                  DriveInfo::EDriveExternallyMountable;

    TDriveUnit drive(aFullPath);
    TUint driveStatus(0);
    
    TInt err = DriveInfo::GetDriveStatus( iFsSession, drive, driveStatus );
    if ( err != KErrNone )
        {
        return EFalse;
        }
        
    if ( (driveStatus & KMemoryCardBits) == KMemoryCardBits )
        {
        return ETrue;
        }
        
    return EFalse;
    }


//  End of File
