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
* Description:  Abstract interface for handling the master settings change events.
*
*/



#ifndef MPROENGMASTERSETTINGSOBSERVER_H
#define MPROENGMASTERSETTINGSOBSERVER_H

//  INCLUDES
#include <e32def.h>

// FORWARD DECLARATIONS
class MProEngMasterSettingsObserverExt;

// CLASS DECLARATION

/**
* master settings modification observer interface.
* Implement this interface to get notifications of changes in the master settings
* data.
*
*  @lib ProfileEngine.lib
*  @since Series 10.1
*/
class MProEngMasterSettingsObserver
    {
    public: // New functions

        /**
        * Abstract method for handling master settings modification event.
        * This method is called when the master settings has been
        * modified some way. 
        * 
        * @since Series 10.1
        */
        virtual void HandleMasterSettingsModifiedL() = 0;

        /**
         * Error handler for handling the errors in the master settings
         * modification notification process. If HandleMasterSettingsModifiedL()
         * leaves, this method is called with the leave code. This method has
         * default empty implementation.
         * @since 10.1
         * @param aError the code of the error which occurred during
         *        notification process.
         */
        virtual void HandleMasterSettingsNotificationError(
                TInt /* aError */ ) {};

    protected:

        /**
        * Destructor.
        */
        virtual ~MProEngMasterSettingsObserver() {}

    private: // Extension interface

        /**
        * This member is internal and not intended for use.
        */
        virtual MProEngMasterSettingsObserverExt* Extension() { return NULL; }

    };

#endif      // MPROENGMASTERSETTINGSOBSERVER_H

// End of File

