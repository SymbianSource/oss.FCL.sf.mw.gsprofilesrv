/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
*        GS plugin wrapper.
*
*/

#include "GSPluginWrapper.h"

// System includes
#include <ecom.h>


// ================= MEMBER FUNCTIONS =======================

// ----------------------------------------------------------------------------
// CGSPluginWrapper::CGSPluginWrapper
//
// 
// ----------------------------------------------------------------------------
//
CGSPluginWrapper::CGSPluginWrapper( CGSBaseDocument& aDocument )
:   iDocument( aDocument )
    {
    }


// ----------------------------------------------------------------------------
// CGSPluginWrapper::~CGSPluginWrapper
//
// 
// ----------------------------------------------------------------------------
//
EXPORT_C CGSPluginWrapper::~CGSPluginWrapper()
    {
    if  ( iDestructorIdKey != KNullUid )
        {
        REComSession::DestroyedImplementation( iDestructorIdKey );
        }
    }


// ----------------------------------------------------------------------------
// CGSPluginWrapper::SetDetails
//
// 
// ----------------------------------------------------------------------------
//
EXPORT_C void CGSPluginWrapper::SetDetails( TUid aDestructorIdKey )
    {
    iDestructorIdKey = aDestructorIdKey;
    }


//  End of File
