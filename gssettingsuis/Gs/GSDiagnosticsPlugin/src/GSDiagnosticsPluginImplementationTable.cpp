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
* Description:  ECOM proxy table for GSDiagnosticsPlugin.
*
*/


// User includes
#include "GSDiagnosticsPlugin.h"

// System includes
#include <e32std.h>
#include <implementationproxy.h>


// Constants
const TImplementationProxy KGSDiagnosticsSettingsPluginImplementationTable[] =
    {
    IMPLEMENTATION_PROXY_ENTRY( 0x102750D4,  CGSDiagnosticsPlugin::NewL )
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
    aTableCount = sizeof( KGSDiagnosticsSettingsPluginImplementationTable )
        / sizeof( TImplementationProxy );
    return KGSDiagnosticsSettingsPluginImplementationTable;
    }


// End of File