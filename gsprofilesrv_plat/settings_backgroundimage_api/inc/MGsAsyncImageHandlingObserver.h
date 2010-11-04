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
* Description:   
*   Observer class for asynchronous image handling.
*
*/



#ifndef MGS_ASYNC_IMAGE_HANDLING_OBSERVER_H
#define MGS_ASYNC_IMAGE_HANDLING_OBSERVER_H

//  INCLUDES
#include <e32base.h>

// CLASS DECLARATION
/**
*  Pure virtual observer class to notify events from CGSAsyncImageHandling.
*
*  @lib GsEngine.lib
*/
class MGSAsyncImageHandlingObserver
    {
    public: // New functions

        /**
        * Notifies that image handling is complete.
        * image handling involves loading, scaling and dithering
        * @param aError Error value if something went wrong.
        */
		virtual void ImageHandlingCompleteL( TInt aError ) = 0;
    };

#endif	// MGS_ASYNC_IMAGE_HANDLING_OBSERVER_H

// End of File