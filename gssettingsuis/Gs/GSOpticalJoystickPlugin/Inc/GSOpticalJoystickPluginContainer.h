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
* Description:  Container for Optical Joystick
*
*/


#ifndef GSOPTICALJOYSTICKPLUGINCONTAINER_H
#define GSOPTICALJOYSTICKPLUGINCONTAINER_H

// INCLUDES
#include <bldvariant.hrh>
#include <gsbasecontainer.h>

#include "gssettingid.h"

// FORWARD DECLARATION
class CGSListBoxItemTextArray;
class CRepository;

// CLASS DECLARATION

/**
*  CGSOpticalJoystickPluginContainer container class
*  @since Series 60_3.1
* 
*/
class CGSOpticalJoystickPluginContainer : public CGSBaseContainer
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
        ~CGSOpticalJoystickPluginContainer();

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
        * Set the status of optical joystick
        * @param aStatus Status of the optical joystick.
        */
        void GetOpticalJoystickStatus(TInt& aStatus);

        /**
        * Set the status of optical joystick
        * @param aStatus Status of the optical joystick to be set to.
        */
        void SetOpticalJoystickStatusL(TInt aStatus);

    protected: // from CGSBaseContainer
        void ConstructListBoxL( TInt aResLbxId );

    private: // new
        void CreateListBoxItemsL();
        void MakeFocusedItemContentL();
    
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
        //Repository to access optical joystick status
        CRepository* iPersonalizationRepository;
    };

#endif //GSOPTICALJOYSTICKPLUGINCONTAINER_H

// End of File
