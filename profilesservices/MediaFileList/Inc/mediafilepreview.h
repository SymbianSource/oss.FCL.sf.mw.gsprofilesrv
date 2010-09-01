/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Class for preview audio and video
*
*/



#ifndef MEDIAFILEPREVIEW_H
#define MEDIAFILEPREVIEW_H

#include "mediafiledialogutils.h"

//  INCLUDES

#include <e32base.h>
#include <DrmAudioSamplePlayer.h>
#include <mdaaudiotoneplayer.h>
#include <videoplayer.h>
#include <centralrepository.h>
#include <apgcli.h>  // for RApaLsSession
#include <e32property.h>



// CONSTANTS


//	FORWARD DECLARATIONS
class C3DRingingToneInterface;
class RWindow;




/**
* MPreviewHandlerObserver
* 
* For observing media file preview events.
*
*/
NONSHARABLE_CLASS (MPreviewHandlerObserver)
	{
    public:
    enum TPreviewHandler
        {
        EVideoPreviewComplete,
        EAudioPreviewComplete,
        EPreviewError
        };
	
	public:
		virtual void HandlePreviewEventL( TInt aEvent, TInt aError ) = 0;
	};




/**
* CMFPreviewHandlerBase
*
* Base class for CMFAudioPreviewHandler and CMFVideoPreviewHandler.
*/
NONSHARABLE_CLASS (CMFPreviewHandlerBase) : public CBase
    {
    public:
        enum TPlayerStatus
            {
            EPlayerNotCreated,
            EPlayerInitializing,
            EPlayerReady,
            EPlayerPlaying,
			EPlayerInitialized,
			EPlayerPlayingWith3DEffect
            };

        // these must match with the ones in Profile Engine
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
    
    protected:// Constructors and destructor

        /**
        * Destructor.
        */
        virtual ~CMFPreviewHandlerBase();
    
    protected:

        /**
        * C++ default constructor.
        */
        CMFPreviewHandlerBase();
	
        /**
        * By default Symbian OS constructor is private.
        */
        void ConstructL();

	public:
		void SetAttrL( TInt aAttr, TInt aValue );
		void SetAttrL( TInt aAttr, const TDesC& aValue );
		void SetAttrL( TInt aAttr, TAny* aValue );
		TInt Attr( TInt aAttr );
		void SetObserver( MPreviewHandlerObserver* aObserver );
        virtual void PlayL() = 0;
        virtual void Stop() = 0;
        virtual TBool IsPlaying() = 0;

    protected:
        TInt ConvertVolume( TInt aVolume );
        void ReadActiveProfileL();
        TInt GetDataType( const TDesC& aFileName, TDataType& aDataType );
        void ReadDefaultToneL();
        void Panic( TInt aReason );
        void EnableScreenSaver( TBool aEnable );
        
        void DisableBackLight();
        		
        static TInt DoResetInactivityTimer( TAny* aObject );
    protected:
        TInt RingingVolume();
        TInt RingingType();
        TInt Vibra();
        TInt Echo3D();
        TInt Effect3D();
        
    protected:
        static TInt ConvertVolume( TInt aVolume, TInt aMaxVolume );
        static TBool IsCallOngoing();
        
    protected:
        TInt iRingingVolume;
        TInt iRingingType;
        TInt iVibra;
        TInt i3DEffect;
        TInt i3DEcho;
        TInt iMediaType;
        TInt iFileSize;
        HBufC* iFullName;
        
        TBool iActiveProfileRead;
        TInt iActiveProfileRingingVolume;
        TInt iActiveProfileRingingType;
        TInt iActiveProfileVibra;
        TInt iActiveProfile3DEffect;
        TInt iActiveProfile3DEcho;


        MPreviewHandlerObserver* iObserver;
        
        TInt iPlayerStatus;
        
        // handle to window
        RWindow* iWindow;  // does not own
        
        // for getting file MIME types
        RApaLsSession iApaSession;
        
        // for setting screensaver on/off
        RProperty iProperty;
        
        // default ringing tone
        TFileName iDefaultTone;
        
        // file server session
        //RFs iFsSession;
        
        /**
		* Timer for resetting the user inactivity timeout
		*/
		CPeriodic* iBacklightTimer;
    };



/**
* CMFVideoPreviewHandler
* 
* For previewing video files.
*/
NONSHARABLE_CLASS (CMFVideoPreviewHandler) : public CMFPreviewHandlerBase,
                                             public MVideoPlayerUtilityObserver
    {

    public:// Constructors and destructor

        /**
        * Two-phased constructor.
        */
		static CMFVideoPreviewHandler* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CMFVideoPreviewHandler();
    
    private:

        /**
        * C++ default constructor.
        */
        CMFVideoPreviewHandler();
	
        /**
        * By default Symbian OS constructor is private.
        */
        void ConstructL();

	public:
        void PlayL();
        void Stop();
        TBool IsPlaying();
        void SetDisplayWindowL();

    
    private:
        TInt VolumeRamp();
        void SetRingingType( TInt aRingingType );
        TInt ConvertVolume( TInt aVolume );
        void Cancel();
        void GetRectData( TRect aRect, TDes& aText );
        

        
    private:
        static TInt VolumeRampTimerCallback( TAny* aObj );
        
	private:	// from MVidePlayerUtilityObserver
		void MvpuoOpenComplete( TInt aError );
		void MvpuoFrameReady( CFbsBitmap& aFrame, TInt aError );
		void MvpuoPlayComplete( TInt aError );
		void MvpuoEvent( const TMMFEvent& aEvent );
		void MvpuoPrepareComplete( TInt aError );
        
        
    private:
        
    	//  video player
    	CVideoPlayerUtility* iVideoPlayer;
        
        // ascending volume timer
        CPeriodic* iVolumeRampTimer;
        
        // Ascending volume
        TInt iRampedVolume;
    };
    


/**
* CMFAudioPreviewHandler
* 
* For previewing audio files (tones, music and recordings).
*
*/
NONSHARABLE_CLASS (CMFAudioPreviewHandler) : public CMFPreviewHandlerBase,
                                        public MDrmAudioPlayerCallback,
                                        public MMdaAudioToneObserver

    {
    public:// Constructors and destructor

        /**
        * Two-phased constructor.
        */
		static CMFAudioPreviewHandler* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CMFAudioPreviewHandler();
    
    private:

        /**
        * C++ default constructor.
        */
        CMFAudioPreviewHandler();
	
        /**
        * By default Symbian OS constructor is private.
        */
        void ConstructL();

	public:
        void PlayL();
        void Stop();
        TBool IsPlaying();

    private:
        void Cancel();
        void SetAudioRingingType( TInt aRingingType );
        void SetToneRingingType( TInt aRingingType );
        TInt ConvertVolume( TInt aVolume );
        
        
    private:  // from MMdaAudioToneObserver
	    virtual void MatoPrepareComplete( TInt aError );
	    virtual void MatoPlayComplete( TInt aError );

    private:  // from MDrmAudioPlayerCallback
        void MdapcInitComplete( TInt aError, const TTimeIntervalMicroSeconds& aDuration );
        void MdapcPlayComplete( TInt aError );

    private:
        // audio player
        CDrmPlayerUtility* iAudioPlayer;

		/// Audioplayer status
        TPlayerStatus iAudioPlayerStatus;

		/// toneplayer
        CMdaAudioToneUtility* iTonePlayer;

		/// Toneplayer status
        TPlayerStatus iTonePlayerStatus;
        
        // plugin for playing 3D effects
        C3DRingingToneInterface* i3dRingingTonePlugin;
        
    };
    

    


#endif  // MEDIAFILEPREVIEW_H

// End of File
