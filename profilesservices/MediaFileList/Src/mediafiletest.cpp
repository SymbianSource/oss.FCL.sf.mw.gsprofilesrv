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
* Description:   Class create for testing.
*
*/




// INCLUDE FILES

#include "mediafiletest.h"
#include "mediafiledialog.h"
#include "mediafilepreview.h"
#include "mediafilelistdebug.h"

#include <mediafilelist.rsg>

#include <aknappui.h>  // for macro iAvkonAppUi
#include <eikcapc.h>   // for CEikCaptionedControl
#include <e32math.h>  // Math::FRand

#include <bautils.h>  // bafl.lib
#include <s32file.h>  // RFileReadStream



#ifdef _DEBUG
/******************************************************************************
 * class CMFDialogTest
 ******************************************************************************/



// -----------------------------------------------------------------------------
// CMFDialogTest::NewL
//
// -----------------------------------------------------------------------------
CMFDialogTest* CMFDialogTest::NewL( CMediaFileDialog* aDialog )
    {
    CMFDialogTest* self = new (ELeave) CMFDialogTest( aDialog );
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }


// -----------------------------------------------------------------------------
// CMFDialogTest::CMFDialogTest
// 
// -----------------------------------------------------------------------------
//
CMFDialogTest::CMFDialogTest( CMediaFileDialog* aDialog )
    {
    iDlg = aDialog;
    
    iTestCounter = 0;
    iBottomVisited = EFalse;
    iDestroyedPtr = NULL;
    }


// ----------------------------------------------------------------------------
// Destructor
//
// ----------------------------------------------------------------------------
//
CMFDialogTest::~CMFDialogTest()
    {
    SetScreenSaverPeriod( iScreenSaverPeriod );
    
    delete iTestCaller;
    delete iRandomNum;
   
    if ( iPopupNote )
        {
        iPopupNote->HideInfoPopupNote();
        delete iPopupNote;
        }

    if ( iDestroyedPtr )
        {
        *iDestroyedPtr = ETrue;
        iDestroyedPtr = NULL;
        }
    }


// -----------------------------------------------------------------------------
// CMFDialogTest::ConstructL
//
// -----------------------------------------------------------------------------
//
void CMFDialogTest::ConstructL()
    {
    const TInt KLongPeriod = 60*60*24; // 24 h in seconds
    
    iTestCaller = CMFActiveCaller::NewL( this );
    iRandomNum = CRandomNum::NewL();
   
    iScreenSaverPeriod = ScreenSaverPeriodL();
    SetScreenSaverPeriod( KLongPeriod );
    
    iPopupNote = CAknInfoPopupNoteController::NewL();
    }


// -----------------------------------------------------------------------------
// CMFDialogTest::TestL
//
// -----------------------------------------------------------------------------
//
TBool CMFDialogTest::TestL( TInt /*aAttr*/ )
    {
    TInt index = 0;
    CDesCArray *arr = new(ELeave) CDesCArrayFlat( 1 );
    CleanupStack::PushL( arr );
     
    arr->AppendL(_L("random play"));
    arr->AppendL(_L("test 1"));
    arr->AppendL(_L("test 2"));

    if ( !TMFDialogUtil::ShowListQueryL( _L("huu"), arr, &index ) )
        {
        CleanupStack::PopAndDestroy( arr );
        return EFalse;
        }
    if ( index == 0 )
        {
        NextListItemL();
        }
    else if ( index == 1 )
        {
        Test1L( 0 );    
        }
    else if ( index == 2 )
        {
        Test2L( 0 );
        }
    else
        {
        }

    CleanupStack::PopAndDestroy( arr );
    return ETrue; 
    }


// -----------------------------------------------------------------------------
// CMFDialogTest::Test1L
//
// -----------------------------------------------------------------------------
//
void CMFDialogTest::Test1L( TInt /*aAttr*/ )
    {
    }



// -----------------------------------------------------------------------------
// CMFDialogTest::Test2L
//
// -----------------------------------------------------------------------------
//
void CMFDialogTest::Test2L( TInt /*aAttr*/ )
    {
    TInt num10 = 0; TInt num11 = 0; TInt num12 = 0; 
    TInt num13 = 0; TInt num14 = 0; TInt num15 = 0;
    TInt num16 = 0; TInt num17 = 0; TInt num18 = 0;
    TInt num19 = 0; TInt num20 = 0; TInt numOther = 0;
    
    for (TInt i=0; i<100000; i++)
        {
        TInt num = iRandomNum->Random( 10, 20 );
        
        if ( num == 10 )
            {
            num10++;
            }
        else if ( num == 11 )
            {
            num11++;
            }
        else if ( num == 12 )
            {
            num12++;
            }
        else if ( num == 13 )
            {
            num13++;
            }
        else if ( num == 14 )
            {
            num14++;
            }
        else if ( num == 15 )
            {
            num15++;
            }
        else if ( num == 16 )
            {
            num16++;
            }
        else if ( num == 17 )
            {
            num17++;
            }
        else if ( num == 18 )
            {
            num18++;
            }
        else if ( num == 19 )
            {
            num19++;
            }
        else if ( num == 20 )
            {
            num20++;
            }
        else
            {
            numOther++;
            }
      
        }
    
    }


// -----------------------------------------------------------------------------
// CMFDialogTest::NextFolderL
//
// -----------------------------------------------------------------------------
//
void CMFDialogTest::NextFolderL() 
    {
    const TInt KDelay = 2000;
    
    TBool destroyed = EFalse;
    iDestroyedPtr = &destroyed;  // updated in this class destructor
    
    TInt currentFolder = iDlg->iState->CurrentFolder();
    TInt currentIndex = iDlg->CurrentItemListIndex();
    TInt folderId = iDlg->iState->FolderId( currentIndex );
    
    if ( currentFolder == EEFolderRoot )
        {
        // root folder
        iBottomVisited = EFalse;
        if ( iRandomNum->Match( 30 ) )
            {
            SimulateSearchEventL();  // search
            }
        else if ( folderId == EEFolderMusic || folderId == EEFolderTones ||
             folderId == EEFolderRecordings || folderId == EEFolderVideos )
            {
            SimulateOpenEventL();  // go down    
            }
        else
            {
            iTestCaller->Start( ENextListItem, KDelay );
            }
        }
    else if ( currentIndex == KErrNotFound )
        {
        // empty folder
        SimulateBackEventL();  // go up
        }
    else if ( folderId == KErrNotFound )
        {
        // this folder has files, not subfolders
        iBottomVisited = ETrue;
        if ( iRandomNum->Match( 50 ) )
            {
            // stay in this folder and select a file
            iTestCaller->Start( ENextListItem, KDelay );
            return;
            }
        else
            {
            SimulateBackEventL();  // go up
            }
        }
    else  // this folder has subfolders
        {
        TInt num = 15;
        if ( !iBottomVisited )
            {
            num = 85;
            }
            
        if ( iRandomNum->Match( num ) )
            {
            SimulateOpenEventL();  // go down 
            }
        else
            {
            SimulateBackEventL();  // go up
            }
        }
    
    // select listbox item after delay
    if ( !destroyed )
        {
        iDestroyedPtr = NULL;
        iTestCaller->Start( ENextListItem, KDelay );
        }
    }


// -----------------------------------------------------------------------------
// CMFDialogTest::NextListItemL
//
// -----------------------------------------------------------------------------
//
void CMFDialogTest::NextListItemL() 
    {
    const TInt KDelay = 250;
    const TInt KLongDelay = 2000;
    
    TInt delay = KDelay;
    
    TInt currentIndex = iDlg->CurrentItemListIndex();
    TInt folderId = iDlg->iState->FolderId( currentIndex );
    
    if ( folderId == KErrNotFound && currentIndex == 0 )
        {
        // this folder contain files - make delay longer to 
        // allow first file preview
        delay = KLongDelay;  
        }
        
    TInt itemCount = iDlg->iListBox->Model()->NumberOfItems();
    iNewIndex = iRandomNum->Random( itemCount - 1);
   
    if ( iDlg->iState->CurrentFolder() == EEFolderRoot )
        {
        if ( iRandomNum->Match( 50 ) )
            {
            iNewIndex = 2; // more chances to Music folder
            }
        }
    
    if ( iNewIndex == currentIndex )
        {
        iTestCaller->Start( ENextFolder, delay );
        }
    else
        {
        iTestCaller->Start( EMovePosition, delay );
        }
    }


// -----------------------------------------------------------------------------
// CMFDialogTest::MovePositionL
//
// Scrolls listbox until selected item (iNewIndex) is found.
// -----------------------------------------------------------------------------
//
void CMFDialogTest::MovePositionL()
    {
    const TInt KMoveDelay = 150;
    const TInt KThousand = 1000;
   
    TInt currentIndex = iDlg->CurrentItemListIndex();
    TInt folderId = iDlg->iState->FolderId( currentIndex );
    TInt itemCount = iDlg->iListBox->Model()->NumberOfItems();

    TBool itemFound = EFalse;
    if ( currentIndex == iNewIndex )
        {
        itemFound = ETrue; // listbox has scrolled to new item
        }
    if ( currentIndex == KErrNotFound )
        {
        itemFound = ETrue; // folder is empty
        }
    if ( iNewIndex >= itemCount )
        {
        itemFound = ETrue; // wrong folder
        }

    if ( itemFound )
        {
        // next listbox selection found
        
        TInt delay = KMoveDelay;
        if ( folderId == KErrNotFound )
            {
            // selection is file - allow time for preview
            delay = iRandomNum->Random( 3, 10 );
            delay = delay * KThousand;
            }
        iTestCaller->Start( ENextFolder, delay );
        return;
        }
    
    // Simulate listbox item selection.
    SimulateKeyEventL( EKeyUpArrow );
    
    //FLOG( _L("CMediaFileDialog::MovePositionL '%d/%d'"), currentIndex, iNewIndex );
    
    iTestCaller->Start( EMovePosition, KMoveDelay );
    }


// -----------------------------------------------------------------------------
// CMFDialogTest::SimulateKeyEventL
//
// -----------------------------------------------------------------------------
//
void CMFDialogTest::SimulateKeyEventL( TInt aEvent )
    {
    TKeyEvent key;
    key.iModifiers = 0;
    key.iRepeats = 0;
    key.iCode = aEvent;
    key.iScanCode = aEvent;

    //iListBox->SetCurrentItemIndexAndDraw( newIndex );
    CCoeEnv::Static()->SimulateKeyEventL( key, EEventKey );
    }


// -----------------------------------------------------------------------------
// CMFDialogTest::SimulateOpenEventL
//
// -----------------------------------------------------------------------------
//
void CMFDialogTest::SimulateOpenEventL()
    {
    TKeyEvent keyEvent;
    keyEvent.iCode = EKeyDevice3;
    keyEvent.iScanCode = EStdKeyDevice3;
    keyEvent.iRepeats = 0;          
    CCoeEnv::Static()->SimulateKeyEventL( keyEvent, EEventKey );
    }


// -----------------------------------------------------------------------------
// CMFDialogTest::SimulateBackEventL
//
// -----------------------------------------------------------------------------
//
void CMFDialogTest::SimulateBackEventL()
    {
    TKeyEvent keyEvent;
    keyEvent.iCode = EKeyDevice1;
    keyEvent.iScanCode = EStdKeyDevice1;
    keyEvent.iRepeats = 0;          
    CCoeEnv::Static()->SimulateKeyEventL( keyEvent, EEventKey );
    }


// -----------------------------------------------------------------------------
// CMFDialogTest::SimulateSearchEventL
//
// -----------------------------------------------------------------------------
//
void CMFDialogTest::SimulateSearchEventL()
    {
    // letter a 97/65
    // letter b 98/66
    
    TKeyEvent keyEvent;
    keyEvent.iCode = 97;
    keyEvent.iScanCode = 65;
    keyEvent.iRepeats = 0;          
    CCoeEnv::Static()->SimulateKeyEventL( keyEvent, EEventKey );
    }


// -----------------------------------------------------------------------------
// CMFDialogTest::SetScreenSaverPeriod
//
// -----------------------------------------------------------------------------
//
void CMFDialogTest::SetScreenSaverPeriod( TInt aPeriod )
    {
    TRAP_IGNORE( SetScreenSaverPeriodL( aPeriod ) );
    }


// -----------------------------------------------------------------------------
// CMFDialogTest::SetScreenSaverPeriodL
//
// -----------------------------------------------------------------------------
//
void CMFDialogTest::SetScreenSaverPeriodL( TInt aPeriod )
    {
    // from SettingsInternalCRKeys.h
    const TUid KCRUidPersonalizationSettings = {0x101F877C};
    const TUint32 KSettingsScreenSaverPeriod =  0x00000008;

    CRepository* repository =
                 CRepository::NewL( KCRUidPersonalizationSettings );
    
    // ignore error
    repository->Set( KSettingsScreenSaverPeriod, aPeriod );
    
    delete repository;
    }


// -----------------------------------------------------------------------------
// CMFDialogTest::ScreenSaverPeriodL
//
// -----------------------------------------------------------------------------
//
TInt CMFDialogTest::ScreenSaverPeriodL()
    {
    // from SettingsInternalCRKeys.h
    const TUid KCRUidPersonalizationSettings = {0x101F877C};
    const TUint32 KSettingsScreenSaverPeriod =  0x00000008;

    CRepository* repository =
                 CRepository::NewL( KCRUidPersonalizationSettings );
    
    TInt period = 60; // 60 seconds
    // ignore error
    repository->Get( KSettingsScreenSaverPeriod, period );

    delete repository;
    
    return period;
    }


// -----------------------------------------------------------------------------
// CMFDialogTest::HandleActiveCallL (from MMFActiveCallerObserver)
//
// -----------------------------------------------------------------------------
//
void CMFDialogTest::HandleActiveCallL( TInt aCallId )
    {
    if ( aCallId == ENextFolder )
        {
        //FLOG( _L("CMFDialogTest::HandleActiveCallL: ENextFolder") );
        
        TRAP_IGNORE( NextFolderL() );
        iDestroyedPtr = NULL;
        }
    if ( aCallId == ENextListItem )
        {
        //FLOG( _L("CMFDialogTest::HandleActiveCallL: ENextListItem") );
        NextListItemL();    
        }

    if ( aCallId == EMovePosition )
        {
        //FLOG( _L("CMFDialogTest::HandleActiveCallL: EMovePosition") );
        
        MovePositionL();    
        }
    }


// -----------------------------------------------------------------------------
// CMFDialogTest::SetTime
//
// -----------------------------------------------------------------------------
//
void CMFDialogTest::SetTime()
    {
    iTime = TimeBefore();
    }


// -----------------------------------------------------------------------------
// CMFDialogTest::PrintTimeL
//
// -----------------------------------------------------------------------------
//
void CMFDialogTest::PrintTimeL( const TDesC& aText )
    {
    TBuf<128> buf;
    TInt milliSeconds = TimeAfter( iTime );
    buf.Format( _L("%S %d ms"), &aText, milliSeconds );
    
    ShowPopupNoteL( buf );
    }


// -----------------------------------------------------------------------------
// CMFDialogTest::TimeBefore
//
// Function returns TTime of the current time (needed with function TimeAfter).
// -----------------------------------------------------------------------------
//
TTime CMFDialogTest::TimeBefore()
    {
    TTime now;
    now.UniversalTime();
    return now;
    }


// -----------------------------------------------------------------------------
// CMFDialogTest::TimeAfter
//
// Function returns milliseconds passed since aBefore.
// -----------------------------------------------------------------------------
//
TInt64 CMFDialogTest::TimeAfter( TTime aBefore )
    {
    TTime after;
    after.UniversalTime();
    TTimeIntervalMicroSeconds microSeconds=after.MicroSecondsFrom( aBefore ); 
    TInt64 milliSeconds=microSeconds.Int64()/1000;
    return milliSeconds;
    }


// -----------------------------------------------------------------------------
// CMFDialogTest::ShowPopupNote
//
// -----------------------------------------------------------------------------
//
void CMFDialogTest::ShowPopupNoteL( const TDesC& aText ) 
    {
    const TInt KPopupDisplayTime = 3*1000;
    const TInt KPopupDelayTime = 500;
    
    iPopupNote->SetTextL( aText );
    iPopupNote->SetTimeDelayBeforeShow( KPopupDelayTime );
    iPopupNote->SetTimePopupInView( KPopupDisplayTime );    
    iPopupNote->ShowInfoPopupNote();    
    }




/******************************************************************************
 * class CRandomNum
 ******************************************************************************/


 
// -----------------------------------------------------------------------------
// CRandomNum::NewLC
// 
// -----------------------------------------------------------------------------
//
CRandomNum* CRandomNum::NewLC()
    {
    CRandomNum* randomdata = new (ELeave) CRandomNum;
    CleanupStack::PushL( randomdata );
    return randomdata;
    }


// -----------------------------------------------------------------------------
// CRandomNum::NewL
// 
// -----------------------------------------------------------------------------
//
CRandomNum* CRandomNum::NewL()
    {
    CRandomNum* randomdata = new (ELeave) CRandomNum;
    return randomdata;
    }

// -----------------------------------------------------------------------------
// CRandomNum::CRandomNum
// 
// -----------------------------------------------------------------------------
//
CRandomNum::CRandomNum()
    {
    TTime now;
    now.UniversalTime();
    iSeed = now.Int64();
    }

// -----------------------------------------------------------------------------
// CRandomNum::~CRandomNum
// 
// -----------------------------------------------------------------------------
//
CRandomNum::~CRandomNum()
    {
    }


// -----------------------------------------------------------------------------
// CRandomNum::Random
// 
// Return random integer from 0 to aMax (including 0 and aMax).
// -----------------------------------------------------------------------------
//
TInt CRandomNum::Random( TInt aMax )
    {
    TReal real = Math::FRand( iSeed ); // FRand updates iSeed
    TReal real2 = real * ( aMax+1 );
    TInt num = (TInt) real2;
    return num;
    }


// -----------------------------------------------------------------------------
// CRandomNum::Random
// 
// Return random integer from aMin to aMax (including aMin and aMax).
// -----------------------------------------------------------------------------
//
TInt CRandomNum::Random( TInt aMin, TInt aMax )
    {
    if ( aMin >= aMax )
        {
        return aMin;
        }
    
    TInt range = aMax - aMin;
    
    TInt num = Random( range );
    
    return aMin + num;
    }


// -----------------------------------------------------------------------------
// CRandomNum::Match
//
// Return ETrue according to percentage parameter (eg. in case
// aPercentage is 10 Match returns ETrue in 10% of the calls).
// -----------------------------------------------------------------------------
//
TBool CRandomNum::Match( TInt aPercentage )
    {
    if ( aPercentage < 1 )
        {
        return EFalse;
        }
    
    TInt num = Random( 99 ); // 0..99
    num++; // 1..100

    if ( num <= aPercentage )
        {
        return ETrue;
        }
    
    return EFalse;
    }






/******************************************************************************
 * class CMediaFileDialog3
 ******************************************************************************/

/*

// -----------------------------------------------------------------------------
// media file dialog resource (test code).
// -----------------------------------------------------------------------------
//
RESOURCE DIALOG r_media_file_dialog_3
    {
    //flags = EEikDialogFlagNoBackup | EEikDialogFlagFillAppClientRect;
    flags=EEikDialogFlagWait | EEikDialogFlagFillAppClientRect | EEikDialogFlagNotifyEsc;
    //buttons = R_AVKON_SOFTKEYS_OK_BACK__OK;
    buttons = r_media_file_dialog_3_cba;

    items=
        {
        DLG_LINE
            {
            type=EAknCtSingleGraphicListBox;
            itemflags=EEikDlgItemTakesEnterKey | EEikDlgItemOfferAllHotKeys;
            id=1;
            control=LISTBOX
                {
                flags = EAknListBoxSelectionList;
                };   
            }
        };
    
    }


//------------------------------------------------------------------------------
// Softkey resource
//------------------------------------------------------------------------------
//
RESOURCE CBA r_media_file_dialog_3_cba
    {
    buttons =
        {
        CBA_BUTTON
            {
            id = EAknSoftkeyOptions;
            txt = text_softkey_option;
            },
        CBA_BUTTON
            {
            id = EAknSoftkeyBack;
            txt = text_softkey_back;
            },
        CBA_BUTTON
            {
            id = EAknSoftkeySelect;
            txt = qtn_msk_select;
            }
        };
    }

RESOURCE MENU_BAR r_media_file_dialog_3_menu
    {
    titles= 
        { 
        MENU_TITLE 
            { 
            menu_pane=r_media_file_dialog_3_menu_pane;
            }
        };
    }

RESOURCE MENU_PANE r_media_file_dialog_3_menu_pane
    {
    items=
        {
        MENU_ITEM { command=EAknCmdOpen; txt=qtn_options_open; },
        MENU_ITEM { command=EAknCmdHelp; txt=qtn_options_help; },
        MENU_ITEM { command=EAknCmdExit; txt=qtn_options_exit; }        
        };
    }
*/
/*
 
// -----------------------------------------------------------------------------
// CMediaFileDialog3::ShowDialogL
// 
// -----------------------------------------------------------------------------
TBool CMediaFileDialog3::ShowDialogL()
    {
    CMediaFileDialog3* dlg = CMediaFileDialog3::NewL();
    TBool ret = dlg->ExecuteLD( R_MEDIA_FILE_DIALOG_3 );

    return ret;
    }

// ----------------------------------------------------------------------------
// Destructor
//
// ----------------------------------------------------------------------------
//
CMediaFileDialog3::~CMediaFileDialog3()
    {
    delete iVideoPreviewHandler;
    
    if ( iAvkonAppUi )
        {
        iAvkonAppUi->RemoveFromStack( this );
        }
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog3::CMediaFileDialog3
// 
// -----------------------------------------------------------------------------
//
CMediaFileDialog3::CMediaFileDialog3()
    {
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog3::NewL
//
// -----------------------------------------------------------------------------
CMediaFileDialog3* CMediaFileDialog3::NewL()
    {
    CMediaFileDialog3* self = new (ELeave) CMediaFileDialog3();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }

// -----------------------------------------------------------------------------
// CMediaFileDialog3::ConstructL
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialog3::ConstructL()
    {
    FLOG( _L("CMediaFileDialog3::ConstructL START") );
    
    CAknDialog::ConstructL( R_MEDIA_FILE_DIALOG_3_MENU );

    iVideoPreviewHandler = CMFVideoPreviewHandler::NewL();
    
    FLOG( _L("CMediaFileDialog3::ConstructL END") );
    }

// -----------------------------------------------------------------------------
// CMediaFileDialog3::PreLayoutDynInitL
// 
// -----------------------------------------------------------------------------
//
void CMediaFileDialog3::PreLayoutDynInitL()
    {
    }

//------------------------------------------------------------------------------
// CMediaFileDialog3::OkToExitL
//
//------------------------------------------------------------------------------
//
TBool CMediaFileDialog3::OkToExitL(TInt aButtonId)
    {
    if ( aButtonId == EAknSoftkeyBack )
        {
        }

    return CAknDialog::OkToExitL( aButtonId );
    }



//------------------------------------------------------------------------------
// CMediaFileDialog3::ProcessCommandL
//
// Handle commands from menu.
//------------------------------------------------------------------------------
//
void CMediaFileDialog3::ProcessCommandL(TInt aCommandId)
    {
    HideMenu();
    
    
    switch (aCommandId)
        {
        case EAknCmdHelp:
            {
            break;
            }
            
        case EAknCmdOpen:
            {
            CEikCaptionedControl *ctrl = Line( 1 );  // dialog has one line (listbox) with id 1
            iVideoPreviewHandler->SetAttrL( TMFDialogUtil::EAttrDrawingWindow, (ctrl->iControl->DrawableWindow()) );
                
            iVideoPreviewHandler->SetAttrL( TMFDialogUtil::EAttrFileName, _L("C:\\Data\\Videos\\video2.mp4") );
            iVideoPreviewHandler->PlayL(); 

            break;
            }
 
        case EAknCmdExit:
        case EEikCmdExit:
            {
            // close dialog and exit calling application
            iAvkonAppUi->ProcessCommandL( EAknCmdExit );
            break;
            }

        default:            
            break;
        }
    }
*/


// -----------------------------------------------------------------------------
// CMediaFileHandler::QueryAllL (obsolete)
// 
// Finds all tones, music, videos or recordings.
// -----------------------------------------------------------------------------
//
/*
void CMediaFileHandler::QueryAllL( TInt aAttr )
    {
    if ( !iSessionOpen )
        {
        User::Leave( KErrDisconnected );
        }

    if ( aAttr != EAttrMusic && aAttr != EAttrVideo )
        {
        User::Leave( KErrNotSupported );
        }
        
    CMdENamespaceDef& defNS = iSession->GetDefaultNamespaceDefL();
    CMdEObjectDef& musicObjectDef = defNS.GetObjectDefL( MdeConstants::Audio::KAudioObject );
    CMdEObjectDef& videoObjectDef = defNS.GetObjectDefL( MdeConstants::Video::KVideoObject );
   
    delete iQuery;
    iQuery = NULL;

    if ( aAttr == EAttrVideo )
        {
        iQuery = iSession->NewObjectQueryL( defNS, videoObjectDef, this );
        iQueryType = EQueryVideo;
        }
    else
        {
        iQuery = iSession->NewObjectQueryL( defNS, musicObjectDef, this );
        iQueryType = EQueryAudio;
        }
    
    
    // set attributes that are included in query result  
    CMdEPropertyDef& namePropertyDef = PropertyDefL( EAttrSongName );
    iQuery->AddPropertyFilterL( &namePropertyDef );
    
    // define sort order
    iQuery->AppendOrderRuleL( TMdEOrderRule( namePropertyDef, ESortAscending ) );
    
    iQuery->SetResultMode( EQueryResultModeItem );
    
    iQuery->FindL();
    
    iQueryType = EQueryAudio;
    }
*/




// -----------------------------------------------------------------------------
// CMediaFileHandler::QueryArtistUnknownL (obsolete)
// 
// Finds all songs for aArtist that miss album property.
// -----------------------------------------------------------------------------
//
/*
void CMediaFileHandler::QueryArtistUnknownL( const TDesC& aArtist )
    {
    if ( !iSessionOpen )
        {
        return;
        }

    CMdENamespaceDef& defNS = iSession->GetDefaultNamespaceDefL();
    CMdEObjectDef& objectDef = defNS.GetObjectDefL( MdeConstants::Audio::KAudioObject );

    iQueryType = EQueryAudio;
    
    delete iQuery;
    iQuery = NULL;
    iQuery = iSession->NewObjectQueryL( defNS, objectDef, this );
    
    iQuery->SetResultMode( EQueryResultModeItem );
    
    CMdEPropertyDef& artistPropertyDef = PropertyDefL( EAttrArtist );
    CMdEPropertyDef& namePropertyDef = PropertyDefL( EAttrSongName );
    CMdEPropertyDef& albumPropertyDef = PropertyDefL( EAttrAlbum );
    
    
    CMdELogicCondition& conditions = iQuery->Conditions();
    
    // add root condition: get all songs by aArtist
    conditions.AddPropertyConditionL( artistPropertyDef, 
                                      ETextPropertyConditionCompareEquals,
                                      aArtist );
                                      
    // set attributes that are included in query result  
    iQuery->AppendOrderRuleL( TMdEOrderRule( namePropertyDef, ESortAscending ) );
    iQuery->AddPropertyFilterL( &namePropertyDef );
    iQuery->AddPropertyFilterL( &artistPropertyDef );
    
    
    // add other condition: get songs that do not have album attribute
    CMdELogicCondition& albumCondition = 
        conditions.AddLogicConditionL( ELogicConditionOperatorAnd );
    albumCondition.AddPropertyConditionL( albumPropertyDef );
    albumCondition.SetNegate( ETrue );

    iQuery->FindL();
    }
*/

// -----------------------------------------------------------------------------
// CMediaFileHandler2::QueryAlbumsL (obsolete)
// 
// Finds all albums for aArtist.
// -----------------------------------------------------------------------------
//
/*
void CMediaFileHandler::QueryAlbumsL( const TDesC& aArtist )
    {
    if ( !iSessionOpen )
        {
        return;
        }

    CMdENamespaceDef& defNS = iSession->GetDefaultNamespaceDefL();
    CMdEObjectDef& objectDef = defNS.GetObjectDefL( MdeConstants::Audio::KAudioObject );

    iQueryType = EQueryAudio;
    
    delete iQuery;
    iQuery = NULL;
    iQuery = iSession->NewObjectQueryL( defNS, objectDef, this );
    
    iQuery->SetResultMode( EQueryResultModeDistinctValues );
    
    CMdEPropertyDef& propertyDef = PropertyDefL( EAttrAlbum );
    
    // distinct query requires exactly one property filter
    CMdELogicCondition& conditions = iQuery->Conditions();
    conditions.AddPropertyConditionL( propertyDef );
    iQuery->AppendOrderRuleL( TMdEOrderRule( propertyDef, ESortAscending ) );
    iQuery->AddPropertyFilterL( &propertyDef );
    
    CMdEPropertyDef& propertyDef2 = PropertyDefL( EAttrArtist );
    conditions.AddPropertyConditionL( propertyDef2, 
                            ETextPropertyConditionCompareEquals, aArtist );
        
    iQuery->FindL();
    }
*/

// -----------------------------------------------------------------------------
// CMediaFileHandler::QueryArtistAlbumL (obsolete)
// 
// Finds songs for aArtist/aAlbum.
// -----------------------------------------------------------------------------
//

/*
void CMediaFileHandler::QueryArtistAlbumL( const TDesC& aArtist, const TDesC& aAlbum )
    {
    if ( !iSessionOpen )
        {
        return;
        }
    
    CMdENamespaceDef& defNS = iSession->GetDefaultNamespaceDefL();
    CMdEObjectDef& objectDef = defNS.GetObjectDefL( MdeConstants::Audio::KAudioObject );

    iQueryType = EQueryAudio;

    delete iQuery;
    iQuery = NULL;
    iQuery = iSession->NewObjectQueryL( defNS, objectDef, this );
    
    iQuery->SetResultMode( EQueryResultModeItem );
    
    CMdEPropertyDef& artistDef = PropertyDefL( EAttrArtist );
    CMdEPropertyDef& albumDef = PropertyDefL( EAttrAlbum );
    CMdEPropertyDef& nameDef = PropertyDefL( EAttrSongName );
    
    CMdELogicCondition& conditions = iQuery->Conditions();
    
    conditions.AddPropertyConditionL( artistDef, 
                            ETextPropertyConditionCompareEquals, aArtist );

    conditions.AddPropertyConditionL( albumDef, 
                            ETextPropertyConditionCompareEquals, aAlbum );
        
    iQuery->AppendOrderRuleL( TMdEOrderRule( nameDef, ESortAscending ) );
    iQuery->AddPropertyFilterL( &nameDef );
   
    
    User::Leave(KErrNoMemory);
    iQuery->FindL();
    
    
    }
*/


// -----------------------------------------------------------------------------
// CMediaFileHandler::QueryL (test code)
// 
// -----------------------------------------------------------------------------
//
/*
void CMediaFileHandler::QueryL( TInt aAttr )
    {
    if ( aAttr != EAttrArtist && aAttr != EAttrAlbum &&
         aAttr != EAttrGenre && aAttr != EAttrComposer )
        {
        User::Leave( KErrNotSupported );
        }
   

    CMdENamespaceDef& defNS = iSession->GetDefaultNamespaceDefL();
    CMdEObjectDef& objectDef = defNS.GetObjectDefL( MdeConstants::Audio::KAudioObject );
    iQuery = iSession->NewObjectQueryL( defNS, objectDef, this );
    
    
    RArray<TItemId> itemIds;
    itemIds.Append( 22 );
    itemIds.Append( 5 );
    itemIds.Append( 18 );
    //itemLogicCondition->AddObjectConditionL(itemIds);
            
    CMdELogicCondition& conditions = iQuery->Conditions();
    conditions.AddObjectConditionL( itemIds );
    iQuery->SetResultMode( EQueryResultModeItem );
    
    itemIds.Close();
    
    iQuery->FindL();
    }
*/

// -----------------------------------------------------------------------------
// CMediaFileHandler::QueryL (test code)
// 
// -----------------------------------------------------------------------------
//

/*
void CMediaFileHandler::QueryL()
    {
    if ( !iSession )
        {
        User::Leave( KErrSessionClosed );
        }
   
    delete iQuery;
    iQuery = NULL;
    
    CMdENamespaceDef& defNS = iSession->GetDefaultNamespaceDefL();
    
    CMdEObjectDef& objectDef = defNS.GetObjectDefL( MdeConstants::Audio::KAudioObject );
    iQuery = iSession->NewObjectQueryL( defNS, objectDef, this );
    iQuery->SetResultMode( EQueryResultModeId );
    iQuery->FindL();
    
    
    CMdELogicCondition& conditions = iQuery->Conditions();
    
    //conditions.AddPropertyConditionL(composerPropertyDef, 
    //CMdETextPropertyCondition::ECompareEquals, text);

    }
*/


// -----------------------------------------------------------------------------
// CMediaFileHandler::HandleQueryCompletedOneL
// 
// -----------------------------------------------------------------------------
//

/*
void CMediaFileHandler::HandleQueryCompletedOneL( CMdEQuery& aQuery, 
                                                  TInt aError )
    {
    CMdENamespaceDef& defNS = iSession->GetDefaultNamespaceDefL();
    
    CMdEObjectDef& objectDef      = defNS.GetObjectDefL( 
                                    MdeConstants::Object::KBaseObject );
    CMdEObjectDef& mediaObjectDef = defNS.GetObjectDefL( 
                                    MdeConstants::MediaObject::KMediaObject );
    CMdEObjectDef& audioObjectDef = defNS.GetObjectDefL( 
                                    MdeConstants::Audio::KAudioObject );
    
    
    CMdEPropertyDef& songPropertyDef   = objectDef.GetPropertyDefL( 
                                         MdeConstants::Object::KTitleProperty );
    CMdEPropertyDef& artistPropertyDef = mediaObjectDef.GetPropertyDefL( 
                                         MdeConstants::MediaObject::KArtistProperty );
    CMdEPropertyDef& albumPropertyDef  = audioObjectDef.GetPropertyDefL( 
                                         MdeConstants::Audio::KAlbumProperty );
    CMdEPropertyDef& genrePropertyDef  = mediaObjectDef.GetPropertyDefL( 
                                         MdeConstants::MediaObject::KGenreProperty );

    CMdEProperty* property = NULL;
    
    const RArray<TItemId>& arr = iQuery->ResultIds();
    
    TBuf<256> buf;
    TInt count = arr.Count();
    for ( TInt i=0; i<count; i++ )
        {
        TItemId id = arr[i];
        CMdEObject* object = iSession->GetFullObjectL( id );
        CleanupStack::PushL( object );
        
        buf = object->Uri();
        TInt propertyCount = object->PropertyCount();
        for ( TInt j=0; j<propertyCount; j++ )
            {
            CMdEProperty& pr = object->PropertyL( j );
            const CMdEPropertyDef& prDef = pr.Def();
            buf.Append(_L(" "));
            buf.Append(prDef.Name());
            }
        TMFDialogUtil::ShowInformationQueryL( buf );
        
        CMdEPropertyDef& tmpDef = object->Def().GetPropertyDefL( MdeConstants::MediaObject::KArtistProperty );
        TInt ret = object->Property( tmpDef, property, 0 );
        if ( ret != KErrNotFound )
            {
            buf = property->TextValueL();    
            }

        
        ret = object->Property( songPropertyDef, property, 0 );
        if ( ret != KErrNotFound )
            {
            buf = property->TextValueL();    
            }

        ret = object->Property( artistPropertyDef, property, 0 );
        if ( ret != KErrNotFound )
            {
            buf = property->TextValueL();    
            }
        
        ret = object->Property( albumPropertyDef, property, 0 );
        if ( ret != KErrNotFound )
            {
            buf = property->TextValueL();    
            }
            
        ret = object->Property( genrePropertyDef, property, 0 );
        if ( ret != KErrNotFound )
            {
            buf = property->TextValueL();    
            }
        

        CleanupStack::PopAndDestroy( object );
        }
    }
*/

// -----------------------------------------------------------------------------
// CMediaFileHandler::HandleQueryCompletedTwoL
// 
// -----------------------------------------------------------------------------
//

/*
void CMediaFileHandler::HandleQueryCompletedTwoL( CMdEQuery& aQuery,
                                                  TInt aError )
    {
    TBuf<256> buf;
    
    TInt count = iQuery->Count();
    for ( TInt i=0; i<count; i++ )
        {
        buf = iQuery->ResultDistinctValue( i );
       
        CMdEObject& object = iQuery->Result( i );
        
        buf = object.Uri();
        TInt propertyCount = object.PropertyCount();
        for ( TInt j=0; j<propertyCount; j++ )
            {
            CMdEProperty& pr = object.PropertyL( j );
            const CMdEPropertyDef& prDef = pr.Def();
            buf.Append(_L(" "));
            buf.Append(prDef.Name());
            }
        TMFDialogUtil::ShowInformationQueryL( buf );
        
        CMdEProperty* property = NULL;
        CMdEPropertyDef& propDef = object.Def().GetPropertyDefL( MdeConstants::Audio::KAlbumProperty );
        TInt ret = object.Property( propDef, property, 0 );
        if ( ret != KErrNotFound )
            {
            buf = property->TextValueL();    
            }
        }
    }
*/


// -----------------------------------------------------------------------------
// CMediaFileHandler::ResultCount
// 
// -----------------------------------------------------------------------------
//

/*
TInt CMediaFileHandler::ResultCount()
    {
    if ( iQueryType == EQueryRomFile )
        {
        return iRomFileList.Count();
        }
         
    if ( iQueryType == ESearchVideo )
        {
        return iSearchList->Count();
        }

    if ( !iQuery || !iQueryReady || !iSessionOpen )
        {
        return 0;
        }
        
    TInt count = 0;
    TQueryResultMode resultMode = iQuery->ResultMode();
    
    if ( resultMode == EQueryResultModeDistinctValues ||
         resultMode == EQueryResultModeItem ||
         resultMode == EQueryResultModeCount )
        {
        count = iQuery->Count();
        }
    else if ( resultMode == EQueryResultModeId )
        {
        count = iQuery->Count();
        const RArray<TItemId>& arr = iQuery->ResultIds();
        count = arr.Count();
        }
    
    return count;
    }
*/    


// -----------------------------------------------------------------------------
// CMediaFileHandler::GetRecentFileAttribute
// 
// -----------------------------------------------------------------------------
//
/*
void CMediaFileHandler::GetRecentFileAttribute( TInt aIndex, TInt aAttr, TDes& aValue )
    {
    TInt count = iRecentList->Count();
    
    __ASSERT_ALWAYS( aIndex >= 0 && aIndex < count, Panic( KErrGeneral ) );
    
    if ( aAttr == EAttrFullName )
        {
        StrCopy( aValue, iRecentList->FileName( aIndex ) );
        }
    else if ( aAttr == EAttrSongName || aAttr == EAttrFileName )
        {
        TParsePtrC parse( iRecentList->FileName( aIndex ) );
        StrCopy( aValue, parse.Name() );
        }
    }
*/


// -----------------------------------------------------------------------------
// CMediaFileHandler::RecentFileAttribute
// 
// -----------------------------------------------------------------------------
//
/*
TInt CMediaFileHandler::RecentFileAttribute( TInt aIndex, TInt aAttr )
    {
    TInt count = iRecentList->Count();
    __ASSERT_ALWAYS( aIndex >= 0 && aIndex < count, Panic( KErrGeneral ) );
    
    TInt ret = KErrNotFound;
    
    if ( aAttr == EAttrMediaType )
        {
        ret = EAttrMusic;
        }
    if ( aAttr == EAttrStorageType )
        {
        ret = EPhoneMemory;
        }

    return ret;
    }
*/


// -----------------------------------------------------------------------------
// CMediaFileHandler::Attribute
// 
// -----------------------------------------------------------------------------
//
/*
TInt CMediaFileHandler::Attribute( TInt aIndex, TInt aAttr )
    {
    if ( iQueryType == EQueryRomFile )
        {
        return RomFileAttribute( aIndex, aAttr );
        }
    if ( iQueryType == ESearchVideo )
         {
         return iSearchList->Attribute( aIndex, aAttr );
         }
  
    if ( aAttr == EAttrMediaType )
        {
        if ( iQueryType == EQueryVideo )
            {
            return EAttrVideo;
            }
        else
            {
            return EAttrMusic;
            }
        }
    else if ( aAttr == EAttrStorageType )
         {
         TInt ret = CMediaFileHandler::EPhoneMemory;

         if ( !iQuery || !iQueryReady || !iSessionOpen )
              {
              return ret;
              }
         TQueryResultMode resultMode = iQuery->ResultMode();
         if ( resultMode != EQueryResultModeItem )
             {
             return ret;
             }
         
         CMdEObject& object = iQuery->Result( aIndex );
         if ( iDriveUtil->IsMemoryCard( object.Uri() ) )
             {
             ret = CMediaFileHandler::EMemoryCard;
             }
         else if ( iDriveUtil->IsMassStorage( object.Uri() ) )
             {
             ret = CMediaFileHandler::EMassStorage;
             }
         return ret;
         }
    
    else if ( aAttr == EAttrMediaFileId )
          {
          if ( !iQuery || !iQueryReady || !iSessionOpen )
              {
              return KErrNotFound;
              }
          CMdEObject& object = iQuery->Result( aIndex );
          return object.Id();
          }

    return KErrNotFound;
    }
*/


// -----------------------------------------------------------------------------
// CMediaFileHandler::GetAttributeL
// 
// -----------------------------------------------------------------------------
//
/*
void CMediaFileHandler::GetAttributeL( TInt aIndex, TInt aAttr, TDes& aValue )
    {
    aValue = KNullDesC;
    
    if ( iQueryType == EQueryRomFile )
        {
        GetRomFileAttribute( aIndex, aAttr, aValue );
        return;
        }
    if ( iQueryType == ESearchVideo )
        {
        if ( iSessionOpen )
            {
            iSearchList->SetSession( iSession );
            }
        else
            {
            iSearchList->SetSession( NULL );
            }
        iSearchList->GetAttributeL( aIndex, aAttr, aValue );
        return;
        }
    

    if ( !iQuery || !iQueryReady || !iSessionOpen )
        {
        return;
        }

    TInt count = ResultCount();
    __ASSERT_ALWAYS( aIndex >= 0 && aIndex < count, Panic( KErrGeneral ) );
    
    TQueryResultMode resultMode = iQuery->ResultMode();
    
    if ( resultMode == EQueryResultModeDistinctValues )
        {
        TPtrC ptr = iQuery->ResultDistinctValue( aIndex );
        StrCopy( aValue, ptr );
        }

    if ( resultMode == EQueryResultModeItem )
        {
        CMdEObject& object = iQuery->Result( aIndex );
        
        if ( aAttr == EAttrFullName )
            {
            StrCopy( aValue, object.Uri() );
            return;
            }
        
        CMdEPropertyDef& propDef = PropertyDefL( aAttr );
        CMdEProperty* property = NULL;
        TInt ret = object.Property( propDef, property, 0 );
        if ( ret != KErrNotFound )
            {
            StrCopy( aValue, property->TextValueL() );
            }
        else if ( aAttr == EAttrSongName || aAttr == EAttrFileName)
            {
            TParsePtrC parse( object.Uri() );
            StrCopy( aValue, parse.Name() );
            }
        }
    }
*/    


// -----------------------------------------------------------------------------
// CMediaFileHandler::UpdateRecentListL
//
// -----------------------------------------------------------------------------
//
/*
void CMediaFileHandler::UpdateRecentListL()
    {
    if ( !iSessionOpen )
        {
        return;
        }
    
    TInt count = iRecentList->Count();
    
    for ( TInt i=0; i<count; i++ )
        {
        TItemId id = iRecentList->Id( i );
        CMdEObject* object = NULL;
        
        TInt err = KErrNone;
        TRAP( err, object = iSession->GetFullObjectL( id ) );
        if ( err != KErrNone || !object )
            {
            iRecentList->SetPresent( EFalse, i );
            }
        else
            {
            CleanupStack::PushL( object );
            iRecentList->SetFileNameL( object->Uri(), i );
            CleanupStack::PopAndDestroy( object );
            }        
        }
        
    iRecentList->Clean();
    }
*/

// -----------------------------------------------------------------------------
// CMediaFileHandler::StoreRecentFileL
//
// -----------------------------------------------------------------------------
//
/*
void CMediaFileHandler::StoreRecentFileL( TInt aIndex )
    {
    if ( iQueryType == EQueryRomFile )
        {
        return;  // tones not part of recent files
        }
    
    if ( iQueryType == EQueryRecentFile )
        {
        TInt64 id = iRecentList->Id( aIndex );
        iRecentList->AddItemL( id );
        return;
        }
        
    if ( !iQuery || !iQueryReady || !iSessionOpen )
        {
        return;
        }

    TQueryResultMode resultMode = iQuery->ResultMode();
    if ( resultMode == EQueryResultModeItem )
        {
        CMdEObject& object = iQuery->Result( aIndex );
        TItemId id = object.Id();
        iRecentList->AddItemL( id );
        }
    }
*/


// -----------------------------------------------------------------------------
// CMediaFileHandler::HandleQueryCompletedL
//
// -----------------------------------------------------------------------------
//

/*
void CMediaFileHandler::HandleQueryCompletedL( CMdEQuery& aQuery, TInt aError )
    {
    iQueryReady = ETrue;
    
    if ( !iQuery )
        {
        iObserver->HandleMFEventL( MMediaFileHandlerObserver::EError, KErrCancel );
        return;  // query has been canceled
        }
    
    if ( aError != KErrNone )
        {
        iObserver->HandleMFEventL( MMediaFileHandlerObserver::EError, aError );
        return;
        }
    
    TQueryResultMode resultMode = iQuery->ResultMode();
    
    if ( resultMode == EQueryResultModeDistinctValues )
        {
        iObserver->HandleMFEventL( MMediaFileHandlerObserver::EQueryComplete, aError );
        }
    else if ( resultMode == EQueryResultModeId )
        {
        // const RArray<TItemId>& arr = iQuery->ResultIds();
        // CopyArrayL( iResultArray, arr );
        //
        // copy this arr into member iResultIds and perform full search
        // for the whole set or a partial search if result set is very large
        // partial search indexes could be in another member iPartialResultdIds
        // observer gets notified after the first partial search
        
        iObserver->HandleMFEventL( MMediaFileHandlerObserver::EError, KErrNotSupported );
        }
    else if ( resultMode == EQueryResultModeCount )
        {
        iObserver->HandleMFEventL( MMediaFileHandlerObserver::EUnknownQueryComplete, aError );
        }
    else if ( resultMode == EQueryResultModeItem )
        {
        if ( iQueryType == ESearchAudio )
            {
            iSearchList->Reset();
            iSearchList->TakeResultL( iQuery, EAttrMusic );
            SearchVideoL( iSearchText );
            }
        else if ( iQueryType == ESearchVideo )
            {
            iSearchList->TakeResultL( iQuery, EAttrVideo );
            iSearchList->Sort();
            iObserver->HandleMFEventL( MMediaFileHandlerObserver::EQueryComplete, aError );
            }
        else
            {
            iObserver->HandleMFEventL( MMediaFileHandlerObserver::EQueryComplete, aError );
            }
        }
    else
        {
        iObserver->HandleMFEventL( MMediaFileHandlerObserver::EError, KErrNotSupported );
        }
    }
*/


/*
    //private:
        //void ReadDirectoryL( const TDesC& aDirectory );
        //void ReadRomFilesL();
        //void SortRomFiles();
        //static TInt CompareFileName( const TFileName& aFirst, const TFileName& aSecond );
        //static TInt CompareStr( const TFileName& aFirst, const TFileName& aSecond );
  
// -----------------------------------------------------------------------------
// CMediaFileHandler::ReadRomFilesL (to be removed)
//
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::ReadRomFilesL()
    {
    _LIT( KFormat, "%S%S" );
    
    iRomFileList.Reset();
    
    TFileName fileName;
    fileName.Format( KFormat, &PathInfo::RomRootPath(), &PathInfo::SoundsPath() );
    TRAP_IGNORE( ReadDirectoryL( fileName ) );
    
    fileName.Format( KFormat, &PathInfo::RomRootPath(), &PathInfo::DigitalSoundsPath() );
    TRAP_IGNORE( ReadDirectoryL( fileName ) );

    fileName.Format( KFormat, &PathInfo::RomRootPath(), &PathInfo::SimpleSoundsPath() );
    TRAP_IGNORE( ReadDirectoryL( fileName ) );

    TInt count = iRomFileList.Count();
    for ( TInt i=0; i<count; i++ )
        {
        fileName = iRomFileList[i];
        }
        
    SortRomFiles();
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::ReadDirectoryL (to be removed)
//
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::ReadDirectoryL( const TDesC& aDirectory )
    {
    _LIT( KFormat, "%S%S" );
    
    TBuf<128> buf( aDirectory );
    
    CDir* dir = NULL;
    RFs fsSession;
    User::LeaveIfError( fsSession.Connect() ); 
    CleanupClosePushL( fsSession );
    
    if ( !BaflUtils::FolderExists( fsSession, aDirectory ) )
        {
        User::Leave( KErrNotFound );
        }
    
    TInt err = fsSession.GetDir( aDirectory, KEntryAttNormal, ESortByName, dir );
    User::LeaveIfError( err );
    CleanupStack::PushL( dir );
    
    TInt count = dir->Count();
    
    for ( TInt i=0; i<count; i++ )
        {
        const TEntry item = (*dir)[i];
        TFileName fullName;
        fullName.Format( KFormat, &aDirectory, &item.iName );
        iRomFileList.Append( fullName );
        }

    CleanupStack::PopAndDestroy( dir );

    CleanupStack::PopAndDestroy(); // fsSession
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::SortRomFiles (to be removed)
//
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::SortRomFiles()
    {
    iRomFileList.Sort(TLinearOrder<TFileName>(CMediaFileHandler::CompareFileName));
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::QueryRomFilesL (to be removed)
// 
// Finds all tones from rom directories.
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::QueryRomFilesL( TInt aAttr )
    {
    QueryEmptyL();
    
    iQueryType = EQueryRomFile;
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::GetRomFileAttribute (to be removed)
// 
// -----------------------------------------------------------------------------
//
void CMediaFileHandler::GetRomFileAttribute( TInt aIndex, TInt aAttr, TDes& aValue )
    {
    TInt count = iRomFileList.Count();
    
    __ASSERT_ALWAYS( aIndex >= 0 && aIndex < count, Panic( KErrGeneral ) );
    
    if ( aAttr == EAttrFullName )
        {
        StrCopy( aValue, iRomFileList[aIndex] );
        }
    else if ( aAttr == EAttrSongName || aAttr == EAttrFileName )
        {
        TParsePtrC parse( iRomFileList[aIndex] );
        StrCopy( aValue, parse.Name() );
        }
    }


// -----------------------------------------------------------------------------
// CMediaFileHandler::RomFileAttribute (to be removed)
// 
// -----------------------------------------------------------------------------
//
TInt CMediaFileHandler::RomFileAttribute( TInt aIndex, TInt aAttr )
    {
    TInt count = iRomFileList.Count();
    
    __ASSERT_ALWAYS( aIndex >= 0 && aIndex < count, Panic( KErrGeneral ) );
    
    TInt ret = KErrNotFound;
    if ( aAttr == EAttrMediaType )
        {
        ret = EMediaTypeTone;
        }
    else if ( aAttr == EAttrStorageType )
        {
        ret = EPhoneMemory;
        }
    
    return ret;
    }
*/

// -----------------------------------------------------------------------------
// CMediaFileHandler::CompareFileName
//
// -----------------------------------------------------------------------------
//
/*
TInt CMediaFileHandler::CompareFileName(const TFileName& aFirst, const TFileName& aSecond)
    {
    TParsePtrC parseFirst( aFirst );
    TParsePtrC parseSecond( aSecond );
    
    return parseFirst.Name().Compare( parseSecond.Name() );
    }
*/

// -----------------------------------------------------------------------------
// CMediaFileHandler::CompareStr
//
// -----------------------------------------------------------------------------
//
/*
TInt CMediaFileHandler::CompareStr(const TFileName& aFirst, const TFileName& aSecond)
    {
    return aFirst.Compare( aSecond );
    }
*/



/*******************************************************************************
 * class TMediaFileHistoryItem
 *******************************************************************************/

/*
// -----------------------------------------------------------------------------
// TMediaFileHistoryItem::Id
//
// -----------------------------------------------------------------------------
//
TInt64 TMediaFileHistoryItem::Id()
    {
    return iId;
    }


// -----------------------------------------------------------------------------
// TMediaFileHistoryItem::SetId
//
// -----------------------------------------------------------------------------
//
void TMediaFileHistoryItem::SetId( TInt64 aId )
    {
    iId = aId;
    }


// -----------------------------------------------------------------------------
// TMediaFileHistoryItem::Time
//
// -----------------------------------------------------------------------------
//
TTime TMediaFileHistoryItem::Time()
    {
    return iTime;
    }


// -----------------------------------------------------------------------------
// TMediaFileHistoryItem::SetTime
//
// -----------------------------------------------------------------------------
//
void TMediaFileHistoryItem::SetTime( TTime aTime )
    {
    iTime = aTime;
    }


// -----------------------------------------------------------------------------
// TMediaFileHistoryItem::InternalizeL
//
// -----------------------------------------------------------------------------
//
void TMediaFileHistoryItem::InternalizeL( RReadStream& aStream )
    {
    aStream >> iId;

    if ( iId < 0 )
        {
        User::Leave( KErrCorrupt );
        }

    TInt64 temp = 0;
    aStream >> temp;
    
    if ( temp <= 0 )
        {
        User::Leave( KErrCorrupt );
        }
    TTime time( temp );
    iTime = time;
    
    iPresent = ETrue;
    iFullName = KNullDesC;
    }


// -----------------------------------------------------------------------------
// TMediaFileHistoryItem::ExternalizeL
//
// -----------------------------------------------------------------------------
//
void TMediaFileHistoryItem::ExternalizeL( RWriteStream& aStream )
    {
    aStream << iId;
    aStream << iTime.Int64();
    }
*/



/*******************************************************************************
 * class CMediaFileHistory
 *******************************************************************************/

/*
// -----------------------------------------------------------------------------
// CMediaFileHistory::NewL
//
// -----------------------------------------------------------------------------
//
CMediaFileHistory* CMediaFileHistory::NewL()
    {
    CMediaFileHistory* self = new (ELeave) CMediaFileHistory();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self );

    return self;
    }


// -----------------------------------------------------------------------------
// CMediaFileHistory::CMediaFileHistory
// 
// -----------------------------------------------------------------------------
//
CMediaFileHistory::CMediaFileHistory()
    {
    }


// -----------------------------------------------------------------------------
// CMediaFileHistory::ConstructL
//
// Note: Sessionpath in emulator is '\epoc32\winscw\c\private\2000F85A'
// -----------------------------------------------------------------------------
//
void CMediaFileHistory::ConstructL()
    {
    _LIT( KHistoryFile, "MediaFileHistory" );
    const TInt KHistoryDrive = EDriveC;
    
    User::LeaveIfError( iFsSession.Connect() );
    User::LeaveIfError( iFsSession.SetSessionToPrivate( KHistoryDrive ) );
    
    User::LeaveIfError( iFsSession.SessionPath( iPath ) );
    BaflUtils::EnsurePathExistsL( iFsSession, iPath );
    iPath.Append( KHistoryFile );
    
    TInt err = KErrNone;
    TRAP( err, ReadHistoryL() );
    
    if ( err != KErrNone && err != KErrNoMemory )
        {
        // history file was just created or it is corrupted
        iList.Reset();
        }
    } 


// ----------------------------------------------------------------------------
// Destructor
//
// ----------------------------------------------------------------------------
//
CMediaFileHistory::~CMediaFileHistory()
    {
    Clean();
    
    TRAP_IGNORE( WriteHistoryL() );
    
    iList.Close();
    iFsSession.Close();
    }
    

// -----------------------------------------------------------------------------
// CMediaFileHistory::AddItemL
//
// -----------------------------------------------------------------------------
//
void CMediaFileHistory::AddItemL( TInt64 aId )
    {
    TMediaFileHistoryItem item;
    
    TTime t;
    t.UniversalTime();
    
    item.SetTime( t );
    item.SetId( aId );
    
    AddItemL( item );
    }


// -----------------------------------------------------------------------------
// CMediaFileHistory::Count
//
// -----------------------------------------------------------------------------
//
TInt CMediaFileHistory::Count()
    {
    return iList.Count();
    }


// -----------------------------------------------------------------------------
// CMediaFileHistory::Reset
//
// -----------------------------------------------------------------------------
//
void CMediaFileHistory::Reset()
    {
    iList.Reset();
    }


// -----------------------------------------------------------------------------
// CMediaFileHistory::SetFileNameL
//
// -----------------------------------------------------------------------------
//
void CMediaFileHistory::SetFileNameL( const TDesC& aFullName, TInt aIndex )
    {
    if ( aIndex < 0 || aIndex >= iList.Count() )
        {
        TMFDialogUtil::Panic( KErrArgument );
        }
        
    TMediaFileHistoryItem& item = iList[aIndex];
    TMFDialogUtil::StrCopy( item.iFullName, aFullName );
    }
    
    
// -----------------------------------------------------------------------------
// CMediaFileHistory::FileName
//
// -----------------------------------------------------------------------------
//
const TDesC& CMediaFileHistory::FileName( TInt aIndex )
    {
    if ( aIndex < 0 || aIndex >= iList.Count() )
        {
        TMFDialogUtil::Panic( KErrArgument );
        }
        
    TMediaFileHistoryItem& item = iList[aIndex];
    return item.iFullName;
    }


// -----------------------------------------------------------------------------
// CMediaFileHistory::SetPresent
//
// -----------------------------------------------------------------------------
//
void CMediaFileHistory::SetPresent( TBool aPresent, TInt aIndex )
    {
    if ( aIndex < 0 || aIndex >= iList.Count() )
        {
        TMFDialogUtil::Panic( KErrArgument );
        }
        
    TMediaFileHistoryItem& item = iList[aIndex];
    item.iPresent = aPresent;
    }


// -----------------------------------------------------------------------------
// CMediaFileHistory::Id
//
// -----------------------------------------------------------------------------
//
TInt64 CMediaFileHistory::Id( TInt aIndex )
    {
    if ( aIndex < 0 || aIndex >= iList.Count() )
        {
        TMFDialogUtil::Panic( KErrArgument );
        }
        
    TMediaFileHistoryItem& item = iList[aIndex];
    return item.iId;

    }


// -----------------------------------------------------------------------------
// CMediaFileHistory::InternalizeL
//
// -----------------------------------------------------------------------------
//
void CMediaFileHistory::InternalizeL( RReadStream& aStream )
    {
    TInt count = aStream.ReadInt32L();
    if ( count < 0 || count > KMaxHistoryItemCount )
        {
        User::Leave( KErrCorrupt );
        }
    
    for ( TInt i=0; i<count; i++ )
        {
        TMediaFileHistoryItem item;
        TInt err = KErrNone;
        TRAP( err, item.InternalizeL( aStream ) );
        if ( err != KErrNone )
            {
            User::Leave( err );
            }
        iList.AppendL( item );
        }
    }


// -----------------------------------------------------------------------------
// CMediaFileHistory::ExternalizeL
//
// -----------------------------------------------------------------------------
//
void CMediaFileHistory::ExternalizeL(RWriteStream& aStream)
    {
    TInt count = iList.Count();
    aStream.WriteInt32L( count );
        
    for ( TInt i=0; i<count; i++ )
        {
        TMediaFileHistoryItem& item = iList[i];
        item.ExternalizeL( aStream );
        }
    }


// -----------------------------------------------------------------------------
// CMediaFileHistory::ReadHistoryL
//
// -----------------------------------------------------------------------------
//
void CMediaFileHistory::ReadHistoryL()
    {
    RFile file;
    TInt err = file.Open( iFsSession, iPath, EFileRead );
    if ( err == KErrNotFound )
        {
        err = file.Create( iFsSession, iPath, EFileRead );
        }
    User::LeaveIfError( err );
        
    CleanupClosePushL( file );
    
    RFileReadStream stream( file, 0 );
    CleanupClosePushL( stream );

    TRAP( err, InternalizeL( stream ) );
    User::LeaveIfError( err );

    CleanupStack::PopAndDestroy( &stream );
    CleanupStack::PopAndDestroy( &file );
    }



// -----------------------------------------------------------------------------
// CMediaFileHistory::WriteHistoryL
//
// -----------------------------------------------------------------------------
//
void CMediaFileHistory::WriteHistoryL()
    {
    RFile file;
    User::LeaveIfError( file.Replace( iFsSession, iPath, EFileWrite ) );
    CleanupClosePushL( file );
    
    RFileWriteStream stream( file, 0 );
    CleanupClosePushL( stream );

    TInt err = KErrNone;
    TRAP( err, ExternalizeL( stream ) );
    
    if ( err == KErrNone )
        {
        TRAP( err, stream.CommitL() );
        User::LeaveIfError( err );
        }

    CleanupStack::PopAndDestroy( &stream );
    CleanupStack::PopAndDestroy( &file );
    }


// -----------------------------------------------------------------------------
// CMediaFileHistory::Clean
//
// -----------------------------------------------------------------------------
//
void CMediaFileHistory::Clean()
    {
    Sort();
    
    for (;;)
        {
        TInt ret = RemoveOldItem();
        if ( !ret )
            {
            break;
            }
        }
    }


// -----------------------------------------------------------------------------
// CMediaFileHistory::RemoveOldItem
//
// -----------------------------------------------------------------------------
//
TBool CMediaFileHistory::RemoveOldItem()
    {
    TInt count = iList.Count();
    
    for ( TInt i=0; i<count; i++ )
        {
        TMediaFileHistoryItem& item= iList[i];
        if ( !item.iPresent )
            {
            iList.Remove( i );
            return ETrue;
            }
        }
    
    count = iList.Count();
    
    if ( count > KMaxHistoryItemCount )
        {
        iList.Remove( count - 1 );
        return ETrue;
        }
    
    return EFalse;
    }


// -----------------------------------------------------------------------------
// CMediaFileHistory::AddItemL
//
// -----------------------------------------------------------------------------
//
void CMediaFileHistory::AddItemL( TMediaFileHistoryItem aItem )
    {
    TInt index = ItemIndex( aItem.iId );
    if ( index != KErrNotFound )
        {
        iList.Remove(index);
        }

    User::LeaveIfError( iList.Append( aItem ) );
    }


// -----------------------------------------------------------------------------
// CMediaFileHistory::ItemIndex
//
// -----------------------------------------------------------------------------
//
TInt CMediaFileHistory::ItemIndex( TInt aId )
    {
    TInt count = iList.Count();
    
    for (TInt i=0; i<count; i++)
        {
        TMediaFileHistoryItem& item= iList[i];
        if ( item.iId == aId )
            {
            return i;
            }
        }
        
    return KErrNotFound;
    }


// -----------------------------------------------------------------------------
// CMediaFileHistory::Item
//
// -----------------------------------------------------------------------------
//
TMediaFileHistoryItem& CMediaFileHistory::Item( TInt aIndex )
    {
    if ( aIndex < 0 || aIndex >= iList.Count() )
        {
        TMFDialogUtil::Panic( KErrArgument );
        }

    return iList[aIndex];
    }


// -----------------------------------------------------------------------------
// CMediaFileHistory::Sort
//
// -----------------------------------------------------------------------------
//
void CMediaFileHistory::Sort()
    {
    iList.Sort( TLinearOrder<TMediaFileHistoryItem>(CMediaFileHistory::CompareItems) );
    }


// -----------------------------------------------------------------------------
// CMediaFileHistory::CompareItems
//
// -----------------------------------------------------------------------------
//
TInt CMediaFileHistory::CompareItems( const TMediaFileHistoryItem& aFirst, 
                                      const TMediaFileHistoryItem& aSecond )
    {
    if ( aFirst.iTime > aSecond.iTime )
        {
        return -1;
        }
    else if ( aFirst.iTime < aSecond.iTime )
        {
        return 1;
        }
    return 0;
    }


// -----------------------------------------------------------------------------
// CMediaFileHistory::GetTimeText
//
// -----------------------------------------------------------------------------
//
void CMediaFileHistory::GetTimeText( TDes& aText, TTime aTime )
    {
    _LIT( KFormat, "%02d.%02d.%04d %02d:%02d:%02d" );
    
    TDateTime dt = aTime.DateTime();
    aText.Format( KFormat, dt.Day()+1, dt.Month()+1, dt.Year(),  dt.Hour(), dt.Minute(), dt.Second() );
    }

*/

#endif  // #ifdef _DEBUG

//  End of File  

