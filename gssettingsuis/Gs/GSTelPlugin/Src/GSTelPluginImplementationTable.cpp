/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  ECOM proxy table for GSTelPlugin.
*
*/


// System includes
#include <e32std.h>
#include <implementationproxy.h>

// User includes
#include "GSTelPlugin.h"

// Constants
const TImplementationProxy KGSTelPluginImplementationTable[] =
	{
	IMPLEMENTATION_PROXY_ENTRY( 0x1020723D,	CGSTelPlugin::NewL )
	};


// ---------------------------------------------------------------------------
// ImplementationGroupProxy
// Gate/factory function
//
// ---------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(
    TInt& aTableCount )
	{
	aTableCount = sizeof( KGSTelPluginImplementationTable )
        / sizeof( TImplementationProxy );
	return KGSTelPluginImplementationTable;
	}


// End of File
