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
#include "cppluginloader.h"
#include <qstring>
#include <qdebug>
#include <qfileinfo>
#include <qpluginloader>
#include <cppluginplatinterface.h>
#include "cpbasepath.h"
#include "cplogger.h"
#include "cputility.h"


#ifdef WIN32
    #define PLUGINFILE_SUFFIX "dll"
#else
    #define PLUGINFILE_SUFFIX "qtplugin"
#endif

CpPluginPlatInterface *CpPluginLoader::loadCpPlugin(const QString &pluginFile)
{
    CpLogger logger;
    logger << "loading plugin:" << pluginFile << "\r\n";
    
    QFileInfo fileInfo(pluginFile);

    if (!fileInfo.isAbsolute()) {
        QString fileName = fileInfo.fileName();
        if (fileInfo.suffix().compare(PLUGINFILE_SUFFIX,Qt::CaseInsensitive)) {
            fileName = fileInfo.baseName() + '.' + PLUGINFILE_SUFFIX;
        }

		QStringList pluginDirs = CpUtility::pluginDirs();
		foreach(const QString &pluginDir,pluginDirs) {
			fileInfo.setFile(pluginDir + fileName);
			if (fileInfo.exists() && QLibrary::isLibrary(fileInfo.absoluteFilePath())) {
				break;
			}
		}
    }

	QPluginLoader loader(fileInfo.absoluteFilePath());
	CpPluginPlatInterface *plugin = qobject_cast<CpPluginPlatInterface*> (loader.instance());
	if (!plugin) {
		loader.unload();
	}

    logger << (plugin ? "load plugin succeed." : "load plugin failed.") << "\r\n";
    
    return plugin;
}
