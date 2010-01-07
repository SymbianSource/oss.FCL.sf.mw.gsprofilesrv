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
* Description:   Header file of mediafiledialog. UI control of mediafilelist
*
*/




#ifndef MEDIALFILEDIALOG_H
#define MEDIALFILEDIALOG_H


//  INCLUDES
#include "mediafiledialogutils.h"
#include "mediafilepreview.h"
#include "mediafilehandler.h"
#include "mediafiledialogstate.h"
#include "mediafilewaitnote.h"


#include <e32base.h>
#include <aknlists.h>
#include <AknDialog.h>
#include <AknInfoPopupNoteController.h> // CAknInfoPopupNoteController
#include <eikedwin.h>
#include <aknsfld.h>
#include <peninputsrveventhandler.h>
#include <peninputclient.h>


// CONSTANTS

// CLASS DECLARATION
class CMFDialogTest;
class CMFProtectionHandler;
class CAknInputBlock;
class CVideoPreviewDialog;



/**
* CMFListHandlerObserver
* 
* CMFListHandler uses CMFListHandlerObserver to get listbox item texts.
*/
NONSHARABLE_CLASS (CMFListHandlerObserver)
    {
    public:
        virtual TPtrC ConstructListboxItem( TInt aListIndex ) = 0;
        virtual TInt ListboxItemCount() = 0;
    };


/**
* CMFListHandler
* 
* This class is used as listbox model's item array. Listbox uses it to draw 
* list items. CMFListHandler gets list item texts from CMediaFileHandler
* (metadata query result). This way there is no need to copy all list items
* into listbox model.  
*/
NONSHARABLE_CLASS (CMFListHandler) : public MDesCArray
    {
    public:
	    static CMFListHandler* NewL();
	    ~CMFListHandler();
    private:	
	    void ConstructL(void);
        CMFListHandler();

    public:  // from MDesCArray
	    TInt MdcaCount() const;
	    TPtrC16 MdcaPoint(TInt aIndex) const;

    public:
  	    void SetObserver( CMFListHandlerObserver* aObserver );
	    static void Panic( TInt aReason );
	    
    private:
	    CMFListHandlerObserver* iObserver;  // does not own
    };






/**
* CMediaFileDialog
*
* CMediaFileDialog provides UI for selecting/previewing media files.
*/
NONSHARABLE_CLASS (CMediaFileDialog) :  public CAknDialog, 
                                        public MEikListBoxObserver,
                                        public MCoeForegroundObserver,
                                        public CMFListHandlerObserver,
                                        public MMediaFileHandlerObserver,
                                        public MMFActiveCallerObserver,
                                        public MPreviewHandlerObserver,
                                        public MMediaFileWaitNoteObserver,
                                        public MAdaptiveSearchTextObserver,  // not used
                                        public MPenUiActivationHandler

    {
    
#ifdef _DEBUG     
    friend class CMFDialogTest;
#endif    
    
    enum TListOperationType
        {
        EAttrSelect,
        EAttrOpen,
        EAttrClicked,
        EAttrDoubleClicked
        };

    enum TDelayedOperationType
        {
        EAttrPreview,
        EAttrSearch,
        EAttrChangeCba
        };


    public:// Constructors and destructor

        /**
        * Two-phased constructor.
        */
		static CMediaFileDialog* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CMediaFileDialog();
    
    private:

        /**
        * C++ default constructor.
        */
        CMediaFileDialog();
	
        /**
        * By default Symbian OS constructor is private.
        */
        void ConstructL();

	public:
		TBool ShowDialogL( TDes* aFileName, TInt* aNullItem,
		                   TInt* aInfo, TDes* aDesInfo );
		
		void SetAttrL( TInt aAttr, TInt aValue );
		void SetAttrL( TInt aAttr, const TDesC& aValue );
		void SetAttrL( TInt aAttr, TAny* aValue );
        void SetNullItemL( const TDesC& aText, const TDesC& aMediaFile,
	                       TInt aMediaFileType, TInt aIconId );
		
	private:
        /**
        * From MEikListBoxObserver, called by framework.
        * @param aListBox.
        * @param aEventType.
		* @return None
        */
		void HandleListBoxEventL( CEikListBox* aListBox, TListBoxEvent aEventType );

        /**
        * From the base class.
		* Called by framework before the dialog is shown.
        * @param None
		* @return None
        */
        void PreLayoutDynInitL();

        /**
        * From the base class.
		* Called by framework before exiting the dialog.
        * @param aButtonId Button id.
		* @return ETrue to exit\ EFalse to not to exit.
        */
        TBool OkToExitL( TInt aButtonId );

        /**
        * From the base class.
		* Called by framework for key event handling.
        * @param aKeyEvent.
		* @param aType.
		* @return Return code.
        */
		TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );

        /**
        * From the base class.
		* Called by framework when dialog is activated.
        * @param None.
		* @return None.
        */
		void ActivateL();

		/**
        * From the base class.
		* Called by framework before menu is shown.
        * @param aResourceId Menu resource id.
		* @param aMenuPane Pointer to the menu.
        * @return None.
        */
		void DynInitMenuPaneL( TInt aResourceID, CEikMenuPane* aMenuPane );

		/**
		* From the base class.
		* Handles menu events.
		* @param  aCommandId Command id.
		* @return None.
		*/
		void ProcessCommandL( TInt aCommandId );
		
		/**
		* From base class.
		* Called when UI layout changes. 
		* @param aType.
    	* @return None.
		*/
        void HandleResourceChange( TInt aType );
        
        /** SetSizeAndPosition
        * From CAknDialog.
        * Sets dialog's size to whole main pane.
        * @param aSize ignored
        */
        void SetSizeAndPosition( const TSize &aSize );
        
        SEikControlInfo CreateCustomControlL( TInt aControlType );
        
        virtual CCoeControl* ComponentControl( TInt aIndex ) const;
        virtual TInt CountComponentControls() const;
        

    private:
		TBool DoHandleOKL( TInt aAttr );
		TBool HandleOKL( TInt aAttr );
		TBool DoHandleBackL();
		TBool HandleBackL();
        void SearchL();
        void DoSearchL();
        static TInt StartRomScaning( TAny* aInstance );
        void StartRomScan();

    private:
        void ResetDialogL( TInt aError);
		void QueryL( TInt aFolderId, TBool aDatabaseChanged = EFalse );
		void GetSelectedItemFileName( TInt aListboxIndex, TDes& aFileName );
		TInt CurrentItemListIndex();
		void SetIconsL();
		CAknSearchField* FindBox() const;
		void ResetFindBoxL();
		void FindBoxCursorMoveL( TInt aType );
		void ShowContextMenuL( TInt aResource );
		void PreviewL( TInt aMediaType );
		void PreviewVideoL( const TDesC& aFileName );
		void PreviewNullItemL( TInt aFolderId );
		void StartPreviewWithDelay();
		void CancelPreview();
		void StartSearchWithDelay();
		TBool IsVideoFile( TInt aCurrentIndex );
		TBool IsValidVideoFileL( TInt aCurrentIndex, TInt aIntention );
		TInt MediaTypeIcon( TInt aListboxIndex );
		TInt StorageTypeIcon( TInt aListboxIndex );
		void GetListItemText( TInt aListboxIndex, TDes& aText );
		
    private:
		void CheckListboxIndex();
		void HandleMediaFileChanged();
		
    private:
		void UpdateListBoxL( TBool aRestFindBox = ETrue );
		void DoUpdateListBoxL( TBool aRestFindBox );
		void UpdateCbaL( TInt aAttr );
    	TInt MapAttribute( TInt aFolderId );
		
		void ShowErrorNoteL( TInt aError );
    	void ShowPreviewErrorNoteL( TInt aError, TInt aMediaType );
		void DeleteMenuItem( CEikMenuPane* aMenuPane, TInt aCommandId );
		CGulIcon* IconL(TAknsItemID aId, const TDesC& aFileName, TInt aFileIndex,
			TInt aFileMaskIndex);
		CGulIcon* IconL(TAknsItemID aId, const TDesC& aFileName, TInt aFileIndex,
			TInt aFileMaskIndex, TAknsQsnIconColorsIndex colorIndex);
		static TInt CloseDialog( TAny *aObj );
		void CloseDialogWithDelayL();
		
    private:
        void ShowWaitNoteL( TBool aDatabaseChanged );
		void ShowPopupNoteL( const TDesC& aText );
		void ShowDetailsPopupL();
		void GetPopupText( CMediaFileData* aData, TDes& aText );
		TBool MatchFound( const TDesC& aAttribute, const TDesC& aText );


    private:
		static void Panic( TInt aReason );

    private:  // from CMFListHandlerObserver
        TPtrC ConstructListboxItem( TInt aListboxIndex );
        TInt ListboxItemCount();
        
    private:   // from MPreviewHandlerObserver
        void HandlePreviewEventL( TInt aEvent, TInt aError );

    private:   // from MMediaFileHandlerObserver
        void HandleMFEventL( TInt aEvent, TInt aError );
        
    private:   // from MMFActiveCallerObserver
        void HandleActiveCallL( TInt aCallId );
        
    private:  //from MCoeControlObserver
        void HandleControlEventL( CCoeControl* aControl, TCoeEvent aEventType );
        
    private:  // from MCoeForegroundObserver
        void HandleGainingForeground();
        void HandleLosingForeground();
        
    private:  // from MAdaptiveSearchTextObserver
        void AdaptiveSearchTextChanged( CAknSearchField* aSearchField );
        
    private:  // from MPenUiActivationHandler
        void OnPeninputUiDeactivated();
        void OnPeninputUiActivated();
        
    private:  // MMediaFileWaitNoteObserver
        void HandleWaitNoteL( TInt aButtonId );
    
#ifdef _DEBUG
    private: // test code
        CMFDialogTest* iTest;
#endif
   
    private:
		// listbox
		CAknSingleGraphicStyleListBox* iListBox;

		// for title and icon handling
		CStatusPaneHandler* iStatusPaneHandler;

		// long buffer for string handling
		TBuf<KBufSize256> iBuf;
		
		// long buffer for string handling
		TBuf<KBufSize256> iPopupText;
		
		// long buffers for listbox items
		TBuf<KBufSize128> iListboxItemText1;
		TBuf<KBufSize256> iListboxItemText2;
		
		// text from search field
		TBuf<KBufSize128> iSearchText;
	
		// long buffers for icon file names
		TFileName iIconFileName;
		
		// list box model item array
		CMFListHandler* iMFListHandler;
		
		// dialog state
		CMediaFileDialogState* iState;
		
		// metadata engine wrapper
		CMediaFileHandler* iMediaFileHandler;
		
		// search box
		CAknSearchField* iFindCtrl;
		
		// for generating active object calls
		CMFActiveCaller* iCaller;
		CMFActiveCaller* iSearchCaller;
		
		// for previewing video files
		CMFVideoPreviewHandler* iVideoPreviewHandler;
		
		// for previewing audio files
		CMFAudioPreviewHandler* iAudioPreviewHandler;
        
		// popup note
        CAknInfoPopupNoteController* iPopupNote;

		// cancel note in case database query takes more than 2 seconds
		CMediaFileWaitNote* iWaitNote;
	
		// needed for finding out whether search field is in pen input mode
		RPeninputServer iPeninputServer;
		
		// for file protection information (drm etc)
		CMFProtectionHandler* iProtectionHandler;
		
		// for closing dialog
		CIdle* iIdle;
		
		TBool ifilevalid;
        // is dialog in foreground (visible)
        TBool iForeGround;
        
        // dll resource file id
        TInt iResourceFileId;
        
        // is metadata databse open
        TBool iDatabaseOpen;
        
        // is pen input active
        TBool iPeninputActive;

        // dialog attribute values
        HBufC* iNullItemFileNameOne;
        HBufC* iNullItemFileNameTwo;
        TInt iNullItemFileTypeOne;
        TInt iNullItemFileTypeTwo;
        TBuf<KBufSize128> iDialogTitle;
        
        CAknInputBlock* iInputBlock;
		
		// dialog output parameters
		
		// selected media file
	    TDes* iFileName;
	    // selected null item
	    TInt*iNullItem;
	    // extra parameter for later use
	    TInt* iInfo;
	    // extra parameter for later use
	    TDes* iDesInfo;
	    //is double clicked the item of the listbox
	    TBool iIsDoubleClicked;
	    TBool iFileisvalid;
	    // Video preview dialogue, model dialog
	    CVideoPreviewDialog* iVideoPreviewDlg;
    };



/**
* CFindControl
*
* Search field functionality (CFindControl) is copied from
* AknSelectionLis (s60/mw/classicui/uifw/AvKon/src/AknSelectionList.cpp).
* Example code can also be found in 
* s60/app/imppapplications/wvuing/wvuiave/AppSrc/CCAContactSelectionDialog.cpp
*/
NONSHARABLE_CLASS (CFindControl) : public CCoeControl 
    {
public:
    CAknSearchField *iFind;
    ~CFindControl() { delete iFind; }
    void Draw(const TRect &) const
        {
        // this removes flicker from CEikDialog's Draw().
        }
    void SizeChanged()
        {
        TRect mainPane;
        AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EMainPane, mainPane );
        
        TAknLayoutRect listRect;
    
        listRect.LayoutRect( mainPane, AknLayout::list_gen_pane(0) );

        TRect r( listRect.Rect() );
        iPosition = TPoint( r.iTl );
        iSize.SetSize( r.Width(), r.Height() );
        }

    TSize MinimumSize()
        {
        if ( iFind )
            {
            if ( iFind->Size() != TSize( 0, 0 ) )
                {
                return iFind->Size();
                }
            }
            
        return iSize;
        }
    };




#endif      // MEDIALFILEDIALOG_H
            
// End of File
