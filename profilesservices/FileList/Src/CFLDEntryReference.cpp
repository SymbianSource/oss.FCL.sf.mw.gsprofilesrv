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
* Description: 
*     A simple container class that is used to reference a single
*     entry in the file list.
*
*
*/


// CLASS HEADER
#include    "CFLDEntryReference.h"

// INTERNAL INCLUDES

// EXTERNAL INCLUDES

// ================= MEMBER FUNCTIONS =======================

// Constructor
CFLDEntryReference::CFLDEntryReference()
    {
    }
    
// Destructor
CFLDEntryReference::~CFLDEntryReference()
    {
    delete iFormattedPresentation;
    delete iPathAndMediaFileName;
    }

// -----------------------------------------------------------------------------
// CFLDEntryReference::FormattedPresentation()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
HBufC& CFLDEntryReference::FormattedPresentation() const
	{
	return *iFormattedPresentation;
	}

// -----------------------------------------------------------------------------
// CFLDEntryReference::PathAndMediaFileName()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
HBufC& CFLDEntryReference::PathAndMediaFileName() const
	{
	return *iPathAndMediaFileName;
	}

// -----------------------------------------------------------------------------
// CFLDEntryReference::MediaType()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
TInt32 CFLDEntryReference::MediaType() const
	{
	return iMediaType;
	}	

// -----------------------------------------------------------------------------
// CFLDEntryReference::SetPathAndMediaFileName()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDEntryReference::SetPathAndMediaFileName( HBufC* aPathAndMediaFileName ) // CSI: 60 # ownership is transferred
	{
	delete iPathAndMediaFileName;
	iPathAndMediaFileName = NULL;
	
	iPathAndMediaFileName = aPathAndMediaFileName;
	}

// -----------------------------------------------------------------------------
// CFLDFileListContainer::SetAutomatedType()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDEntryReference::SetFormattedPresentation( HBufC* aFormattedPresentation ) // CSI: 60 # ownership is transferred
	{
	delete iFormattedPresentation;
	iFormattedPresentation = NULL;
	
	iFormattedPresentation = aFormattedPresentation;
	}

// -----------------------------------------------------------------------------
// CFLDEntryReference::SetMediaType()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
void CFLDEntryReference::SetMediaType( TInt32 aMediaType )
	{
	iMediaType = aMediaType;
	}

//  End of File
