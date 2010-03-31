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


#ifndef CPREMOTELOCKDATAFORMVIEWITEM_H
#define CPREMOTELOCKDATAFORMVIEWITEM_H

#include <hbdataformviewitem.h>
#include <hbdataformmodelitem.h>

class CpRemoteLockDataFormViewItem : public HbDataFormViewItem
{
	Q_OBJECT
public:
	enum { CpRemoteLockItem = HbDataFormModelItem::CustomItemBase + 10 };

    explicit CpRemoteLockDataFormViewItem(QGraphicsItem *parent = 0);
    virtual ~CpRemoteLockDataFormViewItem();
    virtual HbAbstractViewItem* createItem();
	virtual bool canSetModelIndex(const QModelIndex &index) const;
protected:
    virtual HbWidget* createCustomWidget();
};

#endif //CPREMOTELOCKDATAFORMVIEWITEM_H

