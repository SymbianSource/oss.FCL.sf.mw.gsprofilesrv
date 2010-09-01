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
* Description:  Implementation for Download tones feature
*
*/



// CLASS HEADER
#include    "CFLDBrowserLauncher.h"

// INTERNAL INCLUDES
#include    "CFLDCommandAbsorbingControl.h"

// EXTERNAL INCLUDES
#include    <browserlauncher.h>
#include    <DocumentHandler.h>
#include    <eikenv.h>
#include    <apmstd.h>
#include	<AiwCommon.h>
#include    <browseroverriddensettings.h>
#include    <favouriteslimits.h> // Context Id:s


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CFLDBrowserLauncher::CFLDBrowserLauncher
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CFLDBrowserLauncher::CFLDBrowserLauncher()
    {
    }

// -----------------------------------------------------------------------------
// CFLDBrowserLauncher::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CFLDBrowserLauncher::ConstructL()
    {
    iBrowserLauncher = CBrowserLauncher::NewL();
    iDocumentHandler = CDocumentHandler::NewL( CEikonEnv::Static()->Process() );
    }

// -----------------------------------------------------------------------------
// CFLDBrowserLauncher::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CFLDBrowserLauncher* CFLDBrowserLauncher::NewLC()
    {
    CFLDBrowserLauncher* self = new( ELeave ) CFLDBrowserLauncher();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// Destructor
CFLDBrowserLauncher::~CFLDBrowserLauncher()
    {
    delete iDocumentHandler;
    delete iBrowserLauncher;
    }

// -----------------------------------------------------------------------------
// CFLDBrowserLauncher::HandleContentL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CFLDBrowserLauncher::HandleContentL(
		 const TDesC& aFileName,
	  		const CAiwGenericParamList& aParamList, TBool& aContinue )
	{
	TBool isSaved( EFalse );

    if( aParamList.Count() > 0 )
   		{
        TInt index( 0 );
        aParamList.FindFirst( index, EGenericParamFileSaved );

        if( index != KErrNotFound )
        	{
	        isSaved = ( aParamList[ index ].Value().AsTInt32() );
       		}

         }

	// Tone is NOT yet saved by a browser, save it by using DocHandler
	if(!isSaved)
		{
		//Let documenthandler to find out the datatype
		TDataType nullType;
		iDocumentHandler->CopyL( aFileName, KNullDesC, nullType, NULL );
		}
	
	aContinue = ETrue;
	return ETrue;
	}

// -----------------------------------------------------------------------------
// CFLDBrowserLauncher::HandleContentL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TAny* CFLDBrowserLauncher::DownloadedContentHandlerReserved1( TAny* /*aAnyParam*/ )
	{
	return NULL;
	}

// -----------------------------------------------------------------------------
// CFLDBrowserLauncher::LaunchBrowserL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDBrowserLauncher::LaunchBrowserL()
    {
	 // Absorb all key events and commands until browsing is finished
    CFLDCommandAbsorbingControl::NewLC(); // Discard pointer, it's in stack.

	iBrowserLauncher->LaunchBrowserSyncEmbeddedL(
	 KFavouritesAudioContextId );

    CleanupStack::PopAndDestroy(); // Browsing finished, remove command absorber.

    }

//  End of File
