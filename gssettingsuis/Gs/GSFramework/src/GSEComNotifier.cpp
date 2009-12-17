/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: 
*
*/


#include <gsplugininterface.h>
#include "GSEComNotifier.h"
#include "GsLogger.h"

// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
CGSEComNotifier::CGSEComNotifier( MGSEComObserver* aObserver,
                                  RImplInfoPtrArray aImplInfo )
    : CActive( EPriorityStandard ),
      iObserver( aObserver ),
      iOriginalImplInfo( aImplInfo )
    {
    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void CGSEComNotifier::ConstructL()
    {
    iEComSession = REComSession::OpenL();
    IssueRequest();
    }


// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C CGSEComNotifier* CGSEComNotifier::NewL( MGSEComObserver* aObserver,
                                                 RImplInfoPtrArray aImplInfo )
    {
    CGSEComNotifier* self = new( ELeave ) CGSEComNotifier(
        aObserver,
        aImplInfo );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C CGSEComNotifier::~CGSEComNotifier()
    {
    Cancel();
    iEComSession.Close();
    }


// ---------------------------------------------------------------------------
// See base class.
// ---------------------------------------------------------------------------
//
void CGSEComNotifier::DoCancel()
    {
    iEComSession.CancelNotifyOnChange( iStatus );
    }


// ---------------------------------------------------------------------------
// See base class.
// ---------------------------------------------------------------------------
//
void CGSEComNotifier::RunL()
    {
    __GSLOGSTRING1( "[CGSEComNotifier::RunL] iStatus:%d", iStatus );

    if( PluginCountDeltaL() < 0 )
        {
        iObserver->HandleEComEvent( MGSEComObserver::EPluginRemoved );
        }

    IssueRequest();

    __GSLOGSTRING1( "[CGSEComNotifier::RunL] iStatus:%d", iStatus );
    }


// ---------------------------------------------------------------------------
// See base class.
// ---------------------------------------------------------------------------
//
TInt CGSEComNotifier::RunError( TInt aError )
    {
    __GSLOGSTRING2(
        "[CGSEComNotifier::RunError] aError:%d iStatus:%d",
        aError, iStatus );

    // Special case: Application can throw KLeaveExit basically anytime so we
    // have to forward it upwards to app framework.
    if( aError == KLeaveExit )
        {
        return aError;
        }
    return KErrNone;
    }


// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void CGSEComNotifier::IssueRequest()
    {
    if( !IsActive() )
        {
        // NotifyOnChange is single shot.
        iEComSession.NotifyOnChange( iStatus );
        SetActive();
        }
    }


// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
TInt CGSEComNotifier::PluginCountDeltaL()
    {
    // Owned
    RImplInfoPtrArray newImplInfo;

    REComSession::ListImplementationsL(
        KGSPluginInterfaceUid,
        newImplInfo );

    TInt oldInfoCount = iOriginalImplInfo.Count();
    TInt newInfoCount = newImplInfo.Count();

    __GSLOGSTRING2(
        "[CGSEComNotifier::PluginCountDeltaL] iOriginalImplInfo:%d newImplInfo:%d",
        oldInfoCount, newInfoCount );

    newImplInfo.ResetAndDestroy();
    newImplInfo.Close();

    // Count the delta between old and current infos:
    return newInfoCount - oldInfoCount;
    }


// End of File
