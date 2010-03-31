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
#include "cpitemdatahelper.h"
#include <qvector>
#include <qlist>
#include <hbdataform.h>
#include <hbdataformmodelitem.h>

class CpItemDataHelperPrivate
{
public:
	class ItemConnection
	{
	public:
		ItemConnection() : item(0),receiver(0)
		{
		}

		bool operator == (const ItemConnection &other)
		{
			return item == other.item && signal == other.signal
				&& receiver == other.receiver && method == other.method;
		}

		HbDataFormModelItem *item;
		QString signal;
		QObject *receiver;
		QString method;
	};

	CpItemDataHelperPrivate()
	{
	}

	~CpItemDataHelperPrivate()
	{
	}

	void addConnection(HbDataFormModelItem *item,
		const QString &signal,
		QObject *receiver,
		const QString &method)
	{
		ItemConnection connection;
		connection.item = item;
		connection.signal = signal;
		connection.receiver = receiver;
		connection.method = method;
		mConnections.append(connection);
	}

	void removeConnection(HbDataFormModelItem *item,
		const QString &signal,
		QObject *receiver,
		const QString &method)
	{
		ItemConnection connection;
		connection.item = item;
		connection.signal = signal;
		connection.receiver = receiver;
		connection.method = method;
		int index = mConnections.indexOf(connection);
		if (index >= 0) {
			mConnections.remove(index);
		}
	}

	void addItemPrototype(HbAbstractViewItem *prototype)
	{
		mItemPrototypes.append(prototype);
	}

	void bindToForm(HbDataForm *form) const
	{
		foreach(const ItemConnection &connection,mConnections) {
			form->addConnection(connection.item,connection.signal.toAscii(),
				connection.receiver,connection.method.toAscii());
		}
		QList<HbAbstractViewItem *> protoTypeList = form->itemPrototypes();
		protoTypeList.append(mItemPrototypes);
        form->setItemPrototypes(protoTypeList);	
	}

public:
	QVector<ItemConnection> mConnections;
	QList<HbAbstractViewItem *>  mItemPrototypes;
};


/*!
    \class CpItemDataHelper
    \brief The CpItemDataHelper class is a helper class to help control panel plugins (group plugins) to connect slots to inner widgets of setting items.
	 And add prototypes to dataform.
 */


/*!
    Constructor of CpItemDataHelper.
 */
CpItemDataHelper::CpItemDataHelper()
: d(new CpItemDataHelperPrivate())
{
}


/*!
    Destructor of CpItemDataHelper.
 */
CpItemDataHelper::~CpItemDataHelper()
{
    delete d;
}

/*!
    Connect a slot to inner widget's signal of setting item.
 */
void CpItemDataHelper::addConnection(HbDataFormModelItem *item,
									 const QString &signal,
									 QObject *receiver,
									 const QString &method)
{
	d->addConnection(item,signal,receiver,method);
	emit connectionAdded(item,signal,receiver,method);
}

/*!
    Disconnect a slot to inner widget's signal of setting item.
 */
void CpItemDataHelper::removeConnection(HbDataFormModelItem *item,
										const QString &signal,
										QObject *receiver,
										const QString &method)
{
	d->removeConnection(item,signal,receiver,method);
	emit connectionRemoved(item,signal,receiver,method);
}

/*!
    Add a prototype to data form, to create custom widget for a custom setting item.
 */
void CpItemDataHelper::addItemPrototype(HbAbstractViewItem *prototype)
{
	d->addItemPrototype(prototype);
	emit prototypeAdded(prototype);
}

/*!
    Bind the connections and prototypes to a data form.
 */
void CpItemDataHelper::bindToForm(HbDataForm *form) const
{
	d->bindToForm(form);
}


