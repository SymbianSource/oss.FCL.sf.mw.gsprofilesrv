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
#ifndef CPPINCODEPLUGINVIEW_H
#define CPPINCODEPLUGINVIEW_H

#include <cpbasesettingview.h>

class SecCodeSettings;
class CpPinCodePluginView : public CpBaseSettingView
{
    Q_OBJECT
public:
    explicit CpPinCodePluginView(QGraphicsItem *parent = 0);
    virtual ~CpPinCodePluginView();
private slots:
	void onPinCodeClicked();
	void onPin2CodeClicked();
private:
	SecCodeSettings *mSecCodeSettings;
};

#endif  //CPPINCODEPLUGINVIEW_H
