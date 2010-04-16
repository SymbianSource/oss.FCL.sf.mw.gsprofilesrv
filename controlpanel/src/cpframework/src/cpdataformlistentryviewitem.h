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


#ifndef CPDATAFORMLISTENTRYVIEWITEM_H
#define CPDATAFORMLISTENTRYVIEWITEM_H

#include <hbdataformviewitem.h>

class CpDataFormListEntryViewItem : public HbDataFormViewItem
{
    Q_OBJECT
public:
    explicit CpDataFormListEntryViewItem(QGraphicsItem *parent = 0);
    virtual ~CpDataFormListEntryViewItem();
    virtual HbAbstractViewItem* createItem();
    virtual bool canSetModelIndex(const QModelIndex &index) const;
protected:
    virtual HbWidget* createCustomWidget();
};

#endif //CPDATAFORMLISTENTRYVIEWITEM_H

//End of File
