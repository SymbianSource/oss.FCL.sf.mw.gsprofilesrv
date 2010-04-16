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
* Description:  Delegate notifications about master settings modifications
*               to the observer.
*
*/



#ifndef CPROENGMASTERSETTINGSEVENTDELEGATE_H
#define CPROENGMASTERSETTINGSEVENTDELEGATE_H

// INCLUDES
#include "CProEngCenRepObserverBase.h"

// FORWARD DECLARATIONS
class MProEngMasterSettingsObserver;

// CLASS DECLARATION

/**
* This class delegates notifications about master settings modifications to the observer
* implemented by the client of Profiles Engine Wrapper API. This is a helper
* class for CProEngNotifyHandlerImpl.
*
*  @lib ProfileEngine.lib
*  @since 10.1
*/
NONSHARABLE_CLASS( CProEngMasterSettingsEventDelegate )
    : public CProEngCenRepObserverBase
    {
    public:  // constructor and destructor

        static CProEngMasterSettingsEventDelegate* NewL(
                MProEngMasterSettingsObserver& aObserver );

        virtual ~CProEngMasterSettingsEventDelegate() {};

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

        CProEngMasterSettingsEventDelegate( TUint32 aPartialCenRepKey,
                                            TUint32 aKeyMask,
                                            MProEngMasterSettingsObserver& aObserver );

    private: // Data:

        // The client implemented observer for profile modification events:
        MProEngMasterSettingsObserver& iObserver;

    };

#endif      //  CPROENGMASTERSETTINGSEVENTDELEGATE_H

// End of File

