/*
* Copyright (c) 2002, 2003 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Container for Data call settings view.
*
*/


#ifndef GSDATACALLPLUGINCONTAINER_H
#define GSDATACALLPLUGINCONTAINER_H

// INCLUDES
#include <bldvariant.hrh>
#include <gsbasecontainer.h>

#include "gssettingid.h"

// FORWARD DECLARATION
class CGSListBoxItemTextArray;

// CLASS DECLARATION

/**
*  CGSDataCallPluginContainer container class
*  @since Series 60_3.1
* 
*/
class CGSDataCallPluginContainer : public CGSBaseContainer
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
        ~CGSDataCallPluginContainer();

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

    protected: // from CGSBaseContainer
        void ConstructListBoxL( TInt aResLbxId );

    private: // new
        void CreateListBoxItemsL();
        void MakeAutodisconItemL();
    
    private:
        /**
        * Required for help.
        * 
        */
        void GetHelpContext( TCoeHelpContext& aContext ) const;
    
    private: // data
        //Auto disconnect list item
        CDesCArrayFlat* iAutodisconValue;
        //Listbox item array model
        CGSListBoxItemTextArray* iListboxItemArray;
        
    };

#endif //GSDATACALLPLUGINCONTAINER_H

// End of File
