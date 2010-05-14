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
#include <cplauncherinterface.h>
#include <cpbasepath.h>
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

template <typename INTERFACE>
static INTERFACE* loadPluginInterface(const QString &pluginFile)
{
    CPPERF_LOG( QLatin1String("Loading plugin: ") + pluginFile );
    
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
			    CPPERF_LOG( QLatin1String("Valid plugin stub found: ") + fileInfo.absoluteFilePath() );
				break;
			}
		}
    }

	QPluginLoader loader(fileInfo.absoluteFilePath());
	INTERFACE *plugin = qobject_cast<INTERFACE*> (loader.instance());
	if (!plugin) {
		loader.unload();
	}
	
	CPPERF_LOG( QLatin1String("Load plugin ") + (plugin ? QLatin1String("succeed.") : QLatin1String("failed.")) );
    
    return plugin;
}

/*!
    load a CpPluginInterface by plugin file.
    the plugin file can either absoulte file path or only file name.
    acceptable format:
        sampleplugin
        sampleplugin.qtplugin
        sampleplugin.dll
        C:/resource/qt/plugins/controlpanel/sampleplugin.qtplugin
        C:/resource/qt/plugins/controlpanel/sampleplugin.dll
 */
CpPluginInterface *CpPluginLoader::loadCpPluginInterface(const QString &pluginFile)
{
    return ::loadPluginInterface<CpPluginInterface>(pluginFile);
}

/*!
    load a CpLauncherInterface by plugin file.
    the plugin file can either absoulte file path or only file name.
    acceptable format:
        sampleplugin
        sampleplugin.qtplugin
        sampleplugin.dll
        C:/resource/qt/plugins/controlpanel/sampleplugin.qtplugin
        C:/resource/qt/plugins/controlpanel/sampleplugin.dll
 */
CpLauncherInterface *CpPluginLoader::loadCpLauncherInterface(const QString &pluginFile)
{
    return ::loadPluginInterface<CpLauncherInterface>(pluginFile);    
}


