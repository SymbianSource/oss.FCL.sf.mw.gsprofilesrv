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
* Description:  GSPDataAccessPointPlugin model.
*
*/

#ifndef  GSPDATAACCESSPOINTMODEL_H
#define  GSPDATAACCESSPOINTMODEL_H

// INCLUDES
// INCLUDES
#include    <cdblen.h>
#include    <centralrepository.h>
#include    <e32property.h>
#include    <etel.h>
#include    <etelpckt.h>
#include    <mmtsy_names.h>

// CONSTANTS

const TInt KAttachSettingWhenNeeded = 0;
const TInt KAttachSettingWhenAvailable = 1;
const TInt KGSMaxDAPName = KCommsDbSvrMaxFieldLength; 

// CLASS DEFINITIONS

class CRfsHandler;
class CCommsDatabase;
class CCommsDbTableView;

/**
*  CGSPDataAccessPointModel is the model class of GS app.
*  It provides functions to get and set setting values.
*/
class   CGSPDataAccessPointModel : public CBase
    {
    public:  // Constructor and destructor
        /**
        * Two-phased constructor
        */
        static CGSPDataAccessPointModel* NewL();

        /**
        * Destructor
        */
        ~CGSPDataAccessPointModel();

    public: //new
        /**
        * Gets dialup access point name from commdb.
        * @param aName Descriptor which will contain the dialup access point name.
        *              Max length is KGSMaxDAPName.
        * @return Error code.
        */
        TInt GetDialupAPNameL( TDes& aName );

        /**
        * Stores dialup access point name to commdb.
        * @param aName Descriptor which contains the dialup access point name.
        *              Max length is KGSMaxDAPName.
        */
        void SetDialupAPNameL( const TDesC& aName );

    private: // Private constructors
        /**
        * Default C++ contructor
        */
        CGSPDataAccessPointModel();

        /**
        * Symbian OS default constructor
        * @return void
        */
        void ConstructL();

    private: // Data

        // Handle to Comms database.
        CCommsDatabase* iCommsDb;
        
        // Handle to packet service.
        RPacketService iPktService;
        
        // Handle to phone.
        RPhone iPhone;
        
        // Handle to tel server.
        RTelServer iTelServer;
        
        // Networking repository
        CRepository* iNetworkRepository;

    };


#endif // GSPDATAACCESSPOINTMODEL_H
// End of File
