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

#include "cpcategorysettingformmodel.h"
#include <qstring>

#include "cputility.h"
#include "cpitemDatahelper.h"

class CpCategorySettingFormModelPrivate
{
public:
	CpCategorySettingFormModelPrivate(const QString &configFile) : 
	  mInitialized(false),
	  mConfigFile(configFile)
	{
	}

	~CpCategorySettingFormModelPrivate()
	{
	}

public:
	bool mInitialized;
	QString mConfigFile;
};

CpCategorySettingFormModel::CpCategorySettingFormModel(const QString &configFile) : 
	d (new CpCategorySettingFormModelPrivate(configFile))
{
}

CpCategorySettingFormModel::~CpCategorySettingFormModel()
{
	delete d;
}

void CpCategorySettingFormModel::initialize(CpItemDataHelper &itemDataHelper)
{
    //avoid to be called twice
    if (!d->mInitialized) {
        //give derived class a change do their special things before loading config plugins
        beforeLoadingConfigPlugins(itemDataHelper);
        //load plugins which are configured
		CpUtility::buildConfigPluginItems(invisibleRootItem(),d->mConfigFile,itemDataHelper);
        //give derived class a change do their special things after loading config plugins
        afterLoadingConfigPlugins(itemDataHelper);

        d->mInitialized = true;
    }
}


void CpCategorySettingFormModel::beforeLoadingConfigPlugins(CpItemDataHelper&/*itemDataHelper*/)
{
}

void CpCategorySettingFormModel::afterLoadingConfigPlugins(CpItemDataHelper &/*itemDataHelper*/)
{
}
