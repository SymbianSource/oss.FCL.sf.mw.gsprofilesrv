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
* Description:  GSSynchronisationPlugin header.
*
*/


#ifndef GSSYNCHRONISATIONPLUGIN_H
#define GSSYNCHRONISATIONPLUGIN_H

// Includes
#include <aknview.h>
#include <eikclb.h>
#include <gsparentplugin.h>
#include <gsfwviewuids.h>

// Classes referenced
class CAknViewAppUi;
class RConeResourceLoader;
class CGSTabHelper;

// Constants
_LIT( KGSSynchronisationPluginResourceFileName, "z:gssynchronisationpluginrsc.rsc" );
_LIT( KGSSynchronisationPluginIconDirAndName, "z:gssynchronisationplugin.mbm"); // Use KDC_BITMAP_DIR

// CLASS DECLARATION

/**
* CGSSynchronisationPlugin view class.
*
* @since Series60_3.2
*/
class CGSSynchronisationPlugin : public CGSParentPlugin
    {
    public: // Constructors and destructor

        /**
        * Symbian OS two-phased constructor
        * @return
        */
        static CGSSynchronisationPlugin* NewL( TAny* aInitParams );

        /**
        * Destructor.
        */
        ~CGSSynchronisationPlugin();

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
        CGSSynchronisationPlugin();

        /**
        * Symbian OS default constructor.
        */
        void ConstructL();

    private: // Data

        // Resource loader.
        RConeResourceLoader iResourceLoader;
    };


#endif // GSSYNCHRONISATIONPLUGIN_H
// End of File
