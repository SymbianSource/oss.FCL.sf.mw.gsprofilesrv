/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  GSPDataPlugin header.
*
*/


#ifndef GSPDATAPLUGIN_H
#define GSPDATAPLUGIN_H

// Includes
#include <aknview.h>
#include <eikclb.h>
#include <eikmenup.h>
#include <gsparentplugin.h>
#include <gsfwviewuids.h>

// Classes referenced
class CAknViewAppUi;
class RConeResourceLoader;

// Constants
_LIT( KPDataResourceFileName, "z:gspdatapluginrsc.rsc" );
_LIT( KGSPDataPluginIconDirAndName, "z:gspdataplugin.mbm"); // Use KDC_BITMAP_DIR

// CLASS DECLARATION

/**
* CGSPDataPlugin view class.
*
* @since Series60_3.1
*/
class CGSPDataPlugin : public CGSParentPlugin
    {
    public: // Constructors and destructor

        /**
        * Symbian OS two-phased constructor
        * @return
        */
        static CGSPDataPlugin* NewL( TAny* aInitParams );

        /**
        * Destructor.
        */
        ~CGSPDataPlugin();

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
        
        TAny* CustomOperationL( TAny* aParam1, TAny* aParam2 );
        
    protected: // From MEikMenuObserver

        void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );

    protected: // New
        /**
        * C++ default constructor.
        */
        CGSPDataPlugin();

        /**
        * Symbian OS default constructor.
        */
        void ConstructL();

    private: // Data

        // Resource loader.
        RConeResourceLoader iResourceLoader;
        
        // Indicates view active status
        TBool iViewActive;

    };


#endif // GSPDATAPLUGIN_H
// End of File
