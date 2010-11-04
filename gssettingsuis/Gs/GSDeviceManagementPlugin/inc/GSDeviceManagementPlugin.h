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
* Description:  GSDeviceManagementPlugin header.
*
*/


#ifndef GSDEVICEMANAGEMENTPLUGIN_H
#define GSDEVICEMANAGEMENTPLUGIN_H

// Includes
#include <aknview.h>
#include <eikclb.h>
#include <gsparentplugin.h>
#include <gsfwviewuids.h>

// Classes referenced
class CAknViewAppUi;
class RConeResourceLoader;

// Constants
_LIT( KGSDeviceManagementPluginResourceFileName, "z:gsdevicemanagementpluginrsc.rsc" );
_LIT( KGSDeviceManagementPluginIconDirAndName, "z:gsdevicemanagementplugin.mbm"); // Use KDC_BITMAP_DIR

// CLASS DECLARATION

/**
* CGSDeviceManagementPlugin view class.
*
* @since Series60_3.2
*/
class CGSDeviceManagementPlugin : public CGSParentPlugin
    {
    public: // Constructors and destructor

        /**
        * Symbian OS two-phased constructor
        * @return
        */
        static CGSDeviceManagementPlugin* NewL( TAny* aInitParams );

        /**
        * Destructor.
        */
        ~CGSDeviceManagementPlugin();

    public: // From CAknView

        /**
        * See base class.
        */
        TUid Id() const;

        /**
        * See base class.
        */
        void DoActivateL( const TVwsViewId& aPrevViewId,
                          TUid aCustomMessageId,
                          const TDesC8& aCustomMessage );
        /**
        * See base class.
        */
        void DoDeactivate();

    public: // From CGSParentPlugin

        /**
        * See base class.
        */
        TUid UpperLevelViewUid();

        /**
        * See base class.
        */
        void GetHelpContext( TCoeHelpContext& aContext );

    public: // From CGSPluginInterface

        /**
        * See base class.
        */
        void GetCaptionL( TDes& aCaption ) const;

        /**
        * See base class.
        */
        CGulIcon* CreateIconL( const TUid aIconType );

        /**
        * See base class.
        */
        TInt PluginProviderCategory() const;

    protected: // New
        /**
        * C++ default constructor.
        */
        CGSDeviceManagementPlugin();

        /**
        * Symbian OS default constructor.
        */
        void ConstructL();

    private: // Data

        // Resource loader.
        RConeResourceLoader iResourceLoader;
    };


#endif // GSDEVICEMANAGEMENTPLUGIN_H
// End of File
