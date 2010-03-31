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
#include "cpktfdbkview.h"
#include <hbdataform.h>
#include <qstringlist>
#include <qdebug>
#include <hbdataformmodel.h>
#include <cpsettingformitemdata.h>
#include <hbmessagebox.h>
#include <cpprofilemodel.h>


CpKtfdbkView::CpKtfdbkView(QGraphicsItem *parent) :
    CpBaseSettingView(0,parent),
    mKTSliderItem(0),
    mSTSliderItem(0),
    mSVSliderItem(0),
	mModel(0)

{
    HbDataForm *form = settingForm();
    if (form) {
		mModel = new CpProfileModel();
        HbDataFormModel *model = new HbDataFormModel;
        
        form->setHeading(tr("Key & touch screen feedback"));
// Key tones slider
        mKTSliderItem = new CpSettingFormItemData(HbDataFormModelItem::SliderItem,
                QString("Key tones"));
        form->addConnection(mKTSliderItem,SIGNAL(valueChanged(int)),this,SLOT(KTsliderValueChanged(int)));

		model->appendDataFormItem(mKTSliderItem, model->invisibleRootItem());
//Screen tones        
        mSTSliderItem = new CpSettingFormItemData(HbDataFormModelItem::SliderItem,
                QString("Screen tones"));
        form->addConnection(mSTSliderItem,SIGNAL(valueChanged(int)),this,SLOT(STsliderValueChanged(int)));

        model->appendDataFormItem(mSTSliderItem, model->invisibleRootItem());
//Screen vibra
        mSVSliderItem = new CpSettingFormItemData(HbDataFormModelItem::SliderItem,
                QString("Screen vibra"));
        form->addConnection(mSVSliderItem,SIGNAL(valueChanged(int)),this,SLOT(SVsliderValueChanged(int)));

        model->appendDataFormItem(mSVSliderItem, model->invisibleRootItem());
//initialize sliders
		mKTSliderItem->setContentWidgetData( QString("value"), mModel->keyVolume() );
                mKTSliderItem->setContentWidgetData( QString("minimum"), 0 );
		mKTSliderItem->setContentWidgetData( QString("maximum"), 3 );

		mSTSliderItem->setContentWidgetData( QString("value"), mModel->screenVolume() );
		mSTSliderItem->setContentWidgetData( QString("minimum"), 0 );
		mSTSliderItem->setContentWidgetData( QString("maximum"), 3 );

		mSVSliderItem->setContentWidgetData( QString("value"), mModel->screenVibra() );
		mSVSliderItem->setContentWidgetData( QString("minimum"), 0 );
		mSVSliderItem->setContentWidgetData( QString("maximum"), 3 );
//
        form->setModel(model);
//
    }
    

}
CpKtfdbkView::~CpKtfdbkView()
{
	if(mModel)	delete mModel;
	mModel = 0;
}

void CpKtfdbkView::KTsliderValueChanged(int value)
{
	if(mModel)	mModel->setKeyVolume( value );
}

void CpKtfdbkView::STsliderValueChanged(int value)
{
	if(mModel)	mModel->setScreenVolume( value );
}

void CpKtfdbkView::SVsliderValueChanged(int value)
{
	if(mModel)	mModel->setScreenVibra( value );
}


