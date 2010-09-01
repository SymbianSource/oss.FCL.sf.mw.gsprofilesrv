/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  GS plugin loader.
*
*/


// INCLUDE FILES
#include    <gspluginloader.h>
#include    <gsplugininterface.h>
#include    <gsprivatepluginproviderids.h>
#include    "GsLogger.h"
#include    "GSBaseDocument.h"
#include    "GSPluginWrapper.h"
#include    "GSPluginAndViewIdCache.h"
#include    <mgswatchdog.h>

#include    <aknViewAppUi.h>
#include    <AknIconArray.h>
#include    <aknlists.h> // CAknSingleLargeStyleListbox
#include    <eikclbd.h> // CColumnListBoxData
#include    <gulicon.h> // For CGulIcon
#include    <utf.h> // CnvUtfConverter
#include  	<basched.h>
#include    <AknInfoPopupNoteController.h>

//Flag for enabling/disablign compare by category feature
#undef RD_GS_COMPARE_BY_CATEGORY

// ================= MEMBER FUNCTIONS =======================

#ifdef _DEBUG
    #pragma message("-----_DEBUG ACTIVATED IN GS!-----")
#endif //_GEBUG

#ifdef _GS_PERFORMANCE_TRACES
    #pragma message("-GS plugin load performance measurements activated-")
#endif // _GS_PERFORMANCE_TRACES

const TInt KGSCaptionSize = 256;

// ----------------------------------------------------------------------------
// CGSPluginLoader::NewL
//
// EPOC two-phased constructor
// ----------------------------------------------------------------------------
//
EXPORT_C CGSPluginLoader* CGSPluginLoader::NewL( CAknViewAppUi* aAppUi )
    {
    CGSPluginLoader* self = new( ELeave ) CGSPluginLoader;
    CleanupStack::PushL( self );
    self->ConstructL( aAppUi );
    CleanupStack::Pop( self );
    return self;
    }


// ----------------------------------------------------------------------------
// CGSPluginLoader::CGSPluginLoader
//
// C++ default constructor can NOT contain any code, that
// might leave.
//
//  Uses low priority for Active Object so that more bandwith is available for 
//  the most important pluginloaders. 
// ----------------------------------------------------------------------------
//
CGSPluginLoader::CGSPluginLoader() 
    : CActive( EPriorityLow ),
      iRequestedPriority( EPriorityLow )
    {
    __GSLOGSTRING( "[GSPlgLoader] CGSPluginLoader()" );
    }


// ----------------------------------------------------------------------------
// CGSPluginLoader::ConstructL
//
// EPOC default constructor can leave.
// ----------------------------------------------------------------------------
//
EXPORT_C void CGSPluginLoader::ConstructL( CAknViewAppUi* aAppUi )
    {
    __GSLOGSTRING( "[GSPlgLoader] ConstructL()" );
    iAppUi = aAppUi;
    iDocument = static_cast<CGSBaseDocument*>( iAppUi->Document() );
    iImplInfoArray = iDocument->GetImplInfo();

    CActiveScheduler* scheluder = CActiveScheduler::Current();
    __GSLOGSTRING1( "[GSPlgLoader] Current CActiveScheduler:0x%X", scheluder );
    __GSLOGSTRING1( "[GSPlgLoader] CActiveScheduler stackdepth: %d",
        scheluder->StackDepth() );

    CActiveScheduler::Add( this );

    iWatchDog = iDocument->WatchDog();
    }


// ----------------------------------------------------------------------------
// CGSPluginLoader::~CGSPluginLoader
//
// Destructor
// ----------------------------------------------------------------------------
//
EXPORT_C CGSPluginLoader::~CGSPluginLoader()
    {
    __GSLOGSTRING( "[GSPlgLoader] ~CGSPluginLoader()" );
    AbortAsyncLoad();
    Cancel();

    if( iErrorPopup )
        {
        iErrorPopup->HideInfoPopupNote();
        delete iErrorPopup;
        iErrorPopup = NULL;
        }
    //iPluginArray is not owned and therefore not deleted.
    }


// ----------------------------------------------------------------------------
// CGSPluginLoader::LoadAsync
//
//
// ----------------------------------------------------------------------------
//
EXPORT_C void CGSPluginLoader::LoadAsyncL(
    TUid aInterfaceUid,
    TUid aParentUid,
    CArrayPtrFlat<CGSPluginInterface>* aPluginArray )
    {
    iPluginArray = aPluginArray;
    iParentUid = aParentUid;

    __GSLOGSTRING3(
        "[GSPlgLoader] LoadAsync(). aInterfaceUid:0x%X aParentUid:0x%X, aPluginArray:0x%X",
        aInterfaceUid, aParentUid, aPluginArray );
    // Reset iterator:
    iImplInfoArrayIterator = 0;

    __GSLOGSTRING1( "[GSPlgLoader] Implementation info count: %d",
        iImplInfoArray.Count() );

    NotifyProgress();

    //Begin CActive asynchronous loop.
    CompleteOwnRequest();
    }


// ----------------------------------------------------------------------------
// CGSPluginLoader::LoadSyncL
//
//
// ----------------------------------------------------------------------------
//
CGSPluginInterface& CGSPluginLoader::LoadSyncL( TUid aInterfaceUid,
                                                TUid aImplementationUid )
    {
    Cancel();
    CGSPluginInterface* ret = NULL;

    // Get a list of all implementations, even though we only want one specific.
    // There appears to be no way to otherwise extract a specific implementation
    // info object :(
    // Search for the implementation that matches aImplementationUid
    const TInt impCount = iImplInfoArray.Count();
    for( TInt i=0; i<impCount; i++ )
        {
        const CImplementationInformation* info = iImplInfoArray[ i ];
        if  ( info->ImplementationUid() == aImplementationUid )
            {
            ret = &CreatePluginInstanceL( *info );
            break;
            }
        }

    if  ( !ret )
        {
        User::Leave( KErrNotFound );
        }
    return *ret;
    }


// ----------------------------------------------------------------------------
// CGSPluginLoader::AbortAsyncLoad
//
//
// ----------------------------------------------------------------------------
//
EXPORT_C void CGSPluginLoader::AbortAsyncLoad()
    {
    __GSLOGSTRING( "[GSPlgLoader] AbortAsyncLoad()" );
    Cancel();
    }


// ----------------------------------------------------------------------------
// CGSPluginLoader::RunL
//
//
// ----------------------------------------------------------------------------
//
void CGSPluginLoader::RunL()
    {
    iRunLDebugCount++;
    
    // This must be done only in RunL or otherwise request might be 
    // outstanding.
    if( iRequestedPriority != Priority() )
        {
        __GSLOGSTRING3(
            "[CGSPluginLoader::RunL] 0x%X original priority:%d requested priority:%d", 
            iParentUid.iUid, 
            Priority(),
            iRequestedPriority );
        
        SetPriority( iRequestedPriority );
        }
    
    LoadNextPluginL();

    // Check if there are still more plugins to be loaded:
    if ( iImplInfoArrayIterator < iImplInfoArray.Count() )
        {
        NotifyProgress();
        // Continue CActive asynchronous loop.
        CompleteOwnRequest();
        }
    else
        {
        // All plugins loaded:
        __GSLOGSTRING( "[GSPlgLoader] Loading plugins finished." );
        NotifyFinished();
        }
    }


// ---------------------------------------------------------------------------
// CScGenreItemConstructionConductor::CompleteOwnRequest
//
// Issue request complete notification.
// ---------------------------------------------------------------------------
void CGSPluginLoader::CompleteOwnRequest()
    {
    TRequestStatus* status = &iStatus;
    User::RequestComplete( status, KErrNone );
    SetActive();
    }


// ----------------------------------------------------------------------------
// CGSPluginLoader::RunError
//
//
// ----------------------------------------------------------------------------
//
TInt CGSPluginLoader::RunError( TInt aError )
    {
    // This method is called when a plugin loading fails.
    // Always "fake" the return value so that ActiveSchedule
    // keeps running and later plugins are continued to be loaded.
    // Check if still plugins to be loaded:
    if( iImplInfoArrayIterator < iImplInfoArray.Count() )
        {
        NotifyProgress();

        //Continue CActive asynchronous loop.
        CompleteOwnRequest();
        }
    else // All plugins loaded:
        {
        NotifyFinished();
        }

    if ( aError == KLeaveExit )
        {
        return KLeaveExit;
        }

    return KErrNone;
    }


// ----------------------------------------------------------------------------
// CGSPluginLoader::DoCancel
//
//
// ----------------------------------------------------------------------------
//
void CGSPluginLoader::DoCancel()
    {

    }


// ----------------------------------------------------------------------------
// CGSPluginLoader::NotifyProgress
//
//
// ----------------------------------------------------------------------------
//
void CGSPluginLoader::NotifyProgress()
    {
    if( iObserver )
        {
        iObserver->HandlePluginLoaded( MGSPluginLoadObserver::EGSSuccess);
        }
    }


// ----------------------------------------------------------------------------
// CGSPluginLoader::NotifyFinished
//
//
// ----------------------------------------------------------------------------
//
void CGSPluginLoader::NotifyFinished()
    {

    #ifdef _GS_PLUGINLOADER_FINAL_SORTING_TRACES
        TRAP_IGNORE( PrintOrderTracesL( iPluginArray ) );
    #endif // _GS_PLUGINLOADER_FINAL_SORTING_TRACES

    // I have finished loading: No need to keep me in the expensive scheduler
    // queue.
    Deque();

    if( iObserver )
        {
        iObserver->HandlePluginLoaded( MGSPluginLoadObserver::EGSFinished );
        }
    
    __GSLOGSTRING1( "[CGSPluginLoader::NotifyFinished] 0x%X", 
                    iParentUid.iUid );
    }


// ----------------------------------------------------------------------------
// CGSPluginLoader::SetObserver
//
//
// ----------------------------------------------------------------------------
//
EXPORT_C void CGSPluginLoader::SetObserver(MGSPluginLoadObserver* aObserver)
    {
    __GSLOGSTRING1("[GSPlgLoader] Observer set:0x%X", aObserver);
    iObserver = aObserver;
    }


// ----------------------------------------------------------------------------
// CGSPluginLoader::ParseToUid
// Parses a UID from descriptor of form '0xNNNNNNNN' where N is hexadecimal.
//
// ----------------------------------------------------------------------------
//
TInt CGSPluginLoader::ParseToUid( const TDesC8& aSource, TUid& aTarget )
    {
    // Remove "0x" from the descriptor if it exists
    _LIT8(KHexPrefix, "0x");

    TPtrC8 pSource( aSource );
    const TInt prefixPosition = pSource.Find( KHexPrefix );
    if  ( prefixPosition != KErrNotFound )
        {
        pSource.Set( aSource.Mid( prefixPosition + KHexPrefix().Length() ) );
        }

    // Parse to integer
    TLex8 lex( pSource );
    TUint integer = 0;

    // Parse using TRadix::EHex as radix:
    const TInt err = lex.Val( integer, EHex );
    aTarget.iUid = integer;

    if( err != KErrNone )
        {
        // If parsing parent UID failed, do not load plugin:
        __GSLOGSTRING1(
            "[GSPlgLoader] Parsing parent UID failed. Error code:%d",
            err );
        }
    return err;
    }


// ----------------------------------------------------------------------------
// CGSPluginLoader::ParseOrderNumber
//
//
// ----------------------------------------------------------------------------
//
TInt CGSPluginLoader::ParseOrderNumber( const TDesC8& aSource, TInt& aOrderNumber )
    {
    // Parse plugin's order number from opaque_data:
    TLex8 lex( aSource );
    const TInt orderErr = lex.Val( aOrderNumber );
    return orderErr;
    }


// ----------------------------------------------------------------------------
// CGSPluginLoader::PrintInfoDebug
// Print CImplementationInformation to log. Used for debugging.
//
// ----------------------------------------------------------------------------
//
void CGSPluginLoader::PrintInfoDebugL( const CImplementationInformation& aInfo,
                                      TInt aIterator,
                                      TInt aPluginCount )
    {


    #pragma message("-CGSPluginLoader verbose traces activated-")
    __GSLOGSTRING2( "[GSPlgLoader::LoadNextPluginL] %d/%d",
                    aIterator,
                    aPluginCount);

    __GSLOGSTRING1( "[GSPlgLoader] --Plugin 0x%X info--", &aInfo);
    __GSLOGSTRING1( "[GSPlgLoader] DisplayName:%S", &aInfo.DisplayName() );
    __GSLOGSTRING1( "[GSPlgLoader] ImplementationUid:0x%X",
                    aInfo.ImplementationUid() );

    const TInt KMaxEComDataLength = 256;

    HBufC* dataType = HBufC::New( KMaxEComDataLength );
    HBufC* opaqueData = HBufC::New( KMaxEComDataLength );

    dataType->Des().Copy( aInfo.DataType() );
    opaqueData->Des().Copy( aInfo.OpaqueData() );
    __GSLOGSTRING1( "[GSPlgLoader] DataType:%S", dataType );
    __GSLOGSTRING1( "[GSPlgLoader] OpaqueData:%S", opaqueData );

    delete opaqueData;
    delete dataType;
    }


// ----------------------------------------------------------------------------
// CGSPluginLoader::LoadNextPluginL
// Iterate through iImplInfoArray. Load the plugin if it is eligible for
// loading. Loaded plugin is added to iPluginArray. Each time a plugin is
// loaded, iObserver is notified.
//
// ----------------------------------------------------------------------------
//
EXPORT_C void CGSPluginLoader::LoadNextPluginL()
    {
    // Iterate through iImplInfoArray. This loop continues between function
    // calls. Therefore member variable iImplInfoArrayIterator is used as a
    // counter. Loop will break when match is found and continues on next RunL.
    for( ; iImplInfoArrayIterator < iImplInfoArray.Count();  )
        {
        const CImplementationInformation* info =
            iImplInfoArray[ iImplInfoArrayIterator ];

        iImplInfoArrayIterator++;

        #ifdef _GS_PLUGINLOADER_VERBOSE_TRACES
            PrintInfoDebugL( *info, iImplInfoArrayIterator, iImplInfoArray.Count() );
        #endif //_GS_PLUGINLOADER_VERBOSE_TRACES

        // Parse parent UID from default_data:
        TUid parentUid;
        const TInt uidErr = ParseToUid( info->DataType(), parentUid );

        if( uidErr == KErrNone  && iParentUid == parentUid )
            {
            // If this plugin is OK -> load it:
            __GSLOGSTRING2( "[GSPlgLoader] %S eligible for parent 0x%X",
                    &info->DisplayName(), iParentUid.iUid );
            CGSPluginInterface* plugin = NULL;
            TInt error = KErrCancel;
        #ifdef GS_ENABLE_WATCH_DOG
            if( !iWatchDog->IsInBlackList( info->ImplementationUid() ) )
        #endif
                {
                // Only panics move quarantined plugins to blacklist. Leaving is
                // normal programmatic functionality and therefore does not move
                // plugin to blacklist.
                #ifdef GS_ENABLE_WATCH_DOG
                    iWatchDog->QuarantineL( info->ImplementationUid() );
                #endif

                #ifdef _GS_PERFORMANCE_TRACES
                    TTime timeStart;
                    TTime timeEnd;
                    timeStart.HomeTime();
                #endif //_GS_PERFORMANCE_TRACES

                // Create plugin. Trap leave for debugging purposes.
                TRAP( error, plugin = &CreatePluginInstanceL( *info ); );

                #ifdef _GS_PERFORMANCE_TRACES
                    timeEnd.HomeTime();
                    TTimeIntervalMicroSeconds funcDuration = timeEnd.MicroSecondsFrom( timeStart );
                    __GSLOGSTRING2( "[GSPlgLoader::LoadNextPluginL/perf] %Ld    (%S)", funcDuration, &info->DisplayName() );
                #endif //_GS_PERFORMANCE_TRACES

                #ifdef GS_ENABLE_WATCH_DOG
                    TRAP_IGNORE( iWatchDog->RemoveFromQuarantineL( info->ImplementationUid() ); );
                #endif
                }
            if( error == KErrNone )
                {
                // Plugin ownership is transfered to iPluginArray
                InsertPluginInOrderL( plugin, iPluginArray );
                }
            else if( error == KLeaveExit )
                {
                __GSLOGSTRING( "[GSPlgLoader::LoadNextPluginL] LEAVE: KLeaveExit!!!" );
                // Must pass KLeaveExit through or otherwise Exit-command will
                // not be handled.
                User::Leave( KLeaveExit );
                }
            else
                {
                // Error note is displayed even if plugin is not loaded
                // -> plugin is in blacklist -> blacklist note displayed.
                #ifdef _DEBUG
                    DisplayErrorPopupL( error, info );
                #endif //_DEBUG
                }
            // Wait for next round
            break;
            }
        }
    }


// ----------------------------------------------------------------------------
// CGSPluginLoader::DisplayErrorPopupL
//
// ----------------------------------------------------------------------------
//
void CGSPluginLoader::DisplayErrorPopupL(
        TInt aError,
        const CImplementationInformation* aInfo )
    {

    // Log error:
    __GSLOGSTRING3( "[GSPlgLoader] %S (0x%X) loading failed with error code %d",
                    &aInfo->DisplayName(),
                    aInfo->ImplementationUid().iUid,
                    aError );

    // Runtime info message used only in _DEBUG builds.
    // Buffer needs space for about 50 chars for debug text
    // + 8 chars for hex UID
    // + DisplayName().Length()
    // + error code 10 chars...
    HBufC* buf = HBufC::NewLC( 100 + aInfo->DisplayName().Length() );
    TPtr ptr = buf->Des();

    if( aError == KErrCancel )
        {
        _LIT( KDbgMsgBlacklisted, "Plugin in blacklist:\n%S (0x%X)" );
        ptr.Format( KDbgMsgBlacklisted,
                    &aInfo->DisplayName(),
                    aInfo->ImplementationUid().iUid );
        }
    else
        {
        _LIT( KDbgMsg, "Error:\n%S (0x%X)\nloading failed with error code %d" );
        ptr.Format( KDbgMsg,
                    &aInfo->DisplayName(),
                    aInfo->ImplementationUid().iUid,
                    aError );
        }
    if( iErrorPopup )
        {
        delete iErrorPopup;
        iErrorPopup = NULL;
        }
    iErrorPopup = CAknInfoPopupNoteController::NewL();
    iErrorPopup->SetTextL( ptr );
    CleanupStack::PopAndDestroy( buf );
    iErrorPopup->ShowInfoPopupNote();
    }

// ----------------------------------------------------------------------------
// CGSPluginLoader::CreatePluginInstanceL
//
//
// ----------------------------------------------------------------------------
//
CGSPluginInterface& CGSPluginLoader::CreatePluginInstanceL(
    const CImplementationInformation& aImpInfo )
    {
    // Create a wrapper for the plugin - this will take
    // care of cleaning up the plugin dll. This must be on
    // the cleanup stack above the plugin object itself - since
    // the cleanup stack will first delete the plugin, then the
    // plugin wrapper will be called to unload the ECOM plugin
    // dll itself.
    CGSPluginWrapper* wrapper = iDocument->NewPluginUnloadWrapperLC();

    // Now we can load the plugin
    const TUid implUid = aImpInfo.ImplementationUid();

    CGSPluginInterface* plugin = CGSPluginInterface::NewL( implUid,
                                                           iAppUi );// Remove iAppUi
    CleanupStack::PushL ( plugin );

    // And now its okay to update the wrapper with the plugin's
    // ECOM destructor info.
    wrapper->SetDetails( plugin->iDtor_ID_Key );

    // If plugin's resource definition had a valid order number,
    // set it to plugin.
    //
    // If parent is GSAppsPlugin, this is where the ordering
    // could be changed to follow the to-be-implemented resource file
    // defining the plugin order.

    // Parse plugin's order number from opaque_data:
    TInt orderNumber = 0;
    const TInt orderErr = ParseOrderNumber( aImpInfo.OpaqueData(), orderNumber );
    if  ( orderErr == KErrNone && orderNumber >= 0 )
        {
        plugin->iOrder = orderNumber;
        }

    // When a specific view has been activated externally to GS (e.g
    // whilst GS is not running) we have to load the view's plugin on-demand.
    // This means that the plugin is essentially free-floating and is not bound
    // to a parent's CGSParentPlugin::iPluginArray.
    //
    // In this situation, even though we have activated a specific view,
    // the plugin loader(s) continue to operate in the background.
    //
    // Eventually, as the loaders progress, a second attempt will occur to
    // load the same view (that was activated externally).
    //
    // In this situation, we discard the recently loaded instance of the plugin
    // and instead preserve the view that the user is (presumably) already using.
    //
    // However, we must ensure that we synchronous the parent's iPluginArray
    // with the pre-existing view instance.
    const TUid viewId = plugin->Id();
    CGSPluginAndViewIdCache& pluginViewIdCache = iDocument->PluginViewIdCache();
    const TBool isAlreadyLoaded = pluginViewIdCache.IsPluginLoaded( viewId );

    if  ( !isAlreadyLoaded )
        {
        // Cache the view uid & ECOM plugin dll uid to enable us
        // to more quickly handle external view activation requests
        // when GS is not already running.
        pluginViewIdCache.RegisterViewAndImplementationAssociationL( viewId, implUid );

        // Prepare to register the plugin with the cache.  Pushes a cleanup stack
        // item in case adding the view should leave.
        pluginViewIdCache.PrepareToRegisterPluginInstanceLC( viewId );

        // Add loaded view to appUi. At this point, the GS framework
        // is no longer responsible for the lifetime of the 'plugin'
        // object.
        //
        // However, to ensure sucessful cleanup of deleted
        // views, we have separated the ECOM plugin dll (controlled by
        // plugin->iDtor_ID_Key) to a separate object. This object is
        // owned by the loader.
        iAppUi->AddViewL( plugin );

        // Pop the cleanup item - all is well now.
        CleanupStack::Pop(); // cleanup item from PrepareToRegisterPluginInstanceLC

        CleanupStack::Pop( plugin ); // view framework is now responsible for this memory.

        // Document, when it is destroyed, will handle unloading of ECOM dll.
        CleanupStack::Pop( wrapper );

        // Also register that we have loaded an instance of the specified view
        // from a plugin.
        pluginViewIdCache.RegisterPluginInstanceL( viewId, *plugin );
        }
    else
        {
        // Plugin is already loaded, presumably due to external view activation
        // request.
        //
        // Discard "just loaded" instance and use the pre-loaded one instead.
        CleanupStack::PopAndDestroy( 2, wrapper );
        plugin = pluginViewIdCache.PluginInstance( viewId );
        }

    return *plugin;
    }

// ----------------------------------------------------------------------------
// CGSPluginLoader::SortPluginsL
//
// ----------------------------------------------------------------------------
//
EXPORT_C void CGSPluginLoader::SortPluginsL(
        CArrayPtrFlat<CGSPluginInterface>* aPlugins )
    {
    RPointerArray<CGSPluginInterface> plugins;
    TLinearOrder<CGSPluginInterface> order( CGSPluginLoader::Compare );

    // Insertion will also order
    for( TInt i = 0; i < aPlugins->Count(); i++ )
        {
        plugins.InsertInOrderL( (*aPlugins)[i], order );
        }

    // Replace original array content with sorted items
    aPlugins->Reset();
    for( TInt i = 0; i < plugins.Count(); i++ )
        {
        aPlugins->AppendL( plugins[i] );
        }
    }


// ----------------------------------------------------------------------------
// CGSPluginLoader::Compare
//
// Compare two plugins.
// Precedence:
// [1. plugin provider category]
// 2. plugin order number
// 3. plugin caption
// Plugin provider gategory is currently disabled (not supported yet).
// ----------------------------------------------------------------------------
//
TInt CGSPluginLoader::Compare( const CGSPluginInterface& aFirst,
                               const CGSPluginInterface& aSecond )
    {
    TInt comparison = CompareCategory( aFirst, aSecond );
    if( comparison == KGSComparisonEqual )
        {
        comparison = CompareIndex( aFirst, aSecond );
        if( comparison == KGSComparisonEqual )
            {
            comparison = CompareCaption( aFirst, aSecond );
            }
        }
    return comparison;
    }


// ----------------------------------------------------------------------------
// CGSPluginLoader::InsertPluginInOrderL
//
// ----------------------------------------------------------------------------
//
void CGSPluginLoader::InsertPluginInOrderL(
    CGSPluginInterface* aPlugin,
    CArrayPtrFlat<CGSPluginInterface>* aPlugins )
    {
    CGSPluginInterface* comparedPlugin;
    TInt comparison = 0;
    TBool inserted = EFalse;

    for( TInt i = 0; i < aPlugins->Count(); i++ )
        {
        comparedPlugin = (*aPlugins)[i];
        // Optimization: do not call time consuming Compare() multiple times!
        comparison = Compare( *aPlugin, *comparedPlugin );
        if( comparison < 0 )
            {
            aPlugins->InsertL( i, aPlugin );
            inserted = ETrue;
            break;
            }
        else if( comparison == 0 )
            {
            aPlugins->InsertL( i+1, aPlugin );
            inserted = ETrue;
            break;
            }
        }
    // Plugin was not before any other plugin - make sure it's appended
    if( !inserted )
        {
        aPlugins->AppendL( aPlugin );
        }

    #ifdef _GS_PLUGINLOADER_ITERATION_SORTING_TRACES
        PrintOrderTracesL( aPlugins );
    #endif // _GS_PLUGINLOADER_ITERATION_SORTING_TRACES

    }

// ----------------------------------------------------------------------------
// CGSPluginLoader::CompareCategory
//
// ----------------------------------------------------------------------------
//
void CGSPluginLoader::PrintOrderTracesL(
        CArrayPtrFlat<CGSPluginInterface>* aPlugins )
    {
    __GSLOGSTRING1( "---[CGSPluginLoader] Sorted list for 0x%X---", iParentUid.iUid );
    HBufC* name = HBufC::New( KGSCaptionSize );
    CleanupStack::PushL( name );

    TPtr ptr = name->Des();
    CGSPluginInterface* plg;

    for( TInt i = 0; i < aPlugins->Count(); i++ )
        {
        plg = (*aPlugins)[i];
        plg->GetCaptionL( ptr );
        __GSLOGSTRING4( "[CGSPluginLoader] Sorted list[%d] Category:%d (0x%X) %S",
                plg->iOrder,
                plg->PluginProviderCategory(),
                plg->Id().iUid,
                &ptr );
        }
    CleanupStack::PopAndDestroy( name );

    }


// ----------------------------------------------------------------------------
// CGSPluginLoader::CompareCategory
//
//
// ----------------------------------------------------------------------------
//
TInt CGSPluginLoader::CompareCategory( const CGSPluginInterface& aFirst,
                                       const CGSPluginInterface& aSecond )
    {
#ifdef RD_GS_COMPARE_BY_CATEGORY
    TInt comparison = KGSComparisonBefore;//KGSComparisonEqual;

    // Compare if a is before b:
    TInt a = aFirst.PluginProviderCategory();
    TInt b = aSecond.PluginProviderCategory();

    // Cannot use less/greater comparison because int values used in here
    // (KGSPluginProviderInternal) must not be revealed to 3rd parties.
    if( a != b )
        {
        switch ( a )
            {
/*            case KGSPluginProviderInternal:
                if( b != KGSPluginProviderInternal )
                    {
                    comparison = KGSComparisonAfter;
                    }
                break;*/
            case CGSPluginInterface::EGSPluginProviderOEM:
                if( b == KGSPluginProviderInternal )
                    {
                    comparison = KGSComparisonAfter;
                    }
                break;
            case CGSPluginInterface::EGSPluginProviderOperator:
                if( b == KGSPluginProviderInternal ||
                    b == CGSPluginInterface::EGSPluginProviderOEM )
                    {
                    comparison = KGSComparisonAfter;
                    }
                break;
            case CGSPluginInterface::EGSPluginProvider3rdParty:
                if( b == KGSPluginProviderInternal ||
                    b == CGSPluginInterface::EGSPluginProviderOEM ||
                    b == CGSPluginInterface::EGSPluginProviderOperator )
                    {
                    comparison = KGSComparisonAfter;
                    }
                break;
            default:
                comparison = KGSComparisonBefore;
                break;
            }
        }
    else
        {
        comparison = KGSComparisonEqual;
        }

    return comparison;

#else //RD_GS_COMPARE_BY_CATEGORY
    #pragma message("Comparing by category DISABLED")
    // Force comparison to equal so category comparison will not matter. If
    // comparison by gategory is needed, simply remove the line below:
    return KGSComparisonEqual;
#endif //RD_GS_COMPARE_BY_CATEGORY
    }


// ----------------------------------------------------------------------------
// CGSPluginLoader::CompareCaption
//
//
// ----------------------------------------------------------------------------
//
TInt CGSPluginLoader::CompareCaption( const CGSPluginInterface& aFirst,
                                      const CGSPluginInterface& aSecond )
    {
    HBufC* firstCaptionBuf =  HBufC::New( KGSCaptionSize );
    HBufC* secondCaptionBuf = HBufC::New( KGSCaptionSize );
    TPtr firstCaption  = firstCaptionBuf->Des();
    TPtr secondCaption = secondCaptionBuf->Des();
    TInt comparison = KGSComparisonEqual;

    TRAPD( err,  aFirst.GetCaptionL( firstCaption ); );
    TRAPD( err2, aSecond.GetCaptionL( secondCaption ); );

    if( err == KErrNone && err2 == KErrNone )
        {
        // CompareC return value must be converted to KGSComparisonXXX value.
        TInt result = secondCaption.CompareC( firstCaption );
        if( result < 0 )
            {
            comparison = KGSComparisonAfter;
            }
        else if( result > 0 )
            {
            comparison = KGSComparisonBefore;
            }
        }
    delete firstCaptionBuf;
    delete secondCaptionBuf;
    return comparison;
    }


// ----------------------------------------------------------------------------
// CGSPluginLoader::CompareIndex
//
//
// ----------------------------------------------------------------------------
//
TInt CGSPluginLoader::CompareIndex( const CGSPluginInterface& aFirst,
                                    const CGSPluginInterface& aSecond )
    {
    TInt comparison = KGSComparisonEqual;

    // The plugin having index is before the one not having one

    if( aFirst.iOrder  == KGSPluginNotIndexed &&
        aSecond.iOrder == KGSPluginNotIndexed )
        {
        // Neither have index -> equal
        comparison = KGSComparisonEqual;
        }
    else if( aFirst.iOrder == KGSPluginNotIndexed )
        {
        // The plugin having index is before the one not having one
        comparison = KGSComparisonAfter;
        }
    else if( aSecond.iOrder == KGSPluginNotIndexed )
        {
        // The plugin having index is before the one not having one
        comparison = KGSComparisonBefore;
        }
    else if( aFirst.iOrder < aSecond.iOrder )
        {
        // Compare actual index values
        comparison = KGSComparisonBefore;
        }
    else if( aFirst.iOrder > aSecond.iOrder )
        {
        // Compare actual index values
        comparison = KGSComparisonAfter;
        }

    return comparison;
    }



EXPORT_C void CGSPluginLoader::RequestPriority( CActive::TPriority aPriority )
    {
    iRequestedPriority = aPriority; 
    }


//  End of File
