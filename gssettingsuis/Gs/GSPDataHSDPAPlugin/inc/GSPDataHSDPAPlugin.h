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
* Description:  GSPDataHSDPAPlugin implementation.
*
*/

#ifndef GSPDATAHSDPAPLUGIN_H
#define GSPDATAHSDPAPLUGIN_H

// User includes

// System includes
#include <gsplugininterface.h>
#include <aknview.h>
#include <ConeResLoader.h>
#include <eikclb.h>
#include <gspubsubslistener.h>
#include <mgssettingpsobserver.h>
#include <MProfileEngine.h>
#include <MProEngProfileActivationObserver.h>
#include <MProEngNotifyHandler.h>

// Classes referenced
class CAknNavigationDecorator;
class CAknViewAppUi;
class CAknWaitDialog;
class CGSPDataHSDPAModel;

// Constants
const TUid KGSPDataHSDPAPluginUID   = { 0x10282DC8 };
_LIT( KPDataHSDPAResourceFileName, "z:gspdatahsdpapluginrsc.rsc" );

// CLASS DECLARATION

/**
*  CGSPDataHSDPAPlugin view class.
*
* This class handles state and application logic of PDataHSDPA settings.
* The plugin is a type of EGSItemTypeSettingDialog and therefore the GS FW will
* call HandleSelection() instead of DoActivate().
*
*/
class CGSPDataHSDPAPlugin : public CGSPluginInterface,
                            public MGSSettingPSObserver,
                            public MProEngProfileActivationObserver
    {
    public: // Constructors and destructor

        /**
        * Symbian OS two-phased constructor
        * @return
        */
        static CGSPDataHSDPAPlugin* NewL( TAny* aInitParams );

        /**
        * Destructor.
        */
        ~CGSPDataHSDPAPlugin();

	public: // From CAknView

        /**
        * See base class.
        */
        TUid Id() const;

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
        * See base class.
        */
        TGSListboxItemTypes ItemType();

        /**
        * See base class.
        */
        void GetValue( const TGSPluginValueKeys aKey,
                       TDes& aValue );

        /**
        * See base class.
        */
        void HandleSelection( const TGSSelectionTypes aSelectionType );

        /**
        * See base class.
        */
        TGSMenuActivationItems MenuActivationItem();
        
        /**
        * @see CGSPluginInterface header file.
        */
        TBool Visible() const;

    public: //From MProEngProfileActivationObserver
    
        /**
        * Callback for device profile change
        * Used for observing offline mode activation
        */
        void HandleProfileActivatedL( TInt aProfileId );

    protected: // New

        /**
        * C++ default constructor.
        */
        CGSPDataHSDPAPlugin();

        /**
        * Symbian OS default constructor.
        */
        void ConstructL();

    protected: // From CAknView

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

    private: // New

        /**
        * Sets HSDPA state.
        * Launches a setting page with a confirmation dialog
        */
        void SetHSDPAStateL();

        /**
        * Opens localized resource file.
        */
        void OpenLocalizedResourceFileL(
            const TDesC& aResourceFileName,
            RConeResourceLoader& aResourceLoader );
            
        /**
        * Resets all Active Connections, If Any.
        */
        void ResetActiveConnectionsL();
        
       /*
        *
        * Indexes of HSDPA on/off items in a listbox are not the same as 
        * aPDataHSDPState value. Therefore matching between these two is 
        * required.
       */
        TBool ShowHSDPASettingPageL( TInt& aHsdpaState );

        /**
        * Leaving version of GetValue()
        */
        void GetValueL( TDes& aValue );
        
        /**
        *  Updating parent view
        */
        void UpdateParentViewL();
    
    private: // From MGSSettingPSObserver
        /**
        * Callback from MGSSettingPSObserver
        */
        void HandleNotifyPSL( const TUid aUid, const TInt& aKey, 
                              const TRequestStatus& aStatus );

    protected: // Data

        // PDataHSDPA model.
        CGSPDataHSDPAModel* iModel;

        // Resource loader.
        RConeResourceLoader iResources;
        
        //PubSub object for NW mode state listener
        CGSPubSubsListener* iNwModeListener;
        
        MProfileEngine* iProfileEngine;
        MProEngNotifyHandler* iNotifyHandler;
        
        TInt iNetworkMode;
        
        TBool iHSDPASettingSupport;
    };

#endif // GSPDATAHSDPAPLUGIN_H
// End of File
