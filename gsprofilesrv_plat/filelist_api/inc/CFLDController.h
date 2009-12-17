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
*     CFLDController observes the list box and invokes a MFLDFileProcessor
*     if the timer expires.
*
*
*/



#ifndef __CFLDCONTROLLER_H__
#define __CFLDCONTROLLER_H__

//	INTERNAL INCLUDES
#include "MFLDFileListBoxObserver.h"

//	EXTERNAL INCLUDES
#include <e32base.h>
#include <ConeResLoader.h>	// For RConeResourceLoader

// FORWARD DECLARATIONS
class MFLDFileObserver;
class MFLDFileProcessor;
class RWindow;

#ifdef RD_VIDEO_AS_RINGING_TONE
	class CFLDSoftKeyChanger;
#endif

//  CLASS DECLARATION

/**
* CFLDController observes the list box and invokes a MFLDFileProcessor
* if the timer expires.
*
* @lib filelist.lib
* @since 2.1
*/
class   CFLDController
    : public CBase, public MFLDFileListBoxObserver
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
    public:     // Constructors and destructors

		/**
        * Completes the construction of CFLDController.
        * @param aWindow A reference to window handle
        */
		IMPORT_C void CompleteConstructionL( RWindow& aWindow );

		/**
        * Destructor
        */
        IMPORT_C virtual ~CFLDController();

        /**
        * Two-phased static constructor.
        * @param aShowErrorMsgs Show an error note if the file format is not supported
        * @param aDelay The delay in microseconds until the timer expires
        * @return A pointer to a fully constructed CController instance
        */
        IMPORT_C static CFLDController* NewL( TBool aShowErrorMsgs,
          TTimeIntervalMicroSeconds32 aDelay );

        /**
        * Two-phased static constructor, leaves a pointer to cleanup stack
        * @param aShowErrorMsgs Show an error note if the file format is not supported
        * @param aDelay The delay in microseconds until the timer expires
        * @return A pointer to a fully constructed CController instance
        */
        IMPORT_C static CFLDController* NewLC( TBool aShowErrorMsgs,
         TTimeIntervalMicroSeconds32 aDelay );

    private:  // Constructors and destructors

        /**
        * Constructor
        * @param aShowErrorMsgs Show an error note if the file format is not supported
        * @param aDelay The delay in microseconds until the timer expires
        */
        CFLDController( TBool aShowErrorMsgs,
         TTimeIntervalMicroSeconds32 aDelay );

        /**
        * Second phase constructor
        */
        void ConstructL( );

    public:     // New methods

        /**
        * Set the delay until the timer expires.
        * @param aDelay The delay in microseconds
        */
        IMPORT_C void SetDelay( TTimeIntervalMicroSeconds32 aDelay );

		/**
        * Set the volume level on which the sound is played
        * If this method is not called, audio and video players
        * uses the setting in active profile.
        * @param aVolume The volume level
        */
        IMPORT_C void SetVolume( TInt aVolume );

        /**
        * Sets the ringing type
         * If this method is not called, audio and video players
        * uses the setting in active profile.
        * @param aRingingType The ringing type (see enum TRingingTypes)
        */
        IMPORT_C void SetRingingType( TInt aRingingType );

        /**
        * Sets vibrating alert on or off.
         * If this method is not called, audio and video players
        * uses the setting in active profile.
        * @param aVibra True: Vibra is on. False: Vibra is off.
        */
        IMPORT_C void SetVibra( TBool aVibra );

		/**
		* Sets the file observer for the class.
		* The file observer is asked if the file is ok to be processed.
		* @since 2.5
		* @param aFileObserver Pointer to the file observer.
		*/
		IMPORT_C void SetFileObserver( MFLDFileObserver* aFileObserver );
		
	 	/**
        * Sets 3d-effects on or off.
        * If this method is called using parameter EFalse 3d-effects are 
        * always disabled even though those are set in active profile.
        * Otherwise toneplayer uses the setting in active profile.
        * @param a3dEffects True: 3dEffects are on. False: 3dEffects are off.
        */
        IMPORT_C void Set3dEffects( TBool a3dEffects );
        
        /**
         * Release allocated resources
         */
        void Release();

    private:  // New methods

        /**
        * This callback method is called when the timer expires. Calls
        * MFLDFileProcessor::ProcessFileL().
		* @param aPtr pointer to CFLDController
		* @return Returns always zero
        */
        static TInt HandleTimerTickL( TAny* aPtr );

    public:     // Methods derived from MFLDFileListBoxObserver

        /**
        * Receives the event notification from the file list box
        */
        IMPORT_C void HandleFileListBoxEventL( TFileListBoxEvent aEvent,
                                                   const TDesC& aFileName );
		/**
        * Passes softkey notifications to the file list box
        */
		void HandleSoftKeyState( TFileListSoftKeyState& aSoftKeyState );

    private:    // Data

          /// Own: Pointer to the file handler
        MFLDFileProcessor* iAudioProcessor;

#ifdef RD_VIDEO_AS_RINGING_TONE
        /// Own: Pointer to the file handler
        MFLDFileProcessor* iVideoProcessor;
#endif
		/// Own: Pointer to timer
        CPeriodic* iTimer;

		/// Ref: Pointer to window
		RWindow* iWindow;

        /// Currently focused file in the list
        TFileName iCurrentFile;

        /// The delay in microseconds until timer expires
        TTimeIntervalMicroSeconds32 iDelay;

		/// Ref: Pointer to file observer
		MFLDFileObserver* iFileObserver;

#ifdef RD_VIDEO_AS_RINGING_TONE
		/// Own: Pointer to the sofkey changer
		CFLDSoftKeyChanger* iSoftKeyChanger;
#endif
		/// Ringingvolume
        TInt iRingingVolume;

		/// Ringingtype
        TInt iRingingType;

		/// Vibrasetting (on/off)
        TBool iVibra;

        /// Show errors
        TBool iShowErrorMsgs;

		/// Resourceloader which loads used resources
       	RConeResourceLoader iResourceLoader;        
       	
       	/// 3DSettings (on/off)
       	TBool i3dEffects;

    };

#endif      //  __CFLDCONTROLLER_H__

// End of File
