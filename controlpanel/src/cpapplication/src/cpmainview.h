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
#ifndef CPMAINVIEW_H
#define CPMAINVIEW_H

#include <cpbasesettingview.h>

class CpMainModel;
class HbAbstractViewItem;
class HbDataFormModelItem;

class CpMainView : public CpBaseSettingView
{
	Q_OBJECT
public:
    explicit CpMainView(QGraphicsItem *parent = 0);
    virtual ~CpMainView();
private slots:
	void onConnectionAdded(HbDataFormModelItem *item,\
		const QString &signal,
		QObject *receiver,
		const QString &method);

	void onConnectionRemoved(HbDataFormModelItem *item,
		const QString &signal,
		QObject *receiver,
		const QString &method);

	void onPrototypeAdded(HbAbstractViewItem *prototype); 
private:
    Q_DISABLE_COPY(CpMainView)
private:
	CpMainModel *mMainModel;
};

#endif
//End of file

