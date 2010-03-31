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
#include "seccodeuiutility.h"
#include <QString>
#include <hbinputdialog.h>
#include <hbmessagebox.h>

QString SecCodeUiUtility::getPassword(const QString &label,bool *ok /*= 0*/,QGraphicsItem *parent /*= 0*/)
{
	HbInputDialog *dlg = new HbInputDialog(parent);
	dlg->setEchoMode(HbLineEdit::Password);
    QString result;
    dlg->setPromptText(label);
	dlg->setInputMode(HbInputDialog::TextInput);	
    HbAction* action = dlg->exec();
    if(action == dlg->secondaryAction()) { //Cancel was pressed
        if(ok) {
			*ok = false;
        }
    } else { //OK was pressed
        if(ok) {
			*ok = true;
        }
        result = dlg->textValue().toString();
    }
    delete dlg;
    return result;
}

void SecCodeUiUtility::showInformationMessage(const QString &message,QGraphicsItem *parent /*= 0*/ )
{
	HbMessageBox::launchInformationMessageBox(message,0,0,parent);
}

void SecCodeUiUtility::showWarningMessage(const QString &message,QGraphicsItem *parent /*= 0*/)
{
	HbMessageBox::launchWarningMessageBox(message,0,0,parent);
}

void SecCodeUiUtility::showErrorMessage(const QString &message,QGraphicsItem *parent /*= 0*/)
{
	HbMessageBox::launchInformationMessageBox(message,0,0,parent);
}

QString SecCodeUiUtility::getPinRemainingAttemptsMessage(int remainingAttempts)
{
	Q_UNUSED(remainingAttempts);

	return QString("PIN code:");
}

QString SecCodeUiUtility::getPin2RemainingAttemptsMessage(int remainingAttempts)
{
	Q_UNUSED(remainingAttempts);

	return QString("PIN2 code:");
}
