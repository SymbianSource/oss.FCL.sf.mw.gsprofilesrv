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
* Description:  View for Device & SIM security plug-in.
*
*/


#ifndef GSAUTOKEYGUARDPLUGIN_H
#define GSAUTOKEYGUARDPLUGIN_H

// INCLUDES
#include "GSAutoKeyguardPluginContainer.h"
#include "GSAutoKeyguardPluginModel.h"
#include "MGSSettingPSObserver.h"
#include "GSPubSubsListener.h"

#include <aknsettingpage.h>
#include <ConeResLoader.h>
#include <gsplugininterface.h>
#include <gsfwviewuids.h>
#include <gsbaseview.h>
#include <secuisecuritysettings.h>
#include <RSSSettings.h>

// CONSTANTS
const TInt KMaxStringLength = 80;
const TUid KGSAutoKeyguardPluginUid = { 0x200110f0 };

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CAknViewAppUi;
class CGSAutoKeyguardPluginContainer;


// CLASS DECLARATION

/**
*  CGSAutoKeyguardPlugin view class
*
*  View class for Sim Security sub-folder
*/
class CGSAutoKeyguardPlugin : public CGSBaseView,
                        private MGSSettingPSObserver
    {
    public: // Constructors and destructor
        
        /**
        * Symbian OS two-phased constructor
        * @return GS sim & device security view.
        */
        static CGSAutoKeyguardPlugin* NewL( TAny* aInitParams );
        
        /**
        * C++ default constructor.
        */
        CGSAutoKeyguardPlugin();

        /**
        * Symbian OS default constructor.
        */
        void ConstructL();

        /**
        * Destructor.
        */
        ~CGSAutoKeyguardPlugin();

    public: // Functions from base classes
        
       /**
        * Returns view id.
        * @return TUid.
        */
        TUid Id() const;

        /**
        * Handles commands.
        * @param aCommand Command to be handled.
        * 
        */
        void HandleCommandL( TInt aCommand );

    public: //new

        /**
        * Updates listbox's item's value.
        * @param aItemId An item which is updated.
        * 
        */
        void UpdateListBoxL( TInt aItemId );
        
        //From CGSBaseView
        void HandleResourceChangeL( TInt aType );

    public: // From CGSPluginInterface

        /**
        * @see CGSPluginInterface header file.
        */
        void GetCaptionL( TDes& aCaption ) const;
        /**
        * See base class.
        */
        CGulIcon* CreateIconL( const TUid aIconType );
        /**
        * @see CGSPluginInterface header file.
        */
        TInt PluginProviderCategory() const;

        /**
        * @see CGSPluginInterface header file.
        */
        TBool Visible() const;
        
    protected: // From MEikMenuObserver

        void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );

    private: // from CAknView
        /**
        * Activates the view.
        * @param aPrevViewId ID of previous view
        * @param aCustomMessageId customized message ID
        * @param aCustomMessage sutomized message payload
        */
        void DoActivateL( const TVwsViewId& aPrevViewId,
                          TUid aCustomMessageId,
                          const TDesC8& aCustomMessage );

    private: // from CGSBaseView
       
        void NewContainerL();
       
        /**
        * From CGSBaseView, handles list box selections.
        * 
        */
        void HandleListBoxSelectionL();
 
    private:
        /**
        * Callback from MGSSettingPSObserver
        */
        void HandleNotifyPSL( const TUid aUid, const TInt& aKey, 
                                      const TRequestStatus& aStatus );
      
    private: //new
    	/**
        * Sets Autokeyguard period
        * 
        */
        void SetAutoKeyguardTimeL( TInt aPeriod, TInt aMaxPeriod );

        /**
        * Get CGSAutoKeyguardPlugin's ccontainer.
        */
        CGSAutoKeyguardPluginContainer* Container();
        
    private: // Data
        //plugin model.
        CGSAutoKeyguardPluginModel* iModel;
        //resource loader
        RConeResourceLoader iResourceLoader;
        //PubSub object for BT SAP state
        CGSPubSubsListener* iBtSapListener;

    };

#endif //GSAUTOKEYGUARDPLUGIN_H

// End of File
