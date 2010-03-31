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

#include "cpmainview.h"
#include "cpmainmodel.h"
#include <qlist>
#include <hbdataform.h>
#include <cpitemdatahelper.h>

CpMainView::CpMainView(QGraphicsItem *parent /*= 0*/)
: CpBaseSettingView(0,parent), mMainModel(0)
{
	if (HbDataForm *form = settingForm()) {
		mMainModel = new CpMainModel;
		form->setModel(mMainModel);

		mMainModel->itemDataHelper()->bindToForm(form);

		connect(mMainModel->itemDataHelper(),
			SIGNAL(connectionAdded(HbDataFormModelItem*,QString,QObject *,QString)),
			this,
			SLOT(onConnectionAdded(HbDataFormModelItem*,QString,QObject *,QString)));

		connect(mMainModel->itemDataHelper(),
			SIGNAL(connectionRemoved(HbDataFormModelItem*,QString,QObject *,QString)),
			this,
			SLOT(onConnectionRemoved(HbDataFormModelItem*,QString,QObject *,QString)));

		connect(mMainModel->itemDataHelper(),
			SIGNAL(prototypeAdded(HbAbstractViewItem *)),
			this,
			SLOT(onPrototypeAdded(HbAbstractViewItem *)));
	}	
	setTitle( QObject::tr("Control Panel") );	//should use qtTrId("txt_cp_title_control_panel")
}

CpMainView::~CpMainView()
{
    delete mMainModel;
}

void CpMainView::onConnectionAdded(HbDataFormModelItem *item,
								   const QString &signal,
								   QObject *receiver,
								   const QString &method)
{
	if (HbDataForm *form = settingForm()) {
		form->addConnection(item,signal.toAscii(),receiver,method.toAscii());
	}
}

void CpMainView::onConnectionRemoved(HbDataFormModelItem *item,
									 const QString &signal,
									 QObject *receiver,
									 const QString &method)
{
	if (HbDataForm *form = settingForm()) {
		form->removeConnection(item,signal.toAscii(),receiver,method.toAscii());
	}
}

void CpMainView::onPrototypeAdded(HbAbstractViewItem *prototype)
{
	if (HbDataForm *form = settingForm()) {
		QList<HbAbstractViewItem *> prototypes = form->itemPrototypes();
		prototypes.append(prototype);
		form->setItemPrototypes(prototypes);
	}
}

//
