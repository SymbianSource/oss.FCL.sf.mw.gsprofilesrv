/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  GSAppsPlugin header file.
*
*/


#ifndef GSAPPSPLUGIN_H
#define GSAPPSPLUGIN_H

// Includes
#include <gsparentplugin.h>
#include <gsfwviewuids.h>

#include <aknview.h>
#include <eikclb.h>
#include <eikmenup.h>

// Classes referenced
class CAknViewAppUi;
class RConeResourceLoader;
class CGSTabHelper;

// Constants
_LIT( KGSAppsPluginResourceFileName, "z:GSAppsPluginRsc.rsc" );
_LIT( KGSAppsPluginIconDirAndName, "z:gsappsplugin.mbm"); // Use KDC_BITMAP_DIR

// CLASS DECLARATION

/**
*  Applications Plugin. Displays plugins owned by applications view.
*
*  @since Series60_3.1
*/
class CGSAppsPlugin : public CGSParentPlugin
    {
    public: // Constructors and destructor

        /**
        * Symbian OS two-phased constructor
        * @return
        */
        static CGSAppsPlugin* NewL( TAny* aInitParams );

        /**
        * Destructor.
        */
        ~CGSAppsPlugin();

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
        
    protected: // From MEikMenuObserver

        void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );
        
    protected: // New

        /**
        * C++ default constructor.
        */
        CGSAppsPlugin();

        /**
        * Symbian OS default constructor.
        */
        void ConstructL();

    private: // Data

        // Resource loader.
        RConeResourceLoader iResources;

        // Tab helper.
        CGSTabHelper* iTabHelper;
    };


#endif // GSAPPSPLUGIN_H
// End of File
