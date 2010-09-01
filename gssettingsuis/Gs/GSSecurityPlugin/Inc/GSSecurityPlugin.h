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
* Description:  Container for the Security sub-folder
*
*/


#ifndef GSSECURITYPLUGIN_H
#define GSSECURITYPLUGIN_H

// Includes
#include <aknview.h>
#include <eikclb.h>
#include <eikmenup.h>
#include <gsparentplugin.h>
#include <gsfwviewuids.h>

// Classes referenced
class CAknViewAppUi;
class RConeResourceLoader;


// CLASS DECLARATION

/**
* CGSSecurityPlugin view class.
*
* @since Series60_3.1
*/
class CGSSecurityPlugin : public CGSParentPlugin
    {
    public: // Constructors and destructor

        /**
        * Symbian OS two-phased constructor
        * @return
        */
        static CGSSecurityPlugin* NewL( TAny* aInitParams );

        /**
        * Destructor.
        */
        ~CGSSecurityPlugin();

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
        TInt PluginProviderCategory() const;

        /**
        * @see CGSPluginInterface header file.
        */
        CGulIcon* CreateIconL( const TUid aIconType );
        
    protected: // From MEikMenuObserver

        void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );

    protected: // New
        /**
        * C++ default constructor.
        */
        CGSSecurityPlugin();

        /**
        * Symbian OS default constructor.
        */
        void ConstructL();

    private: // Data

        // Resource loader.
        RConeResourceLoader iResourceLoader;

    };

#endif //GSSECURITYPLUGIN_H

// End of File
