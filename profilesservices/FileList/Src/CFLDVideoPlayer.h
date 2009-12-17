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
*	Creates a video player and starts video preview
*
*
*/


#ifndef __CFLDVIDEOPLAYER_H__
#define __CFLDVIDEOPLAYER_H__

// INTERNAL INCLUDES
#include "CFLDPlayerBase.h"

// EXTERNAL INCLUDES
#include <e32property.h>
#include <videoplayer.h> // For MVideoPlayerUtilityObserver

// FORWARD DECLARATIONS
class RWindow;

//  CLASS DECLARATION

/**
* Creates a video player and starts video preview
*
* @lib filelist.lib
* @since 3.1
*/
NONSHARABLE_CLASS( CFLDVideoPlayer )
	 : public CFLDPlayerBase,
	  public MVideoPlayerUtilityObserver
    {

	  private:  // Enumerations

    // These match with the ones in Profile Engine
   		 enum TRingingTypes
    		{
         	ERingingTypeRinging = 0,
         	ERingingTypeAscending,
         	ERingingTypeRingOnce,
         	ERingingTypeBeepOnce,
         	ERingingTypeSilent
         	};

    	 enum TVideoPlayerStatus
            {
            EVideoPlayerNotCreated = 0,
            EVideoPlayerInitializing,
            EVideoPlayerInitialized,
            EVideoPlayerReady,
            EVideoPlayerPlaying
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
         * @return A pointer to a fully constructed CFLDVideoPlayer instance
         */
        static CFLDVideoPlayer* NewL( TBool aShowErrorMsgs = ETrue );

        /**
         * Destructor
         */
        virtual ~CFLDVideoPlayer();

    private:    // Constructors and destructors

        /**
         * Constructor
         * @param aShowErrorMsgs Show an error note if the file format is not supported
         */
        CFLDVideoPlayer( TBool aShowErrorMsgs);

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
        
	private:	// From MVidePlayerUtilityObserver
		void MvpuoOpenComplete(TInt aError);
		void MvpuoFrameReady(CFbsBitmap& aFrame,TInt aError);
		void MvpuoPlayComplete(TInt aError);
		void MvpuoEvent(const TMMFEvent& aEvent);
		void MvpuoPrepareComplete(TInt aError);

    private:    // New methods

    	/** When videoplayer is ready, set its ringing type
        * @param aRingingType The ringing type (see enum TRingingTypes)
        */
        void DoSetRingingType( TInt aRingingType );

		/**
        * Converts volume from 1 to 10 to audio driver understandable value.
        * @param aVolume Volume level to be converted.
        * @return Returns converted volume level
        */
		TInt ConvertVolume( TInt aVolume );

		/**
         * Callback function for ascending ringing type
         * @param aObj Pointer to callback instance
         */
		static TInt VolumeRampTimerCallback( TAny* aObj );
		/**
         * Performs volume ramping
         */
		TInt DoVolumeRamp();
		/**
         * Performs video scaling and cropping
         * @param aDisplayWindow Reference to window handle
         */
		void AdjustToWindow( RWindow& aDisplayWindow );
		/**
         * Returns video frame size
         * @return size of the video frame
         */
		TSize VideoFrameSize() const;

        /**
        * Callback function to do inactivity timer resetting.
        */
        static TInt DoResetInactivityTimer( TAny* aObject );

    private:	// Data

    	/// Own: video player
    	CVideoPlayerUtility* iVideoPlayer;
        /// Own: Ascending volume timer
        CPeriodic* iVolumeRampTimer;
        /// Ref: handle to window
        RWindow* iWindow;
		/// Player state
        TVideoPlayerStatus iVideoPlayerStatus;
        /// Ascending volume
        TInt iRampedVolume;

        /**
		* Pub&Sub property.
		* For setting the state of the screen saver.
		*/
        RProperty iPropScreenSaver;

        /**
		* Timer for resetting the user inactivity timeout
		*/
		CPeriodic* iBacklightTimer;

    };

#endif      //  __CFLDVIDEOPLAYER_H__

// End of File
