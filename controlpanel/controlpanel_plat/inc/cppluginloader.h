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

#ifndef CPPLUGINLOADER_H
#define CPPLUGINLOADER_H

#include <cpglobal.h>
#include <QList>

class QString;
class CpPluginInterface;
class CpLauncherInterface;
class CpPluginPlatInterface;

class CP_EXPORT CpPluginLoader
{
public:
    /* OLD INTERFACES* DPRECATED!!! */
    static CpPluginInterface     *loadCpPlugin(const QString &pluginFile);
    /* OLD INTERFACES* DPRECATED!!! */
    static CpPluginPlatInterface *loadPlatCpPlugin(const QString &pluginFile);
    
    /*NEW INTERFACES*/
    static CpPluginInterface     *loadCpPluginInterface(const QString &pluginFile);
    static CpLauncherInterface   *loadCpLauncherInterface(const QString &pluginFile);
};

#endif /* CPPLUGINLOADER_H */
