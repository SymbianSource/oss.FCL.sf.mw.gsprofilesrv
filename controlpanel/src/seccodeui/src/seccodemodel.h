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

#ifndef SECCODEMODEL_H
#define SECCODEMODEL_H

#include <QObject>

class SecCodeModelPrivate;
class SecCodeModel : public QObject
{
    Q_OBJECT
public:
	SecCodeModel(QObject * parent = 0);
	virtual ~SecCodeModel();
	
	bool simCardRemoved();
	
	int pinRemainingAttempts();
	
	int changePinCode(const QString &oldCode,const QString &newCode,const QString &verifyCode);
	
	int pin2RemainingAttempts();
	
    int changePin2Code(const QString &oldCode,const QString &newCode,const QString &verifyCode);
	
private:
	SecCodeModelPrivate *d_ptr;
};


#endif //SECCODEMODEL_H

