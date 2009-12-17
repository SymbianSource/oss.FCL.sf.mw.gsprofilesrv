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
* Description:    Header file for plugin.
*
*/


#ifndef GSPRSLNPLUGIN_H
#define GSPRSLNPLUGIN_H

// Includes
#include <GSParentPlugin.h>
#include <GSFWViewUIDs.h>

#include <aknview.h>
#include <eikclb.h>
#include <eikmenup.h>


// Classes referenced
class CAknViewAppUi;
class RConeResourceLoader;
class CGSTabHelper;

// Constants
_LIT( KGSPrslnPluginResourceFileName, "z:GSPrslnPluginRsc.rsc" );
_LIT( KGSPrslnPluginIconDirAndName, "z:gsprslnplugin.mbm"); // Use KDC_BITMAP_DIR

// CLASS DECLARATION

/**
* CGSPrslnPlugin view class.
*
* @since Series60_3.1
*/
class CGSPrslnPlugin : public CGSParentPlugin
    {
    public: // Constructors and destructor

        /**
        * Symbian OS two-phased constructor
        * @return
        */
        static CGSPrslnPlugin* NewL( TAny* aInitParams );

        /**
        * Destructor.
        */
        ~CGSPrslnPlugin();

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
        TGSListboxTypes ListBoxType();
        
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
        CGSPrslnPlugin();

        /**
        * Symbian OS default constructor.
        */
        void ConstructL();

    private: // Data

        // Resource loader.
        RConeResourceLoader iResourceLoader;

        // Tab hepler.
        CGSTabHelper* iTabHelper;
        
        //Indicates if cba is already constructed
        TBool iCbaConstructed;
    };


#endif // GSPRSLNPLUGIN_H
// End of File
