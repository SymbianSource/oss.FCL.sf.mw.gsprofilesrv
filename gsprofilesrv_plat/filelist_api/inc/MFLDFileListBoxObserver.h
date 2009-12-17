/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
*     MFLDFileListBoxObserver defines an abstract interface to observe
*     file list box events.
*
*/



#ifndef __MFLDFILELISTBOXOBSERVER_H__
#define __MFLDFILELISTBOXOBSERVER_H__

// INTERNAL INCLUDES

// EXTERNAL INCLUDES
#include <e32base.h>

//  FORWARD DECLARATIONS
class RWindow;

// CLASS DEFINITION

/**
* MFLDFileListBoxObserver defines an abstract interface to observe
* file list box events.
*/
class MFLDFileListBoxObserver
    {
    public:     // Enumerations
        enum TFileListBoxEvent
            {
            EFocusChanged = 1,
            EListBoxClosed,
            EOtherKeyEvent,
            EVideoPreview,
            EVideoPreviewSelected,
            EVideoPreviewCanceled
            };

		enum TFileListSoftKeyState
		    {
   		    EToneSelectionSoftKeyState = 1,
		    EPreviewSoftKeyState,
		    EPreviewSelectSoftKeyState
    		};

    protected:  // Constructors and destructors

        /**
        * Destructor.
        */
        virtual ~MFLDFileListBoxObserver() {}

    public:     // New methods

        /**
        * Receives information about a file list box event
        * @param aEvent The event type that happened
        * @param aFileName The file name that is currently focused, or KNullDesC
        * construct different components using existing window.
		*/
        virtual void HandleFileListBoxEventL( TFileListBoxEvent aEvent,
            const TDesC& aFileName = KNullDesC ) = 0;

        /**
        * Method for listbox to receive current state of the softkeys.
        * @param aSoftKeyState This parameter will return the softkey state
        * TFileListSoftKeyState as a return parameter
		*/
        virtual void HandleSoftKeyState( TFileListSoftKeyState& aSoftKeyState ) = 0;

    };

#endif      //  __MFLDFILELISTBOXOBSERVER_H__

// End of File
