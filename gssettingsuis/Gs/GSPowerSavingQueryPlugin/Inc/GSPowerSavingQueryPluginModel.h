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


#ifndef GSPOWERSAVINGQUERYPLUGINMODEL_H
#define GSPOWERSAVINGQUERYPLUGINMODEL_H

// INCLUDES
#include <e32base.h>

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CRepository;
// CLASS DEFINITION
/**
*  CGSPowerSavingQueryPluginModel is the model class of Power saving query plugin.
*  It provides functions to get and set setting values.
*  @lib GSPowerSavingQueryPlugin.lib
*  @since Series 60_5.1

*/

class CGSPowerSavingQueryPluginModel : public CBase
    {
    public:  // Constructor and destructor
        /**
        * Two-phased constructor
        */
        static CGSPowerSavingQueryPluginModel* NewL();

        /**
        * Destructor
        */
        ~CGSPowerSavingQueryPluginModel();
        
    public:
        /**
        * Returns the power saving query value (0 or 1) 
        * 
        * @return TInt
        */
        TInt PowerSavingQueryEnabled();

        /**
        * Sets the the power saving query value.
        * @param aQuery TInt (0 or 1)
        *
        * @return ETrue: no errors
        *         EFalse: an error has occurred
        */
        TBool EnablePowerSavingQuery( TInt aQueryValue );

    private: // Private constructors
        /**
        * Default C++ contructor
        */
        CGSPowerSavingQueryPluginModel();

        /**
        * Symbian OS default constructor
        * @return void
        */
        void ConstructL();
        
    private: // data
        CRepository* iDeviceManagementRepository;
    
    };


#endif //GSPOWERSAVINGQUERYPLUGINMODEL_H

// End of File
