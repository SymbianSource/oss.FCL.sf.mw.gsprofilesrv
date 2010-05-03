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
#ifndef CPDATAFORMBUTTONENTRYVIEWITEM_H
#define CPDATAFORMBUTTONENTRYVIEWITEM_H

#include <hbdataformviewitem.h>

class CpDataFormButtonEntryViewItemPrivate;

class CpDataFormButtonEntryViewItem : public HbDataFormViewItem
{
    Q_OBJECT
public:
    explicit CpDataFormButtonEntryViewItem(QGraphicsItem *parent = 0);
    virtual ~CpDataFormButtonEntryViewItem();
    
    virtual HbAbstractViewItem *createItem();
	virtual bool canSetModelIndex(const QModelIndex &index) const;
public slots:
    virtual void load();
protected:
	CpDataFormButtonEntryViewItem(const CpDataFormButtonEntryViewItem &ohter);
	CpDataFormButtonEntryViewItem &operator = (const CpDataFormButtonEntryViewItem &ohter);
    virtual HbWidget *createCustomWidget();
private:
	CpDataFormButtonEntryViewItemPrivate *d_ptr;
};

#endif // CPDATAFORMBUTTONENTRYVIEWITEM_H

//End of File
