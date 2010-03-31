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

#include "cppersonalizationplugin.h"
#include "cppersonalizationgroupitemdata.h"
#include <cpcategorysettingformitemdata.h>
#include <cpitemdatahelper.h>

CpPersonalizationPlugin::CpPersonalizationPlugin()
{
}

CpPersonalizationPlugin::~CpPersonalizationPlugin()
{
}

CpSettingFormItemData *CpPersonalizationPlugin::createSettingFormItemData(CpItemDataHelper &/*itemDataHelper*/) const
{
    CpPersonalizationGroupItemData *itemData = 
            new CpPersonalizationGroupItemData(
            HbDataFormModelItem::GroupItem,
            QString("Tones"),
            QString("cppersonalizationplugin.cpcfg") );
    return itemData;
}

Q_EXPORT_PLUGIN2(cppersonalizationplugin, CpPersonalizationPlugin);


