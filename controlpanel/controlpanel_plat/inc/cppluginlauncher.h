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
* Description:  This class is for loading and display a plugin's view.
*								This class may dropped, suggest not using it. If you 
*								want to use it, pls inform us, or check the wiki 
*								http://s60wiki.nokia.com/S60Wiki/Team_S60_China_SW/Qt_Control_Panel/FAQ
*
*/

#ifndef CPPLUGINLAUNCHER_H
#define CPPLUGINLAUNCHER_H

#include "cpglobal.h"
#include <QVariant>

class QString;
class CpBaseSettingView;

class CP_EXPORT CpPluginLauncher
{
public:
	/*  OLD Interface - Deprecated! */
    static bool launchCpPluginView(const QString &pluginFile,int index = 0);
    
    /*  */
	static CpBaseSettingView* launchSettingView(const QString &pluginFile,const QVariant &hint = QVariant());
};

#endif /* CPPLUGINLAUNCHER_H */
