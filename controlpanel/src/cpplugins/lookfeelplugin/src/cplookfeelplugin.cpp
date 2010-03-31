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

#include "cplookfeelplugin.h"
#include "cpcategorysettingformitemdata.h"

CpLookFeelPlugin::CpLookFeelPlugin()
{
}

CpLookFeelPlugin::~CpLookFeelPlugin()
{
}

CpSettingFormItemData *CpLookFeelPlugin::createSettingFormItemData(CpItemDataHelper &/*itemDataHelper*/) const
{
    CpCategorySettingFormItemData *itemData = 
            new CpCategorySettingFormItemData(
            HbDataFormModelItem::GroupItem,
            tr("Look&Feel"),
            QString("cplookfeelplugin.cpcfg") );
    return itemData;
}

Q_EXPORT_PLUGIN2(cplookfeelplugin, CpLookFeelPlugin);
