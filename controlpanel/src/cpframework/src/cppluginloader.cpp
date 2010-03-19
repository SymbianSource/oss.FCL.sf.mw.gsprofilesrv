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
#include <cppluginloader.h>
#include <QString>
#include <QDir>
#include <QFileInfo>
#include <QPluginLoader>
#include <cpplugininterface.h>
#include <cppluginplatinterface.h>
#include "cpbasepath.h"
#include "cputility.h"
#include <cplogger.h>


/*!
    \class CpPluginLoader
    \brief The CpPluginLoader class loads a controlpanel plugin at run-time.
 */

#ifdef WIN32
    #define PLUGINFILE_SUFFIX "dll"
#else
    #define PLUGINFILE_SUFFIX "qtplugin"
#endif

template <typename PLUGIN>
static PLUGIN* loadPlugin(const QString &pluginFile)
{
    QFileInfo fileInfo(pluginFile);

    if (!fileInfo.isAbsolute()) {
        QString fileName = fileInfo.fileName();
        if (fileInfo.suffix().compare(PLUGINFILE_SUFFIX,Qt::CaseInsensitive)) {
            fileName = fileInfo.baseName() + '.' + PLUGINFILE_SUFFIX;
        }

		QStringList pluginDirs = CpUtility::pluginDirectories();
		foreach(const QString &pluginDir,pluginDirs) {
			fileInfo.setFile(pluginDir + fileName);
			if (fileInfo.exists() && QLibrary::isLibrary(fileInfo.absoluteFilePath())) {
				break;
			}
		}
    }

	QPluginLoader loader(fileInfo.absoluteFilePath());
	PLUGIN *plugin = qobject_cast<PLUGIN*> (loader.instance());
	if (!plugin) {
		loader.unload();
	}
    
    return plugin;
}

/*!
    load a controlpanel plugin by plugin file.
    the plugin file can either absoulte file path or only file name.
    acceptable format:
        sampleplugin
        sampleplugin.qtplugin
        sampleplugin.dll
        C:/resource/qt/plugins/controlpanel/sampleplugin.qtplugin
        C:/resource/qt/plugins/controlpanel/sampleplugin.dll
 */

CpPluginInterface *CpPluginLoader::loadCpPlugin(const QString &pluginFile)
{
	return loadPlugin<CpPluginInterface>(pluginFile);
}


/*!
    \deprecated  CpPluginPlatInterface *CpPluginLoader::loadPlatCpPlugin(const QString &) is deprecated.
    please use CpPluginInterface to implement controlpanel plugin and use CpPluginLoader::loadCpPlugin(const QString &) to load the plugin.
 */

CpPluginPlatInterface *CpPluginLoader::loadPlatCpPlugin(const QString &pluginFile)
{    
	return loadPlugin<CpPluginPlatInterface>(pluginFile);
}


