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
#include "cppincodepluginview.h"
#include <hbdataform.h>
#include <hbdataformmodel.h>
#include <hbdataformmodelitem.h>
#include <hblineedit.h>
#include <seccodeeditdataformviewitem.h>
#include <seccodesettings.h>

CpPinCodePluginView::CpPinCodePluginView(QGraphicsItem *parent /*= 0*/)
: CpBaseSettingView(0,parent), mSecCodeSettings(new SecCodeSettings())
{
	if (HbDataForm *form = settingForm()) {

		QList<HbAbstractViewItem *> protoTypeList = form->itemPrototypes();
		protoTypeList.append(new SecCodeEditDataFormViewItem());
        form->setItemPrototypes(protoTypeList);
		
		form->setHeading(tr("PIN code"));

		HbDataFormModel *formModel = new HbDataFormModel();

		HbDataFormModelItem *pinCodeRequestItem = new HbDataFormModelItem(
			HbDataFormModelItem::ToggleValueItem,tr("PIN code requests"));
    
		pinCodeRequestItem->setContentWidgetData("text",tr("On"));
		pinCodeRequestItem->setContentWidgetData("additionalText",tr("Off"));

		formModel->appendDataFormItem(pinCodeRequestItem);

		HbDataFormModelItem *pinCodeItem = new HbDataFormModelItem(
			static_cast<HbDataFormModelItem::DataItemType>(SecCodeEditDataFormViewItem::SecCodeEditItem),
			tr("PIN code"));
		pinCodeItem->setContentWidgetData("echoMode",HbLineEdit::Password);
		pinCodeItem->setContentWidgetData("text","1111");
		pinCodeItem->setContentWidgetData("readOnly",true);
		form->addConnection(pinCodeItem,SIGNAL(clicked()),this,SLOT(onPinCodeClicked()));

		formModel->appendDataFormItem(pinCodeItem);

		HbDataFormModelItem *pin2CodeItem = new HbDataFormModelItem(
			 static_cast<HbDataFormModelItem::DataItemType>(SecCodeEditDataFormViewItem::SecCodeEditItem),
			 tr("PIN2 code"));
		pin2CodeItem->setContentWidgetData("echoMode",HbLineEdit::Password);
		pin2CodeItem->setContentWidgetData("text","1111");
		pin2CodeItem->setContentWidgetData("readOnly",true);
		form->addConnection(pin2CodeItem,SIGNAL(clicked()),this,SLOT(onPin2CodeClicked()));

		formModel->appendDataFormItem(pin2CodeItem);

		form->setModel(formModel);
	}
}

CpPinCodePluginView::~CpPinCodePluginView()
{
	delete mSecCodeSettings;
}

void CpPinCodePluginView::onPinCodeClicked()
{
	mSecCodeSettings->changePinCode();
}

void CpPinCodePluginView::onPin2CodeClicked()
{
	mSecCodeSettings->changePin2Code();
}
