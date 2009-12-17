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


#ifndef GSCALLBARRINGPLUGINCONTAINER_H
#define GSCALLBARRINGPLUGINCONTAINER_H

// INCLUDES
#include "gsbasecontainer.h"
#include "GSPhoneSettingConstants.h"

// FORWARD DECLARATIOIN
class CGSListBoxItemTextArray;

// CLASS DECLARATION

/**
* CGSCallBarringPluginContainer container class
*/
class CGSCallBarringPluginContainer : public CGSBaseContainer
    {
    public: // Constructors and destructor
        
        /**
        * Symbian OS default constructor. 
        *
        * @param aRect gives the correct TRect for construction.
        */
        void ConstructL( const TRect& aRect );

        /* Destructor */
        ~CGSCallBarringPluginContainer();

    public:  //from CGSBaseContainer

        /* 
        * Creates list box object.
        * @param aResLbxId resource list id to create list box.
        */
        void ConstructListBoxL( TInt aResLbxId );

    private: //new

        /**
        * Required for help.
        */
        void GetHelpContext( TCoeHelpContext& aContext ) const;

    private: //member variables
        //listbox array
        CGSListBoxItemTextArray* iListboxItemArray;
        //Is VoIP supported (feature on, dynamic voip setting on)
        TBool iVoIPSupported;        
    };

#endif //GSCALLBARRINGPLUGINCONTAINER_H

// End of File
