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
* Description:  Container for Barrings sub-menu in General Settings.
*
*/


#ifndef GSSETTLISTBARRINGCONTAINER_H
#define GSSETTLISTBARRINGCONTAINER_H

// INCLUDES
#include "gsbasecontainer.h"
#include "GSPhoneSettingConstants.h"

// CLASS DECLARATION

/**
* CGSSettListBarringContainer container class
*/
class CGSSettListBarringContainer : public CGSBaseContainer
    {
    public: // Constructors and destructor
        
        /**
        * Symbian OS default constructor. 
        *
        * @param aRect gives the correct TRect for construction.
        */
        void ConstructL( const TRect& aRect );

        /* Destructor */
        ~CGSSettListBarringContainer();

    public:  //from CGSBaseContainer

        /* 
        * Creates list box object.
        * @param aResLbxId resource list id to create list box.
        */
        void ConstructListBoxL( TInt aResLbxId );

    private: //new

        /*
        * Creates items for the list box.
        */
        void CreateListBoxItemsL();

        /**
        * Required for help.
        */
        void GetHelpContext( TCoeHelpContext& aContext ) const;

    private: //member variables
        //Resource texts for setting items
        CDesCArrayFlat*         iItems;
        //Setting items
        CDesCArray*             iItemArray;
    };

#endif //GSSETTLISTBARRINGCONTAINER_H

// End of File
