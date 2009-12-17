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
* Description:  ECOM proxy table for GSPDataPlugin.
*
*/


// User includes
#include "GSPDataPlugin.h"

// System includes
#include <e32std.h>
#include <implementationproxy.h>



// Constants
const TImplementationProxy KGSPDataSettingsPluginImplementationTable[] = 
    {
    IMPLEMENTATION_PROXY_ENTRY( 0x1020724E, CGSPDataPlugin::NewL )
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
    aTableCount = sizeof( KGSPDataSettingsPluginImplementationTable ) 
        / sizeof( TImplementationProxy );
    return KGSPDataSettingsPluginImplementationTable;
    }


// End of File
