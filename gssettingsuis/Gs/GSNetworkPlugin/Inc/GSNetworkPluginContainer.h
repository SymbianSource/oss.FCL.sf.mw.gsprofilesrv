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
* Description:  Container for the Network sub-folder
*
*/


#ifndef GSNETWORKPLUGINCONTAINER_H
#define GSNETWORKPLUGINCONTAINER_H

// INCLUDES
#include "GSNetworkPluginModel.h"
#include "GsNetworkPlugin.hrh"
#include "GSPhoneSettingConstants.h"  //for GS & PS constants
#include <bldvariant.hrh>
#include <gsbasecontainer.h>
#include <MPsetNetworkModeObs.h>
#include "MGsFWMSKLabelObserver.h"

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CPsetNetwork;
class MEikListBoxObserver;
class CGSListBoxItemTextArray;
class CGSRadioButtonSettingPageItemTextArray;
class CPsetContainer;
class CGSNetworkPluginModel;
class CGSNetworkPlugin;

// CLASS DECLARATION

/**
*  GSNetworkPluginContainer container class
*
*  container class for Device settings view
*  @lib GSNetworkPlugin.lib
*  @since Series 60_3.1
*/
class CGSNetworkPluginContainer : public CGSBaseContainer
    {
    public:
        enum {
            ENetFirstBit = 1,
            ENetSecondBit = 2
            };

    public: // Constructors and destructor


        /** C++ Constructor */
        CGSNetworkPluginContainer( CGSNetworkPlugin* aPlugin );

        /**
        * Symbian OS constructor.
        * @param aRect Listbox's rect.
        *
        */
        void ConstructL( const TRect& aRect );

        /**
        * Destructor.
        */
        ~CGSNetworkPluginContainer();

    public: //new

        /**
        * Updates listbox's item's value.
        * @since Series 60_3.1
        * @param aItemId An item which is updated.
        */
        void UpdateListBoxL( TInt aItemId, TInt aValue );

        /**
        * Retrieves the currently selected listbox feature id
        * @since Series 60_3.1
        * @return feature id.
        */

        TInt CurrentFeatureId() const;
       /**
        * Gets currently active network selection mode.
        * @return currently active network selection mode
        */
        TInt GetSelectionMode();

        /**
        *  Gets Mcn value from model.
        *  @param aMcnValue MCN value read from shared data.
        */
        void GetMcnValue( TInt &aMcnValue );

        /**
        *  Sets MCN value to model.
        *  @param aMcnValue MCN value written to shared data.
        */
        void SetMcnValue( TInt &aMcnValue );

        /**
        * Trims non-supported variable options from the array
        * This array items are displayed as radio button setting page items
        *
        * @param aItemArray array contents to trim from
        */
        void CheckAndAlterContentsL( CGSRadioButtonSettingPageItemTextArray&
                                    aItemArray );

        /**
        * return the model pointer
        */
        CGSNetworkPluginModel* NetPluginModel();

        /**
        * Defines observer for the middle softkey label changes.
        * Can only be set once. Further attempts are ignored.
        * @since S60 v3.1
        */
        void SetMiddleSoftkeyObserver( MGsFWMSKObserver* aObserver );

        /**
        * OfferKeyEvent to this Call Container class for updating the label
        * Up and Down rocker keys are handled
        * @since S60 v3.1
        */
        TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent,
                                     TEventCode aType );

        /**
        * Callback from model.
        */
        void HandleNetworkModeChangeL();

        /**
        * Returns the current selection of network mode. Note! network
        * selection can be unknow in case network is slow and actual data
        * has not yet been received...
        * @return current network selection.
        */
        TGSNetworkModeItems GetCurrentNetworkModeSelectionL();

        /**
        * Sets the current selection of network mode
        * @param aNetworkMode current selection
        */
        void SetCurrentNetworkModeSelectionL(
                TGSNetworkModeItems aNetworkModeLbxItem );

    protected: // from CGSBaseContainer
        /**
        * Creates list box.
        *
        * @param aResLbxId is resource number to create
        */
        void ConstructListBoxL( TInt aResLbxId );

        /**
        * Required for help.
        *
        */
        void GetHelpContext(TCoeHelpContext& aContext) const;

    private: //new

        /**
        * Creates list box items.
        */
        void CreateListBoxItemsL();

        /**
        * Creates Network list box item.
        * @param aValue value of setting item.
        */
        void MakeNwItemL( TInt aValue = -1 );

        /**
        * Creates MCN list box item.
        */
        void MakeMcnItemL();

        /**
        * Creates Network mode UI list box item.
        */
        void MakeNwModeUiItemL();

    private: // Member variables

        // Resource based features
        CGSListBoxItemTextArray* iListboxItemArray;

        // CGSNetworkPluginModel is the model class of Network plugin.
        CGSNetworkPluginModel*   iModel;

        // Currently active network item in listbox.
        TGSNetworkModeItems iSelectedNetworkModeLbxItem;

        /**
        * Middle softkey label observer.
        * Own.
        */
        MGsFWMSKObserver* iMSKObserver;

        /**
        * Reference to container's owner. Not owned.
        */
        CGSNetworkPlugin* iPlugin;

    };

#endif //GSNETWORKPLUGINCONTAINER_H

//End of File
