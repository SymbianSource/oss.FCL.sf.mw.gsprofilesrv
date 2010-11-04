/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  View handling for Call Divert.
*
*/


#ifndef CFVIEW_H
#define CFVIEW_H

// INCLUDES
#include "CallDivertPlugin.hrh"

#include <aknlists.h>
#include <gsbaseview.h>
#include <gspubsubslistener.h>
#include <mgssettingpsobserver.h>
#include <PsetCSP.h>
#include <eikmenup.h>

// FORWARD DECLARATIONS
class CGSCallDivertPluginContainer;
class CGSCallDivertView;
class CPsetCustomerServiceProfile;
class CGSCallDivertModel;

// CONSTANTS
_LIT( KGSCallDivertPluginIconDirAndName, "z:GSCallDivertPlugin.mbm");

// CLASS DECLARATION

/**
*  CGSCallDivertPlugin view class
*/
class CGSCallDivertPlugin : public CGSBaseView,
                            public MGSSettingPSObserver
    {
    public: // Constructors and destructor

        /* Symbian OS default constructor */
        void ConstructL();

        /* Destructor */
        ~CGSCallDivertPlugin();


        /* Symbian OS two-phase Constructor
        *
        * @return pointer to CGSCallDivertPlugin object
        */
        static CGSCallDivertPlugin* NewLC();

        /* C++ Constructor */
        CGSCallDivertPlugin();

        /**
        * Symbian OS two-phased constructor
        * @return GS connection view.
        */
        static CGSCallDivertPlugin* NewL( TAny* aInitParams );


    public: // functions from base class CAknView

        /**
        * From CAknView, gives the id of the view.
        *
        * @return Returns the TUid of the view.
        */
        TUid Id() const;

        /**
        * From MEikCommandObserver, handles the commands.
        *
        * @param aCommand identifies the command given.
        */
        void HandleCommandL( TInt aCommand );

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

        /**
        *
        */
        void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );


    public: //new

        /**
        * Updates listbox's item's value.
        * @param aItemId An item which is updated.
        *
        */
        void UpdateListBox( TInt aItemId );

        void SetCSPActiveL( TBool aValue );

    private: // from base class CGSBaseView

        /* From CAknView, activates the view. */
        void DoActivateL(
            const TVwsViewId& /*aPrevViewId*/,
            TUid /*aCustomMessageId*/,
            const TDesC8& /*aCustomMessage*/ );

        /* From CAknView, deactivates the view. */
        void DoDeactivate();

        void NewContainerL();

    private: // from CGSBaseView

        void HandleListBoxSelectionL();

        //Retrieves container pointer.
        CGSCallDivertPluginContainer*  Container();

    private: // From MGSSettingPSObserver
        /**
        * Callback from MGSSettingPSObserver
        */
        void HandleNotifyPSL( const TUid aUid, const TInt& aKey,
                              const TRequestStatus& aStatus );

    private:

        // Local view of call diverts.
        CGSCallDivertView* iCallDivertView;
        // PubSub object for BT SAP state.
        CGSPubSubsListener* iBtSapListener;
        // Check if CSP is active.
        CPsetCustomerServiceProfile* iCSP;

        // Model for call diverts.
        CGSCallDivertModel* iCallDivertModel;
    };

#endif
// End of File
