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
* Description:  Model for Power saving query plugin.
*
*/


// INCLUDES
#include "GSPowerSavingQueryPluginModel.h"
#include <settingsinternalcrkeys.h>
#include <centralrepository.h>
#include "GSPowerSavingQueryPlugin.hrh"
#include "GsLogger.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// ============================= LOCAL FUNCTIONS ==============================

// ========================= MEMBER FUNCTIONS =================================

// ----------------------------------------------------------------------------
// CGSPowerSavingQueryPluginModel::NewL
// 
// Symbian OS two-phased constructor
// ----------------------------------------------------------------------------
//
CGSPowerSavingQueryPluginModel* CGSPowerSavingQueryPluginModel::NewL()
    {
    CGSPowerSavingQueryPluginModel* self = 
                   new( ELeave ) CGSPowerSavingQueryPluginModel();
    
    CleanupStack::PushL( self );
    self->ConstructL();

    CleanupStack::Pop( self );
    return self;
    }


// ----------------------------------------------------------------------------
// CGSPowerSavingQueryPluginModel::CGSPowerSavingQueryPluginModel
// 
// 
// C++ default constructor can NOT contain any code, that might leave.
// ----------------------------------------------------------------------------
//
CGSPowerSavingQueryPluginModel::CGSPowerSavingQueryPluginModel()
    {
    
    }


// ----------------------------------------------------------------------------
// CGSPowerSavingQueryPluginModel::ConstructL
// 
// Symbian OS default constructor can leave.
// ----------------------------------------------------------------------------
//
void CGSPowerSavingQueryPluginModel::ConstructL()
    {
    __GSLOGSTRING("[CGSPowerSavingQueryPluginModel]-->CGSPowerSavingQueryPluginModel::ConstructL");
    
    iDeviceManagementRepository = 
               CRepository::NewL( KCRUidDeviceManagementSettings );
    
    __GSLOGSTRING("[CGSPowerSavingQueryPluginModel]<--CGSPowerSavingQueryPluginModel::ConstructL");
    }


// ----------------------------------------------------------------------------
// CGSPowerSavingQueryPluginModel::~CGSPowerSavingQueryPluginModel
// 
// Destructor
// ----------------------------------------------------------------------------
//
CGSPowerSavingQueryPluginModel::~CGSPowerSavingQueryPluginModel()
    {
    delete iDeviceManagementRepository;
    }

// ----------------------------------------------------------------------------
// CGSPowerSavingQueryPluginModel::PowerSavingQueryEnabled();
// 
// Reads power saving query value from central repository
// ----------------------------------------------------------------------------
//
TInt CGSPowerSavingQueryPluginModel::PowerSavingQueryEnabled()
    {
    TInt queryValue( EGSPowerSavingQueryOff );
    iDeviceManagementRepository->Get( KSettingsPowerSavingQuery, queryValue );
    return queryValue;
    }


// ----------------------------------------------------------------------------
// CGSPowerSavingQueryPluginModel::EnablePowerSavingQuery
// 
// Writes power saving query value to central repository
// ----------------------------------------------------------------------------
//
TBool CGSPowerSavingQueryPluginModel::EnablePowerSavingQuery(  TInt aQueryValue )
    {
    return iDeviceManagementRepository->Set( KSettingsPowerSavingQuery, 
              aQueryValue );    
    }
  
// End of File
