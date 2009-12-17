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
* Description: 
*     CGSApp is an application class required by Symbian OS architecture.
*
*/


#ifndef GSAPP_H
#define GSAPP_H

// INCLUDES
#include <aknapp.h>
#include <e32std.h>


// CONSTANTS

// CLASS DECLARATION

/**
* CGSApp application class.
* @since Series60_3.1
*/
class CGSApp : public CAknApplication
{
    private: // from CApaApplication

        /**
        * Create CGSDocument document object.
        */
        CApaDocument* CreateDocumentL();

        /**
        * Returns application UID.
        * @return KUidGS.
        */
        TUid AppDllUid() const;
};


#endif // GSAPP_H
// End of File
