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
#include "cpcommunicationgroupitemdata.h"
#include <QStringList>

CpCommunicationGroupItemData::CpCommunicationGroupItemData(const QString &configFile,
                                                           const HbDataFormModelItem *parent):
CpCategorySettingFormItemData(configFile,parent)
{

}

CpCommunicationGroupItemData::CpCommunicationGroupItemData(HbDataFormModelItem::DataItemType type,
                                                           const QString &label,
                                                           const QString &configFile,
                                                           const HbDataFormModelItem *parent):
CpCategorySettingFormItemData(type, label, configFile, parent)
{

}
CpCommunicationGroupItemData::~CpCommunicationGroupItemData()
{
    
}
void CpCommunicationGroupItemData::beforeLoadingConfigPlugins(CpItemDataHelper &/*itemDataHelper*/)
{
    HbDataFormModelItem *airPlaneMode = new HbDataFormModelItem(HbDataFormModelItem::ToggleValueItem,
                                                                QString("Airplane Mode"));
    QStringList airModeList;
    airModeList << QObject::tr("On") << QObject::tr("Off");
    
    airPlaneMode->setContentWidgetData("text",airModeList.at(1));
    airPlaneMode->setContentWidgetData("additionalText",airModeList.at(0));

    this->appendChild(airPlaneMode);
    
}
