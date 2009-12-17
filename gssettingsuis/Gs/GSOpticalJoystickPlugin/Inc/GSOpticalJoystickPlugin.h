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
* Description:  View for GSOpticalJoystickPlugin
*
*/

#ifndef GSOPTICALJOYSTICKPLUGIN_H
#define GSOPTICALJOYSTICKPLUGIN_H

// INCLUDES
#include <aknsettingpage.h>
#include <gsbaseview.h>
#include <ConeResLoader.h>
#include <gsplugininterface.h>


//CONSTANTS
const TUid KGSOpticalJoystickPluginUid = { 0x1028331B };
_LIT( KGSOpticalJoystickPluginIconDirAndName, "z:GSOpticalJoystickPlugin.mbm"); // Use KDC_BITMAP_DIR

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CAknViewAppUi;
class CGSOpticalJoystickPluginContainer;
class CGSOpticalJoystickPluginModel;

// CLASS DEFINITION
/**
*  CGSOpticalJoystickPlugin view class for data call settings
*  @since Series 60_3.1
*
*/
class CGSOpticalJoystickPlugin : public CGSBaseView,
                                 public MAknSettingPageObserver
    {
    public:
        enum TGSSettingIds
            {
            KGSOpticalJoystickSettingId
            };

    public: // Constructors and destructor

        /**
        * Symbian OS two-phased constructor
        * @return GS connection view.
        */
        static CGSOpticalJoystickPlugin* NewL( TAny* aInitParams );

        /**
        * Destructor
        */
        ~CGSOpticalJoystickPlugin();

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

    public: // from MAknSettingPageObserver

        /**
        * Handle setting page events
        */
        virtual void HandleSettingPageEventL( CAknSettingPage* aSettingPage,
                                            TAknSettingPageEvent aEventType );

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

    public: // new

        /**
        * Updates listbox's item's value.
        * @param aItemId An item which is updated.
        *
        */
        void UpdateListBoxL( TInt aItemId );

        /**
        * Get CGSOpticalJoystickPlugin's ccontainer.
        */
        CGSOpticalJoystickPluginContainer* Container();

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
        CGSOpticalJoystickPlugin();

        /**
        * Symbian OS default constructor.
        *
        */
        void ConstructL();

        void HandleClientRectChange();

    private: // from CGSBaseView

        void NewContainerL();
        void HandleListBoxSelectionL();

    private: // new methods

        /**
        * Display dialup auto disconnect setting page.
        */
        void ShowSettingPageL();

    private:
        // resource loader
        RConeResourceLoader iResourceLoader;

    };

#endif //GSOPTICALJOYSTICKPLUGIN_H

// End of File
