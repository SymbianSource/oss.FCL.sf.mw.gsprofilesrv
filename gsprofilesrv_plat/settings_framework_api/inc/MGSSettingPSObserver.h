/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Observer interface for indicating publish and subscribe 
*				         setting changes
*
*/




#ifndef MGSSETTINGPSOBSERVER_H
#define MGSSETTINGPSOBSERVER_H

//  INCLUDES
#include <e32std.h>

// CLASS DECLARATION

/**
*  Observer interface for indicating setting changes
*
*  @lib GSTelPlugin
*  @since 3.1
*/
class MGSSettingPSObserver
    {
    public:  // New functions

        /**
        * Handler for setting changed event
        * @param aUid uid of setting        
        * @param aId id of setting
        * @param aStatus status of completed AO operation
        */
        virtual void HandleNotifyPSL( const TUid aUid, const TInt& aKey, 
                                      const TRequestStatus& aStatus ) = 0;
    };

#endif // MGSSETTINGPSOBSERVER_H

// End of File
