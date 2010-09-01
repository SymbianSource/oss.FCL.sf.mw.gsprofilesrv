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
* Description:  Active object for handling PSM related CenRep changes.
*
*/


#ifndef GSNETWORKPLUGINAO_H
#define GSNETWORKPLUGINAO_H

// INCLUDES
#include <psmtypes.h>

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD CLASS DECLARATION

// CLASS DECLARATION

/**
*  CGSNetworkPluginAO. Acts as a CenRep observer for PSM related state 
*  changes.
*
*  @lib GSNetworkPlugin.lib
*  @since Series 60_5.1
*/
class CRepository;
class CGSNetworkPlugin;

class CGSNetworkPluginAO : public CActive
    {
    public:
    
        static CGSNetworkPluginAO* NewL();
        ~CGSNetworkPluginAO();
    
        /**
         * Set view which should be updated when PSM mode changes.
         */
        void SetView( CGSNetworkPlugin* aNetworkPluginView );
        
        /**
         * @return PSM mode. See TPsmsrvMode.
         */
        TInt Mode() const;
        
    protected: // From CActive

        void DoCancel();
        void RunL();
        TInt RunError( TInt aError );
        
    private:

        CGSNetworkPluginAO();
        void ConstructL();

    private: // Data
        
        CRepository* iPsmRepository;
        TInt iPsmMode;
        CGSNetworkPlugin* iNetworkPluginView;
    };


#endif // GSNETWORKPLUGINAO_H

//End of File
