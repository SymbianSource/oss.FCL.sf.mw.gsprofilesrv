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
#include <cppluginplatinterface.h>
#include <cpsettingformentryitemdata.h>
#include <qsharedpointer>
#include "cppluginloader.h"
#include "cpitemdatahelper.h"

static CpSettingFormEntryItemData *createEntrySettingItem(const QString &pluginFile) 
{
    CpPluginPlatInterface *plugin = CpPluginLoader().loadCpPlugin(pluginFile);
    if (plugin) {
        CpItemDataHelper dummy;
        CpSettingFormItemData *itemData = plugin->createSettingFormItemData(dummy);
        return qobject_cast<CpSettingFormEntryItemData*>(itemData);
    }
    
    return 0;
}

/*!
    Load and display a plugin with the plugin's related file.
    The pluginFile can either absoulte file path or only file name. Acceptable format:
      sampleplugin
      sampleplugin.qtplugin
      sampleplugin.dll
      C:/resource/qt/plugins/controlpanel/sampleplugin.qtplugin
      C:/resource/qt/plugins/controlpanel/sampleplugin.dll
*/
bool CpPluginLauncher::launchCpPluginView(const QString &pluginFile)
{
    QSharedPointer<CpSettingFormEntryItemData> entryItemData(createEntrySettingItem(pluginFile));
    if (!entryItemData.isNull()) {
        entryItemData->onLaunchView();
        return true;            
    }
    return false;
}

