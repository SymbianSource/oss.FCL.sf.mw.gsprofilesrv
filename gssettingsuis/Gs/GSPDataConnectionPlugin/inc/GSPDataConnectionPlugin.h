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
* Description:  GSPDataConnectionPlugin implementation.
*
*/

#ifndef GSPDATACONNECTIONPLUGIN_H
#define GSPDATACONNECTIONPLUGIN_H

// User includes

// System includes
#include <gsplugininterface.h>
#include <aknview.h>
#include <ConeResLoader.h>
#include <eikclb.h>

// Classes referenced
class CAknViewAppUi;
class CGSPDataConnectionModel;


// Constants
const TUid KGSPDataConnectionPluginUID   = { 0x10282DC4};
_LIT( KPDataConnectionResourceFileName, "z:gspdataconnectionpluginrsc.rsc" );

// CLASS DECLARATION

/**
*  CGSPDataConnectionPlugin view class.
*
* This class handles state and application logic of PDataConnection settings.
* The plugin is a type of EGSItemTypeSettingDialog and therefore the GS FW will
* call HandleSelection() instead of DoActivate().
*
*/
class CGSPDataConnectionPlugin : public CGSPluginInterface
    {
    public: // Constructors and destructor

        /**
        * Symbian OS two-phased constructor
        * @return
        */
        static CGSPDataConnectionPlugin* NewL( TAny* aInitParams );

        /**
        * Destructor.
        */
        ~CGSPDataConnectionPlugin();

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

    protected: // New

        /**
        * C++ default constructor.
        */
        CGSPDataConnectionPlugin();

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
        * Sets Attach state.
        * Launches Setting Page for setting the item
        */
        void SetAttachStateL();

        /**
        * Launches Attach on/off dialog.
        * @param aPDataConnectionState Current value of Attach state. This WILL BE
        *        UPDATED to contain new user selected value.
        * @return ETrue if value updated.
        *         EFalse if value not updated.
        */
        TBool ShowAttachSettingPageL( TInt& aPDataConnectionState );

        /**
        * Opens localized resource file.
        */
        void OpenLocalizedResourceFileL(
            const TDesC& aResourceFileName,
            RConeResourceLoader& aResourceLoader );

        /**
        * Leaving version of GetValue()
        */
        void GetValueL( TDes& aValue );

    protected: // Data

        // PDataConnection model.
        CGSPDataConnectionModel* iModel;

        // Resource loader.
        RConeResourceLoader iResources;
    };

#endif // GSPDATACONNECTIONPLUGIN_H
// End of File
