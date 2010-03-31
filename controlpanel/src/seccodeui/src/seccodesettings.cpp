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

#include "seccodesettings.h"
#include "seccodemodel.h"
#include "seccodeuiutility.h"

class SecCodeSettingsPrivate
{
public:
    SecCodeSettingsPrivate()
    {
        mSecCodeModel = new SecCodeModel();
    }
    
    ~SecCodeSettingsPrivate()
    {
        delete mSecCodeModel;
    }
    
    SecCodeModel *mSecCodeModel;
};

SecCodeSettings::SecCodeSettings(QObject * parent /*= 0*/)
: QObject(parent), d_ptr(new SecCodeSettingsPrivate())
{

}

SecCodeSettings::~SecCodeSettings()
{
	delete d_ptr;
}

bool SecCodeSettings::pinCodeRequest() const
{
//    return d_ptr->mSecCodeModel->pinCodeRequest();
    return true;
}

void SecCodeSettings::setPinCodeRequest(bool on)
{
    Q_UNUSED(on);
}

void SecCodeSettings::changePinCode()
{
    if (d_ptr->mSecCodeModel->simCardRemoved()) {
        SecCodeUiUtility::showErrorMessage(tr("SIM removed."));
        return;
    }
    
    if (!pinCodeRequest()) {
        SecCodeUiUtility::showErrorMessage(tr("Set PIN code request ON."));
        return;
    }
    
    QString title = SecCodeUiUtility::getPinRemainingAttemptsMessage(
            d_ptr->mSecCodeModel->pinRemainingAttempts());
    
    QString oldPassword = SecCodeUiUtility::getPassword(title);
    if (oldPassword.isEmpty()) {
        return;
    }
    
    QString newPassword;
    QString verifyPassword;
    bool firstTry (true);
    
    do {
        if (firstTry) {
            firstTry = false;
        }
        else {
            newPassword.clear();
            verifyPassword.clear();
            SecCodeUiUtility::showErrorMessage(tr("PIN does not match."));
        }
        
        newPassword = SecCodeUiUtility::getPassword(tr("New PIN:"));
        if (newPassword.isEmpty()) {
            return;
        }
        
        verifyPassword = SecCodeUiUtility::getPassword(tr("Verify PIN:"));;
        if (verifyPassword.isEmpty()) {
            return;
        }
    } while (verifyPassword != newPassword);
    
    int err = d_ptr->mSecCodeModel->changePinCode(oldPassword,newPassword,verifyPassword);
    switch (err) {
    case SecCode::ErrNone:
		SecCodeUiUtility::showInformationMessage(tr("Code changed."));
        break;
    default:
        break;
    }
}


void SecCodeSettings::changePin2Code()
{
    if (d_ptr->mSecCodeModel->simCardRemoved()) {
        SecCodeUiUtility::showErrorMessage(tr("SIM removed."));
        return;
    }
    
    if (!pinCodeRequest()) {
        SecCodeUiUtility::showErrorMessage(tr("Set PIN code request ON."));
        return;
    }
    
    QString title = SecCodeUiUtility::getPin2RemainingAttemptsMessage(
            d_ptr->mSecCodeModel->pin2RemainingAttempts());
    
    QString oldPassword = SecCodeUiUtility::getPassword(title);
    if (oldPassword.isEmpty()) {
        return;
    }
    
    QString newPassword;
    QString verifyPassword;
    bool firstTry (true);
    
    do {
        if (firstTry) {
            firstTry = false;
        }
        else {
            newPassword.clear();
            verifyPassword.clear();
            SecCodeUiUtility::showErrorMessage(tr("PIN2 does not match."));
        }
        
        newPassword = SecCodeUiUtility::getPassword(tr("New PIN2:"));
        if (newPassword.isEmpty()) {
            return;
        }
        
        verifyPassword = SecCodeUiUtility::getPassword(tr("Verify PIN2:"));;
        if (verifyPassword.isEmpty()) {
            return;
        }
    } while (verifyPassword != newPassword);
    
    int err = d_ptr->mSecCodeModel->changePin2Code(oldPassword,newPassword,verifyPassword);
    switch (err) {
    case SecCode::ErrNone:
		SecCodeUiUtility::showInformationMessage(tr("Code changed."));
        break;
    default:
        break;
    }
}
