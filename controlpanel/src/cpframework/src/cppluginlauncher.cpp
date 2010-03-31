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

#include <cppluginlauncher.h>
#include <cpplugininterface.h>
#include <cppluginplatinterface.h>
#include <cpsettingformentryitemdata.h>
#include <QSharedPointer>
#include <cppluginloader.h>
#include "cpitemdatahelper.h"

/*!
    \class CpPluginLauncher
    \brief The CpPluginLauncher class loads a controlpanel plugin at run-time.And display the specify plugin view in client process.
 */

static CpSettingFormEntryItemData *createEntrySettingItem(const QString &pluginFile,int index = 0) 
{
	{
		CpPluginInterface *plugin = CpPluginLoader().loadCpPlugin(pluginFile);
		if (plugin) {
			CpItemDataHelper dummy(0);
			QList<CpSettingFormItemData*> allItemData = plugin->createSettingFormItemData(dummy);

            CpSettingFormItemData *found = 0;

            int size = allItemData.size();
            for (int i = 0; i < size; ++i) {
                if (i == index) {
                    found = allItemData.value(i);
                }
                else {
                    delete allItemData.value(i);
                }
            }
            
            if (found) {
                CpSettingFormEntryItemData *entryItemData = qobject_cast<CpSettingFormEntryItemData*>(found);
                if (entryItemData) {
                    return entryItemData;
                }
                else {
                    delete found;
                }
            }
		}
	}


	//CpPluginPlatInterface : deprecated!
	{
        CpPluginPlatInterface *plugin = CpPluginLoader().loadPlatCpPlugin(pluginFile);
        if (plugin) {
            CpItemDataHelper dummy(0);
            CpSettingFormItemData *itemData = plugin->createSettingFormItemData(dummy);
            CpSettingFormEntryItemData *entryItemData = qobject_cast<CpSettingFormEntryItemData*>(itemData);
            if (entryItemData) {
                return entryItemData;
            }
            else {
                delete itemData;
            }
		}
	}
    
    return 0;
}

/*!
    Load and display a plugin view in client process. The client must be a orbit based application.
    The pluginFile can either absoulte file path or only file name. 
    Acceptable format:
        sampleplugin
        sampleplugin.qtplugin
        sampleplugin.dll
        C:/resource/qt/plugins/controlpanel/sampleplugin.qtplugin
        C:/resource/qt/plugins/controlpanel/sampleplugin.dll
*/
bool CpPluginLauncher::launchCpPluginView(const QString &pluginFile,int index /*= 0*/)
{
    QSharedPointer<CpSettingFormEntryItemData> entryItemData(createEntrySettingItem(pluginFile,index));
    if (!entryItemData.isNull()) {
        entryItemData->onLaunchView();
        return true;            
    }
    return false;
}

