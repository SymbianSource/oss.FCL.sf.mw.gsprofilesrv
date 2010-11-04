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
* Description:  View for Display settings.
*
*/


#ifndef GSDISPLAYPLUGIN_H
#define GSDISPLAYPLUGIN_H

// INCLUDES
#include <aknsettingpage.h>
#include <ConeResLoader.h>
#include <gsplugininterface.h>
#include <gsfwviewuids.h>
#include <gsbaseview.h>
#include <MMGFetchVerifier.h>
#include <mgsasyncimagehandlingobserver.h>
#include <backgroundimage.h>
#include <eikmenup.h>

#ifdef FF_POWER_SAVE
    #include <psmtypes.h>
#endif // FF_POWER_SAVE

// CONSTANTS
const TUid KGSDisplayPluginUid = { 0x10207441 };
_LIT( KGSDefaultDisplayIconFileName, "Z:\\resource\\apps\\GSDisplayplugin.mbm");
_LIT( KGSDisplayPluginIconFileName, "\\resource\\apps\\GSDisplayplugin.mif");

const TInt KGSOpLogoLbxIndexOn = 0;
const TInt KGSOpLogoLbxIndexOff = 1;
// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD CLASS DECLARATION
class CAknViewAppUi;
class CAknWaitDialog;
class CAknRadioButtonSettingPage;
//class CAknQueryDialog;
class CGSDisplayPluginContainer;
class CGSDisplayPluginModel;

#ifdef FF_POWER_SAVE
    class CGSDisplayPluginAO;
#endif // FF_POWER_SAVE
    
// CLASS DECLARATION
/**
*  CGSDisplayPlugin view class
*
*  Plugin implementation for Display settings
*  @lib GSDisplayPlugin.lib
*  @since Series 60_3.1
*/
class CGSDisplayPlugin : public CGSBaseView, 
                         public MAknSettingPageObserver,
                         public MMGFetchVerifier,
                         public MGSAsyncImageHandlingObserver
    {
    public:
    
       enum TGSSettingIds
            {
            KGSContrastSettingId,
            KGSBrightnessSettingId,
            KGSSaverObjectSettingId,
            KGSAutodisconSettingId,
            KGSWelcomeNoteSettingId,
            KGSAmbientLightSensorId
            };
            
    public: // Constructors and destructor

        /**
        * Symbian OS two-phased constructor
        * @return GS connection view.
        */
        static CGSDisplayPlugin* NewL( TAny* aInitParams );

        /**
        * Destructor.
        */
        ~CGSDisplayPlugin();

    public: // from base classes
        
        /**
        * Returns view id.
        * @return TUid
        */
        TUid Id() const;

        /**
        * Handles commands.
        * @param aCommand Command to be handled.
        * 
        */
        void HandleCommandL( TInt aCommand );

    public: // From CGSPluginInterface

        /**
        * @see CGSPluginInterface header file.
        */
        void GetCaptionL( TDes& aCaption ) const;
        
        /**
        * @see CGSPluginInterface header file.
        */
        TInt PluginProviderCategory() const;
        
        /**
        * See base class.
        */
        CGulIcon* CreateIconL( const TUid aIconType );
        
    public: // from MAknSettingPageObserver
        
        /**
        * Handle setting page events
        */
        virtual void HandleSettingPageEventL( CAknSettingPage* aSettingPage,
                                            TAknSettingPageEvent aEventType );
            
    public: //new

        /**
        * Updates listbox's item's value.
        * @param aItemId An item which is updated.
        * 
        */
        void UpdateListBoxL( TInt aItemId );
        
        /**
        * Get CGSDisplayPlugin's ccontainer.
        */
        CGSDisplayPluginContainer* Container();

        
        /**
        * Updates display plugin view.
        */
        void UpdateViewL();
        void CloseDialog();
        
    protected: // From CAknView 

        void DoActivateL( const TVwsViewId& aPrevViewId,
                          TUid aCustomMessageId,
                          const TDesC8& aCustomMessage );
        void DoDeactivate();
        
    protected: // From MEikMenuObserver

        void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );
    
    protected:

        /**
        * C++ default constructor.
        */
        CGSDisplayPlugin();

        /**
        * Symbian OS default constructor.
        * 
        */
        void ConstructL();
        
        void HandleClientRectChange();

    private: // from CGSBaseView
        
        void NewContainerL();
        void HandleListBoxSelectionL();
        
    private: //from MMGFetchVerifier
        
        /**
        * Display Backlight setting page
        */
        TBool VerifySelectionL( const MDesCArray* aSelectedFiles );
    
    private: //from MGSSyncImageHandlingObserver
        /**
        * Display Welcome note image list from media gallery
        */
        void ImageHandlingCompleteL( TInt aError );
    
    private: //new methods
        
        /**
        * Display Constrast setting page
        */
        void ShowContrastSettingPageL();
        
        /**
        * Display Brightness setting page
        */
        void ShowBrightnessSettingPageL();
        
        /**
        * Display ALS setting page
        */
        void ShowAmbientLightSensorSettingPageL();
        
        /**
        * Display Screen saver timeout setting page
        */
        void ShowScreenSaverPeriodSettingPageL();
        
        /**
        * Display Backlight setting page
        */
        void ShowBacklightSettingPageL();
        
        /**
        * Display Welcome note setting page
        */
        void ShowWelcomeNoteSettingPageL();
        
        /**
        * Display Welcome note text input dialog
        */
        void ShowWelcomeNoteTextQueryL();
    
        /**
        * Display Welcome note image list from media gallery
        */
        void ShowWelcomeNoteImageListL();


        /**
        * Show display text size setting page
        */
        void ShowDisplayTextSizeSettingPageL();


        /**
        * Display Power save LED setting page
        * @param aSettingPage ETrue if setting page is to be shown
        *                     EFalse, otherwise
        */
        void ShowPowerSaveLedSettingPageL( TBool aSettingPage );
        
        /**
        * Display load wait-note dialog during image loading
        */
        void ShowImageLoadWaitNoteL();
        
        /**
        * Hide load wait-note dialog after the image is loaded
        */
        void HideImageLoadWaitNoteL();
        
        /**
        * Check if the input file is DRM protected
        * @param aOriginalFileName Path of the bitmap file
        * @return ETrue if file is DRM protected.
        *         EFalse otherwise.
        */
        TBool CheckDRMProtectionL( const TDesC& aOriginalFileName );
        
        /**
        * Handle any errors during image processing
        * @param aError system wide error code
        */
        void HandleImageErrorsL( TInt aError );

        /**
        * Changes operator logo value.
        * @param aUseSettingPage ETrue if setting page should be used.
        *                        EFalse is just switch between the values.
        */
        void ChangeOperatorLogoSettingL( TBool aUseSettingPage );

        /**
        * Displays operator logo setting page.
        * @param aOperatorLogoValue current operator logo value.
        */
        TBool ShowOperatorLogoSettingPageL( TInt& aOperatorLogoValue );
#ifdef FF_POWER_SAVE    
        /**
         *  Displays blocking note. Used if PSM is on.
         */
        void DisplayBlockNoteL();
#endif // FF_POWER_SAVE
        
    protected:
        //check if screen saver text is to be updated
        TBool iUpdateSSText;
        //check if welcome note is supported
        TBool iUpdateWNote;
        // for load wait dialog
        CAknWaitDialog* iWaitDialog;
        CAknRadioButtonSettingPage* iOprtLogoSettingPage;
                
    private:
        // plugin model
        CGSDisplayPluginModel *iModel;
        // resource loader
        RConeResourceLoader iResourceLoader;
        //for image handling
        CGSAsyncImageHandling* iImageHandler;
        //welcome note type: none, text, image       
        TInt iWelcomeNoteType;
        TBool iReshowOprtSetPage;
#ifdef FF_POWER_SAVE
        // Active object for handling PSM realted CenRep events.
        CGSDisplayPluginAO* iPsmActive;
#endif // FF_POWER_SAVE
        // Prevent multiple launch the MFetch.
        TBool iLaunchedImageFetch;
        
    };

   
    #endif // GSDISPLAYPLUGIN_H

//End of File
