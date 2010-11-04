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
* Description:  GSDiagnosticsPlugin implementation.
*
*/

#ifndef GSDIAGNOSTICSPLUGIN_H
#define GSDIAGNOSTICSPLUGIN_H

// User includes

// System includes
#include <GSPluginInterface.h>
#include <aknview.h>
#include <ConeResLoader.h>
#include <eikclb.h>

// Classes referenced
class CImplementationInformation;


// Constants
const TUid KGSDiagnosticsPluginUID = { 0x102750D4 };
_LIT( KOperatorLogoResourceFileName, "z:gsdiagnosticspluginrsc.rsc" );

// CLASS DECLARATION

/**
* This is a utility plugin used to find errors in GS plugins.
* - Crashes on construction/deletion
* - Memory leaks on construction/deletion
*
*/
class CGSDiagnosticsPlugin : public CGSPluginInterface
    {
    public: // Constructors and destructor

        /**
        * Symbian OS two-phased constructor
        * @return
        */
        static CGSDiagnosticsPlugin* NewL( TAny* aInitParams );

        /**
        * Destructor.
        */
        ~CGSDiagnosticsPlugin();

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
        TBool Visible() const;

    protected: // New

        /**
        * C++ default constructor.
        */
        CGSDiagnosticsPlugin();

        /**
        * Symbian OS default constructor.
        */
        void ConstructL();

    protected: // From CAknView

        /**
        * This implementation is empty because this class, being just a dialog,
        * does not implement the CAknView finctionality.
        */
        void DoActivateL( const TVwsViewId& aPrevViewId,
                          TUid aCustomMessageId,
                          const TDesC8& aCustomMessage );

        /**
        * This implementation is empty because this class, being just a dialog,
        * does not implement the CAknView finctionality.
        */
        void DoDeactivate();

    private: // New

        /**
        * Opens localized resource file.
        */
        void OpenLocalizedResourceFileL(
            const TDesC& aResourceFileName,
            RConeResourceLoader& aResourceLoader );

        /**
        * Prints debug info.
        * @param aInfo info to be printed.
        */
        void PrintInfoDebug( CImplementationInformation* aInfo );

        /**
        * Checks plugin implementations. Uses __UHEAP_MARK and __UHEAP_MARKEND
        * so will panic if plugin leaks memory.
        */
        void CheckInfoL( TUid aInterfaceUid );

    protected: // Data

        // Resource loader.
        RConeResourceLoader iResources;

        // Print implementation info.
        TBool iPrintInfo;

        // Try loading and deleting each plugin.
        TBool iTestLoad;

        // Check heap change in case iTEstLoad in true.
        TBool iCheckHeap;

    };

#endif // GSDIAGNOSTICSPLUGIN_H
// End of File
