/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Launches GS views.
*
*/

#include "GSLauncher.h"

#include <e32property.h>
#include <w32std.h>
#include <apgcli.h>
#include <viewcli.h>
#ifdef  SYMBIAN_ENABLE_SPLIT_HEADERS
#include <viewclipartner.h>
#endif

#include <gsfwviewuids.h>
#include <s32mem.h>


const TUid KGSMessageViewLaunch = { 0x1 };
const TInt KGSApaMsgLength = 1024;

// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// description_if_needed
// ---------------------------------------------------------------------------
//
CGSLauncher::CGSLauncher()
	{
	}


// ---------------------------------------------------------------------------
// description_if_needed
// ---------------------------------------------------------------------------
//
void CGSLauncher::ConstructL()
	{
	//code
	}
	

// ---------------------------------------------------------------------------
// description_if_needed
// ---------------------------------------------------------------------------
//
EXPORT_C CGSLauncher* CGSLauncher::NewL()
  {
  CGSLauncher* self = CGSLauncher::NewLC();
  CleanupStack::Pop( self );
  return self;
  }

// ---------------------------------------------------------------------------
// description_if_needed
// ---------------------------------------------------------------------------
//
EXPORT_C CGSLauncher* CGSLauncher::NewLC()
  {
  CGSLauncher* self = new( ELeave ) CGSLauncher;
  CleanupStack::PushL( self );
  self->ConstructL();
  return self;
  }


// ---------------------------------------------------------------------------
// description_if_needed
// ---------------------------------------------------------------------------
//
CGSLauncher::~CGSLauncher()
    {
    }

// ---------------------------------------------------------------------------
// implementation_description
// ---------------------------------------------------------------------------
//
EXPORT_C void CGSLauncher::LaunchGSViewL( const TUid aTarget, 
                                          const TUid aCustomMessageId,
                                          const TDesC8& aCustomMessage )
    {
	RProperty		prop;
	RWsSession 		ws;
	User::LeaveIfError(ws.Connect());
	CleanupClosePushL( ws );
	
	TApaTaskList 	l(ws);
	TApaTask 		gs = l.FindApp(KUidGS);
	
	// GS not running, launch via avkon
	if ( !gs.Exists() )
		{
		CVwsSessionWrapper* wsw = CVwsSessionWrapper::NewLC();
		const TVwsViewId viewId( KUidGS, aTarget );
		wsw->CreateActivateViewEvent( viewId,aCustomMessageId, aCustomMessage );
		CleanupStack::PopAndDestroy( wsw );
		}
	else
		{
		// GS running, send parameters in string
        // Pachage custom message
		CBufFlat* buffer = CBufFlat::NewL( KGSApaMsgLength ); // etc
		CleanupStack::PushL( buffer );
		RBufWriteStream stream( *buffer );
		stream << aTarget;
		// In case none plugin needs custom message currently, set it to NULL.
		stream << aCustomMessageId;
		stream << aCustomMessage;
//		stream << TUid::Uid(0);
//		stream << KNullDesC8;
		stream.CommitL();
		stream.Close();
		TPtr8 pBuffer( buffer->Ptr( 0 ) );

        gs.SendMessage( KGSMessageViewLaunch, pBuffer );
		CleanupStack::PopAndDestroy( buffer );
		gs.BringToForeground();
		}		
	CleanupStack::PopAndDestroy( &ws );
    }


