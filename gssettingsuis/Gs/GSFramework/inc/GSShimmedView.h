/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Shim view
*
*/


#ifndef GSSHIMMEDVIEW_H
#define GSSHIMMEDVIEW_H

// INCLUDES
#include <e32base.h>
#include <vwsdef.h>
#include <coeview.h>

// CONSTANTS

// FORWARD DECLARATIONS
class CGSPluginAndViewIdCache;

// CLASS DECLARATION

/**
*  CGSShimmedView is used by CGSPluginAndViewIdCache to provide view-framework
*  with all existing plugin views immediately in GS start-up. Shimmed views
*  are used to cache actual views: in case GS view is opened outside GS
*  application while GS is not running in the background, view-framework needs
*  the requested view to exist immediately after GS start-up. This is a problem
*  as GS loads plugins asynchronously after GS start-up. Therefore we provide
*  view-framework with shimmed views in the GS start-up instead of the actual
*  views. Afterwards, when each plugin is loaded, we replace the shimmed view
*  representing the plugin with a real view provided by the loaded plugin.
*  (Each plugin is normally also a view).
*
*  Shimmed views are only used and needed for enabling external GS plugin
*  view launch.
*
*  See CGSPluginAndViewIdCache for details.
*
*  @since Series60_3.1
*
*/
class CGSShimmedView : public CActive, public MCoeView
    {
    public: // Constructor and destructor

        /**
        *
        */
        static CGSShimmedView* NewLC( TUid aViewId, CGSPluginAndViewIdCache& aCache, CCoeAppUi& aAppUi );

        /**
        * Destructor.
        */
        ~CGSShimmedView();

    private:

        /**
        * C++ constructor
        */
        CGSShimmedView( TUid aViewId, CGSPluginAndViewIdCache& aCache, CCoeAppUi& aAppUi );

        /**
        * Second phase constructor
        */
        void ConstructL();

    public: // API

        /**
        *
        */
        void RegisterViewL();

        /**
        *
        */
        void DeregisterView();

    public: // From MCoeView
	    TVwsViewId ViewId() const;
	    void ViewActivatedL( const TVwsViewId& aPrevViewId, TUid aCustomMessageId, const TDesC8& aCustomMessage );
	    void ViewDeactivated();

    private: // State
        enum TState
            {
            EStateInactive = 0,
            EStateActivatingPlaceholderView,
            EStateLoadingPlugin,
            EStateDeletingSelf
            };

        void SetState( TState aState, TBool aCompleteRequest = ETrue );
        void StateActivatePlaceholderViewL();
        void StateLoadPluginL();

    private: // From CActive
        void RunL();
        void DoCancel();

    private: // Data
        TUid iViewId;
        CGSPluginAndViewIdCache& iCache;
        CCoeAppUi& iAppUi;
        TState iState;
        TBool iIsRegistered;
        HBufC8* iCustomMessage;
        TUid iCustomMessageId;
    };


#endif // GSSHIMMEDVIEW_H
// End of File
