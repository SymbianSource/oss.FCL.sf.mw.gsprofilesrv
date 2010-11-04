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
* Description: 
*           Base Document class required by Symbian OS architecture.
*
*/


#ifndef GSBASEDOCUMENT_H
#define GSBASEDOCUMENT_H

// INCLUDES
#include <AknDoc.h>
#include <ecom/implementationinformation.h>

// CONSTANTS

// FORWARD DECLARATIONS
class CEikAppUi;
class CGSPluginWrapper;
class CGSPluginAndViewIdCache;
class MGSWatchDog;
class CGSWatchDog;


// CLASS DECLARATION

/**
*  CGSBaseDocument application class.
*  @since Series60_3.1
*/
class CGSBaseDocument : public CAknDocument
{
    public: // Constructor and destructor

        /**
        * Destructor.
        */
        IMPORT_C ~CGSBaseDocument();

    public: // New

        /**
        *
        */
        IMPORT_C CGSPluginWrapper* NewPluginUnloadWrapperLC();

        /**
        *
        */
        IMPORT_C void PreparePluginCacheL( CEikAppUi& aAppUi );

        /**
        *
        */
        IMPORT_C void PrepareForUIDestruction();

        /**
        * @return Pointer to GS application-wide watchdog. Does not transfer
        *         ownership.
        */
        IMPORT_C MGSWatchDog* WatchDog();

        /**
        * Stored array of GS plugin implementation infos. Use this instead of
        * repeatedly using REComSession::ListImplementationsL (performance).
        *
        * @ return reference to plugin inplementation info array.
        *          Does not transfer ownership.
        */
        IMPORT_C RImplInfoPtrArray GetImplInfo();
        
        IMPORT_C void CreateShimmedViewL(const TUid aViewUid);

    public: // API - but not exported
        CGSPluginAndViewIdCache& PluginViewIdCache();
        const CGSPluginAndViewIdCache& PluginViewIdCache() const;

    protected:

        /**
        * C++ constructor
        */
        IMPORT_C CGSBaseDocument( CEikApplication& aApp );

        /**
        * Second phase constructor
        */
        IMPORT_C void ConstructL();

    private: // Cleanup support
        static void CleanupPluginWrapperObject( TAny* aWrapper );
        void DoCleanupPluginWrapper( CGSPluginWrapper* aWrapper );
        static void PrintImplInfoArray( const RImplInfoPtrArray& aImplInfoArray );
        static void PrintImplInfo( const CImplementationInformation& aInfo );
        
    private: // Data

        // Plugin view id cache
        CGSPluginAndViewIdCache* iPluginViewIdCache;

        // Asynchronous loader for the GS plug-ins.
        RPointerArray<CGSPluginWrapper> iUnloadWrapperObjects;

        // Watchdog used GS application-wide. Owned.
        CGSWatchDog* iWatchDog;

        // Array of GS plugin implementation info-objects. Array is created on
        // GS startup for better performance. Use this instead of repeatedly
        // querying from ECOM.
        RImplInfoPtrArray iImplInfoArray;
};

#endif // GSBASEDOCUMENT_H
// End of File
