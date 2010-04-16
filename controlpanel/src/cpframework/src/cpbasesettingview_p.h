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
#ifndef CPBASESETTINGVIEW_P_H
#define CPBASESETTINGVIEW_P_H

class QObject;
class QString;
class CpBaseSettingView;
class HbAction;
class HbView;
class HbDataForm;
class QModelIndex;
class HbDataFormModelItem;
class HbAbstractViewItem;
class CpItemDataHelper;
class CpItemDataHelper;
class QGraphicsWidget;

class CpBaseSettingViewPrivate
{
public:
    CpBaseSettingViewPrivate();
    ~CpBaseSettingViewPrivate();
    /*
     initialize 
     */
    void init(QGraphicsWidget *widget,CpBaseSettingView *baseSettingView);
    void setSettingForm(HbDataForm *settingForm);
    
    //SLOTS
    void _q_softkeyClicked();
private:
    //DATA
    CpBaseSettingView *mBaseSettingView;
    HbAction *mSoftKeyBackAction;
    HbDataForm *mSettingForm;
    friend class CpBaseSettingView;
};

#endif //CPBASESETTINGVIEW_P_H
