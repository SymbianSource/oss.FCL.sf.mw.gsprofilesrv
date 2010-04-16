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
* Description:  Implementation of the ProEngFactory2.
*
*/



// INCLUDE FILES
#include    <ProEngFactory2.h>
#include    "CProEngNotifyHandlerImpl.h"

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// ProEngFactory::NewNotifyHandlerL
// -----------------------------------------------------------------------------
//
EXPORT_C MProEngNotifyHandler2* ProEngFactory2::NewNotifyHandler2L()
    {
    return CProEngNotifyHandlerImpl::NewL();
    }

//  End of File

