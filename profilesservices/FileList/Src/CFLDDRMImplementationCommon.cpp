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
#include    "CFLDRingingTonePlayer.h"
#include	"CFLDFileListModel.h"

// EXTERNAL INCLUDES
#include    <aknnotewrappers.h>
#include	<StringLoader.h>
#include    <FileList.rsg>
#include    <MProfilesLocalFeatures.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CFLDDRMImplementation::CFLDDRMImplementation
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CFLDDRMImplementation::CFLDDRMImplementation( CFLDFileListModel* aModel )
	: iModel( aModel ),
	iAutomatedType( CDRMHelper::EAutomatedTypeRingingTone )
    {
    }

// -----------------------------------------------------------------------------
// CFLDDRMImplementation::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CFLDDRMImplementation* CFLDDRMImplementation::NewL( CFLDFileListModel* aModel )
    {
    CFLDDRMImplementation* self = new( ELeave ) CFLDDRMImplementation( aModel );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CFLDDRMImplementation::ShowErrorNoteL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDDRMImplementation::ShowErrorNoteL( TInt aResourceId ) const
    {
    HBufC* errorText = StringLoader::LoadLC( aResourceId );
    CAknInformationNote* note = new( ELeave ) CAknInformationNote( EFalse );

    note->ExecuteLD( *errorText );

	// errorText
	CleanupStack::PopAndDestroy( errorText );
    }

// -----------------------------------------------------------------------------
// CFLDDRMImplementation::IsFileValidUnprotectedL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CFLDDRMImplementation::IsFileValidUnprotectedL(
    const TDesC& aFileName, TIntention aIntention ) const
    {
	TBuf<KMaxDataTypeLength> tempDataType( DataTypeL( aFileName ).Des() );

    if( iProfilesFeatures->IsBlockedType( tempDataType ) )
        {
        if( aIntention == ESelect )
            {
            ShowErrorNoteL( R_FLD_QTN_TEXT_NOT_ALLOWED );
            }
    	return EFalse;
        }

    // Operator requirement. Check if DRM is required with tones.
	if( aIntention == EPlay )
		{
        if( iProfilesFeatures->IsBlockedDemoPlayType( tempDataType ) )
            {
			return EFalse;
            }
        }
    else
        {
        if( iProfilesFeatures->IsBlockedUnprotectedType( tempDataType ) )
            {
			ShowErrorNoteL( R_FLD_QTN_PROFILES_INFO_TONE_NO_DRM );
			return EFalse;
            }
        }

    return ETrue;
    }

// -----------------------------------------------------------------------------
// CFLDDRMImplementation::DataTypeL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TDataType CFLDDRMImplementation::DataTypeL(
    const TDesC& aFileName ) const
    {
    RApaLsSession apaLsSession;
    User::LeaveIfError( apaLsSession.Connect() );
    CleanupClosePushL( apaLsSession );

    TUid dummyUid = { 0 }; // instantiate as zero
    TDataType dataType( dummyUid );
    User::LeaveIfError(
        apaLsSession.AppForDocument( aFileName, dummyUid, dataType ) );

    CleanupStack::PopAndDestroy(); // apaLsSession.Close()
	return dataType;
    }

// -----------------------------------------------------------------------------
// CFLDDRMImplementation::MediaFileType
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt32 CFLDDRMImplementation::MediaFileType( const TDesC& aFileName ) const
	{
	return iModel->MediaFileType( aFileName );
	}

//  End of File
