/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Delegate notifications about active settings modifications
*               to the observer.
*
*/



#ifndef CPROENGACTIVESETTINGSEVENTDELEGATE_H
#define CPROENGACTIVESETTINGSEVENTDELEGATE_H

// INCLUDES
#include "CProEngCenRepObserverBase.h"

// FORWARD DECLARATIONS
class MProEngActiveSettingsObserver;

// CLASS DECLARATION

/**
* This class delegates notifications about active profile settings modifications to the observer
* implemented by the client of Profiles Engine Wrapper API. This is a helper
* class for CProEngNotifyHandlerImpl.
*
*  @lib ProfileEngine.lib
*  @since 10.1
*/
NONSHARABLE_CLASS( CProEngActiveSettingsEventDelegate )
    : public CProEngCenRepObserverBase
    {
    public:  // constructor and destructor

        static CProEngActiveSettingsEventDelegate* NewL(
                MProEngActiveSettingsObserver& aObserver );

        virtual ~CProEngActiveSettingsEventDelegate() {};

    public: // Functions from base classes

        /**
         * From CProEngObserverBase.
         */
        void NotifyObserverL();

        /**
         * From CProEngObserverBase.
         */
        void NotifyError( TInt aError );


    public: // New functions

    private:  // constructor

        CProEngActiveSettingsEventDelegate( TUint32 aPartialCenRepKey,
                                            TUint32 aKeyMask,
                                            MProEngActiveSettingsObserver& aObserver );

    private: // Data:

        // The client implemented observer for profile modification events:
        MProEngActiveSettingsObserver& iObserver;

    };

#endif      //  CPROENGACTIVESETTINGSEVENTDELEGATE_H

// End of File

