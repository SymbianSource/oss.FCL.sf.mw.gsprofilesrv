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
* Description:  Implementation of the CFLDDRMImplementation.
*
*/



// INCLUDE FILES
#include    "CFLDDRMImplementation.h"

#include    <DRMCommon.h>
#include    <e32svr.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CFLDDRMImplementation::ConstructL
// Empty implementation
// -----------------------------------------------------------------------------
//
void CFLDDRMImplementation::ConstructL()
    {
	iDRMCommon = DRMCommon::NewL();
	User::LeaveIfError( iDRMCommon->Connect() );
    }
    
// Destructor
CFLDDRMImplementation::~CFLDDRMImplementation()
    {
	if( iDRMCommon )
		{
		iDRMCommon->Disconnect(); // ignore possible error
		delete iDRMCommon;
		}
    }

// -----------------------------------------------------------------------------
// CFLDDRMImplementation::IsFileValidL
// Empty implementation
// -----------------------------------------------------------------------------
//
TBool CFLDDRMImplementation::IsFileValidL(
    const TDesC& aFileName, TIntention aIntention )
	{
	TBool isValidUnprotected( ETrue );

    if( aFileName.Compare( KNullDesC ) != 0 )
		{
		TBool isProtected( EFalse );
		iDRMCommon->IsProtectedFile( aFileName, isProtected );
		if( !isProtected )
			{
			// The file in question is not a DCF file.
			// Return ETrue if file is also Ok unprotected, otherwise EFalse.
			isValidUnprotected =
				IsFileValidUnprotectedL( aFileName, aIntention );
			}
		}

    return isValidUnprotected;
	}

//  End of File  
