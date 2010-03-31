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

#include "cpdevicelockpluginview.h"
#include <QStringList>
#include <hblineedit.h>
#include <hbdataform.h>
#include <hbdataformmodel.h>
#include <hbdataformmodelitem.h>
#include "cpremotelockdataformviewitem.h"

CpDeviceLockPluginView::CpDeviceLockPluginView(QGraphicsItem *parent /*= 0*/)
: CpBaseSettingView(0,parent)
{
	if (HbDataForm *form = settingForm()) {

		QList<HbAbstractViewItem *> protoTypeList = form->itemPrototypes();
		protoTypeList.append(new CpRemoteLockDataFormViewItem());
        form->setItemPrototypes(protoTypeList);

		HbDataFormModel *formModel = new HbDataFormModel();

		form->setHeading(tr("Device lock"));

		HbDataFormModelItem *deviceLockItem = new HbDataFormModelItem(
			HbDataFormModelItem::ToggleValueItem,tr("Device lock"));
    
		deviceLockItem->setContentWidgetData("text",tr("On"));
		deviceLockItem->setContentWidgetData("additionalText",tr("Off"));

		formModel->appendDataFormItem(deviceLockItem);

		HbDataFormModelItem *lockCodeItem = new HbDataFormModelItem(
			HbDataFormModelItem::TextItem,tr("Lock code"));
		lockCodeItem->setContentWidgetData("echoMode",HbLineEdit::Password);
		lockCodeItem->setContentWidgetData("text","1111");
		lockCodeItem->setContentWidgetData("readOnly",true);

		formModel->appendDataFormItem(lockCodeItem);

		HbDataFormModelItem *deviceLockAfterItem = new HbDataFormModelItem(
			HbDataFormModelItem::RadioButtonListItem,tr("Device will be locked automatically after:"));

		QStringList items;
		items << tr("15 minutes") << tr("30 minutes") << tr("1 hour") << tr("4 hours") ;
		deviceLockAfterItem->setContentWidgetData("items",items);
		deviceLockAfterItem->setContentWidgetData("selected",0);

		formModel->appendDataFormItem(deviceLockAfterItem);

		HbDataFormModelItem *remoteLockItem = new HbDataFormModelItem(
			static_cast<HbDataFormModelItem::DataItemType>(CpRemoteLockDataFormViewItem::CpRemoteLockItem),
			tr("Remote locking"));
		remoteLockItem->setContentWidgetData("plainText",
			tr("Device can be locked remotely by sending lock code to the device as SMS."));
		remoteLockItem->setContentWidgetData("TextWrapping",Hb::TextWordWrap);

		formModel->appendDataFormItem(remoteLockItem);

		form->setModel(formModel);
	}
}

CpDeviceLockPluginView::~CpDeviceLockPluginView()
{
}


