/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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



#include "TestGSBaseView.h"


	
// CONSTRUCTION
CTestGSBaseView* CTestGSBaseView::NewL()
    {
    CTestGSBaseView* self = CTestGSBaseView::NewLC();
    CleanupStack::Pop();

    return self;
    }

CTestGSBaseView* CTestGSBaseView::NewLC()
    {
    CTestGSBaseView* self = new( ELeave ) CTestGSBaseView();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor 
CTestGSBaseView::~CTestGSBaseView()
    {
    }

// Default constructor
CTestGSBaseView::CTestGSBaseView()
    {
    }

// Second phase construct
void CTestGSBaseView::ConstructL()
    {
    }

//  METHODS

void CTestGSBaseView::NewContainerL()
    {
    }

void CTestGSBaseView::HandleListBoxSelectionL()
    {
    }
    
void CTestGSBaseView::GetCaptionL( TDes& aCaption ) const
	{
	
	}

TUid CTestGSBaseView::Id() const
	{
	
	}
	
void CTestGSBaseView::SetNaviPaneL()
	{
	CGSBaseView::SetNaviPaneL();
	}
	
void CTestGSBaseView::CreateNaviPaneContextL( TInt aResourceId )
	{
	CGSBaseView::CreateNaviPaneContextL( aResourceId );
	}
//  END OF FILE
