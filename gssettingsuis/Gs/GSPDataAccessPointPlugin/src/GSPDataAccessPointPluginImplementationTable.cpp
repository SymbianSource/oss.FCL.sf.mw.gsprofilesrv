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
* Description:  ECOM proxy table for GSPDataAccessPointPlugin.
*
*/


// User includes
#include "GSPDataAccessPointPlugin.h"

// System includes
#include <e32std.h>
#include <implementationproxy.h>


// Constants
const TImplementationProxy KGSPDataAccessPointSettingsPluginImplementationTable[] =
    {
    IMPLEMENTATION_PROXY_ENTRY( 0x10282DC6,  CGSPDataAccessPointPlugin::NewL )
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
    aTableCount = sizeof( KGSPDataAccessPointSettingsPluginImplementationTable )
        / sizeof( TImplementationProxy );
    return KGSPDataAccessPointSettingsPluginImplementationTable;
    }



// End of File
