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
#include "cpmastervolumeslider.h"

CpMasterVolumeSlider::CpMasterVolumeSlider(QGraphicsItem *parent /* =0 */)
											:HbSlider(parent),
											 mPreviousValue(0)		 
{
	QList<HbSlider::SliderElement> elements;
    elements << HbSlider::IncreaseElement
			 << HbSlider::TrackElement
			 << HbSlider::DecreaseElement
			 << HbSlider::IconElement
			 << HbSlider::TextElement;

    setElements( elements );
	setOrientation(Qt::Horizontal);
	setMaximum(11);
	setMinimum(0);
	
	setIcon(HbSlider::DecreaseElement, HbIcon(":/icon/hb_vol_slider_decrement.svg"));
	setIcon(HbSlider::IncreaseElement, HbIcon(":/icon/hb_vol_slider_increment.svg"));
	setIcon(HbSlider::IconElement, HbIcon(":/icon/hb_vol_slider_muted.svg"));
	
	setIconCheckable(true);

	// set tick and beep label
	
	setMajorTickInterval(1);
	setTickPosition(Hb::SliderTicksBothSides);
	QStringList labelList;
	labelList<< tr("")
			 << tr("Beep");
			 
	this->setMajorTickLabels(labelList);

	connect(this, SIGNAL(iconToggled(bool)), 
			this, SLOT(onMuteIconToggled(bool)));
	connect(this, SIGNAL(valueChanged(int)),
			this, SLOT(onValueChanged(int)));
//	onValueChanged(value());
}

CpMasterVolumeSlider::~CpMasterVolumeSlider()
{
}

bool CpMasterVolumeSlider::isBeepMode()
{
	if (value() == minimum()+1) {
		return true;
	}
	else {
		return false;
	}
}

void CpMasterVolumeSlider::setBeepMode(bool isBeepMode)
{
	if (isBeepMode == true) {
		setValue(minimum()+1);	
	}
	else {
		setValue(mPreviousValue);
	}
}

bool CpMasterVolumeSlider::isSilentMode()
{
	if (value() == minimum()) {
		return true;
	}
	else {
		return false;
	}
}

void CpMasterVolumeSlider::setSilentMode(bool isSilentMode)
{
	if (isSilentMode == true) {
		setValue(minimum());	
	}
	else {
		setValue(mPreviousValue);
	}
}

int CpMasterVolumeSlider::normalValue()
{
	return (value()-1);
}

void CpMasterVolumeSlider::setNormalValue(int normalValue)
{
	setValue(normalValue+1);
}
	
void CpMasterVolumeSlider::onValueChanged(int value)
{
	if (this->minimum( ) == value) { 
        // volume is mute
        this->setIconChecked( true );
		emit silentActivated();
    } 
	else {
        // unmute volume
		this->setIconEnabled( true );
		this->setIconCheckable(false);
    this->setIconChecked( false );
    this->setIcon(HbSlider::IconElement, HbIcon(":/icon/hb_vol_slider_unmuted.svg"));
		this->setIconCheckable(true);
		
		
		if (value == 1) {
			emit beepActivated(); 
		}
		else {
			int valueMap = value - 1;
			emit normalValueChanged(valueMap);
		}
    }
}

void CpMasterVolumeSlider::onIncreaseIconClicked()
{
}

void CpMasterVolumeSlider::onDecreaseIconClicked()
{
}

void CpMasterVolumeSlider::onMuteIconToggled(bool isToggled)
{
	if (isToggled) {
		mPreviousValue = value();
		this->setIcon(HbSlider::IconElement, HbIcon(":/icon/hb_vol_slider_muted.svg"));
		setValue(minimum());
	}
	else {
		this->setIcon(HbSlider::IconElement, HbIcon(":/icon/hb_vol_slider_unmuted.svg"));
		if (mPreviousValue == minimum()||mPreviousValue == minimum()+1) {
			setValue(minimum()+1);
		}
		else {
			setValue(mPreviousValue);
		}
		mPreviousValue = 0;
	}
}

