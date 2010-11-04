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
* Description:  gsIndicatorPlugin plugin implementation
*
*/



// INCLUDE FILES
#include "GSindicatorpluginimplementation.h"


#include <coemain.h> // CCoeEnv
#include <coeaui.h> // CCoeAppUi
#include <gsfwviewuids.h>
#include <data_caging_path_literals.hrh> // KDC_RESOURCE_FILES_DIR
#include <gslauncher.h>

// CONSTANTS


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CGSIndicatorPluginImplementation::CGSIndicatorPluginImplementation
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CGSIndicatorPluginImplementation::CGSIndicatorPluginImplementation()
    {
    }

// ---------------------------------------------------------------------------
// CGSIndicatorPluginImplementation::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CGSIndicatorPluginImplementation::ConstructL()
    {
    iTimer = CPeriodic::NewL(CActive::EPriorityStandard);
    }

// -----------------------------------------------------------------------------
// CGSIndicatorPluginImplementation::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CGSIndicatorPluginImplementation* CGSIndicatorPluginImplementation::NewL()
    {
    CGSIndicatorPluginImplementation* self = 
        new( ELeave ) CGSIndicatorPluginImplementation;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// Destructor
CGSIndicatorPluginImplementation::~CGSIndicatorPluginImplementation()
    {
    if ( iTimer )
    	{
    	iTimer->Cancel();
    	delete iTimer;
    	}
    }


// ---------------------------------------------------------------------------
// CGSIndicatorPluginImplementation::HandleIndicatorTapL
// ---------------------------------------------------------------------------
//
void CGSIndicatorPluginImplementation::HandleIndicatorTapL( const TInt /*aUid */)
    {
    if ( iTimer->IsActive() )
    	{
    	iTimer->Cancel();
    	}
    iTimer->Start(10, 10, TCallBack(Tick, this));
		// Launch connection view
//		CGSLauncher* l = CGSLauncher::NewLC();
//		l->LaunchGSViewL ( KGSConPluginUid, TUid::Uid(0), KNullDesC8 ); 
//    CleanupStack::PopAndDestroy(l);
    }

TInt CGSIndicatorPluginImplementation::Tick(TAny* aObject)
	{
	return static_cast<CGSIndicatorPluginImplementation*>(aObject)->DoTick();
	}

TInt CGSIndicatorPluginImplementation::DoTick()
	{
	if ( iTimer )
		{
		iTimer->Cancel();
		}

	// Launch connection view
	CGSLauncher* l = CGSLauncher::NewLC();
	TInt error = KErrNone;
	TRAP(error, l->LaunchGSViewL ( KGSConPluginUid, TUid::Uid(0), KNullDesC8 ));
	CleanupStack::PopAndDestroy(l);
	return error;
	}




//  End of File
