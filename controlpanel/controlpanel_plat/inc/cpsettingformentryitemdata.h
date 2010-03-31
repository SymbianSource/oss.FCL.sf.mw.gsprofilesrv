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

#ifndef CP_SETTINGFORM_ENTRY_ITEM_DATA_H
#define CP_SETTINGFORM_ENTRY_ITEM_DATA_H

#include <cpglobal.h>
#include <qobject>
#include <cpsettingformitemdata.h>
#include <hbicon.h>

class CpItemDataHelper;
class CpBaseSettingView;
class HbDataForm;

class CP_EXPORT CpSettingFormEntryItemData : public CpSettingFormItemData 
{
	Q_OBJECT
public:
	enum ItemType{ EntryItem = HbDataFormModelItem::CustomItemBase + 1};

	explicit CpSettingFormEntryItemData(const HbDataFormModelItem *parent = 0);

	explicit CpSettingFormEntryItemData(CpItemDataHelper &itemDataHelper,
			const QString &text = QString(),
	        const QString &description = QString(),
			const HbIcon &icon = HbIcon(),
			const HbDataFormModelItem *parent = 0);	 

	explicit CpSettingFormEntryItemData(HbDataForm *dataForm,
			const QString &text = QString(),
	        const QString &description = QString(),
			const HbIcon &icon = HbIcon(),
			const HbDataFormModelItem *parent = 0);	

	virtual ~CpSettingFormEntryItemData();	
	
	QString text() const;
	void setText(const QString &text);
	
	QString description() const;
	void setDescription(const QString &description);

	HbIcon entryItemIcon() const;
	void setEntryItemIcon(const HbIcon &icon);

private slots:
	void onLaunchView();
private:
	virtual CpBaseSettingView *createSettingView() const = 0;
	friend class CpPluginLauncher;
};

#endif
