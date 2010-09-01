/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Utility class for matching network related enumerations to
*                 descriptor.
*
*/


#ifndef GSNETWORKDEBUGHELPER_H_
#define GSNETWORKDEBUGHELPER_H_

#include "GsNetworkPlugin.hrh"
#include <e32base.h>
#include <e32def.h>
#include <NWHandlingEngine.h>
#include <MPsetNetworkModeObs.h>
#include <MPsetNetworkInfoObs.h>

/**
* Static helper class for transfering network enumerations into descriptors.
* Pretty handy in printing debug traces from network events...
*
* Example (do not print in release mode!):
* #ifdef _DEBUG
*   TBuf<KGSNetworkModeDebugStrLen> myBuf;
*   GSNetworkDebugHelper::XXXToDes( myEnum, aModeCaps );
*   __GSLOGSTRING2( "%S", &aModeCaps, &myEnum );
* #endif //_DEBUG
*/
class GSNetworkDebugHelper
    {
public:

    /**
    * Prints enum name into descriptor.
    *
    * @param aMode  Enumeration whose name should be used.
    * @param buf    Descriptor indicating the enumeration name. Use
    *               KGSNetworkModeDebugStrLen as length.
    */
    static void NwToDes( TGSNetworkModeItems aMode, TDes& buf );

    /**
    * Prints enum name into descriptor.
    * From:
    * enum RMmCustomAPI::TNetworkModeCaps
    *    {
    *    KCapsNetworkModeGsm = 0x01,
    *    KCapsNetworkModeUmts = 0x02,
    *    KCapsNetworkModeDual = 0x04
    *    };
    * AND KGSNetworkModeCapsNotUpdated
    *
    * Can be used for debugging purposes.
    *
    * @param aModeCaps  Enumeration whose name should be used.
    * @param buf    Descriptor indicating the enumeration name. Use
    *               KGSNetworkModeDebugStrLen as length.
    */
    static void NwCapsToDes(
            TUint32 aModeCaps,
            TDes& buf );

    /**
    * Prints enum name into descriptor.
    *
    * Example:
    *   TBuf<KGSNetworkModeDebugStrLen> myBuf;
    *   GSNetworkDebugHelper::NwCapsToDes( myEnum, aModeCaps );
    *   __GSLOGSTRING2( "%S", &aModeCaps, &myEnum );
    *
    * @param aNetworkStatus  Enumeration whose name should be used.
    * @param buf    Descriptor indicating the enumeration name. Use
    *               KGSNetworkModeDebugStrLen as length.
    *
    */
    static void NetworkStatusToDes(
            TNWRegistrationStatus aNetworkStatus,
            TDes& aDes );

    /**
    * Prints enum name into descriptor.
    *
    * @param aNetworkMessage  Enumeration whose name should be used.
    * @param buf    Descriptor indicating the enumeration name. Use
    *               KGSNetworkModeDebugStrLen as length.
    */
    static void NetworkMessageToDes(
            MNWMessageObserver::TNWMessages aNetworkMessage,
            TDes& aDes );

    /**
    * Prints enum name into descriptor.
    *
    * @param aNetworkOperation  Enumeration whose name should be used.
    * @param buf    Descriptor indicating the enumeration name. Use
    *               KGSNetworkModeDebugStrLen as length.
    */
    static void NetworkOperationToDes(
            MNWMessageObserver::TNWOperation aNetworkOperation,
            TDes& aDes );

    /**
    * Prints enum name into descriptor.
    *
    * @param aRequest  Enumeration whose name should be used.
    * @param buf    Descriptor indicating the enumeration name. Use
    *               KGSNetworkModeDebugStrLen as length.
    */
    static void NetworkModeRequestToDes(
            MPsetNetworkModeObserver::TServiceRequest aRequest,
            TDes& aDes );

    /**
    * Prints enum name into descriptor.
    *
    * @param aRequest  Enumeration whose name should be used.
    * @param buf    Descriptor indicating the enumeration name. Use
    *               KGSNetworkModeDebugStrLen as length.
    */
    static void NetworkInfoRequestToDes(
            MPsetNetworkInfoObserver::TServiceRequest aRequest,
            TDes& aDes );

    };

#endif /*GSNETWORKDEBUGHELPER_H_*/


//End of File
