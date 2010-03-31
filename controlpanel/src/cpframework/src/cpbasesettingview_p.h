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

class CpBaseSettingViewPrivate
{
public:
    CpBaseSettingViewPrivate();
    ~CpBaseSettingViewPrivate();
    /*
     initialize 
     */
    void init(HbDataForm *settingForm,CpBaseSettingView *baseSettingView);
    void setSettingForm(HbDataForm *settingForm);
    
    /*
     softkey 
     */
    void setSoftkey();
    void clearSoftkey();

    //SLOTS
    void _q_softkeyClicked();
    void _q_viewChanged(HbView *view);
private:
    //DATA
    CpBaseSettingView *mBaseSettingView;
    HbAction *mSoftKeyBackAction;
    HbDataForm *mSettingForm;
    bool mIsActiveView;
    friend class CpBaseSettingView;
};

#endif //CPBASESETTINGVIEW_P_H
