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
* Description:  Model for GSPDataConnectionPlugin.
*
*/


// INCLUDE FILES
#include    "GSPDataConnectionModel.h"
#include    "GsLogger.h"

#include    <commdb.h>
#include    <featmgr.h>
#include    <commdb.h>
#include    <settingsinternalcrkeys.h>


// CONSTANTS
// Default values for some of the settings
const   TUint32 KGSDefaultAttachMode = 0;               // when available

// ================= MEMBER FUNCTIONS =======================


// ----------------------------------------------------------------------------
// CGSPDataConnectionModel::NewL
//
// EPOC two-phased constructor
// ----------------------------------------------------------------------------
//
CGSPDataConnectionModel* CGSPDataConnectionModel::NewL()
    {
    CGSPDataConnectionModel* self = new( ELeave ) CGSPDataConnectionModel;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ----------------------------------------------------------------------------
// CGSPDataConnectionModel::CGSPDataConnectionModel
//
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CGSPDataConnectionModel::CGSPDataConnectionModel()
    {
    iIsFeatureIdLoggerGprs = EFalse;
    __GSLOGSTRING( "[CGSPDataConnectionModel] CGSPDataConnectionModel()" );
    }


// ----------------------------------------------------------------------------
// CGSPDataConnectionModel::ConstructL
//
// EPOC default constructor can leave.
// ----------------------------------------------------------------------------
//
void CGSPDataConnectionModel::ConstructL()
    {
    iCommsDb = CCommsDatabase::NewL( EDatabaseTypeIAP );
    if( FeatureManager::FeatureSupported( KFeatureIdLoggerGprs ) )
        {
        iIsFeatureIdLoggerGprs = ETrue;
        User::LeaveIfError( iTelServer.Connect() );
        User::LeaveIfError( iPhone.Open( iTelServer, KMmTsyPhoneName ) );
        User::LeaveIfError( iPktService.Open( iPhone ) );
        }
    }


// ----------------------------------------------------------------------------
// CGSPDataConnectionModel::~CGSPDataConnectionModel
//
// Destructor
// ----------------------------------------------------------------------------
//
CGSPDataConnectionModel::~CGSPDataConnectionModel()
    {
    delete iCommsDb;
    if( iIsFeatureIdLoggerGprs )
        {
        iPktService.Close();
        iPhone.Close();
        iTelServer.Close();
        }
    __GSLOGSTRING( "[CGSPDataConnectionModel] Handles closed." );
    }


// ----------------------------------------------------------------------------
// CGSPDataConnectionModel::PDataAttachL
//
// Returns packet data attach mode.
// ----------------------------------------------------------------------------
//
TInt CGSPDataConnectionModel::PDataAttachL()
    {
    TUint32 attachMode = KGSDefaultAttachMode;
    TRAPD( error, iCommsDb->GetGlobalSettingL( TPtrC( GPRS_ATTACH_MODE ),
                                               attachMode ) );
    if ( error == KErrNotFound )
        {
        iCommsDb->SetGlobalSettingL( TPtrC( GPRS_ATTACH_MODE ), attachMode );
        }
    else if ( error != KErrNone )
        {
        User::Leave( error );
        }
    return static_cast<TInt> ( attachMode );
    }

// ----------------------------------------------------------------------------
// CGSPDataConnectionModel::SetPDataAttachL
//
// Sets packet data attach mode.
// ----------------------------------------------------------------------------
//
void CGSPDataConnectionModel::SetPDataAttachL( const TInt aAttach )
    {
    iCommsDb->SetGlobalSettingL( TPtrC( GPRS_ATTACH_MODE ), aAttach );

    if( FeatureManager::FeatureSupported( KFeatureIdLoggerGprs ) )
        {
        TRequestStatus status;
        RPacketService::TAttachMode mode(
            static_cast<RPacketService::TAttachMode> ( aAttach ) );
        iPktService.SetAttachMode( status, mode );
        User::WaitForRequest( status );
        }
        
    }  
//  End of File
