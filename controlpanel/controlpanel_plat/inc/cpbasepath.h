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

#ifndef CPBASEPATH_H
#define CPBASEPATH_H

#include <QLatin1String>

#ifdef Q_OS_SYMBIAN
        #define CP_PLUGIN_PATH QLatin1String("/resource/qt/plugins/controlpanel")
        #define CP_PLUGIN_CONFIG_PATH QLatin1String("/resource/qt/plugins/controlpanel/config")
#else 
    #ifdef _DEBUG
        #define CP_PLUGIN_PATH QLatin1String("/ControlPanel/debug/bin")
        #define CP_PLUGIN_CONFIG_PATH QLatin1String("/ControlPanel/debug/bin/config")
    #else
        #define CP_PLUGIN_PATH QLatin1String("/ControlPanel/release/bin")
        #define CP_PLUGIN_CONFIG_PATH QLatin1String("/ControlPanel/release/bin/config")
    #endif
#endif 

#ifdef Q_OS_SYMBIAN
    #ifdef __WINS__
        #define CP_RESOURCE_PATH QLatin1String("C:/resource/controlpanel")
    #else
        #define CP_RESOURCE_PATH QLatin1String("Z:/resource/controlpanel")
    #endif
#else 
    #ifdef _DEBUG
        #define CP_RESOURCE_PATH QLatin1String("C:/controlpanel/debug/resource")
    #else
        #define CP_RESOURCE_PATH QLatin1String("C:/controlpanel/release/resource")
    #endif
#endif 

#define WIDGETML_SUB_PATH QLatin1String("widgetml")
#define ICON_SUB_PATH     QLatin1String("icon")

#endif  //CPBASEPATH_H

//End of File

