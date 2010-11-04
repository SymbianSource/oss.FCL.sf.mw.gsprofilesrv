/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Call sub-menu in General Settings.
*
*/


#ifndef GSCALLPLUGIN_H
#define GSCALLPLUGIN_H

// INCLUDES
#include "GSCallPluginModel.h"
#include "MGsFWMSKLabelObserver.h"

#include <MSSSettingsObserver.h> //for MSSSettingsObserver
#include <MMGFetchVerifier.h>
#include <mgsasyncimagehandlingobserver.h>
#include <centralrepository.h>
#include <cenrepnotifyhandler.h>
#include <ConeResLoader.h>

#include <gsbaseview.h>
#include <gsfwviewuids.h>
#include <gsplugininterface.h>

// FORWARD DECLARATIONS
class CGSCallPluginContainer;
class CAknRadioButtonSettingPage;
class CPsetContainer;
class CPsuiContainer;
class CPsuiWaitingObs;
class CPsetCallWaiting;
class CAknPopupList;
class CGSTelPluginModel;
class CGSCcbsHandler;
class CAknWaitDialog;
class CAknViewAppUi;

// Constants
const TUid KCallPluginUID = { 0x102824A2 };

// Resource file name
_LIT( KGSCallPluginResourceFileName, "z:gscallpluginrsc.rsc" );

_LIT( KGSCallPluginIconDirAndName, "z:GSCallPlugin.mbm");

// CLASS DECLARATION
/**
*  CGSCallPlugin view class 
*/
class CGSCallPlugin : public CGSBaseView, 
                      public MSSSettingsObserver,
                      public MMGFetchVerifier,
                      public MGSAsyncImageHandlingObserver,
                      public MGsFWMSKObserver
    {
    public: // Constructor and destructor

        /* C++ Constructor */
        CGSCallPlugin();
        
        /**
        * Symbian OS two-phased constructor
        * @return
        */
        static CGSCallPlugin* NewL( TAny* aInitParams );

        /* Symbian OS two-phase Constructor 
        *
        * @return pointer to CGSCallPlugin object
        */
        static CGSCallPlugin* NewLC();

        /**
        * Destructor.
        */
        ~CGSCallPlugin();

    public: // Functions from CAknView
        
        /**
        * From CAknView, returns the views id.
        *
        * @return Returns id of the view.
        */
        TUid Id() const;
        
        /**
        * See base class.
        */
        void HandleClientRectChange();
        
        /**
        * See base class.
        */
        void HandleStatusPaneSizeChange();

        /**
        * From MEikCommandObserver, handles the menu based commands.
        *
        * @param aCommand identifies the given command.
        */
        void HandleCommandL( TInt aCommand );
        
        /**
        * See base class.
        */
        void DoActivateL( const TVwsViewId& aPrevViewId,
                          TUid aCustomMessageId,
                          const TDesC8& aCustomMessage );

        /**
        * See base class.
        */
        void DoDeactivate();

    public: // From base classes
        /*
        * From MSSSettingsObserver, observer function 
        * @param aSetting Current setting
        * @param aNewValue New value for the setting
        */
        void PhoneSettingChanged( TSSSettingsSetting aSetting, 
                                  TInt aNewValue );

        /**
        * Updates specific value to container 
        * @param aListItem item index to update
        * @param aNewValue new value for the setting
        */
        void UpdateListBoxL( const TInt& aListItem, TInt aNewValue );
        
        /**
        * From MEikCommandObserver, handles the commands.
        *
        * @param aCommand identifies the command given.
        */
        void ProcessCommandL( TInt aCommand );
      
        /**
        * Checks if the MSK label needs to be adjusted.
        */
        void CheckMiddleSoftkeyLabelL();
        
    public: // From CGSPluginInterface

        /**
        * See base class.
        */
        void GetCaptionL( TDes& aCaption ) const;

        /**
        * See base class.
        */
        TInt PluginProviderCategory() const;
        
        /**
        * See base class.
        */
        CGulIcon* CreateIconL( const TUid aIconType );
        
    public: //new
        /**
        * Remove command and label from MSK.
        */
        void RemoveCommandFromMSK( const TBool flag);
        
        /**
        * Adds given resource text as MSK to CBA.
        * 
        * @param aResourceId middle softkey label.
        * @param aCommandId command that should be performed when MSK
        *        is pressed.
        * @since S60 v3.1
        */
        void SetMiddleSoftKeyLabelL( const TInt aResourceId,const TInt aCommandId );
        
    protected: // from CGSBaseView
        /**
        * See base class.
        */
        void NewContainerL();

        /**
        * See base class.
        */
        void HandleListBoxSelectionL();

    protected: //from MMGFetchVerifier
        
        /**
        * Display Backlight setting page
        */
        TBool VerifySelectionL( const MDesCArray* aSelectedFiles );

    protected: //from MGSSyncImageHandlingObserver
        
        /**
        * Method derived from MGSSyncImageHandlingObserver
        * Run when the asynchronous image converting is complete.
        */
        void ImageHandlingCompleteL( TInt aError );

    protected: // From MEikMenuObserver

        void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );

    protected: //new

        /**
        * Symbian OS default constructor. 
        */
        void ConstructL();

        /**
        * Shows settingpage for on-off settings.
        *
        * @param aPage reveals what page should be displayed.
        */
        void ShowSettingPageL( TGSCallItemIds aPage );

        /**
        * Responsible for showing als related settingpage.
        * Separate method is needed because als settings differ quite
        * a bit from the other on/off settings.
        *
        * @param aPage reveals what page should be displayed.
        */
        void ShowAlsSettingPageL( TGSCallItemIds aPage );

        /**
        * Processes user's selection
        * @param aSetting tells which value to switch
        * Used when Central Repository is enabled
        */
        void SwitchOnOffSetting( TCallSettId aSetting );

        /**
        * Adds item to a list
        * @param aList reference to a list
        * @param aItem resource ID of the item to be added
        */
        void AppendItemL( CDesCArrayFlat& aList, TInt aItem );


        /**
        * Selects what kind of menu to create next
        *
        * @param aIndex currently selected item
        */
        virtual void HandleCreatePopupL( TInt aIndex );

        /**
        * Sets title to a setting page.
        *
        * @param aDlg reference to a setting page
        * @param aTitleID resource ID of title
        */
        void SetTitleToSettingL( CAknRadioButtonSettingPage& aDlg, 
                                 TInt aTitleID );
        
        /**
        * Sets title to status pane.
        *
        * @param aTitleID resource ID of title
        */
        /*void SetTitleL( TInt aTitleID );*/

        /**
        * Switches value between shared data (0 = off, 1 = on)
        * and setting page (1 = off, 0 = on).
        * @param aValue value to switch
        * @return switched value
        */
        TInt SwitchValue( TInt& aValue );

        /**
        * Shows notes when Als Block value has been changed.
        */
        void ShowAlsBlockNoteL();

        /**
        * Converts value between setting page and SsSettings.
        * @param aValue value to convert
        */
        void AlsBlockConvertValue( TInt& aValue );

        /**
        * Converts value between setting page and SsSettings.
        * @param aValue value to convert
        */
        void CliConvertValue( TInt& aValue );

        /**
        * Calls Security to make a Pin2 query.
        *
        * @return KPSPinQueryOk if query OK, otherwise 0.
        */
        TInt MakePin2QueryL();

        /**
        * Calls Security to make a Security code query.
        *
        * @return ETrue if query OK.
        */
        TBool MakeSecCodeQueryL();

        /**
        * Changes value in shared data for Als or Als Blocking.
        */
        void SwitchAlsSettingL();


        /**
        * Shows Soft reject setting page.
        */
        void ShowSoftRejectSettingPageL();



        /**
        * Display the Setting page for Replace Prefix item
        * @param aCurrentValue currently selected value in the setting page.
        * @return ETrue if the setting page selection has changed.
        */
        TBool ShowPrefixSettingPageL( TInt& aCurrentValue );

        /**
        * Toggles the Replace prefix between ON & OFF
        * @param aSettingPage Tells whether to display the setting page
        */
        void ChangePrefixSettingL( TBool aSettingPage );
        
        /**
        * Set the new changes to VOIP variables.
        * @param aPage one of the TGSCallItemIds value w.r.t VOIP
        * @param aCurrentSetting currently selected setting page item
        */
        void SetVoipSettingBasedSelectedLine( 
            const TGSCallItemIds aPage, 
            TInt aCurrentSetting );

        /**
        * Gets line for selected VoIP setting.
        * @param aPage one of the TGSCallItemIds value w.r.t VOIP
        * @return line for selected VoIP setting.
        */
        TInt GetLineForSelectedVoipSetting( const TGSCallItemIds aPage );

        /**
        * Display background image list from media gallery.
        */
        void ShowVTStillImageSettingPageL();

        /**
        *
        */
        void ShowStillImageListL();

        /**
        * Display image load wait note dialog.
        */
        void ShowImageLoadWaitNoteL();

        /**
        * Hide image load wait note dialog.
        */
        void HideImageLoadWaitNoteL();

        /**
        * Prompt image related errors to the user.
        * @param aError system wide Symbian errors
        */
        void HandleImageErrorsL( TInt aError );

        /**
        * Check if the input file is DRM protected
        * @param aOriginalFileName Path of the bitmap file
        * @return ETrue if file is DRM protected.
        *         EFalse otherwise.
        */
        TBool CheckDRMProtectionL( const TDesC& aOriginalFileName );
        
        TBool ShowCallDurationSettingPageL( TInt& aCurrentValue );
        
        void ChangeCallDurationSettingL( TBool aSettingPage );

        /**
        * Shows note if preferred call setting 
        * was changed to the Internet Telephone.
        */
        void ShowPreferredTelephonyNoteL();
        
        /*
        *  Toggles between the setting values based on aShowSettingPage
        *  @param aShowSettingPage is ETrue then setting page is shown
        */
        void ChangeLongPressCallKeySettingL( const TBool aShowSettingPage );
        
        /*
        * Launches setting page with the LongPressCallKey setting items
        * @Param gets the updated value set
        */
        TBool ShowLongPressCallKeySettingPageL( TInt& aCurrentValue );

        /**
        * Launches settings page for Own Image during video call setting  
        */
        void ShowOwnImageVtCallSettingPageL();
        
    private:
        //Retrieves container pointer.
        CGSCallPluginContainer*  Container();
        
        /*
        * Checks call state.
        * @return one of the call states from P&S key
        */
        TInt CheckCallStateL();

    protected: // Member Variables
        //CGSCallPluginModel class variable
        CGSCallPluginModel*         iModel;
        //General access class for phonesettings.
        CPsetContainer*             iSettings;
        //Call waiting SS handler
        CPsetCallWaiting*           iWaiting;
        //General access class for PSUI.
        CPsuiContainer*             iObsContainer;
        //Call waiting note handler
        CPsuiWaitingObs*            iCwObserver;
        //Fetches ss related phone settings.
        RSSSettings*                iSsSettings;
        //Value of als.
        TInt                        iAls;
        //Value of ALS Blocking.
        TInt                        iAlsBlock;
        //Value of Clir
        TInt                        iClir;
        //Has the rocker been pressed.
        TBool                       iRockerPress;
        //Creates and handles calls to CCBS Manager.
        CGSCcbsHandler*             iCcbsHandler;
        
    private:
        // for image handling
        CGSAsyncImageHandling*      iImageHandler;
        // for load wait dialog
        CAknWaitDialog*             iWaitDialog;
        // local variation for restricted CLIR
        TBool                       iRestrictedClir;
        // local variation for secure CLIR
        TBool                       iSecureClir;
        // video Telephony Still Image state
        TInt                        iVtStillImage;
        // Previous video Telephony Still Image state
        TInt                        iPreviousVtStillImage;
        // check if which command MSK is set during destruction
        TBool                       iMskCommandFlag;
        //Lock for VT still image listbox item
        TBool iVTImageSelectionOngoing;
           
    };


#endif  //GSCALLPLUGIN_H

// End of File
