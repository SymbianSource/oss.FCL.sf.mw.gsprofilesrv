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
* Description: Abstract interface for handling profile change notification
*
*/



#ifndef MPROENGNOTIFYHANDLER2_H
#define MPROENGNOTIFYHANDLER2_H

//  INCLUDES
#include <MProEngNotifyHandler.h>

// FORWARD DECLARATIONS
class MProEngNotifyHandler2Ext;
class MProEngActiveSettingsObserver;
class MProEngMasterSettingsObserver;


// CLASS DECLARATION

/**
* Profiles change notification handler.
* This class is used for registration to get notifications about changes in
* Profiles data.
*
*  @lib ProfileEngine.lib
*  @since S60 3.1
*/
class MProEngNotifyHandler2 : public MProEngNotifyHandler
    {
    public: // New functions
    
    
        /**
         * Request notifications of active profile settings. The notifications are
         * received as long as CancelActiveSettingsNotificationsL() or
         * CancelAll() is called. If the client wants to change the observer,
         * he should first cancel these notifications and then re-request them
         * with the new observer instance.
         * @since 10.1
         * @param aObserver Reference to the observer to be notified in the
         *        event of profile activation.
         * @return KErrNone if succesful, KErrAlreadyExists if this notification
         *         handler is already used for requesting profile activation
         *         notifications. In other cases another system wide error code.
         */
        virtual TInt RequestActiveSettingsNotificationsL( 
                MProEngActiveSettingsObserver &aObserver ) = 0;
        
        /**
         * Request notifications of master settings. The notifications are
         * received as long as CanceltMasterSettingsNotificationsL() or
         * CancelAll() is called. If the client wants to change the observer,
         * he should first cancel these notifications and then re-request them
         * with the new observer instance.
         * @since 10.1
         * @param aObserver Reference to the observer to be notified in the
         *        event of profile activation.
         * @return KErrNone if succesful, KErrAlreadyExists if this notification
         *         handler is already used for requesting profile activation
         *         notifications. In other cases another system wide error code.
         */
        virtual TInt RequestMasterSettingsNotificationsL( 
                MProEngMasterSettingsObserver &aObserver ) = 0;
        
        
        /**
         * Cancel the registration for receiving notifications about the active
         * settings modification events.
         * @since 10.1
         */
        virtual void CancelActiveSettingsNotificationsL() = 0;
        
        /**
         * Cancel the registration for receiving notifications about the master
         * settings modification events.
         * @since 10.1
         */
        virtual void CancelMasterSettingsNotificationsL() = 0;

    public:

        /**
        * Destructor.
        */
        virtual ~MProEngNotifyHandler2() {}

    private: // Extension interface

        /**
        * This member is internal and not intended for use.
        */
        virtual MProEngNotifyHandler2Ext* Extension2() { return NULL; }

    };

#endif      // MPROENGNOTIFYHANDLER2_H

// End of File

