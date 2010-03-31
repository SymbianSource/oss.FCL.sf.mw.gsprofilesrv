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

#ifndef CPPROFILEMODEL_P_H
#define CPPROFILEMODEL_P_H

#include <qglobal.h>

class MProfileEngineExtended;
class MProfileExtended;
class MProfileFeedbackSettings;
class MProfileSetFeedbackSettings;
class CRepository;
struct TProfileToneSettings;


class CpProfileModelPrivate
    {
public:
    CpProfileModelPrivate();
    ~CpProfileModelPrivate();

public:
    int initiationFlag();
    QString ringTone();
    void setRingTone(const QString& filePath);
    QString messageTone();
    void setMessageTone(const QString& filePath);
    QString emailTone();
    void setEmailTone(const QString& filePath);
    QString calendarTone();
    void setCalendarTone(const QString& filePath);
    QString alarmTone();
    void setAlarmTone(const QString& filePath);
    int ringVolume();
    void setRingVolume(int volume);
    void activateBeep();
    bool isBeep();
    void activateSilent();
    bool isSilent();
    bool vibraStatus();
    void setVibraStatus(bool status);
    int keyVolume();
    void setKeyVolume(int volume);
    int screenVolume();
    void setScreenVolume(int volume);
    int screenVibra();
    void setScreenVibra(int volume);

#ifdef Q_OS_SYMBIAN
private:
    void initModel();
    int commitChange();

private:
    MProfileEngineExtended* mEngine;
    MProfileExtended* mProfileExt;
    TProfileToneSettings* mToneSettings;
    const MProfileFeedbackSettings* mFeedbackSettings;
    MProfileSetFeedbackSettings* mSetFeedbackSettings;
    CRepository* mVibraCenRep;
    bool mInitErrFlag;
#endif // Q_OS_SYMBIAN
    };

#endif
