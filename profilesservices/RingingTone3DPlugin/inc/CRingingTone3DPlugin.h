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
* Description:  Plug-In that implements C3DRingingToneInterface.
*
*/


#ifndef CRINGINGTONE3DPLUGIN_H
#define CRINGINGTONE3DPLUGIN_H

//  INCLUDES
#include    <DrmAudioSamplePlayer.h>
#include    <mdaaudiotoneplayer.h>
#include    <videoplayer.h>

#include <c3dringingtoneinterface.h>
#include "C3DAudioTimeOutTimer.h"
#include "C3DAudioPattern.h"
#include "C3DAudioXMLParser.h"

#include <centralrepository.h>
#include <MProfile.h>


// FORWARD DECLARATIONS
class C3DAudioTimeOutTimer;
class CEnvironmentalReverbUtility;
class CSourceDoppler;
class CSourceLocation;
class CListenerLocation;

class C3DPatternParser;

#ifdef __STEREO_WIDENING_EFFECT_UI 
class CStereoWidening;
#endif


// CONSTANTS
const TInt32 KRingingTone3DAudioEComPluginUID = 0x10208ADF;
const TInt KBufSize256 = 256;



// CLASS DECLARATION


/**
* Plug-In implementing C3DRingingToneInterface.
*
* Used for playing 3D ringing tones. For playing, plugin uses player utility
* (eg CDrmPlayerUtility) from calling application. 3D effect is achieved by
* adding 3D data into normal ringing tone. C3DAudioXMLParser reads this
* data from .xml files (one file per 3D effect).
*  
* @lib RingingTone3DPlugin.dll
* @since 5.0
*/
NONSHARABLE_CLASS (CRingingTone3DPlugin): public C3DRingingToneInterface,
							              public M3DAudioTimeOutNotifier
    {
    private:
    
    enum TRingingTonePlayerType
        {
        EMdaAudioPlayerUtility,
        EMdaAudioToneUtility,
        EDrmPlayerUtility,
        EVideoPlayerUtility
        };
        

    public:
        
        /**
        * Two-phased constructor.
        */
        static CRingingTone3DPlugin* NewL( TAny* aParameter );
        
        /**
        * Destructor.
        */
        virtual ~CRingingTone3DPlugin();
        
    private:

       /**
        * C++ default constructor.
        */
        CRingingTone3DPlugin();

       /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL( TAny* aParameter );
        

    public: // Functions from C3DRingingToneInterface

       /**
        * Set 3D plugin attribute.
        * @param aAttributeKey
        * @param aValue
        * @return System-wide error code.
        */
        TInt SetAttr( T3DRingingToneInterfaceAttr aAttributeKey, TInt aValue );

       /**
        * Set 3D plugin attribute.
        * @param aAttributeKey
        * @param aValue
        * @return System-wide error code.
        */
        TInt SetAttr( T3DRingingToneInterfaceAttr aAttributeKey, const TDesC& aValue );

       /**
        * Set 3D plugin attribute.
        * @param aAttributeKey
        * @param aValue
        * @return System-wide error code.
        */
        TInt SetAttr( TInt aAttributeKey, TAny* aValue );

       /**
        * Plays 3D ringing tone.
        */
    	void PlayL();

       /**
        * Stops playing 3D ringing tone.
        */
    	void Stop();

    
    private:  // from M3DAudioTimeOutNotifier

       /**
        * Callback initiated from C3DAudioTimeOutTimer.
        */
		void TimerExpiredL();

	private:
		
       /**
        * Initializes player utility.
        * @param aSamplePlayer Player utility.
        */
        void Init( CMdaAudioPlayerUtility* aSamplePlayer );

       /**
        * Initializes player utility.
        * @param aSamplePlayer Player utility.
        */
        void Init( CMdaAudioToneUtility* aSamplePlayer );

       /**
        * Initializes player utility.
        * @param aSamplePlayer Player utility.
        */
        void Init( CDrmPlayerUtility* aSamplePlayer );

#ifdef RD_VIDEO_AS_RINGING_TONE
       /**
        * Initializes player utility.
        * @param aSamplePlayer Player utility.
        */
        void Init( CVideoPlayerUtility* aSamplePlayer  );
#endif
        
       /**
        * Plays 3D ringing tone.
        */        
        void DoPlayL();
        
       /**
        * Add 3D pattern (effect) data into normal ringing tone.
        */
        void SetNext3DPatternPositionL();

	   /**
        * Plays 3D ringing tone.
        */		        
        void PlayRingingTone();

	   /**
        * Stops playing 3D ringing tone.
        */		        
        void StopRingingTone();
  
    private:
    
	   /**
        * Calculates the correct dTime according to velocity-value in cenrep.
        * @param aTime Time from the script to be calculated.
        */		        
        void CalculateDTime( TInt& aTime );
        
       /**
        * Plugin initialization.
        */		        
        void InitializeL();
        
       /**
        * Plugin uninitialization. Needed in error case when normal ringing 
        * tone is played.
        */		        
        void UnInitialize();
        

    private:
    
	   /**
        * Gets CEnvironmentalReverbUtility object.
        * @return CEnvironmentalReverbUtility.
        */		        
        CEnvironmentalReverbUtility* EnvironmentalReverbUtilityL();

	   /**
        * Gets CSourceLocation object.
        * @return CSourceLocation.
        */		        
        CSourceLocation* SourceLocationL();
        
	   /**
        * Gets CListenerLocation object.
        * @return CListenerLocation.
        */		        
        CListenerLocation* ListenerLocationL();
        
	   /**
        * Gets CSourceDoppler object.
        * @return CSourceDoppler.
        */		        
        CSourceDoppler* SourceDopplerL();
      
#ifdef __STEREO_WIDENING_EFFECT_UI
        CStereoWidening* StereoWideningL();
#endif        


    private:

       /**
        * Read active profile setting from profile engine. Needed in case calling
        * app does not set 3D effect and echo attributes.
        */
        void ReadActiveProfileL();
        
       /**
        * Checks that tone can be used as 3D ringing tone. This check is only
        * done if calling app explicitly sets ringing tone attribute.
        */
        TBool CheckRingingTone();

	   /**
        * Panics plugin dll.
        * @param aReason Panic code.
        */		        
        void Panic(TInt aReason);
        
        
#ifdef _DEBUG
    public:
        void FlogRepositoryL(CRepository* aRepository, TInt aEcho);
        //void FlogPatternsL(TInt aOldPattern, TInt aNewPattern);
        void FlogProfileL(MProfile* aProfile);
        void FlogPlayer(TInt aPlayer);
        void Check3DEchoL(TInt aEcho);
        void Check3DPatternL(TInt aPattern);
        void FlogPositionL(T3DPosition& aPosition, TInt aIndex);
        void FlogVelocityL(T3DVelocity& aVelocity, TInt aIndex);
        void FlogTimeL(TInt aTime, TInt aIndex);
        void FlogPubSubL(TInt aEcho);
#endif        


    private:    // Data
   	
    	// 3D Ringing tone velocity
    	TInt iVelocity;		
    	
    	// Doppler status
    	TBool iDoppler;		
    	
    	// 3D effect (pattern)
    	TInt iProfile3DToneEffect;
    	
    	// 3D echo
    	TInt iProfile3DToneEcho;
    	
    	// ringing tone, used only for validity check
    	TFileName iRingingTone;
    	
    	// profile id, currently not used
    	TInt iProfileId;
    	
    	// ringing tone player type
    	TInt iPlayerType;
    	
    	// is ringing tone playing
    	TInt iPlaying;
		
		// 3D pattern parser
		C3DAudioXMLParser* iParser;
                
    	// current position in pattern script
    	TInt iCurrentUpdate;
    	
    	// player utility used for playback
    	CMdaAudioPlayerUtility* iAudioPlayerUtility;

    	// player utility
    	CMdaAudioToneUtility* iAudioToneUtility;
    	
    	// player utility
    	CDrmPlayerUtility* iDrmPlayerUtility;
    	
#ifdef RD_VIDEO_AS_RINGING_TONE
    	// player utility
    	CVideoPlayerUtility* iVideoPlayerUtility; 
#endif
  
    	// timer for updating 3D pattern position
    	C3DAudioTimeOutTimer* iTimer;
    
    	// reverberation utility used to set reverb presets
    	CEnvironmentalReverbUtility* iReverbEffect;
       	
       	// doppler-effect
       	CSourceDoppler* iDopplerEffect;
    	
    	// source location-effect
    	CSourceLocation* iSource;
    	
    	// listener location-effect
    	CListenerLocation* iListener;
     	
#ifdef __STEREO_WIDENING_EFFECT_UI    	
    	// stereo widening-effect
    	CStereoWidening* iStereoWidening;
#endif
    };




#endif      // CRINGINGTONE3DPLUGIN_H
            
// End of File
