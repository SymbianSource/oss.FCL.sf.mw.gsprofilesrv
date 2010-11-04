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
*     CFLDPlayerBase is used to play sound files. It takes into account
*     the current volume and ringing type settings.
*
*
*/


#ifndef __CFLDPLAYERBASE_H__
#define __CFLDPLAYERBASE_H__

// INTERNAL INCLUDES
#include "MFLDFileProcessor.h"

// EXTERNAL INCLUDES
#include <e32def.h>
#include <coemain.h>
#include <apgcli.h>
#include <cenrepnotifyhandler.h>

//	FORWARD DECLARATIONS
class C3DRingingToneInterface;

//  CONSTANTS
const TInt KFLDMinVolumeLevel( 1 );

//  CLASS DECLARATION

/**
* CFLDPlayerBase is used to play sound files. It takes into account the current
* volume and ringing type settings.
*
* @lib filelist.lib
* @since 2.1
*/
NONSHARABLE_CLASS( CFLDPlayerBase )
    : public CBase,
      public MFLDFileProcessor,
	  public MCenRepNotifyHandlerCallback,
      public MCoeForegroundObserver
    {
    protected:     // Destructor

        /**
         * Destructor
         */
        virtual ~CFLDPlayerBase();

    protected:    // Constructors

        /**
         * C++ constructor
         * @param aShowErrorMsgs Show an error note if the file format is not supported
         */
        CFLDPlayerBase( TBool aShowErrorMsgs );

        /**
         * Second phase constructor
         */
        void BaseConstructL();

    public:		// From MFLDFileProcessor

        /**
        * Set the volume level on which the sound is played
        * If this method is not called, player uses the setting in active profile.
        * @param aVolume The volume level
        */
        void SetVolume( TInt aVolume );

        /**
        * Sets the ringing type
        * If this method is not called, player uses the setting in active profile.
        * @param aRingingType The ringing type (see enum TRingingTypes)
        */
        void SetRingingType( TInt aRingingType );

        /**
        * Sets vibrating alert on or off.
        * If this method is not called, player uses the setting in active profile.
        * @param aVibra True: Vibra is on. False: Vibra is off.
        */
        void SetVibra( TBool aVibra );
        
        /**
        * Sets 3d-effects on or off.
        * If this method is called using parameter EFalse 3d-effects are 
        * always disabled even though those are set in active profile.
        * Otherwise toneplayer uses the setting in active profile.
        * @param a3dEffects True: 3dEffects are on. False: 3dEffects are off.
        */
        void Set3dEffects( TBool a3dEffects );

    protected:    // New methods

        /** See if a call is ongoing.
        * @return ETrue if a call is ongoing, EFalse otherwise.
        */
        TBool IsCallOngoing() const;

        /**
         * Helper method to adapt the Profiles volume to player volume.
         * @param aVolume the volume value to be converted.
         * @param aMaxVolume maximum volume level of the player used.
         * @return the converted volume.
         */
        TInt BaseConvertVolume( TInt aVolume, TInt aMaxVolume );

        /**
        * Show note "Tone format not supported"
        */
        void DisplayErrorNoteL();

        /**
         * Gets the data type of the given file.
         * @param aFileName path to the file which data type must be found out
         * @param aDataType value return parameter to hold the data type
         * @return system-wide error code
         */
        TInt DataType( const TDesC& aFileName, TDataType& aDataType );

	private: 	// Methods derived from MCenRepNotifyHandlerCallback
        void HandleNotifyInt(TUint32 aId, TInt aNewValue);

    private:    // Methods derived from MCoeForegroundObserver

        /**
        * Derived from MCoeForegroundObserver. Called when application gains foreground.
        */
        void HandleGainingForeground();

        /**
        * Derived from MCoeForegroundObserver. Stops playing the file if
        * this application loses focus.
        */
        void HandleLosingForeground();

    protected:    // Data

		/// Ringingvolume
        TInt iRingingVolume;
		/// Ringingtype
        TInt iRingingType;
		/// Vibrasetting (on/off)
        TBool iVibra;
		/// Own: pointer to error message
        HBufC* iErrorMessage;
        /// Prevent tone playing, if application has lost focus
        TBool iFocusLost;
        /// Default tone
        TFileName iDefaultTone;
        // Ringingtone Plugin (should be created/deleted by subclass)
		C3DRingingToneInterface* i3dRingingTonePlugin;
		/// 3DSettings (on/off)		
		TBool i3dEffects;

    private:    // Data

		/// Own: Pointer to volumenotifyhandler
		CCenRepNotifyHandler* iVolumeNotifyHandler;
		/// Own: Pointer to typenotifyhandler
		CCenRepNotifyHandler* iTypeNotifyHandler;
		/// Own: Pointer to vibranotifyhandler
		CCenRepNotifyHandler* iVibraNotifyHandler;
		/// Own: Pointer to CR session
        CRepository* iSession;
		/// Own: Pointer to CR vibra session
        CRepository* iVibraSession;
        /// Show an error, if the file format is not supported
        TBool iShowErrorMsgs;
		// Find out tone MIME -type
        RApaLsSession iApaSession;

    };

#endif      //  __CFLDPLAYERBASE_H__

// End of File

