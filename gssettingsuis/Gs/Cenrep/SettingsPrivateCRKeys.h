/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Settings UIs private Central Repository keys.
*
*/


#ifndef SETTINGSPRIVATECRKEYS_H
#define SETTINGSPRIVATECRKEYS_H

// INCLUDES
#include <e32def.h>

// CONSTANTS

// ---------------------------------------------------------------------------
/** Following keys belong to category KCRUidMiscSettings  */

/**
*
* String value.
*/
const TUint32 KWatchDogQuarantine               = 0x00000001;

/**
*
* String value.
*/
const TUint32 KWatchDogBlackList                = 0x00000002;
const TUint32 KWatchDogGSRunning                = 0x00000003;
const TUint32 KWatchDogCrashCounter             = 0x00000004;
const TUint32 KWatchdogActivationLimit          = 0x00000005;
const TUint32 KWatchdogMaxPluginBlackListedRuns = 0x00000006;


// ---------------------------------------------------------------------------
/** Following keys belong to category KCRUidNetworkSettings  */

/**
* This key is required for the programs to control if the "network mode"
* item should be available in the UI.
*
* Possible integer values:
* 0 = "network mode" item hidden
* 1 = "network mode" item visible in GS
*/
const TUint32 KSettingsNetworkMode  = 0x00000001;

/**
* This key is required for the programs to list the supported
* network mode UI list items that will be visible from GS setting page.
*/
const TUint32 KSettingsNetworkModeList = 0x00000002;

/**
* This key is required for the programs to set a default value to the
* network mode UI list item visible from GS.
* This value is used to display the selected item from associated setting page.
*
* Possible integer values:
* 0 = Dual mode
* 1 = UMTS
* 2 = GSM
*/
const TUint32 KSettingsNetworkModeValue = 0x00000003;

/**
* HSDPA Visibility
*
* Possible values:
* 0 = Hide
* 1 = Show
*/
const TUint32 KSettingsHSDPAVisbility = 0x00000007;

/**
* HSDPA Value
*
* Possible values:
* 0 = Disable
* 1 = Enable
*/
const TUint32 KSettingsHSDPAValue = 0x00000008;


// ---------------------------------------------------------------------------
/** Following keys belong to category KCRUidPersonalizationSettings */

/**
* Background image.
* Possible values are:
* 0 = No image
* 1 = Image used
*/
const TUint32 KSettingsBackgroundImage = 0x00000001;

/**
* Background image path.
* The value is a string which length can vary from 0 to maximum length.
*/
const TUint32 KSettingsBackgroundImagePath = 0x00000002;

/**
* Hide/Unhide user welcome note item in UI.
* 0 = Hide
* 1 = Show
*/
const TUint32 KSettingsWelcomeNoteSupported = 0x00000005;


// ---------------------------------------------------------------------------
/** Following keys belong to category KCRUidCdmaTelephonySettings  */

/**
* This key indicates whether the call guard is activated
* for MO and MT calls.
*
* 0 = Disabled
* 1 = Enabled
*/
const TUint32 KSettingsCallGuard = 0x00000001;


/**
* This key indicates whether voice privacy settings
* needs to be included and voice privacy status.
*
* Possible integer values:
* 0 = Option 1 - {VP Menu excluded, VP Off}
* 1 = Option 2 - {VP Menu excluded, VP On}
* 2 = Option 3 - {VP Menu included, VP Off}
* 3 = Option 4 - {VP Menu included, VP On}
*/
const TUint32 KSettingsVoicePrivacy = 0x00000002;


/**
* This key indicates whether the distinctive ring tone
* is enabled while roaming.
*
* 0 = Disabled
* 1 = Enabled
*/
const TUint32 KSettingsRoamingRingingTone = 0x00000003;


// ---------------------------------------------------------------------------
/** Following keys belong to category KCRUidSettingsVariation  */

/**
* Settings Variation API
* Keys under this category are used in defining settings variation.
*/
const TUid KCRUidSettingsVariation = {0x1020720A};

/**
* Settings Local Variation Flags. Values are defined in GeneralSettingsVariant.hrh.
*/
const TUint32 KSettingsVariationFlags = 0x00000001;

 
#endif      // SETTINGSPRIVATECRKEYS_H

// End of file