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
* Description: 
*
*/


#ifndef PSMNETWORKPLUGIN_H
#define PSMNETWORKPLUGIN_H

//  INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <psmpluginbase.h> //CPsmPluginBase

class CGSNetworkPluginModel;

/**
*  PSMNetwork plugin stub
*
*  @lib ?library
*  @since Series 60_5.1
*/
class CPSMNetworkPlugin : public CPsmPluginBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
		* @return The created object.
        */
        static CPSMNetworkPlugin* NewL( TPsmPluginCTorParams& initParams );

        /**
        * Destructor.
        */
        virtual ~CPSMNetworkPlugin();

    public:

        // From CPsmPluginBase
        void NotifyModeChange( const TInt aMode );

	private:

	    CPSMNetworkPlugin( TPsmPluginCTorParams& aInitParams );
	    void ConstructL();

        /**
        *  Internal leaving function calleb from non-leaving NotifyModeChange.
        */
        void DoModeChangeL( const TInt aMode );
        
        /**
        * To verify if the phone is in the offline mode. (or mostly if mode is supported???)
        */
        TBool IsPhoneOfflineL() const;
        
    private: // data

        CGSNetworkPluginModel *iModel;        //data model

    };

#endif // PSMNETWORKPLUGIN_H
