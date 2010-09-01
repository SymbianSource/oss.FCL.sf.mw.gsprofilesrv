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
* Description:  GSPDataHSDPAPlugin model.
*
*/

#ifndef  GSPDATAHSDPAMODEL_H
#define  GSPDATAHSDPAMODEL_H

// INCLUDES
// INCLUDES
#include    <centralrepository.h>
#include    <e32property.h>
#include    <etel.h>
#include    <etelpckt.h>
#include    <mmtsy_names.h>
#include    <rmmcustomapi.h>
// CONSTANTS

const TInt KSettingHsdpaDisable = 0;
const TInt KSettingHsdpaEnable = 1;


// CLASS DEFINITIONS

class CRfsHandler;
class RMmCustomAPI;
class RMobilePhone;
/**
*  CGSPDataHSDPAModel is the model class of GS app.
*  It provides functions to get and set setting values.
*/
class   CGSPDataHSDPAModel : public CBase
    {
    public:  // Constructor and destructor
        /**
        * Two-phased constructor
        */
        static CGSPDataHSDPAModel* NewL();

        /**
        * Destructor
        */
        ~CGSPDataHSDPAModel();

    public: //new
                /**
        * Returns packet data HSDPA state.
        * @return 0: Disabled
        *         1: Enabled
        */
        TInt CurrentHSDPAItemL();
        
        /**
        * Set packet data HSDPA state.
        * @param ahsdpaValue 0: Disabled
        *                    1: Enabled
        */
        void SetHSDPAItemValueL( TInt aHsdpaValue );
        
                /**
        * Returns packet data HSDPA state.
        * @return 1: Show
        *         0 : Hide
        */
        TInt HSDPAItemVisibilityL();
        
        /**
        * Set packet data HSDPA Visibility.
        * @param aHsdpaVisible 0: Hide
        *                      1: Show
        */
        void SetHSDPAItemVisibilityL( TInt aHsdpaVisible );

    private: // Private constructors
        /**
        * Default C++ contructor
        */
        CGSPDataHSDPAModel();

        /**
        * Symbian OS default constructor
        * @return void
        */
        void ConstructL();

    private: // Data
        // Handle to tel server.
        RTelServer iTelServer;
        
        //Handle to Custom API
        RMmCustomAPI iCustomAPI;
        
        // Handle to Mobile phone.
        RMobilePhone iMobilePhone ;
        
        // Networking repository
        CRepository* iNetworkRepository;

    };
#endif // GSPDATAHSDPAMODEL_H
// End of File
