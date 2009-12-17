/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of the CProfileExtraTonesImpl.
*
*/



// INCLUDE FILES
#include "CProfileExtraTonesImpl.h"
#include "ProfilesVariant.hrh"
#include <s32strm.h>
#include <centralrepository.h>
#include "ProfileEngUtils.h"
#include "ProfileEnginePrivateCRKeys.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileExtraTonesImpl::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProfileExtraTonesImpl::ConstructL()
    {
    iIMMessageAlertTone = KNullDesC().AllocL();
    iEmailAlertTone = KNullDesC().AllocL();
    iVideoCallRingingTone = KNullDesC().AllocL();
    }

// -----------------------------------------------------------------------------
// CProfileExtraTonesImpl::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProfileExtraTonesImpl::ConstructL(
    const MProfileExtraTones& aProfileExtraTones )
    {
    iIMMessageAlertTone = aProfileExtraTones.IMMessageAlertTone().AllocL();
    iEmailAlertTone = aProfileExtraTones.EmailAlertTone().AllocL();
    iVideoCallRingingTone =
        aProfileExtraTones.VideoCallRingingTone().AllocL();
    }

// -----------------------------------------------------------------------------
// CProfileExtraTonesImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProfileExtraTonesImpl* CProfileExtraTonesImpl::NewL()
    {
    CProfileExtraTonesImpl* self = new( ELeave ) CProfileExtraTonesImpl;

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }

// -----------------------------------------------------------------------------
// CProfileExtraTonesImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProfileExtraTonesImpl* CProfileExtraTonesImpl::NewL(
    const MProfileExtraTones& aProfileExtraTones )
    {
    CProfileExtraTonesImpl* self = new( ELeave ) CProfileExtraTonesImpl;

    CleanupStack::PushL( self );
    self->ConstructL( aProfileExtraTones );
    CleanupStack::Pop();

    return self;
    }


// Destructor
CProfileExtraTonesImpl::~CProfileExtraTonesImpl()
    {
    delete iIMMessageAlertTone;
    delete iEmailAlertTone;
    delete iVideoCallRingingTone;
    }


// -----------------------------------------------------------------------------
// CProfileExtraTonesImpl::InternalizeL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileExtraTonesImpl::InternalizeL( CRepository& aCenRep,
                                           TInt aProfileId )
    {
    delete iIMMessageAlertTone;
    iIMMessageAlertTone = NULL;

    TFileName toneFile;

    // IM Message alert tone
    User::LeaveIfError(
            aCenRep.Get( ProfileEngUtils::ResolveKey( KProEngImTone,
            aProfileId ), toneFile ) );
    iIMMessageAlertTone = toneFile.AllocL();

    delete iEmailAlertTone;
    iEmailAlertTone = NULL;

    // E-mail alert tone
    User::LeaveIfError(
            aCenRep.Get( ProfileEngUtils::ResolveKey( KProEngEmailTone,
            aProfileId ), toneFile ) );
    iEmailAlertTone = toneFile.AllocL();

    delete iVideoCallRingingTone;
    iVideoCallRingingTone = NULL;

    // Video call ringing tone
    User::LeaveIfError(
            aCenRep.Get( ProfileEngUtils::ResolveKey( KProEngVideoCallTone,
            aProfileId ), toneFile ) );
    iVideoCallRingingTone = toneFile.AllocL();
    }

// -----------------------------------------------------------------------------
// CProfileExtraTonesImpl::ExternalizeL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileExtraTonesImpl::ExternalizeL( CRepository& aCenRep,
                                           TInt aProfileId ) const
    {
    // IM Message Alert Tone
    User::LeaveIfError(
            aCenRep.Set( ProfileEngUtils::ResolveKey( KProEngImTone,
            aProfileId ), *iIMMessageAlertTone ) );

    // E-mail Alert Tone
    User::LeaveIfError(
            aCenRep.Set( ProfileEngUtils::ResolveKey( KProEngEmailTone,
            aProfileId ), *iEmailAlertTone ) );

    // Video call ringing tone
    User::LeaveIfError(
            aCenRep.Set( ProfileEngUtils::ResolveKey( KProEngVideoCallTone,
            aProfileId ), *iVideoCallRingingTone ) );
    }

// -----------------------------------------------------------------------------
// CProfileExtraTonesImpl::IMMessageAlertTone
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TDesC& CProfileExtraTonesImpl::IMMessageAlertTone() const
    {
    return *iIMMessageAlertTone;
    }

// -----------------------------------------------------------------------------
// CProfileExtraTonesImpl::SetIMMessageAlertToneL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileExtraTonesImpl::SetIMMessageAlertToneL(
    const TDesC& aIMMessageAlertTone )
    {
    HBufC* tmp = aIMMessageAlertTone.Left( KMaxFileName ).AllocL();
    delete iIMMessageAlertTone;
    iIMMessageAlertTone = tmp;
    }

// -----------------------------------------------------------------------------
// CProfileExtraTonesImpl::EmailAlertTone
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TDesC& CProfileExtraTonesImpl::EmailAlertTone() const
    {
    return *iEmailAlertTone;
    }

// -----------------------------------------------------------------------------
// CProfileExtraTonesImpl::SetEmailAlertToneL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileExtraTonesImpl::SetEmailAlertToneL(
    const TDesC& aEmailAlertTone )
    {
    HBufC* tmp = aEmailAlertTone.Left( KMaxFileName ).AllocL();
    delete iEmailAlertTone;
    iEmailAlertTone = tmp;
    }

// -----------------------------------------------------------------------------
// CProfileExtraTonesImpl::VideoCallRingingTone
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TDesC& CProfileExtraTonesImpl::VideoCallRingingTone() const
    {
    return *iVideoCallRingingTone;
    }

// -----------------------------------------------------------------------------
// CProfileExtraTonesImpl::SetVideoCallRingingToneL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileExtraTonesImpl::SetVideoCallRingingToneL(
    const TDesC& aRingingTone )
    {
    HBufC* tmp = aRingingTone.Left( KMaxFileName ).AllocL();
    delete iVideoCallRingingTone;
    iVideoCallRingingTone = tmp;
    }

//  End of File
