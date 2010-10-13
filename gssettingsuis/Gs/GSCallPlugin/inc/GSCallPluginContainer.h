/*
* Copyright (c) 2002 - 2005 Nokia Corporation and/or its subsidiary(-ies).
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
*     Container for Call sub-menu in General Settings.
*
*/


#ifndef GSCALLPLUGINCONTAINER_H
#define GSCALLPLUGINCONTAINER_H

// INCLUDES
#include "MGsFWMSKLabelObserver.h"
#include "GSPhoneSettingConstants.h"

#include <RSSSettings.h>
#include <ConeResLoader.h>
#include <StringLoader.h>
#include <bautils.h>
#include <SoftReject.rsg>
#include <gsbasecontainer.h>
#include <gssettingid.h>

// FORWARD DECLARATIONS
class RSSSettings;
class CGSListBoxItemTextArray;
class CGSCallPluginModel;

// CONSTANTS
// this is required also by the view class implementation.
_LIT( KGSSoftRejectResource, "z:softreject.rsc" );


// CLASS DECLARATION
/**
*  CGSCallPluginContainer container class
*/
class CGSCallPluginContainer  : public CGSBaseContainer
    {
    public: // Constructors and destructor
        
        /**
        * Symbian OS default constructor.
        *
        * @param aRect gives the correct TRect for construction.
        */
        void ConstructL( const TRect& aRect );

        /**
        * Destructor.
        */
        ~CGSCallPluginContainer();

    public:

        /**
        * Updates list box
        *
        * @param aItemId is a updated list box item
        * @param aNewValue is a new value to be used for settings
        */
        virtual void UpdateListBoxL( TInt aItemId, TInt aNewValue );

        /**
        * Creates list box
        *
        * @param aResLbxId is resource number to create.
        */  
        void ConstructListBoxL( TInt aResLbxId );

        /**
        * Retrieves the currently selected listbox feature id
        * @return feature id.
        * @since 2.5
        */
        TInt CurrentFeatureId( ) const;

        /**
        * Retrieves soft reject text.
        * @param aSoftRejectTxt retrieved text.
        * @since 2.5
        */
        void GetSoftRejectTextL( TDes& aSoftRejectTxt );

        /** 
        * Reads from soft reject resource file.
        * @param aResourceId reource ID to read.
        * @return read resource text.
        * @since 2.5
        */
        HBufC* ReadFromResourceL( TInt aResourceId );

        /**
        * Defines observer for the middle softkey label changes.
        * Can only be set once. Further attempts are ignored.
        * @since S60 v3.1
        */
        void SetMiddleSoftkeyObserver( MGsFWMSKObserver* aObserver );
        
        /**
        * OfferKeyEvent to this Call Container class for updating the label
        * Up and Down rocker keys are handled 
        * @since S60 v3.1
        */
        TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, 
                                                    TEventCode aType );

    protected: //new

        /**
        * Creates list box items
        */  
        virtual void CreateListBoxItemsL();

        
        /**
        * Creates Still Image list box item.
        */  
        virtual void MakeStillImageItemL();

        /**
        * Creates Own Number sending list box item.
        */  
        virtual void MakeONSItemL();

        /**
        * Creates Call Waiting list box item
        */  
        virtual void MakeCWItemL();

        /**
        * Creates Phone line used list box item
        * @param aAlsValue value of setting
        */  
        virtual void MakePLUItemL( TInt& aAlsValue);

        /**
        * Creates Phone line blocked list box item
        * @param aAlsBlockValue value of setting
        */  
        void MakePLBItemL( TInt& aAlsBlockValue );

        /**
        * Creates Speed Dialing, Anykey Answer, Summary After Call
        * and Automatic redial items.
        * @param aIndex which item to create 
        */  
        virtual void MakeCallItemL( TInt aIndex );



        /**
        * Creates Soft reject text item.
        */
        void MakeSoftRejectTxtItemL();               

        /**
        * Required for help.
        */
        void GetHelpContext(TCoeHelpContext& aContext) const;

        /**
        * Creates Replace Prefix list box item
        */
        void MakeReplacePrefixItemL();

        /**
        * Create IC CLIR item.
        * @since 3.0
        */
        void MakeICClirItemL();

        /**
        * Create IC waiting item.
        * @since 3.0
        */
        void MakeICWaitingItemL();

        /**
        * Create Preferred call type item.
        * @since 3.0
        */
        void MakeICPreferredCallTypeItemL();
        
        /**
        * Create DND item.
        * @since 3.0
        */
        void MakeDndItemL();
        
        /**
        * Create Call Duration item.
        * @since 3.1
        */
        void MakeCallDurationItemL();
        
        /**
        * Create Long press call key items.
        * @since 4.0
        */
        void MakeLongPressCallKeyItemL();
        
        /**
        * Creates Own Image during video call item
        * @since 3.2 
        */
        void MakeOwnImageVtCallItemL();
        
        /**
         * Create "Opening slide to answer call" and "Closing slide to end call" item
         * @since 5.1
         */
        void MakeSlideSettingsItem( TInt aItemId );
        
    protected: // Member variables
        //SsSettings engine for CLI.
        RSSSettings* iSsSettings;
        //Resource based features.
        CGSListBoxItemTextArray* iListboxItemArray;
        //user selection for still image
        CDesCArrayFlat* iStillImgValues;
        //replace prefix items
        CDesCArrayFlat* iChangeModeItems;
        // List items in Internet Call CLIR. 
        CDesCArrayFlat* iClirItems;  
        // List items in Internet Call waiting. 
        CDesCArrayFlat* iCWItems;  
        // List items in Preferred call type.
        CDesCArrayFlat* iPCTItems;  
        // List items in do not disturb.
        CDesCArrayFlat* iDndItems;
        // Call Plugin model object
        CGSCallPluginModel* iModel;
        // List items in call duration item.
        CDesCArrayFlat* iCallDurationItems;
        // List items in call duration item.
        CDesCArrayFlat* iLongPressCallKeyItems;
        // List items in own image vt call item.
        CDesCArrayFlat* iOwnImageVtCallItems;
        /**
        * Middle softkey label observer.
        * Own.
        */
        MGsFWMSKObserver* iMSKObserver;
        
    };

#endif //GSCALLPLUGINCONTAINER_H
