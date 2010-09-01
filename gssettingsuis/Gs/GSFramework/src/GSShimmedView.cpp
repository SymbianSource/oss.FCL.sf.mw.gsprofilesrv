/*
* Copyright (c) 2006-1008 Nokia Corporation and/or its subsidiary(-ies).
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

#include "GSShimmedView.h"

// System includes
#include <eikenv.h>
#include <aknViewAppUi.h>

// User includes
#include "GSPluginAndViewIdCache.h"
#include <gsfwviewuids.h>
#include <gspluginloader.h>
#include <gsplugininterface.h>

// ================= MEMBER FUNCTIONS =======================

// ----------------------------------------------------------------------------
// CGSShimmedView::CGSShimmedView
//
//
// ----------------------------------------------------------------------------
//
CGSShimmedView::CGSShimmedView( TUid aViewId, 
                                CGSPluginAndViewIdCache& aCache, 
                                CCoeAppUi& aAppUi )
:   CActive( KMaxTInt ), iViewId( aViewId ), iCache( aCache ), iAppUi( aAppUi )
    {
    }


// ----------------------------------------------------------------------------
// CGSShimmedView::~CGSShimmedView
//
//
// ----------------------------------------------------------------------------
//
CGSShimmedView::~CGSShimmedView()
    {
    Cancel();
    DeregisterView();
    iCache.HandleShimDestruction( iViewId );
    delete iCustomMessage;
    }


// ----------------------------------------------------------------------------
// CGSShimmedView::ConstructL
//
//
// ----------------------------------------------------------------------------
//
void CGSShimmedView::ConstructL()
    {
    RegisterViewL();
    }


// ----------------------------------------------------------------------------
// CGSShimmedView::NewLC
//
//
// ----------------------------------------------------------------------------
//
CGSShimmedView* CGSShimmedView::NewLC( TUid aViewId, 
                                       CGSPluginAndViewIdCache& aCache, 
                                       CCoeAppUi& aAppUi )
    {
    CGSShimmedView* self = new(ELeave) CGSShimmedView( aViewId, aCache, aAppUi );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ----------------------------------------------------------------------------
// CGSShimmedView::ViewId
//
//
// ----------------------------------------------------------------------------
//
TVwsViewId CGSShimmedView::ViewId() const
    {
    const TVwsViewId ret( KUidGS, iViewId );
    return ret;
    }


// ----------------------------------------------------------------------------
// CGSShimmedView::ViewActivatedL
//
//
// ----------------------------------------------------------------------------
//
void CGSShimmedView::ViewActivatedL( const TVwsViewId& aPrevViewId, 
                                     TUid aCustomMessageId, 
                                     const TDesC8& aCustomMessage )
    {
    const TBool isLoaded = iCache.IsPluginLoaded( iViewId );
    if  ( isLoaded == EFalse )
        {
        HBufC8* customMessage = aCustomMessage.AllocL();
        delete iCustomMessage;
        iCustomMessage = customMessage;

        iCustomMessageId = aCustomMessageId;

        iCache.SetPriorToPlaceholderActiveViewId( aPrevViewId );
        SetState( EStateActivatingPlaceholderView );
        }
    }


// ----------------------------------------------------------------------------
// CGSShimmedView::ViewDeactivated
//
//
// ----------------------------------------------------------------------------
//
void CGSShimmedView::ViewDeactivated()
    {
    }


// ----------------------------------------------------------------------------
// CGSShimmedView::RunL
//
//
// ----------------------------------------------------------------------------
//
void CGSShimmedView::RunL()
    {
    // NB: At this point, we are the active view.

    switch( iState )
        {
    default:
    case EStateInactive:
        break;

    case EStateActivatingPlaceholderView:
        StateActivatePlaceholderViewL();
        break;

    case EStateLoadingPlugin:
        StateLoadPluginL();
        break;

    case EStateDeletingSelf:
        SetState( EStateInactive, EFalse );
        delete this;
        break;
        }
    }


// ----------------------------------------------------------------------------
// CGSShimmedView::RegisterViewL
//
//
// ----------------------------------------------------------------------------
//
void CGSShimmedView::RegisterViewL()
    {
    iAppUi.RegisterViewL( *this );
    iIsRegistered = ETrue;
    }


// ----------------------------------------------------------------------------
// CGSShimmedView::DeregisterView
//
//
// ----------------------------------------------------------------------------
//
void CGSShimmedView::DeregisterView()
    {
    if  ( iIsRegistered )
        {
        iAppUi.DeregisterView( *this );
        }
    iIsRegistered = EFalse;
    }



// ----------------------------------------------------------------------------
// CGSShimmedView::DoCancel
//
//
// ----------------------------------------------------------------------------
//
void CGSShimmedView::DoCancel()
    {
    // Nothing to do here - requests already completed
    }


// ----------------------------------------------------------------------------
// CGSShimmedView::SetState
//
//
// ----------------------------------------------------------------------------
//
void CGSShimmedView::SetState( TState aState, TBool aCompleteRequest )
    {
    Cancel();
    //
    iState = aState;
    //
    if  ( aCompleteRequest )
        {
        if  ( !IsAdded() )
            {
            // Add only on-demand to avoid cluttering the scheduler
            CActiveScheduler::Add( this );
            }

        // Complete ourselves to continue activation process asynchronously
        // via RunL().
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, KErrNone );
        SetActive();
        }
    }


// ----------------------------------------------------------------------------
// CGSShimmedView::StateActivatePlaceholderViewL
//
//
// ----------------------------------------------------------------------------
//
void CGSShimmedView::StateActivatePlaceholderViewL()
    {
    iCache.ActivatePlaceholderViewL();
    SetState( EStateLoadingPlugin );
    }


//Disabling warning caused by err variable used inside macros
#pragma diag_suppress 550
// ----------------------------------------------------------------------------
// CGSShimmedView::StateLoadPluginL
//
//
// ----------------------------------------------------------------------------
//
void CGSShimmedView::StateLoadPluginL()
    {
    CAknViewAppUi& appUi = static_cast< CAknViewAppUi& >( iAppUi );

    // As a fall back, we'll try to activate the view that was being
    // displayed prior to us attempting to load a plugin.
    TUid viewIdToActivate = iCache.PriorToPlaceholderActiveViewId().iViewUid;

    // Get the implementation uid that contains this view.
    const TUid impUid = iCache.PluginImplementationUidForView( iViewId );

    if  ( impUid != KNullUid )
        {
        CGSPluginLoader* loader = CGSPluginLoader::NewL( &appUi );
        CleanupStack::PushL( loader );

        // Now try to load the specific instance of the GS plugin that
        // implements the real view's concrete implementation.
        //
        // Since the act of loading a plugin will also attempt to register
        // that plugin with the app ui (since a GS plugin "is a" view) we
        // must first unregister the shim view. If there was an error
        // loading the plugin, then we'll re-register the shim.
        DeregisterView();
       
        CGSPluginInterface* plugin = NULL;
        TRAPD( loadError, plugin = &loader->LoadSyncL( KGSPluginInterfaceUid, impUid ) );
        CleanupStack::PopAndDestroy( loader );

        if  ( loadError == KErrNone )
            {
            // .. and set us up to activate the newly loaded plugin view.
            viewIdToActivate = iViewId;

            // Set ourselves up to be destroyed in the next RunL callback
            SetState( EStateDeletingSelf );
            }
        else
            {
            // Didn't manage to load plugin. Re-register the view
            // and bail out.
            SetState( EStateInactive, EFalse );
            RegisterViewL();
            }
        }

    appUi.ActivateLocalViewL( viewIdToActivate, iCustomMessageId, *iCustomMessage );
    }
//Enabling warnings
#pragma diag_default 550





//  End of File
