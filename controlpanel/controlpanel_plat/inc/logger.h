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

#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QSettings>
#include <loggerglobal.h>

class LogOutput;
class QSettings;
class LoggerPrivate;

/*

configuration format:

    [myapplog]
    logDateTime = 1
    logLoggerName = 1
    output = debugoutput consoleoutput fileoutput
    fileoutput/logfile = C:/data/log/myapp.log
    fileoutput/truncate = 1

code examples:

    QSettings settings("myapp.ini",QSettings::IniFormat);
    Logger::instance("myapplog")->configure(settings);
    Logger::instance("myapplog")->log("Hello world!");

*/

class LOGGER_EXPORT Logger : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool logDateTime READ logDateTime WRITE setLogDateTime)
    Q_PROPERTY(bool logLoggerName READ logLoggerName WRITE setLogLoggerName)
    Q_PROPERTY(QString dateTimeFormat READ dateTimeFormat WRITE setDateTimeFormat)
public:
    static Logger *instance(const QString &name);	
    static void close(const QString &name);
    static void closeAll();
public:
    virtual ~Logger();

    void log(const QString &log);

    void configure(const QString &configFile,QSettings::Format format = QSettings::NativeFormat);

    void configure(QSettings &settings);

    bool addLogOutput(LogOutput *output);

    void removeLogOutput(LogOutput *output);

    LogOutput *logOutput(const QString &name);

    void clearAllLogOutput();

    QString name() const;

    bool logDateTime() const;
    void setLogDateTime(bool on);

    bool logLoggerName() const;
    void setLogLoggerName(bool on);

    QString dateTimeFormat() const;
    void setDateTimeFormat(const QString &format);
private:
    explicit Logger(const QString &name = QString(),QObject *parent = 0);
    LoggerPrivate *d_ptr;
};

#endif //LOGGER_H
