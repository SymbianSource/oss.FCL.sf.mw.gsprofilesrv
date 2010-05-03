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
#include <QtAlgorithms>
#include <HbView.h>
#include <hbdataform.h>
#include <hbdataformmodel.h>
#include <hbdataformmodelitem.h>
#include <hbdataformviewitem.h>
#include <hbabstractviewitem.h>
#include <hbpushbutton.h>
//testing following classes

#include "cpbasesettingview.h"
#include "cpsettingformentryitemdata.h"
#include "cpsettingformentryitemdataimpl.h"
#include <cpsettingformitemdata.h>
#include <cpitemdatahelper.h>
#include <cppluginutility.h>
#include <cppluginloader.h>
#include <cpplugininterface.h>
#include <cplogger.h>

void TestCpAPI::initTestCase()
{    
	mainWindow = new HbMainWindow;
  mainWindow->show();
}

void TestCpAPI::cleanupTestCase()
{
	//delete mainWindow;
	//mainWindow = 0;
	//if delete mainwindow, test app will be freezing
	QCoreApplication::processEvents();
}
void TestCpAPI::testCpBaseSettingView()
{
	CpBaseSettingView * pview = new CpBaseSettingView(0);
	QVERIFY( pview != 0 );
        HbDataForm *pForm = new HbDataForm(0);
        pview->setSettingForm(pForm);
        QVERIFY( pview->settingForm() == pForm );
	delete pview;
	HbPushButton *widget = new HbPushButton();
	QVERIFY(widget != 0);
	CpBaseSettingView *pView1 = new CpBaseSettingView(widget);
	QVERIFY(pView1!=0);
	HbPushButton *button = qobject_cast<HbPushButton *>(pView1->widget());
	QVERIFY(button != 0);
	delete pView1;
	
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
    HbDataFormViewItem *pitem = new HbDataFormViewItem();
    phelper->addItemPrototype(pitem);
    delete phelper;
}
void TestCpAPI::testCpPluginUtility()
{
    HbDataForm* form = new HbDataForm();
    QList<HbAbstractViewItem *> oldList = form->itemPrototypes();
    CpPluginUtility::addCpItemPrototype(form);
    QList<HbAbstractViewItem *> newList = form->itemPrototypes();
    QVERIFY( oldList.count() < newList.count() );
}

void TestCpAPI::testCpPluginLoader()
{
    CpPluginInterface *plugin = CpPluginLoader::loadCpPlugin("non_existing_plugin.dll");
    QVERIFY(plugin == 0);

    plugin = CpPluginLoader::loadCpPlugin("cppincodeplugin");
    QVERIFY(plugin != 0);

    plugin = CpPluginLoader::loadCpPlugin("cppincodeplugin.dll");
    QVERIFY(plugin != 0);

    plugin = CpPluginLoader::loadCpPlugin("cppincodeplugin.qtplugin");
    QVERIFY(plugin != 0);
}

void TestCpAPI::testCpPluginInterface()
{
    CpPluginInterface *plugin = CpPluginLoader::loadCpPlugin("cppersonalizationplugin.dll");
    if (plugin) {
        CpItemDataHelper helper;
        QList<CpSettingFormItemData*> itemData = plugin->createSettingFormItemData(helper);
        QVERIFY(itemData.size() > 0);
        qDeleteAll(itemData.begin(),itemData.end());
        itemData.clear();
    }
}

void TestCpAPI::testCpLogger()
{
    const QString logConf = "C:/data/.config/ut_cpapi/controlpanellog.conf";
    Logger::instance(CPFW_LOGGER_NAME)->configure(logConf,QSettings::IniFormat);
    CPFW_LOG("Hello World!");
    Logger::closeAll();
}


QTEST_MAIN(TestCpAPI)
