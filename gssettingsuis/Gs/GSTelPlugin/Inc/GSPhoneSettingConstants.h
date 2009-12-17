/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Constants to use for Call, Barring and Network settings
*
*/


#ifndef GSPHONESETTINGCONSTANTS_H
#define GSPHONESETTINGCONSTANTS_H


enum TGeneralSettingsPanic {
    EInvalidIndex,
    EInvalidCurrentItem,
    EInvalidDivertAction,
    ECFDefaultNumberCountError,
    EAIViewCompleteWithNullViewId,
    EAIViewCreateWithNonNullViewId
    };

//  LOCAL FUNCTIONS
GLDEF_C inline void Panic( TDesC aClassName, TGeneralSettingsPanic aPanic )
    {
    User::Panic( aClassName, aPanic );
    }

// maximum string length for resource strings
const TInt KGSMaxStringLength = 64;

// barring pwd length
const TInt KGSBarringPasswordLength = 4;

// custom message id, when trying to activate view from phone
const TInt KGSCustomActivateNetView = 9000;

// on/off setting ON
const TInt KGSSettingOn = 1;

// on/off setting OFF
const TInt KGSSettingOff = 0;

// on/off setting's index 1 is OFF
const TInt KGSIndexOff = 1;

// on/off setting's index 0 is ON
const TInt KGSIndexOn = 0;

// manual select mode
const TInt KGSManualSelectMode = 1;

// automatic select mode
const TInt KGSAutomaticSelectMode = 0;

// CLIR values
const TInt KGSCliDefault = 0;
const TInt KGSCliOn = 1;
const TInt KGSCliOff = 2;

// ALS block values
const TInt KGSAlsBlockOn = 0;
const TInt KGSAlsBlockOff = 1;

// array granularity
const TInt KGSNetworkInfoArrayGranularity = 10; 

// Granularity of the arrays in Popup menus
const TInt KGSCallPopupMenuItems = 4;
const TInt KGSNetPopupMenuItems = 3;

//Error note type ID - for Net view internal usage only
const TInt KGSErrorNote = 5001;

//Confirmation note type ID - for Net view internal usage only
const TInt KGSConfirmationNote = 5002;

//Information note type ID - for Net view internal usage only
const TInt KGSInformationNote = 5003;

// PIN query OK
const TInt KGSPinQueryOk = 1;

// MCN is active
const TInt KGSMcnServiceOn = 1;

// Single match for CF search.
const TInt KCFSingleContact = 1;

//Number of default CF numbers.
const TInt KCFDefaultNumbers = 3;

//Number of Delay times.
const TInt KGSNrOfDelays = 6;

//Increment of each delay step.
const TInt KCFDelayStep = 5; //5,10,15,20,25,30

//Number has several '+''s in it.
const TInt KCFErrIllegalNumber = 1;

//No delay time is needed.
const TInt KCFNoNeedToGetTimeValue = 1;

//Divert to mailbox number.
const TInt KCFDivertToMailBox = 0;

//How many characters to use for CF match.
const TInt KCFCharactersToMatch = 7;

//Max. number of target items for CFs.
const TInt KCFMaxNrOfTargets   =   5;

//If none of the four special characters are found from CF number, 
//this value is returned. KErrNotFound(-1)*4 = -4.
const TInt KCFValidPhoneNumber =  -4;

//Invalid characters in the CF number.
_LIT ( KCFInvalidChars, "*#pw" );

//Plus sign.
_LIT ( KCFPlusSign, "+");

//Length of a setting item.
const TInt KGSSettingItemLength = 128;

//Typedef for setting items.
typedef TBuf<KGSSettingItemLength> TSettingItem;

//Length of short network name.
const TInt KGSNwShortNameLength = 10;

//Length of long network name.
const TInt KGSNwLongNameLength = 20;

//Network setting item typedef.
typedef TBuf<KGSNwLongNameLength> TNetworkID;

//Length of SMS txt editor.
const TInt KGSSMSTxtEditorLength = 160;

//Typedef for SMS txt editor.
typedef TBuf<KGSSMSTxtEditorLength> TGSSMSTextEditor;

//Empty space.
_LIT( KGSEmptySpace, " " );

//unused parameter
const TInt KGSNotUsed = -1;

//Japanese replace prefix max length
const TInt KGSReplacePrefixTextLength = 10;

#endif //GSPHONESETTINGCONSTANTS_H
//End of file
