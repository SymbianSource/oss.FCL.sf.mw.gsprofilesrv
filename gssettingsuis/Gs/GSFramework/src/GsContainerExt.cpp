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
* Description:  Container for GSParentPlugin.
*
*/

#include "GsContainerExt.h"

const TInt KInterval = 1000000; // 1 seconds

// ========================= MEMBER FUNCTIONS ================================
// -----------------------------------------------------------------------------
// CGsDoubleClickPreventer::CGsDoubleClickPreventer()
//
//
// -----------------------------------------------------------------------------
//
CGsDoubleClickPreventer::CGsDoubleClickPreventer()
                        : CTimer(CActive::EPriorityStandard)
    {
    }

// -----------------------------------------------------------------------------
// CGsDoubleClickPreventer::NewL()
//
//
// -----------------------------------------------------------------------------
//
EXPORT_C CGsDoubleClickPreventer* CGsDoubleClickPreventer::NewL()
    {
    CGsDoubleClickPreventer* self = new(ELeave) CGsDoubleClickPreventer();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CGsDoubleClickPreventer::ConstructL()
//
//
// -----------------------------------------------------------------------------
//
void CGsDoubleClickPreventer::ConstructL()
    {
    CTimer::ConstructL();
    CActiveScheduler::Add(this);    
    }

// -----------------------------------------------------------------------------
// CGsDoubleClickPreventer::Start()
//
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CGsDoubleClickPreventer::Start()
    {
    if(!IsActive())
        {
        After(KInterval);
        }
    }

// -----------------------------------------------------------------------------
// CGsContainerExt::NewL()
//
//
// -----------------------------------------------------------------------------
//
EXPORT_C CGsContainerExt* CGsContainerExt::NewL()
    {
    CGsContainerExt* self = new(ELeave) CGsContainerExt;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CGsContainerExt::ConstructL()
//
//
// -----------------------------------------------------------------------------
//
void CGsContainerExt::ConstructL()
    {
    iDblClickPreventer = CGsDoubleClickPreventer::NewL();
    }

