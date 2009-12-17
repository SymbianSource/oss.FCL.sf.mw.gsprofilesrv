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


// INCLUDE FILES
#include "GSNetworkPlugin.h"
#include "GSNetworkPluginAO.h"
#include "GsLogger.h"
#include <psmsrvdomaincrkeys.h>

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES

// CONSTANTS

// ----------------------------------------------------------------------------------
// CGSNetworkPluginAO::CGSNetworkPluginAO
// ----------------------------------------------------------------------------------
CGSNetworkPluginAO::CGSNetworkPluginAO() :
    CActive( EPriorityStandard ),
    iPsmRepository( NULL ),
    iPsmMode( -1 ),
    iNetworkPluginView( NULL )

    {
    
    }


// ----------------------------------------------------------------------------------
// CGSNetworkPluginAO::~CGSNetworkPluginAO
// ----------------------------------------------------------------------------------
CGSNetworkPluginAO::~CGSNetworkPluginAO()
    {
    Cancel();
    delete iPsmRepository;
    iPsmRepository = NULL;
    }


// ----------------------------------------------------------------------------------
// CGSNetworkPluginAO::DoCancel
// ----------------------------------------------------------------------------------
void CGSNetworkPluginAO::DoCancel()
    {
    iPsmRepository->NotifyCancel( KPsmCurrentMode );
    }

// ----------------------------------------------------------------------------------
// CGSNetworkPluginAO::RunL
// ----------------------------------------------------------------------------------
void CGSNetworkPluginAO::RunL()
    {
    // Request complete, notify request again and get new value
    iPsmRepository->Get( KPsmCurrentMode, iPsmMode );
    iPsmRepository->NotifyRequest( KPsmCurrentMode, iStatus );
    
    SetActive();
    
    __GSLOGSTRING1( "[CGSNetworkPluginAO::RunL] iPsmMode:%d", iPsmMode );

    // Update network plugin view when PSM mode has changed
    if ( iNetworkPluginView )
        {
        iNetworkPluginView->UpdateOnPsmChanged();
        }
    }

// ----------------------------------------------------------------------------------
// CGSNetworkPluginAO::RunError
// ----------------------------------------------------------------------------------
TInt CGSNetworkPluginAO::RunError( TInt /*aError*/ )
    {
    return KErrNone;
    }


// ----------------------------------------------------------------------------------
// CGSNetworkPluginAO::NewL
// ----------------------------------------------------------------------------------
CGSNetworkPluginAO* CGSNetworkPluginAO::NewL()
    {
    CGSNetworkPluginAO* self = new( ELeave ) CGSNetworkPluginAO();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;    
    }


// ----------------------------------------------------------------------------------
// CGSNetworkPluginAO::ConstructL
// ----------------------------------------------------------------------------------
void CGSNetworkPluginAO::ConstructL()
    {
    CActiveScheduler::Add ( this );
    iPsmRepository = CRepository::NewL( KCRUidPowerSaveMode );
    // Read from CenRep so iPsmMode gets correct init value
    iPsmRepository->Get( KPsmCurrentMode, iPsmMode );
    iPsmRepository->NotifyRequest( KPsmCurrentMode, iStatus );
    
    SetActive();
    }


// ----------------------------------------------------------------------------------
// CGSNetworkPluginAO::SetView
// ----------------------------------------------------------------------------------
void CGSNetworkPluginAO::SetView( CGSNetworkPlugin* aNetworkPluginView )
    {
    iNetworkPluginView = aNetworkPluginView;
    }


// ----------------------------------------------------------------------------------
// CGSNetworkPluginAO::Mode
// ----------------------------------------------------------------------------------
TInt CGSNetworkPluginAO::Mode() const
    {
    return iPsmMode;
    }

// End of File


