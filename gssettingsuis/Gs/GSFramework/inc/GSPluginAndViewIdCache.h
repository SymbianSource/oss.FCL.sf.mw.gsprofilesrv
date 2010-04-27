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
* Description:  Plugin and view id caching support
*
*/


#ifndef GSPLUGINANDVIEWIDCACHE_H
#define GSPLUGINANDVIEWIDCACHE_H

// INCLUDES
#include <e32base.h>
#include <vwsdef.h>
#include <f32file.h>
#include <e32hashtab.h>

// CONSTANTS

// FORWARD DECLARATIONS
class CEikAppUi;
class CGSShimmedView;
class CGSPluginInterface;
class CGSPlaceholderView;

// CLASS DECLARATION

/**
*  CGSPluginAndViewIdCache application class.
*   - Creates/Loads a list of CGSShimmedViews from a cache file on GS start-up.
*   - Inserts CGSShimmedViews into view-framework view list.
*
*
*  Problem/rationale:
*  Launching GS plugin view outside GS application when GS application is not
*  already running may fail. This was caused by application inter working
*  combined with GS application's asynchronized plugin loading. High level
*  functionality for external view launching goes:
*   1. Application framework starts desired application
*   2. Application framework calls view-activation on the desired application view.
*   3. Desired view handles the activation call-back by drawing itself on
*      screen or whatever required.
*
*  Starting from phase 2, things can go wrong: when GS start-up call returns, GS
*  has not yet loaded all the plugins and the views owned by plugins are not
*  added yet to the view-framework view list. Calling non-existing view will
*  naturally fail.
*
*
*  Solution:
*  When GS start-up is called, populate view-frameworks view list with all the
*  views provided by GS application before function return. Kind of: This would
*  ruin GS's asynchronous plugin loading and would cause devastating freeze-up
*  times on GS start-up. Hence, on GS start-up we're not actually loading the
*  plugins nor the real views. We use light-weight dummy views
*  (aka CGSShimmedView) wich we add into view-framework view list immedieately
*  on start-up. If it happens so, that the view framework-decides to call view
*  activation on one of these dummy views, dummy view will load manually the
*  real plugin view and switch it with the dummy view. Dummy views are created
*  by CGSPluginAndViewIdCache on GS-application start-up.
*
*
*  Different use scenarios:
*
*  1. First GS-application start on a new device, create cache & save as file.
*
*  2. Launch GS-application second time. Always populate CCoeAppUi with
*     CGSShimmedView instances created from cache file (cache maps view uid
*     to ecom plugin implementation uid).
*     - When pluginloader loads plugin, it replaces dummy view (shim) with the real
*       view from the plugin. The shim is then destroyed.
*
*  3. GS plugin view is activated externally when GS is not running:
*     - Uses normal application framework to launch GS application.
*     - Same functionality as in scenario 2 but if shimmed view is requested
*       before actual view is created, load actual plugin manually and
*       synchronously.
*     - CGSPlaceholderView: View-FW will only switch between two different view
*       uids. This is why we need to switch between 3 different views.
*     -> Will result in 3x fold different view activation.
*     -  Used view activations are:
*        1. Shimmed view DoActivate for view uid <view_uid>.
*        2. Shimmed view calls Placeholder::DoActivate with a dummy "uid DEADBED".
*        3. Placeholder view calls actual plugin::DoActivate with actual view uid
*           <view_uid>.
*
*
*  NOTE: Requirements:
*  - Cache: GS has to be launched once successfully for the cache to work.
*    -> otherwise error if launching from external application.
*  - Views which need to be used externally (external view launch) must be
*    implemented as a GS-plugin. Those cannot be implemented as a local view.
*
*
*  @since Series60_3.1
*
*/
class CGSPluginAndViewIdCache : public CBase
    {
    public: // Constructor and destructor

        /**
        *
        */
        static CGSPluginAndViewIdCache* NewL( CEikAppUi& aAppUi );

        /**
        * Destructor.
        */
        ~CGSPluginAndViewIdCache();

    private:

        /**
        * C++ constructor
        */
        CGSPluginAndViewIdCache( CEikAppUi& aAppUi );

        /**
        * Second phase constructor
        */
        void ConstructL();

    public: // API - placeholder view related

        /**
        *
        */
        void ActivatePlaceholderViewL();

        /**
        *
        */
        const TVwsViewId& PriorToPlaceholderActiveViewId() const;

        /**
        *
        */
        void SetPriorToPlaceholderActiveViewId( const TVwsViewId& aViewId );

    public: // API - caching

        /**
        *
        */
        void RegisterViewAndImplementationAssociationL( TUid aViewId, TUid aImplementationUid );

        /**
        *
        */
        void RegisterViewAndImplementationAssociationL( TUid aViewId, TUid aImplementationUid, TBool aCreateShim );

        /**
        *
        */
        void PrepareToRegisterPluginInstanceLC( TUid aViewId );

        /**
        *
        */
        void RegisterPluginInstanceL( TUid aViewId, CGSPluginInterface& aPluginInstance );

        /**
        *
        */
        TBool IsPluginLoaded( TUid aViewId ) const;

        /**
        *
        */
        CGSPluginInterface* PluginInstance( TUid aViewId ) const;

        /**
        *
        */
        TUid PluginImplementationUidForView( TUid aViewId ) const;

        /**
        *
        */
        void PrepareForUIDestruction();

        /**
        *
        */
        void HandleShimDestruction( TUid aViewId );

    private: // Internal methods
        void DestroyShims();
        void RestoreL();
        void StoreL();
        void GetCacheFileNameL( TDes& aFileName );
        void GetCachePathL( TDes& aPath );
        static void ReRegisterShimWithUi( TAny* aSelf );

    private: // Hash function support
        static TUint32 HashFunction( const TUid& aViewId );
        static TBool HashIdentityRelation( const TUid& aLeft, const TUid& aRight );

    private: // Internal object

        struct TCacheEntry
            {
            TUid iImplementationUid;
            CGSShimmedView* iShim;
            CGSPluginInterface* iLoadedPluginImplementation;
            };

    private: // Data

        // UI - not owned
        CEikAppUi& iAppUi;

        // File server session - ref
        RFs& iFsSession;

        // Cache table - owned
        RHashMap< TUid /* KEY = view id */, TCacheEntry /* VALUE */ > iLookupTable;

        // Placeholder view - owned
        CGSPlaceholderView* iPlaceholderView;

        // Transient variable used during plugin registration preparation.
        TUid iTransientCleanupViewUid;
    };


#endif // GSBASEDOCUMENT_H
// End of File
