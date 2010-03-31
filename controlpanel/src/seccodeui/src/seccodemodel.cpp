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

#include "seccodemodel.h"
#include "seccodemodel_p.h"

SecCodeModel::SecCodeModel(QObject * parent /*= 0*/)
: QObject(parent),d_ptr(new SecCodeModelPrivate())
{
}

SecCodeModel::~SecCodeModel()
{
	delete d_ptr;
}

bool SecCodeModel::simCardRemoved()
{
//    return d_ptr->simCardRemoved();
    return false;
}


int SecCodeModel::pinRemainingAttempts()
{
 //   return d_ptr->pinRemainingAttempts();
    return 3;
}

int SecCodeModel::pin2RemainingAttempts()
{
//    return d_ptr->pin2RemainingAttempts();
    return 3;
}

int SecCodeModel::changePinCode(const QString &oldCode,const QString &newCode,const QString &verifyCode)
{
	Q_UNUSED(oldCode);
	Q_UNUSED(newCode);
	Q_UNUSED(verifyCode);
    return 0;
}

int SecCodeModel::changePin2Code(const QString &oldCode,const QString &newCode,const QString &verifyCode)
{
	Q_UNUSED(oldCode);
	Q_UNUSED(newCode);
	Q_UNUSED(verifyCode);

    return 0;
}
