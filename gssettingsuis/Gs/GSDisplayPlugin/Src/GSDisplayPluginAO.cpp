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
* Description: 
*
*/


// INCLUDE FILES
#include "GSDisplayPlugin.h"
#include "GSDisplayPluginAO.h"
#include "GsLogger.h"
#include <psmsrvdomaincrkeys.h>

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES

// CONSTANTS

// ----------------------------------------------------------------------------------
// CGSDisplayPluginAO::CGSDisplayPluginAO
// ----------------------------------------------------------------------------------
CGSDisplayPluginAO::CGSDisplayPluginAO() : CActive( EPriorityStandard )
    {
    
    }


// ----------------------------------------------------------------------------------
// CGSDisplayPluginAO::~CGSDisplayPluginAO
// ----------------------------------------------------------------------------------
CGSDisplayPluginAO::~CGSDisplayPluginAO()
    {
    Cancel();
    delete iPsmRepository;
    }


// ----------------------------------------------------------------------------------
// CGSDisplayPluginAO::DoCancel
// ----------------------------------------------------------------------------------
void CGSDisplayPluginAO::DoCancel()
    {
    iPsmRepository->NotifyCancel( KPsmCurrentMode );
    }

// ----------------------------------------------------------------------------------
// CGSDisplayPluginAO::RunL
// ----------------------------------------------------------------------------------
void CGSDisplayPluginAO::RunL()
    {
    // Request complete, notify request again and get new value
    iPsmRepository->Get( KPsmCurrentMode, iPsmMode );
    iPsmRepository->NotifyRequest( KPsmCurrentMode, iStatus );
    SetActive();
    __GSLOGSTRING1( "[CGSDisplayPluginAO::RunL] iPsmMode:%d", iPsmMode );

    // Update display view when PSM mode has changed
    iDisplayView->UpdateViewL();
    }

// ----------------------------------------------------------------------------------
// CGSDisplayPluginAO::RunError
// ----------------------------------------------------------------------------------
TInt CGSDisplayPluginAO::RunError( TInt /*aError*/ )
    {
    return KErrNone;
    }


// ----------------------------------------------------------------------------------
// CGSDisplayPluginAO::NewL
// ----------------------------------------------------------------------------------
CGSDisplayPluginAO* CGSDisplayPluginAO::NewL()
    {
    CGSDisplayPluginAO* self = new( ELeave ) CGSDisplayPluginAO();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;    
    }


// ----------------------------------------------------------------------------------
// CGSDisplayPluginAO::ConstructL
// ----------------------------------------------------------------------------------
void CGSDisplayPluginAO::ConstructL()
    {
    CActiveScheduler::Add ( this );
    iPsmRepository = CRepository::NewL( KCRUidPowerSaveMode );
    // Read from CenRep so iPsmMode gets correct init value
    iPsmRepository->Get( KPsmCurrentMode, iPsmMode );
    iPsmRepository->NotifyRequest( KPsmCurrentMode, iStatus );
    SetActive();
    }


// ----------------------------------------------------------------------------------
// CGSDisplayPluginAO::SetView
// ----------------------------------------------------------------------------------
void CGSDisplayPluginAO::SetView( CGSDisplayPlugin* aDisplayView )
    {
    iDisplayView = aDisplayView;
    }


// ----------------------------------------------------------------------------------
// CGSDisplayPluginAO::Mode
// ----------------------------------------------------------------------------------
TInt CGSDisplayPluginAO::Mode()
    {
    return iPsmMode;
    }


// End of File


