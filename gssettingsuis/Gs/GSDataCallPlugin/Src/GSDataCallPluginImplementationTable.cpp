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
* Description:  ECOM proxy table for GSDataCallPlugin
*
*/


// INCLUDES
#include "GSDataCallPlugin.h"

#include <e32std.h>
#include <implementationproxy.h>


// CONSTANTS
const TImplementationProxy KGSDataCallPluginImplementationTable[] = 
	{
	IMPLEMENTATION_PROXY_ENTRY( 0x10207436,	CGSDataCallPlugin::NewL )
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
	aTableCount = sizeof( KGSDataCallPluginImplementationTable ) 
        / sizeof( TImplementationProxy );
	return KGSDataCallPluginImplementationTable;
	}



// End of File
