/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Common constants and enums used in all three implementations.
*
*/


#ifndef TDAUDIOCONSTANTS_H
#define TDAUDIOCONSTANTS_H

// CONSTANTS
// Application UID, same UID is used for Central Repository.
//const TUid KUid3DAudioRingingTone = { 0x10201AFD };
//const TUid K3DAudioRepositoryUid = { 0x10201AFD };

const TUid KUid3DAudioRingingTone = { 0x10208ADF };
const TUid K3DAudioRepositoryUid = { 0x10208ADF };

#ifdef __SERIES60_30__
// Key to ringing tone in first profile (number 0)
const TInt KProEngRingingTone = 0x00000004;

// Multiplier for each profile.
const TInt KProfileMultiplier = 0x01000000;

#endif

// Extension for not supported ringing tones.
_LIT( KRngType, ".rng" );

// Filename of "No-Sound" ringing tone.
_LIT( KNoToneFileName, "Z:\\resource\\No_Sound.wav" );

// Default velocity for all motion patterns.
const TInt KMotionPatternDefaultVelocity = 5;

// DATA TYPES
// Keys to Central Repository.
enum T3DAudioRepositoryKeys
	{
	E3DAudioRepositoryStatus = 0x1,
	E3DAudioRepositoryPattern,
	E3DAudioRepositoryVelocity,
	E3DAudioRepositoryReverb,
	E3DAudioRepositoryDoppler,
	E3DAudioRepositoryDopplerAvailable,
	E3DAudioRepositoryRingingToneInUse,
	E3DAudioRepositoryProfileId,
	
	E3DAudioRepositoryLoadedPattern = 0x10,
	E3DAudioRepositoryBlockCount,
	E3DAudioRepositoryBlockStart = 0x20
	};

#endif // TDAUDIOCONSTANTS_H

// End of File
