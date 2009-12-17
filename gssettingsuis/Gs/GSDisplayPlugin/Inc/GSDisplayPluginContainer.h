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


#ifndef GSDISPLAYPLUGINCONTAINER_H
#define GSDISPLAYPLUGINCONTAINER_H

// INCLUDES
#include "GSDisplayPluginModel.h"

#include <bldvariant.hrh>
#include <gsbasecontainer.h>

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CGSListBoxItemTextArray;
class CGulIcon;


// CLASS DECLARATION

/**
*  GSDisplayPluginContainer container class
*
*  container class for Device settings view
*  @lib GSDisplayPlugin.lib
*  @since Series 60_3.1
*/
class CGSDisplayPluginContainer : public CGSBaseContainer
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
        ~CGSDisplayPluginContainer();
        
        /**
        * Constructor
        */
        CGSDisplayPluginContainer();

    public: //new

        /**
        * Updates listbox's item's value.
        * @since Series 60_3.1
        * @param aItemId An item which is updated.
        */
        void UpdateListBoxL( TInt aFeatureId );
        
        /**
        * Retrieves the currently selected listbox feature id
        * @since Series 60_3.1
        * @return feature id.
        */
        TInt CurrentFeatureId() const;

        /**
        * Converts the value from Model's Avkon to setting page item index
        * @param aCurrentZoom value from TAknUiZoom in AknDef.hrh
        */
        void MapZoomLevelToUi( TInt& aCurrentZoom );

        /**
        * Converts the value from setting page item index to Model's Avkon value
        * @param aCurrentZoom value from TZoomLevel in CPP file for this header
        */
        void MapZoomLevelFromUi ( TInt& aCurrentZoom );
        
        /**
         * Return its member variable iMode
         * @param
         */
        CGSDisplayPluginModel* Model();
        void CloseDialog();

    protected: // from CGSBaseContainer
        void ConstructListBoxL( TInt aResLbxId );
        
    protected: // from CCoeControl
        /**
        * Handles skin changes to resources
        * @since Series 60_3.1
        * @param aType type of the event to handle
        */
        void HandleResourceChange( TInt aType );

    private: // new
        // create listbox from resource
        void CreateListBoxItemsL();
        // create contrast item
        void MakeContrastItemL();
        // create brightness item
        void MakeBrightnessItemL();
        // create screen saver time-out item
        void MakeSSPItemL();
        // create backlight timeout item
        void MakeBacklightItemL();
         // create ALS item
        void MakeAmbientLightSensorItemL();
        // create Display text size item
        void MakeDisplayTextSizeItemL();
        // create welcome note item
        void MakeWNIItemL();
        //PowerSave LED item
        void MakePowerSaveLedItemL();
        //Operator logo item
        void MakeOperatorLogoItemL();
        // create static bitmap for contrast and brightness
        void CreateBitmapL( TInt aValue, TInt aResourceId, TInt aIndex );
        
    private: // from CGSBaseContainer
        /**
        * Required for help.
        */
        void GetHelpContext( TCoeHelpContext& aContext ) const;
        
    private: // Data
        //display text size setting page items
        CDesCArrayFlat* iDispTxtSizeItems;
        //welcome note setting page items 
        CDesCArrayFlat* iWelcomeItems;
        //power save led setting page items
        CDesCArrayFlat* iLedItems;
        //operator logo on/off
        CDesCArrayFlat* iOperatorLogoItems;
        //icons for contrast & brightness
        CArrayPtr<CGulIcon>* iIconArray;
        //listbox item array
        CGSListBoxItemTextArray* iListboxItemArray;
        // plugin model
        CGSDisplayPluginModel* iModel;
        
    };

#endif //GSDISPLAYPLUGINCONTAINER_H

//End of File
