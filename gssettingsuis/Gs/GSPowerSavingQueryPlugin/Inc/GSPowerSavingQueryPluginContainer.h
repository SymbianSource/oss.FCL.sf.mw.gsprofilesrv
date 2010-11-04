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
* Description:  Container for Power saving query settings view.
*
*/


#ifndef GSPOWERSAVINGQUERYPLUGINCONTAINER_H
#define GSPOWERSAVINGQUERYPLUGINCONTAINER_H

// INCLUDES
#include <gsbasecontainer.h>

// FORWARD DECLARATION
class CGSListBoxItemTextArray;
class CGSPowerSavingQueryPluginModel;

// CLASS DECLARATION

/**
*  CGSPowerSavingQueryPluginContainer container class
*  @since Series 60_3.1
* 
*/
class CGSPowerSavingQueryPluginContainer : public CGSBaseContainer
    {
    public: // Constructors and destructor
        
        /**
        * Symbian OS constructor.
        * @param aRect Listbox's rect.
        * 
        */
        void ConstructL( const TRect& aRect );

        /**
        * Destructor.
        */
        ~CGSPowerSavingQueryPluginContainer();

    public: //new

        /**
        * Updates listbox's item's value.
        * @param aFeatureId An item which is updated.
        * 
        */
        void UpdateListBoxL( TInt aFeatureId );

        /**
        * Retrieves the currently selected listbox feature id
        * @return feature id.
        */
        TInt CurrentFeatureId() const;
        
        /**
        * Retrieves the power saving query plugin data model
        * @return the data model
        */
        CGSPowerSavingQueryPluginModel* Model();
        
    protected: // from CGSBaseContainer
        void ConstructListBoxL( TInt aResLbxId );

    private: // new
        void CreateListBoxItemsL();
        
        /*
         * MakePowerSavingQueryItemL
         */
        void MakePowerSavingQueryItemL();
    
    private: // data
        //Listbox item array model
        CGSListBoxItemTextArray* iListboxItemArray;
        //Data Model
        CGSPowerSavingQueryPluginModel* iModel;
    };

#endif //GSPOWERSAVINGQUERYPLUGINCONTAINER_H

// End of File
