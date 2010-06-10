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
#ifndef CPLOGGER_H
#define CPLOGGER_H


/* 
    Windows: C:/ControlPanel/debug/bin/controlpanellog.conf
    Symbian: C:/data/.config/controlpanellog.conf

    Format:

    [CpFramework]
    logdatetime = 1
    logloggername = 1
    datetimeformat = hh:mm:ss:zzz
    output = debugoutput fileoutput
    fileoutput/logfile = C:/data/logs/cpframework.log
    fileoutput/truncate = 1
    
    [CpPerformance]
    logdatetime = 1
    datetimeformat = hh:mm:ss:zzz
    output = fileoutput
    fileoutput/logfile = C:/data/logs/cpperformance.log
    fileoutput/truncate = 1
*/

#include <QLatin1String>
#include <logger.h>

#define CPFW_LOGGER_NAME       QLatin1String("CpFramework")
#define CPPERF_LOGGER_NAME     QLatin1String("CpPerformance")

#if defined (Q_OS_SYMBIAN)
    #define CP_LOGGER_CONFIG_PATH QLatin1String("C:/data/.config/controlpanellog.conf")
#elif defined (Q_WS_WIN)
    #ifdef _DEBUG
        #define CP_LOGGER_CONFIG_PATH QLatin1String("C:/controlpanel/debug/bin/controlpanellog.conf")
    #else 
        #define CP_LOGGER_CONFIG_PATH QLatin1String("C:/controlpanel/release/bin/controlpanellog.conf")
    #endif
#endif

#define CPFW_LOG(str)   Logger::instance(CPFW_LOGGER_NAME)->log(str);
#define CPPERF_LOG(str) Logger::instance(CPPERF_LOGGER_NAME)->log(str);

#endif /* CPLOGGER_H */