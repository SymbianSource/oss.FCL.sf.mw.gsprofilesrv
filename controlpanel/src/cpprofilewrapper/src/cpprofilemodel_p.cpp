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

#include "cpprofilemodel_p.h"
#include <e32base.h>
#include <QString>
#include <MProfileEngineExtended.h>
#include <MProfileExtended.h>
#include <MProfileTones.h>
#include <MProfileSetTones.h>
#include <MProfileExtraTones.h>
#include <MProfileSetExtraTones.h>
#include <MProfileExtraSettings.h>
#include <MProfileSetExtraSettings.h>
#include <MProfileFeedbackSettings.h>
#include <MProfileSetFeedbackSettings.h>

#include <TProfileToneSettings.h>
#include <hwrmvibrasdkcrkeys.h>
#include <centralrepository.h>


QString stringFromDescriptor(const TDesC& dsp)
{
    return QString::fromUtf16(dsp.Ptr(), dsp.Length());
}

HBufC* descriptorFromString(const QString& str)
{
    TPtrC ptr(reinterpret_cast<const TUint16*>(str.utf16()));
    return ptr.Alloc();
}

CpProfileModelPrivate::CpProfileModelPrivate()
{
     TRAPD( mInitErrFlag, initModel() );
}

void CpProfileModelPrivate::initModel()
{
    mEngine = CreateProfileEngineExtendedL();
    mProfileExt = mEngine->ProfileL( mEngine->ActiveProfileId() );

    // General tones
    TProfileToneSettings& toneSettings = mProfileExt->ProfileSetTones().SetToneSettings();
    mToneSettings = &toneSettings;
    mVibraCenRep = CRepository::NewL( KCRUidVibraCtrl );

    // Feedback settings, used to get screen tone
    const MProfileFeedbackSettings& feedback = 
                            mProfileExt->ProfileExtraSettings().ProfileFeedbackSettings();
    mFeedbackSettings = &feedback;

    // Feedback settings, used to set screen tone
    MProfileSetFeedbackSettings& setFeedback = 
                            mProfileExt->ProfileSetExtraSettings().ProfileSetFeedbackSettings();
    mSetFeedbackSettings = &setFeedback;
}

CpProfileModelPrivate::~CpProfileModelPrivate()
{
    delete mVibraCenRep;
    mVibraCenRep = NULL;
}

/*
 * Get the result of the initiation
 */
int CpProfileModelPrivate::initiationFlag()
{
    return mInitErrFlag;
}

/*
 * Get path and file name of ring tone file
 */
QString CpProfileModelPrivate::ringTone()
{
    return stringFromDescriptor( mProfileExt->ProfileTones().RingingTone1() );
}

/*
 * Set path and file to ring tone
 */
void CpProfileModelPrivate::setRingTone(const QString& filePath)
{
    mProfileExt->ProfileSetTones().SetRingingTone1L( *descriptorFromString(filePath) );
    commitChange();
}

/*
 * Get path and file name of message tone file
 */
QString CpProfileModelPrivate::messageTone()
{
    return stringFromDescriptor( mProfileExt->ProfileTones().MessageAlertTone() );
}

/*
 * Set path and file to message tone
 */
void CpProfileModelPrivate::setMessageTone(const QString& filePath)
{
    mProfileExt->ProfileSetTones().SetMessageAlertToneL( *descriptorFromString(filePath) );
    commitChange();
}

/*
 * Get path and file name of email tone file
 */
QString CpProfileModelPrivate::emailTone()
{
    return stringFromDescriptor( mProfileExt->ProfileExtraTones().EmailAlertTone() );
}

/*
 * Set path and file to email tone
 */
void CpProfileModelPrivate::setEmailTone(const QString& filePath)
{
    mProfileExt->ProfileSetExtraTones().SetEmailAlertToneL( *descriptorFromString(filePath) );
    commitChange();
}

/*
 * Get path and file name of calendar event tone file
 */
QString CpProfileModelPrivate::calendarTone()
{
    return "";
}

/*
 * Set path and file to calendar event tone
 */
void CpProfileModelPrivate::setCalendarTone(const QString& filePath)
{
    Q_UNUSED(filePath);
}

/*
 * Get path and file name of clock alarm tone file
 */
QString CpProfileModelPrivate::alarmTone()
{
    return "";
}

/*
 * Set path and file to clock alarm tone
 */
void CpProfileModelPrivate::setAlarmTone(const QString& filePath)
{
    Q_UNUSED(filePath);
}

/*
 * Get the value of master volume
 */
int CpProfileModelPrivate::ringVolume()
{
    return mToneSettings->iRingingVolume;
}

/*
 * Set master volume, the value should be between 1-10
 */
void CpProfileModelPrivate::setRingVolume(int volume)
{
    if (volume < 0 || volume >10)
        {
        return;
        }

    mToneSettings->iRingingType = EProfileRingingTypeRinging;
    mToneSettings->iRingingVolume = volume;
    commitChange();
}

/*
 * Activate master volume to beep
 */
void CpProfileModelPrivate::activateBeep()
{
    mToneSettings->iRingingType = EProfileRingingTypeBeepOnce;
    commitChange();
}

/*
 * Get beep status in master volume
 */
bool CpProfileModelPrivate::isBeep()
{
    return (EProfileRingingTypeBeepOnce == mToneSettings->iRingingType) ? true : false;
}

/*
 * Activate master volume to silent
 */
void CpProfileModelPrivate::activateSilent()
{
    mToneSettings->iRingingType = EProfileRingingTypeSilent;
    commitChange();
}

/*
 * Get silent status in master volume
 */
bool CpProfileModelPrivate::isSilent()
{
    return (EProfileRingingTypeSilent == mToneSettings->iRingingType) ? true : false;
}

/*
 * Get master vibra's status
 */
bool CpProfileModelPrivate::vibraStatus()
{
    return mToneSettings->iVibratingAlert;
}

/*
 * Set master vibra's status
 */
void CpProfileModelPrivate::setVibraStatus(bool status)
{
    mVibraCenRep->Set(KVibraCtrlProfileVibraEnabled, status );
    mToneSettings->iVibratingAlert = status;
    commitChange();
}

/*
 * Get keypad' volume
 */
int CpProfileModelPrivate::keyVolume()
{
    return mToneSettings->iKeypadVolume;
}

/*
 * Set keypad's volume, 
 * the value of the volume should be between 0-3
 */
void CpProfileModelPrivate::setKeyVolume(int volume)
{
    if (volume < 0 || volume > 3)
    {
        return;
    }

    mToneSettings->iKeypadVolume = (TProfileKeypadVolume)volume;
    commitChange();
}

/*
 * Get screen tone's volume
 */
int CpProfileModelPrivate::screenVolume()
{
    return mFeedbackSettings->AudioFeedback();
}

/*
 * Set screen tone's volume, 
 * the value of the volume should be between 0-3
 */
void CpProfileModelPrivate::setScreenVolume(int volume)
{
    if (volume < 0 || volume > 3)
    {
        return;
    }

    mSetFeedbackSettings->SetAudioFeedback( (TProfileAudioFeedback)volume );
    commitChange();
}

/*
 * Get screen vibra's level
 */
int CpProfileModelPrivate::screenVibra()
{
    return mFeedbackSettings->TactileFeedback();
}

/*
 * Set screen vibra's level, 
 * the value of the level should be between 0-3
 */
void CpProfileModelPrivate::setScreenVibra(int volume)
{
    if (volume < 0 || volume > 3)
    {
        return;
    }

    mSetFeedbackSettings->SetTactileFeedback( (TProfileTactileFeedback)volume );
    commitChange();
}

int CpProfileModelPrivate::commitChange()
{
    TRAPD( err, mEngine->CommitChangeL(*mProfileExt) );
    return err;
}

// End of file

