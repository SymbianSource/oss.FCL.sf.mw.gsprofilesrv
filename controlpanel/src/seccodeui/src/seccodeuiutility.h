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

#ifndef SECCODEUIUTILITY_H
#define SECCODEUIUTILITY_H

class QString;
class QGraphicsItem;

class SecCodeUiUtility
{
public:
    static QString getPassword(const QString &label,bool *ok = 0,QGraphicsItem *parent = 0);
    static QString getPinRemainingAttemptsMessage(int remainingAttempts);
    static QString getPin2RemainingAttemptsMessage(int remainingAttempts);
    static void showInformationMessage(const QString &message, QGraphicsItem *parent = 0);
    static void showWarningMessage(const QString &message, QGraphicsItem *parent = 0);
    static void showErrorMessage(const QString &message, QGraphicsItem *parent = 0);
};

#endif //SECCODEUIUTILITY_H
