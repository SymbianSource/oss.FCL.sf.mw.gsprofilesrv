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
* Description:  Model for GSPDataHSDPAPlugin.
*
*/


// INCLUDE FILES
#include    "GSPDataHSDPAModel.h"
#include    "GsLogger.h"

#include    <featmgr.h>
#include    <commdb.h>
#include    <settingsinternalcrkeys.h>


// CONSTANTS

// ================= MEMBER FUNCTIONS =======================


// ----------------------------------------------------------------------------
// CGSPDataHSDPAModel::NewL
//
// EPOC two-phased constructor
// ----------------------------------------------------------------------------
//
CGSPDataHSDPAModel* CGSPDataHSDPAModel::NewL()
    {
    CGSPDataHSDPAModel* self = new( ELeave ) CGSPDataHSDPAModel;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ----------------------------------------------------------------------------
// CGSPDataHSDPAModel::CGSPDataHSDPAModel
//
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CGSPDataHSDPAModel::CGSPDataHSDPAModel()
    {
    __GSLOGSTRING( "[CGSPDataHSDPAModel] CGSPDataHSDPAModel()" );
    }


// ----------------------------------------------------------------------------
// CGSPDataHSDPAModel::ConstructL
//
// EPOC default constructor can leave.
// ----------------------------------------------------------------------------
//
void CGSPDataHSDPAModel::ConstructL()
    {
    if( FeatureManager::FeatureSupported( KFeatureIdLoggerGprs ) )
        {
        User::LeaveIfError( iTelServer.Connect() );
        User::LeaveIfError( iMobilePhone.Open( iTelServer, KMmTsyPhoneName ) );
        User::LeaveIfError( iCustomAPI.Open( iMobilePhone ) );
        }
    }


// ----------------------------------------------------------------------------
// CGSPDataHSDPAModel::~CGSPDataHSDPAModel
//
// Destructor
// ----------------------------------------------------------------------------
//
CGSPDataHSDPAModel::~CGSPDataHSDPAModel()
    {
    if( FeatureManager::FeatureSupported( KFeatureIdLoggerGprs ) )
        {
        iCustomAPI.Close();
        iMobilePhone.Close();
        iTelServer.Close();
        }
    __GSLOGSTRING( "[CGSPDataConnectionModel] Handles closed." );
    }

// ----------------------------------------------------------------------------
// CGSPDataModel::CurrentHSDPAItemL
//
// Returns 0( Disabled ) /1 (Enabled) .
// ----------------------------------------------------------------------------
//
TInt CGSPDataHSDPAModel::CurrentHSDPAItemL()
    {
    TInt hsdpaValue = EFalse;
    TRequestStatus status;
    RMmCustomAPI::THSxPAStatus hSxPAStatus;
    iCustomAPI.ReadHSxPAStatus( status,hSxPAStatus );
    User::WaitForRequest( status );
    if ( hSxPAStatus == RMmCustomAPI::EHSxPADisabled )
        {
        hsdpaValue = EFalse; // HSDPA Disable
        }
    else if ( hSxPAStatus == RMmCustomAPI::EHSxPAEnabled )
        {
        hsdpaValue = ETrue; //HSDPA Enable
        }
    return hsdpaValue;
    }

// ----------------------------------------------------------------------------
// CGSPDataModel::CurrentHSDPAItemL
//
// Set 0( Disabled ) /1 (Enabled) .
// ----------------------------------------------------------------------------
//    
void CGSPDataHSDPAModel::SetHSDPAItemValueL( TInt aHsdpaValue )
    {
    TRequestStatus status;
    RMmCustomAPI::THSxPAStatus hSxPAStatus;
    if ( aHsdpaValue == 0 )
        {
    	hSxPAStatus = RMmCustomAPI::EHSxPADisabled;
    	}
    else
        {
        hSxPAStatus = RMmCustomAPI::EHSxPAEnabled;
        }
    iCustomAPI.WriteHSxPAStatus( status,hSxPAStatus );
    User::WaitForRequest( status );
    }

//  End of File
