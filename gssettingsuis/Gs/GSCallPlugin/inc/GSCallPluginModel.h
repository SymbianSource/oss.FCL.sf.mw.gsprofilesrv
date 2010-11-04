/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Call Settings plugin model.
*
*/


#ifndef GSCALLPLUGINMODEL_H
#define GSCALLPLUGINMODEL_H

// INCLUDES
#include "GSCallPlugin.hrh"

#include <e32base.h>
#include <centralrepository.h>
#include <backgroundimage.h>


// CONSTANTS
// Panic codes for used in this compilation unit
enum KGSCallPluginModelPanicCodes
    {
    EGSCallPluinModelPanicNullPtr = 1
    };

const TInt KGSVTStillImageValue = 0;

// FORWARD DECLARATIONS
class CPsetCustomerServiceProfile;
class CGSBackgroundImage;

class CSPSettings;

// CLASS DEFINITION
/**
*  CGSCallPluginModel is the model class of GS app.
*  It provides functions to get and set setting values.
*/
class   CGSCallPluginModel : public CBase
    {
    public:  // Constructor and destructor
        /**
        * Two-phased constructor
        */
        static CGSCallPluginModel* NewL();

        /**
        * Destructor
        */
        ~CGSCallPluginModel();

    public: //new 
        
        
        /* 
        * Returns one of the call related values from shared data.
        * @param aKeyName name of the shared data key to fetch
        * @param aId value of shared data key
        * @return ETrue: no errors
        *         EFalse: an error has occurred
        */
        TBool GetCallSettingValue( const TCallSettId aCallId, TInt& aId );        
                
        /* 
        * Sets one of the call related values from shared data.
        * @param aKeyName name of the shared data key to set
        * @param aId value of shared data key
        * @return ETrue: no errors
        *         EFalse: an error has occurred
        */
        TInt SetCallSettingValue( const TCallSettId aCallId, TInt aId );
        
        /**
        * Gets CSP status
        * 
        * @return ETrue: CSP is set
        *         EFalse: CSP is not set
        */
        TBool GetCSPStatus();

        /**
        * Sets CSP Active
        * @param aValue TBool
        * @return ETrue: CSP is set
        *         EFalse: CSP is not set
        */
        void SetCSPActiveL( TBool aValue );

        /**
        * Returns if CSP setting is supported
        * @param aSettingNumber TInt
        * @return ETrue: CSP is supported
        *         EFalse: CSP is not supported
        */
        TBool IsSettingSupported( TInt aSettingNumber );

		/**
        * Gets soft reject text from shared data.
        * @param aText User defined text for soft reject. 
        */
        TBool GetSoftRejectText( TDes& aText );

        /**
        * Sets user defined text to shared data for soft reject.
        * @param aText User defined text for soft reject.
        */
        TBool SetSoftRejectText( TDes& aText );

        //Still Image functions
        /**
        * Get the current value for Still Image
        * from Shared Data
        */
        TInt VTStillImageL();

        /**
        * Set the current value for Still Image
        * to Shared Data
        */
        void SetVTStillImageL( const TInt aValue );        

        /**
        * Get the Japanese prefix change mode value from shared data
        * @return mode
        */
        TInt PrefixChangeModeL();

        /**
        * Set the Japanese prefix change mode value to shared data
        * @param aMode integer value (0/1)
        */
        void SetPrefixChangeModeL( TInt aMode );

        /**
        * Get the Japanese prefix data from shared data
        * @param aText the edited value from data query
        */
        void PrefixChangeDataL( TDes& aText );

        /**
        * Set the Japanese prefix data to shared data
        * @param aText the edited value from data query
        */
        void SetPrefixChangeDataL( const TDesC& aText );

        /**
        * Check if Restricted Send Caller Id is supported.
        * This is handled using GS local variation
        * @return ETrue if supported.
        */
        TBool RestrictedSendCallerIdSupportedL();
        
        /**
        * Check if Secure Send Caller Id is supported.
        * This is handled using GS local variation
        * @return ETrue if supported.
        */
        TBool SecureSendCallerIdSupportedL();
        
        /** 
        * Returns one of the VoIP call related values from Cent. Repository
        * @param aVoipSettingId name of the Cent. Repository key to fetch
        * @param aVoipId value of Cent. Repository key
        * @return KErrNone if successful
        */
        TInt GetVoIPCallSettingValue( 
            const TGSVoIPSetting aVoipSettingId, 
            TInt& aVoipId );        
                
        /** 
        * Sets one of the VoIP call related values from Cent. Repository
        * @param aVoipSettingId name of the Cent. Repository key to set
        * @param aVoipId value of Cent. Repository key
        * @return KErrNone if successful
        */
        TInt SetVoIPCallSettingValue( 
            const TGSVoIPSetting aVoipSettingId, 
            TInt aVoipId ); 
            
        /**
        * Gets SCCP status from Central Repository
        * @param aStatus value of Cent. Repository key
        * @return KErrNone if successful
        */    
        TInt GetSCCPStatus( TInt& aStatus );    
              
        /**
        * Switches value between CenRep (0 = off, 1 = on)
        * and setting page (1 = off, 0 = on).
        * @param aValue value to switch
        * @return switched value
        */
        void SwitchValue( TInt& aValue );
        
        /**
        * Get Call Duration value from CenRep
        */
        TInt CallDurationL();
        
        /**
        * Set Call Duration value to CenRep key.
        * @param aValue value to set
        */
        void SetCallDurationL( TInt aValue );
        
        /**
        * Get Long Press Call Key value
        */
        TInt LongPressCallKeyL();
        
        /**
        * Set Long Press Call Key value to CenRep key.
        * @param aValue value to set
        */
        void SetLongPressCallKeyL( const TInt aValue );
        
        /**
        * Maps the Long Press Call Key setting value
        * @Param value to be mapped
        **/
        void MapLongPressKeyCallKeyValue( TInt &aValue );
        
        
        /**
        * Check if VoIP features are supported
        * This is done using Central Repository
        * @return ETrue if VoIP is supported
        */       
        TBool VoIPSupported();

        /**
        * Get own image vt call status
        */
        TInt OwnImageVtCallStatusL();
        
        /**
        * Set Own Image video call value to CenRep key.
        * @param aValue value to set
        */
        void SetOwnImageVtCallStatusL( const TInt aValue );
        
        /**
        * Check if slide settings items should be shown
        * @return ETrue if slide settings items should be shown
        */
        TBool SlideSettingsShownL() const;

    private: // Private constructors
        /**
        * Default C++ contructor
        */
        CGSCallPluginModel();

        /**
        * Symbian OS default constructor
        * @return void
        */
        void ConstructL();


    private: // new
		void InitializeCentralRepositoryL();
		void UninitializeCentralRepository();

    public: // data members
        CGSBackgroundImage* iBackgroundApi;

	private:
		CPsetCustomerServiceProfile* iCSP; //check if CSP is active

        //Central repository objects
        CRepository* iTelephonyRepository;
        CRepository* iCommonCallephonyRepository;
        CRepository* iGSVariationRepository;
        CRepository* iRichCallRepository;
        CRepository* iLogsRepository;
        CRepository* iCommonTelephonyRepository;
        
        TInt iLocalVariationValues; //local variated constants
		CSPSettings* iSpSettings;

    };

#endif // GSCALLPLUGINMODEL_H
            
// End of File
