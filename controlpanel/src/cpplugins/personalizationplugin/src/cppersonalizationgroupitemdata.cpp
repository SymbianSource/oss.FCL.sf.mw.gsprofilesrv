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
#include "cppersonalizationgroupitemdata.h"
#include <hbdataformmodelitem.h>
#include <cpsettingformentryitemdataimpl.h>
#include <cpbasesettingview.h>
#include <cpitemdatahelper.h>
#include <hbmessagebox.h>
#include "cppersonalizationcustomviewitem.h"
#include "cpmastervolumevaluecontroller.h"

#ifdef Q_OS_SYMBIAN
#include <cpprofilemodel.h>
#endif


CpPersonalizationGroupItemData::CpPersonalizationGroupItemData(HbDataFormModelItem::DataItemType type,
                                                               const QString &label,
                                                               const QString &configFile,
                                                               const HbDataFormModelItem *parent):
                                                               CpCategorySettingFormItemData(type, label, configFile, parent),
                                                               mMasterVolume(0),
                                                               mMasterVibra(0),
                                                               mRingTone(0),
															   mMessageTone(0),
															   mEmailTone(0),
															   mReminderTone(0),
															   mClockTone(0),
                                                               mProfileModel(0),
															   mMasterVolumeValueController(0)
{
#ifdef Q_OS_SYMBIAN
	mProfileModel = new CpProfileModel();
#endif
}

CpPersonalizationGroupItemData::~CpPersonalizationGroupItemData()
{
#ifdef Q_OS_SYMBIAN
	delete mProfileModel;
#endif
	delete mMasterVolumeValueController;
}

void CpPersonalizationGroupItemData::beforeLoadingConfigPlugins(CpItemDataHelper &itemDataHelper)
{
	itemDataHelper.addItemPrototype(new CpPersonalizationCustomViewItem);

	mMasterVolume = new HbDataFormModelItem(static_cast<HbDataFormModelItem::DataItemType>(MasterVolumeSliderItem));
	mMasterVolumeValueController = new CpMasterVolumeValueController(mProfileModel,mMasterVolume,itemDataHelper);

    //mMasterVolume->setContentWidgetData("maxmum",10);
    //mMasterVolume->setContentWidgetData("minmum",1);
	//itemDataHelper.addConnection(mMasterVolume,SIGNAL(valueChanged (int)),this,SLOT(masterVolumeValueChanged(int)) );
    this->appendChild(mMasterVolume);
    mMasterVibra = new HbDataFormModelItem(HbDataFormModelItem::CheckBoxItem,
                                                                 QString());
    mMasterVibra->setContentWidgetData("text",QObject::tr("Vibrating alerts"));
	itemDataHelper.addConnection(mMasterVibra,SIGNAL(stateChanged(int)),this,SLOT(onVibraValueChange(int)));
#ifdef Q_OS_SYMBIAN
	if (mProfileModel->vibraStatus()) {
		mMasterVibra->setContentWidgetData("checkState", Qt::Checked);
	}
	else {
		mMasterVibra->setContentWidgetData("checkState", Qt::Unchecked);
	}
#endif
    this->appendChild(mMasterVibra);
	


	mRingTone = new CpSettingFormEntryItemDataImpl<CpBaseSettingView>(itemDataHelper,
																		tr("Ring tone"),
																		tr("Nokia tone"));
	this->appendChild(mRingTone);
	mMessageTone = new CpSettingFormEntryItemDataImpl<CpBaseSettingView>(itemDataHelper,
																		tr("Message tone"),
																		tr("Nokia message tone"));
	this->appendChild(mMessageTone);
	mEmailTone = new CpSettingFormEntryItemDataImpl<CpBaseSettingView>(itemDataHelper,
																		tr("E-mail tone"),
																		tr("Nokia e-mail tone"));
	this->appendChild(mEmailTone);
	mReminderTone = new CpSettingFormEntryItemDataImpl<CpBaseSettingView>(itemDataHelper,
																		tr("Reminder tone"),
																		tr("Nokia calendar tone"));
	this->appendChild(mReminderTone);
	mClockTone = new CpSettingFormEntryItemDataImpl<CpBaseSettingView>(itemDataHelper,
																		tr("Clock tone"),
																		tr("Nokia clock tone"));
	this->appendChild(mClockTone);

	
}

void CpPersonalizationGroupItemData::masterVolumeValueChanged(int value)
{
	Q_UNUSED(value);
    HbMessageBox::launchInformationMessageBox(QString("volume changed to:%1").arg(value));
}
void CpPersonalizationGroupItemData::onVibraValueChange(int isVibra)
{
	//Q_UNUSED(isVibra);
#ifdef Q_OS_SYMBIAN
	if (isVibra) {
		mProfileModel->setVibraStatus(true);
	}
	else {
		mProfileModel->setVibraStatus(false);
	}
#endif
}


