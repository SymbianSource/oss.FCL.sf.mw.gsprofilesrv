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
#ifndef CP_ITEM_DATA_HELPER_H
#define CP_ITEM_DATA_HELPER_H

#include <qobject>
#include <cpglobal.h>

class QObject;
class HbDataForm;
class HbAbstractViewItem;
class HbDataFormModelItem;
class CpItemDataHelperPrivate;

class CP_EXPORT CpItemDataHelper : public QObject
{
	Q_OBJECT
public:
    CpItemDataHelper();
    ~CpItemDataHelper();

    void addConnection(HbDataFormModelItem *item,
        const QString &signal,
        QObject *receiver,
        const QString &method);

    void removeConnection(HbDataFormModelItem *item,
        const QString &signal,
        QObject *receiver,
        const QString &method);

	void addItemPrototype(HbAbstractViewItem *prototype);

	void bindToForm(HbDataForm *form) const;

signals:
	void connectionAdded(HbDataFormModelItem *item,
		const QString &signal,
		QObject *receiver,
		const QString &method);
	void connectionRemoved(HbDataFormModelItem *item,
		const QString &signal,
		QObject *receiver,
		const QString &method);
	void prototypeAdded(HbAbstractViewItem *prototype);
private:
    CpItemDataHelperPrivate *d;
};

#endif
