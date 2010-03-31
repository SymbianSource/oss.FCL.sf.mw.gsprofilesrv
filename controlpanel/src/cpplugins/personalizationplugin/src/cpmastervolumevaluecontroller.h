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
#ifndef CPMASTERVOLUMEVALUECONTROLLER_H
#define CPMASTERVOLUMEVALUECONTROLLER_H

#include <qobject>

class CpProfileModel;
class HbDataFormModelItem;
class CpItemDataHelper;

class CpMasterVolumeValueController : public QObject
{
	Q_OBJECT
public:
	CpMasterVolumeValueController(CpProfileModel *profileModel,
		HbDataFormModelItem *masterVolumeItem,
		CpItemDataHelper &itemDataHelper);
	virtual ~CpMasterVolumeValueController();
private slots:
	void onBeepActivated();
	void onSilentActivated();
	void onNormalValueChanged(int value);
private:
	void updateMasterVolumeValue();
private:
	CpProfileModel *mProfileModle;
	HbDataFormModelItem *mMasterVolumeItem;
};

#endif
