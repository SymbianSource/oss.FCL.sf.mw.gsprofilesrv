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
* Description:  GSPDataConnectionPlugin model.
*
*/

#ifndef  GSPDATACONNECTIONMODEL_H
#define  GSPDATACONNECTIONMODEL_H

// INCLUDES
// INCLUDES
#include    <centralrepository.h>
#include    <e32property.h>
#include    <etel.h>
#include    <etelpckt.h>
#include    <mmtsy_names.h>

// CONSTANTS

const TInt KAttachSettingWhenNeeded = 1;
const TInt KAttachSettingWhenAvailable = 0;


// CLASS DEFINITIONS

class CRfsHandler;
class CCommsDatabase;
class CCommsDbTableView;


/**
*  CGSPDataConnectionModel is the model class of GS app.
*  It provides functions to get and set setting values.
*/
class   CGSPDataConnectionModel : public CBase
    {
    public:  // Constructor and destructor
        /**
        * Two-phased constructor
        */
        static CGSPDataConnectionModel* NewL();

        /**
        * Destructor
        */
        ~CGSPDataConnectionModel();

    public: //new

        /**
        * Gets AI state.
        */
        TInt PDataAttachL();

        /**
        * Sets AI state.
        */
        void SetPDataAttachL( const TInt aAttach );


    private: // Private constructors
        /**
        * Default C++ contructor
        */
        CGSPDataConnectionModel();

        /**
        * Symbian OS default constructor
        * @return void
        */
        void ConstructL();

    private: // Data

        // Flag to indicate whether the feature KFeatureIdLoggerGprs is supported
        TBool iIsFeatureIdLoggerGprs;

        // Handle to Comms database.
        CCommsDatabase* iCommsDb;
        
        // Handle to packet service.
        RPacketService iPktService;
        
        // Handle to phone.
        RPhone iPhone;
        
        // Handle to tel server.
        RTelServer iTelServer;

    };
#endif // GSPDATACONNECTIONMODEL_H

// End of File
