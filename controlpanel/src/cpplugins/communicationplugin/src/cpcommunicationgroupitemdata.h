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
#ifndef CPCOMMUNICATIONGROUPITEMDATA_H
#define CPCOMMUNICATIONGROUPITEMDATA_H

#include <cpcategorysettingformitemdata.h>
class CpItemDataHelper;
class CpCommunicationGroupItemData: public CpCategorySettingFormItemData
{
public:
    explicit CpCommunicationGroupItemData(const QString &configFile = QString(),
        const HbDataFormModelItem *parent = 0);

    CpCommunicationGroupItemData(HbDataFormModelItem::DataItemType type,
        const QString &label,
        const QString &configFile = QString(),
        const HbDataFormModelItem *parent = 0);

    ~CpCommunicationGroupItemData();
private:
    virtual void beforeLoadingConfigPlugins(CpItemDataHelper &itemDataHelper);
};
#endif /* CPCOMMUNICATIONGROUPITEMDATA_H */
