/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
*
*/
#include "TestGSPlugin.h"


	
// CONSTRUCTION
CTestGSPlugin* CTestGSPlugin::NewL()
    {
    CTestGSPlugin* self = CTestGSPlugin::NewLC();
    CleanupStack::Pop();

    return self;
    }

CTestGSPlugin* CTestGSPlugin::NewLC()
    {
    CTestGSPlugin* self = new( ELeave ) CTestGSPlugin();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor 
CTestGSPlugin::~CTestGSPlugin()
    {
    }

// Default constructor
CTestGSPlugin::CTestGSPlugin()
    {
    }

// Second phase construct
void CTestGSPlugin::ConstructL()
    {
    }


void CTestGSPlugin::GetCaptionL( TDes& aCaption ) const
	{
	
	}

TUid CTestGSPlugin::Id() const
	{
	
	}
	
//  END OF FILE