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

class QString;
class CpPluginPlatInterface;

class CpPluginLoader
{
public:

    /*
     load a cp plugin by plugin file.
     the pluginFile can either absoulte file path or only file name.
     acceptable format:
     sampleplugin
     sampleplugin.qtplugin
     sampleplugin.dll
     C:/resource/qt/plugins/controlpanel/sampleplugin.qtplugin
     C:/resource/qt/plugins/controlpanel/sampleplugin.dll
     */
    static CpPluginPlatInterface *loadCpPlugin(const QString &pluginFile);

};

#endif /* CPPLUGINLOADER_H */
