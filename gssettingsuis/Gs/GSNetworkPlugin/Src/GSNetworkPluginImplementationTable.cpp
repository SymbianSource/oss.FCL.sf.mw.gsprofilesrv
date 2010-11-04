/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
*        ECOM proxy table for this plugin
*
*/


// System includes
#include <e32std.h>
#include <implementationproxy.h>

// User includes
#include "GSNetworkPlugin.h"

#ifdef FF_POWER_SAVE
    #include "PSMNetworkPlugin.h"
#endif // FF_POWER_SAVE

// Constants
const TImplementationProxy KGSNetworkPluginImplementationTable[] =
	{
	IMPLEMENTATION_PROXY_ENTRY( 0x102824A8,	CGSNetworkPlugin::NewL )
#ifdef FF_POWER_SAVE
    ,
    IMPLEMENTATION_PROXY_ENTRY( 0x2000B594, CPSMNetworkPlugin::NewL )
#endif // FF_POWER_SAVE 
	};


// ---------------------------------------------------------------------------
// ImplementationGroupProxy
//
// Gate/factory function
// ---------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(
                                                  TInt& aTableCount )
	{
	aTableCount = sizeof( KGSNetworkPluginImplementationTable )
        / sizeof( TImplementationProxy );
	return KGSNetworkPluginImplementationTable;
	}


// End of File
