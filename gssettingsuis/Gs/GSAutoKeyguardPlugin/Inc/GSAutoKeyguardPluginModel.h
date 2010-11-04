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


#ifndef GSAUTOKEYGUARDPLUGINMODEL_H
#define GSAUTOKEYGUARDPLUGINMODEL_H

// INCLUDES
#include <e32base.h>
#include <centralrepository.h>

// CONSTANTS
const TInt KGSBufSize128 = 128;

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DEFINITION
/**
*  CGSAutoKeyguardPluginModel is the model class of device & sim security plugin.
*  It provides functions to get and set setting values.
*  @lib GSAutoKeyguardPlugin.lib
*  @since Series 60_5.0

*/
class CGSAutoKeyguardPluginModel : public CBase
    {
    public:  // Constructor and destructor
        /**
        * Two-phased constructor
        */
        static CGSAutoKeyguardPluginModel* NewL();

        /**
        * Destructor
        */
        ~CGSAutoKeyguardPluginModel();
        
    public:
        /**
        * Returns the autokeyguard period time (minutes). 
        *
        * @return TInt: period
        */
        TInt AutoKeyguardPeriod();
        /**
        * Returns the autokeyguard maximum period time (minutes). 
        *
        * @return TInt: period
        */
        TInt AutoKeyguardMaxPeriod();
        /**
        * Sets the autokeyguard period. After this period the keyguard is activated.
        * @param aNewPeriod TInt (minutes)
        *
        * @return ETrue: no errors
        *         EFalse: an error has occurred
        */
        TBool SetAutoKeyguardPeriod(const TInt newPeriod);

    private: // Private constructors
        /**
        * Default C++ contructor
        */
        CGSAutoKeyguardPluginModel();

        /**
        * Symbian OS default constructor
        * @return void
        */
        void ConstructL();
        
    private: // data
        CRepository* iSecurityRepository;
    
    };


#endif //GSAUTOKEYGUARDPLUGINMODEL_H

// End of File
