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

#ifndef SECCODESETTINGS_H
#define SECCODESETTINGS_H

#include <QObject>
#include "seccodeuiglobal.h"

class SecCodeSettingsPrivate;

class SECCODEUI_EXPORT SecCodeSettings : public QObject
{
	Q_OBJECT
public:
	explicit SecCodeSettings(QObject *parent = 0);
	virtual ~SecCodeSettings();
	
	bool pinCodeRequest() const;
	
	void setPinCodeRequest(bool on);
	
	void changePinCode();
	
	void changePin2Code();
	
	bool deviceLock() const;
	
	void setDeviceLock(bool on);
	
	void changeLockCode();
	
	int autoLockPeriod() const;
	
	void setAutoLockPeriod(int period);
	
	bool remoteLock() const;
	
	void setRemoteLock(bool on,const QString &lockCode);
	
private:
	SecCodeSettingsPrivate *d_ptr;
};

#endif //SECCODESETTINGS_H
