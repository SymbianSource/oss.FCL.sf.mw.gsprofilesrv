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
class CpPluginPlatInterface;

class CP_EXPORT CpPluginLoader
{
public:
    static CpPluginInterface *loadCpPlugin(const QString &pluginFile);
    static CpPluginPlatInterface *loadPlatCpPlugin(const QString &pluginFile);
};

#endif /* CPPLUGINLOADER_H */
