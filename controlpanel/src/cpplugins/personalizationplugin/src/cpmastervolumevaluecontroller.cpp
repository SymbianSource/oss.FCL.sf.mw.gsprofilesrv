
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

#include "cpmastervolumevaluecontroller.h"
#include <cpitemdatahelper.h>
#include <hbdataformmodelitem.h>

#ifdef Q_OS_SYMBIAN
	#include <cpprofilemodel.h>
#endif

CpMasterVolumeValueController::CpMasterVolumeValueController(CpProfileModel *profileModel,
															 HbDataFormModelItem *masterVolumeItem,
															 CpItemDataHelper &itemDataHelper)
															 : mProfileModle(profileModel),
															 mMasterVolumeItem(masterVolumeItem)
{
	itemDataHelper.addConnection(mMasterVolumeItem,SIGNAL(beepActivated()),this,SLOT(onBeepActivated()));
	itemDataHelper.addConnection(mMasterVolumeItem,SIGNAL(silentActivated()),this,SLOT(onSilentActivated()));
	itemDataHelper.addConnection(mMasterVolumeItem,SIGNAL(normalValueChanged(int)),this,SLOT(onNormalValueChanged(int)));
	updateMasterVolumeValue();
}

CpMasterVolumeValueController::~CpMasterVolumeValueController()
{

}

void CpMasterVolumeValueController::onBeepActivated()
{
#ifdef Q_OS_SYMBIAN
	mProfileModle->activateBeep();
#endif
}

void CpMasterVolumeValueController::onSilentActivated()
{
#ifdef Q_OS_SYMBIAN
	mProfileModle->activateSilent();
#endif
}

void CpMasterVolumeValueController::onNormalValueChanged(int value)
{
#ifdef Q_OS_SYMBIAN
	mProfileModle->setRingVolume(value);
#endif
}

void CpMasterVolumeValueController::updateMasterVolumeValue()
{
#ifdef Q_OS_SYMBIAN
	if (mProfileModle->isBeep()) {
		mMasterVolumeItem->setContentWidgetData(QString("beepMode"),QVariant(true));
	}
	else if (mProfileModle->isSilent()) {
		mMasterVolumeItem->setContentWidgetData(QString("silentMode"),QVariant(true));
	}
	else {
		mMasterVolumeItem->setContentWidgetData(QString("normalValue"),QVariant(mProfileModle->ringVolume()));
	}
#endif
}

