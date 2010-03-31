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

#include <qdebug>
#include <qfile>
#include <qsettings>
class QFile;

class CpLogger 
{
    enum { NoLog = 0, StdTraces, LogToFile };
public:
    CpLogger() : mDebug(0), mLogFile(0)
    {
        QSettings settings;
        int logMethod = settings.value("Log/logmethod",NoLog).toInt();
        switch (logMethod)
        {
        default:
        case NoLog:
            break;
        case StdTraces:
            mDebug = new QDebug(QtDebugMsg);
            break;
        case LogToFile:
            //TBD: hardcode path will be replaced by some other form 
            mLogFile = new QFile("C:\\data\\logs\\controlpanel.log");
            mLogFile->open(QFile::WriteOnly | QFile::Append);
            mDebug = new QDebug(mLogFile);
            break;
        }
    }

    ~CpLogger()
    {
        delete mDebug;
        delete mLogFile;
    }
    
    template<typename T>
    inline CpLogger &operator << (const T &log) 
    {
        if (mDebug)  
        {
            (*mDebug) << log;
        }
        return *this;
    }
    
private:
    Q_DISABLE_COPY(CpLogger)
    QDebug *mDebug;
    QFile  *mLogFile;
};

#endif /* CPLOGGER_H */
