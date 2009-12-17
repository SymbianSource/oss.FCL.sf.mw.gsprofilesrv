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
* Description:  A thin CAknPopupList extension.
*
*/



#ifndef CFLDPOPUPLIST_H
#define CFLDPOPUPLIST_H

// INTERNAL INCLUDES
#include "MFLDFileListBoxObserver.h" // For TFileListSoftKeyState

// EXTERNAL INCLUDES
#include <aknPopup.h>
#include <coemain.h> // For MCoeForegroundObserver

// FORWARD DECLARATIONS
class MFLDFileObserver;
class CFLDFileListModel;

// CLASS DECLARATION

/**
*  A thin CAknPopupList extension.
*  Prevents closing the pop-up if the pop-up observer so wishes.
*
*  @lib filelist.lib
*  @since 2.1
*/
NONSHARABLE_CLASS( CFLDPopupList )
 : public CAknPopupList, public MCoeForegroundObserver, public MCoeFocusObserver
    {


    public: // New methods

		/**
		* Two-phased constructor.
		* @param aListbox A parameter to CAknPopupList.
		* @param aCbaResource A parameter to CAknPopupList.
        * @param aFileObserver File observer.
        * @param aListBoxObserver Listbox observer, can be NULL.
        * @param aModel Filelist Model.
		* @param aType A parameter to CAknPopupList.
		*/
		static CFLDPopupList* NewL(
			CEikListBox* aListBox,
			TInt aCbaResource,
			MFLDFileObserver& aFileObserver,
            MFLDFileListBoxObserver* aListBoxObserver,
			CFLDFileListModel& aModel,
			AknPopupLayouts::TAknPopupLayouts aType = AknPopupLayouts::EMenuWindow );
		
		void ConstructL(CEikListBox* aListBox, TInt aCbaResource, AknPopupLayouts::TAknPopupLayouts aType );


		/**
        * Destructor.
        */	
		virtual ~CFLDPopupList();

		/**
		* Returns Popuplist RWindow handle
		*/
		RWindow& PopupListWindow() const;

		/**
		* Populates icon array and updates ListBox
		*/
		void PopulateIconArrayL();

		/**
		* Cancels audio/video preview
		*/
		void CancelPreview();
		
		/**
		* Handles pointer events of popups.
		*/
		void HandlePointerEventL(const TPointerEvent& aPointerEvent);
		
		/**
		 * Returns FileListBoxObserver.
		 */
		MFLDFileListBoxObserver* GetFileListBoxObserver();

    private:	// From CAknPopupList
        void AttemptExitL( TBool aAccept );
        void HandleResourceChange(TInt aType);
        void HandleListBoxEventL(CEikListBox* aListBox, TListBoxEvent aEventType);
        
	private:	// From MCoeForegroundObserver
        void HandleGainingForeground();
        void HandleLosingForeground();        

	private:	// From MCoeFocusObserver
        void HandleChangeInFocus();
    	void HandleDestructionOfFocusedItem();

	private:

		/**
		* C++ constructor.
        * @param aFileObserver File observer.
        * @param aListBoxObserver Listbox observer, can be NULL.
        * @param aModel Filelist Model.
		*/
		CFLDPopupList(
			MFLDFileObserver& aFileObserver,
            MFLDFileListBoxObserver* aListBoxObserver,
			CFLDFileListModel& aModel );

	private: // New methods

    private:    // Data

        /// File observer
        MFLDFileObserver& iFileObserver;

        /// Ref: Listbox observer
        MFLDFileListBoxObserver* iListBoxObserver;

		/// FileList Model
		CFLDFileListModel& iModel;

#ifdef RD_VIDEO_AS_RINGING_TONE
		/// Current softkeystate
		MFLDFileListBoxObserver::TFileListSoftKeyState iSoftKeyState;

#endif

		TFileName iCurrentFileName;

		// Have the checks in AttemptExitL been done already
		TBool iExitChecksDone;
		TPoint iPoint;
    };

#endif      // CFLDPOPUPLIST_H

// End of File
