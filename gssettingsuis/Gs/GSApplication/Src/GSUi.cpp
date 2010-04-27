/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  General Settings User Interface
*
*/


// INCLUDE FILES
#include    <bldvariant.hrh>
#include    <eikapp.h>
#include    <avkon.hrh>
#include    <akncontext.h>
#include    <aknnotewrappers.h>
#include    <aknnavi.h>
#include    <aknnavide.h>
#include    <akntabgrp.h>
#include    <akntitle.h>
#include    <AknUtils.h>
#include    <AknsConstants.h> // For determining skin change
#include    <barsread.h>
#include    <StringLoader.h>
#include    <GSApp.rsg>
#include    <eikenv.h>
#include    <ConeResLoader.h>
#include    <apgcli.h>
#include    <f32file.h>
#include    <hlplch.h> // For HlpLauncher
#include    <w32std.h>
#include    <eikproc.h> // Embedding CC in GS
#include    <aknclearer.h> // Embedding CC in GS
#include    <apacln.h> // Embedding CC in GS
#include    <PSVariables.h>
#include    <e32std.h> // The USER class
#include    <featmgr.h>
#include    <s32mem.h>

#include    <gsplugininterface.h>
#include    <gsbaseview.h>
#include    <gsfwviewuids.h>

#include  <apgwgnam.h>
#include    "GsLogger.h"
#include    "GSUi.h"
#include    <gsmainview.h>
#include    "GSDocument.h"
#include    "gssettingid.h"
#include    <mgswatchdog.h>

// System Includes
#include  <e32property.h>
#include <sbdefs.h>
using namespace conn;

//CONSTANTS

const TInt KGSPhoneAppID = 0x100058b3;


// ================= MEMBER FUNCTIONS =======================


// ----------------------------------------------------
// CGSUi::CGSUi()
//
// Default constructor.
// ----------------------------------------------------
CGSUi::CGSUi()
    {
    }


// ----------------------------------------------------
// CGSUi::~CGSUi()
// destructor
//
// ----------------------------------------------------
CGSUi::~CGSUi()
    {
    GSDocument().PrepareForUIDestruction();

    FeatureManager::UnInitializeLib();

    //Embedding - removing created document
    if ( iEmbedded )
        {
        iEikonEnv->Process()->DestroyDocument( iEmbedded );
        }
    if( iEComNotifier )
        {
        delete iEComNotifier;
        }
    }


// ----------------------------------------------------
// CGSUi::ConstructL()
//
// Symbian OS two-phased constructor
// ----------------------------------------------------
void CGSUi::ConstructL()
    {
    __GSLOGSTRING( "[CGSUi] ConstructL()" );
    FeatureManager::InitializeLibL();
    BaseConstructL( EAknEnableSkin | EAknEnableMSK |EAknSingleClickCompatible );
    TInt backupInfo = 0;
  const TInt error = RProperty::Get( TUid::Uid( KUidSystemCategoryValue ),
                             KUidBackupRestoreKey,
                             backupInfo );
  // We check if the backup/restore process is underway
  if  ( error == KErrNone )
    {
        // Work out what kind of operation is underway:
    if ( ( backupInfo & KBURPartTypeMask ) == EBURNormal ||
         ( backupInfo & KBURPartTypeMask ) == EBURUnset )
      {
      ConstructViewsL();
      //Activate the main view to be the first view when
      // application is opened.
      SetDefaultViewL( *iMainView );
      }
    else
      {
      if ( ( backupInfo & KBURPartTypeMask) == EBURBackupFull ||
           ( backupInfo & KBURPartTypeMask) == EBURBackupPartial )
        {
            // Backup In Progress.. show a Note and Dont Launch GS
            ShowNoteGsNotOpenedDuringBackupRestoreL();
        }
      else if ( ( backupInfo & KBURPartTypeMask) == EBURRestoreFull ||
                ( backupInfo & KBURPartTypeMask) == EBURRestorePartial )
        {
            // Restore In Progress.. show a Note and Dont Launch GS
                    ShowNoteGsNotOpenedDuringBackupRestoreL();
        }
      }
    }

    GSDocument().PreparePluginCacheL( *this );

    // This will observe plugin install events.
    iEComNotifier = CGSEComNotifier::NewL( this, GSDocument().GetImplInfo() );
    }


// ----------------------------------------------------
// CGSUi::ConstructViewsL()
//
// ----------------------------------------------------
void CGSUi::ConstructViewsL()
    {
    __GSLOGSTRING( "[CGSUi] Constructing views..." );

    iMainView = CGSMainView::NewL();
    AddViewL( iMainView );         // transfer ownership to CAknViewAppUi

    __GSLOGSTRING( "[CGSUi] Constructing views done." );
    }


// ----------------------------------------------------
// CGSUi::HandleCommandL(TInt aCommand)
//
//
// ----------------------------------------------------
void CGSUi::HandleCommandL( TInt aCommand )
    {
    __GSLOGSTRING1( "[CGSUi] HandleCommandL(%d)", aCommand );
    switch ( aCommand )
        {
        case EEikCmdExit:
        case EAknCmdExit:
        case EAknSoftkeyExit:
        #ifdef GS_ENABLE_WATCH_DOG
            GSDocument().WatchDog()->ReportCleanExitL();
        #endif
            Exit();
            break;
        default:
            break;
        }
    }


// ----------------------------------------------------
// CGSUi::HandleResourceChangeL()
//
//
// ----------------------------------------------------
//
void CGSUi::HandleResourceChangeL( TInt aType )
    {
    CAknViewAppUi::HandleResourceChangeL( aType );
    }


// ----------------------------------------------------
// CGSUi::HandleKeyEventL()
//
//
// ----------------------------------------------------
TKeyResponse CGSUi::HandleKeyEventL( const TKeyEvent& aKeyEvent,
                                     TEventCode aType )
    {
    __GSLOGSTRING1( "[CGSUi] HandleKeyEventL(code:%d)", aKeyEvent.iCode );

    CAknNavigationControlContainer* naviContainer =
        static_cast<CAknNavigationControlContainer*>
        ( StatusPane()->ControlL( TUid::Uid(EEikStatusPaneUidNavi ) ) );

    TKeyResponse response = EKeyWasNotConsumed;

    if( naviContainer )
        {
        CAknNavigationDecorator* naviDec = naviContainer->Top();
        if( naviDec )
            {
            CCoeControl* decControl = naviDec->DecoratedControl();
            if( decControl)
                {
                response = decControl->OfferKeyEventL( aKeyEvent, aType );
                }
            }
        }

    return response;
    }


// ----------------------------------------------------
// CGSUi::PhoneIdle()
//
//
// ----------------------------------------------------
//
void CGSUi::PhoneIdle()
    {
    TRAP_IGNORE( CreateActivateViewEventL( TVwsViewId( TUid::Uid( KGSPhoneAppID ),
        TUid::Uid( KGSPhoneAppID ) ), KNullUid, KNullDesC8() ) );
    }


// ---------------------------------------------------------------------------
// CGSUi::EmbedAppL()
//
// Launches application as embedded.
// ---------------------------------------------------------------------------
//
void CGSUi::EmbedAppL( const TAppInfo& aApp )
    {
    if ( iEmbedded )
        {
        iEikonEnv->Process()->DestroyDocument( iEmbedded );
        iEmbedded = NULL;
        }
    CAknLocalScreenClearer* clearer = CAknLocalScreenClearer::NewLC();
    CEikProcess* process = iEikonEnv->Process();
    TApaApplicationFactory apaAppFactory( aApp.iUid );
    iEmbedded = process->AddNewDocumentL( apaAppFactory );
    TApaDocCleanupItem cleanup( process, iEmbedded );
    CleanupStack::PushL( cleanup );
    iEmbedded->NewDocumentL();
    CleanupStack::Pop();
    iEmbedded->EditL( NULL );
    CleanupStack::PopAndDestroy( clearer );
    }


// ---------------------------------------------------------------------------
// CGSUi::Document()
//
//
// ---------------------------------------------------------------------------
//
CGSDocument& CGSUi::GSDocument() const
    {
    CGSDocument* doc = static_cast< CGSDocument* >( Document() );
    return *doc;
    }


// ---------------------------------------------------------------------------
// CGSUi::ShowNoteGsNotOpenedDuringBackupRestoreL()
//
//
// ---------------------------------------------------------------------------
//
void CGSUi::ShowNoteGsNotOpenedDuringBackupRestoreL()
    {
    HBufC* prompt =
     StringLoader::LoadL( R_CONFIRM_NOTE_GS_EXIT_BACKUP_RESTORE_STRING );
    CleanupStack::PushL( prompt );
    CAknInformationNote* note =
                new( ELeave ) CAknInformationNote( ETrue );
    note->ExecuteLD( *prompt );
    CleanupStack::PopAndDestroy( prompt );
#ifdef GS_ENABLE_WATCH_DOG
    GSDocument().WatchDog()->ReportCleanExitL();
#endif
    Exit();
    }


// ---------------------------------------------------------------------------
// CGSUi::HandleMessageL
// Handles messages sent from gslauncher
//
// ---------------------------------------------------------------------------
//
MCoeMessageObserver::TMessageResponse CGSUi::HandleMessageL(
    TUint32 /*aClientHandleOfTargetWindowGroup*/,
    TUid /*aMessageUid*/,
    const TDesC8 &aMessageParameters )
  {
  const TUint8* ptr = aMessageParameters.Ptr();
  __GSLOGSTRING1( "[CGSUi::HandleMessageL] aMessageParameters: %S", &aMessageParameters );

  FindAndKillEmbeddedAppL();

  TUid viewUid;
  TUid uid2;

  RDesReadStream str;
  str.Open(aMessageParameters);
  str >> viewUid;
  str >> uid2;
  HBufC8* msg8 = HBufC8::NewLC(str, 1024);
  str.Close();

  __GSLOGSTRING1( "[CGSUi::HandleMessageL] Launching  view 0x%x ", viewUid.iUid );
  TVwsViewId id( KUidGS , viewUid );
  if ( !View(viewUid) )
	  {
	  GSDocument().CreateShimmedViewL(viewUid);
	  }
  ActivateViewL( id, uid2, *msg8 );
  CleanupStack::PopAndDestroy( msg8 );
  return MCoeMessageObserver::EMessageNotHandled;
  }


// ---------------------------------------------------------------------------
// CGSUi::FindAndKillEmbeddedAppL()
// Find possiple child app of GS, and kill it.
//
// ---------------------------------------------------------------------------
//
TInt CGSUi::FindAndKillEmbeddedAppL()
    {
    RWsSession ws  = CEikonEnv::Static()->WsSession();
    TApaTaskList taskList( ws );
    TApaTask gstask = taskList.FindApp( KUidGS  ); // Find GS

    if ( gstask.Exists() ) // Task should exist
        {
        TInt gswgid = gstask.WgId();
        RArray<RWsSession::TWindowGroupChainInfo> wgrp;
        ws.WindowGroupList( &wgrp );
        TInt i = 0;

        // Loop window groups, find GS's child app
        for ( i=0; i < wgrp.Count() ; ++i )
            {
            TBuf<100> name;
            ws.GetWindowGroupNameFromIdentifier( wgrp[i].iId , name );
            RWsSession::TWindowGroupChainInfo wginfo = wgrp[i];
            __GSLOGSTRING3( "[CGSUi] wginfo %d (%d) '%S'",  wginfo.iId, wginfo.iParentId, &( name.Left( 20 ) )  );
            if ( wginfo.iParentId == gswgid ) // Child of GS found
                {
                CApaWindowGroupName* windowName1 = CApaWindowGroupName::NewLC( ws, wginfo.iId );
                TUid embeddeeuid;
                embeddeeuid = windowName1->AppUid();
                TApaTask embeddee = taskList.FindApp( embeddeeuid );

                // Kill the child app
                if ( embeddee.Exists() )
                    {
                    __GSLOGSTRING2( "[CGSUi] Embeddee: 0x%x  Embeddee thread=0x%x",
                                    embeddeeuid.iUid,
                                    embeddee.ThreadId() );
                    embeddee.KillTask();
                    }
                CleanupStack::PopAndDestroy( windowName1 );  //windowName1
                }
            }
        wgrp.Close();
        }
    return KErrNone;
    }



// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void CGSUi::HandleEComEvent( TEComEvent aEvent )
    {
    /**
     * Quietly close GS if GS-plugin is uninstalled. This is a precaution for
     * scenario where GS-plugin is uninstalled and GS tries to access the
     * plugin even though plugin's resources do not exist anymore.
     *
     * More sophisticated solution would have been to update whole GS plugin
     * structure but this would have been overkill for this very rare use
     * scenario and rather impossible due to BC requirements.
     */
    __GSLOGSTRING1( "[CGSUi::HandleEComEvent] aEvent:%d", aEvent );

    switch( aEvent )
        {
        case MGSEComObserver::EPluginRemoved:
            __GSLOGSTRING( "[CGSUi::HandleAppListEvent] Closing GS..." );
            #ifdef GS_ENABLE_WATCH_DOG
                TRAP_IGNORE( GSDocument().WatchDog()->ReportCleanExitL() );
            #endif
            Exit();
            break;
        default:
            break;
        }

    }




//End of File
