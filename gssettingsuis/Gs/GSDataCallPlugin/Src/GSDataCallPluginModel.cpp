/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Data Call Settings model implementation.
*
*/


// INCLUDE FILES
#include "GSDataCallPluginModel.h"

#include <commdb.h>
#include <cdbcols.h>

#include <settingsinternalcrkeys.h>

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS
// Autodisconnect time
const TUint8 KGSOneSecond = 1;
const TUint8 KGSSecondsInMinute = 60;
const TUint  KGSMaxTimeInSeconds = 5940;
//const TUint8 KGSMaxTimeInMinutes = 99;

_LIT( KGSCSDModem, "CSD Modem" );

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ==============================

// ========================= MEMBER FUNCTIONS =================================

// ----------------------------------------------------------------------------
// CGSDataCallPluginModel::NewL
//
// Symbian OS two-phased constructor
// ----------------------------------------------------------------------------
//
CGSDataCallPluginModel* CGSDataCallPluginModel::NewL()
    {
    CGSDataCallPluginModel* self = new( ELeave ) CGSDataCallPluginModel;
    CleanupStack::PushL( self );
    self->ConstructL();

    CleanupStack::Pop( self );
    return self;
    }


// ----------------------------------------------------------------------------
// CGSDataCallPluginModel::CGSDataCallPluginModel
//
//
// C++ default constructor can NOT contain any code, that might leave.
// ----------------------------------------------------------------------------
//
CGSDataCallPluginModel::CGSDataCallPluginModel()
    {
    }


// ----------------------------------------------------------------------------
// CGSDataCallPluginModel::ConstructL
//
// Symbian OS default constructor can leave.
// ----------------------------------------------------------------------------
//
void CGSDataCallPluginModel::ConstructL()
    {
    iCommDb = CCommsDatabase::NewL( EDatabaseTypeIAP );
    }


// ----------------------------------------------------------------------------
// CGSDataCallPluginModel::~CGSDataCallPluginModel
//
// Destructor
// ----------------------------------------------------------------------------
//
CGSDataCallPluginModel::~CGSDataCallPluginModel()
    {
    delete iCommDb;
    }


// ----------------------------------------------------------------------------
// CGSDataCallPluginModel::AutodisconnectTimeL
//
// Gets autodisconnect time.
// ----------------------------------------------------------------------------
//
TInt CGSDataCallPluginModel::AutodisconnectTimeL()
    {
    CCommsDbTableView* table = iCommDb->OpenViewMatchingTextLC(
        TPtrC( MODEM_BEARER ),
        TPtrC( COMMDB_NAME ), KGSCSDModem );

    User::LeaveIfError(  table->GotoFirstRecord() );

    TUint32 timeOutValue = 0;
    table->ReadUintL( TPtrC( LAST_SOCKET_ACTIVITY_TIMEOUT ), timeOutValue );
    CleanupStack::PopAndDestroy( table );

    TInt timeInMinutes;

    if ( timeOutValue < KGSOneSecond )
        {
        timeInMinutes = static_cast<TInt> ( KGSTimeUnlimited );

        if ( timeOutValue != KGSTimeUnlimited )
            {
            SetAutodisconnectTimeL( KGSTimeUnlimited );
            }
        }
    else if ( timeOutValue < KGSSecondsInMinute )
        {
        timeInMinutes = KGSOneMinute;
        }
    else if ( timeOutValue < KGSMaxTimeInSeconds )
        {
        timeInMinutes = timeOutValue/KGSSecondsInMinute;
        }
    else
        {
        timeInMinutes = static_cast<TInt> ( KGSTimeUnlimited );
        }
    return  timeInMinutes;
    }


// ----------------------------------------------------------------------------
// CGSDataCallPluginModel::SetAutodisconnectTimeL
//
// Sets autodisconnect time.
// ----------------------------------------------------------------------------
//
TBool CGSDataCallPluginModel::SetAutodisconnectTimeL( const TInt aTime )
    {
    TInt time;

    if ( aTime >= KGSOneMinute )
        {
        time = aTime * KGSSecondsInMinute;
        }
    else
        {
        time = static_cast<TInt> ( KGSTimeUnlimited );
        }

    CCommsDbTableView* table = iCommDb->OpenViewMatchingTextLC(
        TPtrC( MODEM_BEARER ),
        TPtrC( COMMDB_NAME ), KGSCSDModem );

    User::LeaveIfError( table->GotoFirstRecord() );
    User::LeaveIfError( table->UpdateRecord() );
    table->WriteUintL( TPtrC( LAST_SOCKET_ACTIVITY_TIMEOUT ), time );
    table->WriteUintL( TPtrC( LAST_SOCKET_CLOSED_TIMEOUT ), time );
    User::LeaveIfError( table->PutRecordChanges() );
    CleanupStack::PopAndDestroy( table );

    return ETrue;
    }


// ========================== OTHER EXPORTED FUNCTIONS =========================

// End of File
