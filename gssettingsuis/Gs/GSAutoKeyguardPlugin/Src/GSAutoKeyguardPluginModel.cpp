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
* Description:  Model for Device & SIM security plug-in.
*
*/


// INCLUDES
#include "GSAutoKeyguardPluginModel.h"

#include <settingsinternalcrkeys.h>

#include <bldvariant.hrh>
// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS
const TInt KGSSettingOff = 0;
const TInt KGSDefaultAutoKeyguardTime = 60;
const TInt KGSUseDefaultMaxAutoKeyguardTime = 0;

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// ============================= LOCAL FUNCTIONS ==============================

// ========================= MEMBER FUNCTIONS =================================

// ----------------------------------------------------------------------------
// CGSAutoKeyguardPluginModel::NewL
// 
// Symbian OS two-phased constructor
// ----------------------------------------------------------------------------
//
CGSAutoKeyguardPluginModel* CGSAutoKeyguardPluginModel::NewL()
    {
    CGSAutoKeyguardPluginModel* self = new( ELeave ) CGSAutoKeyguardPluginModel;
    CleanupStack::PushL( self );
    self->ConstructL();

    CleanupStack::Pop( self );
    return self;
    }


// ----------------------------------------------------------------------------
// CGSAutoKeyguardPluginModel::CGSAutoKeyguardPluginModel
// 
// 
// C++ default constructor can NOT contain any code, that might leave.
// ----------------------------------------------------------------------------
//
CGSAutoKeyguardPluginModel::CGSAutoKeyguardPluginModel()
    {
    
    }


// ----------------------------------------------------------------------------
// CGSAutoKeyguardPluginModel::ConstructL
// 
// Symbian OS default constructor can leave.
// ----------------------------------------------------------------------------
//
void CGSAutoKeyguardPluginModel::ConstructL()
    {
    iSecurityRepository = CRepository::NewL( KCRUidSecuritySettings );
    }


// ----------------------------------------------------------------------------
// CGSAutoKeyguardPluginModel::~CGSAutoKeyguardPluginModel
// 
// Destructor
// ----------------------------------------------------------------------------
//
CGSAutoKeyguardPluginModel::~CGSAutoKeyguardPluginModel()
    {
    if(iSecurityRepository)
        {
            delete iSecurityRepository;
            iSecurityRepository = NULL;
        }
    
    }

// ----------------------------------------------------------------------------
// CGSAutoKeyguardPluginModel::AutoKeyguardPeriod();
// 
// Reads Autolock period from .ini file and returns it
// ----------------------------------------------------------------------------
//
TInt CGSAutoKeyguardPluginModel::AutoKeyguardPeriod()
    {
    TInt period = KGSSettingOff;
    
    if ( iSecurityRepository->
         Get( KSettingsAutomaticKeyguardTime, period ) != KErrNone )
            {
            period = KGSDefaultAutoKeyguardTime;
            iSecurityRepository->Set( KSettingsAutomaticKeyguardTime, period );
            }
    
    return period;
    }

// ----------------------------------------------------------------------------
// CGSAutoKeyguardPluginModel::AutoKeyguardMaxPeriod();
// 
// Reads Autokeyguard period maximum allowed value from .ini file and returns it
// ----------------------------------------------------------------------------
//
TInt CGSAutoKeyguardPluginModel::AutoKeyguardMaxPeriod()
    {
    TInt maxPeriod(600);
    
    if ( iSecurityRepository->
         Get( KSettingsMaxAutomaticKeyguardTime, maxPeriod ) != KErrNone )
            {
            maxPeriod = KGSUseDefaultMaxAutoKeyguardTime;
            }
    return maxPeriod;
    }
            

// ----------------------------------------------------------------------------
// CGSAutoKeyguardPluginModel::SetAutoKeyguardPeriod
// 
// Writes Autokeyguard period time to Cenrep
// ----------------------------------------------------------------------------
//
TBool CGSAutoKeyguardPluginModel::SetAutoKeyguardPeriod( TInt aLockTime )
    {
    TInt ret = iSecurityRepository->Set( KSettingsAutomaticKeyguardTime, aLockTime );    
    return ret;
    }
  
// End of File
