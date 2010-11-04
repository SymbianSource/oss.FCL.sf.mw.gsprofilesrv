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


#ifndef GSWATCHDOG_H
#define GSWATCHDOG_H

// System includes
#include <e32base.h>
#include <mgswatchdog.h>

// CONSTANTS


/**
* Maximum number of GS runs during which plugin is held in blacklist. After
* this plugin is allowed normal execution unless it crashes again.
*
* Used only in case no CenRep value is defined.
*/
const TInt KGSDefaultMaxPluginBlackListedRuns = 5;


/**
* Defines how many crashes are observed before watchdog is activated. The
* bigger the value, more tolerant behaviour towards crashing plugins.
*
* -1 = Watchdog is completely disabled regardless any crash
*      (KGSWatchdogActivationDisabled).
*  1 = In practise, the fastest reaction, meaning watchdog is enabled the next
*      run GS starts after a crash.
*  2 = GS can have 1 crash without watchdog being enabled = 2nd GS crash in a
*      row will enable watchdog.
*  n = GS can have n-1 crashes before watchdog being activated etc.
*
* Used only in case no CenRep value is defined.
*/
const TInt KGSDefaultWatchdogActivationLimit = 1;

/**
* Watchdog is completely disabled.
*/
const TInt KGSWatchdogActivationDisabled = -1;


// FORWARD DECLARATIONS
class CRepository;
class RReadStream;
class RWriteStream;

// CLASS DECLARATION



/**
* Container class for quarantined plugin data.
*/
class TGSPluginQuarantine
    {
    public:
        /**
        * Read data from aStream.
        * @param aStream read stream.
        */
        void InternalizeL( RReadStream& aStream );

        /**
        * Write data to aStream.
        * @param aStream write stream.
        */
        void ExternalizeL( RWriteStream& aStream ) const;

    public:

        /**
        * Uid of the plugin.
        */
        TUid iUid;

        /**
        * Counter which is incremented each time GS is started and the plugin
        * is in blacklist. This is used for removing plugin from blacklist
        * after certain number of GS runs (iMaxPluginBlackListedRuns). This
        * means plugins are not in blacklist for ever but it is possible to try
        * running the plugin after a number of GS runs. This is needed in case
        * of ramdom crash while some essential plugin is in quarantine.
        */
        TInt iRunsAfterCrash;
    };

/**
*  CGSWatchDog is responsible for quarantining plugins and updating blacklist
*  for crashing GS plugins. This is the class responsible for keeping GS
*  application functional even if some plugin to panics all the time.
*
*  @since Series60_3.2
*/
NONSHARABLE_CLASS( CGSWatchDog ) : public CBase,
                                   public MGSWatchDog
    {
    public: // Constructor and destructor

        /**
        * Symbian OS two-phased constructor
        * @return CGSWatchDog.
        */
        static CGSWatchDog* NewL();

        /**
        * Destructor.
        */
        ~CGSWatchDog();

    public: // From MGSWatchDog

        /**
        * Adds plugin to quarantine. In case GS crashes, quarantined plugin
        * will be blacklisted so it is not loaded in next GS run. Same plugin
        * can be quarantined multiple times or even have several uid instances
        * in quarantine if needed as long as all these are removed
        * correspondingly.
        *
        * NOTE: Plugin must ALWAYS be removed from quarantine after plugin's
        * function execution.
        * 1. QuarantineL().
        * 2. Call quarantined plugin function.
        * 3. RemoveFromQuarantineL().
        *
        * @param aPluginUid uid of the quarantined plugin.
        */
        void QuarantineL( TUid aPluginUid );

        /**
        * Removes plugin from quarantine. In case plugin is not removed from
        * quarantine before GS exits, the plugin will be blacklisted during
        * next GS startup.
        *
        * @param aPluginUid uid of the plugin to be removed from quarantine.
        */
        void RemoveFromQuarantineL( TUid aPluginUid );

        /**
        * @return ETrue if GS exited cleanly last time.
        *         EFalse if GS crashed during last execution.
        */
        TBool WasCleanExitL();

        /**
        * This must be called in all the cases GS exits normally. If GS panics,
        * this will not be called resulting in banning of all quarantined
        * plugins.Increments iRunsAfterCrash counter for blacklisted plugins.
        * Sets iGSAppRunning to EFalse.
        */
        void ReportCleanExitL();

        /**
        * Checks if plugin is in blacklist. This can be used to determine if
        * plugin should be loaded.
        *
        * @return ETrue in case plugin is in blacklist and should not be loaded.
        *         EFalse in case plugin should not be loaded
        */
        TBool IsInBlackList( TUid aPluginUid );

    protected: // New

        /**
        * C++ constructor
        */
        CGSWatchDog();

        /**
        * Second phase constructor
        */
        void ConstructL();

        /**
        * Reads previous state from persistent data storage.
        */
        void ReadStateL();

        /**
        * Stores iGSAppRunning, iQuarantinedPluginUid and iBlackList data to
        * persistent storage.
        */
        void StoreStateL();

        /**
        * Sets state values to default values and stores these.
        */
        void InitStateL();

        /**
        * Reads quarantine data from persistent storage.
        */
        void ReadQuarantineL();

        /**
        * Stores quarantine data to persistent storage.
        */
        void StoreQuarantineL();

        /**
        * Reads blacklist data from persistent storage.
        */
        void ReadBlackListL();

        /**
        * Stores blacklist data to persistent storage.
        */
        void StoreBlackListL();

        /**
        * Updates run counter of each blacklisted plugin. Removes plugin from
        * blacklist if it has been there long enough
        * (KGSDefaultMaxPluginBlackListedRuns).
        */
        void UpdateBlackListedPluginRunCounters();

    private: // Data

        /**
        * ETrue if WatchDog is active (GS has crashed in previous run).
        */
        TBool iIsActive;

        /**
        * Counter for GS crashes in a row. This is used to customize watchdog
        * behaviour. Defines how many crashes are observed before watchdog is
        * activated. The bigger the value, more tolerant behaviour towards crashing
        * plugins. See iWatchdogActivationLimit.
        *
        * -1 = Watchdog is completely disabled regardless any crash.
        *  1 = In paractise the fastest reaction, meaning watchdog is enabled the next
        *      run GS starts after a crash.
        *  2 = GS can have 1 crash without watchdog being enabled = 2nd GS crash in a
        *      row will enable watchdog.
        *  n = GS can have n-1 crashes before watchdog being activated etc.
        */
        TInt iCrashCounter;

        /**
        * Emulates stored data. Set on when GS running, off when GS exits
        * cleanly. Check this when starting GS to see if last exit was clean.
        */
        TBool iGSAppRunning;

        /**
        * Uids for plugins which should be banned. They caused GS crash in
        * previous runs.
        */
        RArray<TGSPluginQuarantine> iBlackList;

        /**
        * Run time quarantine for plugins.
        */
        RArray<TUid> iQuarantine;

        /**
        * Persistent storage.
        */
        CRepository* iStore;

        /**
        * Defines how long a plugin is held in blacklist.
        * KGSDefaultMaxPluginBlackListedRuns is used if no CenRep value is defined.
        */
        TInt iMaxPluginBlackListedRuns;

        /**
        * Defines how many crashes is needed to activate watchdog.
        * KGSDefaultWatchdogActivationLimit is used in case no CenRep value is
        * defined.
        */
        TInt iWatchdogActivationLimit;
    };

#endif // GSWATCHDOG_H
// End of File
