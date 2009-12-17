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
* Description:  GSInstallationsPlugin header.
*
*/


#ifndef GSINSTALLATIONSPLUGIN_H
#define GSINSTALLATIONSPLUGIN_H

// Includes
#include <aknview.h>
#include <eikclb.h>
#include <gsparentplugin.h>
#include <gsfwviewuids.h>

// Classes referenced
class CAknViewAppUi;
class RConeResourceLoader;

// Constants
_LIT( KGSInstallationsPluginResourceFileName, "z:GSInstallationsPluginRsc.rsc" );
_LIT( KGSInstallationsPluginIconDirAndName, "z:GSInstallationsPlugin.mbm"); // Use KDC_BITMAP_DIR

// CLASS DECLARATION

/**
* CGSInstallationsPlugin view class.
*
* @since Series60_3.2
*/
class CGSInstallationsPlugin : public CGSParentPlugin
    {
    public: // Constructors and destructor

        /**
        * Symbian OS two-phased constructor
        * @return
        */
        static CGSInstallationsPlugin* NewL( TAny* aInitParams );

        /**
        * Destructor.
        */
        ~CGSInstallationsPlugin();

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
        CGSInstallationsPlugin();

        /**
        * Symbian OS default constructor.
        */
        void ConstructL();

    private: // Data

        // Resource loader.
        RConeResourceLoader iResourceLoader;
    };


#endif // GSINSTALLATIONSPLUGIN_H
// End of File
