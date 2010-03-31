/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0""
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  
*
*/

#include "seccodemodel_p.h"
#include "seccodeuiglobal.h"
#include <e32err.h>
#include <exterror.h>

static int symbianErrtoQtErr(int err)
{
    switch (err) {
    case KErrNone:
        return SecCode::ErrNone;
    case KErrGsm0707IncorrectPassword:
        return SecCode::ErrGsm0707IncorrectPassword;
    case KErrAccessDenied:
        return SecCode::ErrAccessDenied;
    case KErrGsmSSPasswordAttemptsViolation:
        return SecCode::ErrGsmSSPasswordAttemptsViolation;
    case KErrLocked:
        return SecCode::ErrLocked;
    case KErrGsm0707OperationNotAllowed:
        return SecCode::ErrGsm0707OperationNotAllowed;
    case KErrGsm0707SIMPuk2Required:
        return SecCode::ErrGsm0707SIMPuk2Required;
    case KErrAbort:
        return SecCode::ErrAbort;
    default:
        break;
    }
    
    return err;
}

