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
#include "cpremotelockdataformviewitem.h"
#include <hblabel.h>

CpRemoteLockDataFormViewItem::CpRemoteLockDataFormViewItem(QGraphicsItem *parent )
: HbDataFormViewItem(parent)
{
}

CpRemoteLockDataFormViewItem::~CpRemoteLockDataFormViewItem()
{
}

HbAbstractViewItem* CpRemoteLockDataFormViewItem::createItem()
{
	return new CpRemoteLockDataFormViewItem(*this);
}

bool CpRemoteLockDataFormViewItem::canSetModelIndex(const QModelIndex &index) const
{
	int type = index.data(HbDataFormModelItem::ItemTypeRole).toInt();
	return type == CpRemoteLockItem;
}

HbWidget *CpRemoteLockDataFormViewItem::createCustomWidget()
{
    int type = modelIndex().data(HbDataFormModelItem::ItemTypeRole).toInt();

	if (type == CpRemoteLockItem) {
		HbLabel *label = new HbLabel("Device can be locked remotely by sending lock code to the device as SMS.");
		label->setElideMode  (Qt::ElideNone );
		label->setTextWrapping(Hb::TextWordWrap);
		return label;
	}

	return 0;
}

