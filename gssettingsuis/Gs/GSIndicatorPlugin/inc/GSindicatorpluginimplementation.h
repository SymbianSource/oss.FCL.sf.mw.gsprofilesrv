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
* Description:  GSIndicatorplugin plugin implementation
*
*/



#ifndef CGSINDICATORPLUGINIMPLEMENTATION_H
#define CGSINDICATORPLUGINIMPLEMENTATION_H


//  INCLUDES
#include <AknIndicatorPlugin.h> // CAknIndicatorPlugin

/**
*  GSIndicatorplugin. When user taps signal pane in indicator pop-up pane
*  this component will open general settings.
*
*  @lib gsindicatorplugin.lib
*  @since 5.0
*/
class CGSIndicatorPluginImplementation : public CAknIndicatorPlugin                                        
    {
        
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CGSIndicatorPluginImplementation* NewL();

        /**
        * Destructor.
        */
        ~CGSIndicatorPluginImplementation();
    public:
    	static TInt Tick(TAny* aObject);

    private: // From CAknIndicatorPlugin
        
        /**
         * @see CAknIndicatorPlugin
         */
        void HandleIndicatorTapL( const TInt /*aUid */);
        
    private:

        /**
        * C++ default constructor.
        */
        CGSIndicatorPluginImplementation();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private: // new functions
    	TInt DoTick();


    private: // data
    	CPeriodic* iTimer;
    };

#endif      // CGSINDICATORPLUGINIMPLEMENTATION_H

// End of File
