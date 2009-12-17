/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Document for the General Settings Application
*
*/


// INCLUDE FILES
#include    <bldvariant.hrh>

#include    "GSDocument.h"
#include    "GSUi.h"
#include    "gsplugininterface.h"
#include    "gspluginloader.h"


// ================= MEMBER FUNCTIONS =======================

// ----------------------------------------------------
// CGSDocument::ConstructL()
// Symbian OS default constructor can leave.
//
// ----------------------------------------------------
//
void CGSDocument::ConstructL()
    {
    CGSBaseDocument::ConstructL();
    }


// ----------------------------------------------------
// CGSDocument::~CGSDocument
// Symbian OS destructor
//
// ----------------------------------------------------
//
CGSDocument::~CGSDocument()
    {
    }


// ----------------------------------------------------
// CGSDocument::NewL()
// Two-phased constructor.
//
// ----------------------------------------------------
//
CGSDocument* CGSDocument::NewL( CEikApplication& aApp )
    {
    CGSDocument* self = new( ELeave ) CGSDocument( aApp );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ----------------------------------------------------
// CGSDocument::ModelCdma()
// Returns the CDMA model.
//
// ----------------------------------------------------
//
CGSModelCdma* CGSDocument::ModelCdma()
    {
    // CDMA engine is not supported by this variant.
    return NULL;
    }


// ----------------------------------------------------
// CGSDocument::CreateAppUiL()
// constructs CGSUi
//
// ----------------------------------------------------
//
CEikAppUi* CGSDocument::CreateAppUiL()
    {
    CGSUi* appUi = new(ELeave) CGSUi();
    return appUi;
    }


// End of File
