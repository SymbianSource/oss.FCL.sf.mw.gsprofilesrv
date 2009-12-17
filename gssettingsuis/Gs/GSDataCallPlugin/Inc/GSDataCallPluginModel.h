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
* Description:  Data Call Settings model.
*
*/


#ifndef GSDATACALLPLUGINMODEL_H
#define GSDATACALLPLUGINMODEL_H

// INCLUDES
#include <cdblen.h>
#include <e32base.h>

// CONSTANTS
// autodisconnect time unlimited
const   TUint   KGSTimeUnlimited = KMaxTUint;
// autodisconnect time 0 minutes
const   TInt    KGSZeroMinutes = 0;
// autodisconnect time 1 minute
const   TInt    KGSOneMinute = 1;

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CCommsDatabase;
class CCommsDbTableView;

// CLASS DEFINITION
/**
*  CGSDataCallPluginModel is the model class of GS display plugin.
*  It provides functions to get and set setting values.
*  @lib GSDisplayPlugin.lib
*  @since Series 60_3.1

*/
class CGSDataCallPluginModel : public CBase
    {
    public:  // Constructor and destructor
        /**
        * Two-phased constructor
        */
        static CGSDataCallPluginModel* NewL();

        /**
        * Destructor
        */
        ~CGSDataCallPluginModel();

    public: // new ones

        /**
        * Returns the period after the data call is disconnected
        * if it has been unused.
        * @return Minutes between 0 and 99.
        */
        TInt AutodisconnectTimeL();

        /**
        * Sets the period after the data call is disconnected
        * if it has been unused.
        * @param aTime Autodisconnect time (minutes).
        * @return ETrue: no errors
        *         EFalse: an error has occurred
        */
        TBool SetAutodisconnectTimeL( const TInt aTime );

    private: // Private constructors
        /**
        * Default C++ contructor
        */
        CGSDataCallPluginModel();

        /**
        * Symbian OS default constructor
        * @return void
        */
        void ConstructL();

    private:
        //handle to CommsDB
        CCommsDatabase* iCommDb;

    };

#endif //GSDATACALLPLUGINMODEL_H

// End of File
