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
#ifndef CP_PERSONALIZATION_CUSTOMVIEWITEM_H
#define CP_PERSONALIZATION_CUSTOMVIEWITEM_H

#include <hbdataformviewitem.h>
#include <hbdataformmodelitem.h>

const int MasterVolumeSliderItem = HbDataFormModelItem::CustomItemBase+21; 

class CpPersonalizationCustomViewItem: public HbDataFormViewItem
{
	Q_OBJECT
public:
    explicit CpPersonalizationCustomViewItem(QGraphicsItem *parent = 0);
    ~CpPersonalizationCustomViewItem();
    virtual HbAbstractViewItem* createItem();
	virtual bool canSetModelIndex(const QModelIndex &index) const;
private slots:
	void onValueChanged(int value);
protected:
    virtual HbWidget* createCustomWidget();
};
#endif
