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

#ifdef HB_EFFECTS
    #include "hbeffect.h"
#endif

#ifdef HB_GESTURE_FW
    #include <hbtapgesture.h>
    #include <hbframeitem.h>
#endif

#include <hbdataformmodel.h>
#include <hbdataformmodelitem.h>
#include <hbabstractitemview.h>
#include <cpsettingformentryitemdata.h>

#define CP_DATAFORMLISTENTRYVIEWITEM QLatin1String("cpdataformlistentryviewitem")

CpDataFormListEntryViewItem::CpDataFormListEntryViewItem(QGraphicsItem *parent) : 
    HbDataFormViewItem(parent)
{
#ifdef HB_EFFECTS       
    HbEffect::add(CP_DATAFORMLISTENTRYVIEWITEM + "-focus", "listviewitem_press", "pressed");
    HbEffect::add(CP_DATAFORMLISTENTRYVIEWITEM + "-focus", "listviewitem_release", "released");
#endif
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
        case CpSettingFormEntryItemData::ListEntryItem: {
            break;
        }
        default: {
            break;
        }
    }
    
    return 0;
}

void CpDataFormListEntryViewItem::pressStateChanged(bool pressed, bool animate)
{
#ifdef HB_EFFECTS
    QGraphicsItem *focusItem =  primitive("dataItem_Background");
    bool doAnimate = animate;

    if (pressed) {
        if (doAnimate) {
            HbEffect::cancel(this, "released");
            HbEffect::cancel(focusItem, "released");

            HbEffect::start(this, CP_DATAFORMLISTENTRYVIEWITEM, "pressed");
            HbEffect::start(focusItem, CP_DATAFORMLISTENTRYVIEWITEM + QString("-focus"), "pressed");
        }
    } else {
        if (doAnimate) {
            HbEffect::cancel(this, "pressed");
            HbEffect::cancel(focusItem, "pressed");

            HbEffect::start(this, CP_DATAFORMLISTENTRYVIEWITEM, "released");
            HbEffect::start(focusItem, CP_DATAFORMLISTENTRYVIEWITEM + QString("-focus"), "released");
        } else {
            HbEffect::cancel(this, "pressed");
            HbEffect::start(this, CP_DATAFORMLISTENTRYVIEWITEM, "released");
            if (focusItem) {
                HbEffect::cancel(focusItem, "pressed");
                HbEffect::start(focusItem, CP_DATAFORMLISTENTRYVIEWITEM + QString("-focus"), "released", this);
            }
        }
    }
#endif
}

#ifdef HB_GESTURE_FW
void CpDataFormListEntryViewItem::gestureEvent(QGestureEvent *event)
{
    HbFrameItem *backgroundItem = static_cast<HbFrameItem*> ( primitive("dataItem_Background") );
    HbTapGesture *tap = qobject_cast<HbTapGesture *>(event->gesture(Qt::TapGesture));
    
    if(backgroundItem && tap) {
        switch(tap->state()) {
        case Qt::GestureStarted: {
                backgroundItem->frameDrawer().setFrameGraphicsName(QLatin1String("qtg_fr_list_pressed"));
            }
            break;
        case Qt::GestureUpdated: {
            
            }
            break;
        case Qt::GestureCanceled: {
                backgroundItem->frameDrawer().setFrameGraphicsName(QLatin1String("qtg_fr_list_normal"));
            }
            break;
        case Qt::GestureFinished: {
                backgroundItem->frameDrawer().setFrameGraphicsName(QLatin1String("qtg_fr_list_normal"));
            }
            break;
        default:
            break;
        }
    }

    HbDataFormViewItem::gestureEvent( event );
}
#endif


//End of File
