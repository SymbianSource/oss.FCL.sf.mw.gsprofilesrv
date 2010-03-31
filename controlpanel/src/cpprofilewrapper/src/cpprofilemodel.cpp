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

#include "cpprofilemodel.h"
#include "cpprofilemodel_p.h"

CpProfileModel::CpProfileModel()
{
    dptr = new CpProfileModelPrivate();
}

CpProfileModel::~CpProfileModel()
{
    delete dptr;
}

/*
 * Get the result of the initiation
 */
int CpProfileModel::initiationFlag()
{
    return dptr->initiationFlag();
}

/*
 * Get path and file name of ring tone file
 */
QString CpProfileModel::ringTone()
{
    return dptr->ringTone();
}

/*
 * Set path and file to ring tone
 */
void CpProfileModel::setRingTone(const QString& filePath)
{
    dptr->setRingTone(filePath);
}

/*
 * Get path and file name of message tone file
 */
QString CpProfileModel::messageTone()
{
    return dptr->messageTone();
}

/*
 * Set path and file to message tone
 */
void CpProfileModel::setMessageTone(const QString& filePath)
{
    dptr->setMessageTone(filePath);
}

/*
 * Get path and file name of email tone file
 */
QString CpProfileModel::emailTone()
{
    return dptr->emailTone();
}

/*
 * Set path and file to email tone
 */
void CpProfileModel::setEmailTone(const QString& filePath)
{
    dptr->setEmailTone(filePath);
}

/*
 * Get path and file name of calendar event tone file
 */
QString CpProfileModel::calendarTone()
{
    return dptr->calendarTone();
}

/*
 * Set path and file to calendar event tone
 */
void CpProfileModel::setCalendarTone(const QString& filePath)
{
    dptr->setCalendarTone(filePath);
}

/*
 * Get path and file name of clock alarm tone file
 */
QString CpProfileModel::alarmTone()
{
    return dptr->alarmTone();
}

/*
 * Set path and file to clock alarm tone
 */
void CpProfileModel::setAlarmTone(const QString& filePath)
{
    dptr->setAlarmTone(filePath);
}

/*
 * Get the value of master volume
 */
int CpProfileModel::ringVolume()
{
    return dptr->ringVolume();
}

/*
 * Set master volume, the value should be between 1-10
 */
void CpProfileModel::setRingVolume(int volume)
{
    dptr->setRingVolume(volume);
}

/*
 * Activate master volume to beep
 */
void CpProfileModel::activateBeep()
{
    dptr->activateBeep();
}

/*
 * Get beep status in master volume
 */
bool CpProfileModel::isBeep()
{
    return dptr->isBeep();
}

/*
 * Activate master volume to silent
 */
void CpProfileModel::activateSilent()
{
    dptr->activateSilent();
}

/*
 * Get silent status in master volume
 */
bool CpProfileModel::isSilent()
{
    return dptr->isSilent();
}

/*
 * Get master vibra's status
 */
bool CpProfileModel::vibraStatus()
{
    return dptr->vibraStatus();
}

/*
 * Set master vibra's status
 */
void CpProfileModel::setVibraStatus(bool status)
{
    dptr->setVibraStatus(status);
}

/*
 * Get keypad' volume
 */
int CpProfileModel::keyVolume()
{
    return dptr->keyVolume();
}

/*
 * Set keypad's volume, 
 * the value of the volume should be between 0-3
 */
void CpProfileModel::setKeyVolume(int volume)
{
    dptr->setKeyVolume(volume);
}

/*
 * Get screen tone's volume
 */
int CpProfileModel::screenVolume()
{
    return dptr->screenVolume();
}

/*
 * Set screen tone's volume, 
 * the value of the volume should be between 0-3
 */
void CpProfileModel::setScreenVolume(int volume)
{
    dptr->setScreenVolume(volume);
}

/*
 * Get screen vibra's level
 */
int CpProfileModel::screenVibra()
{
    return dptr->screenVibra();
}

/*
 * Set screen vibra's level, 
 * the value of the level should be between 0-3
 */
void CpProfileModel::setScreenVibra(int volume)
{
    dptr->setScreenVibra(volume);
}

// End of file

