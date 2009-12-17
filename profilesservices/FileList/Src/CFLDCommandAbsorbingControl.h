/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Command absorber to eat all key presses.
*
*/

#ifndef __CFLDCOMMANDABSORBINGCONTROL_H
#define __CFLDCOMMANDABSORBINGCONTROL_H

// INTERNAL INCLUDES

// EXTERNAL INCLUDES
#include <coecntrl.h>


// FORWARD DECLARATIONS
class CEikAppUi;
class CEikButtonGroupContainer;

// CLASS DECLARATION
/**
 * Command absorber.
 *
 * Puts empty CBA on the screen and absorbs
 * all the key events.
 *
 * @lib FileList.lib
 * @since 2.1
 */
NONSHARABLE_CLASS( CFLDCommandAbsorbingControl ) : public CCoeControl
    {
    public:
    
      /**
         * Two-phased constructor.
         */
        static CFLDCommandAbsorbingControl* NewL();
        
        /**
         * Two-phased constructor.
         * Leaves object on the CleanupStack.
         */
        static CFLDCommandAbsorbingControl* NewLC();

        /**
         * Destructor.
         */
        virtual ~CFLDCommandAbsorbingControl();

    private:

        /**
         * Symbian OS constructor.
         */
        void ConstructL();


        /**
         * C++ default constructor.
         */
        CFLDCommandAbsorbingControl();

    public:  // From CCoeControl

       /**
        * Absorbing OfferkeyEvent() method.
        * Absorbs all offered key events (returns
        * always EKeyWasConsumed).
        */
        TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent,
                                     TEventCode aType );

    private:  // own helpers

		void Release();
		
       /**
        * Alters CBA visibility.
        *
        * @since 2.1
        * @param aVisible If ETrue makes buttons visible. Else
        * makes them invisible.
        */
        void MakeCbaVisible( TBool aVisible ) const;

    private: //data
    
		/// Ref: Application under what we are running
        CEikAppUi* iAppUi;  
        
		 /// Ref: Used CBA button container
        CEikButtonGroupContainer* iCba; 
    };

#endif      // __CFLDCOMMANDABSORBINGCONTROL_H
// End of File

