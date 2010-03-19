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
#include <QGraphicsLayout>
#include "cpitemdatahelper.h"
#include "cpdataformviewitem.h"
#include "cpsettingformentryitemdata.h"
#include "cppluginutility.h"

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
    //mSettingForm(0),
    mIsActiveView(false)
{
}

CpBaseSettingViewPrivate::~CpBaseSettingViewPrivate()
{
    clearSoftkey();
    
    delete mSoftKeyBackAction;
    mSoftKeyBackAction = 0;
}

void CpBaseSettingViewPrivate::init(QGraphicsWidget *widget,CpBaseSettingView *baseSettingView)
{
    mBaseSettingView = baseSettingView;
    
    if (!widget) {
        widget = new HbDataForm();
		CpPluginUtility::addCpItemPrototype(qobject_cast<HbDataForm *>(widget));
    }    
    
    //setSettingForm(settingForm);
	mBaseSettingView->setWidget(widget);
    
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
    mBaseSettingView->setWidget(settingForm); 
    CpPluginUtility::addCpItemPrototype(settingForm);
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

