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

#include "cpbasesettingview_p.h"
#include "cpbasesettingview.h"
#include <hbinstance.h>
#include <hbaction.h>
#include <hbmainwindow.h>
#include <hbdataform.h>
#include <hbdataformmodel.h>
#include <qgraphicslayout>
#include "cpitemdatahelper.h"
#include "cpdataformviewitem.h"
#include "cpsettingformentryitemdata.h"

static HbMainWindow *mainWindow() 
{
    QList< HbMainWindow* > mainWindows = hbInstance->allMainWindows();
    if (!mainWindows.isEmpty()) {
        return mainWindows.front();
    }
    return 0;
}


CpBaseSettingViewPrivate::CpBaseSettingViewPrivate() :
    mBaseSettingView(0),
    mSoftKeyBackAction(0),
    mSettingForm(0),
    mIsActiveView(false)
{
}

CpBaseSettingViewPrivate::~CpBaseSettingViewPrivate()
{
    clearSoftkey();
    
    delete mSoftKeyBackAction;
    mSoftKeyBackAction = 0;
}

void CpBaseSettingViewPrivate::init(HbDataForm *settingForm,CpBaseSettingView *baseSettingView)
{
    mBaseSettingView = baseSettingView;
    
    if (!settingForm) {
        settingForm = new HbDataForm();

		QList<HbAbstractViewItem *> protoTypeList = settingForm->itemPrototypes();
		protoTypeList.append(new CpDataFormViewItem(CpSettingFormEntryItemData::EntryItem, 
													mSettingForm));
        settingForm->setItemPrototypes(protoTypeList);
    }    
    
    setSettingForm(settingForm);
    
    mBaseSettingView->setTitle("Control Panel");	//give a default title, sub classes need set it correctly

    mSoftKeyBackAction = new HbAction(Hb::BackAction, mBaseSettingView);
    QObject::connect(mSoftKeyBackAction, 
            SIGNAL(triggered()), 
            mBaseSettingView, 
            SLOT(_q_softkeyClicked()) );

    if (mainWindow()) {
        QObject::connect(mainWindow(), 
                SIGNAL(currentViewChanged(HbView *)), 
                mBaseSettingView, 
                SLOT(_q_viewChanged(HbView *)) );
    }

    setSoftkey();
}

void CpBaseSettingViewPrivate::setSettingForm(HbDataForm *settingForm)
{
	mSettingForm = settingForm;
    mBaseSettingView->setWidget(mSettingForm); 

	QList<HbAbstractViewItem *> protoTypeList = settingForm->itemPrototypes();
	// add the control panel custom proto type to data form
	protoTypeList.append(new CpDataFormViewItem(CpSettingFormEntryItemData::EntryItem,
												mSettingForm));
    settingForm->setItemPrototypes(protoTypeList);
}

void CpBaseSettingViewPrivate::setSoftkey()
{
    if (mainWindow()) {
        mainWindow()->addSoftKeyAction(Hb::SecondarySoftKey,
                mSoftKeyBackAction);
    }
}

void CpBaseSettingViewPrivate::clearSoftkey()
{
    if (mainWindow())
    {
        mainWindow()->removeSoftKeyAction(Hb::SecondarySoftKey,
                mSoftKeyBackAction);
    }
}

void CpBaseSettingViewPrivate::_q_softkeyClicked()
{
    emit mBaseSettingView->aboutToClose();
}

void CpBaseSettingViewPrivate::_q_viewChanged(HbView *view)
{
    if (mIsActiveView && view != mBaseSettingView) {
        mIsActiveView = false;
        clearSoftkey();
    }
    else if (!mIsActiveView && mBaseSettingView == view) {
        mIsActiveView = true;
        setSoftkey();
    }
}

#include "moc_cpbasesettingview.cpp"

