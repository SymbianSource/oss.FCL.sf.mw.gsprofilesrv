/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  View for Network settings.
*
*/


#ifndef GSNETWORKPLUGIN_H
#define GSNETWORKPLUGIN_H

// INCLUDES
#include <aknsettingpage.h>
#include <ConeResLoader.h>
#include <gsplugininterface.h>
#include <gsfwviewuids.h>
#include <gsbaseview.h>
#include <MSSSettingsRefreshObserver.h>
#include <MPsetNetworkInfoObs.h>//for base class
#include <etelmm.h>
#include <msatrefreshobserver.h>
//For listening BT SAP events
#include <gspubsubslistener.h>
#include <mgssettingpsobserver.h>
#include <eikmenup.h>

#include "GSPhoneSettingConstants.h"
#include "GSNetworkPluginContainer.h"
#include "GsNetworkPlugin.hrh"
#include "MGsFWMSKLabelObserver.h"

#include <etelmm.h>
#include <e32base.h>

#include <NWHandlingEngine.h>

// CONSTANTS
const TUid KGSNetworkPluginUid = { 0x102824A8 };
// Resource file name
_LIT( KGSNetworkPluginResourceFileName, "z:GSNetworkPluginRsc.rsc" );
// Icon file name
_LIT( KGSNetworkPluginIconDirAndName, "z:GSNetworkPlugin.mbm");

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD CLASS DECLARATION
class CAknViewAppUi;
class CGSNetworkPluginContainer;
class CPsetNetwork;
class CAknWaitDialog;
class CPsetContainer;
class CAknRadioButtonSettingPage;
class CAknPopupList;
class MSSSettingsRefreshObserver;
class CPSetRefreshHandler;

class CNWSession;
class TNWInfo;
class CGSParentPlugin;

// CLASS DECLARATION
/**
*  CGSNetworkPlugin view class
*
*  Plugin implementation for Network settings
*  @lib GSNetworkPlugin.lib
*  @since Series 60_3.1
*/
#ifdef FF_POWER_SAVE
    class CGSNetworkPluginAO;
#endif // FF_POWER_SAVE

class CGSNetworkPlugin : public CGSBaseView,
                         public MPsetNetworkInfoObserver,
                         public MSSSettingsRefreshObserver,
                         public MGsFWMSKObserver,
                         public MGSSettingPSObserver,
                         public MNWMessageObserver
    {
    public: // Constructors and destructor

        /** C++ Constructor */
        CGSNetworkPlugin();

        /**
        * Symbian OS two-phased constructor
        * @return GS connection view.
        */
        static CGSNetworkPlugin* NewL( TAny* aInitParams );

        static CGSNetworkPlugin* NewLC();

        /**
        * Destructor.
        */
        ~CGSNetworkPlugin();

    public: // from base classes

        /**
        * Returns view id.
        * @return TUid
        */
        TUid Id() const;

        /**
        * Handles commands.
        * @param aCommand Command to be handled.
        *
        */
        void HandleCommandL( TInt aCommand );

        /**
        * From MEikCommandObserver, handles the commands.
        *
        * @param aCommand identifies the command given.
        */
        void ProcessCommandL( TInt aCommand );

    public: // From CGSPluginInterface

        /**
        * @see CGSPluginInterface header file.
        */
        void GetCaptionL( TDes& aCaption ) const;

        /**
         * @see CGSPluginInterface header file.
         */
         void GetValue( const TGSPluginValueKeys aKey,
                        TDes& aValue );

        /**
        * @see CGSPluginInterface header file.
        */
        TInt PluginProviderCategory() const;

        /**
        * See base class.
        */
        CGulIcon* CreateIconL( const TUid aIconType );

        /**
        * @see CGSPluginInterface header file.
        */
        TBool Visible() const;

    public: //new

        /**
        * Get CGSNetworkPlugin's ccontainer.
        */
        CGSNetworkPluginContainer* Container();

        /**
        * Checks if the MSK label needs to be adjusted.
        */
        void CheckMiddleSoftkeyLabelL();

                /**
        * Remove command and label from MSK.
        */
        void RemoveCommandFromMSK( const TBool flag);

        /**
        * Adds given resource text as MSK to CBA.
        *
        * @param aResourceId middle softkey label.
        * @param aCommandId command that should be performed when MSK
        *        is pressed.
        * @since S60 v3.1
        */
        void SetMiddleSoftKeyLabelL( const TInt aResourceId,const TInt aCommandId );

        /**
        * Shows a requested note.
        * @param aResourceId reource ID for note text
        * @param aType type of note
        */
        void ShowNoteL( TInt aResourceId, TInt aType );

    public: // from base class MPsetNetworkInfoObserver

        /**
        * @@see MPsetNetworkInfoObserver::HandleNetworkInfoReceivedL
        */
        void HandleNetworkInfoReceivedL(
            const CNetworkInfoArray* aInfoArray, const TInt aResult );

        /**
        * @@see MPsetNetworkInfoObserver::HandleCurrentNetworkInfoL - not used
        */
        inline void HandleCurrentNetworkInfoL(
            const MPsetNetworkSelect::TCurrentNetworkInfo& /* aCurrentInfo */,
            const TInt /* aResult */ ) {}

        /**
        * DEPRECATED in MPsetNetworkInfoObserver.
        *
        * @@see MPsetNetworkInfoObserver::HandleNetworkChangedL
        */
        void HandleNetworkChangedL(
            const MPsetNetworkSelect::TNetworkInfo& aCurrentInfo,
            const MPsetNetworkSelect::TCurrentNetworkStatus aStatus,
            const TInt aResult );

        /**
        * @@see MPsetNetworkInfoObserver::HandleNetworkChangedL
        */
        void HandleNetworkChangedL(
            const MPsetNetworkSelect::TNetworkInfo& aCurrentInfo,
            const RMobilePhone::TMobilePhoneRegistrationStatus& aStatus,
            const TInt aResult );


        /**
        * @@see MPsetNetworkInfoObserver::HandleSearchingNetworksL
        */
        void HandleSearchingNetworksL( TServiceRequest aRequest );

        /**
        * @@see MPsetNetworkInfoObserver::HandleRequestingSelectedNetworkL
        */
        void HandleRequestingSelectedNetworkL( TBool aOngoing );

        /**
        * @@see MPsetNetworkInfoObserver::HandleCallActivatedL
        */
        void HandleCallActivatedL();

        /**
        * @@see MPsetNetworkInfoObserver::HandleNetworkErrorL
        */
        void HandleNetworkErrorL( const TServiceRequest aRequest,
            const TInt aError );

    public: //from base class MSSSettingsRefreshObserver
        /**
        * @@see MSSSettingsRefreshObserver::AllowRefresh
        */
        TBool AllowRefresh(
            const TSatRefreshType aType,
            const TSatElementaryFiles aFiles );

        /**
        * @@see MSSSettingsRefreshObserver::Refresh
        */
        void Refresh(
            const TSatRefreshType aType,
            const TSatElementaryFiles aFiles );

    public: // From MNWMessageObserver

        /**
        * Offers message interface to the client
        * @param aMessage
        * This methods execute time must be short,since code
        * starting to run from RunL.
        *
        * Function updates data into iNWInfo.
        */
        void HandleNetworkMessage( const TNWMessages aMessage );

        /**
        * Offers error message interface to the client
        * @param aOperation operation which failed
        * @param aErrorCode returned Symbian OS error code
        *
        */
        void HandleNetworkError( const TNWOperation aOperation, TInt aErrorCode );

        /**
        * Updates network setting page in case it is visible. This is needed in
        * case network mode is change while setting page is oppen.
        */
        void UpdateNetworkSettingPageL();
    public:
#ifdef FF_POWER_SAVE
        /*
         * Update network plugin view when PSM mode has changed
         */
        void UpdateOnPsmChanged();
#endif
        
    protected: // From CAknView
        void DoActivateL( const TVwsViewId& aPrevViewId,
                          TUid aCustomMessageId,
                          const TDesC8& aCustomMessage );
        void DoDeactivate();

    protected: // From MEikMenuObserver

        void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );

    protected:

        /**
        * C++ default constructor.
        */
/*        CGSNetworkPlugin();*/

        /**
        * Symbian OS default constructor.
        *
        */
        void ConstructL();

        void HandleClientRectChange();

    private: // from CGSBaseView

        void NewContainerL();
        void HandleListBoxSelectionL();

    private: // From MGSSettingPSObserver
        /**
        * Callback from MGSSettingPSObserver
        */
        void HandleNotifyPSL( const TUid aUid, const TInt& aKey,
                              const TRequestStatus& aStatus );

    private: //new methods

        /**
        * Check if calls are active. Show note if Nw-menu is tried to open.
        * @param aItemId list item ID in network view
        */
        void CheckCallActiveL( TGSNetworkItemIds aItemId );

        /**
        * Provides the network operator list.
        */
        void NetworkListL();

        /**
        * Displays setting page for highlighted setting.
        *
        * @param aPage identifies the page that will be displayed.
        */
        void ShowSettingPageL( TGSNetworkItemIds aPage );

        /**
        * Sets title to a popup list.
        * @param aList pointer to list
        * @param aTitleID resource ID of the title
        */
        void SetTitleToPopupL( CAknPopupList& aList, TInt aTitleID );

        /**
        * Adds item to a list.
        * @param aList pointer to a list
        * @param aItem resource ID of the item to be added
        */
        void AppendItemL( CDesCArrayFlat& aList, TInt aItem );

        /**
        * Sets title to a settings page.
        * @param aDlg setting page
        * @param aTitleID resource ID of the title
        */
        void SetTitleToSettingsL( CAknRadioButtonSettingPage& aDlg,
            TInt aTitleID );

        /**
        * Shows a note if GPRS is active, when MCN is turned on.
        * @param aCurrentItem value of setting currently
        */
        void CheckGPRSConnectionL( TInt aCurrentItem );

        /**
        * Switches between shared data and setting page values (0 > 1, 1 > 0).
        * @param aValue value to switch
        */
        void SwitchOnOffValue( TInt& aValue );

        /**
        * Makes a Network/MCN Ss operation.
        * @param aIndex currently active list item.
        * @param aPage which setting page is open
        */
        void CreateNetworkSsCallL( TInt aIndex, TGSNetworkItemIds aPage );

        /**
        * Creates a waiting note (member variable).
        * @param aDelayOff is the note shown immediately or not
        */
        void CheckAndCreateDlgL( TBool aDelayOff );

        /**
        * Empties fetched network provider's list.
        */
        void PurgeNetworkList();

        /**
        * Updates listbox in container
        */
        void UpdateListBoxL( const TInt& aListItem, const TInt aValue = -1 );

        /**
        * Activate the Phone Idle view.
        */
        void PhoneIdle();

        /**
        * To verify if the phone is in the offline mode. (or mostly if mode is supported???)
        */
        TBool IsPhoneOfflineL() const;

        /**
        * Actually checks from telephone server cached data (iNWInfo) if phone
        * is online.
        *
        * @return ETrue if phone is online.
        */
        TBool PhoneOnline();

        /**
        * Updates iNetworkText from the iNWInfo values.
        */
        void UpdateNetworkTextL();

        /**
        * Easy access to parent plugin. Casts view from AppUI.
        */
        CGSParentPlugin* Parent();

        /**
        *  Convert TInt to TGSNetworkModeItems.
        *  Will panic in case TInt is of wrong value.
        */
        TGSNetworkModeItems IntToEnum( TInt aFeatureId );

        /**
        * Closes dialog (iSettingDlg) if it is open. 
        */
        void CloseDialog();
        
#ifdef FF_POWER_SAVE    
        /**
         *  Displays blocking note. Used if PSM is on.
         */
        void DisplayBlockNoteL();
#endif // FF_POWER_SAVE
        
    public: //enumerations

        enum TGSNetworkList
            {
            EAutomaticMode,
            EManualMode
            };

    protected:

        // for load wait dialog
        CAknWaitDialog* iWaitDialog;

    private:

        //Requesting note
        CAknWaitDialog*         iDlg;
        //PhoneSettings container
        CPsetContainer*         iSettingsContainer;
        //PhoneSettings engine object
        CPsetNetwork*           iPhoneSettingsEngine;
        //Array of networks
        CNetworkInfoArray*      iNetworkArray;
        //Network selection list
        CAknPopupList*          iNetworkPopupList;
        //Setting page
        CAknRadioButtonSettingPage* iSettingDlg;
        //Value of MCN setting item
        TInt                    iMCN;
        //Has the user selected a valid network
        TBool                   iApprovedNetwork;
        //PhoneSettings refresh handler
        CPSetRefreshHandler*    iPSRefreshHandler;
        //Previous network selection state
        TBool                   iPreviousState;
        // check if which command MSK is set during destruction
        TBool                   iMskCommandFlag;
        //PubSub object for BT SAP state
        CGSPubSubsListener*     iBtSapListener;
        TBool                   iSearchForNetworksActive;

        // Session to network handling engine. Owned.
        CNWSession* iNWSession;

        // Cached network info structure. Will be updated by iNWSession in a
        // callback.
        TNWInfo iNWInfo;

        // Cached network text read by GetValue(). When network handling engine
        // updates data, this buffer is re-created accordingly.
        HBufC* iNetworkText;
#ifdef FF_POWER_SAVE
        // Active object for handling PSM realted CenRep events.
        CGSNetworkPluginAO* iPsmActive;
#endif // FF_POWER_SAVE

    };

#endif // GSNETWORKPLUGIN_H

//End of File
