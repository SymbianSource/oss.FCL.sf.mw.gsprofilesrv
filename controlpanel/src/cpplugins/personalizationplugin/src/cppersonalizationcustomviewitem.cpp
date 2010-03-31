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

#include "cppersonalizationcustomviewitem.h"
#include "cpmastervolumeslider.h"
#include <hbdataformmodelitem.h>
#include <hbdataformmodel.h>
#include <hbabstractitemview.h>

CpPersonalizationCustomViewItem::CpPersonalizationCustomViewItem(QGraphicsItem *parent )
																: HbDataFormViewItem(parent)
{
}
CpPersonalizationCustomViewItem::~CpPersonalizationCustomViewItem()
{
}
HbAbstractViewItem* CpPersonalizationCustomViewItem::createItem()
{
	return new CpPersonalizationCustomViewItem(*this);
}
bool CpPersonalizationCustomViewItem::canSetModelIndex(const QModelIndex &index) const
{
	int type = index.data(HbDataFormModelItem::ItemTypeRole).toInt();
	if (type == MasterVolumeSliderItem) {
		return true;
	}
	else {
		return false;
	}
}

HbWidget *CpPersonalizationCustomViewItem::createCustomWidget()
{
    int type = modelIndex().data(HbDataFormModelItem::ItemTypeRole).toInt();
	if (type == MasterVolumeSliderItem) {
		CpMasterVolumeSlider *masterVolumeSlider = new CpMasterVolumeSlider();
		connect(masterVolumeSlider, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));
		return masterVolumeSlider;
	}
	else {
		return 0;
	}
}
void CpPersonalizationCustomViewItem::onValueChanged(int value)
{
	HbDataFormModelItem::DataItemType itemType = static_cast<HbDataFormModelItem::DataItemType>(
									modelIndex().data(HbDataFormModelItem::ItemTypeRole).toInt());

	if(itemType == MasterVolumeSliderItem) {

		QModelIndex itemIndex = modelIndex();
		HbDataFormModel *model = static_cast<HbDataFormModel*>(itemView()->model());;
		HbDataFormModelItem *modelItem = static_cast<HbDataFormModelItem*>(
			model->itemFromIndex(itemIndex));
		modelItem->setContentWidgetData("value",value);
	}
}
