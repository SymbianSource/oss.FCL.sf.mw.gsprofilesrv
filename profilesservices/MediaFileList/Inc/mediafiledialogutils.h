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
* Description:   Basic functionality of MediaFileList.
*
*/




#ifndef MEDIALFILEDIALOGUTILS_H
#define MEDIALFILEDIALOGUTILS_H


//  INCLUDES

#include <e32base.h>
#include <s32strm.h>  // RReadStream RWriteStream
#include <f32file.h>  // RFs

#include <eikspane.h>   // CEikStatusPane
#include <aknnavi.h>    // CAknNavigationControlContainer
#include <eikimage.h>   // CEikImage

#include <downloadedcontenthandler.h> // MDownloadedContentHandler


// CONSTANTS

const TInt KBufSize64  = 64;
const TInt KBufSize128 = 128;
const TInt KBufSize256 = 256;
const TInt KBufSize512 = 512;


//	FORWARD DECLARATIONS

class CAknAppUi;
class CRepository;

class CDocumentHandler;
class CBrowserLauncher;



/**
* TMFDialogUtil
*
* TMFDialogUtil is utility class for media file dialog.
*/
NONSHARABLE_CLASS (TMFDialogUtil)
    {
    public:

    enum TMFDialogPrivateAttribute
        {
        EAttrFileName = 100,
        EAttrDrawingWindow
        };
    
        
    public:
        static void ShowInformationQueryL( const TDesC& aText );
        static void ShowInformationQueryL( TInt aResource );
        static void ShowInformationNoteL( const TDesC& aText );
        static void ShowInformationNoteL( TInt aResource );

        static void ShowErrorNoteL( const TDesC& aText );
        static void ShowErrorNoteL( TInt aResource );
        static void ShowErrorNoteL( const TDesC& aText, TInt aError );
        static TBool ShowNumberQueryL( TInt& aNumber, const TDesC& aTitle );
        
        static TBool ShowListQueryL( const TDesC& aTitle, CDesCArray* aList,
                                     TInt* aSelectedIndex );

        
        static TInt AddResFileL();
        static void StrCopy( TDes& aTarget, const TDesC& aSource );
        static void Panic( TInt aReason );
        static void SetIconFileNameL( const TDesC& aName, TDes& aFullName );
        static CEikImage* CreateImageLC( TAknsItemID aId, const TDesC& aFileName,
                                         TInt aFileBitmapId, TInt aFileMaskId );
        static TBool IsEmpty( const TDesC& aText );
        
    };


/**
* MMFActiveCallerObserver
*
* MMFActiveCallerObserver is for observing CMFActiveCaller.
*/
NONSHARABLE_CLASS (MMFActiveCallerObserver)
    {
    public:
	    virtual void HandleActiveCallL( TInt aCallId ) = 0;
    };




/**
* CMFActiveCaller
*
* CMFActiveCaller is used for generating a call from active scheduler.
* Typical use is to start some operation after a short delay.
*/
NONSHARABLE_CLASS (CMFActiveCaller) : public CActive
    {
    public:
        static CMFActiveCaller* NewL( MMFActiveCallerObserver* aObserver );
        virtual ~CMFActiveCaller();

    private:
	    CMFActiveCaller( MMFActiveCallerObserver* aObserver );
	    void ConstructL();

	public:
		void Start( TInt aCallId, TInt aMilliseconds );
		void Stop();
		void Request();

    private:
	    void RunL();
	    void DoCancel();
        
    private:
		// call that is passed to observer
		TInt iCallId;

		// timer
		RTimer iTimer;

		// observer that gets called
		MMFActiveCallerObserver* iObserver;
    };



/**
* CNaviPaneHandler
* 
* This class is used for changing navipane title.
*/
NONSHARABLE_CLASS (CNaviPaneHandler) : public CBase
    {
    public:
        /**
        * Destructor.
        */
        virtual ~CNaviPaneHandler();

        /**
        * C++ default constructor.
        */
        CNaviPaneHandler(CEikStatusPane* aStatusPane);

    public:

        /**
        * Sets navi pane title.
        * @param aTitle.
        * @return None.
        */
        void SetNaviPaneTitleL(const TDesC& aTitle);
        
        /**
        * Store navi pane.
        * @param None.
        * @return None.
        */
        void StoreNavipaneL();

    private:
        // status pane
        CEikStatusPane* iStatusPane;
    
        // navi Pane
        CAknNavigationControlContainer* iNaviPane;
        
        // navi decorator
        CAknNavigationDecorator* iNaviDecorator;
        
        // has Navidecorator been pushed into navi pane
        TBool iNavidecoratorPushed;
        
        // has navi pane been pushed into navigation pane's object stack
        TBool iNavipanePushed;
    };



/**
* CStatusPaneHandler
* 
* This class is used for changing dialog title and image.
*/
NONSHARABLE_CLASS (CStatusPaneHandler) : public CBase
    {
    public:
        /**
        * Two-phased constructor.
        */
        static CStatusPaneHandler* NewL( CAknAppUi* aAppUi );

        /**
        * Destructor.
        */
        virtual ~CStatusPaneHandler();

    private:
        /**
        * By default Symbian OS constructor is private.
        */
        void ConstructL();

        /**
        * C++ default constructor.
        */
        CStatusPaneHandler( CAknAppUi* aAppUi );

    public:
        /**
        * Stores original title so it can be restored when dialog closes.
        * @param  None.
        * @return None.
        */
        void StoreOriginalTitleL();

        /**
        * Restores original title.
        * @param  None.
        * @return None.
        */
        void RestoreOriginalTitleL();

        /**
        * Sets dialog title.
        * @param  aText.
        * @return None.
        */
        void SetTitleL( const TDesC& aText );

        /**
        * Sets dialog title.
        * @param  aResourceId.
        * @return None.
        */
        void SetTitleL( TInt aResourceId );

    public:
        /**
        * Sets dialog title.
        * @param aAppUi.
        * @param aText.
        * @return Return code.
        */
        static TBool SetTitleL( CAknAppUi* aAppUi, const TDesC& aText );

        /**
        * Gets dialog title.
        * @param aAppUi.
        * @param aText.
        * @return Return code.
        */
        static TBool GetTitleL( CAknAppUi* aAppUi, TDes& aText );
        
        /**
        * Sets navi pane title.
        * @param aTitle.
        * @return None.
        */
        void SetNaviPaneTitleL( const TDesC& aTitle );
        
        /**
        * Sets navi pane title.
        * @param aTitle.
        * @return None.
        */
        void SetNaviPaneTitle( const TDesC& aTitle );
        
        /**
        * Sets navi pane title.
        * @param aTitle.
        * @return None.
        */
        void SetNaviPaneTitleL( TInt aResource );
        
        /**
        * Store navi pane.
        * @param None.
        * @return None.
        */
        void StoreNavipaneL();

    private:
        // access to app ui
        CAknAppUi* iAppUi;

        // original status pane title
        TBuf<KBufSize256> iOriginalTitle;
        
        // is original status pane title stored
        TBool iOriginalTitleStored;

        // original context pane image
        CEikImage* iOriginalImage;
        
        CNaviPaneHandler* iNaviPaneHandler;
    };




/**
* CDriveUtil
*
* CDriveUtil is used for getting drive information.
*/
NONSHARABLE_CLASS (CDriveUtil) : public CBase
    {
    public:
        static CDriveUtil* NewL();
        virtual ~CDriveUtil();

    private:
        CDriveUtil();
        void ConstructL();

    public:
        TBool IsMemoryCard( const TDesC& aFullPath );
        TBool IsMassStorage( const TDesC& aFullPath );
        TBool IsRom( const TDesC& aFullPath );
        
    private:
        RFs iFsSession;
    };


/**
* CMimeTypeList
*
* CMimeTypeList is used for storing mime types that 
* are excluded from media file list.
*/
NONSHARABLE_CLASS (CMimeTypeList) : public CBase
    {
    public:
        static CMimeTypeList* NewL();
        virtual ~CMimeTypeList();

    private:
        CMimeTypeList();
        void ConstructL();

    public:
        void ResetL();
        TInt Count();
        const TDesC& MimeType( TInt aIndex );
        void AddMimeTypeL( const TDesC& aText );
        TBool FindMimeTypeL( const TDesC& aText );
        
       
    public:
        static TInt Compare( const TFileName& aFirst, const TFileName& aSecond );        
    
    private:
        RArray<TFileName> iList;
    };


/**
* CWebBrowserLauncher
*
* CWebBrowserLauncher is used for launching web browser.
*/
NONSHARABLE_CLASS(CWebBrowserLauncher) : public CBase,
                                         public MDownloadedContentHandler
    {
    public:
        static CWebBrowserLauncher* NewLC();
        virtual ~CWebBrowserLauncher();

    private:
        CWebBrowserLauncher();
        void ConstructL();

    public:
        void LaunchBrowserL();

    public:
        TBool HandleContentL( const TDesC& aFileName,
                              const CAiwGenericParamList& aParamList,
                              TBool& aContinue );
        
        TAny* DownloadedContentHandlerReserved1( TAny* aAnyParam );


    private:
        // Browser Launcher
        CBrowserLauncher* iBrowserLauncher;

        // Document Handler for saving downloaded tones.
        CDocumentHandler* iDocumentHandler;
    };





#endif      // MEDIALFILEDIALOGUTILS_H


            
// End of File
