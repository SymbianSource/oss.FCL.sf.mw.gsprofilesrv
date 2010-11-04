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
* Description:  GSPDataAccessPointPlugin implementation.
*
*/

#ifndef GSPDATAACCESSPOINTPLUGIN_H
#define GSPDATAACCESSPOINTPLUGIN_H

// User includes

// System includes
#include <gsplugininterface.h>
#include <aknview.h>
#include <ConeResLoader.h>
#include <eikclb.h>

// Classes referenced
class CAknViewAppUi;
class CGSPDataAccessPointModel;


// Constants
const TUid KGSPDataAccessPointPluginUID   = { 0x10282DC6 };
_LIT( KPDataAccessPointResourceFileName, "z:gspdataaccesspointpluginrsc.rsc" );

// CLASS DECLARATION

/**
*  CGSPDataAccessPointPlugin view class.
*
* This class handles state and application logic of PDataAccessPoint settings.
* The plugin is a type of EGSItemTypeSettingDialog and therefore the GS FW will
* call HandleSelection() instead of DoActivate().
*
*/
class CGSPDataAccessPointPlugin : public CGSPluginInterface
    {
    public: // Constructors and destructor

        /**
        * Symbian OS two-phased constructor
        * @return
        */
        static CGSPDataAccessPointPlugin* NewL( TAny* aInitParams );

        /**
        * Destructor.
        */
        ~CGSPDataAccessPointPlugin();

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
        
    protected: // New

        /**
        * C++ default constructor.
        */
        CGSPDataAccessPointPlugin();

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
        * Show Access point setting page.
        * 
        */
        void ShowAccessPointSettingPageL();

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

        // PDataAccessPoint model.
        CGSPDataAccessPointModel* iModel;

        // Resource loader.
        RConeResourceLoader iResources;
    };

#endif // GSPDATAACCESSPOINTPLUGIN_H

// End of File
