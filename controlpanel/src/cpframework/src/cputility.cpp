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
#include "cputility.h"
#include <qstring>
#include <qdir>
#include <qfileinfo>
#include "cpbasepath.h"
#include "cppluginloader.h"
#include <cppluginplatinterface.h>
#include <cpsettingformentryitemdata.h>
#include "cpcategorysettingformitemdata.h"
#include "cppluginconfigreader.h"

void CpUtility::buildConfigPluginItems(HbDataFormModelItem *parent,
									   const QString &configFile,
									   CpItemDataHelper &itemDataHelper)
{
    if (!parent) {
        return;
    }

	QString configPath(configFile);
	QFileInfo fi(configFile);
	if (!fi.isAbsolute()) {
		QStringList dirs = CpUtility::cpcfgDirs();
		foreach(const QString &dir,dirs) {
			configPath = dir + fi.fileName();
			if (QFileInfo(configPath).exists()) {
				break;		
			}
		}
	}
    	
    QList<CpPluginConfig> cpPluginConfigs = CpPluginConfigReader(configPath).readCpPluginConfigs();
		
	foreach(const CpPluginConfig &cpPluginConfig, cpPluginConfigs)  {
		CpPluginPlatInterface *plugin = CpPluginLoader().loadCpPlugin(cpPluginConfig.mPluginFile);

		if (plugin) {
			CpSettingFormItemData *itemData = plugin->createSettingFormItemData(itemDataHelper);
			if (itemData) {
				//append the new created setting form item to its parent item.
				parent->appendChild(itemData);

				if (CpCategorySettingFormItemData *categoryItemData 
					= qobject_cast<CpCategorySettingFormItemData*>(itemData)) {
					categoryItemData->initialize(itemDataHelper);
				}

				//set the description from config if it is empty.
				if (CpSettingFormEntryItemData *cpEntryItemData
					= qobject_cast<CpSettingFormEntryItemData*>(itemData)) {
						if (cpEntryItemData->text().isEmpty()) {
							cpEntryItemData->setText(cpPluginConfig.mDisplayName);
						}
						if (cpEntryItemData->description().isEmpty()) {
							cpEntryItemData->setDescription(cpPluginConfig.mDescription);
						}
				}

			}
		}

	} //end foreach
}


QStringList CpUtility::drives()
{
	static QStringList drives;

	if (drives.empty()) {
#ifdef WIN32
		drives.append("C:");
#else
		QFileInfoList fileInfoList = QDir::drives();
		foreach(const QFileInfo &fileInfo,fileInfoList) {
			QString str = fileInfo.filePath();
			if (str.length() > 2) {
				str = str.left(2);
			}
			drives.append(str);
		}
#endif
	}

	return drives;
}

static QStringList listDirs(const QString &baseDir)
{
	QStringList dirs;

	QStringList drives = CpUtility::drives();
	foreach(const QString &drive,drives) {
		QString dir = drive + baseDir + QDir::separator();
		if (QFileInfo(dir).exists()) {
			dirs.append(dir);
		}
	}

	return dirs;
}

QStringList CpUtility::pluginDirs()
{
	static QStringList dirs;
	if (dirs.empty()) {
		dirs = listDirs(CP_PLUGIN_PATH);
	}
	return dirs;
}

QStringList CpUtility::cpcfgDirs()
{
	static QStringList dirs;
	if (dirs.empty()) {
		dirs = listDirs(CP_PLUGIN_CONFIG_PATH);
	}
	return dirs;
}

