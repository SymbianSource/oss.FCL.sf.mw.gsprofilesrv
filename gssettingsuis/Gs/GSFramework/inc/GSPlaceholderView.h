/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Placeholder view during synchronous view activation
*
*/


#ifndef GSPLACEHOLDERVIEW_H
#define GSPLACEHOLDERVIEW_H

// INCLUDES
#include <e32base.h>
#include <vwsdef.h>
#include <coeview.h>

// CONSTANTS

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  Dummy view class used by CGSPluginAndViewIdCache view switching between two
*  actual views. See CGSPluginAndViewIdCache for details.
*
*  @since Series60_3.1
*
*/
class CGSPlaceholderView : public CBase, public MCoeView
    {
    public: // Constructor and destructor

        /**
        *
        */
        static CGSPlaceholderView* NewL( CCoeAppUi& aAppUi );

        /**
        * Destructor.
        */
        ~CGSPlaceholderView();

    private:

        /**
        * C++ constructor
        */
        CGSPlaceholderView( CCoeAppUi& aAppUi );

        /**
        * Second phase constructor
        */
        void ConstructL();

    public: // API

        /**
        *
        */
        void ActivateL();

        /**
        *
        */
        const TVwsViewId& PreviouslyActiveViewId() const;

        /**
        *
        */
        void SetPreviouslyActiveViewId( const TVwsViewId& aViewId );

    private: // From MCoeView
	    TVwsViewId ViewId() const;
	    void ViewActivatedL( const TVwsViewId& aPrevViewId, TUid aCustomMessageId, const TDesC8& aCustomMessage );
	    void ViewDeactivated();

    private: // Internal methods

    private: // Data
         CCoeAppUi& iAppUi;
         TVwsViewId iPreviouslyActiveViewId;
    };


#endif // GSPLACEHOLDERVIEW_H
// End of File
