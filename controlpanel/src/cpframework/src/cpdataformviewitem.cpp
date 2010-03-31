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
#include <QMetaProperty>
/*!
    \class CpDataFormViewItem
    \brief The CpDataFormViewItem is a item proto type of HbDataForm which is supplied by control panel.
	When you want use setting items which are come from control panel, you should append this class to the proto type list of your data form's instance.
	Code example:
	\code
	HbDataForm *form = new HbDataForm();
	QList<HbAbstractViewItem *> protoType = form->itemProtoTypetypes();
	protoType.append(CpDataFormViewItem::createCpItemProtoType());
	form->setItemPrototypes();
	\endcode
*/
class CpDataFormViewItemPrivate 
{
public:
	CpDataFormViewItemPrivate() : mWidget(0)
	{
	}
	~CpDataFormViewItemPrivate()
	{
	}
private:
	HbWidget *mWidget;
	friend class CpDataFormViewItem;
};

/*!
    Constructor
 */
CpDataFormViewItem::CpDataFormViewItem(QGraphicsItem *parent)
: HbDataFormViewItem(parent),d_ptr(new CpDataFormViewItemPrivate())
{
}

/*!
	Destructor of CpDataFormViewItem
 */
CpDataFormViewItem::~CpDataFormViewItem()
{
	delete d_ptr;
}

/*!
	Creates CpDataFormViewItem. This function is called form HbAbstractItemContainer when model is getting parsed for creating items.
 */
HbAbstractViewItem *CpDataFormViewItem::createItem()
{
	return new CpDataFormViewItem(*this);
}

/*!
	Copy constructor of CpDataFormViewItem
 */
CpDataFormViewItem::CpDataFormViewItem(const CpDataFormViewItem &other)
: HbDataFormViewItem(other), d_ptr(new CpDataFormViewItemPrivate(*other.d_ptr))
{
	
}

/*!
	Assignment operator
 */
CpDataFormViewItem &CpDataFormViewItem::operator=(const CpDataFormViewItem &other)
{
	if (&other == this) {
		return *this;
	}

	*d_ptr = *(other.d_ptr);
	return *this;
}
/*!
	Inherit from HbAbstractViewItem, return true if the model item can be supported by CpDataFormViewItem.
 */
bool CpDataFormViewItem::canSetModelIndex(const QModelIndex &index) const
{
	int itemTypeId = index.data(HbDataFormModelItem::ItemTypeRole).toInt();
	return (itemTypeId == HbDataFormModelItem::CustomItemBase+1);
}
/*!
	Inherit from HbDataFormViewItem, return the setting item's widget of control panel
 */
HbWidget *CpDataFormViewItem::createCustomWidget()
{
    HbDataFormModelItem::DataItemType itemType = static_cast<HbDataFormModelItem::DataItemType>(
        modelIndex().data(HbDataFormModelItem::ItemTypeRole).toInt());

	if (itemType == HbDataFormModelItem::CustomItemBase+1) {
		HbPushButton *button = new HbPushButton(QString("Push button"));
		d_ptr->mWidget = button;
		button->setMinimumHeight(50);
		button->setMaximumHeight(60);
		button->setOrientation(Qt::Horizontal);
		button->setTextAlignment( Qt::AlignLeft );
		return button;
	}
    
    return 0;
}
/*!
	Inherit from HbDataForm. This function is called by hbdataform's framework, 
	for supporting to load entry item's text, icon and additional text dynamically.
	It is not recommanded to call this function mannually.
 */
void CpDataFormViewItem::load()
{
	HbDataFormViewItem::load();

	if (d_ptr->mWidget) {
		HbDataFormModelItem::DataItemType itemType = static_cast<HbDataFormModelItem::DataItemType>(
			modelIndex().data(HbDataFormModelItem::ItemTypeRole).toInt());

		if(itemType == HbDataFormModelItem::CustomItemBase+1) {

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

