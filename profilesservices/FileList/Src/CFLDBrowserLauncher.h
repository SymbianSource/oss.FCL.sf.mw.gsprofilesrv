/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for Download tones feature
*
*/



#ifndef CFLDBROWSERLAUNCHER_H
#define CFLDBROWSERLAUNCHER_H

// EXTERNAL INCLUDES
#include <e32base.h>
#include <downloadedcontenthandler.h>

// FORWARD DECLARATIONS
class CDocumentHandler;
class CBrowserLauncher;

// CLASS DECLARATION

/**
*  Implementation for Download tones feature.
*
*  @lib FileList.lib
*  @since 2.1
*/
NONSHARABLE_CLASS( CFLDBrowserLauncher ) : public CBase,
 public MDownloadedContentHandler
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CFLDBrowserLauncher* NewLC();

        virtual ~CFLDBrowserLauncher();

    public: // New functions
        
        /**
        * Launches Browser embedded. Waits until Browser is closed.
        * @since 2.1
        */
        void LaunchBrowserL();

    public: // Functions from base classes
    
		TBool HandleContentL(
		 const TDesC& aFileName,
	  		const CAiwGenericParamList& aParamList, TBool& aContinue );
		
		TAny* DownloadedContentHandlerReserved1( TAny* aAnyParam );

    private:
		
		/**
		* Default constructor
		*/
        CFLDBrowserLauncher();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // Data
        /// Own: Browser Launcher
        CBrowserLauncher* iBrowserLauncher;

        /// Own: Document Handler for saving downloaded tones.
        CDocumentHandler* iDocumentHandler;

    };

#endif      // CFLDBROWSERLAUNCHER_H
            
// End of File
