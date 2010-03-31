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
#include "cpdataformviewitem.h"
#include <hbdataformmodelitem.h>
#include <hbpushbutton.h>
#include <hbdataformmodel.h>
#include <hbabstractitemview.h>
#include <qmetaproperty>

class CpDataFormViewItemPrivate 
{
public:
	CpDataFormViewItemPrivate() : mCustomId(0), mWidget(0)
	{
	}
	~CpDataFormViewItemPrivate()
	{
	}
	void init(int customId)
	{
		mCustomId = customId;
	}
private:
	int mCustomId;
	HbWidget *mWidget;
	friend class CpDataFormViewItem;
};

CpDataFormViewItem::CpDataFormViewItem(int customId, QGraphicsItem *parent)
: HbDataFormViewItem(parent),d_ptr(new CpDataFormViewItemPrivate())
{
	d_ptr->init(customId);
}

CpDataFormViewItem::~CpDataFormViewItem()
{
	delete d_ptr;
}

HbAbstractViewItem* CpDataFormViewItem::createItem()
{
	return new CpDataFormViewItem(*this);
}

CpDataFormViewItem::CpDataFormViewItem(const CpDataFormViewItem &other)
: HbDataFormViewItem(other), d_ptr(new CpDataFormViewItemPrivate(*other.d_ptr))
{
	
}

CpDataFormViewItem &CpDataFormViewItem::operator=(const CpDataFormViewItem &other)
{
	if (&other == this) {
		return *this;
	}

	*d_ptr = *(other.d_ptr);
	return *this;
}

bool CpDataFormViewItem::canSetModelIndex(const QModelIndex &index) const
{
	int itemTypeId = index.data(HbDataFormModelItem::ItemTypeRole).toInt();
	// currently one proto type only can set one widget
	return (itemTypeId == d_ptr->mCustomId);
}

HbWidget* CpDataFormViewItem::createCustomWidget()
{
    HbDataFormModelItem::DataItemType itemType = static_cast<HbDataFormModelItem::DataItemType>(
        modelIndex().data(HbDataFormModelItem::ItemTypeRole).toInt());

	if (itemType == d_ptr->mCustomId) {
		HbPushButton *button = new HbPushButton(QString("Push button"));
		d_ptr->mWidget = button;
		button->setMinimumHeight(50);
		button->setMaximumHeight(60);
		button->setOrientation(Qt::Horizontal);
		return button;
	}
    
    return 0;
}

void CpDataFormViewItem::load()
{
	HbDataFormViewItem::load();

	if (d_ptr->mWidget) {
		HbDataFormModelItem::DataItemType itemType = static_cast<HbDataFormModelItem::DataItemType>(
			modelIndex().data(HbDataFormModelItem::ItemTypeRole).toInt());

		if(itemType == d_ptr->mCustomId) {

			QModelIndex itemIndex = modelIndex();
			HbDataFormModel *model = static_cast<HbDataFormModel*>(itemView()->model());;
			HbDataFormModelItem *modelItem = static_cast<HbDataFormModelItem*>(
				model->itemFromIndex(itemIndex));

			const QMetaObject *metaObj = d_ptr->mWidget->metaObject();
			int count = metaObj->propertyCount();
			for (int i = 0; i < count; i++) {
				QMetaProperty metaProperty = metaObj->property(i);
				if (metaProperty.isValid() && metaProperty.isWritable()) {
					metaProperty.write(d_ptr->mWidget,modelItem->contentWidgetData(metaProperty.name()));
				}

			}
		}
	}
}

