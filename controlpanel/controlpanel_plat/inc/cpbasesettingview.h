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
#ifndef CPBASESETTINGVIEW_H
#define CPBASESETTINGVIEW_H

#include "cpglobal.h"
#include <hbview.h>

class QModelIndex;
class HbDataForm;
class CpBaseSettingViewPrivate;
class CpItemDataHelper;

class CP_EXPORT CpBaseSettingView : public HbView
{
    Q_OBJECT
public:
    explicit CpBaseSettingView(HbDataForm *settingForm = 0,QGraphicsItem *parent = 0);
    virtual ~CpBaseSettingView();
    void setSettingForm(HbDataForm *settingForm);
    HbDataForm *settingForm() const;
signals:
    void aboutToClose();
private:
    void setWidget(QGraphicsWidget *widget);
private:
    Q_DISABLE_COPY(CpBaseSettingView)
    Q_DECLARE_PRIVATE_D(d_ptr, CpBaseSettingView)
    Q_PRIVATE_SLOT(d_func(), void _q_softkeyClicked())
    Q_PRIVATE_SLOT(d_func(), void _q_viewChanged(HbView *view))
private:
    CpBaseSettingViewPrivate *d_ptr;
};

#endif /* CPBASESETTINGVIEW_H */
