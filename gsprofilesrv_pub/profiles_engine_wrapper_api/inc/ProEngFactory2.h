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
* Description: Factory for Profiles Engine Wrapper.
*
*/



#ifndef PROENGFACTORY2_H
#define PROENGFACTORY2_H

//  INCLUDES
#include <ProEngFactory.h>

// FORWARD DECLARATIONS

class MProEngNotifyHandler2;

// CLASS DECLARATION

/**
*  Factory for Profiles Engine Wrapper.
*  You can create new instances of Profiles Engines, Notification Handlers and
*  Alert Tone Seekers by using this factory.<br><br>
*  Example:
*  @code
*  // Create a new instance of Profiles Engine:
*  MProEngEngine* engine =
*      ProEngFactory2::NewEngineLC();
*
*  // Create a new instance of Notification Handler:
*  MProEngNotifyHandler* notifyHandler =
*      ProEngFactory2::NewNotifyHandlerL();
*
*  // Create a new instance of Alert Tone Seeker:
*  MProEngAlertToneSeeker* toneSeeker =
*      ProEngFactory2::NewAlertToneSeekerL();
*  @endcode
*
*  @lib ProfileEngine.lib
*  @since  10.1
*/
class ProEngFactory2 : public ProEngFactory
    {
    public: // New functions
        /**
        * Create a new instance of Profiles Notification Handler 2.
        * @since 10.1
        * @return New instance of Notification Handler.
        *         Ownership is transferred to the client application.
        */
        IMPORT_C static MProEngNotifyHandler2* NewNotifyHandler2L();

    private:

        // Prohibit C++ default constructor.
        ProEngFactory2();
        // Prohibit Destructor.
        ~ProEngFactory2();

    };

#endif      // PROENGFACTORY2_H

// End of File

