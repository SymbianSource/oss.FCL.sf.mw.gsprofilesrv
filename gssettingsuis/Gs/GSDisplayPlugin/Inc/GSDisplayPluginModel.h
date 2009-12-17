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
* Description:  Display Settings model.
*
*/


#ifndef GSDISPLAYPLUGINMODEL_H
#define GSDISPLAYPLUGINMODEL_H

#include <e32base.h>
#include <e32property.h>
#include <centralrepository.h>
#include <cenrepnotifyhandler.h>
#include <backgroundimage.h>

class CGSDisplayPluginContainer;

// CONSTANTS
// maximum string sizes
const TInt KGSMaxImagePath = 256;  // max lenght of image paths
const TInt KGSMaxWNText = 50;      // max length of welcome note text
const TInt KGSMaxSSText = 15;      // max lenght of screen saver text
const TInt KGSWelcomeNoteTypeValue = 0;
const TInt KGSScreenSaverObjectValue = 0;
const TInt KGSScreenSaverPeriodValue = 5;
const TInt KGSWNTextIndex = 1;
const TInt KGSWNImageIndex = 2;
const TInt KGSBgImageIndex = 1;
const TInt KGSSSTextObjectIndex = 0;

//for switching values in SwitchValue()
const TInt KGSSettingOff = 0;
const TInt KGSSettingOn = 1;

// Setting values for Background and Welcome note image
const TInt KGSUWNoteSupportedValue = 1; // default is show the item
const TInt KGSWelcomeNoteTypeImage = 2;

_LIT( KGSWelcomeNoteImgPath, "c:\\private\\100058ec\\welcomeimage.mbm" );

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CGSBackgroundImage;

// CLASS DEFINITION
/**
*  CGSDisplayPluginModel is the model class of GS display plugin.
*  It provides functions to get and set setting values.
*  @lib GSDisplayPlugin.lib
*  @since Series 60_3.1

*/
NONSHARABLE_CLASS( CGSDisplayPluginModel ): public CBase,
                                            public MCenRepNotifyHandlerCallback
    {
    public:  // Constructor and destructor
        /**
        * Two-phased constructor
        * @param aInitBackgrooundApi Set to ETrue if CGSBackgroundImage is to 
        *        be initialized. This is not to possible if model is created in
        *        a process without a Avkon UI.
        */
        static CGSDisplayPluginModel* NewL( TBool aInitBackgroundApi );
        static CGSDisplayPluginModel* NewL();

        /**
        * Destructor
        */
        ~CGSDisplayPluginModel();

    public: // new ones

        /**
        * Returns welcome note type.
        * @return 0: default
        *         1: text
        *         2: image
        */
        TInt WelcomeNoteTypeL();

        /**
        * Sets welcomenote type.
        * @param aType Type of the welcome note.
        */
        void SetWelcomeNoteTypeL( TInt aType );

        /**
        * Gets welcome note text from shared data.
        * @param aNote Descriptor which will contain the welcome note text.
        *              Max length is KGSMaxWNText.
        */
        void WelcomeNoteTextL( TDes& aNote );

        /**
        * Stores welcome note text to shared data.
        * @param aNote Descriptor which contains the welcome note text.
        *              Max length is KGSMaxWNText.
        */
        void SetWelcomeNoteTextL( const TDesC& aNote );

        /**
        * Returns the deviation from the calibrated contrast value.
        * @return Deviation from the calibrated contrast value.
        *         Min value is -15 and max value is 15.
        */
        TInt ContrastL();

        /**
        * Sets the deviation from the calibrated contrast value.
        * @param aContrast Deviation from the calibrated contrast value.
        *                  Min value is -15 and max value is 15.
        */
        void SetContrastL( const TInt aContrast );

        /**
        * Return value tells what is screen saver object.
        * @return 0: text
        *         1: time and date
        */
        TInt ScreenSaverObjectL();

        /**
        * Sets screen saver object.
        * @param aObject screen saver object.
        */
        void SetScreenSaverObjectL( const TInt aObject );

        /**
        * Gets screen saver text from shared data.
        * @param aText Descriptor which will contain the screen saver text.
        *              Max length is KGSMaxSSText.
        */
        void GetScreenSaverTextL( TDes& aText );

        /**
        * Stores screen saver text to shared data.
        * @param aText Descriptor which contains the screen saver text.
        *              Max length is KGSMaxSSText.
        */
        void SetScreenSaverTextL( const TDesC& aText );

        /**
        * Returns the period of time after which the screen saver starts
        * if the keys are not pressed.
        * @return Minutes between 1 and 30.
        */
        TInt ScreenSaverPeriodL();

        /**
        * Sets the period of time after which the screen saver starts
        * if the keys are not pressed.
        * @param aPeriod Screen saver period (minutes between 1 and 30).
        */
        void SetScreenSaverPeriodL( const TInt aPeriod );

        /**
        * Returns the period of time after which the backlight fades out
        * if the keys are not pressed.
        * @return Seconds between 5 and 60.
        */
        TInt BacklightPeriodL();

        /**
        * Sets the period of time after which the backlights fades out
        * if the keys are not pressed.
        * @param aPeriod backlight period (seconds between 5 and 60).
        */
        void SetBacklightPeriodL( const TInt aPeriod );

        /**
        * Checks whether the user welcome note is supported
        * @return TInt
        */
        TInt IsUWNoteSupportedL();

        /**
        * Returns the deviation from the calibrated brightness value.
        * @return Deviation from the calibrated brightness value.
        *         Min value is 1 and max value is 31.
        */
        TInt BrightnessL();

        /**
        * Sets the deviation from the calibrated brightness value.
        * @param aBrightness Deviation from the calibrated brightness value.
        *                  Min value is 1 and max value is 31.
        */
        void SetBrightnessL( const TInt aBrightness );

        /**
        * Returns the deviation from the calibrated light sensor value.
        * @return Deviation from the calibrated light sensor value.
        *         Min value is 1 and max value is 5.
        */
        TInt AmbientLightSensorL();

        /**
        * Sets the deviation from the calibrated light sensor value.
        * @param aSensorSensivity Deviation from the calibrated brightness value.
        *                  Min value is 1 and max value is 5.
        */
        void SetAmbientLightSensorL( const TInt aSensorSensitivity );

        /**
        * Returns the current display text size.
        * @return One of the following values:
        *         Large
        *         Normal (default)
        *         Small
        */
        TInt DisplayTextSizeL();

        /**
        * Set the new display text size.
        * @param aSize. One of the following values:
        *         Large
        *         Normal (default)
        *         Small
        */
        void SetDisplayTextSizeL( TInt aSize );


        /**
        * Get the Power Save Led value
        * @return:
        * 0: Disabled
        * 1: Enabled
        */
        TInt PowerSaveLedL();

        /**
        * Set the Power Save Led value
        * @param aPowerLedState
        * 0: Disabled
        * 1: Enabled
        */
        void SetPowerSaveLedL( const TInt aPowerLedState );

        /**
        * Checks Power Save Led support
        * @return:
        * 0: Disabled
        * 1: Enabled
        */
        TInt CheckPowerSaveLedSupportL();

        /**
        * @return pointer to background image. Does not transfer ownership.
        */
        CGSBackgroundImage* BackgroundImage();
        
        /**
        * Checks ScreenSaver timeout support
        * @return:
        * ETrue: ScreenSaver timeout setting is supported
        * EFalse: ScreenSaver timeout setting is not supported 
        */
        TBool CheckScreenSaverTimeoutSupportL();
        
        /**
        * Checks if Operator Logo setting item is to be made visible.
        * @return ETrue if setting item is to be shown.
        *         EFalse if setting item is not to be shown.
        */
        TBool ShowOperatorLogoSettingL();

        /**
        * Gets operator logo state.
        * @return KGSSettingOn  if logo is used.
        *         KGSSettingOff if logo is not used.
        */
        TInt OperatorLogoL();
        
        /**
        * @param
        */
        void SetOperatorLogoL( TInt aOperatorLogo );
        
        /**
         * @param
         */
        void SetOwner( CGSDisplayPluginContainer* aPlugin);

    private: // Private constructors

        /**
        * Default C++ contructor
        */
        CGSDisplayPluginModel();

        /**
        * Symbian OS default constructor
        * @return void
        */
        void ConstructL( TBool aInitBackgroundApi );

        /**
        * Initialize CenRep keys used in this class
        */
        void InitializeCentralRepositoryL();

        /**
        * Un-initialize CenRep keys used in this class
        */
        void UninitializeCentralRepository();

    private: // new ones

        void SetBackLightValueL();
		TInt ScaleIntensityValues( TInt aValue, TBool aSample );
		TInt ScaleLightSensorValues( TInt aValue, TBool aDirection );
		
		// From MCenRepNotifyHandlerCallback
		void HandleNotifyInt( TUint32 aId, TInt aNewValue);

    private:
        //Central repository objects
        CRepository* iPersonalizationRepository;
        CRepository* iStartupConfRepository;
        CRepository* iScreensaverRepository;
        CRepository* iLightRepository;
        CRepository* iGSVariationRepository;
        CRepository* iAvkonRepository;
        // Used for OperatorLogo
        CCenRepNotifyHandler* iNotifyHandlerForOpLogo;
        CCenRepNotifyHandler* iNotifyHandlerForOpLogoVisible;
        CGSDisplayPluginContainer* iContainer;

        //P&S keys
        RProperty* iCoreAppProperty;

        //local variated constants
        TInt iLocalVariationValues;

        CGSBackgroundImage* iBackgroundApi;
    };

#endif //GSDISPLAYPLUGINMODEL_H

// End of File