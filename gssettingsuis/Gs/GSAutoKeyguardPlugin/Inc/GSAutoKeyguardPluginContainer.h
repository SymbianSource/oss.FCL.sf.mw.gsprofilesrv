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
* Description:  Container for the Display sub-folder
*
*/



#ifndef GSAUTOKEYGUARDCONTAINER_H
#define GSAUTOKEYGUARDCONTAINER_H

// INCLUDES
#include <bldvariant.hrh>
#include <e32property.h>

#include <gsbasecontainer.h>


// FORWARD DECLARATION
class CGSListBoxItemTextArray;
class CGSAutoKeyguardPluginModel;

// CLASS DECLARATION

/**
*  CGSAutoKeyguardPluginContainer container class
*
*  Container class for SIM security view
*  @lib GSAutoKeyguardPlugin.lib
*  @since Series 60_3.1
*/
class CGSAutoKeyguardPluginContainer : public CGSBaseContainer
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
        ~CGSAutoKeyguardPluginContainer();
        
        /**
        * Constructor
        */
        CGSAutoKeyguardPluginContainer( CGSAutoKeyguardPluginModel* aModel );

   public: // new
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
        
        void HandleResourceChangeL( TInt aType );

   protected: // from CGSBaseContainer
       /**
        * Constructs listbox.
        * @param aResLbxId Resource id for listbox.
        * 
        */
       void ConstructListBoxL( TInt aResLbxId );

   private: //new
        /**
        * Creates list box items
        */   
        void CreateListBoxItemsL();
        /**
        * Creates autokeyguard list box item
        */
        void MakeAutoKeyguardItemL();
    private:
        /**
        * Required for help.
        * 
        */
        void GetHelpContext( TCoeHelpContext& aContext ) const;
   
	private: //data
        //items from resource 
        CDesCArray* iItemArray;
        //GS listbox model
		CGSListBoxItemTextArray* iListboxItemArray;
        //Model pointer. Does not own it, so do not delete this pointer.
        CGSAutoKeyguardPluginModel* iModel;
        //Automatic Keyguard item's text
        CDesCArrayFlat* iAutoKeyguardItems;

    };

#endif //GSAUTOKEYGUARDCONTAINER_H

// End of File
