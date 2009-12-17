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
* Description:  Model for GSPDataAccessPointPlugin.
*
*/


// INCLUDE FILES
#include    "GSPDataAccessPointModel.h"
#include    "GsLogger.h"

#include    <commdb.h>
#include    <featmgr.h>
#include    <commdb.h>
#include    <settingsinternalcrkeys.h>


// CONSTANTS

// ================= MEMBER FUNCTIONS =======================


// ----------------------------------------------------------------------------
// CGSPDataAccessPointModel::NewL
//
// EPOC two-phased constructor
// ----------------------------------------------------------------------------
//
CGSPDataAccessPointModel* CGSPDataAccessPointModel::NewL()
    {
    CGSPDataAccessPointModel* self = new( ELeave ) CGSPDataAccessPointModel;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ----------------------------------------------------------------------------
// CGSPDataAccessPointModel::CGSPDataAccessPointModel
//
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CGSPDataAccessPointModel::CGSPDataAccessPointModel()
    {
    __GSLOGSTRING( "[CGSPDataAccessPointModel] CGSPDataAccessPointModel()" );
    }


// ----------------------------------------------------------------------------
// CGSPDataAccessPointModel::ConstructL
//
// EPOC default constructor can leave.
// ----------------------------------------------------------------------------
//
void CGSPDataAccessPointModel::ConstructL()
    {
    iCommsDb = CCommsDatabase::NewL( EDatabaseTypeIAP );
    iNetworkRepository = 
        CRepository::NewL( KCRUidNetworkSettings );
    if( FeatureManager::FeatureSupported( KFeatureIdLoggerGprs ) )
        {
        User::LeaveIfError( iTelServer.Connect() );
        User::LeaveIfError( iPhone.Open( iTelServer, KMmTsyPhoneName ) );
        User::LeaveIfError( iPktService.Open( iPhone ) );
        }
    }


// ----------------------------------------------------------------------------
// CGSPDataAccessPointModel::~CGSPDataAccessPointModel
//
// Destructor
// ----------------------------------------------------------------------------
//
CGSPDataAccessPointModel::~CGSPDataAccessPointModel()
    {
    if ( iNetworkRepository )
        {
        delete iNetworkRepository;
        }
    delete iCommsDb;
    if( FeatureManager::FeatureSupported( KFeatureIdLoggerGprs ) )
        {
        iPktService.Close();
        iPhone.Close();
        iTelServer.Close();
        }
    __GSLOGSTRING( "[CGSPDataConnectionModel] Handles closed." );
    }


// ----------------------------------------------------------------------------
// CGSPDataModel::GetDialupAPNameL
//
// Gets dial-up acces point name.
// ----------------------------------------------------------------------------
//
TInt CGSPDataAccessPointModel::GetDialupAPNameL( TDes& aName )
    {
    CCommsDbTableView* table = iCommsDb->OpenTableLC( TPtrC( DEFAULT_GPRS ) );
    table->GotoFirstRecord();

    //If dialup AP name is not initialized, ReadTextL will leave
    TRAPD( error, table->ReadTextL( TPtrC( GPRS_APN), aName ) );
    CleanupStack::PopAndDestroy( table );
    return error;
    }


// ----------------------------------------------------------------------------
// CGSPDataModel::SetDialupAPNameL
//
// Sets dial-up acces point name.
// ----------------------------------------------------------------------------
//
void CGSPDataAccessPointModel::SetDialupAPNameL( const TDesC& aName )
    {
    CCommsDbTableView* table = iCommsDb->OpenTableLC( TPtrC( DEFAULT_GPRS ) );
    table->GotoFirstRecord();
    User::LeaveIfError( table->UpdateRecord() );
    table->WriteTextL( TPtrC( GPRS_APN ),aName );
    User::LeaveIfError( table->PutRecordChanges() );
    CleanupStack::PopAndDestroy( table );

    if( FeatureManager::FeatureSupported( KFeatureIdLoggerGprs ) )
        {
        TRequestStatus status;
        RPacketContext::TContextConfigGPRS params;
        TPckg< RPacketContext::TContextConfigGPRS > paramsBuf( params );

        if ( aName.Length() <= params.iAccessPointName.MaxLength() )
            {
            params.iAccessPointName.Copy( aName );
            iPktService.SetDefaultContextParams( status, paramsBuf );
            User::WaitForRequest( status );
            }
        }
    }

//  End of File
