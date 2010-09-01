/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Network Settings model.
*
*/


#ifndef GSNETWORKPLUGINMODEL_H
#define GSNETWORKPLUGINMODEL_H

#include <e32base.h>
#include <e32property.h>
#include <centralrepository.h>
#include <PsetCSP.h>
#include <MPsetNetworkModeObs.h>
#include <PsetNetwork.h>

#include "GSNetworkPluginContainer.h"
#include "GsNetworkPlugin.hrh"

// CONSTANTS

const TUint32 KGSNetworkModeCapsNotUpdated = KMaxTUint32;

// Used only for debugging. Init value for network mode debug strings.
const TInt KGSNetworkModeDebugStrLen = 50;


// Panic codes for used in this compilation unit
enum TGSTelPluginModelPanicCodes
    {
    EGSTelPluinModelPanicNullPtr = 1
    };
// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class MPsetNetworkModeObserver;
class CPsetContainer;
class CGSNetworkPluginContainer;
class CGSNetworkPlugin;

// CLASS DEFINITION
/**
*  CGSNetworkPluginModel is the model class of GS network plugin.
*  It provides functions to get and set setting values.
*  @lib GSNetworkPlugin.lib
*  @since Series 60_3.1

*/
class CGSNetworkPluginModel : public CBase,
                              private MPsetNetworkModeObserver
    {
    public:  // Constructor and destructor

        /**
         * Overloaded constructor meant to be used from container
         */
        static CGSNetworkPluginModel* NewL(
                CGSNetworkPluginContainer* aContainer,
                CGSNetworkPlugin* aPlugin );

        /**
        * Destructor
        */
        ~CGSNetworkPluginModel();

    public: // new ones
        /**
        * Gets CSP status
        *
        * @return ETrue: CSP is set
        *         EFalse: CSP is not set
        */
        TBool GetCSPStatus();

        /**
        * Sets CSP Active
        * @param aValue TBool
        * @return ETrue: CSP is set
        *         EFalse: CSP is not set
        */
        void SetCSPActiveL( TBool aValue );

         /**
        * Get the network mode status from Shared data. This value is used
        * to display/hide the "network mode UI" list item under network
        * folder. Currently, this value is ONLY read and not written.
        * @return ETrue if the value is 1 in SD.
        */
        TBool IsNetworkModeVisible();

        /**
        * Returns if CSP setting is supported
        * @param aSettingNumber TInt
        * @return ETrue: CSP is supported
        *         EFalse: CSP is not supported
        */
        TBool IsSettingSupported( TInt aSettingNumber );

                /*
        * Returns MCN value from shared data.
        * @param aId value of shared data key
        * @return ETrue: no errors
        *         EFalse: an error has occurred
        */
        TBool GetMCNSettingValue( TInt& aId );

        /*
        * Sets MCN value to shared data.
        * @param aId value of shared data key
        * @return ETrue: no errors
        *         EFalse: an error has occurred
        */
        TBool SetMCNSettingValue( TInt& aId );

        /*
        * Gets the supported network mode list from shared data.
        * @return the bit-masked value from shared data
        */
        TInt GetSupportedNetworksL();

        /**
        * Check if graphical network list is supported.
        * This is handled using GS local variation
        * @return ETrue if supported.
        */
        TBool GraphicalNetworkListSupportedL();

        /**
        * Return local variation value for automatic network search
        * support. Local key = EGSConfigNoAutoToAutoNetworkSearch
        * See GeneralSettingsVariant.hrh for details
        * @return local variation value
        */
        TBool AutomaticNetworkSearchSupportedL();


        /**
        *  Creates phone settings engine.
        *  @param aContainer Container object as reference
        */
        void CreatePhoneSettingsEngineL();

        /**
        * StartAsynGetCurrentNetworkModeSelectionL 
        */
        void StartAsynGetCurrentNetworkModeSelectionL();     
        
        /**
         * StartSynGetCurrentNetworkModeSelectionL
         */
        void StartSynGetCurrentNetworkModeSelectionL();
        
        /**
        * GetNetworkSelectionMode
        */
        MPsetNetworkSelect::TSelectMode GetNetworkSelectionMode();

        /**
        * See iNetworkMode.
        * @return TUint32 Current network mode value,  RMmCustomAPI::KCapsNetworkModeUmts etc...
        */
        TUint32 GetNetworkMode();
        
        /**
        * SetNetworkModeL 
        */
        void SetNetworkModeL( TUint32 aNetworkMode );

        /**
        * Prints model state to debug output in case of debug build.
        */
        void PrintState();

        /**
        * Used only in debug mode: verifies network state with the UI selection.
        * For debugging purposes only.
        */
        void CheckState( TGSNetworkModeItems aMode );




        TBool IsCallActive();

    private: // Private constructors

        /**
        * Default C++ contructor
        */
        CGSNetworkPluginModel( CGSNetworkPluginContainer* aContainer,
                               CGSNetworkPlugin* aPlugin );

        /**
        * Symbian OS default constructor
        * @return void
        */
        void ConstructL();

        void ConstructBaseItemsL();

        /**
        * Initialize CenRep keys used in this class
        */
        void InitializeCentralRepositoryL();

        /**
        * Un-initialize CenRep keys used in this class
        */
        void UninitializeCentralRepository();

    private: // from base class MPsetNetworkModeObserver

        /**
        * @@see MPsetNetworkModeObserver::HandleNetworkSystemModeEventsL.
        * @since 2.6
        */
        void HandleNetworkSystemModeEventsL(
            const MPsetNetworkModeObserver::TServiceRequest aRequest,
            const TUint32 aNetworkModeCaps );


        /**
        * @@see MPsetNetworkModeObserver::HandleNetworkErrorL.
        * @since 2.6
        */
        void HandleNetworkErrorL(
            const MPsetNetworkModeObserver::TServiceRequest aRequest,
            const TInt aError );

    private:
        // Central repository objects
        CRepository* iNetworkRepository;
        CRepository* iGSVariationRepository;
  		CPsetCustomerServiceProfile* iCSP; //check if CSP is active
  		TInt iLocalVariationValues; //local variated constants

        // PhoneSettings engine object for network mode
        CPsetNetwork* iPhoneSettingsEngine;
        CPsetContainer* iSettingsContainer;

        // Current network mode value from RMmCustomAPI::TNetworkModeCaps.
        // enum RMmCustomAPI::TNetworkModeCaps
        //    {
        //    KkNetworkModeGsm = 0x01,
        //    KCapsNetworkModeUmts = 0x02,
        //    KCapsNetworkModeDual = 0x04
        //    };
        // and in addition: KGSNetworkModeCapsNotUpdated
        //
        // Stores value aNetworkModeCaps from:
        // MPsetNetworkModeObserver::HandleNetworkSystemModeEventsL(..., TUint32 aNetworkModeCaps )
        //
        TUint32 iNetworkMode;
        
        // Temporarily value of the requested network mode. Stores requested 
        // mode which is updated to iNetworkMode in case request succeeds and 
        // HandleNetworkSystemModeEventsL is called.  
        TUint32 iRequestedNetworkMode;

        // Pointer for communicating with container. Not owned.
        CGSNetworkPluginContainer* iContainer;

        // Reference to network plugin. Not owned.
        CGSNetworkPlugin* iPlugin;
    };

#endif //GSNETWORKPLUGINMODEL_H

// End of File
