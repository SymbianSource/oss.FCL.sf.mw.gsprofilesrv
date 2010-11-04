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
* Description:  GS Application UI.
*
*/


#ifndef GSUI_H
#define GSUI_H

// INCLUDES
#include <aknViewAppUi.h>
#include <aknsettingpage.h>
//#include <ecom/ecom.h>
#include <apgnotif.h>
#include "GSEComNotifier.h"
#include <apadoc.h>


// CLASS DECLARATION
class CAknWaitDialog;
class CGSDocument;

//For embedding CC in CS
class TAppInfo
  {
    public:
      TAppInfo( TUid aUid, const TDesC& aFile )
        : iUid( aUid ), iFile( aFile )
        {}
      TUid iUid;
      TFileName iFile;
  };


/**
* UI class for General Settings application.
* @since Series60_3.1
*/
class CGSUi : public CAknViewAppUi, MGSEComObserver
    {

    public: // Constructors and destructor

        /**
        * C++ default constructor
        */
        CGSUi();

        /**
        * Symbian OS default constructor.
        *
        */
        void ConstructL();

        /**
        * Destructor.
        */
        ~CGSUi();

    public: // New

        /**
        * After successfully selecting a manual network, Phone is activated in
        * Idle state.
        */
        void PhoneIdle();

    /**
    * For embedding CC in CS
    */
    virtual void EmbedAppL( const TAppInfo& aApp );

    /**
    * Get document object
    */
    CGSDocument& GSDocument() const;

    public: // From CEikAppUi
        /**
        * Takes care of command handling.
        * @param aCommand command to be handled
        */
        void HandleCommandL( TInt aCommand );


    protected:  // From CEikAppUi

        /**
        * Message handling from CEikAppUi. See CEikAppUi.
        */
        MCoeMessageObserver::TMessageResponse HandleMessageL(
            TUint32 aClientHandleOfTargetWindowGroup,
            TUid aMessageUid,
            const TDesC8 &aMessageParameters );

    protected: // From MGSEComObserver

        /**
        *  See Base class.
        */
        void HandleEComEvent( TEComEvent aEvent );

    private: // From CEikAppUi

        /**
        * handles key events
        * @param aKeyEvent
        * @param aType type of the key event
        * @return TKeyResponse
        */
        virtual TKeyResponse HandleKeyEventL( const TKeyEvent& aKeyEvent,
                                              TEventCode aType );

    private: // New

        /**
        * Construct views.
        */
        void ConstructViewsL();

        /**
        * @return void
        */
        void ShowNoteGsNotOpenedDuringBackupRestoreL();

        /**
        * Closes embedded application if any exist.
        * @return KErrNone if succeeds.
        */
        TInt FindAndKillEmbeddedAppL();

    private: // From CEikAppUi

        /**
        * Handles resource change.
        */
        void HandleResourceChangeL( TInt aType );

    protected: // Data

        // Pointer to wait dialog.
        CAknWaitDialog* iWaitDialog;

    // Embedding.
        CApaDocument* iEmbedded;
        CAknView* iMainView;

        // Notifies if plugins are installed/uninstalled.
        CGSEComNotifier* iEComNotifier;

    };

#endif // GSUI_H
// End of File
