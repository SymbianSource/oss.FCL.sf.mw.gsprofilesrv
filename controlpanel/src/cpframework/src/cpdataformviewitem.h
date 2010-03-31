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
#ifndef CP_DATAFORMVIEWITEM_H
#define CP_DATAFORMVIEWITEM_H

#include <hbdataformviewitem.h>
#include <cpglobal.h>
class CpDataFormViewItemPrivate;
class CP_EXPORT CpDataFormViewItem : public HbDataFormViewItem
{
    Q_OBJECT
public:
    explicit CpDataFormViewItem(QGraphicsItem *parent = 0);
    ~CpDataFormViewItem();
    virtual HbAbstractViewItem *createItem();
	virtual bool canSetModelIndex(const QModelIndex &index) const;
public slots:
    virtual void load();
protected:
	CpDataFormViewItem(const CpDataFormViewItem &ohter);
	CpDataFormViewItem &operator=(const CpDataFormViewItem &ohter);
    virtual HbWidget *createCustomWidget();
private:
	CpDataFormViewItemPrivate *d_ptr;
	friend class CpDataFormViewItemPrivate;
};


#endif // CP_DATAFORMVIEWITEM_H
