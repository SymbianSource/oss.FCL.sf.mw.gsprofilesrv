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
* Description:  Abstract interface for handling the active profile events.
*
*/


#ifndef MPROENGACTIVESETTINGSOBSERVER_H
#define MPROENGACTIVESETTINGSOBSERVER_H

//  INCLUDES
#include <e32def.h>

// FORWARD DECLARATIONS
class MProEngActiveSettingsObserverExt;

// CLASS DECLARATION

/**
* Active profile settings modification observer interface.
* Implement this interface to get notifications of changes in the active profile settings
* data.
*
*  @lib ProfileEngine.lib
*  @since  10.1
*/
class MProEngActiveSettingsObserver
    {
    public: // New functions

        /**
        * Abstract method for handling active profile settings modification event.
        * This method is called when the current active profile settings has been
        * modified some way. (e.g. switch active profile, change active profile)
        * 
        * @since Series 10.1
        */
        virtual void HandleActiveSettingsModifiedL() = 0;

        /**
         * Error handler for handling the errors in the active profile settings
         * modification notification process. If HandleActiveSettingsModifiedL()
         * leaves, this method is called with the leave code. This method has
         * default empty implementation.
         * @since 10.1
         * @param aError the code of the error which occurred during
         *        notification process.
         */
        virtual void HandleActiveSettingsNotificationError(
                TInt /* aError */ ) {};

    protected:

        /**
        * Destructor.
        */
        virtual ~MProEngActiveSettingsObserver() {}

    private: // Extension interface

        /**
        * This member is internal and not intended for use.
        */
        virtual MProEngActiveSettingsObserverExt* Extension() { return NULL; }

    };

#endif      // MPROENGACTIVESETTINGSOBSERVER_H

// End of File

