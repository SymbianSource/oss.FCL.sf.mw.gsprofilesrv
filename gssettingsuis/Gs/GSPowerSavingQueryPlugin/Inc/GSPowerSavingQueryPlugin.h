/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  View for Data call settings.
*
*/


#ifndef GSPOWERSAVINGQUERYPLUGIN_H
#define GSPOWERSAVINGQUERYPLUGIN_H

// INCLUDES
#include <gsbaseview.h>
#include <ConeResLoader.h>

//CONSTANTS
const TUid KGSPowerSavingQueryPluginUid = { 0x2002120C };
_LIT( KGSPowerSavingQueryPluginIconDirAndName, "z:gspowersavingqueryplugin.mbm"); // Use KDC_BITMAP_DIR

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CGSPowerSavingQueryPluginContainer;

// CLASS DEFINITION
/**
*  CGSPowerSavingQueryPlugin view class for power saving query settings
*  @since Series 60_5.1
*
*/
class CGSPowerSavingQueryPlugin : public CGSBaseView
    {
    public: // Constructors and destructor

        /**
        * Symbian OS two-phased constructor
        * @return GS connection view.
        */
        static CGSPowerSavingQueryPlugin* NewL( TAny* aInitParams );

        /**
        * Destructor
        */
        ~CGSPowerSavingQueryPlugin();

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
    public: // From CAknView

        /**
        * See base class.
        */
        void DoActivateL( const TVwsViewId& aPrevViewId,
                          TUid /*aCustomMessageId*/,
                          const TDesC8& /*aCustomMessage*/ );

        /**
        * See base class.
        */
        void DoDeactivate();

    public: // new

        /**
        * Updates listbox's item's value.
        * @param aItemId An item which is updated.
        *
        */
        void UpdateListBoxL( TInt aItemId );

        /**
        * Get CGSPowerSavingQueryPlugin's ccontainer.
        */
        CGSPowerSavingQueryPluginContainer* Container();

    protected:

        /**
        * C++ default constructor.
        */
        CGSPowerSavingQueryPlugin();

        /**
        * Symbian OS default constructor.
        *
        */
        void ConstructL();

    private: // from CGSBaseView

        void NewContainerL();
        void HandleListBoxSelectionL();

    private: // new methods

        /**
        * Display power saving query setting page.
        */
        void ShowSettingPageL();
        
        /**
         * SwitchOnOffValue
         */
        void SwitchOnOffValue(TInt &aValue);
    private:
        // resource loader
        RConeResourceLoader iResourceLoader;

    };

#endif //GSPOWERSAVINGQUERYPLUGIN_H

// End of File
