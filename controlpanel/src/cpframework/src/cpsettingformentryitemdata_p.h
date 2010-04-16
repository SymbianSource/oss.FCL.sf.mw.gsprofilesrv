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

#ifndef CPSETTINGFORMENTRYITEMDATA_P_H
#define CPSETTINGFORMENTRYITEMDATA_P_H

#include <QString>
#include <QPointer>
#include <hbicon.h>
#include <hbview.h>

class CpSettingFormEntryItemData;
class CpItemDataHelper;
class HbDataForm;
class HbDataFormModelItem;
class QModelIndex;

class CpSettingFormEntryItemDataPrivate
{
public:
    CpSettingFormEntryItemDataPrivate();
    explicit CpSettingFormEntryItemDataPrivate(CpItemDataHelper *itemDataHelper);
    explicit CpSettingFormEntryItemDataPrivate(HbDataForm *dataForm);
    
    void init(CpSettingFormEntryItemData *parent);
    
    QString text() const;
    void setText(const QString &text);
    
    QString description() const;
    void setDescription(const QString &description);

    QString iconName() const;
    void setIconName(const QString &icon);
    
    HbIcon entryItemIcon() const;
    void setEntryItemIcon(const HbIcon &icon);
    
    //SLOTS
    void _q_itemPressed(const QModelIndex &index);
    void _q_itemReleased(const QModelIndex &index);
    void _q_scrollingStarted();
    
    HbDataFormModelItem *modelItemFromModelIndex(const QModelIndex &index);
    
    CpSettingFormEntryItemData *mParent;
    HbDataForm *mDataForm;
    CpItemDataHelper *mItemDataHelper;
    QPointer<HbView> mSettingViewPtr;
    bool mItemPressed;
    bool mDataFormScrolling;
};


#endif //CPSETTINGFORMENTRYITEMDATA_P_H

//End of File
