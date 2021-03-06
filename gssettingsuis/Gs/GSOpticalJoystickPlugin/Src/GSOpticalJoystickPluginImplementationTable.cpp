/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  ECOM proxy table for GSOpticalJoystickPlugin
*
*/


// INCLUDES
#include "GSOpticalJoystickPlugin.h"

#include <e32std.h>
#include <implementationproxy.h>


// CONSTANTS
const TImplementationProxy KGSOpticalJoystickPluginImplementationTable[] = 
	{
	IMPLEMENTATION_PROXY_ENTRY( 0x1028331B,	CGSOpticalJoystickPlugin::NewL )
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
	aTableCount = sizeof( KGSOpticalJoystickPluginImplementationTable ) 
        / sizeof( TImplementationProxy );
	return KGSOpticalJoystickPluginImplementationTable;
	}



// End of File
