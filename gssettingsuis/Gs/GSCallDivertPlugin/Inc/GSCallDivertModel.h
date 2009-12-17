/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Model for call divert functionality. Tries to encapsulate
*                 most of hw/engine related functionality from UI components.
*
*/

#ifndef CGSCALLDIVERTMODEL_H_
#define CGSCALLDIVERTMODEL_H_


#include <data_caging_path_literals.hrh>
#include "CallDivertPlugin.hrh"

#include <etelmm.h>
#include <nwdefs.h>
#include <e32base.h>


// Forward declarations:
class CPsetCallDiverting;
class CPsetContainer;
class CPsuiContainer;
class CPsuiDivertObs;
class CPhCltUssd;
class RSSSettings;
class CPbkContactEngine;
class CGSCallDivertView;
class CRepository;

class CGSCallDivertModel : public CBase
    {
    public:

        /**
        * Symbian OS two-phase Constructor
        * @return
        */
        static CGSCallDivertModel* NewL ( );

        /**
        * Destructor
        */
        virtual ~CGSCallDivertModel ( );

    public: // Functions

       /**
       * Send call divert request.
       * @param aCallDivertSetting Data used for call divert.
       * @param aInfo Defines the precise action.
       * @param aVmbxDivert ETrue if voice mailbox is used as divert number.
       * @param aCommand This is for CancelAll command.
       */
       void SendCallDivertRequestL(
           TCallDivertSetting& aCallDivertSetting,
           RMobilePhone::TMobilePhoneServiceAction aInfo,
           TBool& aVmbxDivert,
           TInt aCommand,
           TGSCallDivertServices aService );

        /**
        * Handles attempts to divert to voicemail box.
        * @param aTelNumber telephone number of the voicemail box
        * @return KErrNone on success, otherwise KErrCancel
        */
        TInt VoiceMailActivationL( TDes& aTelNumber );

        /**
        * Handles attempts to divert to videomail box.
        * @param aTelNumber telephone number of the videomail box
        * @return KErrNone on success, otherwise KErrCancel
        */
        TInt VideoMailActivationL( TDes& aTelNumber );

        /**
        * Handles attempts to check divert status in restricted
        * mode (KFeatureIdRestrictedCallDivert).
        */
        void CheckStatusRestrictedCallDivertL();

        /**
        * Handles attempts to cancel call divert restricted
        * mode (KFeatureIdRestrictedCallDivert).
        */
        void CancelRestrictedCallDivertL();

        /**
        * Returns the contact name matching the number, or the number if
        * no contact name is found.
        * Pushes the found (i.e. returned) name into CleanupStack.
        * @param aTelNum number on which to base the fetch.
        * @return Pointer to contact name
        */
        HBufC* MatchNumberToContactLC( const TDesC& aTelNum );

        /**
        * @ return Reference to divert handling engine. Does NOT transfer
        *          ownership.
        */
        CPsetCallDiverting* DivertHandler();

        /**
        * @param aView Reference to call divert view. Does NOT transfer
        *        ownership.
        */
        void SetDivertView( CGSCallDivertView* aDivertView );

        /**
        * @return Currently selected divert service group.
        */
        TGSCallDivertServiceGroups DivertServiceGroup();

        /**
        * Sets new divert service group. Voice/Data/Fax...
        * See TGSCallDivertServiceGroups
        * @param aDivertServiceGroup New divert service group.
        */
        void SetDivertServiceGroup(
            TGSCallDivertServiceGroups aDivertServiceGroup );

        /**
        * Checks number grouping feature from central repository.
        * @return ETrue  - Feature supported.
        *         EFalse - Feature not supported.
        */
        TBool NumberGroupingSupportedL();

    private: // Functions

        /**
        * Symbian OS default constructor
        */
        void ConstructL ( );

        /**
        *  C++ Constructor
        */
        CGSCallDivertModel ( );

        /**
        * Handles attempts to activate call divert in restricted
        * mode (KFeatureIdRestrictedCallDivert).
        */
        void ActivateRestrictedCallDivertL( TTelNumber& aDivertNumber );

        /**
        * Sets parameters for calling Phone Settings' method.
        */
        void SetDivertParametersL( TCallDivertSetting& aDivert,
                                   TBasicServiceGroups& aBsc,
                                   TGSCallDivertServices aService,
                                   TGSCallDivertServiceGroups aServiceGroup );

        /**
        * Uses the phonesettings-module.
        *
        * @param aDivert gives required information such as number,
        *                timervalue, status etc
        * @param aInfo   defines the precise action
        * @param aVmbxValidNumber divert is to a voicemail box number
        * @param aCommand this is for CancelAll command
        */
        void EngineOperatorL( TCallDivertSetting& aDivert,
            const RMobilePhone::TMobilePhoneServiceAction & aInfo,
            TBool& aVmbxValidNumber,
            TInt aCommand,
            TGSCallDivertServices aService,
            TGSCallDivertServiceGroups aServiceGroup );

        /**
        * Removes special chars ('*','+','p','w' and '#') from VMBX number.
        */
        static void RemoveSpecialChar(
            TDes& aVmbxNumber,
            const TDesC& aSpecChar );


    private: // Member variables

        // Selected call divert group. Selected from the main view. Value is
        // stored here as is accessed by several components.
        TGSCallDivertServiceGroups iDivertServiceGroup;

        // PhoneSettings engine object for diverts
        CPsetCallDiverting* iForward;

        // PhoneSettings container
        CPsetContainer*     iSettings;

        // PSUI container
        CPsuiContainer*     iObsContainer;

        // Shows PSUI notes
        CPsuiDivertObs*     iCfObserver;

        // Sending with notes
        CPhCltUssd* iBasicClient;

        // For fetching line in use info
        RSSSettings* iSsSettings;

        // Contact engine for retrieving contact numbers
        CPbkContactEngine*  iContactEngine;

        // Reference to call divert view. NOT owned.
        CGSCallDivertView* iDivertView;

        // Repository for number grouping run time variation.
        CRepository* iNumberGroupingRepository;
    };

#endif /*CGSCALLDIVERTMODEL_H_*/
