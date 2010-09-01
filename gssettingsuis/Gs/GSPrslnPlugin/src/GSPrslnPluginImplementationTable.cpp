/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    ECOM prxy table for plugin.
*
*/


// System includes
#include <e32std.h>
#include <implementationproxy.h>

// User includes
#include "GSPrslnPlugin.h"

// Constants
const TImplementationProxy KGSPrslnPluginImplementationTable[] = 
	{
	IMPLEMENTATION_PROXY_ENTRY( 0x10207252,	CGSPrslnPlugin::NewL )
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
	aTableCount = sizeof( KGSPrslnPluginImplementationTable ) 
        / sizeof( TImplementationProxy );
	return KGSPrslnPluginImplementationTable;
	}


// End of File
