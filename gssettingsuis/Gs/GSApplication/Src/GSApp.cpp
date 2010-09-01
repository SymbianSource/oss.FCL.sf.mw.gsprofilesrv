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
* Description:  General Settings Application
*
*/


// INCLUDE FILES
#include    <eikstart.h>
#include    "GSApp.h"
#include    "GSDocument.h"
#include    <gsfwviewuids.h>

// ========================= MEMBER FUNCTIONS ================================


// ---------------------------------------------------------------------------
// CGSApp::AppDllUid()
// Returns application UID
//
// ---------------------------------------------------------------------------
TUid CGSApp::AppDllUid() const
    {
    return KUidGS;
    }


// ---------------------------------------------------------------------------
// CGSApp::CreateDocumentL()
// Creates CGSDocument object
//
// ---------------------------------------------------------------------------
CApaDocument* CGSApp::CreateDocumentL()
    {
    return CGSDocument::NewL(*this);
    }


// ===================== OTHER EXPORTED FUNCTIONS ============================
//


// ---------------------------------------------------------------------------
// NewApplication()
// constructs CGSApp
// Returns: CApaDocument*: created application object
//
// ---------------------------------------------------------------------------
LOCAL_C CApaApplication* NewApplication()
    {
    return new CGSApp;
    }


// ---------------------------------------------------------------------------
// E32Main()
//
// ---------------------------------------------------------------------------
GLDEF_C TInt E32Main()
    {
    return EikStart::RunApplication( NewApplication );
    }


// End of File
