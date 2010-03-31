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
#include "cpcategorysettingformitemdata.h"
#include <qstring>
#include "cputility.h"

class CpCategorySettingFormItemDataPrivate
{
public:
	CpCategorySettingFormItemDataPrivate(const QString &configFile) : 
	  mInitialized(false),
	  mConfigFile(configFile)
	{
	}

	~CpCategorySettingFormItemDataPrivate()
	{
	}

public:
	bool mInitialized;
	QString mConfigFile;
};


CpCategorySettingFormItemData::CpCategorySettingFormItemData(
               HbDataFormModelItem::DataItemType type,
	           const QString &label,
			   const QString &configFile,
			   const HbDataFormModelItem *parent /*= 0*/) : 
			   CpSettingFormItemData(type,label,parent),
			   d(new CpCategorySettingFormItemDataPrivate(configFile))
{
}

CpCategorySettingFormItemData::CpCategorySettingFormItemData(
			   const QString &configFile /*= QString()*/,
			   const HbDataFormModelItem *parent /*= 0*/) : 
	           CpSettingFormItemData(parent),
			   d(new CpCategorySettingFormItemDataPrivate(configFile))
{
}

CpCategorySettingFormItemData::~CpCategorySettingFormItemData()
{
	delete d;
}

void CpCategorySettingFormItemData::initialize(CpItemDataHelper &itemDataHelper)
{
    //avoid to be called twice
    if (!d->mInitialized) {
        //give derived class a change do their special things before loading config plugins
        beforeLoadingConfigPlugins(itemDataHelper);
        //load plugins which are configured
		CpUtility::buildConfigPluginItems(this,d->mConfigFile,itemDataHelper);
        //give derived class a change do their special things after loading config plugins
        afterLoadingConfigPlugins(itemDataHelper);

        d->mInitialized = true;
    }
}


void CpCategorySettingFormItemData::beforeLoadingConfigPlugins(CpItemDataHelper &/*itemDataHelper*/)
{
}

void CpCategorySettingFormItemData::afterLoadingConfigPlugins(CpItemDataHelper &/*itemDataHelper*/)
{
}

