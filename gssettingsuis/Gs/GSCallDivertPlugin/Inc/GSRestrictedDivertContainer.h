/*
* Copyright (c) 2003-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Container for Restricted Call Divert view.
*
*/


#ifndef GSDIVERTRESTRICTEDCONTAINER_H
#define GSDIVERTRESTRICTEDCONTAINER_H

// INCLUDES
#include "gsbasecontainer.h"

// CLASS DECLARATION
class CGSListBoxItemTextArray;

// CLASS DECLARATION
/**
*  CGSRestrictedDivertContainer container class
*/
class CGSRestrictedDivertContainer : public CGSBaseContainer
    {
    public: // Constructors and destructor

        /**
        * Symbian OS default constructor.
        *
        * @param aRect gives the size of container
        */
        void ConstructL( const TRect& aRect );

        /* Destructor */
        ~CGSRestrictedDivertContainer();

    private:

        void GetHelpContext( TCoeHelpContext& aContext ) const;

    public:
        /**
        * Updates the listbox items
        * @param aFeatureId: selected listbox item ID
        */
        void UpdateListBoxL( TInt aFeatureId );

        /**
        * Retrieves the feature id for the selected item in the listbox
        * @return listbox item array's current feature.
        */
        TInt CurrentFeatureId() const;


    protected:
        /* Creates listbox
        *   @param aResLbxId
        */
        void ConstructListBoxL( TInt aResLbxId );

    private:
        void CreateListBoxItemsL();
        void MakeRestrictedDivertItemL();

	private:
		CGSListBoxItemTextArray* iListboxItemArray;

    };

#endif //GSDIVERTRESTRICTEDCONTAINER_H

// End of File
