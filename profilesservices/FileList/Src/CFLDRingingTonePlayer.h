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
*     CFLDRingingTonePlayer is used to play sound files. It takes into account
*     the current volume and ringing type settings.
*
*
*/


#ifndef __CFLDRINGINGTONEPLAYER_H__
#define __CFLDRINGINGTONEPLAYER_H__

// INTERNAL INCLUDES
//#include "MFLDFileProcessor.h"
#include "CFLDPlayerBase.h"

// EXTERNAL INCLUDES
#include <e32def.h>
#include <e32property.h>
#include <DrmAudioSamplePlayer.h>
#include <mdaaudiotoneplayer.h>


//	FORWARD DECLARATIONS
class C3DRingingToneInterface;

//  CLASS DECLARATION

/**
* CFLDRingingTonePlayer is used to play sound files. It takes into account the current
* volume and ringing type settings.
*
* @lib filelist.lib
* @since 2.1
*/
NONSHARABLE_CLASS( CFLDRingingTonePlayer )
    : public CFLDPlayerBase,
      public MDrmAudioPlayerCallback,
      public MMdaAudioToneObserver
    {
    private:  // Enumerations

        enum TAudioPlayerStatus
            {
            EAudioPlayerNotCreated,
            EAudioPlayerInitializing,
            EAudioPlayerReady,
            EAudioPlayerPlaying,
			EAudioPlayerInitialized,
			EAudioPlayerPlayingWith3DEffect
            };

        // These match with the ones in Profile Engine
        enum TRingingTypes
            {
            ERingingTypeRinging = 0,
            ERingingTypeAscending,
            ERingingTypeRingOnce,
            ERingingTypeBeepOnce,
            ERingingTypeSilent
            };

		enum TFLAllowScreenSaver
			{
			EFLScreenSaverAllowed = 0,
			EFLScreenSaverNotAllowed
			};

    public:     // Constructors and destructors

        /**
         * Two-phase static constructor
         * @param aShowErrorMsgs Show an error note if the file format is not supported
         * @return A pointer to a fully constructed CFLDRingingTonePlayer instance
         */
        static CFLDRingingTonePlayer* NewL( TBool aShowErrorMsgs = ETrue );

        /**
         * Destructor
         */
        virtual ~CFLDRingingTonePlayer();

    private:    // Constructors and destructors

        /**
         * Constructor
         * @param aShowErrorMsgs Show an error note if the file format is not supported
         */
        CFLDRingingTonePlayer( TBool aShowErrorMsgs );

        /**
         * Second phase constructor
         */
        void ConstructL();

    public:		// From MFLDFileProcessor

        /**
         * Derived from MFLDFileProcessor. Starts playing the sound file
         */
        void ProcessFileL( const TDesC& aFileName, RWindow* aWindow );

        /**
         * Derived from MFLDFileProcessor. Stops playing the file.
         */
        void Cancel();
        
    public:
        /**
        * Enables/disables 3D effects.
        * @param a3dEffects.
        * @return None.
        */
        void Set3dEffects( TBool a3dEffects );



    private:    // New methods

        /** When audioplayer is ready, set its ringing type
        * @param aRingingType The ringing type (see enum TRingingTypes)
        */
        void DoSetRingingType( TInt aRingingType );

        /**
        * Converts volume from 1 to 10 to audio driver understandable value.
        * @param aVolume Volume level to be converted.
        * @return Returns converted volume level
        */
        TInt ConvertVolume( TInt aVolume );

    private:    // Methods derived from MDrmAudioPlayerCallback

        /**
        * Derived from MDrmAudioPlayerCallback
        * This method is called when the audio player initialisation is ready
        */
        void MdapcInitComplete( TInt aError, const TTimeIntervalMicroSeconds& aDuration );

        /**
        * Derived from MDrmAudioPlayerCallback
        * This method is called when the audio player has finished playing
        */
        void MdapcPlayComplete( TInt aError );

    private:    // Methods derived from MMdaAudioToneObserver

	    virtual void MatoPrepareComplete( TInt aError );
	    virtual void MatoPlayComplete( TInt aError );

    private:    // Data

		/// Own: Pointer to audioplayer
        CDrmPlayerUtility* iAudioPlayer;
		/// Audioplayer status
        TAudioPlayerStatus iAudioPlayerStatus;
		/// Own: Pointer to toneplayer
        CMdaAudioToneUtility* iTonePlayer;
		/// Toneplayer status
        TAudioPlayerStatus iTonePlayerStatus;

        /**
		* Pub&Sub property.
		* For setting the state of the screen saver.
		*/
        RProperty iPropScreenSaver;

        // 3D effect
        TInt i3DEffect;
        // 3D effect
        TInt i3DEcho;
        
        // plugin for playing 3D effects
        C3DRingingToneInterface* i3dRingingTonePlugin;


    };

#endif      //  __CFLDRINGINGTONEPLAYER_H__

// End of File
