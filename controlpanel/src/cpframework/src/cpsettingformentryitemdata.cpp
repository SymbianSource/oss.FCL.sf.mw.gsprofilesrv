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
#include "cpsettingformentryitemdata.h"
#include "cpviewlauncher.h"
#include <cpbasesettingview.h>
#include <hbpushbutton.h>
#include <hbdataformviewitem.h>
#include <hbdataform.h>
#include <cpitemdatahelper.h>


/*!
    \class CpSettingFormEntryItemData
    \brief The CpSettingFormEntryItemData can be used to control panel plugin which appears as a setting item. And after clicking
    it, a new setting view will open.
 */

/*
    \fn CpSettingFormEntryItemData::createSettingView
    
    Create a new setting view.
    
    createSettingView will be called automatically when user click a setting item represented by a CpSettingFormEntryItemData.
    
    In most cases, you do not need to write a derived class from CpSettingFormEntryItemData, instead, you can simply use template class CpSettingFormEntryItemDataImpl.
*/


/*!
    Construct a new CpSettingFormEntryItemData with the given parent.
*/
CpSettingFormEntryItemData::CpSettingFormEntryItemData(const HbDataFormModelItem *parent /* = 0*/)
: CpSettingFormItemData(parent)
{
}


/*!
    Construct a new CpSettingFormEntryItemData with the given text,description and parent.
*/

CpSettingFormEntryItemData::CpSettingFormEntryItemData(CpItemDataHelper &itemDataHelper,
		const QString &text /* = QString()*/,
        const QString &description /* = QString()*/,
		const HbIcon &icon,
        const HbDataFormModelItem *parent /*= 0*/) : 
        CpSettingFormItemData(HbDataFormModelItem::CustomItemBase,QString(),parent)
{
	itemDataHelper.addConnection(this,SIGNAL(pressed()),this,SLOT(onLaunchView()));
	setType ( static_cast<HbDataFormModelItem::DataItemType> (CpSettingFormEntryItemData::EntryItem) );
    setContentWidgetData(QString("text"),QVariant(text));
	setContentWidgetData(QString("additionalText"),QVariant(description));
	setContentWidgetData(QString("icon"),QVariant(icon));
}

/*!
    Construct a new CpSettingFormEntryItemData with the given text,description and parent.
*/

CpSettingFormEntryItemData::CpSettingFormEntryItemData(HbDataForm *dataForm,
			const QString &text /*= QString()*/,
	        const QString &description /*= QString()*/,
			const HbIcon &icon /*= HbIcon()*/,
			const HbDataFormModelItem *parent/* = 0*/) :
			CpSettingFormItemData(HbDataFormModelItem::CustomItemBase,QString(),parent)
{
	if (dataForm) {
		dataForm->addConnection(this,SIGNAL(pressed()),this,SLOT(onLaunchView()));
	}
	setType ( static_cast<HbDataFormModelItem::DataItemType> (CpSettingFormEntryItemData::EntryItem) );
    setContentWidgetData(QString("text"),QVariant(text));
	setContentWidgetData(QString("additionalText"),QVariant(description));
	setContentWidgetData(QString("icon"),QVariant(icon));
}


/*!
     Destructor.
*/
CpSettingFormEntryItemData::~CpSettingFormEntryItemData()
{
}


/*!
     Get the text string of the entry item.
*/
QString CpSettingFormEntryItemData::text() const
{
    return contentWidgetData(QString("text")).toString();
}

/*!
     Set the text string of the entry item.
*/
void CpSettingFormEntryItemData::setText(const QString &text)
{
    setContentWidgetData(QString("text"),QVariant(text));
}

/*!
     Get the description string of the entry item.
*/
QString CpSettingFormEntryItemData::description() const
{
    return contentWidgetData(QString("additionalText")).toString();
}

/*!
     Set the description string of the entry item.
*/
void CpSettingFormEntryItemData::setDescription(const QString &description)
{
    setContentWidgetData(QString("additionalText"),QVariant(description));
}

/*!
	Return the icon of the entry item
*/
HbIcon CpSettingFormEntryItemData::entryItemIcon()const
{
	return qvariant_cast<HbIcon>(contentWidgetData(QString("icon")));
}

/*!
	Set the icon for the entry item
*/
void CpSettingFormEntryItemData::setEntryItemIcon(const HbIcon& icon)
{
	setContentWidgetData(QString("icon"),icon);
}

void CpSettingFormEntryItemData::onLaunchView() 
{
	if (CpBaseSettingView *view = createSettingView()) {
		CpViewLauncher::launchView(view);
	}
}
