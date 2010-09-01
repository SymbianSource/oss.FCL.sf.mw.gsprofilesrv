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
* Description:  Active object for handling PSM related CenRep changes.
*
*/


#ifndef GSDISPLAYPLUGINAO_H
#define GSDISPLAYPLUGINAO_H

// INCLUDES
#include <psmtypes.h>

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD CLASS DECLARATION

// CLASS DECLARATION

/**
*  CGSDisplayPluginAO. Acts as a CenRep observer for PSM related state 
*  changes.
*
*  @lib GSDisplayPlugin.lib
*  @since Series 60_3.2
*/
class CGSDisplayPluginAO : public CActive
    {
    public:
    
        static CGSDisplayPluginAO* NewL();
        ~CGSDisplayPluginAO();
    
        /**
         * Set view which should be updated when PSM mode changes.
         */
        void SetView( CGSDisplayPlugin* aDisplayView );
        
        /**
         * @return PSM mode. See TPsmsrvMode.
         */
        TInt Mode();
        
    protected: // From CActive

        void DoCancel();
        void RunL();
        TInt RunError( TInt aError );
        
    private:

        CGSDisplayPluginAO();
        void ConstructL();

    private: // Data
        
        CRepository* iPsmRepository;
        TInt iPsmMode;
        CGSDisplayPlugin* iDisplayView;
    };


#endif // GSDISPLAYPLUGINAO_H

//End of File
