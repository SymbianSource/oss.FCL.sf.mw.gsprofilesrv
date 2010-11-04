/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Data query dialog for Call Divert view.
*
*/


#ifndef GSCALLDIVERTQUERY_H
#define GSCALLDIVERTQUERY_H

// INCLUDES
#include <AknQueryDialog.h>

// CLASS DECLARATION

/**
* GSCallDivertQuery is responsible for creating a data query dialog.
* This class basicly just switches the active softkeys.
*/
class CGSCallDivertQuery : public CAknTextQueryDialog
    {
    public:  // Constructors and destructor

        /* Two-phased constructor */
        static CGSCallDivertQuery* NewL( TDes& aDataText,
            const TTone& aTone,
            TInt aOKCba,
            TInt aDefaultCba );

        /* Destructor */
        ~CGSCallDivertQuery() {};

    public: // Functions from base class

        /**
        * From CAknQueryDialog, takes care of user input.
        *   @param  aKeyEvent which event
        *   @param  aType type of event
        */
        TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent,
            TEventCode aType );

        /**
        * From CAknQueryDialog, takes care of user input.
        *   @param  aKeyEvent which event
        *   @return boolean
        */
        TBool NeedToDismissQueryL( const TKeyEvent& aKeyEvent );

        /**
        * From CEikDialog, called after layout.
        */
        void PostLayoutDynInitL();

        /**
        * From CEikDialog, called before layout.
        */
        void PreLayoutDynInitL();

        /**
        * From CEikdialog, check if it is OK to exit.
        *   @param  aButtonId   id of pressed button
        */
        TBool OkToExitL( TInt aButtonId );

        /**
        * From CAknQueryDialog. Used to update data after stylus event.
        */
        TBool HandleQueryEditorStateEventL( CAknQueryControl* aQueryControl,
                                            TQueryControlEvent aEventType,
                                            TQueryValidationStatus aStatus );

    private:
        /* C++ default constructor */
        CGSCallDivertQuery(   TDes& aDataText,
                    const TTone& aTone,
                    TInt aOKCba,
                    TInt aDefaultCba );

        /**
        * Updates softkeys to Search-Cancel if required.
        */
        void UpdateSoftkeysL();

        /**
        * Changes softkeys as specified.
        *
        *   @param  aCba    new cba
        */
        void SetCbaL( TInt aCba );

    private:
        // OK cba, used when there is text in query.
        TInt        iOKCba;
        // Default cba, used when there is no text in query.
        const TInt  iDefaultCba;
        // Current cba.
        TInt        iCurrentCba;
        // About to exit the Query
        TInt        iLeaving;
    };

#endif
// End of File
