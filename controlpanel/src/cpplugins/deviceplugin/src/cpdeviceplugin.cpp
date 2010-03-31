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

#include "cpdeviceplugin.h"
#include "cpcategorysettingformitemdata.h"
#include <cpitemdatahelper.h>

CpDevicePlugin::CpDevicePlugin()
{
}

CpDevicePlugin::~CpDevicePlugin()
{
}

CpSettingFormItemData *CpDevicePlugin::createSettingFormItemData(CpItemDataHelper &/*itemDataHelper*/) const
{
    CpCategorySettingFormItemData *itemData = 
            new CpCategorySettingFormItemData(
            HbDataFormModelItem::GroupItem,
            tr("Device"),
            QString("cpdeviceplugin.cpcfg") );
    return itemData;
}

Q_EXPORT_PLUGIN2(cpdeviceplugin, CpDevicePlugin);
