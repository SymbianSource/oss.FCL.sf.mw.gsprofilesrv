/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: 
*     MFLDFileProcessor defines an abstract interface for processing
*     files (e.g. playing sound files or displaying image files).
*
*
*/



#ifndef __MFLDFILEPROCESSOR_H__
#define __MFLDFILEPROCESSOR_H__

// INTERNAL INCLUDES

// EXTERNAL INCLUDES
#include <e32base.h>

// CLASS DEFINITION

// FORWARD DECLARATIONS
class RWindow;

/**
* MFLDFileProcessor defines an abstract interface for processing
* files (e.g. playing sound files or displaying image files).
*/
class MFLDFileProcessor
    {
    public:     // Constructors and destructors

        /**
        * Destructor.
        */
        virtual ~MFLDFileProcessor() {}

    public:     // New methods

        /**
        * Starts processing a file (e.g. playing a sound file)
        * @param aFileName The file name to be processed
        */
        virtual void ProcessFileL( const TDesC& aFileName, RWindow* aWindow ) = 0;

        /**
        * Cancels processing a file (e.g. stop playing a sound file)
        */
        virtual void Cancel() = 0;
        
         /**
        * Set the volume level on which the sound is played
        * @param aVolume The volume level
        */
        virtual void SetVolume( TInt aVolume ) = 0;

        /**
        * Sets the ringing type
        * @param aRingingType The ringing type (see enum TRingingTypes)
        */
        virtual void SetRingingType( TInt aRingingType ) = 0;

        /**
        * Sets vibrating alert on or off.
        * @param aVibra True: Vibra is on. False: Vibra is off.
        */
        virtual void SetVibra( TBool aVibra ) = 0;
        
          /**
        * Sets 3d-effects on or off.
        * @param a3dEffects True: 3dEffects are on. False: 3dEffects are off.
        */
        virtual void Set3dEffects( TBool a3dEffects ) = 0;

    };

#endif      //  __MFLDFILEPROCESSOR_H__

// End of File
