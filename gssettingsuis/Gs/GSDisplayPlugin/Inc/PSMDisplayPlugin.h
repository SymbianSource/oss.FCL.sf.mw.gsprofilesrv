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
* Description: 
*
*/


#ifndef PSMDISPLAYPLUGIN_H
#define PSMDISPLAYPLUGIN_H

//  INCLUDES
#include <e32std.h>
#include <e32base.h>

#include <psmpluginbase.h>
#include <centralrepository.h>

class MPsmSettingsProvider;
class MPsmPluginCallback;
class CGSDisplayPluginModel;
class CPowerSaveDisplayMode;

/**
*  PSMDisplay plugin stub
*
*  @lib ?library
*  @since Series ?XX ?SeriesXX_version
*/
class CPSMDisplayPlugin : public CPsmPluginBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
		* @return The created object.
        */
        static CPSMDisplayPlugin* NewL( TPsmPluginCTorParams& initParams );

        /**
        * Destructor.
        */
        virtual ~CPSMDisplayPlugin();

    public:

        // From CPsmPluginBase
        void NotifyModeChange( const TInt aMode );

	private:

	    CPSMDisplayPlugin( TPsmPluginCTorParams& aInitParams );
	    void ConstructL();

        /**
        *  Internal leaving function calleb from non-leaving NotifyModeChange.
        */
        void DoModeChangeL( const TInt aMode );
        
    private: // data
        
        CGSDisplayPluginModel *iModel;

        // For automatic brigthness control
        CPowerSaveDisplayMode *iDisplayModel;
        
    };

#endif // PSMDISPLAYPLUGIN_H
