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
#include "ut_cpapi.h"

#include <hbmainwindow.h>
#include <QGraphicsWidget>
#include <qDebug>
#include <QtTest/QtTest>
#include <HbView.h>
#include <hbdataform.h>
#include <hbdataformmodel.h>
#include <hbdataformmodelitem.h>
#include <hbabstractviewitem.h>
#include <hbsettingformitem.h>
//testing following classes

#include "cpbasesettingview.h"
#include "cpsettingformentryitemdata.h"
#include "cpsettingformentryitemdataimpl.h"
#include <cpsettingformitemdata.h>
#include <cpitemdatahelper.h>

void TestCpAPI::initTestCase()
{    
	mainWindow = new HbMainWindow;
  mainWindow->show();
}

void TestCpAPI::cleanupTestCase()
{
	delete mainWindow;
	mainWindow = 0;
}
void TestCpAPI::testCpBaseSettingView()
{
	CpBaseSettingView * pview = new CpBaseSettingView(0);
	QVERIFY( pview !=0 );
        HbDataForm *pForm = new HbDataForm(0);
        pview->setSettingForm(pForm);
        QVERIFY( pview->settingForm() == pForm );
	delete pview;
}

void TestCpAPI::testCpSettingFormEntryItemDataImpl()
{
	CpItemDataHelper *pHelper = new CpItemDataHelper();
        CpSettingFormEntryItemDataImpl<CpBaseSettingView> *pdataimp = new CpSettingFormEntryItemDataImpl<CpBaseSettingView>( *pHelper, "text", "description"  );
	QVERIFY( pdataimp !=0 );
	QString strtxt = "input texts";
	pdataimp->setText(strtxt);
	QVERIFY( pdataimp->text() == strtxt );
	
	QString strDes = "input descriptions";
	pdataimp->setDescription(strDes);
	QVERIFY( pdataimp->description() == strDes);
	delete pdataimp;
        delete pHelper;
}
void TestCpAPI::testCpSettingFormItemData()
{
        CpSettingFormItemData *pdata = new CpSettingFormItemData( HbDataFormModelItem::TextItem, "label" );
        QVERIFY( pdata !=0 );
        delete pdata;
        pdata = 0;
        pdata = new CpSettingFormItemData( HbDataFormModelItem::SliderItem, "label" );
        QVERIFY( pdata !=0 );
        delete pdata;
        pdata = 0;
        pdata = new CpSettingFormItemData( HbDataFormModelItem::VolumeSliderItem, "label" );
        QVERIFY( pdata !=0 );
        delete pdata;
        pdata = new CpSettingFormItemData( HbDataFormModelItem::CheckBoxItem, "label" );
        QVERIFY( pdata !=0 );
        delete pdata;
        pdata = 0;
        pdata = new CpSettingFormItemData( HbDataFormModelItem::ToggleValueItem, "label" );
        QVERIFY( pdata !=0 );
        delete pdata;
        pdata = 0;
        pdata = new CpSettingFormItemData( HbDataFormModelItem::RadioButtonListItem, "label" );
        QVERIFY( pdata !=0 );
        delete pdata;
        pdata = 0;
        pdata = new CpSettingFormItemData( HbDataFormModelItem::MultiselectionItem, "label" );
        QVERIFY( pdata !=0 );
        delete pdata;
        pdata = 0;
        pdata = new CpSettingFormItemData( HbDataFormModelItem::ComboBoxItem, "label" );
        QVERIFY( pdata !=0 );
        delete pdata;
        pdata = 0;
}
void TestCpAPI::testItemDataHelper()
{
    HbDataForm* form = new HbDataForm();
    HbDataFormModel *model = new HbDataFormModel();
    HbDataFormModelItem *general = model->appendDataFormPage(QString("General"));
    CpItemDataHelper *phelper = new CpItemDataHelper();
    phelper->addConnection( general, "hello",form,"hello");
    phelper->removeConnection(general,"hello",form, "hello");
    phelper->bindToForm(form);
    HbSettingFormItem *pitem = new HbSettingFormItem();
    phelper->addItemPrototype(pitem);
    delete phelper;
}
QTEST_MAIN(TestCpAPI)
