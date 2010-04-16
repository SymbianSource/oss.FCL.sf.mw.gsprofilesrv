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

#include "cpdataformlistentryviewitem.h"
#include <hbdataformmodel.h>
#include <hbdataformmodelitem.h>
#include <hbabstractitemview.h>
#include <cpsettingformentryitemdata.h>

CpDataFormListEntryViewItem::CpDataFormListEntryViewItem(QGraphicsItem *parent) : 
    HbDataFormViewItem(parent)
{
}

CpDataFormListEntryViewItem::~CpDataFormListEntryViewItem()
{
}

HbAbstractViewItem* CpDataFormListEntryViewItem::createItem()
{
    return new CpDataFormListEntryViewItem(*this);
}

bool CpDataFormListEntryViewItem::canSetModelIndex(const QModelIndex &index) const
{
    HbDataFormModelItem::DataItemType itemType = 
        static_cast<HbDataFormModelItem::DataItemType>( 
                index.data(HbDataFormModelItem::ItemTypeRole).toInt());

    return ( itemType == CpSettingFormEntryItemData::ListEntryItem );
}

HbWidget* CpDataFormListEntryViewItem::createCustomWidget()
{
    HbDataFormModelItem::DataItemType itemType =
            static_cast<HbDataFormModelItem::DataItemType> (modelIndex().data(
                    HbDataFormModelItem::ItemTypeRole).toInt());
    switch (itemType)
    {
        case CpSettingFormEntryItemData::ListEntryItem:
        {
            break;
        }
        default:
        {
            break;
        }
    }
    
    return 0;
}

//End of File
