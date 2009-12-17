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
* Description: 
*
*/


// INCLUDE FILES
#include    <bldvariant.hrh>
#include    <featmgr.h>
#include    <centralrepository.h>

// USER INCLUDES
#include    "GSWatchDog.h"
#include    "GsLogger.h"
#include    "settingsinternalcrkeys.h"
#include    "SettingsPrivateCRKeys.h"

#include    <eikenv.h>
#include    <s32mem.h>
#include    <centralrepository.h>

using namespace NCentralRepositoryConstants;

// CONSTANTS


// ========================= MEMBER FUNCTIONS ================================

// ----------------------------------------------------------------------------
// CGSWatchDog::NewL
//
// EPOC two-phased constructor
// ----------------------------------------------------------------------------
//
CGSWatchDog* CGSWatchDog::NewL()
    {
    CGSWatchDog* self = new( ELeave ) CGSWatchDog;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ----------------------------------------------------------------------------
// CGSWatchDog::ConstructL
//
// EPOC default constructor can leave.
// ----------------------------------------------------------------------------
//
void CGSWatchDog::ConstructL()
    {
    __GSLOGSTRING( "[CGSWatchDog] ConstructL()" );
    iStore = CRepository::NewL( KCRUidMiscSettings );

    // Read last stored state for watchdog...
    ReadStateL();

    // If GS crashed last time, activate watchdog, reset run counter. Watchdog
    // will be active as long as exit was not clean (panicked).
    if( !WasCleanExitL() &&
        iWatchdogActivationLimit != KGSWatchdogActivationDisabled  )
        {
        iCrashCounter++;
        iIsActive = ETrue;

#ifdef _GS_WATCHDOG_VERBOSE_TRACES
    #pragma message("-CGSWachDog verbose traces activated-")
    User::InfoPrint( _L("Watchdog activated!") );
#endif //_GS_WATCHDOG_VERBOSE_TRACES

        if( iQuarantine.Count() > 0 )
            {
            for( TInt i = 0; i < iQuarantine.Count(); i++ )
                {
                // If plugin is not already in blacklist, move it there from
                // quarantinelist.
                if( !IsInBlackList( iQuarantine[i] ) )
                    {
                    TGSPluginQuarantine quarantinedPlugin;
                    quarantinedPlugin.iRunsAfterCrash = 0;
                    quarantinedPlugin.iUid = iQuarantine[i];
                    iBlackList.Append( quarantinedPlugin );
                    iQuarantine.Remove( i );
                    }
                }
            }
        }
     else
        {
        iIsActive = EFalse;
        iCrashCounter = 0;

#ifdef _GS_WATCHDOG_VERBOSE_TRACES
    User::InfoPrint( _L("Watchdog disabled.") );
#endif //_GS_WATCHDOG_VERBOSE_TRACES

        // Clean run -> empty quarantine.
        iQuarantine.Reset();
        }

    // Continue normal progress, set application to run state...
    iGSAppRunning = ETrue;

    // Store watchdog state at this point as panic will not call normal
    // destructors and data would be otherwise lost in case of panic.
    StoreStateL();
    }


// ---------------------------------------------------------------------------
// CGSWatchDog::CGSWatchDog
//
// C++ constructor
// ---------------------------------------------------------------------------
CGSWatchDog::CGSWatchDog()
    {
    }



// ---------------------------------------------------------------------------
// CGSWatchDog::~CGSWatchDog
//
// Destructor
// ---------------------------------------------------------------------------
CGSWatchDog::~CGSWatchDog()
    {
    // Update all blacklisted plugins in case no crash FIRST, then store...
    UpdateBlackListedPluginRunCounters();
    TRAP_IGNORE( StoreStateL(); );
    iBlackList.Close();
    iQuarantine.Close();
    delete iStore;
    }


// ---------------------------------------------------------------------------
// CGSWatchDog::Quarantine
//
//
// ---------------------------------------------------------------------------
void CGSWatchDog::QuarantineL( TUid aPluginUid )
    {
    if( iIsActive )
        {
        iQuarantine.Append( aPluginUid );
        StoreQuarantineL(); // Store to persistent storage

#ifdef _GS_WATCHDOG_VERBOSE_TRACES
    __GSLOGSTRING1( "[CGSWatchDog] Quarantined:0x%X", aPluginUid.iUid );
#endif //_GS_WATCHDOG_VERBOSE_TRACES

        }
    }


// ---------------------------------------------------------------------------
// CGSWatchDog::RemoveFromQuarantine
//
//
// ---------------------------------------------------------------------------
void CGSWatchDog::RemoveFromQuarantineL( TUid aPluginUid )
    {
    if( iIsActive )
        {
        for( TInt i = 0; i < iQuarantine.Count(); i++ )
            {
            if( iQuarantine[i] == aPluginUid )
                {
                iQuarantine.Remove( i );

#ifdef _GS_WATCHDOG_VERBOSE_TRACES
    __GSLOGSTRING1( "[CGSWatchDog] Removed from quarantine:0x%X", aPluginUid.iUid );
#endif //_GS_WATCHDOG_VERBOSE_TRACES

                StoreQuarantineL();
                break;
                }
            }
        }
    }

// ---------------------------------------------------------------------------
// CGSWatchDog::WasCleanExit
//
//
// ---------------------------------------------------------------------------
TBool CGSWatchDog::WasCleanExitL()
    {
    return !iGSAppRunning;
    }

// ---------------------------------------------------------------------------
// CGSWatchDog::ReportCleanExit
//
//
// ---------------------------------------------------------------------------
void CGSWatchDog::ReportCleanExitL()
    {
    iGSAppRunning = EFalse;
    iStore->Set( KWatchDogGSRunning, iGSAppRunning );
    }


// ---------------------------------------------------------------------------
// CGSWatchDog::IsInBlackList
//
//
// ---------------------------------------------------------------------------
TBool CGSWatchDog::IsInBlackList( TUid aPluginUid )
    {
    TBool found = EFalse;
    if( iIsActive )
        {
        for( TInt i = 0; i < iBlackList.Count(); i++ )
            {
            if( iBlackList[i].iUid == aPluginUid )
                {
                found = ETrue;
                break;
                }
            }
        }
    
    return found;
    }


// ---------------------------------------------------------------------------
// CGSWatchDog::StoreStateL
//
//
// ---------------------------------------------------------------------------
void CGSWatchDog::StoreStateL()
    {
    //iStore->Set( KWatchDogIsActive,         iIsActive );
    iStore->Set( KWatchDogGSRunning,        iGSAppRunning );
    iStore->Set( KWatchDogCrashCounter,     iCrashCounter );
    iStore->Set( KWatchdogActivationLimit,  iWatchdogActivationLimit );
    iStore->Set( KWatchdogMaxPluginBlackListedRuns,   iMaxPluginBlackListedRuns );

    StoreQuarantineL();
    StoreBlackListL();

#ifdef _GS_WATCHDOG_VERBOSE_TRACES
    // The following is just for debug trace:
    __GSLOGSTRING( "[CGSWatchDog] Storing state:" );
    __GSLOGSTRING1( "   iIsActive: %d",     iIsActive );
    __GSLOGSTRING1( "   iGSAppRunning: %d", iGSAppRunning );
    __GSLOGSTRING1( "   iCrashCounter: %d",   iCrashCounter );
    __GSLOGSTRING1( "   iWatchdogActivationLimit: %d", iWatchdogActivationLimit );
    __GSLOGSTRING1( "   iMaxPluginBlackListedRuns: %d", iMaxPluginBlackListedRuns );

    __GSLOGSTRING( "[CGSWatchDog] Stored plugins from quarantine:" );
    for( TInt i = 0; i < iQuarantine.Count(); i++ )
        {
            __GSLOGSTRING1( "    0x%X", iQuarantine[i].iUid );
        }
    __GSLOGSTRING( "-" );

    __GSLOGSTRING( "[CGSWatchDog] Stored plugins from blacklist:" );
    for( TInt i = 0; i < iBlackList.Count(); i++ )
        {
            __GSLOGSTRING2( "    0x%X %d runs", iBlackList[i].iUid.iUid, iBlackList[i].iRunsAfterCrash );
        }
    __GSLOGSTRING( "-" );
#endif //_GS_WATCHDOG_VERBOSE_TRACES
    }


// ---------------------------------------------------------------------------
// CGSWatchDog::ReadStateL
//
//
// ---------------------------------------------------------------------------
void CGSWatchDog::ReadStateL()
    {
    /*TInt isActive = EFalse; // Needed for mapping to TBool value
    TInt err = iStore->Get( KWatchDogIsActive, isActive );
    if( isActive == 1 )
        {
        iIsActive = ETrue;
        }
    else
        {
        iIsActive = EFalse;
        }*/

    TInt appRunning; // Needed for mapping to TBool value
    TInt err = iStore->Get( KWatchDogGSRunning, appRunning );

    if( err )
        {
        // CenRep values are uninitialized: first runtime of watchdog
        // -> init values
        InitStateL();
        }

    if( appRunning )
        {
        iGSAppRunning = ETrue;
        }
    else
        {
        iGSAppRunning = EFalse;
        }

    iStore->Get( KWatchDogCrashCounter, iCrashCounter );
    iStore->Get( KWatchdogActivationLimit, iWatchdogActivationLimit );
    iStore->Get( KWatchdogMaxPluginBlackListedRuns, iMaxPluginBlackListedRuns );

    ReadQuarantineL();
    ReadBlackListL();

#ifdef _GS_WATCHDOG_VERBOSE_TRACES
    // The following is just for debug trace:
    __GSLOGSTRING( "[CGSWatchDog] Read state:" );
    __GSLOGSTRING1( "   iIsActive: %d",     iIsActive );
    __GSLOGSTRING1( "   iGSAppRunning: %d", iGSAppRunning );
    __GSLOGSTRING1( "   iCrashCounter: %d", iCrashCounter );
    __GSLOGSTRING1( "   iWatchdogActivationLimit: %d", iWatchdogActivationLimit );
    __GSLOGSTRING1( "   iMaxPluginBlackListedRuns: %d", iMaxPluginBlackListedRuns );

    __GSLOGSTRING( "[CGSWatchDog] Read plugins from quarantine:" );
    for( TInt i = 0; i < iQuarantine.Count(); i++ )
        {
            __GSLOGSTRING1( "    0x%X", iQuarantine[i].iUid );
        }
    __GSLOGSTRING( "-" );

    __GSLOGSTRING( "[CGSWatchDog] Read plugins from blacklist:" );
    for( TInt i = 0; i < iBlackList.Count(); i++ )
        {
            __GSLOGSTRING2( "    0x%X %d runs", iBlackList[i].iUid.iUid, iBlackList[i].iRunsAfterCrash );
        }
    __GSLOGSTRING( "-" );
#endif //_GS_WATCHDOG_VERBOSE_TRACES
    }


// ---------------------------------------------------------------------------
// CGSWatchDog::InitStateL
//
//
// ---------------------------------------------------------------------------
void CGSWatchDog::InitStateL()
    {
    __GSLOGSTRING( "[CGSWatchDog] initializing state..." );
    iIsActive = EFalse;
    iGSAppRunning = EFalse;
    iCrashCounter = 0;

    // Check CenRep, if values do not exist, use default values:
    TInt err;
    err = iStore->Get( KWatchDogCrashCounter, iWatchdogActivationLimit );
    if( err )
        {
        iWatchdogActivationLimit = KGSDefaultWatchdogActivationLimit;
        }

    err = iStore->Get( KWatchdogMaxPluginBlackListedRuns, iMaxPluginBlackListedRuns);
    if( err )
        {
        iMaxPluginBlackListedRuns = KGSDefaultMaxPluginBlackListedRuns;
        }

    StoreStateL();
    }


// ---------------------------------------------------------------------------
// CGSWatchDog::ReadQuarantineL
//
//
// ---------------------------------------------------------------------------
void CGSWatchDog::ReadQuarantineL()
    {
    HBufC8* buf = HBufC8::NewLC( KMaxBinaryLength );
    TPtr8 serializedArr = buf->Des();
    iStore->Get( KWatchDogQuarantine, serializedArr );
    RDesReadStream stream;
    stream.Open( serializedArr );
    CleanupClosePushL( stream );

    // In case CenRep value is uninitialized (less than 16 bits), don't try
    //reading it
    TInt count = 0;
    if( buf->Size() > 0 )
        {
        count = stream.ReadUint16L();
        }
    for( TInt i = 0; i < count; i++  )
        {
        TUid uid;
        uid.iUid = stream.ReadInt32L();
        iQuarantine.Append( uid );
        }
    //stream.Close();
    CleanupStack::PopAndDestroy( &stream );
    CleanupStack::PopAndDestroy( buf );
    }


// ---------------------------------------------------------------------------
// CGSWatchDog::StoreQuarantineL
//
//
// ---------------------------------------------------------------------------
void CGSWatchDog::StoreQuarantineL()
    {
    // Hmmm. KMaxBinaryLength = 1024*2 unicode characters (16bit)
    // = 1024*2*16 bits = 32768.
    // 32768 - 16(count) bits reserved for 32 bit TUids = 1023.5 TUids
    // -> We can store 1023 UIds in quarantine. Should be enought as we have
    // about ~50 plugins at the moment and each plugin ought to be listed in
    // quarantine only once at a specific point of time.
    HBufC8* buf = HBufC8::NewLC( KMaxBinaryLength );
    TPtr8 serializedArr = buf->Des();
    RDesWriteStream stream;
    stream.Open( serializedArr );
    CleanupClosePushL( stream );

    stream.WriteUint16L( iQuarantine.Count() );

    for( TInt i = 0; i < iQuarantine.Count(); i++ )
        {
        stream.WriteInt32L( iQuarantine[i].iUid );
        }
    // This also flushes the stream:
    CleanupStack::PopAndDestroy( &stream );

    iStore->Set( KWatchDogQuarantine, serializedArr );

    CleanupStack::PopAndDestroy( buf );
    }


// ---------------------------------------------------------------------------
// CGSWatchDog::ReadBlackListL
//
//
// ---------------------------------------------------------------------------
void CGSWatchDog::ReadBlackListL()
    {
    HBufC8* buf = HBufC8::NewLC( KMaxBinaryLength );
    TPtr8 serializedArr = buf->Des();
    iStore->Get( KWatchDogBlackList, serializedArr );
    RDesReadStream stream;
    stream.Open( serializedArr );
    CleanupClosePushL( stream );

    // In case CenRep value is uninitialized (less than 16 bits), don't try
    //reading it
    TInt count = 0;
    if( buf->Size() > 0 )
        {
        count = stream.ReadUint16L();
        }
    for( TInt i = 0; i < count; i++  )
        {
        TGSPluginQuarantine plugin;
        plugin.InternalizeL( stream );
        iBlackList.Append( plugin );
        }

    //stream.Close();
    CleanupStack::PopAndDestroy( &stream );
    CleanupStack::PopAndDestroy( buf );
    }


// ---------------------------------------------------------------------------
// CGSWatchDog::StoreBlackListL
//
//
// ---------------------------------------------------------------------------
void CGSWatchDog::StoreBlackListL()
    {
    HBufC8* buf = HBufC8::NewLC( KMaxBinaryLength );
    TPtr8 serializedArr = buf->Des();
    RDesWriteStream stream;
    stream.Open( serializedArr );
    CleanupClosePushL( stream );

    stream.WriteUint16L( iBlackList.Count() );

    for( TInt i = 0; i < iBlackList.Count(); i++ )
        {
        iBlackList[i].ExternalizeL( stream );

#ifdef _GS_WATCHDOG_VERBOSE_TRACES
    __GSLOGSTRING1( "[CGSWatchDog] Written to blacklist 0x%X",
                    iBlackList[i].iUid.iUid );
#endif //_GS_WATCHDOG_VERBOSE_TRACES

        }
    // This also flushes the stream:
    CleanupStack::PopAndDestroy( &stream );

    iStore->Set( KWatchDogBlackList, serializedArr );

    CleanupStack::PopAndDestroy( buf );
    }


// ---------------------------------------------------------------------------
// CGSWatchDog::UpdateBlackListedPluginRunCounters
//
//
// ---------------------------------------------------------------------------
void CGSWatchDog::UpdateBlackListedPluginRunCounters()
    {
    TInt count = iBlackList.Count();
    for( TInt i = 0; i < count; i++ )
        {
        iBlackList[i].iRunsAfterCrash++;
        if( iBlackList[i].iRunsAfterCrash > iMaxPluginBlackListedRuns )
            {
//__GSLOGSTRING2( "[CGSWatchDog] 0x%X removed from blacklist (%d runs)", iBlackList[i].iUid.iUid, iBlackList[i].iRunsAfterCrash );
            iBlackList.Remove( i );
            i--;    // Item in index i has been removed
            count--;// One item has been removed from the array
            }
        }
    }


// ---------------------------------------------------------------------------
// TGSPluginQuarantine::InternalizeL
//
//
// ---------------------------------------------------------------------------
void TGSPluginQuarantine::InternalizeL( RReadStream& aStream )
    {
    iUid.iUid = aStream.ReadInt32L();
    iRunsAfterCrash = aStream.ReadInt16L();
    }


// ---------------------------------------------------------------------------
// TGSPluginQuarantine::ExternalizeL
//
//
// ---------------------------------------------------------------------------
void TGSPluginQuarantine::ExternalizeL( RWriteStream& aStream ) const
    {
    aStream.WriteInt32L( iUid.iUid );
    aStream.WriteInt16L( iRunsAfterCrash );
    }


//End of File
