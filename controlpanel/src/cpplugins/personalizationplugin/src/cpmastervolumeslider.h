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
#ifndef CP_MASTERVOLUMESLIDER_H
#define CP_MASTERVOLUMESLIDER_H

#include <hbslider.h>
#include <hbmessagebox.h>
class CpMasterVolumeSlider: public HbSlider
{
	Q_OBJECT
	Q_PROPERTY(bool beepMode READ isBeepMode WRITE setBeepMode)
	Q_PROPERTY(bool silentMode READ isSilentMode WRITE setSilentMode)
	Q_PROPERTY(int normalValue READ normalValue WRITE setNormalValue)
	
public:
	explicit CpMasterVolumeSlider(QGraphicsItem *parent = 0);
	~CpMasterVolumeSlider();
	
public:
	bool isBeepMode();
	void setBeepMode(bool isBeepMode);
	bool isSilentMode();
	void setSilentMode(bool isSilentMode);
	int normalValue();
	void setNormalValue(int normalValue);
	
signals:
	void beepActivated();
	void silentActivated();
	void normalValueChanged(int value);
private slots:
	void onValueChanged(int value);
	void onIncreaseIconClicked();
	void onDecreaseIconClicked();
	void onMuteIconToggled(bool isToggled);
private:
	int mPreviousValue;
};

#endif

